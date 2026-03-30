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

// ---------------- INPUT ----------------
void readLine(char *buffer, int size) {
    fgets(buffer, size, stdin);
    buffer[strcspn(buffer, "\n")] = 0;
}

// ---------------- FILE ----------------
void saveToFile() {
    FILE *fp = fopen("tasks.dat", "wb");
    if (!fp) return;

    fwrite(&count, sizeof(int), 1, fp);
    fwrite(&nextId, sizeof(int), 1, fp);
    fwrite(tasks, sizeof(struct Task), count, fp);

    fclose(fp);
}

void loadFromFile() {
    FILE *fp = fopen("tasks.dat", "rb");
    if (!fp) return;

    fread(&count, sizeof(int), 1, fp);
    fread(&nextId, sizeof(int), 1, fp);
    fread(tasks, sizeof(struct Task), count, fp);

    fclose(fp);
}

// ---------------- DASHBOARD ----------------
void showDashboard() {
    int pending = 0, completed = 0;

    for (int i = 0; i < count; i++) {
        if (tasks[i].isCompleted) completed++;
        else pending++;
    }

    printf("\n========== DASHBOARD ==========\n");
    printf("Total Tasks   : %d\n", count);
    printf("Pending Tasks : %d\n", pending);
    printf("Completed     : %d\n", completed);
    printf("================================\n");
}

// ---------------- PRINT ----------------
void printTask(struct Task t) {
    printf("\n----------------------------------\n");
    printf("ID: %d\n", t.id);
    printf("Title: %s\n", t.title);
    printf("Category: %s\n", t.category);
    printf("DateTime: %s\n", t.datetime);
    printf("Status: %s\n", t.isCompleted ? "Done" : "Pending");
}

// ---------------- SORT ----------------
void sortTasks() {
    struct Task temp;

    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (strcmp(tasks[j].datetime, tasks[j + 1].datetime) > 0) {
                temp = tasks[j];
                tasks[j] = tasks[j + 1];
                tasks[j + 1] = temp;
            }
        }
    }
}

// ---------------- ADD ----------------
void addTask() {
    if (count >= MAX) {
        printf("Task limit reached!\n");
        return;
    }

    struct Task t;
    t.id = nextId++;

    getchar();

    printf("Enter Title: ");
    readLine(t.title, sizeof(t.title));

    if (strlen(t.title) == 0) {
        printf("Title cannot be empty!\n");
        return;
    }

    printf("Enter Category: ");
    readLine(t.category, sizeof(t.category));

    printf("Enter DateTime (YYYY-MM-DD HH:MM): ");
    readLine(t.datetime, sizeof(t.datetime));

    for (int i = 0; i < count; i++) {
        if (strcmp(tasks[i].datetime, t.datetime) == 0) {
            printf("⚠ Conflict: Another task exists at same time!\n");
        }
    }

    t.isCompleted = 0;
    tasks[count++] = t;

    printf("Task Added Successfully!\n");
}

// ---------------- VIEW ----------------
void viewTasks() {
    if (count == 0) {
        printf("No tasks available.\n");
        return;
    }

    sortTasks();

    printf("\n====== PENDING TASKS ======\n");
    for (int i = 0; i < count; i++)
        if (!tasks[i].isCompleted)
            printTask(tasks[i]);

    printf("\n====== COMPLETED TASKS ======\n");
    for (int i = 0; i < count; i++)
        if (tasks[i].isCompleted)
            printTask(tasks[i]);
}

// ---------------- COMPLETE ----------------
void markCompleted() {
    int id;
    printf("Enter Task ID: ");
    scanf("%d", &id);

    for (int i = 0; i < count; i++) {
        if (tasks[i].id == id) {
            tasks[i].isCompleted = 1;
            printf("Marked as completed.\n");
            return;
        }
    }

    printf("Task not found!\n");
}

// ---------------- DELETE ----------------
void deleteTask() {
    int id;
    char confirm;

    printf("Enter Task ID: ");
    scanf("%d", &id);

    printf("Confirm delete (y/n): ");
    scanf(" %c", &confirm);

    if (confirm != 'y') return;

    for (int i = 0; i < count; i++) {
        if (tasks[i].id == id) {
            for (int j = i; j < count - 1; j++)
                tasks[j] = tasks[j + 1];

            count--;
            printf("Deleted successfully.\n");
            return;
        }
    }

    printf("Task not found!\n");
}

// ---------------- UPDATE ----------------
void updateTask() {
    int id;
    printf("Enter Task ID: ");
    scanf("%d", &id);

    getchar();

    for (int i = 0; i < count; i++) {
        if (tasks[i].id == id) {

            printf("Enter New Title: ");
            readLine(tasks[i].title, sizeof(tasks[i].title));

            printf("Enter New Category: ");
            readLine(tasks[i].category, sizeof(tasks[i].category));

            printf("Enter New DateTime: ");
            readLine(tasks[i].datetime, sizeof(tasks[i].datetime));

            printf("Updated successfully.\n");
            return;
        }
    }

    printf("Task not found!\n");
}

// ---------------- SEARCH ----------------
void searchTask() {
    char key[50];

    getchar();
    printf("Enter keyword: ");
    readLine(key, sizeof(key));

    int found = 0;

    for (int i = 0; i < count; i++) {
        if (strstr(tasks[i].title, key) ||
            strstr(tasks[i].category, key)) {
            printTask(tasks[i]);
            found = 1;
        }
    }

    if (!found)
        printf("No matching tasks found.\n");
}

// ---------------- FILTER ----------------
void filterTasks() {
    int choice;

    printf("\n1. Pending\n2. Completed\n3. By Category\nChoice: ");
    scanf("%d", &choice);

    if (choice == 1) {
        for (int i = 0; i < count; i++)
            if (!tasks[i].isCompleted)
                printTask(tasks[i]);
    }

    else if (choice == 2) {
        for (int i = 0; i < count; i++)
            if (tasks[i].isCompleted)
                printTask(tasks[i]);
    }

    else if (choice == 3) {
        char cat[20];
        getchar();
        printf("Enter category: ");
        readLine(cat, sizeof(cat));

        for (int i = 0; i < count; i++)
            if (strcmp(tasks[i].category, cat) == 0)
                printTask(tasks[i]);
    }

    else {
        printf("Invalid choice.\n");
    }
}

// ---------------- MAIN ----------------
int main() {
    int choice;

    loadFromFile();

    while (1) {
        showDashboard();

        printf("\n===== TO DO LIST MANAGER =====\n");
        printf("1. Add Task\n");
        printf("2. View Tasks\n");
        printf("3. Mark Completed\n");
        printf("4. Delete Task\n");
        printf("5. Update Task\n");
        printf("6. Search Task\n");
        printf("7. Filter Tasks\n");
        printf("8. Save & Exit\n");

        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addTask(); break;
            case 2: viewTasks(); break;
            case 3: markCompleted(); break;
            case 4: deleteTask(); break;
            case 5: updateTask(); break;
            case 6: searchTask(); break;
            case 7: filterTasks(); break;
            case 8:
                saveToFile();
                printf("Saved successfully.\n");
                exit(0);
            default:
                printf("Invalid choice!\n");
        }
    }

    return 0;
}