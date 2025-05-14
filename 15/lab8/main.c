#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#define MAX_CHARS 256 // Максимальное количество различных символов (ASCII)

// Структура узла бинарного дерева
typedef struct Node {
    char symbol;       // Символ, хранящийся в узле
    int level;         // Уровень вложенности узла (0 для корня)
    struct Node *left;  // Указатель на левое поддерево
    struct Node *right; // Указатель на правое поддерево
} Node;

// Структура для хранения частоты символов (используется при подсчете)
typedef struct {
    char symbol; // Символ
    int count;   // Количество вхождений
} CharCount;

/* 
 * Создание нового узла дерева
 * Параметры:
 *   - symbol: символ для хранения в узле
 *   - level: уровень вложенности узла
 * Возвращает:
 *   - указатель на созданный узел
 */
Node* createNode(char symbol, int level) {
    // Выделяем память под новый узел
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    
    // Инициализируем поля узла
    newNode->symbol = symbol;
    newNode->level = level;
    newNode->left = newNode->right = NULL;
    
    return newNode;
}

/*
 * Вставка символа в бинарное дерево поиска
 * Параметры:
 *   - root: корень дерева (или поддерева)
 *   - symbol: символ для вставки
 *   - level: текущий уровень вложенности
 * Возвращает:
 *   - указатель на корень дерева (может измениться при вставке нового корня)
 */
Node* insert(Node* root, char symbol, int level) {
    // Если дерево пустое, создаем новый узел
    if (root == NULL) {
        return createNode(symbol, level);
    }
    
    // Сравниваем символы для определения направления вставки
    if (symbol < root->symbol) {
        // Вставляем в левое поддерево с увеличенным уровнем
        root->left = insert(root->left, symbol, level + 1);
    } else {
        // Вставляем в правое поддерево с увеличенным уровнем
        root->right = insert(root->right, symbol, level + 1);
    }
    
    return root;
}

/*
 * Подсчет частоты символов в дереве
 * Параметры:
 *   - root: корень дерева (или поддерева)
 *   - counts: массив для хранения количества вхождений каждого символа
 */
void countSymbols(Node* root, int counts[]) {
    if (root == NULL) return; // Базовый случай рекурсии
    
    // Увеличиваем счетчик для текущего символа
    counts[(unsigned char)root->symbol]++;
    
    // Рекурсивно обходим левое и правое поддеревья
    countSymbols(root->left, counts);
    countSymbols(root->right, counts);
}

/*
 * Визуальный вывод дерева в консоль (вертикальная ориентация)
 * Параметры:
 *   - root: корень дерева (или поддерева)
 *   - space: количество пробелов для отступа
 */
void printTree(Node* root, int space) {
    if (root == NULL) return; // Базовый случай рекурсии
    
    // Увеличиваем отступ для следующего уровня
    space += 5;
    
    // Сначала выводим правое поддерево (оно будет сверху)
    printTree(root->right, space);
    
    // Выводим текущий узел
    printf("\n");
    for (int i = 5; i < space; i++)
        printf(" ");
    printf("%c(L%d)\n", root->symbol, root->level);
    
    // Затем выводим левое поддерево (оно будет снизу)
    printTree(root->left, space);
}

/*
 * Освобождение памяти, занятой деревом
 * Параметры:
 *   - root: корень дерева (или поддерева)
 */
void freeTree(Node* root) {
    if (root == NULL) return; // Базовый случай рекурсии
    
    // Рекурсивно освобождаем левое и правое поддеревья
    freeTree(root->left);
    freeTree(root->right);
    
    // Освобождаем текущий узел
    free(root);
}

/*
 * Удаление поддерева с заданным символом и уровнем
 * Параметры:
 *   - root: корень дерева (или поддерева)
 *   - symbol: символ, по которому ищем поддерево для удаления
 *   - level: уровень, на котором должен находиться удаляемый узел
 * Возвращает:
 *   - указатель на корень дерева (может измениться при удалении)
 */
