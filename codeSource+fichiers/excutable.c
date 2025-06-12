#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <dirent.h>

#define MAX_LINE 1024
#define COLOR_BLUE 9
#define COLOR_GREEN 10
#define COLOR_RED 12
#define COLOR_WHITE 15
#define COLOR_PURPLE 13
#define COLOR_RESET 7
#define MAX_FILES 100

typedef struct
{
    char filename[100];
} FormulaFile;

void clear_screen()
{
    system("cls");
}

void set_color(int color)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

void print_credits()
{
    set_color(COLOR_PURPLE);
    printf("\n");
    printf("╔════════════════════════════════════════════════╗\n");
    printf("║                                                ║\n");
    printf("║          PROPOSITIONAL LOGIC SOLVER            ║\n");
    printf("║                                                ║\n");
    printf("╚════════════════════════════════════════════════╝\n");

    set_color(COLOR_BLUE);
    printf("\n╔════════════════════════════════════════════════╗\n");
    printf("║               DEVELOPMENT TEAM                ║\n");
    set_color(COLOR_GREEN);
    printf("║      RAFA Houssam • BENYETTOU Imed            ║\n");
    printf("║      RACHEM Mohamed Riadh • ALIOUCHE Razine   ║\n");
    set_color(COLOR_BLUE);
    printf("╚════════════════════════════════════════════════╝\n");

    set_color(COLOR_PURPLE);
    printf("\n           ESI Logic Project           \n");
    printf("                 Academic Year 2024-2025         \n\n");
    set_color(COLOR_RESET);
}

void print_menu()
{
    set_color(COLOR_PURPLE);
    printf("\n╔════════════════════════════════════════════════╗\n");
    printf("║                 MAIN MENU                     ║\n");
    printf("╚════════════════════════════════════════════════╝\n\n");

    set_color(COLOR_BLUE);
    printf("   [1] ");
    set_color(COLOR_WHITE);
    printf("Create New Logical Formula\n");

    set_color(COLOR_BLUE);
    printf("   [2] ");
    set_color(COLOR_WHITE);
    printf("Test Existing Formula\n");

    set_color(COLOR_BLUE);
    printf("   [3] ");
    set_color(COLOR_WHITE);
    printf("Browse Formula Library\n");

    set_color(COLOR_RED);
    printf("   [4] ");
    set_color(COLOR_WHITE);
    printf("Exit Application\n\n");

    set_color(COLOR_PURPLE);
    printf("Select an option (1-4): ");
    set_color(COLOR_RESET);
}

void create_new_formula()
{
    char filename[100];
    int num_clauses;
    char input[MAX_LINE];
    FILE *file;

    clear_screen();
    print_credits();

    set_color(COLOR_PURPLE);
    printf("\n╔════════════════════════════════════════════════╗\n");
    printf("║            FORMULA CREATION            ║\n");
    printf("╚════════════════════════════════════════════════╝\n\n");
    set_color(COLOR_RESET);

    printf("Enter formula name (without extension): ");
    scanf("%s", filename);
    getchar();

    char full_filename[120];
    sprintf(full_filename, "%s.cnf", filename);

    file = fopen(full_filename, "w");
    if (!file)
    {
        set_color(COLOR_RED);
        printf("Error creating formula file!\n");
        set_color(COLOR_RESET);
        printf("\nPress Enter to continue...");
        getchar();
        return;
    }

    printf("Number of clauses in formula: ");
    scanf("%d", &num_clauses);
    getchar();

    fprintf(file, "# Propositional logic formula\n");

    for (int i = 0; i < num_clauses; i++)
    {
        set_color(COLOR_BLUE);
        printf("\nClause %d (literals separated by spaces):\n", i + 1);
        printf("Example: A !B C\n> ");
        set_color(COLOR_RESET);

        if (fgets(input, MAX_LINE, stdin))
        {
            input[strcspn(input, "\n")] = 0;
            fprintf(file, "%s\n", input);
        }
    }

    fclose(file);

    set_color(COLOR_GREEN);
    printf("\nFormula successfully saved as: %s\n", full_filename);
    set_color(COLOR_RESET);

    printf("\nTest this formula now? (y/n): ");
    char choice;
    scanf(" %c", &choice);
    getchar();

    if (choice == 'y' || choice == 'Y')
    {
        char command[200];
        sprintf(command, "solver_engine %s", full_filename);
        system(command);
    }

    printf("\nPress Enter to continue...");
    getchar();
}

