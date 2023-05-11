/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcamilo- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 11:17:16 by lcamilo-          #+#    #+#             */
/*   Updated: 2023/01/16 11:17:18 by lcamilo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// Enleve tout les espaces inutiles, ne garde qu'un seul espace
/// afin de pouvoir parser les commandes futures
/// \param shell Structure shell
void	parse_space(t_shell *shell)
{
	t_cmd	*tmp;
	char	c;

	tmp = shell->cmd;
	while (tmp)
	{
		if (tmp->cmd[0] == '\'' || tmp->cmd[0] == '\"')
		{
			c = tmp->cmd[0];
			tmp = tmp->next;
			while (tmp && tmp->cmd[0] != c)
				tmp = tmp->next;
			if (tmp)
				tmp = tmp->next;
		}
		else if (tmp->cmd[0] == ' ' && tmp->next && tmp->next->cmd[0] == ' ')
			tmp = ft_pop(tmp, shell);
		else
			tmp = tmp->next;
	}
}

//Potential error, removed if (tmp) before tmp = tmp->next;
void	parse_space_around(t_shell *shell, int i)
{
	t_cmd	*tmp;

	tmp = shell->cmd;
	while (tmp)
	{
		i = 0;
		while (i != 2)
		{
			if (tmp->cmd[0] == '>' || tmp->cmd[0] == '<' || tmp->cmd[0] == '|')
			{
				if (tmp->prev && tmp->prev->cmd[0] == ' ')
				{
					tmp = tmp->prev;
					tmp = ft_pop(tmp, shell);
				}
				if (tmp->next && tmp->next->cmd[0] == ' ')
				{
					tmp = tmp->next;
					tmp = ft_pop(tmp, shell);
				}
			}
			i++;
		}
		tmp = tmp->next;
	}
}
