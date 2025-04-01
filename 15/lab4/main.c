#include "family_data.h"

// Функция для отображения меню
void display_menu() {
    printf("\nMenu:\n"); // Меню на английском
    printf("1. Load family data from file\n");
    printf("2. Display family list\n");
    printf("3. Add new family member\n");
    printf("4. Sort by number of siblings\n");
    printf("5. Sort by length of faculty\n");
    printf("6. Save family data to file\n");
    printf("7. Exit\n");
}

// Функция для интерактивного добавления нового члена семьи
void add_family_interactive(Family **head) {
    char surname[50], name[50], patronymic[50], faculty[50];
    char father_specialty[50], mother_specialty[50];
    int siblings_count;

    // Запрашиваем данные о новом члене семьи
    printf("Enter surname: "); // Запрос фамилии на английском
    scanf("%s", surname);
    printf("Enter name: "); // Запрос имени на английском
    scanf("%s", name);
    printf("Enter patronymic: "); // Запрос отчества на английском
    scanf("%s", patronymic);
    printf("Enter faculty: "); // Запрос факультета на английском
    scanf("%s", faculty);
    printf("Enter father's specialty: "); // Запрос специальности отца на английском
    scanf("%s", father_specialty);
    printf("Enter mother's specialty: "); // Запрос специальности матери на английском
    scanf("%s", mother_specialty);
    printf("Enter number of siblings: "); // Запрос количества братьев и сестер на английском
    scanf("%d", &siblings_count);

    // Добавляем нового члена семьи в список
    add_family(head, create_family(surname, name, patronymic, faculty, father_specialty, mother_specialty, siblings_count));
}

int main() {
    Family *family_list = NULL; // Указатель на список семей
    int choice; // Переменная для выбора меню
    char filename[100]; // Имя файла для хранения данных

    // Основной цикл программы
    while (1) {
        display_menu(); // Отображаем меню
        printf("Enter your choice: "); // Запрос выбора пользователя на английском
        scanf("%d", &choice); // Получаем выбор пользователя

        switch (choice) {
            case 1:
                printf("Enter filename to load: "); // Запрос имени файла для загрузки на английском
                scanf("%s", filename);
                family_list = load_from_file(filename); // Загружаем данные из файла
                if (!family_list) {
                    printf("Error loading data.\n"); // Сообщение об ошибке на английском
                }
                break;

            case 2:
                printf("Family List:\n"); // Заголовок списка семей на английском
                print_family(family_list); // Показываем список семей
                break;

            case 3:
                add_family_interactive(&family_list); // Добавляем нового члена семьи
                break;

            case 4:
                sort_by_siblings(&family_list); // Сортировка по количеству братьев и сестер
                printf("List sorted by number of siblings.\n"); // Сообщение о сортировке на английском
                break;

            case 5:
                sort_by_faculty_length(&family_list); // Сортировка по длине факультета
                printf("List sorted by length of faculty.\n"); // Сообщение о сортировке на английском
                break;

            case 6:
                printf("Enter filename to save: "); // Запрос имени файла для сохранения на английском
                scanf("%s", filename);
                save_to_file(family_list, filename); // Сохраняем данные в файл
                printf("Data saved to %s.\n", filename); // Сообщение о сохранении на английском
                break;

            case 7:
                free_list(family_list); // Освобождаем память
                return 0; // Завершаем программу

            default:
                printf("Invalid choice. Please try again.\n"); // Сообщение о неверном выборе на английском
        }
    }
}
