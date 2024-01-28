#include "gui.h"

// NOTE: use `init` function
// this is just to speed up development process

Button empty_button = {
    .error = EMPTY,
};

Button main_button = {
    .x = 5,
    .y = 5,
    .len_x = 7,
    .len_y = 2,
    .background_color = BLACK,
    .foreground_color = YELLOW,
    .title = "test",
};

ContextMenu empty_context_menu = {
    .error = EMPTY,
};

ContextButton context_button = {
    .len_y = 2,
    .title = "123",
};

Window init_window(uint16_t x, uint16_t y, uint16_t len_x, uint16_t len_y, 
                    uint16_t background_color, uint16_t foreground_color, char* title, bool debug)
{
    Window new;

    new.debug = debug;
    new.error = NO_ERROR;

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

void check_window(Window window)
{
    if (strlen(window.title) > 70 || strlen(window.title) > (window.len_x - 2))
    {
        window.error = TITLE_BOUNDARY_EXCEEDED;
    }

    if (window.x > 79)
    {
        window.error = X_BOUNDARY_EXCEEDED;
    }

    if (window.y > 24)
    {
        window.error = Y_BOUNDARY_EXCEEDED;
    }

    if (window.len_x >= (79 - window.x))
    {
        window.error = LEN_X_BOUNDARY_EXCEEDED;
    }

    if (window.len_y >= (24 - window.y))
    {
        window.error = LEN_Y_BOUNDARY_EXCEEDED;
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

void draw_window_elements(Window window)
{
    /* ******************************** */
    /*          Buttons                 */
    /* ******************************** */

    for (int i = 0; i < NUMBER_OF_BUTTONS; i++)
    {
        // TODO
        // perform checks

        if (window.elements.button[i].error == EMPTY)
        {
            if (window.debug)
            {
                printf("Empty button! Skipping...\r\n");
            }

            continue;
        }

        if (window.elements.button[i].error != 0)
        {
            printf("Error code during initialization of a button %d: %d\r\n", i, window.elements.button[i].error);
            continue;
        }

        draw_button(window, window.elements.button[i]);
        move_cursor(0, 0);
    }

    /* ******************************** */
    /*          Context menu            */
    /* ******************************** */

    if (window.elements.context_menu.error != EMPTY) /* not empty */
    {
        if (window.elements.context_menu.error == 0) /* no error */
        {
            draw_context_menu(window, window.elements.context_menu);
            move_cursor(0, 0);
        }

        else
        {
            printf("Error code during initialization of a context menu\r\n");
        }
    }

    else
    {
        if (window.debug)
        {
            printf("Empty context menu! Skipping...\r\n");
        }
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

    Window test_window = init_window(5, 5, 15, 15, WHITE, BLACK, "test window", false);

    check_window(test_window);
    if (test_window.error != NO_ERROR)
    {
        printf("Error code: %d\r\n", test_window.error);
        goto keyboard_loop;
    }

    draw_window(test_window);
    move_cursor(0, 0);

    test_window.elements.button[0] = main_button;
    draw_window_elements(test_window);

keyboard_loop:
    move_cursor(0, 0);
    c_keyboard_loop(test_window.x + 1, test_window.y, test_window);
}
