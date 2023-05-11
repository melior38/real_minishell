/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcamilo- <marvin@42.fr>      8======D      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 11:17:16 by lcamilo-          #+#    #+#             */
/*   Updated: 2023/01/16 11:17:18 by lcamilo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_redir	*redirection(t_redir *redir, t_cmd *cmd)
{
	t_cmd	*tmp;

	tmp = cmd;
	while (tmp)
	{
		if (tmp->cmd[0] == '>')
		{
			if (tmp->next && tmp->next->cmd[0] == '>')
			{
				redir_add_back(&redir, redir_create(tmp->next->next->cmd, 1, NULL));
				tmp = tmp->next->next;
			}
			else
				redir_add_back(&redir, redir_create(tmp->next->cmd, 0, NULL));
		}
		else if (tmp->cmd[0] == '<' && tmp->next && tmp->next->cmd[0] != '<')
			redir_add_back(&redir, redir_create(tmp->next->cmd, 2, NULL));
		else if (tmp->cmd[0] == '<' && tmp->next && tmp->next->cmd[0] == '<')
		{
			redir_add_back(&redir, here_doc(tmp, tmp->next->next->cmd));
			while (tmp->next)
			{
				if (tmp->prev && tmp->prev->prev && tmp->prev->cmd[0] != '<' && tmp->prev->prev->cmd[0] != '<')
					break ;
				tmp = tmp->next;
			}
		}
		if (tmp->cmd[0] == '|')
			break ;
		tmp = tmp->next;
	}
	return (redir);
}

t_redir	*import_heredoc(t_cmd *cmd, char *hd)
{
	t_redir	*heredoc;
	t_redir	*redir;

	redir = NULL;
	heredoc = redir_create(NULL, 3, hd);
	while (cmd)
	{
		if (cmd->cmd[0] == '|')
			break ;
		if (cmd->cmd[0] == '>' || cmd->cmd[0] == '<')
		{
			redir = redirection(redir, cmd);
			redir_add_back(&heredoc, redir);
			break ;
		}
		cmd = cmd->next;
	}
	return (heredoc);
}

char	*execute_heredoc(char *rd, char *limiter)
{
	char	*nl;
	char	*str;
	size_t	len;

	str = ft_strdup("");
	while (1)
	{
		nl = malloc(sizeof(char *) * (ft_strlen(rd) + 1));
		nl = ft_memcpy(nl, rd, ft_strlen(rd));
		len = ft_strlen(rd);
		nl[len] = '\n';
		nl[len + 1] = '\0';
		str = ft_strjoin(str, nl);
		free(rd);
		rd = NULL;
		rd = readline("> ");
		if (!rd || g_error_num == 130)
			return (NULL);
		if (ft_strncmp(rd, limiter, ft_strlen(limiter)) == 0)
			break ;
	}
	return (str);
}

//Faire comme varialbe d'env (rajouter dans linked list a la suite)
t_redir	*here_doc(t_cmd *tmp, char *limiter)
{
	char	*rd;
	char	*str;

	run_signals(2);
	rd = readline("> ");
	if (!rd || g_error_num == 130)
		return (NULL);
	if (ft_strncmp(rd, limiter, ft_strlen(limiter)) == 0)
		return (import_heredoc(tmp->next->next, NULL));
	str = execute_heredoc(rd, limiter);
	tmp = tmp->next->next;
	return (import_heredoc(tmp, str));
}
