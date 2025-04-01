#include <stdio.h>

#define MAX_LEN 256  // Максимальная длина строки

int word_length_n(const char *s, int n) {
    if (s == NULL || n < 1) {
        return -1; // Ошибка: некорректный ввод
    }

    int word_count = 0;
    int length = 0;
    int in_word = 0;

    while (*s) {
        if (*s != ' ' && *s != '\n') {  // Если символ не пробел и не перевод строки
            if (!in_word) { // Если слово только началось
                word_count++;
                if (word_count == n) { // Начало нужного слова
                    length = 0;
                }
            }
            if (word_count == n) {
                length++; // Увеличиваем длину нужного слова
            }
            in_word = 1;
        } else { // Если встретился пробел или конец строки
            in_word = 0;
        }
        s++;
    }

    return (word_count >= n) ? length : -1; // Если слово найдено, возвращаем длину, иначе -1
}

int main() {
    char str[MAX_LEN];
    int n;

    printf("Введите строку: ");
    fgets(str, MAX_LEN, stdin);  // Безопасный ввод строки

    printf("Введите номер слова: ");
    if (scanf("%d", &n) != 1) {
        printf("Ошибка: некорректный ввод числа!\n");
        return 1; // Завершаем программу с ошибкой
    }

    int length = word_length_n(str, n);

    if (length != -1) {
        printf("Длина %d-го слова: %d\n", n, length);
    } else {
        printf("Ошибка: слово с номером %d не найдено!\n", n);
    }

    return 0;
}