#include "malloc.h"

int main(void)
{
	char	*ptr;

	ptr = ft_malloc(10);
	// print_context();
	ft_strlcpy(ptr, "salut", 6);
	ptr = ft_realloc(ptr,20);
	ft_printf_fd(1, "%s\n", ptr);
	ft_free(ptr);
	print_context();
	return 0;
}
