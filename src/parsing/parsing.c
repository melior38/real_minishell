/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asouchet <asouchet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 11:17:16 by lcamilo-          #+#    #+#             */
/*   Updated: 2023/05/08 16:16:12 by asouchet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	error_msg(t_shell *shell, char *msg)
{
	free_cmd(shell);
	printf("%s\n", msg);
}

/// Execute toutes les commandes a faire pour le parsing
/// \param shell Structure shell
void	parse_cmd(t_shell *shell)
{
	if (!check_quotes(shell))
	{
		error_msg(shell, "Quotes error");
		return ;
	}
	parse_space(shell);
	parse_space_around(shell, 0);
	parse_dollars(shell);
	remove_quotes(shell);
	get_cmd(shell);
}

void	parse_rd_token(t_shell *shell, int *j)
{
	lst_add_back(&shell->cmd, lst_create(ft_substr(shell->read, 0, 1)));
	*j = *j + 1;
}

/// Recuperer le Readline du shell et le mets dans une linked list.
/// Si un token est rencontré, on recupere ce qu'il y avait avant
/// et on fait une linked liste avec
/// \param shell Structure shell
/// \return Retourne une linked list
t_cmd	*parse_rd(t_shell *shell)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (shell->read[i])
	{
		if (i == 0 && is_token(shell->read[i]))
			parse_rd_token(shell, &j);
		if (is_token(shell->read[i + 1]) || !shell->read[i + 1])
		{
			if (i - j + 1 > 0)
				lst_add_back(&shell->cmd, lst_create(ft_substr(
							shell->read, j, i - j + 1)));
			j = i;
			j++;
			if (shell->read[j])
				lst_add_back(&shell->cmd, lst_create(ft_substr(
							shell->read, j, 1)));
			j++;
		}
		i++;
	}
	return (shell->cmd);
}
