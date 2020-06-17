/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snechaev <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/19 14:19:06 by snechaev          #+#    #+#             */
/*   Updated: 2019/02/21 12:39:44 by snechaev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	unsigned int	i;
	int				cp;

	i = 0;
	cp = 0;
	while (*dst && i < dstsize)
	{
		dst++;
		i++;
	}
	while (*src)
	{
		if (i < (dstsize - 1) && dstsize != 0)
		{
			*dst = *src;
			dst++;
			cp = 1;
		}
		src++;
		i++;
	}
	if (cp == 1)
		*dst = '\0';
	return (i);
}
