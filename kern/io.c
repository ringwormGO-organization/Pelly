/**
 * @author Andrej Bartulin, Stjepan Bilić Matišić
 * PROJECT: Pelly
 * LICENSE: MIT
 * DESCRIPTION: Main C file for IO and GUI events
*/

#include "io.h"

#include "filesystem/fat.h"
#include "gui/gui.h"
#include "libc/string.h"

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
    bool dont_write = false; /* we are clicking save button in notepad if true, do not enter '\r\n' in text variable */

    Window* current_window = &screen->windows[screen->active_window];

    get_real_cursor_position();

    /* Check if close window button is pressed */
    if (cursor_x == current_window->x + 1 && cursor_y == current_window->y)
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
    if (screen->active_window == 6)
    {
        if (cursor_y + 2 >= current_window->y + current_window->len_y)
        {
            clear_window(*current_window);
            draw_window(*current_window);
            draw_window_elements(*current_window, screen->active_window);
            move_real_cursor(current_window->x + 2, cursor_y + 1);
        }

        move_real_cursor(current_window->x + 2, cursor_y + 2);

        /* execute command */
        char parsed_line[60];
        char* command;

        move_real_cursor(current_window->x + 2, cursor_y - 1);
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
            move_real_cursor(current_window->x + 2, cursor_y + 1);
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

            move_real_cursor(current_window->x + 2, cursor_y + 1);
            printf("clear/cls - clear shell window");

            move_real_cursor(current_window->x + 2, cursor_y + 1);
            printf("exit - exit shell window");

            move_real_cursor(current_window->x + 2, cursor_y + 1);
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

        move_real_cursor(current_window->x + 2, cursor_y + 1);
        printf(">> ");

        get_real_cursor_position();
        move_real_cursor(cursor_x, cursor_y);

        return;
    }

    for (int j = 0; j < NUMBER_OF_BUTTONS; j++)
    {
        Button current_button = current_window->elements.button[j];

        if (cursor_x > current_window->x + current_button.x && 
            cursor_x < current_window->x + current_button.x + 
            current_button.len_x)
        {
            if (cursor_y > current_window->y + current_button.y &&
                cursor_y < current_window->y + current_button.y +
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

                            move_real_cursor(current_window->x + 19, 22);
                            printf("                         ");

                            move_real_cursor(current_window->x + 19, 22);
                            printf("First number is: %d", screen->argument->calculator->first_number);
                        }

                        else if (screen->argument->calculator->is_first_number == false)
                        {
                            screen->argument->calculator->second_number = screen->argument->calculator->second_number * 10 + j;

                            move_real_cursor(current_window->x + 19, 22);
                            printf("                         ");

                            move_real_cursor(current_window->x + 19, 22);
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
                                        move_real_cursor(current_window->x + 2, 22);
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
                                move_real_cursor(current_window->x + 2, 22);
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

                /* Perform actions of file explorer's window */
                else if (screen->active_window == 2)
                {
                    FAT_File far* fd = FAT_Open(&screen->diskA, "/");
                    FAT_DirectoryEntry entry;

                    /* Search button */
                    if (j == 0)
                    {
                        move_real_cursor(current_window->x + 2, cursor_y + 2);
                        printf("FAT: \r\n");
                        move_real_cursor(current_window->x + 2, cursor_y + 1);

                        int i = 0;
                        while (FAT_ReadEntry(&screen->diskA, fd, &entry) && i++ < 5)
                        {
                            printf("  ");
                            for (int i = 0; i < 11; i++)
                                putc(entry.Name[i]);

                            printf("\r\n");
                            move_real_cursor(current_window->x + 2, cursor_y + 1);
                        }
                        FAT_Close(fd);

                        /* -------------------- */

                        printf("BBFS v2: \r\n");
                        move_real_cursor(current_window->x + 2, cursor_y + 1);

                        for (int i = 0; i < screen->argument->file_explorer->index; i++)
                        {
                            printf("  %s\r\n", screen->argument->file_explorer->files[i]);
                            move_real_cursor(current_window->x + 2, cursor_y + 1);
                        }
                    }

                    /* Open button */
                    else if (j == 1)
                    {
                        char file_system[5];
                        char path[OPEN_LINE_SIZE - 5];

                        screen->argument->file_explorer->open[screen->argument->file_explorer->open_index] = '\0';

                        for (int i = 0; i < screen->argument->file_explorer->open_index; i++)
                        {
                            if (i < 4)
                            {
                                file_system[i] = screen->argument->file_explorer->open[i];
                            }

                            file_system[5] = '\0';

                            if (i >= 4)
                            {
                                path[i - 4] = screen->argument->file_explorer->open[i];
                            }
                        }

                        path[screen->argument->file_explorer->open_index] = '\0';

                        char* file_system_str = file_system;
                        char* path_str = path;

                        if (strcmp(file_system_str, "fat/") == 0)
                        {
                            /* TODO */
                        }

                        else if (strcmp(file_system_str, "ram/") == 0)
                        {
                            bool found = false;

                            char temp[12];
                            for (int i = 0; i < screen->argument->file_explorer->index; i++)
                            {
                                strncpy(temp, screen->argument->file_explorer->files[i], 11);

                                char* tmp = temp;
                                if (strcmp(tmp, path_str) <= 0)
                                {
                                    move_real_cursor(current_window->x + 2, cursor_y + 2);
                                    printf("File: %s\r\n", screen->argument->file_explorer->files[i]);
                                    move_real_cursor(current_window->x + 2, cursor_y + 1);

                                    found = true;
                                    break;
                                }
                            }

                            if (found == false)
                            {
                                move_real_cursor(current_window->x + 2, cursor_y + 2);
                                printf("File (%s) not found!", path_str);
                                move_real_cursor(current_window->x + 2, cursor_y + 1);
                            }
                        }

                        for (int i = 0; i < screen->argument->file_explorer->open_index; i++)
                        {
                            screen->argument->file_explorer->open[i] = ' ';
                        }

                        screen->argument->file_explorer->open_index = 0;
                    }
                }

                /* Perform actions of notepad's buttons */
                else if (screen->active_window == 3)
                {
                    if (screen->argument->file_explorer->index >= EXPLORER_SIZE)
                    {
                        printf("No space left!");
                        return;
                    }

                    screen->argument->notepad->text[screen->argument->notepad->index] = '\0';
                    screen->argument->notepad->index++;

                    char* text = screen->argument->notepad->text;
                    screen->argument->file_explorer->files[screen->argument->file_explorer->index] = strdup(text);

                    screen->argument->file_explorer->index++;
                    dont_write = true;

                    screen->argument->notepad->index = 0;
                }

                /* Perform actions of paint's buttons */
                else if (screen->active_window == 4)
                {
                    uint16_t bg_colors[9] = {RED, YELLOW, GREEN, CYAN, BLACK, WHITE, LGRAY, BROWN, MAGENTA};
                    screen->argument->paint->background_color = bg_colors[j];

                    dont_draw = true;
                }

                /* Perform actions on random's window */
                else if (screen->active_window == 5)
                {
                    screen->argument->random->lcg_seed = (LCG_MULTIPLIER * screen->argument->random->lcg_seed + LCG_INCREMENT) % LCG_MODULUS;

                    uint16_t min = 0;
                    uint16_t max = 100;
                    
                    uint16_t result = min + screen->argument->random->lcg_seed % (max - min + 1);

                    uint16_t old_x = cursor_x;
                    uint16_t old_y = cursor_y;

                    move_real_cursor(current_window->x + 2, cursor_y + 3);
                    printf("Random number is: %d!", result);

                    move_real_cursor(old_x, old_y);
                }

                break;
            }
        }
    }

    /* Perform actions regarding notepad window */
    if (screen->active_window == 3 && !dont_write)
    {
        move_real_cursor(current_window->x + 2, cursor_y + 1);

        screen->argument->notepad->text[screen->argument->notepad->index] = '\r';
        screen->argument->shell->index++;

        screen->argument->notepad->text[screen->argument->notepad->index] = '\n';
        screen->argument->shell->index++;
    }

    /* Perform actions regarding paint window */
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

                break;

            case 1:
                /* Move real, assembly cursor */
                down_cursor();

                break;

            case 2:
                /* Move real, assembly cursor */
                left_cursor();

                break;

            case 3:
                /* Move real, assembly cursor */
                right_cursor();

                break;

            default:
                /* Enter key */
                if (ascii_code == 13)
                {
                    keyboard_event(screen);

                    /* Print character (disabled) */
                    // printf("\r\n");

                    /* Update global cursor (disabled because we are not printing a character) */
                    // get_real_cursor_position();
                    // move_real_cursor(cursor_x, cursor_y);
                }

                /* Backspace key */
                else if (ascii_code == 8)
                {
                    /* Print character (disabled) */
                    // printf("\b ");

                    /* Update global cursor */
                    // get_real_cursor_position();
                    // move_real_cursor(cursor_x - 1, cursor_y);

                    /* File explorer window */
                    if (screen->active_window == 2)
                    {
                        screen->argument->file_explorer->open_index--;
                    }

                    /* Notepad window */
                    else if (screen->active_window == 3)
                    {
                        screen->argument->notepad->index--;
                    }

                    /* Shell window */
                    else if (screen->active_window == 6)
                    {
                        screen->argument->shell->index--;
                    }
                }

                else
                {
                    /* Print character (disabled) */
                    // printf("%c", ascii_code);

                    /* Update global cursor (disabled because we are not printing a character) */
                    // get_real_cursor_position();
                    // move_real_cursor(cursor_x, cursor_y);

                    /**
                     * If we are in file explorer window, print character.
                     * Otherwise, character printing is disabled as before. 
                    */
                    if (screen->active_window == 2)
                    {
                        printf("%c", ascii_code);

                        get_real_cursor_position();
                        move_real_cursor(cursor_x, cursor_y);

                        if (screen->argument->file_explorer->open_index != OPEN_LINE_SIZE - 1)
                        {
                            screen->argument->file_explorer->open[screen->argument->file_explorer->open_index] = ascii_code;
                            screen->argument->file_explorer->open_index++;
                        }
                    }

                    /**
                     * If we are in notepad window, print character.
                     * Otherwise, character printing is disabled as before. 
                    */
                    else if (screen->active_window == 3)
                    {
                        printf("%c", ascii_code);

                        get_real_cursor_position();
                        move_real_cursor(cursor_x, cursor_y);

                        if (screen->argument->notepad->index != TEXT_SIZE - 1)
                        {
                            screen->argument->notepad->text[screen->argument->notepad->index] = ascii_code;
                            screen->argument->notepad->index++;
                        }
                    }

                    /**
                     * If we are in shell window, print character.
                     * Otherwise, character printing is disabled as before. 
                    */
                    else if (screen->active_window == 6)
                    {
                        printf("%c", ascii_code);

                        get_real_cursor_position();
                        move_real_cursor(cursor_x, cursor_y);

                        if (screen->argument->shell->index != LINE_SIZE - 1)
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
