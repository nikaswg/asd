#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <ctype.h>

void clear_input_buffer() {
    while (getchar() != '\n');
}

int get_valid_number(const char* prompt) {
    int value;
    char ch;
    while (1) {
        printf("%s", prompt);
        if (scanf("%d", &value) != 1) {
            printf("������: ������� �����!\n");
            clear_input_buffer();
            continue;
        }
        
        // ���������, ��� ����� ����� ��� ������ �������� ����� ����������
        while ((ch = getchar()) != '\n') {
            if (!isspace(ch)) {
                printf("������: ������� ������ ����� ��� �������������� ��������!\n");
                clear_input_buffer();
                value = -1; // �������� ��� �������� ��������
                break;
            }
        }
        
        if (value <= 0) {
            printf("������: ����� ������ ���� �������������!\n");
        } else if (value != -1) {
            break;
        }
    }
    return value;
}

void func_newarray(int** a, int** arr, int* size, int m, int n) {
    int i, j, k = 0;
    *size = 0;

    // ������� ��������� ���������
    for (i = 0; i < m; i++) {
        for (j = 0; j < n; j++) {
            if (!((i + 1) % 2 == 0 && (j + 1) % 2 == 0)) { // ������ ������ � ������� - �������
                (*size)++;
            }
        }
    }

    // �������� ������
    *arr = (int*)malloc(*size * sizeof(int));
    if (*arr == NULL) {
        printf("������ ��������� ������!\n");
        exit(1);
    }

    // ��������� ����������� ������
    k = 0;
    for (i = 0; i < m; i++) {
        for (j = 0; j < n; j++) {
            if (!((i + 1) % 2 == 0 && (j + 1) % 2 == 0)) {
                (*arr)[k++] = a[i][j];
            }
        }
    }
}

int NewIndex(int m, int n, int i, int j) {
    int index = 0;
    for (int row = 0; row < i; row++) {
        for (int col = 0; col < n; col++) {
            if ((row + 1) % 2 == 0 && (col + 1) % 2 == 0) continue;
            index++;
        }
    }
    for (int col = 0; col < j; col++) {
        if ((i + 1) % 2 == 0 && (col + 1) % 2 == 0) continue;
        index++;
    }
    return index;
}

void get_val(int* arr, int size, int m, int n, int** a) {
    int i1, j1;
    
    while (1) {
        printf("\n������� i,j (����� ������): ");
        if (scanf("%d %d", &i1, &j1) != 2) {
            printf("������: ������� ��� ����� ����� ������!\n");
            clear_input_buffer();
            continue;
        }
        clear_input_buffer();
        break;
    }
    
    i1--; j1--;

    if (i1 >= m || j1 >= n || i1 < 0 || j1 < 0) {
        printf("����� �� ������� �������\n");
        return;
    }

    if ((i1 + 1) % 2 == 0 && (j1 + 1) % 2 == 0) {
        printf("������� �������\n");
    } else {
        int ind = NewIndex(m, n, i1, j1);
        if (ind >= size) {
            printf("������: ������ ������� �� ������� �������!\n");
        } else {
            printf("val=%d, ind=%d\n", arr[ind], ind);
        }
    }
}

void set_val(int** a, int* arr, int size, int m, int n) {
    int i1, j1, val;
    
    while (1) {
        printf("������� i,j (����� ������): ");
        if (scanf("%d %d", &i1, &j1) != 2) {
            printf("������: ������� ��� ����� ����� ������!\n");
            clear_input_buffer();
            continue;
        }
        clear_input_buffer();
        break;
    }
    
    i1--; j1--;

    if (i1 >= m || j1 >= n || i1 < 0 || j1 < 0) {
        printf("����� �� ������� �������\n");
        return;
    }

    if ((i1 + 1) % 2 == 0 && (j1 + 1) % 2 == 0) {
        printf("���������� �������� ������� �������!\n");
        return;
    }

    int ind = NewIndex(m, n, i1, j1);
    if (ind >= size) {
        printf("������: ������ ������� �� ������� �������!\n");
        return;
    }

    while (1) {
        printf("������� ����� ��������: ");
        if (scanf("%d", &val) != 1) {
            printf("������: ������� �����!\n");
            clear_input_buffer();
            continue;
        }
        clear_input_buffer();
        break;
    }
    
    a[i1][j1] = arr[ind] = val;

    printf("\n����� ������:\n");
    for (int i = 0; i < size; i++)
        printf("%d ", arr[i]);
    printf("\n����� �������:\n");
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++)
            printf("%d ", a[i][j]);
        printf("\n");
    }
}

void initialize_matrix(int** a, int m, int n) {
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if ((i + 1) % 2 == 0 && (j + 1) % 2 == 0) {
                a[i][j] = 0;
            } else {
                a[i][j] = rand() % 10 + 1;
            }
        }
    }
}

int main() {
    setlocale(LC_ALL, "Rus");

    int m, n, i, j;
    
    m = get_valid_number("������� ���������� ����� ������� m: ");
    n = get_valid_number("������� ���������� �������� ������� n: ");

    int** a = (int**)malloc(m * sizeof(int*));
    for (i = 0; i < m; i++)
        a[i] = (int*)malloc(n * sizeof(int));

    initialize_matrix(a, m, n);

    printf("\n�������� ������� %dx%d:\n", m, n);
    for (i = 0; i < m; i++) {
        for (j = 0; j < n; j++)
            printf("%d ", a[i][j]);
        printf("\n");
    }

    int* arr = NULL;
    int size = 0;

    func_newarray(a, &arr, &size, m, n);
    printf("\n����������� ������ (������ %d):\n", size);
    for (i = 0; i < size; i++)
        printf("%d ", arr[i]);
    printf("\n");

    while (1) {
        printf("\n����:\n");
        printf("1. �������� �������� ��������\n");
        printf("2. ���������� �������� ��������\n");
        printf("3. �����\n");
        
        int msg;
        while (1) {
            printf("�������� �������� (1-3): ");
            if (scanf("%d", &msg) != 1) {
                printf("������: ������� �����!\n");
                clear_input_buffer();
                continue;
            }
            clear_input_buffer();
            if (msg < 1 || msg > 3) {
                printf("������: ������� ����� �� 1 �� 3!\n");
                continue;
            }
            break;
        }
        
        if (msg == 1)
            get_val(arr, size, m, n, a);
        else if (msg == 2)
            set_val(a, arr, size, m, n);
        else
            break;
    }

    free(arr);
    for (i = 0; i < m; i++)
        free(a[i]);
    free(a);

    return 0;
}
