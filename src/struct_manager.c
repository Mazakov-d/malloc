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

void	add_page(t_page *page, t_ctx *ctx, int Flag)
{
	t_page	*prev;
	t_page	*ptr;

	if (Flag == TINY)
		ptr = ctx->tiny;
	else if (Flag == SMALL)
		ptr = ctx->small;
	else if (Flag == LARGE)
		ptr = ctx->large;
	if (!ptr)
	{
		page->prev = NULL;
		ptr = page;
		return ;
	}
	prev = ptr;
	while (prev->next)
		prev = prev->next;
	prev->next = page;
	page->prev = prev;
}

t_memory_chunk	*get_free_chunk(size_t size)
{
	t_page			*page;
	t_memory_chunk	*chunk;
	t_ctx			*ctx;

	ctx = get_context();
	page = ctx->tiny;
	while (page)
	{
		chunk = page->first_chunk;
		while (chunk)
		{
			if (chunk->is_free && chunk->size >= size)
				break;
			else
				chunk = chunk->next;
		}
		page = page->next;
	}
	if (!chunk)
		chunk = create_new_page(size);
	return chunk;
}
