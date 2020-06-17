/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snechaev <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/21 16:19:21 by snechaev          #+#    #+#             */
/*   Updated: 2019/02/22 18:09:54 by snechaev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmap(char const *s, char (*f)(char))
{
	char	*p;
	int		i;

	if (!s || !f)
		return (NULL);
	p = (ft_strnew(ft_strlen(s)));
	if (!p)
		return (NULL);
	i = 0;
	while (s[i])
	{
		p[i] = f(s[i]);
		i++;
	}
	return (p);
}
