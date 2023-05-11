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

void	echo(char **args)
{
	int	i;

	if (args[1] && !ft_strcmp("-n", args[1]))
	{
		i = 2;
		while (args[i])
		{
			ft_printf("%s", args[i]);
			i++;
		}
	}
	else
	{
		i = 1;
		while (args[i])
		{
			ft_printf("%s", args[i]);
			i++;
		}
		ft_printf("\n");
	}
}