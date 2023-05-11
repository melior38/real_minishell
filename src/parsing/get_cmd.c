/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asouchet <asouchet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 11:17:16 by lcamilo-          #+#    #+#             */
/*   Updated: 2023/05/09 16:27:39 by asouchet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*return_args_tmp(char ***args, t_cmd *tmp)
{
	int	i;

	i = 0;
	while (tmp && tmp->cmd[0] != '|' && tmp->cmd[0] != '>')
	{
		if (tmp->cmd[0] != ' ' && tmp->cmd[0] != '<')
		{
			(*args)[i] = ft_strdup(tmp->cmd);
			i++;
		}
		else if (tmp->cmd[0] == '<')
			break ;
		tmp = tmp->next;
	}
	(*args)[i] = NULL;
	return (tmp);
}

t_cmd	*check_exec(t_shell *shell, t_cmd *cmd, char **args)
{
	t_cmd	*tmp;
	t_redir	*redir;

	tmp = cmd;
	redir = NULL;
	if (tmp && (tmp->cmd[0] == '>' || tmp->cmd[0] == '<'))
		redir = redirection(redir, tmp);
	while (tmp)
	{
		if (tmp->cmd[0] == '|')
			break ;
		tmp = tmp->next;
	}
	exec_add_back(&shell->exec, exec_create(args, redir));
	return (tmp);
}

/// Permet de creer une linked list contenant les commandes a executer
/// \param shell Structure shell
void	get_cmd(t_shell *shell)
{
	char	**args;
	t_cmd	*tmp;
	t_redir	*redir;

	tmp = shell->cmd;
	redir = NULL;
	args = NULL;
	if (lst_len(tmp) == 1)
    {
        args = malloc(sizeof(char *) * 2);
        tmp = return_args_tmp(&args, tmp);
        exec_add_back(&shell->exec, exec_create(args, redir));
        return ;
    }
	while (tmp)
	{
		if (count_args(tmp))
		{
			args = malloc(sizeof(char *) * (count_args(tmp) + 1));
			tmp = return_args_tmp(&args, tmp);
			if (tmp && tmp->next && tmp->cmd[0] == '<'
				&& tmp->next->cmd[0] == '<')
			{
				if (tmp->next->next)
				{
					redir = here_doc(tmp, tmp->next->next->cmd);
					tmp = tmp->next->next;
					exec_add_back(&shell->exec, exec_create(args, redir));
					while (tmp)
					{
						if (tmp->cmd[0] == '|')
							break ;
						tmp = tmp->next;
					}
				}
			}
			else
				tmp = check_exec(shell, tmp, args);
		}
		else if (!count_args(tmp) && (tmp->cmd[0] == '>' || tmp->cmd[0] == '<'))
		{
			redir = redirection(redir, tmp);
			while (tmp)
			{
				if (count_args(tmp))
				{
					args = malloc(sizeof(char *) * (count_args(tmp) + 1));
					tmp = return_args_tmp(&args, tmp);
				}
				if (tmp && tmp->cmd[0] == '|')
					break ;
				if (tmp)
					tmp = tmp->next;
			}
			exec_add_back(&shell->exec, exec_create(args, redir));
		}
		else if (!count_args(tmp) && tmp->cmd[0] == '<' && tmp->next->cmd[0] == '<')
		{
			redir_add_back(&redir, here_doc(tmp, tmp->next->next->cmd));
			exec_add_back(&shell->exec, exec_create(args, redir));
		}
		else if (!count_args(tmp) && tmp->cmd[0] == '<' && tmp->next->cmd[0] != '<')
			redir_add_back(&redir, redirection(redir, tmp));
		if (tmp)
			tmp = tmp->next;
	}
}
