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
uint16_t format_color(uint16_t background_color, uint16_t foreground_color)
{
    return (background_color * 16) + foreground_color;
}

/**
 * Check if enter key has been pressed on button
 * @param screen screen containing all windows containing all buttons
*/
void keyboard_event(Screen* screen)
{
    bool dont_draw = false; /* we are clicking one of paint's button if true, we are not drawing */

    Window* current_window = &screen->windows[screen->active_window];

    /* Check if close window button is pressed */
    if (global_cursor.x == current_window->x + 1 && global_cursor.y == current_window->y)
    {
        if (screen->active_window != 0)
        {
            // clear_window(*current_window);

            clear_screen();

            draw_window(screen->windows[0]);
            screen->active_window = 0;

            draw_window_elements(screen->windows[0], 0);
        }

        return;
    }

    /* Perform actions regarding shell window */
    if (screen->active_window == 5)
    {
        if (global_cursor.y + 2 >= current_window->y + current_window->len_y)
        {
            clear_window(*current_window);
            draw_window(*current_window);
            draw_window_elements(*current_window, screen->active_window);
            move_cursor(current_window->x + 2, global_cursor.y + 1);
        }

        move_cursor(current_window->x + 2, global_cursor.y + 2);

        /* execute command */
        char parsed_line[60];
        char* command;

        move_cursor(current_window->x + 2, global_cursor.y - 1);
        for (int i = 1; i < screen->argument->shell->index + 1; i++)
        {
            parsed_line[i - 1] = screen->argument->shell->line[i];
        }

        parsed_line[screen->argument->shell->index] = '\0';
        command = parsed_line;

        if (strcmp(command, "about") == 0)
        {
            printf("Pelly (v0.9.0) is a 16-bit operating system!");
        }

        else if (strcmp(command, "clear") == 0 || strcmp(command, "cls") == 0)
        {
            clear_window(*current_window);
            draw_window(*current_window);
            draw_window_elements(*current_window, screen->active_window);
            move_cursor(current_window->x + 2, global_cursor.y + 1);
        }

        else if (strcmp(command, "exit") == 0)
        {
            // clear_window(*current_window);

            clear_screen();

            draw_window(screen->windows[0]);
            screen->active_window = 0;

            draw_window_elements(screen->windows[0], 0);
            return;
        }

        else if(strcmp(command, "help") == 0)
        {
            printf("about - something about Pelly");

            move_cursor(current_window->x + 2, global_cursor.y + 1);
            printf("clear/cls - clear shell window");

            move_cursor(current_window->x + 2, global_cursor.y + 1);
            printf("exit - exit shell window");

            move_cursor(current_window->x + 2, global_cursor.y + 1);
            printf("help - get list of available commands");
        }

        else
        {
            printf("Command (%s) not found!", command);
        }

        for (int i = 0; i < 60; i++)
        {
            screen->argument->shell->line[i] = 0;
        }

        screen->argument->shell->index = 0;

        move_cursor(current_window->x + 2, global_cursor.y + 1);
        printf(">> ");
        move_cursor(global_cursor.x + 3, global_cursor.y);

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

                            move_cursor(current_window->x + 19, 22);
                            printf("                         ");

                            move_cursor(current_window->x + 19, 22);
                            printf("First number is: %d", screen->argument->calculator->first_number);
                        }

                        else if (screen->argument->calculator->is_first_number == false)
                        {
                            screen->argument->calculator->second_number = screen->argument->calculator->second_number * 10 + j;

                            move_cursor(current_window->x + 19, 22);
                            printf("                         ");

                            move_cursor(current_window->x + 19, 22);
                            printf("Second number is: %d", screen->argument->calculator->second_number);
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

                /* Perform actions of paint's buttons */
                else if (screen->active_window == 4)
                {
                    uint16_t bg_colors[9] = {RED, YELLOW, GREEN, CYAN, BLACK, WHITE, LGRAY, BROWN, MAGENTA};
                    screen->argument->paint->background_color = bg_colors[j];

                    dont_draw = true;
                }

                break;
            }
        }
    }

    /* Perform actions regarding shell window */
    if (screen->active_window == 4 && !dont_draw)
    {
        change_color(format_color(BLUE, screen->argument->paint->background_color), (uint16_t)1);
        printf("%c", 0xDB);
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

                    if (screen->active_window == 5)
                    {
                        screen->argument->shell->index--;
                    }
                }

                else
                {
                    /* Update global cursor (disabled because we are not printing a character) */
                    // move_cursor(global_cursor.x + 1, global_cursor.y);

                    /**
                     * If we are in shell window, print character.
                     * Otherwise, character printing is disabled as before. 
                    */
                    if (screen->active_window == 5)
                    {
                        printf("%c", ascii_code);
                        move_cursor(global_cursor.x + 1, global_cursor.y);

                        if (screen->argument->shell->index != 59)
                        {
                            screen->argument->shell->index++;
                            screen->argument->shell->line[screen->argument->shell->index] = ascii_code;
                        }
                    }
                }

                break;
        }
    }
}
