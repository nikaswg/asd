#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STR_LEN 100
#define MAX_LINES 100

int load_strings(const char *filename, char ***arr) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error opening file!\n");
        return -1;
    }

    char buffer[MAX_STR_LEN];
    int count = 0;

    *arr = (char **)malloc(MAX_LINES * sizeof(char *));
    if (!*arr) {
        printf("Memory allocation error!\n");
        fclose(file);
        return -1;
    }

    while (fgets(buffer, MAX_STR_LEN, file) && count < MAX_LINES) {
        buffer[strcspn(buffer, "\n")] = 0;
        (*arr)[count] = strdup(buffer);
        if (!(*arr)[count]) {
            printf("Memory allocation error!\n");
            fclose(file);
            return -1;
        }
        count++;
    }

    fclose(file);
    return count;
}

void print_strings(char **arr, int size, const char *title) {
	int i;
    printf("\n%s\n", title);
    printf("+----+--------------------------------+\n");
    printf("| #  | string                         |\n");
    printf("+----+--------------------------------+\n");
    for (i = 0; i < size; i++) {
        printf("| %-2d | %-30s |\n", i + 1, arr[i]);
    }
    printf("+----+--------------------------------+\n");
}

void shaker_sort(char **arr, int size) {
	int i;
    int left = 0, right = size - 1;
    int swapped = 1;
    while (left < right && swapped) {
        swapped = 0;
        for (i = left; i < right; i++) {
            if (strlen(arr[i]) < strlen(arr[i + 1])) {
                char *temp = arr[i];
                arr[i] = arr[i + 1];
                arr[i + 1] = temp;
                swapped = 1;
            }
        }
        right--;
        
        for (i = right; i > left; i--) {
            if (strlen(arr[i - 1]) < strlen(arr[i])) {
                char *temp = arr[i - 1];
                arr[i - 1] = arr[i];
                arr[i] = temp;
                swapped = 1;
            }
        }
        left++;
    }
}

void free_memory(char **arr, int size) {
	int i;
    for (i = 0; i < size; i++) {
        free(arr[i]);
    }
    free(arr);
}

int main() {
    char **arr;
    int size = load_strings("input.txt", &arr);

    if (size == -1) {
        return 1;
    }

    print_strings(arr, size, "Original array:");

    shaker_sort(arr, size);

    print_strings(arr, size, "Sorted array by length:");

    free_memory(arr, size);
    return 0;
}
