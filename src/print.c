#include "malloc.h"

size_t	print_zone(t_page *page)
{
	t_page			*ptr;
	t_memory_chunk	*chunk;
	size_t	total_size;

	total_size = 0;
	ptr = page;
	while (ptr)
	{
		total_size += page->page_size;
		printf("Zone ptr: %p\n", ptr);
		printf("Page size: %zu\n", ptr->page_size);
		chunk = ptr->first_chunk;
		while (chunk)
		{
			printf("\nChunk ptr = %p\n", chunk);
			printf("Is free: %d\n", chunk->is_free);
			printf("Size: %zu\n", chunk->size);
			printf("Ptr to page: %p\n\n", chunk->page);
			chunk = chunk->next;
		}
		ptr = ptr->next;
	}
	return total_size;
}

void	print_context()
{
	t_ctx		*ctx;
	size_t	total_size;

	total_size = 0;
	ctx = get_context();
	if (!ctx)
	{
		printf("Ctx not set.");
		return ;
	}
	printf("The struc ctx point to: %p\n", ctx);
	printf("The sizes of the struct (align up at 8):\n");
	printf("	t_ctx: %zu\n	t_memory: %zu\n	t_page: %zu\n", ctx->ctx_s, ctx->memory_chunk_s, ctx->page_s);
	printf("\nThe different zone point to: \n");
	printf("	tiny: %p\n	small: %p\n	large: %p\n	others: %p\n", ctx->tiny, ctx->small, ctx->large, ctx->others);
	printf("\n\n--TINY--\n");
	total_size += print_zone(ctx->tiny);
	printf("\n\n--SMALL--\n");
	total_size += print_zone(ctx->small);
	printf("\n\n--LARGE--\n");
	total_size = print_zone(ctx->large);
	printf("\n\n--OTHERS--\n");
	total_size += print_zone(ctx->others);
	printf("Total size: %zu\n\n\n", total_size);
}
