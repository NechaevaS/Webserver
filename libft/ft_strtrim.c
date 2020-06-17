/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snechaev <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/25 12:50:44 by snechaev          #+#    #+#             */
/*   Updated: 2019/02/26 17:00:56 by snechaev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s)
{
	unsigned int	start;
	unsigned int	len;

	if (!s)
		return (0);
	start = 0;
	while (ft_iswsps(s[start]))
	{
		start++;
	}
	start = (ft_skipws((char *)s));
	len = ft_strlen(s);
	while (len > start && ft_iswsps(s[len - 1]))
	{
		len--;
	}
	return (ft_strsub(s, start, len - start));
}
