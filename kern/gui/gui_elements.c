/**
 * @author Andrej Bartulin, Stjepan Bilić Matišić
 * PROJECT: Pelly
 * LICENSE: MIT
 * DESCRIPTION: Main C file for GUI elements
*/

#include "gui.h"

Button* init_button(uint16_t x, uint16_t y, uint16_t len_x, uint16_t len_y, 
                    uint16_t background_color, uint16_t foreground_color, char* title, void (*action)())
{
    Button* new = malloc(sizeof(Button));
    new->error = NO_CHECK;

    new->x = x;
    new->y = y;

    new->len_x = len_x;
    new->len_y = len_y;
    
    new->background_color = background_color;
    new->foreground_color = foreground_color;
    
    new->title = title;
    new->action = action;

    return new;
}

void check_button(Window* window, int id)
{
    Button* button = window->elements.button.array[id];

    uint16_t max_x = button->x + button->len_x;
    uint16_t max_y = button->y + button->len_y;

    if (strlen(button->title) > max_x - 2)
    {
        button->error = TITLE_BOUNDARY_EXCEEDED;
        return;
    }

    if (button->x > (window->x + window->len_x) - 1)
    {
        button->error = X_BOUNDARY_EXCEEDED;
        return;
    }

    if (button->y > (window->x + window->len_x) - 1)
    {
        button->error = Y_BOUNDARY_EXCEEDED;
        return;
    }

    if (max_x >= (window->x + window->len_x) - 1)
    {
        button->error = LEN_X_BOUNDARY_EXCEEDED;
        return;
    }

    if (max_y >= (window->y + window->len_y) - 1)
    {
        button->error = LEN_Y_BOUNDARY_EXCEEDED;
        return;
    }

    /* Check if button interferes with other buttons */
    for (int i = id; i >= 0; i--)
    {
        if (id == 0)
        {
            /* Button passed checks above */
            if (button->error == NO_CHECK)
            {
                button->error = NO_ERROR;
                break;
            }

            /* Return button with error above */
            break;
        }

        /* Button already has an error, we don't need to go deeper */
        if (button->error != NO_CHECK)
        {
            break;
        }

        int previous_button_id = i - 1;
        Button* previous_button = window->elements.button.array[previous_button_id];

        if (button->x + button->len_x < previous_button->x ||
            button->x > previous_button->x + previous_button->len_x)
        {
            if (button->y + button->len_y < previous_button->y ||
                button->y > previous_button->y + previous_button->len_y ||
                button->y + button->len_y == previous_button->y + previous_button->len_y)
            {
                button->error = NO_ERROR;
            }

            else
            {
                button->error = Y_IN_WINDOW;
            }
        }

        else
        {
            button->error = X_IN_WINDOW;
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

ContextMenu init_context_menu(uint16_t x, uint16_t y, uint16_t len_x, uint16_t len_y, 
                    uint16_t background_color, uint16_t foreground_color, vector_t context_buttons)
{
    ContextMenu new;
    new.error = NO_CHECK;

    new.x = x;
    new.y = y;

    new.len_x = len_x;
    new.len_y = len_y;
    
    new.background_color = background_color;
    new.foreground_color = foreground_color;

    new.context_buttons = context_buttons;
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

    context_menu->error = NO_ERROR;
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

    for (int i = 0; i < context_menu.context_buttons.size; i++)
    {
        ContextButton* current_context_button = context_menu.context_buttons.array[i];
        if (current_context_button->content == "n/a")
        {
            if (window.debug)
            {
                printf("Empty button! Skipping...\r\n");
            }
        }

        else
        {
            if (window.y + context_menu.y + ((i + 1) * 3) > window.y + context_menu.y + context_menu.len_y)
            {
                break;
            }

            move_cursor(window.x + context_menu.x, window.y + context_menu.y + ((i + 1) * 3));

            for (uint16_t i = 0; i < context_menu.len_x; i++)
            {
                printf("%c", 0xC4);
            }

            move_cursor(window.x + context_menu.x, window.y + context_menu.y + ((i + 1) * 2));
            printf("%s", current_context_button->content);
        }
    }
}
