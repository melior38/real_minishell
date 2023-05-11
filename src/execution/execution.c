/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asouchet <asouchet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/28 15:23:10 by lcamilo-          #+#    #+#             */
/*   Updated: 2023/05/09 17:04:55 by asouchet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	cmd_nb(t_exec *lst)
{
	int	count;

	count = 0;
	while (lst)
	{
		count++;
		lst = lst->next;
	}
	return (count);
}

int	ft_lst_len_exec(t_exec *lst)
{
	int	count;

	count = 0;
	while (lst)
	{
		count++;
		lst = lst->next;
	}
	return (count);
}

int	improved_dup2(int fildes, int fildes2, t_shell *shell)
{
	int	error;

	error = dup2(fildes, fildes2);
	if (error == -1)
	{
		perror("dup2 error:");
		shell->error->error = error;
	}
	return (error);
}

int	improved_pipe(int fd[2], t_shell *shell)
{
	int	error;

	error = pipe(fd);
	if (error == -1)
	{
		perror("pipe error:");
		shell->error->error = error;
	}
	return (error);
}

pid_t	improved_fork(t_shell *shell)
{
	pid_t	error;

	error = fork();
	if (error == -1)
	{
		perror("fork error:");
		shell->error->error = error;
	}
	return (error);
}

char	*ft_get_path(char *cmd, t_shell *shell)
{
	int			j;
	t_variable	*tmp;
	char		**splited_path;

	j = 0;
	tmp = shell->env;
	if (access(cmd, X_OK) == 0)
	{
		splited_path = ft_split(cmd, ' ');
		return (cmd);
	}
	while (tmp && ft_strncmp(tmp->name, "PATH", 5))
		tmp = tmp->next;
	if (tmp)
		splited_path = ft_split(tmp->value, ':');
	else
		splited_path = NULL;
	while (splited_path[j] && splited_path != NULL)
	{
		splited_path[j] = ft_strjoin(splited_path[j], "/");
		splited_path[j] = ft_strjoin(splited_path[j], &cmd[0]);
		if (access(splited_path[j], X_OK) == 0)
			break ;
		j++;
	}
	return (splited_path[j]);
}

int	is_builtin(char *cmd)
{
	if (ft_strcmp(cmd, "cd") == 0)
		return (1);
	else if (ft_strcmp(cmd, "echo") == 0)
		return (1);
	else if (ft_strcmp(cmd, "env") == 0)
		return (1);
	else if (ft_strcmp(cmd, "exit") == 0)
		return (1);
	else if (ft_strcmp(cmd, "export") == 0)
		return (1);
	else if (ft_strcmp(cmd, "pwd") == 0)
		return (1);
	else if (ft_strcmp(cmd, "unset") == 0)
		return (1);
	return (0);
}

void	builtin(t_shell *shell, char *cmd)
{
	if (ft_strcmp(cmd, "pwd") == 0)
		pwd(shell);
}


void	land_here_doc(t_shell *shell, char *heredoc)
{
	improved_pipe(shell->fd->fd, shell);

	ft_putstr_fd(heredoc, shell->fd->fd[1]);
	shell->fd->input = shell->fd->fd[0]; // sa marche mais faut penser a close le fd[1] pour que sa fonctionne
	close(shell->fd->fd[1]);
}


void	redir_new_file(char *file, t_shell *shell, int cases)
{
	int	fd;

	// printf("on est dans new_file\n");
	if (shell->fd->output != 1)
	{
		close(shell->fd->output);
		shell->fd->output = 1;
	}
	if (cases == 0)
		fd = open(file, O_TRUNC | O_RDWR | O_CREAT, 0644);
	else
		fd = open(file, O_APPEND | O_RDWR | O_CREAT, 0644);
	if (fd == -1)
	{
		ft_putstr_fd("error\n", 2);
		shell->error->error = 1;
	}
	// printf("[%d]\n", fd);
	shell->fd->output = fd;
}

void	redir_file(char *file, t_shell *shell)
{
	int	fd;

	if (shell->fd->input != 0)
	{
		close(shell->fd->input);
		shell->fd->input = 0;
	}
	fd = open(file, O_RDWR, 0644);
	// printf("[%d]\n", fd);
	if (fd == -1)
	{
		ft_putstr_fd("error\n", 2); // changer par une variable stockant l'erreur en plus de display le soucis 
		shell->error->error = 1;
	}
	shell->fd->input = fd;
}

// > = 0 fais une enum batard <3 8=D
// >> = 1
// < = 2
// << = 3

void	parse_redir(t_shell *shell, t_redir *redir)
{
	t_redir	*tmp;

	tmp = redir;
	// printf("on est dans redir\n");
	while (tmp)
	{
		printf("cases[%d]\n", tmp->cases);
		if (tmp->cases == 0 || tmp->cases == 1)
			redir_new_file(redir->file, shell, tmp->cases); // juste penser a passer les bonnes struct
		if (shell->error->error != 0)
			break ;
		tmp = tmp->next;
	}
}

void	parse_opening(t_shell *shell , t_redir *redir)
{
	t_redir	*tmp;

	tmp = redir;
	// printf("on est dans parse opening\n");
	while (tmp)
	{
		printf("cases[%d]\n", tmp->cases);
		if (tmp->cases == 2)
			redir_file(redir->file, shell); // juste penser a passer les bonnes struct
		if (shell->error->error != 0)
			break ;
		if (tmp->cases == 3)
			land_here_doc(shell, tmp->here_doc);
		if (shell->error->error != 0)
			break ;
		tmp = tmp->next;
	}
}

void	t_redir_exec(t_shell *shell, t_exec *exec)
{
	t_redir	*tmp;

	tmp = exec->redir;
	parse_redir(shell, tmp);
	if (shell->error->error != 0)
		return ;
	parse_opening(shell, tmp);
	if (shell->error->error != 0)
		return ;
	printf("input[%d]\noutput[%d]\n", shell->fd->input, shell->fd->output);
}

//void one_cmd(t_shell *shell, t_exec *exec, t_pipex *data, t_fd *fd)
//{
//	int pid;
//
//	pid = improved_fork(shell);
//	if (shell->error->error != 0)
//		return;
//	if (pid == 0)
//	{
//		last_dup_case(shell, fd);
//		close(fd->fd[0]);
//		close(fd->fd[1]);
//		ft_get_path(exec->data, exec->cmd_args[0], shell);
//		if (execve(exec->data->splited_path[exec->data->i], exec->cmd_args,
//				   shell->environnement) == -1)
//			perror("error");
//		exit(1);
//	}
//}

int	is_redir(t_exec *exec)
{
	t_exec	*tmp;

	tmp = exec;
	if (exec == NULL)
		return (0);
	if (tmp->redir == NULL) // wtf pk sa crash ici
		return (0);
	return (1);
}

void	dup_case(t_shell *shell, t_fd *fd)
{
	if (shell->fd->input != 0)
		improved_dup2(shell->fd->input, STDIN_FILENO, shell);
	else
		improved_dup2(fd->tmp[0], STDIN_FILENO, shell);
	if (shell->error->error)
		exit(1);
	if (shell->fd->output != 1)
		improved_dup2(shell->fd->output, STDOUT_FILENO, shell);
	else
		improved_dup2(fd->fd[1], STDOUT_FILENO, shell);
	if (shell->error->error)
		exit(1);
}

void	last_dup_case(t_shell *shell, t_fd *fd)
{
	if (shell->fd->input != 0)
		improved_dup2(shell->fd->input, STDIN_FILENO, shell);
	else if (fd->tmp[0])
		improved_dup2(fd->tmp[0], STDIN_FILENO, shell);
	if (shell->error->error)
		exit(1);
	if (shell->fd->output != 1)
		improved_dup2(shell->fd->output, STDOUT_FILENO, shell);
	if (shell->error->error)
		exit(1);
}

void	minishell_fork(t_shell *shell, t_exec *exec, t_fd *fd)
{
	int	pid;
	char *path;

	pid = improved_fork(shell);
	if (shell->error->error != 0)
		return ;
	if (pid == 0)
	{
		dup_case(shell, fd);
		close(fd->tmp[1]);
		close(fd->fd[0]);
		close(fd->fd[1]);
		// close(shell->fd->input); // dangeureux sa
		// close(shell->fd->output); // dangeureux sa
		path = ft_get_path(exec->cmd_args[0], shell);
		if (shell->error->error != 0)
			exit(1);
		if (execve(ft_get_path(exec->cmd_args[0], shell), exec->cmd_args, shell->environnement) == -1)
			perror("error");
		exit(1);
	}
	close(fd->tmp[0]);
	close(fd->fd[1]);
	close(shell->fd->input);
	shell->fd->input = 0;
	close(shell->fd->output);
	shell->fd->output = 1;
}

void	multiple_cmd(t_shell *shell, t_exec *exec, t_fd *fd)
{
	if (shell->error->error)
		return ;
	fd->tmp[0] = fd->fd[0];
	fd->tmp[1] = fd->fd[1];
	improved_pipe(fd->fd, shell);
	minishell_fork(shell, exec, fd); // a changer
}

void	last_cmd_in_file(t_shell *shell, t_exec *exec, t_fd *fd)
{
	int	pid;
	char *path;

	improved_pipe(fd->fd, shell);
	if (shell->error->error)
		return ;
	fd->tmp[0] = fd->fd[0];
	fd->tmp[1] = fd->fd[1];
	pid = improved_fork(shell);
	if (pid == 0)
	{
		// printf("fd[%d]\n", shell->fd->input);
		// printf("fd[%d]\n", shell->fd->output);
		last_dup_case(shell, fd);
		close(fd->fd[0]);
		close(fd->fd[1]);
		if (shell->fd->input != 0)
			close(shell->fd->input);
		if (shell->fd->output != 1)
			close(shell->fd->output);
		path = ft_get_path(exec->cmd_args[0], shell);
		// printf("cmd[%s]\n", exec->cmd_args[0]);
		// printf("file[%s]\n", exec->redir->file);
		// printf("%s\n%s\n", path, exec->cmd_args[0]);
		// printf("%s%s\n", exec->cmd_args[0], exec->cmd_args[1]);
		if (execve(path, exec->cmd_args, shell->environnement) == -1)
			perror("error ici\n");
		exit(1);
	}
	wait(&pid);
	close(fd->fd[1]);
	close(fd->fd[0]);
}

// Execute une commande
void	execution(t_shell *shell)
{
	t_exec	*tmp;
	t_fd	*fd;
	t_file	file;
	t_error error;
	int		cmd_count;
	int		i;
	// int		pipe[2];
	// int		pipe_tmp[2];

	i = 0;
	fd = malloc(sizeof(t_fd) * 1);
	tmp = shell->exec;
	file.input = 0;
	file.output = 1;
	file.fd[1] = 0;
	file.fd[0] = 0;
	error.error = 0;
	error.global = 0;
	fd->fd[0] = 0;
	fd->fd[1] = 0;
	fd->tmp[0] = fd->fd[0];
	fd->tmp[1] = fd->fd[1];
	shell->error = &error;
	shell->fd = &file;

	// printf("[%d]\n", shell->exec->redir->cases);
	cmd_count = cmd_nb(tmp);
	if (cmd_count == 1)
	{
		if (is_redir(tmp))
			t_redir_exec(shell, tmp); // s'occupe de faire toute les redirections de pipes et set input et output a une valeur
		if (shell->error->error != 0)
			return ;
		if (tmp->cmd_args)
		{
			last_cmd_in_file(shell, tmp, fd);
			// printf("il y a cmd_args\n");
		}
	}
	else
	{
		improved_pipe(fd->fd, shell);
		while (i < ft_lst_len_exec(tmp) - 1)
		{
			if (shell->error->error != 0)
				return ;
			if (is_redir(tmp)) // true quand il trouve une list t_redir
				t_redir_exec(shell, tmp); // doit identifier si il fais un append un trunc ou accede a un fichier existant puis dup2 pour l'operation recherchee
			if (shell->error->error != 0)
				return ;
			if(tmp->cmd_args)
				multiple_cmd(shell, tmp, fd);// checker pour ou foutre le t_pipex ou ne pas l'utiliser
			shell->error->error = 0;
			printf("count++\n");
			tmp = tmp->next; // penser a rajouter ces 2 lignes dans le code envoyer a leandro
		}
		if (is_redir(tmp))
			t_redir_exec(shell, tmp);
		if (shell->error->error != 0)
			return ;
		last_cmd_in_file(shell, tmp, fd); // s'occupe de la derniere execution si il y a redirection de fichier
	}
}
