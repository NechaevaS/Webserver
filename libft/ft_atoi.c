/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snechaev <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/14 14:58:35 by snechaev          #+#    #+#             */
/*   Updated: 2019/02/25 16:33:07 by snechaev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(char *str)
{
	int res;
	int i;
	int neg;

	neg = 1;
	i = ft_skipws(str);
	res = 0;
	if (str[i] == '+')
		i++;
	else if (str[i] == '-')
	{
		neg = -1;
		i++;
		if (str[i] == '2' && str[i + 1] == '1' && str[i + 2] == '4' &&
				str[i + 3] == '7' && str[i + 4] == '4' && str[i + 5] == '8' &&
				str[i + 6] == '3' && str[i + 7] == '6' && str[i + 8] == '4' &&
				str[i + 9] == '8')
			return (-2147483648);
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		res = res * 10 + str[i] - '0';
		i++;
	}
	return (res * neg);
}
