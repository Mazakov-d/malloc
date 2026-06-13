#include "malloc.h"

bool	is_page_unused(t_page	*page)
{
	t_memory_chunk	*chunk;

	if (!page)
		return (true);
	chunk = page->first_chunk;
	while (chunk)
	{
		if (!chunk->is_free)
			return (false);
		chunk = chunk->next;
	}
	return (true);
}

size_t	get_page_size(size_t size)
{
	size_t	allocations_size;
	size_t	page_size;

	if (size <= TINY)
		allocations_size = 100 * TINY;
	else if (size <= SMALL)
		allocations_size = 100 * SMALL;
	else if (size <= LARGE)
		allocations_size = 100 * LARGE;
	else
		allocations_size = size;
	page_size = getpagesize();
	if (allocations_size < page_size)
		return (page_size);
	if (allocations_size % page_size)
		return (page_size * ((allocations_size / page_size) + 1));
	return (page_size * (allocations_size / page_size));
}

t_page	*create_page(size_t	size, int flag_ctx)
{
	void	*addr;
	size_t	page_size;
	size_t	offset;
	t_page	*page;

	page_size = get_page_size(size);
	addr = mmap(NULL, page_size, PROT_WRITE | PROT_READ,
			MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (addr == (void *)-1)
		return (NULL);
	page = addr;
	page->next = NULL;
	page->prev = NULL;
	page->page_size = page_size;
	offset = ALIGN_UP(sizeof(t_page), ALIGNMENT);
	if (flag_ctx)
		offset += set_context((char *)addr + offset);
	page->first_chunk = (t_memory_chunk *)((char *)addr + offset);
	page->first_chunk->is_free = true;
	page->first_chunk->next = NULL;
	page->first_chunk->prev = NULL;
	page->first_chunk->page = addr;
	offset += ALIGN_UP(sizeof(t_memory_chunk), ALIGNMENT);
	page->first_chunk->size = page_size - (ALIGN_UP(sizeof(t_page), ALIGNMENT) + ALIGN_UP(sizeof(t_memory_chunk), ALIGNMENT));
	return (page);
}

void	add_page(t_page	**curr, t_page	*new)
{
	t_page	*prev;

	if (!*curr)
	{
		*curr = new;
		return ;
	}
	prev = *curr;
	while (prev && prev->next)
		prev = prev->next;
	if (prev)
		prev->next = new;
	new->prev = prev;
}

t_page	**get_page_list(size_t size)
{
	t_ctx	*g_ctx;

	g_ctx = get_context();
	if (size <= TINY)
		return (&g_ctx->tiny);
	if (size <= SMALL)
		return (&g_ctx->small);
	if (size <= LARGE)
		return (&g_ctx->large);
	return (&g_ctx->others);
}
