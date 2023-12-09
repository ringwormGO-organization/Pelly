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

Button context_button = {
    .x = 2,
    .y = 2,
    .len_x = 5,
    .len_y = 2,
    .background_color = LGRAY,
    .foreground_color = WHITE,
    .title = "123",
};

Window init_window(uint16_t x, uint16_t y, uint16_t len_x, uint16_t len_y, 
                    uint16_t background_color, uint16_t foreground_color, char* title, bool debug)
{
    Window new;
    new.error = NO_ERROR;

    /* Perform checks */
    if (strlen(title) > 70 || strlen(title) > (len_x - 2))
    {
        new.error = TITLE_BOUNDARY_EXCEEDED;
        return new;
    }

    if (x > 79)
    {
        new.error = X_BOUNDARY_EXCEEDED;
        return new;
    }

    if (y > 24)
    {
        new.error = Y_BOUNDARY_EXCEEDED;
        return new;
    }

    if (len_x >= (79 - x))
    {
        new.error = LEN_X_BOUNDARY_EXCEEDED;
        return new;
    }

    if (len_y >= (24 - y))
    {
        new.error = LEN_Y_BOUNDARY_EXCEEDED;
        return new;
    }

    new.x = x;
    new.y = y;

    new.len_x = len_x;
    new.len_y = len_y;
    
    new.background_color = background_color;
    new.foreground_color = foreground_color;
    
    new.title = title;

    if (debug)
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

void draw_window_elements(Window window, Elements elements, bool debug)
{
    window.elements = elements;

    /* ******************************** */
    /*          Buttons                 */
    /* ******************************** */

    for (int i = 0; i < BUTTON_SIZE; i++)
    {
        if (window.elements.button[i].error == EMPTY)
        {
            if (debug)
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

        if (window.elements.button[i].error != 0)
        {
            printf("Error code during drawing process of a button %d: %d\r\n", i, window.elements.button[i].error);
            continue;
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

    if (test_window.error != NO_ERROR)
    {
        printf("Error code: %d\r\n", test_window.error);
        goto keyboard_loop;
    }

    draw_window(test_window);
    move_cursor(0, 0);

    Button context_buttons[BUTTON_SIZE];
    context_buttons[0] = context_button;

    context_buttons[1] = empty_button;
    context_buttons[2] = empty_button;
    context_buttons[3] = empty_button;

    ContextMenu context_menu = init_context_menu(test_window, 10, 10, 5, 5, LGRAY, WHITE, context_buttons);

    Elements test_elements;
    test_elements.button[0] = main_button;

    test_elements.button[1] = empty_button;
    test_elements.button[2] = empty_button;
    test_elements.button[3] = empty_button;

    test_elements.context_menu = context_menu;
    draw_window_elements(test_window, test_elements, false);

keyboard_loop:
    move_cursor(0, 0);
    c_keyboard_loop(test_window.x + 1, test_window.y, test_window);
}
