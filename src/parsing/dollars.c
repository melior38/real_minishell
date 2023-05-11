/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollars.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcamilo- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 11:17:16 by lcamilo-          #+#    #+#             */
/*   Updated: 2023/01/16 11:17:18 by lcamilo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*extract_value(t_shell *shell, char *cmd, int i)
{
	char	*str;
	char	*env;
	char	*tmp;

	str = ft_substr(cmd, i + 1, ft_strlen(cmd) - i);
	env = get_env_value(shell, str);
	free(str);
	tmp = ft_substr(cmd, 0, i);
	str = ft_strjoin(tmp, env);
	free(cmd);
	free(tmp);
	return (str);
}

void	parse_dollars(t_shell *shell)
{
	t_cmd	*tmp;
	int		i;
	char	c;

	tmp = shell->cmd;
	while (tmp)
	{
		i = 0;
		while (tmp->cmd[i])
		{
			if (tmp->cmd[i] == '\'')
				c = '\'';
			else if (tmp->cmd[i] == '\'' && c == '\'')
				c = 't';
			else if (tmp->cmd[i] == '$' && c != '\'')
			{
				tmp->cmd = extract_value(shell, tmp->cmd, i);
				break ;
			}
			i++;
		}
		tmp = tmp->next;
	}
}

char	*get_env_value(t_shell *shell, char *str)
{
	t_variable	*env;

	env = shell->env;
	if (!str)
		return ("");
	if (!ft_strcmp(str, "?"))
		return (ft_itoa(g_error_num));
	while (env)
	{
		if (!ft_strcmp(env->name, str))
			return (env->value);
		env = env->next;
	}
	return ("");
}
