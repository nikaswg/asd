#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <dirent.h>

// ????????? ??? ???????? ?????? ? ?????
typedef struct Family {
    char surname[50];
    char name[50];
    char patronymic[50];
    char faculty[50];
    char father_specialty[50];
    char mother_specialty[50];
    int siblings_count;
} Family;

// ?????????? ?????????? ??? ?????? ? ??????? ??????? ? ??????
Family current_record;
char current_db_file[256] = "";
FILE *db_file = NULL;

// ????????? ???????
void init_database();
void close_database();
void create_database(const char *filename);
void open_database(const char *filename);
void save_current_record();
void load_record_by_name(const char *name);
void add_new_record();
void delete_record_by_index();
void edit_record_by_index();
void print_all_records();
void external_merge_sort(int sort_field);
void print_sorted_records(int sort_field);
void change_database_file();
void rename_database_file();
void find_all_databases();
void delete_database_file();
int compare_records(const Family *a, const Family *b, int field);
void print_record(const Family *record);
void distribute_runs(FILE *src, FILE *f1, FILE *f2, int run_size, int sort_field);
void merge_runs(FILE *dest, FILE *f1, FILE *f2, int run_size, int sort_field);

// ????????????? ???? ??????
void init_database() {
    memset(&current_record, 0, sizeof(Family));
    current_db_file[0] = '\0';
    db_file = NULL;
}

// ???????? ???? ??????
void close_database() {
    if (db_file) {
        fclose(db_file);
        db_file = NULL;
    }
    current_db_file[0] = '\0';
}

// ???????? ????? ???? ??????
void create_database(const char *filename) {
    close_database();
    db_file = fopen(filename, "wb+");
    if (!db_file) {
        printf("Error creating database file.\n");
        return;
    }
    strcpy(current_db_file, filename);
    printf("Database created successfully: %s\n", filename);
}

// ???????? ???????????? ???? ??????
void open_database(const char *filename) {
    close_database();
    db_file = fopen(filename, "rb+");
    if (!db_file) {
        printf("Error opening database file.\n");
        return;
    }
    strcpy(current_db_file, filename);
    printf("Database opened successfully: %s\n", filename);
}

// ?????????? ??????? ?????? ? ????
void save_current_record() {
    if (!db_file) {
        printf("No database is open.\n");
        return;
    }
    
    fseek(db_file, 0, SEEK_END);
    fwrite(&current_record, sizeof(Family), 1, db_file);
    fflush(db_file);
    printf("Record saved successfully.\n");
}

// ????? ?????? ?? ?????
void load_record_by_name(const char *name) {
    if (!db_file) {
        printf("No database is open.\n");
        return;
    }
    
    rewind(db_file);
    Family record;
    bool found = false;
    
    while (fread(&record, sizeof(Family), 1, db_file)) {
        if (strcmp(record.name, name) == 0) {
            current_record = record;
            found = true;
            break;
        }
    }
    
    if (found) {
        printf("Record found:\n");
        print_record(&current_record);
    } else {
        printf("Record with name '%s' not found.\n", name);
    }
}

// ?????????? ????? ??????
void add_new_record() {
    printf("Enter surname: ");
    scanf("%s", current_record.surname);
    printf("Enter name: ");
    scanf("%s", current_record.name);
    printf("Enter patronymic: ");
    scanf("%s", current_record.patronymic);
    printf("Enter faculty: ");
    scanf("%s", current_record.faculty);
    printf("Enter father's specialty: ");
    scanf("%s", current_record.father_specialty);
    printf("Enter mother's specialty: ");
    scanf("%s", current_record.mother_specialty);
    printf("Enter number of siblings: ");
    scanf("%d", &current_record.siblings_count);
    
    save_current_record();
}

// ???????? ?????? ?? ???????
void delete_record_by_index() {
    if (!db_file) {
        printf("No database is open.\n");
        return;
    }

    rewind(db_file);
    int record_count = 0;
    Family record;
    while (fread(&record, sizeof(Family), 1, db_file)) {
        record_count++;
    }

    if (record_count == 0) {
        printf("Database is empty. Nothing to delete.\n");
        return;
    }

    int index;
    do {
        printf("Enter record index to delete (0-%d): ", record_count - 1);
        scanf("%d", &index);

        if (index < 0 || index >= record_count) {
            printf("Invalid index. Please try again.\n");
        }
    } while (index < 0 || index >= record_count);

    FILE *temp_file = tmpfile();
    if (!temp_file) {
        printf("Error creating temporary file.\n");
        return;
    }

    rewind(db_file);
    int current_index = 0;
    bool deleted = false;

    while (fread(&record, sizeof(Family), 1, db_file)) {
        if (current_index != index) {
            fwrite(&record, sizeof(Family), 1, temp_file);
        } else {
            deleted = true;
            printf("Deleting record: ");
            print_record(&record);
        }
        current_index++;
    }

    if (deleted) {
        freopen(current_db_file, "wb+", db_file);
        rewind(temp_file);

        while (fread(&record, sizeof(Family), 1, temp_file)) {
            fwrite(&record, sizeof(Family), 1, db_file);
        }

        printf("Record deleted successfully.\n");
    } else {
        printf("Record not found.\n");
    }

    fclose(temp_file);
    memset(&current_record, 0, sizeof(Family));
}

