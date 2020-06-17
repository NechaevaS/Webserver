/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snechaev <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/27 12:02:38 by snechaev          #+#    #+#             */
/*   Updated: 2019/02/27 18:32:06 by snechaev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*ft_fill(char *s, int n)
{
	if (n > 9)
		s = ft_fill(s, n / 10);
	*s = (n % 10) + '0';
	return (s + 1);
}

char		*ft_itoa(int n)
{
	char	*str;

	if (n == -2147483648)
		return (ft_strdup("-2147483648"));
	str = ft_strnew(11);
	if (!str)
		return (NULL);
	if (n < 0)
	{
		str[0] = '-';
		ft_fill(str + 1, -n);
		return (str);
	}
	ft_fill(str, n);
	return (str);
}
