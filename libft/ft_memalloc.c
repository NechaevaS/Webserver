/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memalloc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snechaev <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/21 12:21:17 by snechaev          #+#    #+#             */
/*   Updated: 2019/02/21 14:03:30 by snechaev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memalloc(size_t size)
{
	unsigned char	*p;

	p = (unsigned char *)malloc(size);
	if (p)
	{
		p = ft_bzero(p, size);
		return ((void *)p);
	}
	else
		return (NULL);
}
