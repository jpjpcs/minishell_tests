/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_exec.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crocha-s <crocha-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 00:23:51 by crocha-s          #+#    #+#             */
/*   Updated: 2024/05/27 15:52:46 by crocha-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	check_execve_errors(t_shell *shell, char *path)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(path, STDERR_FILENO);
	if (!access(path, F_OK) && access(path, X_OK) && ft_strchr(path, '/'))
		ft_putendl_fd(": Permission denied", STDERR_FILENO);
	else if (!access(path, F_OK) && !access(path, X_OK) && path[0] != '.')
		ft_putendl_fd(": Is a directory", STDERR_FILENO);
	else if (ft_strchr(path, '/') || !env_get_value("PATH", shell))
		ft_putendl_fd(": No such file or directory", STDERR_FILENO);
	else
		ft_putendl_fd(": command not found", STDERR_FILENO);
	g_exit = 127
		- ((!access(path, F_OK) && access(path, X_OK)) || !access(path, F_OK));
	free(path);
	free_exit(shell);
}

static char	*get_path(t_shell *shell, char *cmd)
{
	int		i;
	char	*path;
	char	*path2;
	char	**paths;

	i = 0;
	path = NULL;
	path2 = NULL;
	if (ft_strchr("/.", cmd[0]) || !env_get_value("PATH", shell) || !ft_strcmp(cmd, ""))
		return (ft_strdup(cmd));
	paths = ft_split(env_get_value("PATH", shell), ':');
	while (paths[i])
	{
		path = ft_strjoin(paths[i], "/");
		path2 = ft_strjoin(path, cmd);
		free(path);
		path = NULL;
		if (!access(path2, F_OK))
			return (path2);
		free(path2);
		path2 = NULL;
		i++;
	}
	ft_free_array(paths);
	return (ft_strdup(cmd));
}

static void	expand_argv(t_shell *shell, char **argv)
{
	int		len;
	int		i;
	int		expanded;
	char	*tmp;

	if (!argv[0])
		return ;
	if(ft_strchr(argv[0], '$'))
		expanded = 1;  // Check if is there any envp to be expanded if there is an ocurrence of $
	expand_arg(shell, &argv[0]); //Expands the envp in the first argument
	len = ft_strlen(argv[0]);  //Gets the new lenght of the expanded arg
	trim_arg(shell, argv[0]); //Trims the excess of spaces on the beggining and end of the string
	trim_quotes(argv[0], &len); //Trims quotes and updates length;
	i = 1;
	tmp = argv[0];
	while ((tmp < argv[0] + len) && i < (MAXARGS - 1))
	{
		if (*tmp == '\0' && (ft_strcmp(argv[0], "printf") || i != 2))
			argv[i++] = tmp + 1;
		tmp++;
	}
	if (!argv[0][0] && expanded)
	{
		free(argv[0]);
		argv[0] = NULL;
	}
}

static void	check_exit_status(void)
{
	if (g_exit == 11 || g_exit == 139)
		ft_putendl_fd("Segmentation fault (core dumped)", STDERR_FILENO);
	else if (g_exit == 8 || g_exit == 136)
		ft_putendl_fd("Floating point exception (core dumped)", STDERR_FILENO);
}


void	run_exec(t_shell *shell, t_exec *cmd)
{
	pid_t	pid;
	char	*path;

	expand_argv(shell, cmd->argv);
	if (!cmd->argv[0])				//If the first argument is not valid then return 0
		return (g_exit = 0, (void)0);
	if (run_builtin(shell, cmd))  // Try to use argv as an command for a bultin command, if it succeeds the builtin command will be executed
		return ;
	//sig_handler(SIGCHILD);
	pid = check_fork();   // create a fork and check if its creation was successful
	if (pid == 0) //if it is a child process
	{
		path = get_path(shell, cmd->argv[0]); //gets the command path on the envp and validates it
		execve(path, cmd->argv, shell->envp_char); //executes command and finishes child process
		check_execve_errors(shell, path);
	}
	waitpid(pid, &g_exit, 0); // wait for the child process to finish
	if (WIFEXITED(g_exit))
		g_exit = WEXITSTATUS(g_exit);
	else if (WIFSIGNALED(g_exit))
		g_exit = 128 + WTERMSIG(g_exit);
	check_exit_status();
	//sig_handler(SIGRESTORE);
}