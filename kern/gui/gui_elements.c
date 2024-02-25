/**
 * @author Andrej Bartulin, Stjepan Bilić Matišić
 * PROJECT: Pelly
 * LICENSE: MIT
 * DESCRIPTION: Main C file for GUI elements
*/

#include "gui.h"

Button init_button(Window window, uint16_t x, uint16_t y, uint16_t len_x, uint16_t len_y, 
                    uint16_t background_color, uint16_t foreground_color, char* title)
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

    return new;
}

void check_button(Window* window, int id)
{
    Button button = window->elements.button[id];

    uint16_t max_x = button.x + button.len_x;
    uint16_t max_y = button.y + button.len_y;

    if (strlen(button.title) > max_x - 2)
    {
        window->elements.button[id].error = TITLE_BOUNDARY_EXCEEDED;
        return;
    }

    if (button.x > (window->x + window->len_x) - 1)
    {
        window->elements.button[id].error = X_BOUNDARY_EXCEEDED;
        return;
    }

    if (button.y > (window->x + window->len_x) - 1)
    {
        window->elements.button[id].error = Y_BOUNDARY_EXCEEDED;
        return;
    }

    if (max_x >= (window->x + window->len_x) - 1)
    {
        window->elements.button[id].error = LEN_X_BOUNDARY_EXCEEDED;
        return;
    }

    if (max_y >= (window->y + window->len_y) - 1)
    {
        window->elements.button[id].error = LEN_Y_BOUNDARY_EXCEEDED;
        return;
    }

    /* Check if button interferes with other buttons */
    for (int i = id; i >= 0; i--)
    {
        if (id == 0)
        {
            /* Button passed checks above */
            if (button.error == NO_CHECK)
            {
                window->elements.button[id].error = NO_ERROR;
                break;
            }

            /* Return button with error above */
            break;
        }

        /* Button already has an error, we don't need to go deeper */
        if (window->elements.button[id].error != NO_CHECK)
        {
            break;
        }

        int previous_button = i - 1;

        if (window->elements.button[previous_button].error == NO_ERROR)
        {
            if (button.x +  button.len_x < 
                            window->elements.button[previous_button].x ||
                button.x >  window->elements.button[previous_button].x + 
                            window->elements.button[previous_button].len_x)
            {
                if (button.y +  button.len_y < 
                                window->elements.button[previous_button].y ||
                    button.y >  window->elements.button[previous_button].y + 
                                window->elements.button[previous_button].len_y)
                {
                    window->elements.button[id].error = NO_ERROR;
                }

                else
                {
                    window->elements.button[id].error = Y_IN_WINDOW;
                }
            }

            else
            {
                window->elements.button[id].error = X_IN_WINDOW;
            }
        }
    }
}

void draw_button(Window window, Button button)
{
    /* ******************************** */
    /*          Top border              */
    /* ******************************** */

    move_cursor(window.x + button.x, window.y + button.y);
    
    for (uint16_t i = 0; i < button.len_x; i++)
    {
        printf("%c", 0xC4);
    }

    /* ******************************** */
    /*          Right border            */
    /* ******************************** */

    for (uint16_t i = 1; i < button.len_y; i++)
    {
        move_cursor(window.x + (button.x + button.len_x), window.y + (button.y + i));
        printf("%c", 0xB3);
    }

    /* ******************************** */
    /*          Bottom border           */
    /* ******************************** */

    move_cursor(window.x + button.x, window.y + (button.y + button.len_y));

    for (uint16_t i = 0; i < button.len_x; i++)
    {
        printf("%c", 0xC4);
    }

    /* ******************************** */
    /*          Left border             */
    /* ******************************** */

    move_cursor(window.x + button.x, window.y + button.y);

    for (uint16_t i = 1; i < button.len_y; i++)
    {
        move_cursor((window.x + button.x) - 1, window.y + (button.y + i));
        printf("%c", 0xB3);
    }

    /* ******************************** */
    /*          Title                   */
    /* ******************************** */

    move_cursor(window.x + (button.x + 1), window.y + (button.y + 1));
    printf("%s", button.title);
}

