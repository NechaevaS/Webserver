/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_str_rev.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snechaev <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/27 14:11:49 by snechaev          #+#    #+#             */
/*   Updated: 2019/03/05 14:31:07 by snechaev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_str_rev(char *s)
{
	char	*st;
	char	*end;
	char	tmp;

	if (!s || *s == 0)
		return (s);
	st = s;
	end = end + ft_strlen(s) - 1;
	while (st < end)
	{
		tmp = *st;
		*st = *end;
		*end = tmp;
		st++;
		end--;
	}
	return (s);
}
