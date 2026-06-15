#include "malloc.h"

void	resize_chunk(t_memory_chunk *chunk, size_t memory_chunk_s)
{
	chunk->size += memory_chunk_s + chunk->next->size;
	chunk->next = chunk->next->next;
}

void	colapse_chunk(t_page	*page)
{
	t_ctx			*g_ctx;
	void			*ptr;
	t_memory_chunk	*chunk;

	g_ctx = get_context();
	ptr = (char *)page + g_ctx->page_s;
	if (ptr == g_ctx)
		ptr = (char *)ptr + g_ctx->ctx_s;
	chunk = (t_memory_chunk *)ptr;
	while (chunk)
	{
		if (chunk->is_free && chunk->next && chunk->next->is_free)
			resize_chunk(chunk, g_ctx->memory_chunk_s);
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

void	free(void *ptr)
{
	t_ctx			*g_ctx;
	t_memory_chunk	*header;

	g_ctx = get_context();
	if (!g_ctx)
		return ;
	header = (t_memory_chunk *)((char *)ptr - g_ctx->memory_chunk_s);
	header->is_free = true;
	colapse_chunk(header->page);
	clear_ctx();
}
