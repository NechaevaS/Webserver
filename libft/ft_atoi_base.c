/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi_base.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snechaev <snechaev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/11 13:37:24 by snechaev          #+#    #+#             */
/*   Updated: 2019/09/17 14:41:44 by snechaev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	is_value(char c)
{
	if (c >= '0' && c <= '9')
		return (c - '0');
	else if (c >= 'a' && c <= 'f')
		return (c - 'a' + 10);
	else if (c >= 'A' && c <= 'F')
		return (c - 'A' + 10);
	return (0);
}

int	is_b(char c, int base)
{
	const char	base1[17] = "0123456789abcdef";
	const char	base2[17] = "0123456789ABCDEF";
	int			i;

	i = 0;
	while (i < base)
	{
		if (c == base1[i] || c == base2[i])
			return (1);
		i++;
	}
	return (0);
}

int	ft_atoi_base(const char *str, int str_base)
{
	int	atoi;
	int	sign;

	atoi = 0;
	sign = 1;
	if (str_base < 2 || str_base > 16)
		return (0);
	while (*str != '\0' && (*str == ' ' || *str == '\n' || *str == '\t'
			|| *str == '\f' || *str == '\v' || *str == '\r'))
		str++;
	if (*str == '-')
	{
		sign = -sign;
		str++;
	}
	else if (*str == '+')
		str++;
	while (*str != '\0' && is_b(*str, str_base))
	{
		atoi = (atoi * str_base) + is_value(*str);
		str++;
	}
	return (atoi * sign);
}
