/**
 * @author Andrej Bartulin, Stjepan Bilić Matišić
 * PROJECT: Pelly
 * LICENSE: MIT
 * DESCRIPTION: Main C file for IO
*/

#include "io.h"

#include "gui/gui.h"

int _cdecl cursor_x;
int _cdecl cursor_y;

int _cdecl ascii_code;

/**
 * Format color so it can be used properly in `change_color` function
 * @param background_color current background color
 * @param foreground_color foreground_color
*/
int format_color(int background_color, int foreground_color)
{
    return (background_color * 16) + foreground_color;
}

/**
 * Check if enter key has been pressed on button
 * @param screen screen containing all windows containing all buttons
*/
void keyboard_event(Screen* screen)
{
    Window* current_window = &screen->windows[screen->active_window];

    /* Check if close window button is pressed */
    if (global_cursor.x == current_window->x + 1 && global_cursor.y == current_window->y)
    {
        if (screen->active_window != 0)
        {
            /* Free strings in calculator */
            /*if (screen->active_window == 1)
            {
                for (int i = 0; i < 10; i++)
                {
                    free(screen->windows[screen->active_window].elements.button[i].title);
                }
            }*/

            clear_window(*current_window);

            return;

            draw_window(screen->windows[0]);
            screen->active_window = 0;

            draw_window_elements(screen->windows[0], 0);
        }

        return;
    }

    for (int j = 0; j < NUMBER_OF_BUTTONS; j++)
    {
        Button current_button = current_window->elements.button[j];

        if (global_cursor.x > current_window->x + current_button.x && 
            global_cursor.x < current_window->x + current_button.x + 
            current_button.len_x)
        {
            if (global_cursor.y > current_window->y + current_button.y &&
                global_cursor.y < current_window->y + current_button.y +
                current_button.len_y)
            {
                /* execute a function */

                /* Perform actions on window manager's buttons */
                if (screen->active_window == 0)
                {
                    screen->active_window = j + 1;
                    current_button.action(screen->windows[j + 1], screen->argument);
                }

                /* Perform actions of calculator's buttons */
                else if (screen->active_window == 1)
                {
                    if (j > -1 && j < 10)
                    {
                        if (screen->argument->calculator->is_first_number == true)
                        {
                            screen->argument->calculator->first_number = screen->argument->calculator->first_number * 10 + j;
                        }

                        else if (screen->argument->calculator->is_first_number == false)
                        {
                            screen->argument->calculator->second_number = screen->argument->calculator->second_number * 10 + j;
                        }
                    }

                    else if (j == 10)
                    {
                        screen->argument->calculator->op = '+';
                    }

                    else if (j == 11)
                    {
                        screen->argument->calculator->op = '-';
                    }

                    else if (j == 12)
                    {
                        screen->argument->calculator->op = '*';
                    }

                    else if (j == 13)
                    {
                        screen->argument->calculator->op = '/';
                    }

                    else if (j == 14)
                    {
                        /* enter */

                        if (screen->argument->calculator->is_first_number == true)
                        {
                            screen->argument->calculator->is_first_number = false;
                        }

                        else /* second number is entered */
                        {
                            bool print_result = true;

                            uint32_t result = 0;
                            switch (screen->argument->calculator->op)
                            {
                                case '+':
                                    result = screen->argument->calculator->first_number + screen->argument->calculator->second_number;
                                    break;

                                case '-':
                                    result = screen->argument->calculator->first_number - screen->argument->calculator->second_number;
                                    break;

                                case '*':
                                    result = screen->argument->calculator->first_number * screen->argument->calculator->second_number;
                                    break;

                                case '/':
                                    if (screen->argument->calculator->second_number == 0)
                                    {
                                        move_cursor(current_window->x + 2, 22);
                                        printf("Result is: N/A");

                                        print_result = false;
                                        break;
                                    }

                                    result = screen->argument->calculator->first_number / screen->argument->calculator->second_number;
                                    break;
                                
                                default:
                                    break;
                            }

                            if (print_result == true)
                            {
                                move_cursor(current_window->x + 2, 22);
                                printf("Result is: %d", result);
                            }
                        }
                    }

                    else if (j == 15)
                    {
                        if (screen->argument->calculator->is_first_number == true)
                        {
                            screen->argument->calculator->first_number = 0;
                        }

                        else
                        {
                            screen->argument->calculator->second_number = 0;
                        }
                    }
                }

                /* Perform actions regarding shell window */
                else if (screen->active_window == 5)
                {
                    /**
                     * See comments on line 248-251 in this file
                    */
                    move_cursor(current_window->x + 2, global_cursor.y + 1);

                    printf(">> ");
                    move_cursor(global_cursor.x + 3, global_cursor.y);
                }

                break;
            }
        }
    }
}

/**
 * Main keyboard loop
 * @param screen screen for checking events
*/
void c_keyboard_loop(Screen* screen) 
{
    while (1)
    {
        /* Get pressed key from assembly */
        asm_keyboard_loop();
        
        /* Check ASCII code */
        switch (ascii_code)
        {
            case 0:
                /* Move real, assembly cursor */
                up_cursor();

                /* Update global cursor */
                get_cursor_position();
                move_cursor(cursor_x, cursor_y);

                break;

            case 1:
                /* Move real, assembly cursor */
                down_cursor();

                /* Update global cursor */
                get_cursor_position();
                move_cursor(cursor_x, cursor_y);

                break;

            case 2:
                /* Move real, assembly cursor */
                left_cursor();

                /* Update global cursor */
                get_cursor_position();
                move_cursor(cursor_x, cursor_y);

                break;

            case 3:
                /* Move real, assembly cursor */
                right_cursor();

                /* Update global cursor */
                get_cursor_position();
                move_cursor(cursor_x, cursor_y);

                break;

            default:
                /* Print character (disabled) */
                // printf("%c", ascii_code);

                /**
                 * If we are in shell window, print character.
                 * Otherwise, character printing is disabled as before. 
                */
                if (screen->active_window == 5)
                {
                    printf("%c", ascii_code);
                }

                /* Enter key*/
                if (ascii_code == 13)
                {
                    keyboard_event(screen);

                    /* Update global cursor (disabled because we are not printing a character) */
                    // move_cursor(0, global_cursor.y + 1);
                }

                else if (ascii_code == 8)
                {
                    /* Update global cursor */
                    move_cursor(global_cursor.x - 1, global_cursor.y);
                }

                else
                {
                    /* Update global cursor (disabled because we are not printing a character) */
                    // move_cursor(global_cursor.x + 1, global_cursor.y);

                    /**
                     * See comments on line 248-251 in this file
                    */
                    if (screen->active_window == 5)
                    {
                        move_cursor(global_cursor.x + 1, global_cursor.y);
                    }
                }

                break;
        }
    }
}
