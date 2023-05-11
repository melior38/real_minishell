/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asouchet <asouchet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 11:17:16 by lcamilo-          #+#    #+#             */
/*   Updated: 2023/05/08 16:24:31 by asouchet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	unset(t_shell *shell, char **args)
{
	t_variable *env;
	t_variable *tmp;

	env = shell->env;
	tmp = NULL;
	while (env)
	{
		if (args[1] && env->name == args[1])
			break ;
		env = env->next;
	}
	if (env && env->name == args[1])
	{
		tmp = env->prev;
		free(env->value);
		free(env->name);
		free(env);
		env = tmp;
	}
}