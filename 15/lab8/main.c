#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#define MAX_CHARS 256 // ������������ ���������� ��������� �������� (ASCII)

// ��������� ���� ��������� ������
typedef struct Node {
    char symbol;       // ������, ���������� � ����
    int level;         // ������� ����������� ���� (0 ��� �����)
    struct Node *left;  // ��������� �� ����� ���������
    struct Node *right; // ��������� �� ������ ���������
} Node;

// ��������� ��� �������� ������� �������� (������������ ��� ��������)
typedef struct {
    char symbol; // ������
    int count;   // ���������� ���������
} CharCount;

/* 
 * �������� ������ ���� ������
 * ���������:
 *   - symbol: ������ ��� �������� � ����
 *   - level: ������� ����������� ����
 * ����������:
 *   - ��������� �� ��������� ����
 */
Node* createNode(char symbol, int level) {
    // �������� ������ ��� ����� ����
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    
    // �������������� ���� ����
    newNode->symbol = symbol;
    newNode->level = level;
    newNode->left = newNode->right = NULL;
    
    return newNode;
}

/*
 * ������� ������� � �������� ������ ������
 * ���������:
 *   - root: ������ ������ (��� ���������)
 *   - symbol: ������ ��� �������
 *   - level: ������� ������� �����������
 * ����������:
 *   - ��������� �� ������ ������ (����� ���������� ��� ������� ������ �����)
 */
Node* insert(Node* root, char symbol, int level) {
    // ���� ������ ������, ������� ����� ����
    if (root == NULL) {
        return createNode(symbol, level);
    }
    
    // ���������� ������� ��� ����������� ����������� �������
    if (symbol < root->symbol) {
        // ��������� � ����� ��������� � ����������� �������
        root->left = insert(root->left, symbol, level + 1);
    } else {
        // ��������� � ������ ��������� � ����������� �������
        root->right = insert(root->right, symbol, level + 1);
    }
    
    return root;
}

/*
 * ������� ������� �������� � ������
 * ���������:
 *   - root: ������ ������ (��� ���������)
 *   - counts: ������ ��� �������� ���������� ��������� ������� �������
 */
void countSymbols(Node* root, int counts[]) {
    if (root == NULL) return; // ������� ������ ��������
    
    // ����������� ������� ��� �������� �������
    counts[(unsigned char)root->symbol]++;
    
    // ���������� ������� ����� � ������ ����������
    countSymbols(root->left, counts);
    countSymbols(root->right, counts);
}

/*
 * ���������� ����� ������ � ������� (������������ ����������)
 * ���������:
 *   - root: ������ ������ (��� ���������)
 *   - space: ���������� �������� ��� �������
 */
void printTree(Node* root, int space) {
    if (root == NULL) return; // ������� ������ ��������
    
    // ����������� ������ ��� ���������� ������
    space += 5;
    
    // ������� ������� ������ ��������� (��� ����� ������)
    printTree(root->right, space);
    
    // ������� ������� ����
    printf("\n");
    for (int i = 5; i < space; i++)
        printf(" ");
    printf("%c(L%d)\n", root->symbol, root->level);
    
    // ����� ������� ����� ��������� (��� ����� �����)
    printTree(root->left, space);
}

/*
 * ������������ ������, ������� �������
 * ���������:
 *   - root: ������ ������ (��� ���������)
 */
void freeTree(Node* root) {
    if (root == NULL) return; // ������� ������ ��������
    
    // ���������� ����������� ����� � ������ ����������
    freeTree(root->left);
    freeTree(root->right);
    
    // ����������� ������� ����
    free(root);
}

/*
 * �������� ��������� � �������� �������� � �������
 * ���������:
 *   - root: ������ ������ (��� ���������)
 *   - symbol: ������, �� �������� ���� ��������� ��� ��������
 *   - level: �������, �� ������� ������ ���������� ��������� ����
 * ����������:
 *   - ��������� �� ������ ������ (����� ���������� ��� ��������)
 */
Node* deleteSubtree(Node* root, char symbol, int level) {
    if (root == NULL) return NULL; // ������� ������ ��������
    
    if (symbol < root->symbol) {
        // ���� � ����� ���������
        root->left = deleteSubtree(root->left, symbol, level);
    } else if (symbol > root->symbol) {
        // ���� � ������ ���������
        root->right = deleteSubtree(root->right, symbol, level);
    } else {
        // ����� ���� � ������ ��������
        if (root->level == level) {
            // ������� ��������� � ������ � ���� ����
            freeTree(root);
            return NULL;
        }
        // ���� ������� �� ���������, ���������� ����� � ������ ���������
        root->right = deleteSubtree(root->right, symbol, level);
    }
    
    return root;
}

/*
 * ���������� ������ � ���� (���������� �����)
 * ���������:
 *   - root: ������ ������ (��� ���������)
 *   - file: ��������� �� �������� ���� ��� ������
 */
void saveTree(Node* root, FILE* file) {
    if (root == NULL) {
        // ���������� '#' ��� ����������� NULL
        fprintf(file, "# ");
        return;
    }
    
    // ��������� ������ ���� (������ � �������)
    fprintf(file, "%c %d ", root->symbol, root->level);
    
    // ���������� ��������� ����� � ������ ����������
    saveTree(root->left, file);
    saveTree(root->right, file);
}

