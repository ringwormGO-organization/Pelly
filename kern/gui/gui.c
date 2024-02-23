/**
 * @author Andrej Bartulin, Stjepan Bilić Matišić
 * PROJECT: Pelly
 * LICENSE: MIT
 * DESCRIPTION: Main C file for GUI
*/

#include "gui.h"

// NOTE: use `init` function
// this is just to speed up development process

Button empty_button = {
    .error = EMPTY,
};

Button main_button = {
    .error = NO_CHECK,
    .x = 5,
    .y = 5,
    .len_x = 7,
    .len_y = 2,
    .background_color = BLACK,
    .foreground_color = YELLOW,
    .title = "test",
};

Button invalid_button = {
    .error = NO_CHECK,
    .x = 6,
    .y = 6,
    .len_x = 2,
    .len_y = 2,
    .background_color = BLACK,
    .foreground_color = YELLOW,
    .title = "test",
};

ContextMenu context_menu = {
    .error = NO_ERROR,
    .x = 9,
    .y = 9,
    .len_x = 5,
    .len_y = 5,
    .background_color = RED,
    .foreground_color = WHITE,
};

ContextButton empty_context_button = {
    .content = "n/a",
};

ContextButton context_button = {
    .content = "one",
};

Window init_window(uint16_t x, uint16_t y, uint16_t len_x, uint16_t len_y, 
                    uint16_t background_color, uint16_t foreground_color, char* title, bool debug)
{
    Window new;

    new.debug = debug;
    new.error = NO_CHECK;

    new.x = x;
    new.y = y;

    new.len_x = len_x;
    new.len_y = len_y;
    
    new.background_color = background_color;
    new.foreground_color = foreground_color;
    
    new.title = title;
    
    /* Initializing arrays */

    for (int i = 0; i < NUMBER_OF_BUTTONS; ++i) 
    {
        new.elements.button[i].error = EMPTY;
    }

    if (new.debug)
    {
        printf("X = %d : Y = %d | LEN(X) = %d : LEN(Y) = %d | BG = %d : FG = %d | TITLE = %s \r\n",
                new.x, new.y,
                new.len_x, new.len_y, 
                new.background_color,
                new.foreground_color,
                new.title);
    }

    return new;
}

void check_window(Screen* screen, int id)
{
    Window window = screen->windows[id];

    if (window.error == EMPTY)
    {
        return;
    }

    if (strlen(window.title) > 70 || strlen(window.title) > (window.len_x - 2))
    {
        screen->windows[id].error = TITLE_BOUNDARY_EXCEEDED;
    }

    if (window.x > screen->len_x - 1)
    {
        screen->windows[id].error = X_BOUNDARY_EXCEEDED;
    }

    if (window.y > screen->len_y - 1)
    {
        screen->windows[id].error = Y_BOUNDARY_EXCEEDED;
    }

    if (window.len_x >= ((screen->len_x - 1) - window.x))
    {
        screen->windows[id].error = LEN_X_BOUNDARY_EXCEEDED;
    }

    if (window.len_y >= ((screen->len_y - 1) - window.y))
    {
        screen->windows[id].error = LEN_Y_BOUNDARY_EXCEEDED;
    }

    /* Check if window interferes with other windows */
    for (int i = id; i >= 0; i--)
    {
        if (id == 0)
        {
            /* Window passed checks above */
            if (window.error == NO_CHECK)
            {
                screen->windows[id].error = NO_ERROR;
                break;
            }

            /* Return window with error above */
            break;
        }

        /* Window already has an error, we don't need to go deeper */
        if (screen->windows[id].error != NO_CHECK)
        {
            break;
        }

        int previous_window = i - 1;

        if (screen->windows[previous_window].error == NO_ERROR)
        {
            if (window.x +  window.len_x < 
                            screen->windows[previous_window].x ||
                window.x >  screen->windows[previous_window].x + 
                            screen->windows[previous_window].len_x)
            {
                if (window.y +  window.len_y < 
                                screen->windows[previous_window].y ||
                    window.y >  screen->windows[previous_window].y + 
                                screen->windows[previous_window].len_y)
                {
                    screen->windows[id].error = NO_ERROR;
                }

                else
                {
                    screen->windows[id].error = Y_IN_WINDOW;
                }
            }

            else
            {
                screen->windows[id].error = X_IN_WINDOW;
            }
        }
    }
}

void draw_window(Window window)
{
    /* ******************************** */
    /*          Top border              */
    /* ******************************** */

    move_cursor(window.x, window.y);
    
    for (uint16_t i = 0; i < window.len_x; i++)
    {
        printf("%c", 0xC4);
    }
    
    /* ******************************** */
    /*          Close window but.       */
    /* ******************************** */

    move_cursor(window.x, window.y);
    printf(" X%c",0xB3);

    /* ******************************** */
    /*          Right border            */
    /* ******************************** */

    for (uint16_t i = 0; i < window.len_y; i++)
    {
        move_cursor(window.x + window.len_x, window.y + i);
        printf("%c", 0xB3);
    }

    /* ******************************** */
    /*          Bottom border           */
    /* ******************************** */

    move_cursor(window.x, window.y + window.len_y);

    for (uint16_t i = 0; i < window.len_x + 1; i++)
    {
        printf("%c", 0xC4);
    }

    /* ******************************** */
    /*          Left border             */
    /* ******************************** */

    move_cursor(window.x, window.y);

    for (uint16_t i = 0; i < window.len_y; i++)
    {
        move_cursor(window.x, window.y + i);
        printf("%c", 0xB3);
    }

    /* ------------------------------------------------------------------------ */

    /* ******************************** */
    /*          Title space             */
    /* ******************************** */

    move_cursor(window.x + 1, window.y + 2);
    
    for (uint16_t i = 0; i < window.len_x - 1; i++)
    {
        printf("%c", 0xC4);
    }

    /* ******************************** */
    /*          Title                   */
    /* ******************************** */

    move_cursor(window.x + 1, window.y + 1);
    printf("%s", window.title);
}

