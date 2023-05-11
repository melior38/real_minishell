/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asouchet <asouchet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 11:21:40 by lcamilo-          #+#    #+#             */
/*   Updated: 2023/05/08 19:01:45 by asouchet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <readline/readline.h>
# include <fcntl.h>
# include <errno.h>
# include <string.h>
# include <unistd.h>
# include <stdbool.h>
# include <sys/termios.h>
# include <stdlib.h>
# include <signal.h>
# include <limits.h>
# include <readline/history.h>
# include <sys/ioctl.h>
# include "libft.h"

int	g_error_num;

typedef struct s_cmd
{
	char			*cmd;
	struct s_cmd	*next;
	struct s_cmd	*prev;
}	t_cmd;

typedef struct t_redir
{
	char			*file;
	int				cases;
	char			*here_doc;
	struct t_redir	*next;
	struct t_redir	*prev;
}	t_redir;

typedef struct s_error {
	int				global;
	int				error;
}	t_error;

typedef struct s_fd {
	int				tmp[2];
	int				fd[2];
}	t_fd;

typedef struct s_file {
	int output;
	int input;
	int fd[2];
}                t_file;

typedef struct s_variable
{
	char				*name;
	char				*value;
	struct s_variable	*next;
	struct s_variable	*prev;
}	t_variable;

typedef struct s_exec
{
	char			**cmd_args;
	// char			**splited_path;
	t_redir			*redir;
	struct s_exec	*next;
	struct s_exec	*prev;
}	t_exec;

typedef struct s_shell
{
	char			*read;
	t_variable		*env;
	t_cmd			*cmd;
	t_exec			*exec;
	t_file			*fd;
	t_error			*error;
	char			**environnement;
}	t_shell;


//////////////////////////////MAIN.C////////////////////////////////////////////
void		parsing(t_shell *shell);
void		init_shell(t_shell *shell, char **env);

////////////////////////////////////////////////////////////////////////////////
///////////////////////////////Free/////////////////////////////////////////////
//////////////////////////////Folder////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

//////////////////////////////FREE.C////////////////////////////////////////////
void		free_env(t_shell *shell);
void		free_cmd(t_shell *shell);
void		free_exec(t_shell *shell);

////////////////////////////////////////////////////////////////////////////////
//////////////////////////////Builtin///////////////////////////////////////////
//////////////////////////////Folder////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

/////////////////////////////ECHO.C/////////////////////////////////////////////
void		echo(char **args);

///////////////////////////////CD.C/////////////////////////////////////////////
void		update_env_value(t_shell *shell, char *name, char *value);
void		cd(t_shell *shell, char **cmd);
char		*get_path(t_shell *shell, char *var_name);
//////////////////////////////PWD.C/////////////////////////////////////////////
void		pwd(t_shell *shell);

////////////////////////////////////////////////////////////////////////////////
/////////////////////////////Execution//////////////////////////////////////////
//////////////////////////////Folder////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

//////////////////////////////EXECUTION.C///////////////////////////////////////
int			cmd_nb(t_exec *lst);
int			ft_lst_len_exec(t_exec *lst);
int			improved_dup2(int fildes, int fildes2, t_shell *shell);
int			improved_pipe(int fd[2], t_shell *shell);
pid_t		improved_fork(t_shell *shell);
char		*ft_get_path(char *cmd, t_shell *shell);
int			is_builtin(char *cmd);
void		builtin(t_shell *shell, char *cmd);
void		land_here_doc(t_shell *shell, char *heredoc);
void		redir_new_file(char *file, t_shell *shell, int cases);
void		redir_file(char *file, t_shell *shell);
void		parse_redir(t_shell *shell, t_redir *redir);
void		parse_opening(t_shell *shell, t_redir *redir);
void		t_redir_exec(t_shell *shell, t_exec *exec);
int			is_redir(t_exec *exec);
void		dup_case(t_shell *shell, t_fd *fd);
void		last_dup_case(t_shell *shell, t_fd *fd);
void		minishell_fork(t_shell *shell, t_exec *exec, t_fd *fd);
void		multiple_cmd(t_shell *shell, t_exec *exec, t_fd *fd);
void		last_cmd_in_file(t_shell *shell, t_exec *exec, t_fd *fd);
void		execution(t_shell *shell);
////////////////////////////////////////////////////////////////////////////////
//////////////////////////////Utils/////////////////////////////////////////////
//////////////////////////////Folder////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////SIG.C///////////////////////////////////////////
void		rl_replace_line(const char *text, int clear_undo);
void		run_signals(int sig);
void		restore_prompt(int sig);

//////////////////////////////GET_ENV.C/////////////////////////////////////////
t_variable	*init_env(char **env);

//////////////////////////////UTILS_LST.C///////////////////////////////////////
void		lst_show(t_cmd *lst);
int			lst_len(t_cmd *lst);
void		lst_add_front(t_cmd **lst, t_cmd *new);
void		lst_add_back(t_cmd **lst, t_cmd *new);
t_cmd		*lst_create(char *str);

//////////////////////////////UTILS_LST2.C//////////////////////////////////////
void		printf_list(t_cmd *lst);
void		free_lst(t_shell *shell);
size_t		len_cmd(t_cmd *tmp, char c);
t_cmd		*ft_pop(t_cmd *lst, t_shell *shell);

////////////////////////////////UTILS.C/////////////////////////////////////////
int			is_token(char c);
int			count_args(t_cmd *tmp);
void		print_struct(t_shell *shell);
int			check_quotes(t_shell *shell);
int			check_specific_quotes(t_shell *shell, char c, char quotes);

/////////////////////////////EXEC_UTILS.C///////////////////////////////////////
void		exec_add_back(t_exec **exec, t_exec *new);
t_exec		*exec_create(char **args, t_redir *redir);

/////////////////////////////REDIR_UTILS.C//////////////////////////////////////
t_redir		*redir_create(char *file, int cases, char *hd);
void		redir_add_back(t_redir **redir, t_redir *new);

////////////////////////////////////////////////////////////////////////////////
/////////////////////////////Parsing////////////////////////////////////////////
//////////////////////////////Folder////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////DOLLARS.C////////////////////////////////////////
void		parse_dollars(t_shell *shell);
char		*extract_value(t_shell *shell, char *cmd, int i);
char		*get_env_value(t_shell *shell, char *str);

///////////////////////////////GET_CMD.C////////////////////////////////////////
void		get_cmd(t_shell *shell);
t_cmd		*check_exec(t_shell *shell, t_cmd *cmd, char **args);
t_cmd		*return_args_tmp(char ***args, t_cmd *tmp);

///////////////////////////////SPACE.C//////////////////////////////////////////
void		parse_space(t_shell *shell);
void		parse_space_around(t_shell *shell, int i);

///////////////////////////////PARSING.C////////////////////////////////////////
void		parse_cmd(t_shell *shell);
void		error_msg(t_shell *shell, char *msg);
void		parse_rd_token(t_shell *shell, int *j);
t_cmd		*parse_rd(t_shell *shell);

///////////////////////////////QUOTES.C/////////////////////////////////////////
void		remove_quotes(t_shell *shell);
void		replace_str(char **replace, char *str);
char		*remove_quotes_utils(t_cmd *tmp, char c);

/////////////////////////////HERE_DOC.C/////////////////////////////////////////
t_redir		*here_doc(t_cmd *tmp, char *limiter);
t_redir		*redirection(t_redir *redir, t_cmd *cmd);
t_redir		*import_heredoc(t_cmd *cmd, char *hd);

#endif
