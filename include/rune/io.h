#ifndef C_IO_H_
#define C_IO_H_

#include <sys/stat.h>

struct _IO {
    const char *filename;
    const char *buffer;
    struct stat st;
};

#endif /* C_IO_H_ */
