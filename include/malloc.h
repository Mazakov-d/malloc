#ifndef MALLOC_H
# define MALLOC_H

#include "libft.h"
#include <sys/mman.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys/resource.h>

#define TINY 32
#define SMALL 128
#define MEDIUM 512
#define LARGE 1024
#define GIANT 2048

void	*ft_malloc(size_t size);



#endif