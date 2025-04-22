#include "family_data.h"

// Функция отображения меню
void display_menu() {
    printf("\nMenu:\n");
    printf("1. Load family data from file\n");
    printf("2. Display family list\n");
    printf("3. Add new family member\n");
    printf("4. Edit family member\n");
    printf("5. Delete family member\n");
    printf("6. Sort by number of siblings\n");
    printf("7. Sort by length of faculty\n");
    printf("8. Save family data to file\n");
    printf("9. Exit\n");
}

// Функция для добавления члена семьи с вводом с клавиатуры
void add_family_interactive(Family **head) {
    char surname[50], name[50], patronymic[50], faculty[50];
    char father_specialty[50], mother_specialty[50];
    int siblings_count;

    printf("Enter surname: ");
    scanf("%s", surname);
    printf("Enter name: ");
    scanf("%s", name);
    printf("Enter patronymic: ");
    scanf("%s", patronymic);
    printf("Enter faculty: ");
    scanf("%s", faculty);
    printf("Enter father's specialty: ");
    scanf("%s", father_specialty);
    printf("Enter mother's specialty: ");
    scanf("%s", mother_specialty);
    printf("Enter number of siblings: ");
    scanf("%d", &siblings_count);

    add_family(head, create_family(surname, name, patronymic, faculty, father_specialty, mother_specialty, siblings_count));
}

int main() {
    Family *family_list = NULL;
    int choice;
    char filename[100];
    char surname[50], name[50];

    while (1) {
        display_menu();
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter filename to load: ");
                scanf("%s", filename);
                family_list = load_from_file(filename);
                if (!family_list) {
                    printf("Error loading data.\n");
                }
                break;

            case 2:
                printf("Family List:\n");
                print_family(family_list);
                break;

            case 3:
                add_family_interactive(&family_list);
                break;

            case 4:
                printf("Enter surname of the member to edit: ");
                scanf("%s", surname);
                printf("Enter name of the member to edit: ");
                scanf("%s", name);
                edit_family(family_list, surname, name);
                break;

            case 5:
                printf("Enter surname of the member to delete: ");
                scanf("%s", surname);
                printf("Enter name of the member to delete: ");
                scanf("%s", name);
                delete_family(&family_list, surname, name);
                break;

            case 6:
                sort_by_siblings(&family_list);
                printf("List sorted by number of siblings.\n");
                break;

            case 7:
                sort_by_faculty_length(&family_list);
                printf("List sorted by length of faculty.\n");
                break;

            case 8:
                printf("Enter filename to save: ");
                scanf("%s", filename);
                save_to_file(family_list, filename);
                printf("Data saved to %s.\n", filename);
                break;

            case 9:
                free_list(family_list);
                return 0;

            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}
