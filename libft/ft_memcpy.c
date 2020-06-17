/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snechaev <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/12 18:04:38 by snechaev          #+#    #+#             */
/*   Updated: 2019/02/13 16:41:12 by snechaev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	char	*csrc;
	char	*cdst;

	csrc = (char	*)src;
	cdst = (char	*)dst;
	while (n > 0)
	{
		*cdst = *csrc;
		csrc++;
		cdst++;
		n--;
	}
	return (dst);
}
