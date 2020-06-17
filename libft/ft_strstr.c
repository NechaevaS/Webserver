/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strstr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snechaev <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/15 11:42:52 by snechaev          #+#    #+#             */
/*   Updated: 2019/02/21 12:46:37 by snechaev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

static int	compare(const char *str, const char *to_find)
{
	int i;

	i = 0;
	while (str[i] != '\0' && to_find[i] != '\0')
	{
		if (str[i] != to_find[i])
			return (0);
		i++;
	}
	if (to_find[i] == '\0')
		return (1);
	return (0);
}

char		*ft_strstr(const char *str, const char *to_find)
{
	int i;

	i = 0;
	if (to_find[i] == '\0')
		return ((char *)str);
	while (str[i] != '\0')
	{
		if (compare(str + i, to_find) == 1)
			return ((char *)str + i);
		i++;
	}
	return (0);
}
