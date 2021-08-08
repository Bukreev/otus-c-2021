#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

bool isJpeg(const int* content) {
    if (content[0] == 255 && content[1] == 216)
        return true;
    return false;
}

bool isZipJpg(const int* content, const unsigned int length) {
    if (content[length - 1] == 255 && content[length] == 217)
        return false;
    return true;
}

int get_tail_index(const int* content, const unsigned int length) {
    for (int i = 0; i < length; i++) {
        if (content[i] == 255) {
            if (content[i + 1] == 217)
                return i + 1;
        }
    }
    return 0;
}

void my_read(FILE *fp, int* buffer) {
    int ch;
    int i = 0;
    while (!feof(fp)) {
        ch = fgetc(fp);
        buffer[i] = ch;
        i++;
    }
}

void write_zip(int *buffer, unsigned long size, unsigned int index) {
    FILE* archive = NULL;
    archive = fopen("result.zip", "wb");
    for (unsigned int i = ++index; i < size; i++) {
        fputc(buffer[i], archive);
    }
    fclose(archive);
}
int main() {

    unsigned long file_size;
    FILE* file = NULL;
    int* buffer = NULL;
    char file_path[100];

    puts("Введите адрес до файла\n");

    fflush(stdin);
    if (fgets(file_path, sizeof(file_path), stdin)) {
        file_path[strcspn(file_path, "\n")] = 0;
        file = fopen(file_path, "rb");

        if (file == NULL) {
            puts("файл не найден\n");
            exit(1);
        }

        fseek(file, 0, SEEK_END);
        file_size = ftell(file);
        rewind(file);

        buffer = (int*) malloc(file_size + 1);

        if (buffer == NULL) {
            puts("ошибка выделения памяти\n");
            exit(1);
        }

        my_read(file, buffer);

        if (isJpeg(buffer) && isZipJpg(buffer, file_size)) {
            unsigned int index = get_tail_index(buffer, file_size);
            write_zip(buffer, file_size, index);
        }

        fclose(file);
        free(buffer);
    } else {
        puts("файл не найден\n");
    }
    return 0;
}