void test_existing_formula()
{
    DIR *d;
    struct dirent *dir;
    FormulaFile files[MAX_FILES];
    int count = 0;

    clear_screen();
    print_credits();

    set_color(COLOR_PURPLE);
    printf("\n╔════════════════════════════════════════════════╗\n");
    printf("║              FORMULA VALIDATION               ║\n");
    printf("╚════════════════════════════════════════════════╝\n\n");
    set_color(COLOR_RESET);

    d = opendir(".");
    if (d)
    {
        printf("Available formulas:\n\n");
        while ((dir = readdir(d)) != NULL && count < MAX_FILES)
        {
            if (strstr(dir->d_name, ".cnf"))
            {
                strcpy(files[count].filename, dir->d_name);
                set_color(COLOR_BLUE);
                printf("[%d] ", count + 1);
                set_color(COLOR_WHITE);
                printf("%s\n", files[count].filename);
                count++;
            }
        }
        closedir(d);
    }

    if (count == 0)
    {
        set_color(COLOR_RED);
        printf("No formula files found in directory!\n");
        set_color(COLOR_RESET);
        printf("\nPress Enter to continue...");
        getchar();
        return;
    }

    printf("\nSelect formula to test (1-%d): ", count);
    int choice;
    while (scanf("%d", &choice) != 1 || choice < 1 || choice > count)
    {
        set_color(COLOR_RED);
        printf("Invalid selection! Enter 1-%d: ", count);
        set_color(COLOR_RESET);
        while (getchar() != '\n')
            ;
    }
    getchar();

    char command[200];
    sprintf(command, "solver_engine %s", files[choice - 1].filename);
    system(command);

    printf("\nPress Enter to continue...");
    getchar();
}

void show_formulas()
{
    DIR *d;
    struct dirent *dir;
    FormulaFile files[MAX_FILES];
    int count = 0;

    while (1)
    {
        clear_screen();
        print_credits();

        set_color(COLOR_PURPLE);
        printf("\n╔════════════════════════════════════════════════╗\n");
        printf("║              FORMULA LIBRARY                 ║\n");
        printf("╚════════════════════════════════════════════════╝\n\n");
        set_color(COLOR_RESET);

        d = opendir(".");
        if (d)
        {
            while ((dir = readdir(d)) != NULL && count < MAX_FILES)
            {
                if (strstr(dir->d_name, ".cnf"))
                {
                    strcpy(files[count].filename, dir->d_name);
                    set_color(COLOR_BLUE);
                    printf("[%d] ", count + 1);
                    set_color(COLOR_WHITE);
                    printf("%s\n", files[count].filename);
                    count++;
                }
            }
            closedir(d);
        }

        if (count == 0)
        {
            set_color(COLOR_RED);
            printf("Formula library is empty!\n");
            set_color(COLOR_RESET);
        }

        printf("\nOptions:\n");
        set_color(COLOR_BLUE);
        printf("[1] ");
        set_color(COLOR_WHITE);
        printf("Validate selected formula\n");
        set_color(COLOR_RED);
        printf("[2] ");
        set_color(COLOR_WHITE);
        printf("Return to main menu\n\n");

        printf("Enter selection: ");
        char menu_choice;
        scanf(" %c", &menu_choice);
        getchar();

        if (menu_choice == '1')
        {
            if (count > 0)
            {
                printf("\nSelect formula (1-%d): ", count);
                int file_choice;
                while (scanf("%d", &file_choice) != 1 || file_choice < 1 || file_choice > count)
                {
                    set_color(COLOR_RED);
                    printf("Invalid selection! Enter 1-%d: ", count);
                    set_color(COLOR_RESET);
                    while (getchar() != '\n')
                        ;
                }
                getchar();

                char command[200];
                sprintf(command, "solver_engine %s", files[file_choice - 1].filename);
                system(command);

                printf("\nPress Enter to continue...");
                getchar();
            }
            else
            {
                set_color(COLOR_RED);
                printf("\nNo formulas available for testing!\n");
                set_color(COLOR_RESET);
                printf("\nPress Enter to continue...");
                getchar();
            }
        }
        else if (menu_choice == '2')
        {
            return;
        }
        count = 0;
    }
}

int main()
{
    SetConsoleOutputCP(CP_UTF8);
    char choice;

    while (1)
    {
        clear_screen();
        print_credits();
        print_menu();

        scanf(" %c", &choice);
        getchar();

        switch (choice)
        {
        case '1':
            create_new_formula();
            break;
        case '2':
            test_existing_formula();
            break;
        case '3':
            show_formulas();
            break;
        case '4':
            clear_screen();
            print_credits();
            set_color(COLOR_GREEN);
            printf("\nThank you for using the Propositional Logic Solver!\n");
            set_color(COLOR_RESET);
            return 0;
        default:
            set_color(COLOR_RED);
            printf("\nInvalid option! Press Enter to continue...");
            set_color(COLOR_RESET);
            getchar();
        }
    }

    return 0;
}