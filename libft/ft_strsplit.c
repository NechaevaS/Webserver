/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsplit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snechaev <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/26 12:36:54 by snechaev          #+#    #+#             */
/*   Updated: 2019/02/26 17:41:10 by snechaev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	ft_arr_fill(char **ar, char const *s, char c)
{
	char	**pa;
	char	*p;
	char	*sw;

	pa = ar;
	p = (char *)s;
	sw = 0;
	while (*p)
	{
		if ((*p != c) && ((*(p - 1) == c) || p == s))
			sw = p;
		else if (sw && *p == c && *(p - 1) != c)
		{
			*pa = ft_strnew(p - sw);
			ft_strncpy(*pa, sw, p - sw);
			sw = 0;
			pa++;
		}
		p++;
	}
	if (sw)
		*pa++ = ft_strdup(sw);
	*pa = NULL;
}

char		**ft_strsplit(char const *s, char c)
{
	char	**arr;

	if (!s)
		return (NULL);
	arr = (char **)malloc(sizeof(char*) * ft_cnt_words(s, c) + 1);
	if (!arr)
		return (NULL);
	ft_arr_fill(arr, s, c);
	return (arr);
}
