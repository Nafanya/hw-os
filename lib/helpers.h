#ifndef __HELPERS_H_
#define __HELPERS_H_

#include <unistd.h>

ssize_t read_(int fd, void *buf, size_t count);

ssize_t write_(int fd, const void *buf, size_t count);

#endif // __HELPERS_H_
