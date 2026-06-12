#include "malloc.h"

size_t	align_up(size_t size)
{
	if (size < TINY)
		return TINY;
	if (size < SMALL)
		return SMALL;
	if (size < LARGE)
		return LARGE;
	return (size + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1);
}

void	append_chunk_node(t_memory_chunk *curr, t_memory_chunk *new)
{
	t_memory_chunk	*prev;
	t_memory_chunk	*next;

	next = curr->next;
	prev = curr;
	new->next = next;
	new->prev = prev;
	if (next)
		next->prev = new;
	if (prev)
		prev->next = new;
}

void	split_chunk(t_memory_chunk *chunk, size_t size)
{
	size_t			size_left;
	t_memory_chunk	*new_chunk;
	t_ctx			*ctx;

	ctx = get_context();
	size_left = chunk->size - size - ctx->memory_chunk_s;
	if (size == chunk->size || size_left < TINY)
		return ;
	chunk->size = size;
	chunk->is_free = false;
	new_chunk = (t_memory_chunk*)((char*)chunk + ctx->memory_chunk_s + size);
	new_chunk->is_free = true;
	append_chunk_node(chunk, new_chunk);
	new_chunk->size = size_left;
	new_chunk->page = chunk->page;
}

t_memory_chunk	*find_free_chunk(t_page *page, size_t size)
{
	t_memory_chunk	*chunk;

	while (page)
	{
		chunk = page->first_chunk;
		while (chunk)
		{
			if (chunk->is_free && chunk->size >= size)
				return (chunk);
			chunk = chunk->next;
		}
		if (!page->next)
			break;
		page = page->next;
	}
	return (NULL);
}

t_memory_chunk	*get_free_chunk(size_t size)
{
	t_page			*page;
	t_page			*new_page;
	t_memory_chunk	*chunk;

	page = get_page_list(size);
	chunk = find_free_chunk(page, size);
	if (chunk)
		return chunk;
	new_page = create_page(size, 0);
	if (!new_page)
		return NULL;
	add_page(page, new_page);
	chunk = new_page->first_chunk;
	return chunk;
}
