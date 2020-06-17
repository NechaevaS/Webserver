/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snechaev <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/14 12:03:34 by snechaev          #+#    #+#             */
/*   Updated: 2019/02/21 12:58:24 by snechaev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *str, const char *find, size_t len)
{
	char	*res;
	size_t	flen;

	if (find[0] == '\0')
		return ((char *)str);
	if (len == 0)
		return (0);
	flen = ft_strlen(find);
	res = (char *)str;
	while (1)
	{
		res = ft_strchr(res, find[0]);
		if (!res || (str + len) <= res || (res + flen) > (str + len))
			return (0);
		if (ft_strncmp(res, find, flen) == 0)
			return (res);
		res++;
	}
	return (0);
}
