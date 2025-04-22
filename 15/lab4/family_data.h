#ifndef FAMILY_DATA_H
#define FAMILY_DATA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ��������� ��� �������� ������ � �����
typedef struct Family {
    char surname[50];           // �������
    char name[50];              // ���
    char patronymic[50];        // ��������
    char faculty[50];           // ���������
    char father_specialty[50];  // ������������� ����
    char mother_specialty[50];  // ������������� ������
    int siblings_count;         // ���������� ������� � ������
    struct Family *next;        // ��������� �� ��������� �������
} Family;

// ��������� �������
Family *create_family(char *surname, char *name, char *patronymic, char *faculty,
                      char *father_specialty, char *mother_specialty, int siblings_count);
void add_family(Family **head, Family *new_family);
void print_family(Family *head);
void save_to_file(Family *head, const char *filename);
Family *load_from_file(const char *filename);
void free_list(Family *head);
void sort_by_siblings(Family **head);
void sort_by_faculty_length(Family **head);
void delete_family(Family **head, const char *surname, const char *name);
void edit_family(Family *head, const char *surname, const char *name);

#endif
