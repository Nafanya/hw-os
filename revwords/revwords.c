#include "stdlib.h"
#include "string.h"

#include "helpers.h"

int main() {
    size_t buf_size = 1024;
    char *buf = malloc(buf_size);
    
    size_t bytes_read;
    
    while ((bytes_read = read_(STDIN_FILENO, buf, buf_size)) != -1 && bytes_read != 0) {
        char *space = strchr(buf, ' ');
        if (space == NULL) {
            char* new_buf = realloc(buf, buf_size * 2);
            if (new_buf == NULL) {
                free(buf);
                return 1;
            }
            buf = new_buf;
            buf_size *= 2;
        } else {
            
        }
    }
    
    
    return 0;
}
