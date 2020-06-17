/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snechaev <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/21 16:19:21 by snechaev          #+#    #+#             */
/*   Updated: 2019/02/22 18:10:32 by snechaev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char			*p;
	int				i;
	unsigned int	index;

	if (!s || !f)
		return (NULL);
	p = (ft_strnew(ft_strlen(s)));
	if (!p)
		return (NULL);
	i = 0;
	index = 0;
	while (s[i])
	{
		p[i] = f(index, s[i]);
		i++;
		index++;
	}
	return (p);
}
