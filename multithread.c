#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Thread function to read from input.txt
void* read_file(void* arg) {
    FILE* file = fopen("input.txt", "r");
    if (file == NULL) {
        printf("Unable to open input.txt\n");
        return NULL;
    }

    char ch;
    printf("Reading from input.txt:\n");
    while ((ch = fgetc(file)) != EOF) {
        putchar(ch);
    }
    fclose(file);
    return NULL;
}

// Thread function to write to output.txt
void* write_file(void* arg) {
    FILE* file = fopen("output.txt", "w");
    if (file == NULL) {
        printf("Unable to open output.txt\n");
        return NULL;
    }

    fprintf(file, "This is written by the write thread!\n");
    fclose(file);
    printf("Data written to output.txt\n");
    return NULL;
}

int main() {
    pthread_t readThread, writeThread;

    pthread_create(&readThread, NULL, read_file, NULL);
    pthread_create(&writeThread, NULL, write_file, NULL);

    pthread_join(readThread, NULL);
    pthread_join(writeThread, NULL);

    printf("Both threads finished.\n");
    return 0;
}

