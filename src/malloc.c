#include "malloc.h"


typedef struct s_memory_chunk {
	size_t					size;
	t_page					*page;
	struct s_memory_chunk	*next;
	struct s_memory_chunk	*prev;
} t_memory_chunk;

typedef struct s_page {
	void			*addr;
	size_t			page_size;
	t_memory_chunk	*used_memory_chunk;
	struct s_page	*prev;
	struct s_page	*next;
} t_page;

typedef struct s_ctx {
	t_page			*first_page;
	t_memory_chunk	*free_memory_chunk;
} t_ctx;

static t_ctx *ctx = NULL;

size_t	get_chunk_size(size_t size)
{
	if (size <= TINY)
		return TINY;
	if (size <= SMALL)
		return SMALL;
	if (size <= MEDIUM)
		return MEDIUM;
	if (size <= LARGE)
		return LARGE;
	return size;
}

void	create_new_ctx()
{
	void			*addr;

	addr = mmap(NULL, 4096, PROT_WRITE | PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (addr == (void*)-1)
		return ;
	ctx = addr + get_chunk_size(sizeof(t_page));
	ctx->first_page = addr;
	ctx->first_page->addr = addr;
	ctx->first_page->page_size = 4096;
	ctx->first_page->next = NULL;
	ctx->first_page->prev = NULL;
	ctx->first_page->used_memory_chunk = NULL;
	ctx->free_memory_chunk = addr + get_chunk_size(sizeof(t_page)) + get_chunk_size(sizeof(t_ctx));
	ctx->free_memory_chunk->page = ctx->first_page;
	ctx->free_memory_chunk->next = NULL;
	ctx->free_memory_chunk->prev = NULL;
	ctx->free_memory_chunk->size = 4096 - get_chunk_size(sizeof(t_ctx)) - get_chunk_size(sizeof(t_page)) - get_chunk_size(sizeof(t_memory_chunk));
	return ;
}

void	add_new_page(size_t size)
{
	void	*addr;
	t_page	*new_page;
	t_page	*previous_page;

	if (size <= 4096 - get_chunk_size(sizeof(t_page) - get_chunk_size(sizeof(t_memory_chunk))))
		size = 4096;
	addr = mmap(NULL, size, PROT_WRITE | PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (addr == (void*)-1)
		return ;
	new_page = addr;
	previous_page = ctx->first_page;
	while (previous_page->next)
		previous_page = previous_page->next;
	new_page->prev = previous_page;
	previous_page->next = new_page;
	new_page->next = NULL;
	new_page->allocated_memory_chunk = NULL;
	new_page->free_memory_chunk = addr + get_chunk_size(sizeof(t_page));
	new_page->free_memory_chunk->next = NULL;
	new_page->free_memory_chunk->prev = NULL;
	new_page->free_memory_chunk->size = size - get_chunk_size(sizeof(t_page)) - get_chunk_size(sizeof(t_memory_chunk));
}

void	*find_enough_free_memory(size_t size)
{
	void	*save_ptr;

	save_ptr = ctx->first_page->free_memory_chunk;
	while (ctx->first_page->free_memory_chunk)
	{
		if (!ctx->first_page->free_memory_chunk->next)
			break;
		if (ctx->first_page->free_memory_chunk->size < size)
			ctx->first_page->free_memory_chunk = ctx->first_page->free_memory_chunk->next;
		else
			break;
	}
	if (ctx->first_page->free_memory_chunk->size <= get_chunk_size(size))
		add_new_page(size);

}

void	*allocate_memory(size_t size)
{
	size_t	free_size;
	size_t	needed_memory_size;
	void	*memory_chunk;

	needed_memory_size = get_chunk_size(sizeof(t_memory_chunk)) + get_chunk_size(size);
	memory_chunk = find_enough_free_memory(needed_memory_size);
	if (!memory_chunk)
		return NULL;

	if (ctx->first_page->allocated_memory_chunk)
	{
		ctx->first_page->allocated_memory_chunk->next = ctx->first_page->free_memory_chunk;
		ctx->first_page->allocated_memory_chunk->next->prev = ctx->first_page->allocated_memory_chunk;
		ctx->first_page->allocated_memory_chunk = ctx->first_page->allocated_memory_chunk->next;
	}
	else
		ctx->first_page->allocated_memory_chunk = ctx->first_page->free_memory_chunk;
	ctx->first_page->free_memory_chunk += needed_memory_size;
	ctx->first_page->free_memory_chunk->size = free_size - needed_memory_size;
	return (ctx->first_page->allocated_memory_chunk + get_chunk_size(sizeof(t_memory_chunk)));
}

void	*ft_malloc(size_t size)
{
	void	*ptr;
	void	*save;
	t_page	*working_ptr;

	if (!ctx)
	{
		create_new_ctx();
		if (!ctx)
			return NULL;
	}
	ptr = allocate_memory(size);
	working_ptr = ctx->first_page;
	while (working_ptr)
	{
		if (!working_ptr->used_memory_chunk)
		{
			if (working_ptr->next)
				working_ptr->next->prev = working_ptr->prev;
			if (working_ptr->prev)
				working_ptr->prev = working_ptr->next;
			save = working_ptr->next;
			munmap(working_ptr->addr, working_ptr->page_size);
			working_ptr = save;
		}
		else
			working_ptr = working_ptr->next;
	}
	return ptr;
}

void	*ft_realloc(void *ptr, size_t size)
{
	t_memory_chunk	*chunk;
	void			*new_ptr;

	if (!ptr)
		return ft_malloc(size);
	chunk = ptr - sizeof(t_memory_chunk);
	if (chunk->size - get_chunk_size(sizeof(t_memory_chunk)) >= size)
		return ptr;
	new_ptr = ft_malloc(size);
	if (!new_ptr)
	{
		errno = EAGAIN;
		return (ptr);
	}
	return NULL;
}