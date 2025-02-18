#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

void func_newarray(int** a, int** arr, int* size, int n) {
    int i, j, k = 0;
    *size = 0; // Сброс размера
    // Считаем количество ненулевых элементов
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            if (a[i][j] != 0) {
                (*size)++;
            }
        }
    }
    // Выделяем память для упакованного вектора
    *arr = (int*)malloc(*size * sizeof(int));
    
    // Заполняем упакованный вектор
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            if (a[i][j] != 0) {
                (*arr)[k] = a[i][j];
                k++;
            }
        }
    }
}

int NewIndex(int** a, int n, int i, int j) {
    int index = 0;
    for (int row = 0; row < i; row++) {
        for (int col = 0; col < n; col++) {
            if (a[row][col] != 0)
                index++;
        }
    }
    for (int col = 0; col < j; col++) {
        if (a[i][col] != 0)
            index++;
    }
    return index;
}

void get_val(int* arr, int size, int n, int** a) {
    int i1, j1;
    printf("\nВведите i,j :\n");
    scanf("%d %d", &i1, &j1);
    if (i1 >= n || j1 >= n) {
        printf("Выход за пределы размера матрицы");
    } else {
        int ind = NewIndex(a, n, i1, j1);
        if (a[i1][j1] == 0)
            printf("фоновый элемент\n");
        else
            printf("val=%d, ind=%d\n", arr[ind], ind);
    }
}

void set_val(int** a, int* arr, int size, int n) {
    int i1, j1;
    printf("Введите i,j : ");
    scanf("%d %d", &i1, &j1);
    if (i1 >= n || j1 >= n) {
        printf("Выход за пределы размера матрицы");
    } else {
        int ind = NewIndex(a, n, i1, j1);
        printf("Введите новое значение  : ");
        int val;
        scanf("%d", &val);
        a[i1][j1] = arr[ind] = val;

        printf("\nНовый вектор \n");
        for (int i = 0; i < size; i++)
            printf("%d ", arr[i]);
        printf("\nНовая матрица\n");
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++)
                printf("%d ", a[i][j]);
            printf("\n");
        }
    }
}

int main() {
    setlocale(LC_ALL, "Rus");

    int n, i, j;
    printf("Введите размерность матрицы n :");
    scanf("%d", &n);

    int** a = (int**)malloc(n * sizeof(int*));
    for (i = 0; i < n; i++)
        a[i] = (int*)malloc(n * sizeof(int));

    // Инициализируем матрицу
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            if (i < j && i % 2 == 0) { // Четные строки выше диагонали
                a[i][j] = 0;
            } else if (i > j && i % 2 == 1) { // Нечетные строки ниже диагонали
                a[i][j] = 0;
            } else { // Заполнение случайными элементами
                a[i][j] = rand() % 10 + 1;
            }
        }
    }

    printf("Исходная матрица :\n");
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++)
            printf("%d ", a[i][j]);
        printf("\n");
    }

    int* arr = NULL;
    int size = 0;

    func_newarray(a, &arr, &size, n);
    printf("\nУпакованный вектор\n");
    for (i = 0; i < size; i++)
        printf("%d ", arr[i]);

    while (1) {
        printf("\n1.Получить значение элемента массива из ее упакованного представления по двум индексам\n");
        printf("2.Установить значения элемента массива в ее упакованное представление по двум индексам.\n");
        printf("3.Выход\n");
        int msg;
        scanf("%d", &msg);
        if (msg == 1)
            get_val(arr, size, n, a);
        else if (msg == 2)
            set_val(a, arr, size, n);
        else break;
    }

    free(arr);
    for (i = 0; i < n; i++)
        free(a[i]);
    free(a);

    return 0;
}