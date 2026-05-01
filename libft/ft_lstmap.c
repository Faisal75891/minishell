/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbaras <fbaras@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 23:32:26 by fbaras            #+#    #+#             */
/*   Updated: 2025/07/23 23:32:26 by fbaras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*list;
	void	*temp;
	t_list	*new_node;

	list = NULL;
	while (lst != NULL)
	{
		temp = f(lst->content);
		if (!temp)
		{
			ft_lstclear(&list, del);
			return (NULL);
		}
		new_node = ft_lstnew(temp);
		if (!new_node)
		{
			del(temp);
			ft_lstclear(&list, del);
			return (NULL);
		}
		ft_lstadd_back(&list, new_node);
		lst = lst->next;
	}
	return (list);
}
