/**
 * @author Andrej Bartulin, Stjepan Bilić Matišić
 * PROJECT: Pelly
 * LICENSE: MIT
 * DESCRIPTION: Main C file for GUI
*/

#include "gui.h"

// NOTE: use `init` function
// this is just to speed up development process

void test()
{
    putc('e');
}

Button empty_button = {
    .error = EMPTY,
};

ContextMenu context_menu = {
    .error = NO_ERROR,
    .x = 9,
    .y = 9,
    .len_x = 5,
    .len_y = 5,
    .background_color = RED,
    .foreground_color = WHITE,
};

ContextButton empty_context_button = {
    .content = "n/a",
};

ContextButton context_button = {
    .content = "one",
    .action = test,
};

Window init_window(uint16_t x, uint16_t y, uint16_t len_x, uint16_t len_y, 
                    uint16_t background_color, uint16_t foreground_color, char* title, bool debug)
{
    Window new;

    new.debug = debug;
    new.error = NO_CHECK;

    new.x = x;
    new.y = y;

    new.len_x = len_x;
    new.len_y = len_y;
    
    new.background_color = background_color;
    new.foreground_color = foreground_color;
    
    new.title = title;

    /* Initialize an array */
    for (int i = 0; i < NUMBER_OF_BUTTONS; i++)
    {
        new.elements.button[i].error = EMPTY;
    }
    
    if (new.debug)
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

void check_window(Screen* screen, int id)
{
    Window* current_window = &screen->windows[id];

    if (current_window->error == EMPTY)
    {
        return;
    }

    if (strlen(current_window->title) > 70 || strlen(current_window->title) > (current_window->len_x - 2))
    {
        current_window->error = TITLE_BOUNDARY_EXCEEDED;
    }

    if (current_window->x > screen->len_x - 1)
    {
        current_window->error = X_BOUNDARY_EXCEEDED;
    }

    if (current_window->y > screen->len_y - 1)
    {
        current_window->error = Y_BOUNDARY_EXCEEDED;
    }

    if (current_window->len_x >= ((screen->len_x - 1) - current_window->x))
    {
        current_window->error = LEN_X_BOUNDARY_EXCEEDED;
    }

    if (current_window->len_y >= ((screen->len_y - 1) - current_window->y))
    {
        current_window->error = LEN_Y_BOUNDARY_EXCEEDED;
    }

    /* Check if window interferes with other windows */
    for (int i = id; i >= 0; i--)
    {
        if (id == 0)
        {
            /* Window passed checks above */
            if (current_window->error == NO_CHECK)
            {
                current_window->error = NO_ERROR;
                break;
            }

            /* Return window with error above */
            break;
        }

        /* Window already has an error, we don't need to go deeper */
        if (current_window->error != NO_CHECK)
        {
            break;
        }

        int previous_window_id = i - 1;
        Window* previous_window = &screen->windows[previous_window_id];

        if (previous_window->error == NO_ERROR)
        {
            if (previous_window->x +  previous_window->len_x < 
                                        previous_window->x ||
                previous_window->x >  previous_window->x + 
                                        previous_window->len_x)
            {
                if (previous_window->y +  previous_window->len_y < 
                                            previous_window->y ||
                    previous_window->y >  previous_window->y + 
                                            previous_window->len_y)
                {
                    current_window->error = NO_ERROR;
                }

                else
                {
                    current_window->error = Y_IN_WINDOW;
                }
            }

            else
            {
               current_window->error = X_IN_WINDOW;
            }
        }
    }
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

void draw_window_elements(Window window, int window_id)
{
    /* ******************************** */
    /*          Buttons                 */
    /* ******************************** */

    for (int i = 0; i < NUMBER_OF_BUTTONS; i++)
    {
        Button current_button = window.elements.button[i];
        if (current_button.error == EMPTY)
        {
            if (window.debug)
            {
                printf("Empty button! Skipping...\r\n");
            }

            continue;
        }

        check_button(&window, i);

        if (window.elements.button[i].error != NO_ERROR)
        {
            printf("Error code %d of button %d, window %d\r\n", current_button.error, i, window_id);
            continue;
        }

        draw_button(window, current_button);
        move_cursor(0, 0);
    }

    /*check_context_menu(window, &window.elements.context_menu);

    if (window.elements.context_menu.error == NO_ERROR)
    {
        draw_context_menu(window, window.elements.context_menu);
    }

    else
    {
        printf("Error code %d of context menu, window %d\r\n", window.elements.context_menu.error, window_id);
    }*/
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
        move_cursor(window.x, window.y + i);
        for (uint16_t j = 0; j < window.len_x; j++)
        {
            printf("%c", 0x20);
        }   
    }
}

void calculator(Window program_window, Argument* argument)
{
    clear_screen();
    draw_window(program_window);
    draw_window_elements(program_window, 1);

    return;
}

void file_explorer(Window program_window, Argument* argument)
{
    clear_screen();
    draw_window(program_window);
    draw_window_elements(program_window, 2);

    return;
}

void notepad(Window program_window, Argument* argument)
{
    clear_screen();
    draw_window(program_window);
    draw_window_elements(program_window, 3);

    move_cursor(program_window.x + 2, 11);

    return;
}

void paint(Window program_window, Argument* argument)
{
    clear_screen();
    draw_window(program_window);
    draw_window_elements(program_window, 4);

    return;
}

void random(Window program_window, Argument* argument)
{
    clear_screen();
    draw_window(program_window);
    draw_window_elements(program_window, 5);

    argument->random->lcg_seed = 1234;

    return;
}

void shell(Window program_window, Argument* argument)
{
    clear_screen();
    draw_window(program_window);
    draw_window_elements(program_window, 6);

    move_cursor(program_window.x + 2, program_window.y + 3);

    printf(">> ");
    move_cursor(global_cursor.x + 3, global_cursor.y);

    return;
}

/* void web_browser(Window program_window, Argument* argument)
{
    clear_screen();
    draw_window(program_window);
    draw_window_elements(program_window, 6);

    return;
} */

// 0x0a - \r\n
// 0x0d - \r

void start_gui(DISK diskA)
{
    clear_screen();
    move_cursor(0, 0);

    /* -------------------------------- */

    Screen screen;

    screen.len_x = 80;
    screen.len_y = 25;

    screen.diskA = diskA;

    /* -------------------------------- */

    Window window_manager = init_window(12, 5, 40, 15, BLUE, WHITE, "Window manager", false);
    screen.windows[0] = window_manager;

    Window calculator_window = init_window(12, 3, 40, 20, BLUE, WHITE, "Calculator", false);
    screen.windows[1] = calculator_window;

    Window file_explorer_window = init_window(12, 1, 40, 23, BLUE, WHITE, "File Explorer", false);
    screen.windows[2] = file_explorer_window;

    Window notepad_window = init_window(12, 5, 40, 15, BLUE, WHITE, "Notepad", false);
    screen.windows[3] = notepad_window;

    Window paint_window = init_window(5, 5, 55, 17, BLUE, WHITE, "Paint", false);
    screen.windows[4] = paint_window;

    Window random_window = init_window(12, 5, 40, 15, BLUE, WHITE, "Random", false);
    screen.windows[5] = random_window;

    Window shell_window = init_window(4, 5, 64, 15, BLUE, WHITE, "Shell", false);
    screen.windows[6] = shell_window;

    /* Window web_browser_window = init_window(12, 5, 40, 15, BLUE, WHITE, "Web browser", false);
    screen.windows[6] = web_browser_window; */

    /* ------------ */

    /* Digits 0-9 buttons */

    char* digits[10] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};
    for (int i = 0; i < 10; i++) 
    {
        screen.windows[1].elements.button[i] = init_button(2 + (i % 3) * 9, 3 + (i / 3) * 3, 6, 2, BLUE, WHITE, digits[i], NULL);
    }
    
    screen.windows[1].elements.button[10] = init_button(28, 3, 6, 2, BLUE, WHITE, "+", NULL);
    screen.windows[1].elements.button[11] = init_button(28, 6, 6, 2, BLUE, WHITE, "-", NULL);
    screen.windows[1].elements.button[12] = init_button(28, 9, 6, 2, BLUE, WHITE, "*", NULL);
    screen.windows[1].elements.button[13] = init_button(28, 12, 6, 2, BLUE, WHITE, "/", NULL);
    screen.windows[1].elements.button[14] = init_button(2, 15, 17, 2, BLUE, WHITE, "Enter", NULL);
    screen.windows[1].elements.button[15] = init_button(22, 15, 17, 2, BLUE, WHITE, "Reset", NULL);

    /* ------------ */

    screen.windows[2].elements.button[0] = init_button(2, 3, 8, 2, BLUE, WHITE, "Search", NULL);
    screen.windows[2].elements.button[1] = init_button(12, 3, 6, 2, BLUE, WHITE, "Open", NULL);

    /* ------------ */

    screen.windows[3].elements.button[0] = init_button(2, 3, screen.windows[3].len_x - 4, 2, BLUE, WHITE, "Save", NULL);

    /* ------------ */

    for (int i = 0; i < 9; i++) 
    {
        uint16_t bg_colors[9] = {RED, YELLOW, GREEN, CYAN, BLACK, WHITE, LGRAY, BROWN, MAGENTA};
        uint16_t fg_colors[9] = {WHITE, BLACK, WHITE, WHITE, WHITE, BLACK, WHITE, WHITE, WHITE};

        screen.windows[4].elements.button[i] = init_button(i * 5 + 2, 3, 3, 2, bg_colors[i], fg_colors[i], "1", NULL);
    }

    /* ------------ */

    screen.windows[5].elements.button[0] = init_button(2, 3, 10, 2, BLUE, WHITE, "Generate", NULL);

    /* ------------ */

    /* screen.windows[6].elements.button[0] = init_button(2, 3, 9, 2, BLUE, WHITE, "Execute", NULL); */
    /* screen.windows[6].elements.button[0] = init_button(2, 3, 8, 2, BLUE, WHITE, "Search", NULL); */

    /* -------------------------------- */

    check_window(&screen, 0);
    
    Window current_window = screen.windows[0];
    if (current_window.error == NO_ERROR)
    {
        draw_window(current_window);
        screen.active_window = 0;
    }

    else
    {
        move_cursor(0, 0);
        printf("Error code %d of window %d\r\n", current_window.error, 0);
    }

    /* -------------------------------- */

    Calculator calculator_struct;

    calculator_struct.first_number = 0;
    calculator_struct.op = '+';
    calculator_struct.second_number = 0;
    calculator_struct.is_first_number = true;

    /* ------------ */

    FileExplorer file_explorer_struct;
    file_explorer_struct.index = 0;
    file_explorer_struct.open_index = 0;

    /* ------------ */

    Notepad notepad_struct;
    notepad_struct.index = 0;

    /* ------------ */

    Paint paint_struct;
    paint_struct.background_color = RED;

    /* ------------ */

    Random random_struct;
    random_struct.lcg_seed = 0;

    /* ------------ */

    Shell shell_struct;
    shell_struct.index = 0;

    /* ------------ */

    Argument argument;

    argument.calculator = &calculator_struct;
    argument.file_explorer = &file_explorer_struct;
    argument.notepad = &notepad_struct;
    argument.paint = &paint_struct;
    argument.random = &random_struct;
    argument.shell = &shell_struct;

    screen.argument = &argument;

    char* programs_names[6] = { "Calc", "Explorer", "Notepad", 
                                "Paint", "Random", "Shell"};

    void (*p[6]) (Window, Argument*) = {  calculator, file_explorer, notepad,
                            paint, random, shell};

    for (int i = 0; i < 6; i++)
    {
        const int button_per_row = 3;

        int row = i / button_per_row;
        int col = i % button_per_row;

        const int margin_x = 2;
        const int margin_y = 1;

        const int width = 10;
        const int height = 2;
    
        Button program_button = init_button(
            window_manager.x + margin_x + (width + margin_x) * col - 10,
            window_manager.y + margin_y + (height + margin_y) * row,
            width, height, BLUE, WHITE, programs_names[i], p[i]);
        
        window_manager.elements.button[i] = program_button;
        screen.windows[0] = window_manager;
    }

    /* -------------------------------- */

    draw_window_elements(screen.windows[0], 0);

    change_color(format_color(BLUE, LRED), (uint16_t)35);
    printf("===> Pelly Operating System - 1.0.0\r\n");

    change_color(format_color(BLUE, LGREN), (uint16_t)34);
    printf("Now with BBFS, GUI, heap & IVT! %d\r\n", 30);

keyboard_loop:
    move_cursor(0, 0);
    c_keyboard_loop(&screen);
}
