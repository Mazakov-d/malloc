#include "malloc.h"

static t_ctx *ctx = NULL;

t_ctx	*get_context(void)
{
	return ctx;
}

size_t	get_chunk_size(size_t size)
{
	size_t	size_to_allocate;

	if (size <= TINY)
		return TINY;
	if (size <= SMALL)
		return SMALL;
	if (size <= 4096 - get_chunk_size(sizeof(t_page) - get_chunk_size(sizeof(t_memory_chunk))))
		size_to_allocate = 4096;
	else
		size_to_allocate = size + (64 - (size % 8));
	return size_to_allocate;
}

size_t	get_page_size(size_t size)
{
	size_t	page_size;
	size_t	save;

	page_size = getpagesize();
	save = page_size;
	while (page_size < size)
		page_size += save;
	return page_size;
}

void	create_new_ctx()
{
	void			*addr;
	size_t			size;

	size = getpagesize();
	addr = mmap(NULL, size, PROT_WRITE | PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (addr == (void*)-1)
		return ;
	ctx = (t_ctx*)((char*)addr + get_chunk_size(sizeof(t_page)));
	ctx->first_page = addr;
	ctx->first_page->page_size = 4096;
	ctx->first_page->next = NULL;
	ctx->first_page->prev = NULL;
	ctx->first_page->used_memory_chunk = NULL;
	ctx->free_memory_chunk = (t_memory_chunk*)((char*)addr + get_chunk_size(sizeof(t_page)) + get_chunk_size(sizeof(t_ctx)));
	ctx->free_memory_chunk->is_free = true;
	ctx->free_memory_chunk->page = ctx->first_page;
	ctx->free_memory_chunk->next = NULL;
	ctx->free_memory_chunk->prev = NULL;
	ctx->free_memory_chunk->size = size - get_chunk_size(sizeof(t_ctx)) - get_chunk_size(sizeof(t_page)) - get_chunk_size(sizeof(t_memory_chunk));
	return ;
}

t_memory_chunk	*add_new_page(size_t size)
{
	void			*addr;
	t_page			*new_page;
	t_page			*previous_page;
	t_memory_chunk	*free_memory;

	size = get_page_size(size + sizeof(t_page));
	addr = mmap(NULL, size, PROT_WRITE | PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (addr == (void*)-1)
		return NULL;
	new_page = addr;
	new_page->page_size = size;
	new_page->used_memory_chunk = NULL;
	previous_page = last_page(ctx->first_page);
	new_page->prev = previous_page;
	previous_page->next = new_page;
	new_page->next = NULL;
	free_memory = (t_memory_chunk*)((char*)addr + get_chunk_size(sizeof(t_page)));
	free_memory->next = NULL;
	free_memory->prev = NULL;
	free_memory->is_free = true;
	free_memory->size = size - get_chunk_size(sizeof(t_page)) - get_chunk_size(sizeof(t_memory_chunk));
	free_memory->page = new_page;
	return free_memory;
}



void	*find_enough_free_memory(size_t size)
{
	t_memory_chunk	*ptr;

	ptr = ctx->free_memory_chunk;
	while (ptr)
	{
		if (!ptr)
			break;
		if (ptr->size < size)
			ptr = ptr->next;
		else
		{
			if (ptr == ctx->free_memory_chunk)
				ctx->free_memory_chunk = ctx->free_memory_chunk->next;
			break;
		}
	}
	if (!ptr || ptr->size < size)
		ptr = add_new_page(size);
	remove_node(ptr);
	return ptr;
}

void	*allocate_memory(size_t chunk_size)
{
	t_memory_chunk		*memory_chunk;
	t_memory_chunk		*free_chunk;
	t_memory_chunk		*ptr;
	t_page						*actual_page;

	memory_chunk = find_enough_free_memory(chunk_size);
	if (!memory_chunk)
		return NULL;
	actual_page = (t_page*)memory_chunk->page;
	memory_chunk->size = chunk_size - get_chunk_size(sizeof(t_memory_chunk));
	if (ctx->free_memory_chunk)
	{
		free_chunk = last_memory_chunk(ctx->free_memory_chunk);
		free_chunk->next = (t_memory_chunk*)((char*) memory_chunk + get_chunk_size(sizeof(t_memory_chunk)) + chunk_size);
		free_chunk->next->prev = free_chunk;
		free_chunk = free_chunk->next;
	}
	else
	{
		ctx->free_memory_chunk = (t_memory_chunk*)((char*)memory_chunk + chunk_size);
		free_chunk = ctx->free_memory_chunk;
	}
	free_chunk->page = actual_page;
	free_chunk->next = NULL;
	free_chunk->size = memory_chunk->size - (2 * get_chunk_size(sizeof(t_memory_chunk)) + chunk_size);

	if (actual_page->used_memory_chunk)
	{
		ptr = last_memory_chunk(actual_page->used_memory_chunk);
		ptr->next = memory_chunk;
		memory_chunk->prev = ptr;
	}
	else
		actual_page->used_memory_chunk = memory_chunk;
	memory_chunk->is_free = false;
	return ((char*)memory_chunk + get_chunk_size(sizeof(t_memory_chunk)));
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
	ptr = allocate_memory(get_chunk_size(size) + get_chunk_size(sizeof(t_memory_chunk)));
	working_ptr = ctx->first_page;
	while (working_ptr)
	{
		if (!working_ptr->used_memory_chunk)
		{
			if (working_ptr->next)
				working_ptr->next->prev = working_ptr->prev;
			if (working_ptr->prev)
				working_ptr->prev->next = working_ptr->next;
			save = working_ptr->next;
			munmap(working_ptr, working_ptr->page_size);
			working_ptr = save;
		}
		else
			working_ptr = working_ptr->next;
	}
	return ptr;
}


// void	remove_node()
// {

// }

// void	free(void *ptr)
// {
// 	t_memory_chunk	*header;
// 	t_page					*page;

// 	header = ptr - sizeof(t_memory_chunk);
// 	page = (t_page *)header->page;
// 	header->is_free = true;

// }