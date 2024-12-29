/**
 * @author Andrej Bartulin, Stjepan Bilić Matišić
 * PROJECT: Pelly
 * LICENSE: MIT
 * DESCRIPTION: Main C file for GUI
*/

#include "gui.h"

Window create_window(uint16_t x, uint16_t y, uint16_t len_x, uint16_t len_y, 
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

void initialize_buttons(Window* window)
{
    for (int i = 0; i < NUMBER_OF_BUTTONS; i++)
    {
        window->elements.button[i].error = EMPTY;
    }
}

void check_window(Screen screen, Window* window)
{
    if (window->error == EMPTY)
    {
        return;
    }

    if (strlen(window->title) > 70 || strlen(window->title) > (window->len_x - 2))
    {
        window->error = TITLE_BOUNDARY_EXCEEDED;
    }

    if (window->x > screen.len_x - 1)
    {
        window->error = X_BOUNDARY_EXCEEDED;
    }

    if (window->y > screen.len_y - 1)
    {
        window->error = Y_BOUNDARY_EXCEEDED;
    }

    if (window->len_x >= ((screen.len_x - 1) - window->x))
    {
        window->error = LEN_X_BOUNDARY_EXCEEDED;
    }

    if (window->len_y >= ((screen.len_y - 1) - window->y))
    {
        window->error = LEN_Y_BOUNDARY_EXCEEDED;
    }

    window->error = NO_ERROR;
}

void draw_window(Window* window)
{
    /* ******************************** */
    /*          Top border              */
    /* ******************************** */

    move_real_cursor(window->x, window->y);
    
    for (uint16_t i = 0; i < window->len_x; i++)
    {
        printf("%c", 0xC4);
    }
    
    /* ******************************** */
    /*          Close window but.       */
    /* ******************************** */

    move_real_cursor(window->x, window->y);
    printf(" X%c",0xB3);

    /* ******************************** */
    /*          Right border            */
    /* ******************************** */

    for (uint16_t i = 0; i < window->len_y; i++)
    {
        move_real_cursor(window->x + window->len_x, window->y + i);
        printf("%c", 0xB3);
    }

    /* ******************************** */
    /*          Bottom border           */
    /* ******************************** */

    move_real_cursor(window->x, window->y + window->len_y);

    for (uint16_t i = 0; i < window->len_x + 1; i++)
    {
        printf("%c", 0xC4);
    }

    /* ******************************** */
    /*          Left border             */
    /* ******************************** */

    move_real_cursor(window->x, window->y);

    for (uint16_t i = 0; i < window->len_y; i++)
    {
        move_real_cursor(window->x, window->y + i);
        printf("%c", 0xB3);
    }

    /* ------------------------------------------------------------------------ */

    /* ******************************** */
    /*          Title space             */
    /* ******************************** */

    move_real_cursor(window->x + 1, window->y + 2);
    
    for (uint16_t i = 0; i < window->len_x - 1; i++)
    {
        printf("%c", 0xC4);
    }

    /* ******************************** */
    /*          Title                   */
    /* ******************************** */

    move_real_cursor(window->x + 1, window->y + 1);
    printf("%s", window->title);
}

void draw_window_elements(Window* window, int window_id)
{
    /* ******************************** */
    /*          Buttons                 */
    /* ******************************** */

    for (int i = 0; i < NUMBER_OF_BUTTONS; i++)
    {
        Button current_button = window->elements.button[i];

        check_button(window, &current_button);
        if (current_button.error != NO_ERROR)
        {
            if (current_button.error != EMPTY)
            {
                printf("Error code %d of button %d, window %d\r\n", current_button.error, i, window_id);
            }

            continue;
        }

        draw_button(window, current_button);
        move_real_cursor(0, 0);
    }
}

void clear_window(Window* window)
{   
    /* ******************************** */
    /*          Top border              */
    /* ******************************** */

    move_real_cursor(window->x, window->y);
    
    for (uint16_t i = 0; i < window->len_x; i++)
    {
        printf("%c", 0x20);
    }

    /* ******************************** */
    /*          Close window but.       */
    /* ******************************** */

    move_real_cursor(window->x, window->y);
    printf("   ");

    /* ******************************** */
    /*          Right border            */
    /* ******************************** */

    for (uint16_t i = 0; i < window->len_y; i++)
    {
        move_real_cursor(window->x + window->len_x, window->y + i);
        printf("%c", 0x20);
    }

    /* ******************************** */
    /*          Bottom border           */
    /* ******************************** */

    move_real_cursor(window->x, window->y + window->len_y);

    for (uint16_t i = 0; i < window->len_x + 1; i++)
    {
        printf("%c", 0x20);
    }

    /* ******************************** */
    /*          Left border             */
    /* ******************************** */

    move_real_cursor(window->x, window->y);

    for (uint16_t i = 0; i < window->len_y; i++)
    {
        move_real_cursor(window->x, window->y + i);
        printf("%c", 0x20);
    }

    /* ------------------------------------------------------------------------ */

    /* ******************************** */
    /*          Title space             */
    /* ******************************** */

    move_real_cursor(window->x + 1, window->y + 2);
    
    for (uint16_t i = 0; i < window->len_x - 1; i++)
    {
        printf("%c", 0x20);
    }

    /* ******************************** */
    /*          Title                   */
    /* ******************************** */

    move_real_cursor(window->x + 1, window->y + 1);

    for (uint16_t i = 0; i < strlen(window->title); i++)
    {
        printf("%c", 0x20);
    }

    move_real_cursor(window->x, window->y);
    for (uint16_t i = 0; i < window->len_y; i++)
    {
        move_real_cursor(window->x, window->y + i);
        for (uint16_t j = 0; j < window->len_x; j++)
        {
            printf("%c", 0x20);
        }   
    }
}

void calculator(Window* program_window, Argument* argument)
{
    clear_screen();
    draw_window(program_window);
    draw_window_elements(program_window, 1);

    return;
}

void file_explorer(Window* program_window, Argument* argument)
{
    clear_screen();
    draw_window(program_window);
    draw_window_elements(program_window, 2);

    return;
}

void notepad(Window* program_window, Argument* argument)
{
    clear_screen();
    draw_window(program_window);
    draw_window_elements(program_window, 3);

    move_real_cursor(program_window->x + 2, 11);

    return;
}

void paint(Window* program_window, Argument* argument)
{
    clear_screen();
    draw_window(program_window);
    draw_window_elements(program_window, 4);

    return;
}

void random(Window* program_window, Argument* argument)
{
    clear_screen();
    draw_window(program_window);
    draw_window_elements(program_window, 5);

    argument->random->lcg_seed = 1234;

    return;
}

void shell(Window* program_window, Argument* argument)
{
    clear_screen();
    draw_window(program_window);
    draw_window_elements(program_window, 6);

    move_real_cursor(program_window->x + 2, program_window->y + 3);

    printf(">> ");

    get_real_cursor_position();    
    move_real_cursor(cursor_x, cursor_y);

    return;
}

/* void web_browser(Window* program_window, Argument* argument)
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
    move_real_cursor(0, 0);

    /* ---------------------------------------------------- */
    /*                  Create screen                       */
    /* ---------------------------------------------------- */

    Screen screen;

    screen.len_x = 80;
    screen.len_y = 25;

    screen.diskA = diskA;

    /* ---------------------------------------------------- */
    /*                  Create windows                      */
    /* ---------------------------------------------------- */

    Window window_manager = {
        .x = 12,
        .y = 5,
        .len_x = 40,
        .len_y = 15,
        .background_color = BLUE,
        .foreground_color = WHITE,
        .title = "Window manager",
        .debug = false,
    };
    initialize_buttons(&window_manager);

    Window calculator_window = {
        .x = 12,
        .y = 3,
        .len_x = 40,
        .len_y = 20,
        .background_color = BLUE,
        .foreground_color = WHITE,
        .title = "Calculator",
        .debug = false,
    };
    initialize_buttons(&calculator_window);

    Window file_explorer_window = {
        .x = 12,
        .y = 1,
        .len_x = 40,
        .len_y = 23,
        .background_color = BLUE,
        .foreground_color = WHITE,
        .title = "File explorer",
        .debug = false,
    };
    initialize_buttons(&file_explorer_window);

    Window notepad_window = {
        .x = 12,
        .y = 5,
        .len_x = 40,
        .len_y = 15,
        .background_color = BLUE,
        .foreground_color = WHITE,
        .title = "Notepad",
        .debug = false,
    };
    initialize_buttons(&notepad_window);

    Window paint_window = {
        .x = 5,
        .y = 5,
        .len_x = 55,
        .len_y = 17,
        .background_color = BLUE,
        .foreground_color = WHITE,
        .title = "Paint",
        .debug = false,
    };
    initialize_buttons(&paint_window);

    Window random_window = {
        .x = 12,
        .y = 5,
        .len_x = 40,
        .len_y = 15,
        .background_color = BLUE,
        .foreground_color = WHITE,
        .title = "Random",
        .debug = false,
    };
    initialize_buttons(&random_window);

    Window shell_window = {
        .x = 4,
        .y = 5,
        .len_x = 64,
        .len_y = 15,
        .background_color = BLUE,
        .foreground_color = WHITE,
        .title = "Shell",
        .debug = false,
    };
    initialize_buttons(&shell_window);

    Window web_browser_window = {
        .x = 12,
        .y = 5,
        .len_x = 40,
        .len_y = 15,
        .background_color = BLUE,
        .foreground_color = WHITE,
        .title = "Web browser",
        .debug = false,
    };
    initialize_buttons(&web_browser_window);

    /* ---------------------------------------------------- */
    /*                  Create buttons                      */
    /* ---------------------------------------------------- */

    char* programs_names[6] = { "Calc", "Explorer", "Notepad", 
                                "Paint", "Random", "Shell"};

    void (*p[6]) (Window*, Argument*) = {  calculator, file_explorer, notepad,
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
    
        Button program_button = create_button(
            window_manager.x + margin_x + (width + margin_x) * col - 10,
            window_manager.y + margin_y + (height + margin_y) * row,
            width, height, BLUE, WHITE, programs_names[i], p[i]);
        
        window_manager.elements.button[i] = program_button;
    }

    /* ------------ */

    char* digits[10] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};
    for (int i = 0; i < 10; i++) 
    {
        calculator_window.elements.button[i] = create_button(2 + (i % 3) * 9, 3 + (i / 3) * 3, 6, 2, BLUE, WHITE, digits[i], NULL);
    }
    
    calculator_window.elements.button[10] = create_button(28, 3, 6, 2, BLUE, WHITE, "+", NULL);
    calculator_window.elements.button[11] = create_button(28, 6, 6, 2, BLUE, WHITE, "-", NULL);
    calculator_window.elements.button[12] = create_button(28, 9, 6, 2, BLUE, WHITE, "*", NULL);
    calculator_window.elements.button[13] = create_button(28, 12, 6, 2, BLUE, WHITE, "/", NULL);
    calculator_window.elements.button[14] = create_button(2, 15, 17, 2, BLUE, WHITE, "Enter", NULL);
    calculator_window.elements.button[15] = create_button(22, 15, 17, 2, BLUE, WHITE, "Reset", NULL);

    /* ------------ */

    file_explorer_window.elements.button[0] = create_button(2, 3, 8, 2, BLUE, WHITE, "Search", NULL);
    file_explorer_window.elements.button[1] = create_button(12, 3, 6, 2, BLUE, WHITE, "Open", NULL);

    /* ------------ */

    notepad_window.elements.button[0] = create_button(2, 3, notepad_window.len_x - 4, 2, BLUE, WHITE, "Save", NULL);

    /* ------------ */

    for (int i = 0; i < 9; i++) 
    {
        uint16_t bg_colors[9] = {RED, YELLOW, GREEN, CYAN, BLACK, WHITE, LGRAY, BROWN, MAGENTA};
        uint16_t fg_colors[9] = {WHITE, BLACK, WHITE, WHITE, WHITE, BLACK, WHITE, WHITE, WHITE};

        paint_window.elements.button[i] = create_button(i * 5 + 2, 3, 3, 2, bg_colors[i], fg_colors[i], "1", NULL);
    }

    /* ------------ */

    random_window.elements.button[0] = create_button(2, 3, 10, 2, BLUE, WHITE, "Generate", NULL);

    /* ------------ */

    web_browser_window.elements.button[0] = create_button(2, 3, 9, 2, BLUE, WHITE, "Execute", NULL);
    web_browser_window.elements.button[1] = create_button(2, 3, 8, 2, BLUE, WHITE, "Search", NULL);


    /* ---------------------------------------------------- */
    /*                  Create arguments                    */
    /* ---------------------------------------------------- */

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

    /* ---------------------------------------------------- */
    /*                  Update windows                      */
    /* ---------------------------------------------------- */

    screen.windows[0] = &window_manager;
    screen.windows[1] = &calculator_window;
    screen.windows[2] = &file_explorer_window;
    screen.windows[3] = &notepad_window;
    screen.windows[4] = &paint_window;
    screen.windows[5] = &random_window;
    screen.windows[6] = &shell_window;

    /* ---------------------------------------------------- */
    /*                  Draw main window                    */
    /* ---------------------------------------------------- */

    if (screen.windows[0]->error == NO_ERROR)
    {
        draw_window(screen.windows[0]);
        screen.active_window = 0;
    }

    else
    {
        move_real_cursor(0, 0);
        printf("Error code %d of window %d\r\n", screen.windows[0]->error, 0);
    }

    draw_window_elements(screen.windows[0], 0);

    change_color(format_color(BLUE, LRED), (uint16_t)35);
    printf("===> Pelly Operating System - 1.0.0\r\n");

    change_color(format_color(BLUE, LGREN), (uint16_t)34);
    printf("Now with BBFS, GUI, heap & IVT! %d\r\n", 30);

keyboard_loop:
    move_real_cursor(0, 0);
    c_keyboard_loop(&screen);
}
