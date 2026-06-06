#ifndef MALLOC_H
# define MALLOC_H

#include "libft.h"
#include <sys/mman.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys/resource.h>

#define TINY 32
#define SMALL 64
#define MEDIUM 256
#define LARGE 512

void	*ft_malloc(size_t size);



#endif