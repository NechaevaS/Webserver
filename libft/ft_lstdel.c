/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdel.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snechaev <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/01 12:39:07 by snechaev          #+#    #+#             */
/*   Updated: 2019/03/05 14:23:10 by snechaev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstdel(t_list **alst, void (*del)(void *, size_t))
{
	t_list	*tmp;
	t_list	*cur;

	if (!*alst)
		return ;
	cur = *alst;
	while (cur)
	{
		tmp = cur;
		cur = cur->next;
		ft_lstdelone(&tmp, del);
	}
	*alst = (NULL);
}
