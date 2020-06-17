/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snechaev <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/21 15:34:46 by snechaev          #+#    #+#             */
/*   Updated: 2019/02/22 18:09:06 by snechaev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnew(size_t size)
{
	char	*p;

	p = (char *)malloc(sizeof(char) * (size + 1));
	if (p)
	{
		ft_memset(p, '\0', (size + 1));
		return (p);
	}
	else
		return (NULL);
}
