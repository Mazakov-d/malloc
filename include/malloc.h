#ifndef MALLOC_H
# define MALLOC_H

#include "libft.h"
#include <sys/mman.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys/resource.h>

#define TINY 128
#define SMALL 1024

typedef struct s_memory_chunk {
	size_t					size;
	bool					is_free;
	void					*page;
	struct s_memory_chunk	*next;
	struct s_memory_chunk	*prev;
} t_memory_chunk;

typedef struct s_page {
	size_t			page_size;
	t_memory_chunk	*used_memory_chunk;
	struct s_page	*prev;
	struct s_page	*next;
} t_page;

typedef struct s_ctx {
	t_page			*first_page;
	t_memory_chunk	*free_memory_chunk;
} t_ctx;

void			*ft_malloc(size_t size);
void			free(void *ptr);
void			*realloc(void *ptr, size_t size);

t_ctx			*get_context();
t_page			*last_page();
t_memory_chunk	*first_memory_chunk(t_memory_chunk *chunk);
t_memory_chunk	*last_memory_chunk(t_memory_chunk *chunk);
void			remove_node(t_memory_chunk *ptr);
size_t			get_chunk_size(size_t size);

#endif