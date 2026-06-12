#include "malloc.h"

static t_ctx	*ctx;

t_ctx	*get_context()
{
	return ctx;
}

void	fill_struct_sizes()
{
	if (!ctx)
		return ;
	ctx->ctx_s = ALIGN_UP(sizeof(t_ctx), ALIGNMENT);
	ctx->memory_chunk_s = ALIGN_UP(sizeof(t_memory_chunk), ALIGNMENT);
	ctx->page_s = ALIGN_UP(sizeof(t_page), ALIGNMENT);
	ctx->os_page_size = getpagesize();
}

size_t	get_page_size(size_t size)
{
	size_t	size_needed;

	size_needed = ctx->os_page_size;
	size += ctx->page_s + ctx->memory_chunk_s;
	while (size_needed < size)
		size_needed += ctx->os_page_size;
	return size_needed;
}

t_ctx	*create_context()
{
	void		*addr;
	t_page	*page;
	size_t	os_page_size;

	os_page_size = getpagesize();
	addr = mmap(NULL, os_page_size, PROT_WRITE | PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (addr == (void*)-1)
		return NULL;
	page = addr;
	page->next = NULL;
	page->prev = NULL;
	page->page_size = os_page_size;
	ctx = (t_ctx*)((char*)addr + align_up(sizeof(t_page)));
	ctx->tiny = page;
	fill_struct_sizes();
	page->first_chunk = (t_memory_chunk*)((char*)ctx + ctx->ctx_s);
	page->first_chunk->is_free = true;
	page->first_chunk->next = NULL;
	page->first_chunk->prev = NULL;
	page->first_chunk->page = addr;
	page->first_chunk->size = os_page_size - (ctx->ctx_s + ctx->memory_chunk_s + ctx->page_s);
	return ctx;
}

t_memory_chunk	*create_new_page(size_t size)
{
	void	*addr;
	t_page	*page;
	size_t	align_size;

	align_size = get_page_size(size);
	addr = mmap(NULL, align_size, PROT_WRITE | PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (addr == (void*)-1)
		return NULL;
	page = addr;
	page->page_size = align_size;
	page->next = NULL;
	add_page(page, ctx);
	page->first_chunk = (t_memory_chunk*)((char*)addr + ctx->page_s);
	page->first_chunk->is_free = true;
	page->first_chunk->next = NULL;
	page->first_chunk->prev = NULL;
	page->first_chunk->page = addr;
	page->first_chunk->size = align_size - (ctx->page_s + ctx->memory_chunk_s);
	return page->first_chunk;
}
