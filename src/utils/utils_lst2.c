/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_lst2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcamilo- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/28 15:23:10 by lcamilo-          #+#    #+#             */
/*   Updated: 2023/01/28 15:23:17 by lcamilo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// Affiche les char * de la linked list
/// \param lst La linked list a afficher
void	printf_list(t_cmd *lst)
{
	ft_printf("[");
	while (lst)
	{
		ft_printf("%s", lst->cmd);
		lst = lst->next;
	}
	ft_printf("]\n");
}

/// Permet de retirer une node de la linked list
/// \param lst Linked list dont on va enlever la node
/// \return la nouvelle Linked list
t_cmd	*ft_pop(t_cmd *lst, t_shell *shell)
{
	t_cmd	*tmp;

	tmp = NULL;
	if (lst && lst == shell->cmd)
	{
		shell->cmd->next->prev = NULL;
		tmp = shell->cmd->next;
		free(shell->cmd->cmd);
		free(shell->cmd);
		shell->cmd = tmp;
	}
	else if (lst)
	{
		if (lst->prev)
			lst->prev->next = lst->next;
		if (lst->next)
			lst->next->prev = lst->prev;
		tmp = lst->next;
		free(lst->cmd);
		free(lst);
	}
	return (tmp);
}

/// Permet de calculer la longueur total d'une string jusqu'a retrouver
///le char c
/// \param shell Structure Shell
/// \param c le char c a retrouver
/// \return la longueur total des strings de la Linked list
size_t	len_cmd(t_cmd *tmp, char c)
{
	size_t	i;

	i = 0;
	while (tmp->cmd[0] != c)
	{
		i += ft_strlen(tmp->cmd);
		tmp = tmp->next;
	}
	return (i);
}
