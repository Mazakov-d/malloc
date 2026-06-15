/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mazakov <mazakov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 17:09:53 by dmazari           #+#    #+#             */
/*   Updated: 2026/06/15 13:18:54 by mazakov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_called_printf(char c, va_list ap, int fd)
{
	void	*ptr;

	if (c == 'c')
		return (ft_putchar_fd(va_arg(ap, int), fd));
	if (c == 's')
		return (ft_putstr_fd(va_arg(ap, char *), fd));
	if (c == 'p')
	{
		ptr = va_arg(ap, void *);
		if (!ptr)
			return (ft_putstr_fd("(nil)", fd));
		return (ft_put_p_fd((uintptr_t)ptr, "0123456789abcdef", 0, fd));
	}
	if (c == 'd' || c == 'i')
		return (ft_putn_fd(va_arg(ap, int), fd));
	if (c == 'u')
		return (ft_putnbr_base_fd(va_arg(ap, unsigned int),
				"0123456789", fd));
	if (c == 'x')
		return (ft_putnbr_base_fd(va_arg(ap, unsigned int),
				"0123456789abcdef", fd));
	if (c == 'X')
		return (ft_putnbr_base_fd(va_arg(ap, unsigned int),
				"0123456789ABCDEF", fd));
	if (c == '%')
		return (ft_putchar_fd('%', fd));
	return (-1);
}

int	ft_printf_fd(int fd, const char *fmt, ...)
{
	va_list	ap;
	size_t	i;
	int		count;
	int		tmp;

	if (write(fd, 0, 0) == -1 || !fmt)
		return (-1);
	count = 0;
	i = 0;
	va_start(ap, fmt);
	while (fmt[i])
	{
		if (fmt[i] == '%')
		{
			if (fmt[i + 1] == 'z' && fmt[i + 2] == 'u')
			{
				tmp = ft_putzu_fd(va_arg(ap, size_t), fd);
				if (tmp == -1)
					return (va_end(ap), -1);
				count += tmp;
				i += 3;
				continue ;
			}
			tmp = ft_called_printf(fmt[++i], ap, fd);
			if (tmp == -1)
				return (va_end(ap), -1);
			count += tmp;
		}
		else
			count += ft_putchar_fd(fmt[i], fd);
		i++;
	}
	va_end(ap);
	return (count);
}
