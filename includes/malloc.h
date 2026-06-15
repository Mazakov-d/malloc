#ifndef MALLOC_H
# define MALLOC_H

# include "libft.h"
# include <sys/mman.h>
# include <errno.h>
# include <stdbool.h>
# include <stdio.h>
# include <sys/resource.h>

# define ALIGNMENT 8
# define TINY 128
# define SMALL 512
# define LARGE 1024

# define ALIGN_UP(size, align) (((size) + ((align) - 1)) & ~((align) - 1))

typedef struct s_memory_chunk
{
	size_t					size;
	bool					is_free;
	void					*page;
	struct s_memory_chunk	*next;
	struct s_memory_chunk	*prev;
}	t_memory_chunk;

typedef struct s_page
{
	size_t			page_size;
	t_memory_chunk	*first_chunk;
	struct s_page	*prev;
	struct s_page	*next;
}	t_page;

typedef struct s_ctx
{
	t_page	*tiny;
	t_page	*small;
	t_page	*large;
	t_page	*others;
	size_t	memory_chunk_s;
	size_t	ctx_s;
	size_t	page_s;
	size_t	os_page_size;
}	t_ctx;

/*
** context.c
*/
t_ctx			*get_context(void);
void			fill_struct_sizes(void);
size_t			set_context(void *ptr);
t_ctx			*create_context(void);
void			clear_ctx(void);

/*
** malloc.c
*/
void			*allocate_memory(size_t size);
void			*malloc(size_t size);

/*
** struct_manager.c
*/
size_t			align_up(size_t size);
void			append_chunk_node(t_memory_chunk *curr, t_memory_chunk *new);
void			split_chunk(t_memory_chunk *chunk, size_t size);
t_memory_chunk	*find_free_chunk(t_page *page, size_t size);
t_memory_chunk	*get_free_chunk(size_t size);

/*
** pages.c
*/
bool			is_page_unused(t_page *page);
size_t			get_page_size(size_t size);
t_page			*create_page(size_t size, int flag_ctx);
void			add_page(t_page	**curr, t_page	*new);
t_page			**get_page_list(size_t size);

/*
** print.c
*/
size_t			print_zone(t_page *page);
void			show_alloc_mem(void);

/*
** free.c
*/
void			resize_chunk(t_memory_chunk *chunk, size_t memory_chunk_s);
void			colapse_chunk(t_page *page);
void			remove_unused_pages(t_page *page);
void			free(void *ptr);

/*
** realloc.c
*/
void			*realloc(void *ptr, size_t size);

#endif