#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_LEN 256       // Максимальная длина строки
#define MAX_WORDS 100     // Максимальное количество слов

// Функция для выделения слов из строки (без string.h)
int Words(const char *s, char *words[]) {
    if (s == NULL) {
        return 0; // Если строка NULL, возвращаем 0
    }

    int word_count = 0;
    int in_word = 0;
    int word_start = 0;

    for (int i = 0; s[i] != '\0'; i++) {
        if (isalpha((unsigned char)s[i])) {  // Если символ — буква
            if (!in_word) { // Начало нового слова
                word_start = i;
                in_word = 1;
            }
        } else {
            if (in_word) { // Конец слова
                int word_length = i - word_start;
                words[word_count] = (char *)malloc(word_length + 1); // Выделяем память
                if (words[word_count] == NULL) {
                    printf("Ошибка выделения памяти!\n");
                    return word_count; // Возвращаем текущее количество слов
                }
                // Копируем слово вручную (без strncpy)
                for (int j = 0; j < word_length; j++) {
                    words[word_count][j] = s[word_start + j];
                }
                words[word_count][word_length] = '\0'; // Завершающий нуль
                word_count++;
                if (word_count >= MAX_WORDS) {
                    return word_count; // Достигнут максимум слов
                }
            }
            in_word = 0;
        }
    }

    // Обработка последнего слова, если строка не заканчивается разделителем
    if (in_word) {
        // Вычисляем длину слова вручную (без strlen)
        int word_length = 0;
        while (s[word_start + word_length] != '\0') {
            word_length++;
        }
        words[word_count] = (char *)malloc(word_length + 1);
        if (words[word_count] == NULL) {
            printf("Ошибка выделения памяти!\n");
            return word_count;
        }
        // Копируем слово вручную
        for (int j = 0; j < word_length; j++) {
            words[word_count][j] = s[word_start + j];
        }
        words[word_count][word_length] = '\0';
        word_count++;
    }

    return word_count;
}

int main() {
    char str[MAX_LEN];
    char *words[MAX_WORDS]; // Массив указателей на слова
    int word_count;

    printf("Введите строку: ");
    fgets(str, MAX_LEN, stdin);  // Ввод строки

    // Удаляем символ новой строки вручную (без strcspn)
    int i = 0;
    while (str[i] != '\0' && str[i] != '\n') {
        i++;
    }
    str[i] = '\0';

    word_count = Words(str, words);

    printf("Найдено %d слов:\n", word_count);
    for (int i = 0; i < word_count; i++) {
        printf("%d: %s\n", i + 1, words[i]);
        free(words[i]); // Освобождаем память
    }

    return 0;
}