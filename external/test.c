#include <assert.h>
#include <stdint.h>
#define TFS_IMPL
#include "tfs.h"

#define HSIZE 10

int main() {

    unsigned char header[HSIZE];

    char magic[4] = {'V', 'A', 'L', '1'};
    uint16_t ver = 1;
    uint32_t size = 1234; 

    memcpy(header + 0, magic, sizeof(magic));
    memcpy(header + 4, &ver, sizeof(ver));
    memcpy(header + 6, &size, sizeof(size));

    tfs_file file;
    tfs_file txt;

    tfs_open_binary_read(&txt, "example.txt");
    tfs_open_binary_write(&file, "example.val");

    size_t fsize = HSIZE + txt.size;
    uint8_t* new_buffer = (uint8_t*)malloc(fsize);

    memcpy(new_buffer + 0, header, HSIZE);
    memcpy(new_buffer + HSIZE, txt.buffer, txt.size);

    tfs_write_buffer(&file, new_buffer, fsize);
    free(new_buffer);

    tfs_file_free(&txt);
    return 0;
}
