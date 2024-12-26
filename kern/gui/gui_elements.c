/**
 * @author Andrej Bartulin, Stjepan Bilić Matišić
 * PROJECT: Pelly
 * LICENSE: MIT
 * DESCRIPTION: Main C file for GUI elements
*/

#include "gui.h"

Button create_button(uint16_t x, uint16_t y, uint16_t len_x, uint16_t len_y, 
                    uint16_t background_color, uint16_t foreground_color, char* title, void (*action)())
{
    Button new;
    new.error = NO_CHECK;

    new.x = x;
    new.y = y;

    new.len_x = len_x;
    new.len_y = len_y;
    
    new.background_color = background_color;
    new.foreground_color = foreground_color;
    
    new.title = title;
    new.action = action;

    return new;
}

void check_button(Window window, Button* button)
{
    uint16_t max_x = button->x + button->len_x;
    uint16_t max_y = button->y + button->len_y;

    if (button->error == EMPTY)
    {
        return;
    }

    if (strlen(button->title) > max_x - 2)
    {
        button->error = TITLE_BOUNDARY_EXCEEDED;
    }

    if (button->x > (window.x + window.len_x) - 1)
    {
        button->error = X_BOUNDARY_EXCEEDED;
    }

    if (button->y > (window.x + window.len_x) - 1)
    {
        button->error = Y_BOUNDARY_EXCEEDED;
    }

    if (max_x >= (window.x + window.len_x) - 1)
    {
        button->error = LEN_X_BOUNDARY_EXCEEDED;
    }

    if (max_y >= (window.y + window.len_y) - 1)
    {
        button->error = LEN_Y_BOUNDARY_EXCEEDED;
    }

    button->error = NO_ERROR;
}

void draw_button(Window window, Button button)
{
    if (button.background_color != BLUE)
    {
        move_real_cursor(window.x + button.x, window.y + button.y);
        for (uint16_t i = 1; i < button.len_y; i++)
        {
            move_real_cursor(window.x + button.x, window.y + button.y + i);
            for (uint16_t j = 0; j < button.len_x; j++)
            {
                change_color(format_color(button.background_color, button.background_color), (uint16_t)1);
                printf("%c", 0x20);
            }
        }
    }

    /* ******************************** */
    /*          Top border              */
    /* ******************************** */

    move_real_cursor(window.x + button.x, window.y + button.y);
    
    for (uint16_t i = 0; i < button.len_x; i++)
    {
        printf("%c", 0xC4);
    }

    /* ******************************** */
    /*          Right border            */
    /* ******************************** */

    for (uint16_t i = 1; i < button.len_y; i++)
    {
        move_real_cursor(window.x + (button.x + button.len_x), window.y + (button.y + i));
        printf("%c", 0xB3);
    }

    /* ******************************** */
    /*          Bottom border           */
    /* ******************************** */

    move_real_cursor(window.x + button.x, window.y + (button.y + button.len_y));

    for (uint16_t i = 0; i < button.len_x; i++)
    {
        printf("%c", 0xC4);
    }

    /* ******************************** */
    /*          Left border             */
    /* ******************************** */

    move_real_cursor(window.x + button.x, window.y + button.y);

    for (uint16_t i = 1; i < button.len_y; i++)
    {
        move_real_cursor((window.x + button.x) - 1, window.y + (button.y + i));
        printf("%c", 0xB3);
    }

    /* ******************************** */
    /*          Title                   */
    /* ******************************** */

    if (button.background_color != BLUE)
    {
        change_color(format_color(button.background_color, button.foreground_color), (uint16_t)2);
    }

    move_real_cursor(window.x + (button.x + 1), window.y + (button.y + 1));
    printf("%s", button.title);
}

