/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snechaev <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/13 15:19:17 by snechaev          #+#    #+#             */
/*   Updated: 2019/02/13 15:35:16 by snechaev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	i;
	unsigned char	*str;

	str = (unsigned char *)s;
	i = (unsigned char)c;
	while (n > 0)
	{
		if (*str == i)
			return (str);
		str++;
		n--;
	}
	return (NULL);
}
