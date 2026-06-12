#include "malloc.h"

int main()
{
	char	*ptr;
	ptr = ft_malloc(10);
	// ptr[0] = 'a';
	// ptr[1] = 0;
	print_context();
	ft_free(ptr);
	print_context();
	return 0;
}