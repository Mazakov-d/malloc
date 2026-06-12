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

size_t	set_context(void *ptr)
{
	ctx = ptr;
	fill_struct_sizes();
	ctx->small = NULL;
	ctx->large = NULL;
	ctx->tiny = NULL;
	ctx->others = NULL;
	return ALIGN_UP(sizeof(t_ctx), ALIGNMENT);
}

t_ctx	*create_context()
{
	t_page	*tiny;
	t_page	*small;
	t_page	*large;

	tiny = create_page(TINY, 1);
	if (!tiny)
		return NULL;
	small = create_page(SMALL, 0);
	if (!small)
	{
		munmap(tiny, tiny->page_size);
		return NULL;
	}
	large = create_page(LARGE, 0);
	if (!large)
	{
		munmap(tiny, tiny->page_size);
		munmap(small, small->page_size);
		return NULL;
	}
	ctx->tiny = tiny;
	ctx->small = small;
	ctx->large = large;
	return ctx;
}
