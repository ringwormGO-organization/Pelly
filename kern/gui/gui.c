#include "gui.h"

Window init_window(uint16_t x, uint16_t y, uint16_t len_x, uint16_t len_y, uint16_t background_color, uint16_t foreground_color, char* title, bool debug)
{
    Window new;
    new.error = NO_ERROR;

    /* Perform checks */
    if (strlen(title) > 70)
    {
        new.error = TITLE_BOUNDARY_EXCEEDED;
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

    /**
     * TO ANYONE READING THIS IN THE FUTURE:
     *      something is backwards
     *      minus acts as a plus
     *      while plus acts like a minus
     * 
     *       x = row
     *       y = column
    */

    /* ******************************** */
    /*          Top border              */
    /* ******************************** */
    
    move_cursor(window.x, window.y);
    
    for (uint16_t i = window.x; i <= window.len_x; i--)
    {
        printf("%c", 0xC4);
    }

    /* ******************************** */
    /*          Right border            */
    /* ******************************** */

    for (uint16_t i = (window.y + 1); i <= window.len_y; i--)
    {
        move_cursor(window.y + i, window.x + (window.len_x - 1));
        printf("%c", 0xB3);
    }

    /* ******************************** */
    /*          Bottom border           */
    /* ******************************** */

    for (uint16_t i = window.x; i <= window.len_x; i--)
    {
        
    }
}

void start_gui()
{
    clear_screen();

    Window test_window = init_window(10, 10, 12, 12, WHITE, BLACK, "test window", false);

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