void draw_window_elements(Window window, int window_id)
{
    /* ******************************** */
    /*          Buttons                 */
    /* ******************************** */

    for (int i = 0; i < NUMBER_OF_BUTTONS; i++)
    {
        if (window.elements.button[i].error == EMPTY)
        {
            if (window.debug)
            {
                printf("Empty button! Skipping...\r\n");
            }

            continue;
        }

        check_button(&window, i);

        if (window.elements.button[i].error != 0)
        {
            printf("Error code %d of button %d, window %d\r\n", window.elements.button[i].error, i, window_id);
            continue;
        }

        draw_button(window, window.elements.button[i]);
        move_cursor(0, 0);
    }

    check_context_menu(window, &window.elements.context_menu);

    if (window.elements.context_menu.error == NO_ERROR)
    {
        draw_context_menu(window, window.elements.context_menu);
    }

    else
    {
        printf("Error code %d of context menu, window %d\r\n", window.elements.context_menu.error, window_id);
    }

}

void clear_window(Window window)
{

    /* ******************************** */
    /*          Top border              */
    /* ******************************** */

    move_cursor(window.x, window.y);
    
    for (uint16_t i = 0; i < window.len_x; i++)
    {
        printf("%c", 0x20);
    }
    
    /* ******************************** */
    /*          Close window but.       */
    /* ******************************** */

    move_cursor(window.x, window.y);
    printf("   ");

    /* ******************************** */
    /*          Right border            */
    /* ******************************** */

    for (uint16_t i = 0; i < window.len_y; i++)
    {
        move_cursor(window.x + window.len_x, window.y + i);
        printf("%c", 0x20);
    }

    /* ******************************** */
    /*          Bottom border           */
    /* ******************************** */

    move_cursor(window.x, window.y + window.len_y);

    for (uint16_t i = 0; i < window.len_x + 1; i++)
    {
        printf("%c", 0x20);
    }

    /* ******************************** */
    /*          Left border             */
    /* ******************************** */

    move_cursor(window.x, window.y);

    for (uint16_t i = 0; i < window.len_y; i++)
    {
        move_cursor(window.x, window.y + i);
        printf("%c", 0x20);
    }

    /* ------------------------------------------------------------------------ */

    /* ******************************** */
    /*          Title space             */
    /* ******************************** */

    move_cursor(window.x + 1, window.y + 2);
    
    for (uint16_t i = 0; i < window.len_x - 1; i++)
    {
        printf("%c", 0x20);
    }

    /* ******************************** */
    /*          Title                   */
    /* ******************************** */

    move_cursor(window.x + 1, window.y + 1);

    for (uint16_t i = 0; i < strlen(window.title); i++)
    {
        printf("%c", 0x20);
    }

    move_cursor(window.x, window.y);
    for (uint16_t i = 0; i < window.len_y; i++)
    {
        for (uint16_t j = 0; j < window.len_x; j++)
            printf("%c", 0x20);
        
        move_cursor(window.x, window.y+i);
    }

}

// 0x0a - \r\n
// 0x0d - \r

void start_gui()
{
    clear_screen();
    move_cursor(0, 0);

    /* ------------ */

    Screen screen;

    screen.len_x = 80;
    screen.len_y = 25;

    /* ------------ */

    Window test_window = init_window(5, 5, 15, 15, WHITE, BLACK, "test window", false);
    Window invalid_window = init_window(17, 17, 2, 2, LGRAY, LGRAY, "", false);

    screen.windows[0] = test_window;
    screen.windows[1] = invalid_window;
    screen.windows[2].error = EMPTY;
    screen.windows[3].error = EMPTY;

    for (int i = 0; i < NUMBER_OF_WINDOWS; i++)
    {
        check_window(&screen, i);

        if (screen.windows[i].error == NO_ERROR)
        {
            draw_window(screen.windows[i]);
        }

        else
        {
            if (screen.windows[i].error != EMPTY)
            {
                move_cursor(0, i);
                printf("Error code %d of window %d\r\n", screen.windows[i].error, i);
            }
        }
    }

    screen.windows[0].elements.button[0] = main_button;
    screen.windows[0].elements.button[1] = invalid_button;
    screen.windows[0].elements.button[2].error = EMPTY;
    screen.windows[0].elements.button[3].error = EMPTY;

    ContextButton context_buttons[NUMBER_OF_BUTTONS];
    context_buttons[0] = context_button;
    context_buttons[1] = empty_context_button;
    context_buttons[2] = empty_context_button;
    context_buttons[3] = empty_context_button;

    ContextMenu context_menu = init_context_menu(9, 9, 5, 5, LGRAY, LGRAY, context_buttons);

    screen.windows[0].elements.context_menu = context_menu;

    for (int i = 0; i < NUMBER_OF_WINDOWS; i++)
    {
        if (screen.windows[i].error == NO_ERROR)
        {
            draw_window_elements(screen.windows[i], i);
        }
    }

keyboard_loop:
    move_cursor(0, 0);
    c_keyboard_loop(screen);
}
