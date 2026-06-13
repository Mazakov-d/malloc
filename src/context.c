#include "malloc.h"

static t_ctx	*g_ctx;

t_ctx	*get_context(void)
{
	return (g_ctx);
}

void	fill_struct_sizes(void)
{
	if (!g_ctx)
		return ;
	g_ctx->ctx_s = ALIGN_UP(sizeof(t_ctx), ALIGNMENT);
	g_ctx->memory_chunk_s = ALIGN_UP(sizeof(t_memory_chunk), ALIGNMENT);
	g_ctx->page_s = ALIGN_UP(sizeof(t_page), ALIGNMENT);
	g_ctx->os_page_size = getpagesize();
}

size_t	set_context(void *ptr)
{
	g_ctx = ptr;
	if (!g_ctx)
		return (0);
	fill_struct_sizes();
	g_ctx->small = NULL;
	g_ctx->large = NULL;
	g_ctx->tiny = NULL;
	g_ctx->others = NULL;
	return (ALIGN_UP(sizeof(t_ctx), ALIGNMENT));
}

t_ctx	*create_context(void)
{
	t_page	*tiny;
	t_page	*small;
	t_page	*large;

	tiny = create_page(TINY, 1);
	if (!tiny)
		return (NULL);
	small = create_page(SMALL, 0);
	if (!small)
	{
		munmap(tiny, tiny->page_size);
		return (NULL);
	}
	large = create_page(LARGE, 0);
	if (!large)
	{
		munmap(tiny, tiny->page_size);
		munmap(small, small->page_size);
		return (NULL);
	}
	g_ctx->tiny = tiny;
	g_ctx->small = small;
	g_ctx->large = large;
	return (g_ctx);
}

void	clear_ctx(void)
{
	if (g_ctx->tiny)
		remove_unused_pages(g_ctx->tiny->next);
	if (g_ctx->small)
		remove_unused_pages(g_ctx->small->next);
	if (g_ctx->large)
		remove_unused_pages(g_ctx->large->next);
	if (g_ctx->others)
		remove_unused_pages(g_ctx->others->next);
	if ((g_ctx->tiny && g_ctx->tiny->next)
		|| (g_ctx->small && g_ctx->small->next)
		|| (g_ctx->large && g_ctx->large->next)
		|| (g_ctx->others && g_ctx->others->next))
		return ;
	if (is_page_unused(g_ctx->tiny)
		&& is_page_unused(g_ctx->small)
		&& is_page_unused(g_ctx->large)
		&& is_page_unused(g_ctx->others))
	{
		munmap(g_ctx->small, g_ctx->small->page_size);
		munmap(g_ctx->large, g_ctx->large->page_size);
		if (g_ctx->others)
			munmap(g_ctx->others, g_ctx->others->page_size);
		munmap(g_ctx->tiny, g_ctx->tiny->page_size);
		g_ctx = NULL;
	}
}
