#ifndef __HELPERS_H_
#define __HELPERS_H_

#include <unistd.h>

extern
ssize_t _read(int fd, void *buf, size_t count);

extern
ssize_t _write(int fd, const void *buf, size_t count);

#endif // __HELPERS_H_
