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

char	*get_path(t_shell *shell, char *var_name)
{
	if (!get_env_value(shell, var_name))
	{
		ft_printf("cd : %s not set\n", var_name);
		return (NULL);
	}
	return (get_env_value(shell, var_name));
}

void	update_env_value(t_shell *shell, char *name, char *value)
{
	t_variable	*env;

	env = shell->env;
	while (env)
	{
		if (!ft_strcmp(env->name, name))
		{
			free(env->value);
			env->value = NULL;
			env->value = ft_strdup(value);
			break ;
		}
		env = env->next;
	}
}

void	cd(t_shell *shell, char **cmd)
{
	char	*new_path;
	char	*path_now;
	char	buffer[2048];

	g_error_num = 0;
	new_path = NULL;
	if (!cmd[1])
		new_path = get_path(shell, "HOME");
	else if (cmd[1][0] == '-' && !cmd[1][1])
		new_path = get_path(shell, "OLDPWD");
	else if (cmd[1])
		new_path = cmd[1];
	path_now = getcwd(buffer, sizeof(buffer));
	update_env_value(shell, "OLDPWD", path_now);
	if (!new_path)
		return ;
	if (!chdir(new_path))
		update_env_value(shell, "PWD", new_path);
}
