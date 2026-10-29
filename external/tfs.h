#pragma once
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct tfs_file{
    size_t size;
    uint8_t* buffer;
    FILE* base;
    const char* name;
} tfs_file;

#ifdef TFS_STATIC
    #define TFSDEF static inline
#else
    #define TFSDEF extern
#endif

#ifdef __cplusplus
extern "C" {
#endif

TFSDEF void tfs_get_file_size(tfs_file* f);
TFSDEF int tfs_open_binary_read(tfs_file* f, const char* path);
TFSDEF int tfs_open_binary_write(tfs_file* f, const char* path);
TFSDEF int tfs_load_text(tfs_file* f, const char* path);

TFSDEF int tfs_read_binary_buffer(tfs_file* file);
TFSDEF int tfs_write_buffer(tfs_file* file, uint8_t* buffer, size_t size);

TFSDEF void tfs_print_text(tfs_file* f);
TFSDEF const char* tfs_get_file_extension(tfs_file* f);
TFSDEF void tfs_file_free(tfs_file* f);

#ifdef TFS_IMPL

TFSDEF void tfs_get_file_size(tfs_file* f){
    fseek(f->base, 0, SEEK_END);
    f->size = ftell(f->base);
    rewind(f->base);
}

TFSDEF int tfs_open_binary_read(tfs_file* f, const char* path){
    f->base = fopen(path, "rb");
    f->name = path;
    f->buffer = NULL;

    if (f->base == NULL) {
        printf("error: failed to open file; %s\n", f->name);
        return 1;
    }

    fseek(f->base, 0, SEEK_END);
    f->size = ftell(f->base);
    rewind(f->base);

    f->buffer = (uint8_t*)malloc(f->size);

    if (!f->buffer){
        printf("error: failed to create file buffer; %s\n", f->name);
        return 1;
    }

    size_t read = fread(f->buffer, 1, f->size, f->base);
    fclose(f->base);

    if (read != f->size){
        printf("error: file size is different\n");
        free(f->buffer);
        return 1;
    }

    return 0;
}

TFSDEF int tfs_open_binary_write(tfs_file* f, const char* path){
    f->base = fopen(path, "wb");
    f->name = path;

    if (f->base == NULL) {
        printf("error: failed to open file; %s\n", f->name);
        return 1;
    }

    return 0;
}

TFSDEF int tfs_load_text(tfs_file* f, const char* path){
    f->base = fopen(path, "rb");
    f->name = path;

    if (f->base == NULL) {
        printf("error: failed to open file; %s\n", f->name);
        return 1;
    }

    tfs_get_file_size(f);
    f->buffer = (uint8_t*)malloc(f->size + 1);

    if (!f->buffer){
        printf("error: failed to create file buffer; %s\n", f->name);
        return 1;
    }

    size_t read = fread(f->buffer, 1, f->size, f->base);
    f->buffer[f->size] = '\0';
    fclose(f->base);

    if (read != f->size){
        printf("error: file size is different\n");
        free(f->buffer);
        return 1;
    }

    return 0;
}

TFSDEF int tfs_read_binary_buffer(tfs_file* file){
    fseek(file->base, 0, SEEK_END);
    file->size = ftell(file->base);
    rewind(file->base);
    
    file->buffer = (uint8_t*)malloc(file->size);

    if (!file->buffer){
        printf("error: failed to create file buffer; %s\n", file->name);
        return 1;
    }

    size_t read = fread(file->buffer, 1, file->size, file->base);
    fclose(file->base);

    if (read != file->size){
        printf("error: file size is different\n");
        free(file->buffer);
        return 1;
    }

    return 0;
}

TFSDEF int tfs_write_buffer(tfs_file* file, uint8_t* buffer, size_t size){
    if (!file->base) return 1;
    size_t write = fwrite(buffer, 1, size, file->base);
    fclose(file->base);

    if (write == 0){
        free(file->buffer);
        return 1;
    }

    return 0;
}

TFSDEF void tfs_print_text(tfs_file* f){
    printf("%s\n", f->buffer);
    fflush(stdout);
}

TFSDEF const char* tfs_get_file_extension(tfs_file* f){
    const char* ext = strchr(f->name, '.');

    if (!ext || ext == f->name) {
        return "";
    }

    return ext + 1;
}

TFSDEF void tfs_file_free(tfs_file* f){
    free(f->buffer);
    f->size = 0;
    f->name = NULL;
}

#endif


#ifdef __cplusplus
}
#endif
