#include "../header/Header.h"

void enqueuePath(DirectoryTree *tree, Directory *node, Queue *dirQueue) {
    if (node == NULL) return;
    if (node != tree->root) {
        enqueuePath(tree, node->parent, dirQueue);
        char *nameCopy = strdup(node->name);
        if (!nameCopy) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(1);
        }
        enqueue(dirQueue, nameCopy);
    }
    // 루트 디렉토리의 이름은 "/"로 출력하므로 굳이 enqueue하지 않아도 됨
}

void printQueuePath(Queue *queue) {
    while (!isEmpty(queue)) {
        char *name = dequeue(queue);
        printf("/%s", name);
        free(name);
    }
    printf("\n");
}

void printPath(DirectoryTree *dirTree, Queue *dirQueue) {
    Directory *current = dirTree->current;
    if (current == dirTree->root) {
        printf("/\n");
    } else {
        enqueuePath(dirTree, current, dirQueue);
        printQueuePath(dirQueue);
    }
}

int pwd(DirectoryTree *dirTree, Queue *dirQueue, char *option) {
    if (dirTree == NULL) {
        printf("The current directory information could not be found.\n");
        return false;
    }

    // 옵션이 없거나 -L, -P, --, - 등은 모두 기본 동작
    if (option == NULL || strcmp(option, "-") == 0 || strcmp(option, "--") == 0 ||
        strcmp(option, "-L") == 0 || strcmp(option, "-P") == 0) {
        printPath(dirTree, dirQueue);
    }
    // --help 옵션
    else if (strcmp(option, "--help") == 0) {
        printf("pwd: pwd [-LP]\n");
        printf("  Print the name of the current working directory.\n\n");
        printf("  Options:\n");
        printf("    -L\t print the value of $PWD if it names the current working\n");
        printf("  \t directory\n");
        printf("    -P\t print the physical directory, without any symbolic links\n\n");
        printf("  By default, 'pwd' behaves as if '-L' were specified.\n\n");
        printf("  Exit status:\n");
        printf("  Returns 0 unless an invalid option is given or the current directory\n");
        printf("  cannot be read.\n");
    }
    // 잘못된 옵션 처리
    else if (option[0] == '-') {
        printf("-bash: pwd: %.2s: invalid option\n", option);
        printf("pwd: usage: pwd [-LP]\n");
        return false;
    }
    // 인자가 경로 등일 때는 경로 출력
    else {
        printPath(dirTree, dirQueue);
    }
    return true;
}
