#include "gui.h"

Button init_button(Window window, uint16_t x, uint16_t y, uint16_t len_x, uint16_t len_y, 
                    uint16_t background_color, uint16_t foreground_color, char* title)
{
    Button new;
    new.error = NO_ERROR;

    new.x = x;
    new.y = y;

    new.len_x = len_x;
    new.len_y = len_y;
    
    new.background_color = background_color;
    new.foreground_color = foreground_color;
    
    new.title = title;

    return new;
}

void check_button(Window window, Button* button)
{
    uint16_t max_x = button->x + button->len_x;
    uint16_t max_y = button->y + button->len_y;

    if (strlen(button->title) > ((max_x - button->x) - 2))
    {
        button->error = TITLE_BOUNDARY_EXCEEDED;
        return;
    }

    if (button->x < window.x || button->x > max_x)
    {
        button->error = X_BOUNDARY_EXCEEDED;
        return;
    }

    if (button->y < window.y || button->y > max_y)
    {
        button->error = Y_BOUNDARY_EXCEEDED;
        return;
    }

    if (max_x >= (window.x + window.len_x))
    {
        button->error = LEN_X_BOUNDARY_EXCEEDED;
        return;
    }

    if (max_y >= (window.y + window.len_y))
    {
        button->error = LEN_Y_BOUNDARY_EXCEEDED;
        return;
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

void check_context_menu(Window window, ContextMenu context_menu)
{

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
