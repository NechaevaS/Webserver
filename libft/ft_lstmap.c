/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snechaev <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/01 15:48:57 by snechaev          #+#    #+#             */
/*   Updated: 2019/03/01 16:47:53 by snechaev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, t_list *(*f)(t_list *elem))
{
	t_list	*new;
	t_list	**p;

	if (lst && f)
	{
		new = NULL;
		p = &new;
		while (lst)
		{
			*p = f(lst);
			p = &((*p)->next);
			lst = lst->next;
		}
		return (new);
	}
	return (NULL);
}