// ?????????????? ?????? ?? ???????
void edit_record_by_index() {
    if (!db_file) {
        printf("No database is open.\n");
        return;
    }

    rewind(db_file);
    int record_count = 0;
    Family record;
    while (fread(&record, sizeof(Family), 1, db_file)) {
        record_count++;
    }

    if (record_count == 0) {
        printf("Database is empty. Nothing to edit.\n");
        return;
    }

    int index;
    do {
        printf("Enter record index to edit (0-%d): ", record_count - 1);
        scanf("%d", &index);

        if (index < 0 || index >= record_count) {
            printf("Invalid index. Please try again.\n");
        }
    } while (index < 0 || index >= record_count);

    rewind(db_file);
    for (int i = 0; i <= index; i++) {
        fread(&current_record, sizeof(Family), 1, db_file);
    }

    int choice;
    do {
        printf("\nEdit Menu for record %d: %s %s\n", index, current_record.surname, current_record.name);
        printf("Current values:\n");
        print_record(&current_record);
        printf("\n1. Edit surname\n");
        printf("2. Edit name\n");
        printf("3. Edit patronymic\n");
        printf("4. Edit faculty\n");
        printf("5. Edit father's specialty\n");
        printf("6. Edit mother's specialty\n");
        printf("7. Edit number of siblings\n");
        printf("8. Back to main menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        Family original_record = current_record;

        switch (choice) {
            case 1:
                printf("Enter new surname (current: %s): ", current_record.surname);
                scanf("%s", current_record.surname);
                break;
            case 2:
                printf("Enter new name (current: %s): ", current_record.name);
                scanf("%s", current_record.name);
                break;
            case 3:
                printf("Enter new patronymic (current: %s): ", current_record.patronymic);
                scanf("%s", current_record.patronymic);
                break;
            case 4:
                printf("Enter new faculty (current: %s): ", current_record.faculty);
                scanf("%s", current_record.faculty);
                break;
            case 5:
                printf("Enter new father's specialty (current: %s): ", current_record.father_specialty);
                scanf("%s", current_record.father_specialty);
                break;
            case 6:
                printf("Enter new mother's specialty (current: %s): ", current_record.mother_specialty);
                scanf("%s", current_record.mother_specialty);
                break;
            case 7:
                printf("Enter new number of siblings (current: %d): ", current_record.siblings_count);
                scanf("%d", &current_record.siblings_count);
                break;
            case 8:
                return;
            default:
                printf("Invalid choice. Please try again.\n");
                continue;
        }

        if (choice >= 1 && choice <= 7) {
            FILE *temp_file = tmpfile();
            if (!temp_file) {
                printf("Error creating temporary file.\n");
                current_record = original_record;
                continue;
            }

            rewind(db_file);
            Family temp_record;
            int current_index = 0;

            while (fread(&temp_record, sizeof(Family), 1, db_file)) {
                if (current_index == index) {
                    fwrite(&current_record, sizeof(Family), 1, temp_file);
                } else {
                    fwrite(&temp_record, sizeof(Family), 1, temp_file);
                }
                current_index++;
            }

            freopen(current_db_file, "wb+", db_file);
            rewind(temp_file);

            while (fread(&temp_record, sizeof(Family), 1, temp_file)) {
                fwrite(&temp_record, sizeof(Family), 1, db_file);
            }

            fclose(temp_file);
            printf("Changes saved successfully.\n");
        }
    } while (choice != 8);
}

// ?????? ???? ??????? ? ??????? ????????
void print_all_records() {
    if (!db_file) {
        printf("No database is open.\n");
        return;
    }
    
    rewind(db_file);
    Family record;
    int count = 0;
    
    printf("\nAll records in database:\n");
    printf("%-15s %-10s %-15s %-20s %-20s %-20s %-9s\n",
           "Surname", "Name", "Patronymic", "Faculty", "Father Spec.", "Mother Spec.", "Siblings");
    printf("----------------------------------------------------------------------------------------\n");
    
    while (fread(&record, sizeof(Family), 1, db_file)) {
        print_record(&record);
        count++;
    }
    
    printf("\nTotal records: %d\n", count);
}

// ??????? ????????????? ??????? ?? ???? ??????
void distribute_runs(FILE *src, FILE *f1, FILE *f2, int run_size, int sort_field) {
    rewind(src);
    rewind(f1);
    rewind(f2);
    
    Family record1, record2;
    int count = 0;
    bool toggle = true;
    
    while (fread(&record1, sizeof(Family), 1, src)) {
        count++;
        
        if (toggle) {
            fwrite(&record1, sizeof(Family), 1, f1);
        } else {
            fwrite(&record1, sizeof(Family), 1, f2);
        }
        
        if (count % run_size == 0) {
            toggle = !toggle;
        }
    }
}

// ??????? ??????? ????? ?? ???? ??????
void merge_runs(FILE *dest, FILE *f1, FILE *f2, int run_size, int sort_field) {
    rewind(dest);
    rewind(f1);
    rewind(f2);
    
    Family record1, record2;
    bool has_record1 = fread(&record1, sizeof(Family), 1, f1) == 1;
    bool has_record2 = fread(&record2, sizeof(Family), 1, f2) == 1;
    
    while (has_record1 || has_record2) {
        int count1 = 0, count2 = 0;
        
        while (has_record1 && has_record2 && count1 < run_size && count2 < run_size) {
            if (compare_records(&record1, &record2, sort_field) <= 0) {
                fwrite(&record1, sizeof(Family), 1, dest);
                has_record1 = fread(&record1, sizeof(Family), 1, f1) == 1;
                count1++;
            } else {
                fwrite(&record2, sizeof(Family), 1, dest);
                has_record2 = fread(&record2, sizeof(Family), 1, f2) == 1;
                count2++;
            }
        }
        
        while (has_record1 && count1 < run_size) {
            fwrite(&record1, sizeof(Family), 1, dest);
            has_record1 = fread(&record1, sizeof(Family), 1, f1) == 1;
            count1++;
        }
        
        while (has_record2 && count2 < run_size) {
            fwrite(&record2, sizeof(Family), 1, dest);
            has_record2 = fread(&record2, sizeof(Family), 1, f2) == 1;
            count2++;
        }
    }
}

// ??????? ?????????? ????????
void external_merge_sort(int sort_field) {
    if (!db_file) {
        printf("No database is open.\n");
        return;
    }

    // ??????? ????????? ?????
    FILE *temp1 = tmpfile();
    FILE *temp2 = tmpfile();
    if (!temp1 || !temp2) {
        printf("Error creating temporary files.\n");
        if (temp1) fclose(temp1);
        if (temp2) fclose(temp2);
        return;
    }

    // ?????????? ?????????? ???????
    rewind(db_file);
    int record_count = 0;
    Family record;
    while (fread(&record, sizeof(Family), 1, db_file)) {
        record_count++;
    }

    if (record_count == 0) {
        printf("Database is empty.\n");
        fclose(temp1);
        fclose(temp2);
        return;
    }

    int run_size = 1;
    while (run_size < record_count) {
        // ???????????? ??????
        distribute_runs(db_file, temp1, temp2, run_size, sort_field);
        
        // ??????? ?????
        freopen(current_db_file, "wb+", db_file);
        merge_runs(db_file, temp1, temp2, run_size, sort_field);
        
        run_size *= 2;
    }

    fclose(temp1);
    fclose(temp2);
    printf("Database sorted successfully by field %d.\n", sort_field);
}

// ?????? ??????????????? ???????
void print_sorted_records(int sort_field) {
    if (!db_file) {
        printf("No database is open.\n");
        return;
    }

    // ??????? ????????? ????? ??? ??????????
    FILE *temp_db = tmpfile();
    if (!temp_db) {
        printf("Error creating temporary file.\n");
        return;
    }

    // ???????? ??????
    rewind(db_file);
    Family record;
    while (fread(&record, sizeof(Family), 1, db_file)) {
        fwrite(&record, sizeof(Family), 1, temp_db);
    }

    // ????????? ????????? ????
    FILE *original_db = db_file;
    db_file = temp_db;
    external_merge_sort(sort_field);
    db_file = original_db;

    // ???????? ?????????
    rewind(temp_db);
    printf("\nRecords sorted by field %d:\n", sort_field);
    printf("%-15s %-10s %-15s %-20s %-20s %-20s %-9s\n",
           "Surname", "Name", "Patronymic", "Faculty", "Father Spec.", "Mother Spec.", "Siblings");
    printf("----------------------------------------------------------------------------------------\n");

    int count = 0;
    while (fread(&record, sizeof(Family), 1, temp_db)) {
        print_record(&record);
        count++;
    }

    printf("\nTotal records: %d\n", count);
    fclose(temp_db);
}

// ????????? ??????? ?? ?????????? ????
int compare_records(const Family *a, const Family *b, int field) {
    switch (field) {
        case 1: return strcmp(a->name, b->name);
        case 2: return strcmp(a->surname, b->surname);
        case 3: return strcmp(a->patronymic, b->patronymic);
        case 4: return strcmp(a->faculty, b->faculty);
        case 5: return strcmp(a->father_specialty, b->father_specialty);
        case 6: return strcmp(a->mother_specialty, b->mother_specialty);
        case 7: return a->siblings_count - b->siblings_count;
        default: return 0;
    }
}

// ?????? ????? ??????
void print_record(const Family *record) {
    printf("%-15s %-10s %-15s %-20s %-20s %-20s %-9d\n",
           record->surname, record->name, record->patronymic,
           record->faculty, record->father_specialty, 
           record->mother_specialty, record->siblings_count);
}

// ????????? ??????? (change_database_file, rename_database_file, 
// find_all_databases, delete_database_file, display_main_menu, main)
// ???????? ??? ?????????, ??? ? ?????????? ????

// ????? ???????? ????? ??
void change_database_file() {
    char filename[256];
    printf("Enter new database filename: ");
    scanf("%s", filename);
    open_database(filename);
}

// ?????????????? ????? ??
void rename_database_file() {
    if (!db_file) {
        printf("No database is open.\n");
        return;
    }
    
    char new_name[256];
    printf("Enter new name for database file: ");
    scanf("%s", new_name);
    
    close_database();
    
    if (rename(current_db_file, new_name) == 0) {
        printf("Database renamed successfully.\n");
        strcpy(current_db_file, new_name);
        open_database(new_name);
    } else {
        printf("Error renaming database file.\n");
    }
}

// ????? ???? ?????? ?? ? ??????? ??????????
void find_all_databases() {
    DIR *dir;
    struct dirent *ent;
    int count = 0;
    
    printf("\nDatabase files in current directory:\n");
    
    if ((dir = opendir(".")) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            if (strstr(ent->d_name, ".dat") != NULL) {
                printf("- %s\n", ent->d_name);
                count++;
            }
        }
        closedir(dir);
    } else {
        printf("Could not open directory.\n");
        return;
    }
    
    printf("\nTotal database files found: %d\n", count);
}