Node* deleteSubtree(Node* root, char symbol, int level) {
    if (root == NULL) return NULL; // Базовый случай рекурсии
    
    if (symbol < root->symbol) {
        // Ищем в левом поддереве
        root->left = deleteSubtree(root->left, symbol, level);
    } else if (symbol > root->symbol) {
        // Ищем в правом поддереве
        root->right = deleteSubtree(root->right, symbol, level);
    } else {
        // Нашли узел с нужным символом
        if (root->level == level) {
            // Удаляем поддерево с корнем в этом узле
            freeTree(root);
            return NULL;
        }
        // Если уровень не совпадает, продолжаем поиск в правом поддереве
        root->right = deleteSubtree(root->right, symbol, level);
    }
    
    return root;
}

/*
 * Сохранение дерева в файл (префиксный обход)
 * Параметры:
 *   - root: корень дерева (или поддерева)
 *   - file: указатель на открытый файл для записи
 */
void saveTree(Node* root, FILE* file) {
    if (root == NULL) {
        // Используем '#' для обозначения NULL
        fprintf(file, "# ");
        return;
    }
    
    // Сохраняем данные узла (символ и уровень)
    fprintf(file, "%c %d ", root->symbol, root->level);
    
    // Рекурсивно сохраняем левое и правое поддеревья
    saveTree(root->left, file);
    saveTree(root->right, file);
}

/*
 * Загрузка дерева из файла
 * Параметры:
 *   - file: указатель на открытый файл для чтения
 * Возвращает:
 *   - указатель на корень загруженного дерева
 */
Node* loadTree(FILE* file) {
    char token[10];
    
    // Читаем первый токен
    if (fscanf(file, "%s", token) != 1)
        return NULL;
    
    // Если токен '#', значит это NULL
    if (strcmp(token, "#") == 0)
        return NULL;
    
    // Первый токен - символ, следующий - уровень
    char symbol = token[0];
    int level;
    fscanf(file, "%d", &level);
    
    // Создаем узел и рекурсивно загружаем поддеревья
    Node* node = createNode(symbol, level);
    node->left = loadTree(file);
    node->right = loadTree(file);
    
    return node;
}

/*
 * Поиск и вывод символов с минимальной частотой встречаемости
 * Параметры:
 *   - root: корень дерева (или поддерева)
 */
void findMinFrequency(Node* root) {
    if (root == NULL) {
        printf("Tree is empty.\n");
        return;
    }
    
    // Инициализируем массив для подсчета частот
    int counts[MAX_CHARS] = {0};
    
    // Подсчитываем частоту всех символов в дереве
    countSymbols(root, counts);
    
    // Находим минимальную частоту
    int minFreq = INT_MAX;
    for (int i = 0; i < MAX_CHARS; i++) {
        if (counts[i] > 0 && counts[i] < minFreq) {
            minFreq = counts[i];
        }
    }
    
    // Выводим символы с минимальной частотой
    printf("\nCharacters with the minimum frequency (%d):\n", minFreq);
    for (int i = 0; i < MAX_CHARS; i++) {
        if (counts[i] == minFreq) {
            printf("'%c' occurs %d time(s)\n", (char)i, minFreq);
        }
    }
}

/*
 * Построение дерева из файла
 * Параметры:
 *   - filename: имя файла для чтения
 * Возвращает:
 *   - указатель на корень построенного дерева
 */
Node* buildTreeFromFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file: %s\n", filename);
        return NULL;
    }
    
    Node* root = NULL;
    char ch;
    
    // Читаем файл посимвольно
    while ((ch = fgetc(file)) != EOF) {
        if (isalpha(ch)) {
            // Приводим к нижнему регистру и вставляем в дерево
            ch = tolower(ch);
            root = insert(root, ch, 0);
        }
    }
    
    fclose(file);
    return root;
}

/*
 * Отображение меню программы
 */
