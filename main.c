#include "header.h"

static t_ctx	*ctx;

t_ctx	*get_context()
{
	return ctx;
}

size_t	get_page_size(size_t size)
{
	size_t	size_needed;

	size_needed = ctx->struct_sizes.os_page_size;
	size += ctx->struct_sizes.page_t + ctx->struct_sizes.memory_chunk_t;
	while (size_needed < size)
		size_needed += ctx->struct_sizes.os_page_size;
	return size_needed;
}

size_t	align_up(size_t size)
{
	return (size + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1);
}

void	fill_struct_sizes()
{
	if (!ctx)
		return ;
	ctx->struct_sizes.ctx_t = align_up(sizeof(t_ctx));
	ctx->struct_sizes.memory_chunk_t = align_up(sizeof(t_memory_chunk));
	ctx->struct_sizes.page_t = align_up(sizeof(t_page));
	ctx->struct_sizes.os_page_size = getpagesize();
}

void	init_context(void	*ptr, void *page_addr)
{
	ctx = ptr;
	ctx->first_page = page_addr;
	fill_struct_sizes();
  
	ctx->free_memory_chunk = (t_memory_chunk*)((char*)ptr + ctx->struct_sizes.ctx_t);
	ctx->free_memory_chunk->is_free = true;
	ctx->free_memory_chunk->next = NULL;
	ctx->free_memory_chunk->prev = NULL;
	ctx->free_memory_chunk->page = page_addr;
	ctx->free_memory_chunk->size = 
		ctx->struct_sizes.os_page_size - 
		(ctx->struct_sizes.memory_chunk_t +
		ctx->struct_sizes.ctx_t +
		ctx->struct_sizes.page_t);
}

void	create_context()
{
	void		*addr;
	t_page	*page;
	size_t	os_page_size;

	os_page_size = getpagesize();
	addr = mmap(NULL, os_page_size, PROT_WRITE | PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (addr = (void*)-1)
		return;
	page = addr;
	page->next = NULL;
	page->prev = NULL;
	page->used_memory_chunk = NULL;
	page->page_size = os_page_size;
	init_context((char*)addr + align_up(sizeof(t_page)), addr);
}

void	add_page(t_page *page)
{
	t_page	*prev;

	prev = ctx->first_page;
	while (prev->next)
		prev = prev->next;
	prev->next = page;
	page->prev = prev;
}

void	add_free_chunk(t_memory_chunk *chunk)
{
	t_memory_chunk	*prev;

	prev = ctx->free_memory_chunk;
	while (prev->next)
		prev = prev->next;
	prev->next = chunk;
	chunk->prev = prev;
}

t_memory_chunk	*create_new_page(size_t size)
{
	void						*addr;
	t_page					*page;
	t_memory_chunk	*chunk;
	size_t					align_size;

	align_size = get_page_size(size);
	addr = mmap(NULL, align_size, PROT_WRITE | PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (addr == (void*)-1)
		return NULL;
	page = addr;
	page->page_size = align_size;
	page->used_memory_chunk = NULL;
	page->next = NULL;
	add_page(page);
	chunk = (t_memory_chunk*)((char*)addr + ctx->struct_sizes.page_t);
	chunk->is_free = true;
	chunk->next = NULL;
	add_free_chunk(chunk);
	chunk->page = addr;
	chunk->size = align_size - (ctx->struct_sizes.page_t + ctx->struct_sizes.memory_chunk_t);
	return chunk;
}

void	remove_memory_chunk(t_memory_chunk *chunk)
{
	t_memory_chunk	*prev;
	t_memory_chunk	*next;

	
}

void	split_free_chunk(t_memory_chunk *chunk, size_t size)
{
	t_page					*actual_page;
	t_memory_chunk	*new_chunk;

	actual_page = chunk->page;
	if ()
}

void	*allocate_memory(size_t size)
{
	t_page					*page;
	t_memory_chunk	*chunk;

	page = ctx->first_page;
	chunk = ctx->free_memory_chunk;
	while (chunk)
	{
		if (chunk->size >= size)
			break;
		else
			chunk = chunk->next;
	}
	if (!chunk)
		chunk = create_new_page(size);
	split_free_chunk(chunk, size);
}

void  ft_malloc(size_t size)
{
	void	*ptr;

	if (!ctx)
	{
		create_context();
		if (!ctx)
			return NULL;
	}
	ptr = allocate_memory(align_up(size));
}