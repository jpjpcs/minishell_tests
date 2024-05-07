/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaosilva <joaosilva@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 15:14:45 by crocha-s          #+#    #+#             */
/*   Updated: 2024/05/03 16:10:44 by joaosilva        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <signal.h>
# include <limits.h>
# include <dirent.h>
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <ncurses.h>
# include <term.h>

# define PROMPT_BEGIN "┎─── "
# define PROMPT_END "\n┖─❯ "

# define STOP 0
# define MAXARGS 50
# define EXEC 300

extern int g_exit;

typedef struct s_env
{
    char            *key;
    char            *value;
    int                     visible;
    int                     index;
    struct s_env    *next;
}                           t_env;

typedef struct s_cmd
{
   int  type;
}   t_cmd;

typedef struct s_exec
{
	int		type;
	char	*argv[MAXARGS];
}			t_exec;
typedef struct s_shell
{
    char    *line;
    char    *prompt;
    char    *ps;
    char    *es;
    int         line_len;
    t_env   *env;
    t_cmd   *cmd;
    int     status;
    int     exec_cmd;
    int     envp_size;
    int     pid;
    char    **envp;
} t_shell;

void run_cmd(t_shell *shell, t_cmd *cmd);
int run_builtin(t_shell* shell, t_exec *cmd );
void ms_echo(t_exec *cmd);


void    envp_to_list(char **envp, t_shell *shell);
t_env   *env_add(t_shell *shell, char *key, char *value, int visible);
void    envp_sort(t_shell *shell);
void    envp_update(t_shell *shell);
void    envp_destroy(t_env *env);


#endif
