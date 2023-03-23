#ifndef DATA_H
#define DATA_H

struct info{
    char *test;
    char *test2;
};

struct info *data(int fd, const void *buf, size_t count);

#endif