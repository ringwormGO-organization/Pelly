#include "gui.h"

Cursor init_cursor(uint16_t x, uint16_t y)
{
    Cursor new;

    /* Perform checks */
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

    new.error = NO_ERROR;

    new.x = x;
    new.y = y;

    init_keyboard(new.x, new.y);
    return new;
}

Cursor move_cursor(uint16_t x, uint16_t y)
{
    Cursor new;

    /* Perform checks */
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

    new.error = NO_ERROR;

    new.x = x;
    new.y = y;

    move_keyboard(new.x, new.y);
    return new;
}

Window init_window(uint16_t x, uint16_t y, uint16_t len_x, uint16_t len_y, uint16_t background_color, uint16_t foreground_color, char* title, bool debug)
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

    if (y > 79)
    {
        new.error = Y_BOUNDARY_EXCEEDED;
        return new;
    }

    if (len_x >= (79 - x))
    {
        new.error = LEN_X_BOUNDARY_EXCEEDED;
        return new;
    }

    if (len_y >= (79 - x))
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
        printf("X = %d : Y = %d | LEN(X) = %d : LEN(Y) = %d | BG = %d : FG = %d | TITLE = %s \n",
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

    global_cursor = move_cursor(window.x, window.y);
    
    for (uint16_t i = 0; i < window.len_x; i++)
    {
        printf("%c", 0xC4);
    }

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

    global_cursor = move_cursor(window.x, window.y + window.len_y);

    for (uint16_t i = 0; i < window.len_x + 1; i++)
    {
        printf("%c", 0xC4);
    }

    /* ******************************** */
    /*          Left border             */
    /* ******************************** */

    global_cursor = move_cursor(window.x, window.y);

    for (uint16_t i = 0; i < window.len_y; i++)
    {
        global_cursor = move_cursor(window.x, window.y + i);
        printf("%c", 0xB3);
    }

    /* ------------------------------------------------------------------------ */

    /* ******************************** */
    /*          Title space             */
    /* ******************************** */

    global_cursor = move_cursor(window.x + 1, window.y + 2);
    
    for (uint16_t i = 0; i < window.len_x - 1; i++)
    {
        printf("%c", 0xC4);
    }

    /* ******************************** */
    /*          Title                   */
    /* ******************************** */

    global_cursor = move_cursor(window.x + 1, window.y + 1);
    printf("%s", window.title);
}

// 0x0a - \n
// 0x0d - \r

void start_gui()
{
    clear_screen();

    Window test_window = init_window(5, 5, 15, 15, WHITE, BLACK, "test window", false);

    if (test_window.error != NO_ERROR)
    {
        printf("Error code: %d\n", test_window.error);
    }

    else
    {
        draw_window(test_window);
    }

    c_keyboard_loop();
}
