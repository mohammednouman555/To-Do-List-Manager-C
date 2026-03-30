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

// Function to add task
void addTask() {
    struct Task t;

    t.id = count + 1;

    printf("Enter Title: ");
    getchar(); // clear buffer
    fgets(t.title, sizeof(t.title), stdin);

    printf("Enter Category: ");
    fgets(t.category, sizeof(t.category), stdin);

    printf("Enter Date & Time (YYYY-MM-DD HH:MM): ");
    fgets(t.datetime, sizeof(t.datetime), stdin);

    t.isCompleted = 0;

    tasks[count++] = t;

    printf("\nTask Added Successfully!\n");
}

// View all tasks
void viewTasks() {
    if (count == 0) {
        printf("\nNo tasks available.\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        printf("\nID: %d", tasks[i].id);
        printf("Title: %s", tasks[i].title);
        printf("Category: %s", tasks[i].category);
        printf("DateTime: %s", tasks[i].datetime);
        printf("Status: %s\n", tasks[i].isCompleted ? "Done" : "Pending");
    }
}

// Mark task completed
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

// Main menu
int main() {
    int choice;

    while (1) {
        printf("\n==== TO DO LIST MANAGER ====\n");
        printf("1. Add Task\n");
        printf("2. View Tasks\n");
        printf("3. Mark Completed\n");
        printf("4. Exit\n");

        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addTask(); break;
            case 2: viewTasks(); break;
            case 3: markCompleted(); break;
            case 4: exit(0);
            default: printf("Invalid choice!\n");
        }
    }

    return 0;
}