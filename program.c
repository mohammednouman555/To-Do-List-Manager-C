#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100

struct Task {
    int id;
    char title[50];
    char category[20];
    char datetime[20];
    int isCompleted;
};

struct Task tasks[MAX];
int count = 0;
int nextId = 1;

// ---------------- INPUT HANDLING ----------------
void readLine(char *buffer, int size) {
    fgets(buffer, size, stdin);
    buffer[strcspn(buffer, "\n")] = 0;
}

// ---------------- FILE HANDLING ----------------
void saveToFile() {
    FILE *fp = fopen("tasks.dat", "wb");
    if (fp == NULL) return;

    fwrite(&count, sizeof(int), 1, fp);
    fwrite(&nextId, sizeof(int), 1, fp);
    fwrite(tasks, sizeof(struct Task), count, fp);

    fclose(fp);
}

void loadFromFile() {
    FILE *fp = fopen("tasks.dat", "rb");
    if (fp == NULL) return;

    fread(&count, sizeof(int), 1, fp);
    fread(&nextId, sizeof(int), 1, fp);
    fread(tasks, sizeof(struct Task), count, fp);

    fclose(fp);
}

// ---------------- DISPLAY ----------------
void printTask(struct Task t) {
    printf("\n----------------------------------\n");
    printf("ID: %d\n", t.id);
    printf("Title: %s\n", t.title);
    printf("Category: %s\n", t.category);
    printf("DateTime: %s\n", t.datetime);
    printf("Status: %s\n", t.isCompleted ? "Done" : "Pending");
}

// ---------------- ADD TASK ----------------
void addTask() {
    if (count >= MAX) {
        printf("Task limit reached!\n");
        return;
    }

    struct Task t;
    t.id = nextId++;

    getchar(); // clear buffer

    printf("Enter Title: ");
    readLine(t.title, sizeof(t.title));

    printf("Enter Category: ");
    readLine(t.category, sizeof(t.category));

    printf("Enter Date & Time (YYYY-MM-DD HH:MM): ");
    readLine(t.datetime, sizeof(t.datetime));

    // Conflict warning
    for (int i = 0; i < count; i++) {
        if (strcmp(tasks[i].datetime, t.datetime) == 0) {
            printf("⚠ Warning: Another task exists at same time!\n");
        }
    }

    t.isCompleted = 0;
    tasks[count++] = t;

    printf("\nTask Added Successfully!\n");
}

// ---------------- VIEW TASKS ----------------
void viewTasks() {
    if (count == 0) {
        printf("\nNo tasks available.\n");
        return;
    }

    printf("\n====== PENDING TASKS ======\n");
    for (int i = 0; i < count; i++) {
        if (!tasks[i].isCompleted)
            printTask(tasks[i]);
    }

    printf("\n====== COMPLETED TASKS ======\n");
    for (int i = 0; i < count; i++) {
        if (tasks[i].isCompleted)
            printTask(tasks[i]);
    }
}

// ---------------- MARK COMPLETED ----------------
void markCompleted() {
    int id;
    printf("Enter Task ID: ");
    scanf("%d", &id);

    for (int i = 0; i < count; i++) {
        if (tasks[i].id == id) {
            tasks[i].isCompleted = 1;
            printf("Task marked as completed!\n");
            return;
        }
    }

    printf("Task not found!\n");
}

// ---------------- DELETE TASK ----------------
void deleteTask() {
    int id;
    char confirm;

    printf("Enter Task ID to delete: ");
    scanf("%d", &id);

    printf("Are you sure? (y/n): ");
    scanf(" %c", &confirm);

    if (confirm != 'y') return;

    for (int i = 0; i < count; i++) {
        if (tasks[i].id == id) {
            for (int j = i; j < count - 1; j++) {
                tasks[j] = tasks[j + 1];
            }
            count--;
            printf("Task deleted successfully!\n");
            return;
        }
    }

    printf("Task not found!\n");
}

// ---------------- UPDATE TASK ----------------
void updateTask() {
    int id;
    printf("Enter Task ID to update: ");
    scanf("%d", &id);

    getchar();

    for (int i = 0; i < count; i++) {
        if (tasks[i].id == id) {

            printf("Enter new Title: ");
            readLine(tasks[i].title, sizeof(tasks[i].title));

            printf("Enter new Category: ");
            readLine(tasks[i].category, sizeof(tasks[i].category));

            printf("Enter new Date & Time: ");
            readLine(tasks[i].datetime, sizeof(tasks[i].datetime));

            printf("Task updated successfully!\n");
            return;
        }
    }

    printf("Task not found!\n");
}

// ---------------- SEARCH ----------------
void searchTask() {
    char keyword[50];

    getchar();
    printf("Enter keyword: ");
    readLine(keyword, sizeof(keyword));

    int found = 0;

    for (int i = 0; i < count; i++) {
        if (strstr(tasks[i].title, keyword) ||
            strstr(tasks[i].category, keyword)) {

            printTask(tasks[i]);
            found = 1;
        }
    }

    if (!found) {
        printf("No matching tasks found.\n");
    }
}

// ---------------- MAIN ----------------
int main() {
    int choice;

    loadFromFile();

    while (1) {
        printf("\n====================================\n");
        printf("       TO DO LIST MANAGER\n");
        printf("====================================\n");
        printf("1. Add Task\n");
        printf("2. View Tasks\n");
        printf("3. Mark Completed\n");
        printf("4. Delete Task\n");
        printf("5. Update Task\n");
        printf("6. Search Task\n");
        printf("7. Save & Exit\n");

        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addTask(); break;
            case 2: viewTasks(); break;
            case 3: markCompleted(); break;
            case 4: deleteTask(); break;
            case 5: updateTask(); break;
            case 6: searchTask(); break;
            case 7:
                saveToFile();
                printf("Data saved. Exiting...\n");
                exit(0);
            default:
                printf("Invalid choice!\n");
        }
    }

    return 0;
}