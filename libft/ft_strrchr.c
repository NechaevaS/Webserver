/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snechaev <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/13 17:32:37 by snechaev          #+#    #+#             */
/*   Updated: 2019/02/21 12:43:37 by snechaev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *str, int c)
{
	const char	*p;
	int			l;

	l = ft_strlen(str);
	p = &str[l];
	while (l >= 0)
	{
		if (*p == c)
			return ((char *)p);
		l--;
		p--;
	}
	return (NULL);
}
