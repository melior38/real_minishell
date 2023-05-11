/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asouchet <asouchet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 11:17:16 by lcamilo-          #+#    #+#             */
/*   Updated: 2023/05/08 16:32:57 by asouchet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	split_variable(t_variable *node, char *s)
{
	int	i;

	i = 0;
	while (s[i] && s[i] != '=')
		i++;
	node->name = ft_strlendup(s, i);
	s += i;
	if (!*s)
	{
		node->value = NULL;
		return ;
	}
	s++;
	i = 0;
	while (s[i])
		i++;
	node->value = ft_strlendup(s, i);
}

t_variable	*create_var(char *s)
{
	t_variable	*node;

	node = malloc(sizeof(t_variable));
	if (!node)
		return (NULL);
	split_variable(node, s);
	node->next = NULL;
	node->prev = NULL;
	return (node);
}

void	create_double_env(t_variable *head)
{
	t_variable *tmp;
	t_variable *before_tmp;

	tmp = head;
	while (tmp)
	{
		before_tmp = tmp;
		tmp = tmp->next;
		tmp->prev = before_tmp;
	}
}

t_variable	*init_env(char **env)
{
	t_variable	*head;
	t_variable	*tmp;

	if (!env || !*env)
		return (NULL);
	tmp = create_var(*env);
	head = tmp;
	env++;
	while (*env)
	{
		if (!ft_strncmp(*env, "OLDPWD", 6))
			env++;
		tmp->next = create_var(*env);
		tmp = tmp->next;
		env++;
	}
	tmp->next = NULL;
	// create_double_env(head);
	return (head);
}