/*
 * �������� ������ �� �����
 * ���������:
 *   - file: ��������� �� �������� ���� ��� ������
 * ����������:
 *   - ��������� �� ������ ������������ ������
 */
Node* loadTree(FILE* file) {
    char token[10];
    
    // ������ ������ �����
    if (fscanf(file, "%s", token) != 1)
        return NULL;
    
    // ���� ����� '#', ������ ��� NULL
    if (strcmp(token, "#") == 0)
        return NULL;
    
    // ������ ����� - ������, ��������� - �������
    char symbol = token[0];
    int level;
    fscanf(file, "%d", &level);
    
    // ������� ���� � ���������� ��������� ����������
    Node* node = createNode(symbol, level);
    node->left = loadTree(file);
    node->right = loadTree(file);
    
    return node;
}

/*
 * ����� � ����� �������� � ����������� �������� �������������
 * ���������:
 *   - root: ������ ������ (��� ���������)
 */
void findMinFrequency(Node* root) {
    if (root == NULL) {
        printf("Tree is empty.\n");
        return;
    }
    
    // �������������� ������ ��� �������� ������
    int counts[MAX_CHARS] = {0};
    
    // ������������ ������� ���� �������� � ������
    countSymbols(root, counts);
    
    // ������� ����������� �������
    int minFreq = INT_MAX;
    for (int i = 0; i < MAX_CHARS; i++) {
        if (counts[i] > 0 && counts[i] < minFreq) {
            minFreq = counts[i];
        }
    }
    
    // ������� ������� � ����������� ��������
    printf("\nCharacters with the minimum frequency (%d):\n", minFreq);
    for (int i = 0; i < MAX_CHARS; i++) {
        if (counts[i] == minFreq) {
            printf("'%c' occurs %d time(s)\n", (char)i, minFreq);
        }
    }
}

/*
 * ���������� ������ �� �����
 * ���������:
 *   - filename: ��� ����� ��� ������
 * ����������:
 *   - ��������� �� ������ ������������ ������
 */
Node* buildTreeFromFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file: %s\n", filename);
        return NULL;
    }
    
    Node* root = NULL;
    char ch;
    
    // ������ ���� �����������
    while ((ch = fgetc(file)) != EOF) {
        if (isalpha(ch)) {
            // �������� � ������� �������� � ��������� � ������
            ch = tolower(ch);
            root = insert(root, ch, 0);
        }
    }
    
    fclose(file);
    return root;
}

/*
 * ����������� ���� ���������
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
 * �������� ������� ���������
 */
int main() {
    Node* root = NULL; // ������ ������ (���������� ������)
    char filename[100]; // ����� ��� ����� �����
    int choice; // ����� ������������ � ����
    
    do {
        displayMenu();
        scanf("%d", &choice);
        getchar(); // ������� ����� �����
        
        switch (choice) {
            case 1: { // ���������� ������ �� �����
                printf("Enter filename: ");
                fgets(filename, sizeof(filename), stdin);
                filename[strcspn(filename, "\n")] = '\0'; // ������� ������ ����� ������
                
                root = buildTreeFromFile(filename);
                if (root != NULL) {
                    printf("Tree built successfully.\n");
                }
                break;
            }
            case 2: { // ����� ������
                if (root != NULL) {
                    printf("Binary Tree (by characters):\n");
                    printTree(root, 0);
                } else {
                    printf("Tree is empty. Build a tree first.\n");
                }
                break;
            }
            case 3: { // ����� �������� � ����������� ��������
                if (root != NULL) {
                    findMinFrequency(root);
                } else {
                    printf("Tree is empty. Build a tree first.\n");
                }
                break;
            }
            case 4: { // �������� ���������
                if (root != NULL) {
                    char symbol;
                    int level;
                    
                    printf("Enter character to delete: ");
                    scanf("%c", &symbol);
                    getchar(); // ������� ����� �����
                    
                    printf("Enter level (0 for root): ");
                    scanf("%d", &level);
                    getchar(); // ������� ����� �����
                    
                    root = deleteSubtree(root, tolower(symbol), level);
                    printf("Subtree deleted.\n");
                } else {
                    printf("Tree is empty. Build a tree first.\n");
                }
                break;
            }
            case 5: { // ���������� ������ � ����
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
            case 6: { // �������� ������ �� �����
                printf("Enter filename to load: ");
                fgets(filename, sizeof(filename), stdin);
                filename[strcspn(filename, "\n")] = '\0';
                
                FILE* inFile = fopen(filename, "r");
                if (inFile != NULL) {
                    Node* newTree = loadTree(inFile);
                    fclose(inFile);
                    
                    if (newTree != NULL) {
                        // ����������� ������ ������, ���� ��� ����
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
            case 7: { // ����� �� ���������
                printf("Exiting program.\n");
                break;
            }
            default: { // �������� �����
                printf("Invalid choice. Please try again.\n");
                break;
            }
        }
    } while (choice != 7);
    
    // ����������� ������ ����� �������
    if (root != NULL) {
        freeTree(root);
    }
    
    return 0;
}
