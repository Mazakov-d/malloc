#include "libft.h"
#include <sys/mman.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys/resource.h>

#define ALIGNMENT 8

typedef struct s_struct_size {
	size_t	memory_chunk_t;
	size_t	ctx_t;
	size_t	page_t;
	size_t	os_page_size;
}	t_struct_size;

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
	t_memory_chunk	*free_memory_chunk;
	struct s_page	*prev;
	struct s_page	*next;
} t_page;

typedef struct s_ctx {
	t_page					*first_page;
	t_struct_size		struct_sizes;
} t_ctx;