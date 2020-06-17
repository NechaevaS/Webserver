/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sort_list.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snechaev <snechaev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/01 13:37:26 by snechaev          #+#    #+#             */
/*   Updated: 2019/08/01 14:20:52 by snechaev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	swap(void *a, void *b)
{
	void *tmp;

	tmp = a;
	a = b;
	b = tmp;
}

t_list	*ft_sort_list(t_list *lst, int (*cmp)(void *, void *))
{
	t_list *head;
	t_list *current;

	head = lst;
	while (lst)
	{
		current = lst->next;
		while (current)
		{
			if (cmp(lst->content, current->content) == 0)
				swap(&lst->content, &current->content);
			current = current->next;
		}
		lst = lst->next;
	}
	return (head);
}