// ???????? ????? ??
void delete_database_file() {
    if (!db_file) {
        printf("No database is open.\n");
        return;
    }
    
    char filename[256];
    strcpy(filename, current_db_file);
    
    close_database();
    
    if (remove(filename) == 0) {
        printf("Database file deleted successfully.\n");
    } else {
        printf("Error deleting database file.\n");
    }
}

// ??????? ????
void display_main_menu() {
    printf("\nDatabase Management System\n");
    printf("Current database: %s\n", current_db_file[0] ? current_db_file : "None");
    printf("1. Create new database\n");
    printf("2. Open database\n");
    printf("3. Find record by name\n");
    printf("4. Add new record\n");
    printf("5. Edit current record\n");
    printf("6. Delete current record\n");
    printf("7. Print all records\n");
    printf("8. Print sorted records\n");
    printf("9. Change database file\n");
    printf("10. Rename database file\n");
    printf("11. Find all databases\n");
    printf("12. Delete database file\n");
    printf("13. Exit\n");
    printf("Enter your choice: ");
}

// ??????? ???????
int main() {
    init_database();
    int choice;
    
    while (1) {
        display_main_menu();
        scanf("%d", &choice);
        
        switch (choice) {
            case 1: {
                char filename[256];
                printf("Enter filename for new database: ");
                scanf("%s", filename);
                create_database(filename);
                break;
            }
            case 2: {
                char filename[256];
                printf("Enter filename to open: ");
                scanf("%s", filename);
                open_database(filename);
                break;
            }
            case 3: {
                char name[50];
                printf("Enter name to search: ");
                scanf("%s", name);
                load_record_by_name(name);
                break;
            }
            case 4:
                add_new_record();
                break;
            case 5:
                edit_record_by_index();
                break;
            case 6:
                delete_record_by_index();
                break;
            case 7:
                print_all_records();
                break;
            case 8: {
                printf("Sort by:\n");
                printf("1. Name\n2. Surname\n3. Patronymic\n4. Faculty\n");
                printf("5. Father's specialty\n6. Mother's specialty\n7. Siblings count\n");
                printf("Enter field number: ");
                int field;
                scanf("%d", &field);
                if (field >= 1 && field <= 7) {
                    print_sorted_records(field);
                } else {
                    printf("Invalid field number.\n");
                }
                break;
            }
            case 9:
                change_database_file();
                break;
            case 10:
                rename_database_file();
                break;
            case 11:
                find_all_databases();
                break;
            case 12:
                delete_database_file();
                break;
            case 13:
                close_database();
                printf("Exiting program.\n");
                return 0;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}
