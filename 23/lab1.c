#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <time.h>

// Инициализация матрицы
void initialize_matrix(int** matrix, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            // Первая и последняя строки полностью заполнены
            if (i == 0 || i == n-1) {
                matrix[i][j] = rand() % 9 + 1;
            }
            // Вторая и предпоследняя строки
            else if (i == 1 || i == n-2) {
                if (j < 1 || j > n-2) {
                    matrix[i][j] = 0;
                } else {
                    matrix[i][j] = rand() % 9 + 1;
                }
            }
            // Остальные строки
            else {
                if (j < i || j >= n - i) {
                    matrix[i][j] = 0;
                } else {
                    matrix[i][j] = rand() % 9 + 1;
                }
            }
        }
    }
}

// Вывод матрицы
void print_matrix(int** matrix, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%2d ", matrix[i][j]);
        }
        printf("\n");
    }
}

// Создание упакованного массива
void create_packed_array(int** matrix, int** packed, int* size, int n) {
    *size = 0;
    // Подсчет значимых элементов
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (matrix[i][j] != 0) {
                (*size)++;
            }
        }
    }
    
    // Выделение памяти
    *packed = (int*)malloc(*size * sizeof(int));
    
    // Заполнение массива
    int index = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (matrix[i][j] != 0) {
                (*packed)[index++] = matrix[i][j];
            }
        }
    }
}

// Получение индекса в упакованном массиве
int get_packed_index(int** matrix, int n, int row, int col) {
    int index = 0;
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < n; j++) {
            if (matrix[i][j] != 0) {
                index++;
            }
        }
    }
    for (int j = 0; j < col; j++) {
        if (matrix[row][j] != 0) {
            index++;
        }
    }
    return index;
}

// Получение значения элемента
void get_element(int* packed, int** matrix, int n) {
    int row, col;
    printf("Введите индексы (строка столбец): ");
    scanf("%d %d", &row, &col);
    
    if (row < 0 || row >= n || col < 0 || col >= n) {
        printf("Ошибка: индексы за пределами матрицы\n");
        return;
    }
    
    if (matrix[row][col] == 0) {
        printf("Это фоновый элемент (0)\n");
    } else {
        int index = get_packed_index(matrix, n, row, col);
        printf("Значение: %d, Индекс в упакованном массиве: %d\n", packed[index], index);
    }
}

// Установка значения элемента
void set_element(int* packed, int** matrix, int n, int* packed_size) {
    int row, col, value;
    printf("Введите индексы (строка столбец): ");
    scanf("%d %d", &row, &col);
    
    if (row < 0 || row >= n || col < 0 || col >= n) {
        printf("Ошибка: индексы за пределами матрицы\n");
        return;
    }
    
    if (matrix[row][col] == 0) {
        printf("Ошибка: нельзя изменить фоновый элемент\n");
        return;
    }
    
    printf("Введите новое значение: ");
    scanf("%d", &value);
    
    int index = get_packed_index(matrix, n, row, col);
    matrix[row][col] = packed[index] = value;
    
    printf("Значение успешно изменено\n");
}

int main() {
    setlocale(LC_ALL, "Russian");
    srand(time(NULL));
    
    int n;
    printf("Введите размер матрицы: ");
    scanf("%d", &n);
    
    // Выделение памяти для матрицы
    int** matrix = (int**)malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++) {
        matrix[i] = (int*)malloc(n * sizeof(int));
    }
    
    // Инициализация матрицы
    initialize_matrix(matrix, n);
    
    // Вывод матрицы
    printf("\nИсходная матрица:\n");
    print_matrix(matrix, n);
    
    // Создание упакованного массива
    int* packed = NULL;
    int packed_size = 0;
    create_packed_array(matrix, &packed, &packed_size, n);
    
    // Вывод упакованного массива
    printf("\nУпакованный массив:\n");
    for (int i = 0; i < packed_size; i++) {
        printf("%d ", packed[i]);
    }
    printf("\n");
    
    // Меню
    int choice;
    do {
        printf("\nМеню:\n");
        printf("1. Получить значение элемента\n");
        printf("2. Установить значение элемента\n");
        printf("3. Вывести матрицу\n");
        printf("4. Вывести упакованный массив\n");
        printf("5. Выход\n");
        printf("Выберите действие: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                get_element(packed, matrix, n);
                break;
            case 2:
                set_element(packed, matrix, n, &packed_size);
                break;
            case 3:
                printf("\nТекущая матрица:\n");
                print_matrix(matrix, n);
                break;
            case 4:
                printf("\nУпакованный массив:\n");
                for (int i = 0; i < packed_size; i++) {
                    printf("%d ", packed[i]);
                }
                printf("\n");
                break;
            case 5:
                printf("Выход...\n");
                break;
            default:
                printf("Неверный выбор!\n");
        }
    } while (choice != 5);
    
    // Освобождение памяти
    for (int i = 0; i < n; i++) {
        free(matrix[i]);
    }
    free(matrix);
    free(packed);
    
    return 0;
}