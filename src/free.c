#include "malloc.h"

void	resize_chunk(t_memory_chunk *chunk, size_t memory_chunk_s)
{
	chunk->size += memory_chunk_s + chunk->next->size;
	chunk->next = chunk->next->next;
}

void	colapse_chunk(t_page	*page)
{
	t_ctx	*ctx;
	void	*ptr;
	t_memory_chunk	*chunk;

	ctx = get_context();
	ptr = (char*)page + ctx->page_s;
	if (ptr == ctx)
		ptr = (char*)ptr + ctx->ctx_s;
	chunk = (t_memory_chunk*)ptr;
	while (chunk)
	{
		if (chunk->is_free && chunk->next && chunk->next->is_free)
			resize_chunk(chunk, ctx->memory_chunk_s);
		else
			chunk = chunk->next;
	}
}

void	remove_unused_pages(t_page	*page)
{
	t_page	*ptr;
	t_page	*save;

	ptr = page;
	while (ptr)
	{
		if (is_page_unused(ptr))
		{
			save = ptr->next;
			if (ptr->prev)
				ptr->prev->next = save;
			if (save)
				save->prev = ptr->prev;
			munmap(ptr, ptr->page_size);
			ptr = save;
		}
		else
			ptr = ptr->next;
	}
}

void	ft_free(void *ptr)
{
	t_ctx					*ctx;
	t_memory_chunk	*header;

	ctx = get_context();
	if (!ctx)
		return ;
	header = (t_memory_chunk*)((char*)ptr - ctx->memory_chunk_s);
	header->is_free = true;
	colapse_chunk(header->page);
	clear_ctx();
}