void displayMenu() {
    printf("\n========== MENU ==========\n");
    printf("1. Build tree from file\n");
    printf("2. Print tree\n");
    printf("3. Find characters with minimal frequency\n");
    printf("4. Delete subtree\n");
    printf("5. Save tree to file\n");
    printf("6. Load tree from file\n");
    printf("7. Exit\n");
    printf("=========================\n");
    printf("Enter your choice: ");
}

/*
 * Основная функция программы
 */
int main() {
    Node* root = NULL; // Корень дерева (изначально пустой)
    char filename[100]; // Буфер для имени файла
    int choice; // Выбор пользователя в меню
    
    do {
        displayMenu();
        scanf("%d", &choice);
        getchar(); // Очищаем буфер ввода
        
        switch (choice) {
            case 1: { // Построение дерева из файла
                printf("Enter filename: ");
                fgets(filename, sizeof(filename), stdin);
                filename[strcspn(filename, "\n")] = '\0'; // Удаляем символ новой строки
                
                root = buildTreeFromFile(filename);
                if (root != NULL) {
                    printf("Tree built successfully.\n");
                }
                break;
            }
            case 2: { // Вывод дерева
                if (root != NULL) {
                    printf("Binary Tree (by characters):\n");
                    printTree(root, 0);
                } else {
                    printf("Tree is empty. Build a tree first.\n");
                }
                break;
            }
            case 3: { // Поиск символов с минимальной частотой
                if (root != NULL) {
                    findMinFrequency(root);
                } else {
                    printf("Tree is empty. Build a tree first.\n");
                }
                break;
            }
            case 4: { // Удаление поддерева
                if (root != NULL) {
                    char symbol;
                    int level;
                    
                    printf("Enter character to delete: ");
                    scanf("%c", &symbol);
                    getchar(); // Очищаем буфер ввода
                    
                    printf("Enter level (0 for root): ");
                    scanf("%d", &level);
                    getchar(); // Очищаем буфер ввода
                    
                    root = deleteSubtree(root, tolower(symbol), level);
                    printf("Subtree deleted.\n");
                } else {
                    printf("Tree is empty. Build a tree first.\n");
                }
                break;
            }
            case 5: { // Сохранение дерева в файл
                if (root != NULL) {
                    printf("Enter filename to save: ");
                    fgets(filename, sizeof(filename), stdin);
                    filename[strcspn(filename, "\n")] = '\0';
                    
                    FILE* outFile = fopen(filename, "w");
                    if (outFile != NULL) {
                        saveTree(root, outFile);
                        fclose(outFile);
                        printf("Tree saved to '%s' successfully.\n", filename);
                    } else {
                        printf("Error opening file for writing.\n");
                    }
                } else {
                    printf("Tree is empty. Build a tree first.\n");
                }
                break;
            }
            case 6: { // Загрузка дерева из файла
                printf("Enter filename to load: ");
                fgets(filename, sizeof(filename), stdin);
                filename[strcspn(filename, "\n")] = '\0';
                
                FILE* inFile = fopen(filename, "r");
                if (inFile != NULL) {
                    Node* newTree = loadTree(inFile);
                    fclose(inFile);
                    
                    if (newTree != NULL) {
                        // Освобождаем старое дерево, если оно есть
                        if (root != NULL) {
                            freeTree(root);
                        }
                        root = newTree;
                        printf("Tree loaded from '%s' successfully.\n", filename);
                    } else {
                        printf("Error loading tree from file.\n");
                    }
                } else {
                    printf("Error opening file for reading.\n");
                }
                break;
            }
            case 7: { // Выход из программы
                printf("Exiting program.\n");
                break;
            }
            default: { // Неверный выбор
                printf("Invalid choice. Please try again.\n");
                break;
            }
        }
    } while (choice != 7);
    
    // Освобождаем память перед выходом
    if (root != NULL) {
        freeTree(root);
    }
    
    return 0;
}
