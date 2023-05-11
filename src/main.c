/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asouchet <asouchet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 11:17:16 by lcamilo-          #+#    #+#             */
/*   Updated: 2023/05/09 16:37:29 by asouchet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


/// Check si un token < > | est en fin de commande
/// \param shell Structure shell
/// \return return 1 si il y a une erreur
int	check_error(t_shell *shell)
{
	t_cmd	*tmp;

	tmp = shell->cmd;
	while (tmp->next)
		tmp = tmp->next;
	if (tmp->cmd[0] == '>' || tmp->cmd[0] == '<')
	{
		ft_printf("syntax error near unexpected token `newline\'\n");
		return (1);
	}
	else if (tmp->cmd[0] == '|')
	{
		ft_printf("syntax error near unexpected token `|\'\n");
		return (1);
	}
	else
		return (0);
}

/// Ajoute dans l'historique la commande passé, rentre dans la fonction parsing
/// Fais l'execution et free les linked list
/// \param shell Structure shell
void	parsing(t_shell *shell)
{
	if (shell->read)
		add_history(shell->read);
	shell->cmd = parse_rd(shell);
	if (check_error(shell))
	{
		free_cmd(shell);
		free(shell->read);
		return ;
	}
	parse_cmd(shell);
	run_signals(1);
//	printf("[%s]\n", shell->exec->cmd_args[1]);
	execution(shell);
	free_cmd(shell);
	free_exec(shell);
	free(shell->read);
}

/// Initialise les variables de la structure shell
/// \param shell Structure shell
void	init_shell(t_shell *shell, char **env)
{

	shell->cmd = NULL;
	shell->exec = NULL;
	shell->read = NULL;
	shell->env = init_env(env);
	shell->environnement = env;
}

int	main(int ac, char **av, char **env)
{
	t_shell	shell;

	(void)ac;
	(void)av;
	init_shell(&shell, env);
	add_history("ec$t");
	add_history("echo '$t'");
	add_history("echo \"$t\"");
	add_history("echo \"salut\" > test");
	add_history("echo \"salut\">test");
	add_history("ec$t \"j'ai test\"");
	add_history("echo \"'\"");
	add_history("<makefile cat");
	add_history("< makefile cat");
	add_history("ls -l -a | wc");
	add_history("ls -l -a | wc > a > b > c");
	add_history("grep \"motif\" < entree.txt > sortie.txt");
	add_history("ls >> test.txt");
	add_history("cat | cat | cat | ls");
	add_history("pwd");
	add_history("echo \"tttttttttttttttt\"");
	add_history("ls -l -a | wc > a > b >> c");
	add_history("ls > a | cat << EOF | echo \"Test\"");
	add_history("cat << EOF");
	add_history("<< EOF");
	add_history("ls | > a > b");
	add_history("> a > b > c");
	add_history("cat < a >> b > c");
	add_history("cat << EOF < a");
	add_history("cat << EOF > a");
	add_history("cat << EOF1 << EOF2 << EOF3");
	add_history("< a cat < b");
	add_history("<< EOF");
	add_history("cd /Documents/tryagain/");
	run_signals(1);
	while (1)
	{
		shell.read = readline("Jean minishell crapaud :");
		if (!shell.read)
			run_signals(3);
		if (ft_strncmp(shell.read, "exit", 4) == 0)
			break ;
		else if (shell.read)
			parsing(&shell);
	}
	free(shell.read);
	free_env(&shell);
	return (0);
}