/* ------------------------------------ */
/* ------------------------------------ */
/* ------------------------------------ */

ContextMenu init_context_menu(Window window, uint16_t x, uint16_t y, uint16_t len_x, uint16_t len_y, 
                    uint16_t background_color, uint16_t foreground_color, ContextButton buttons[NUMBER_OF_BUTTONS])
{
    ContextMenu new;
    new.error = NO_ERROR;

    new.x = x;
    new.y = y;

    new.len_x = len_x;
    new.len_y = len_y;
    
    new.background_color = background_color;
    new.foreground_color = foreground_color;

    for (int i = 0; i < NUMBER_OF_BUTTONS; i++)
    {
        new.buttons[i] = buttons[i];
    }

    return new;
}

void check_context_menu(Window window, ContextMenu* context_menu)
{
    uint16_t max_x = context_menu->x + context_menu->len_x;
    uint16_t max_y = context_menu->y + context_menu->len_y;

    if (context_menu->x < window.x || context_menu->x > max_x)
    {
        context_menu->error = X_BOUNDARY_EXCEEDED;
        return;
    }

    if (context_menu->y < window.y || context_menu->y > max_y)
    {
        context_menu->error = Y_BOUNDARY_EXCEEDED;
        return;
    }

    if (max_x >= (window.x + window.len_x))
    {
        context_menu->error = LEN_X_BOUNDARY_EXCEEDED;
        return;
    }

    if (max_y >= (window.y + window.len_y))
    {
        context_menu->error = LEN_Y_BOUNDARY_EXCEEDED;
        return;
    }
}

void draw_context_menu(Window window, ContextMenu context_menu)
{
    
    /* ******************************** */
    /*          Top border              */
    /* ******************************** */

    move_cursor(window.x + context_menu.x, window.y + context_menu.y);
    
    for (uint16_t i = 0; i < context_menu.len_x; i++)
    {
        printf("%c", 0xC4);
    }

    /* ******************************** */
    /*          Right border            */
    /* ******************************** */

    for (uint16_t i = 1; i < context_menu.len_y; i++)
    {
        move_cursor(window.x + (context_menu.x + context_menu.len_x), window.y + (context_menu.y + i));
        printf("%c", 0xB3);
    }

    /* ******************************** */
    /*          Bottom border           */
    /* ******************************** */

    move_cursor(window.x + context_menu.x, window.y + (context_menu.y + context_menu.len_y));

    for (uint16_t i = 0; i < context_menu.len_x; i++)
    {
        printf("%c", 0xC4);
    }

    /* ******************************** */
    /*          Left border             */
    /* ******************************** */

    move_cursor(window.x + context_menu.x, window.y + context_menu.y);

    for (uint16_t i = 1; i < context_menu.len_y; i++)
    {
        move_cursor((window.x + context_menu.x) - 1, window.y + (context_menu.y + i));
        printf("%c", 0xB3);
    }

    /* ******************************** */
    /*          Buttons                 */
    /* ******************************** */

    for (int i = 0; i < NUMBER_OF_BUTTONS; i++)
    {
        if (context_menu.buttons[i].error == EMPTY)
        {
            if (window.debug)
            {
                printf("Empty button! Skipping...\r\n");
            }

            continue;
        }

        else
        {
            if ((window.y + (context_menu.y + i + (i + 2)) > ((window.y + window.len_y) - 2)))
            {
                if (window.debug)
                {
                    printf("No space left! Skipping...\r\n");
                }

                break;
            }

            move_cursor(window.x + context_menu.x, window.y + (context_menu.y + i + (i + 2)));

            for (uint16_t i = 0; i < context_menu.len_x; i++)
            {
                printf("%c", 0xC4);
            }

            move_cursor(window.x + context_menu.x, window.y + (context_menu.y + i + (i + 1)));
            printf("%s", context_menu.buttons[i].title);
        }
    }
}
