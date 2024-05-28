/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crocha-s <crocha-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 18:52:19 by crocha-s          #+#    #+#             */
/*   Updated: 2024/05/27 16:00:59 by crocha-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char	*path_slash(char *cdpath, char **path)
{
	char	*tmp;

	if (cdpath[ft_strlen(cdpath) - 1] != '/')
	{
		if (*path[0] != '/')
		{
			tmp = ft_strjoin("/", *path);
			return (tmp);
		}
		else
			return (ft_strdup(cdpath));
	}
	else
	{
		if (*path[0] == '/')
		{
			tmp = ft_strdup(*path + 1);
			return (tmp);
		}
		else
			return (ft_strdup(cdpath));
	}
}

static bool	cdpath_try(t_shell *shell, char **cdpath, char *path, int index)
{
	char	*tmp;
	char	*tmp_path;

	tmp_path = path_slash(cdpath[index], &path);
	tmp = ft_strjoin(cdpath[index], tmp_path);
	free(tmp_path);
	if (ms_chdir(shell, tmp))
	{
		tmp[ft_strlen(tmp) - 1] = '\0';
		ft_putendl_fd(tmp, STDOUT_FILENO);
		ft_free_array(cdpath);
		free(tmp);
		return (true);
	}
	free(tmp);
	return (false);
}

static bool	cdpath(t_shell *shell, char *path)
{
	char	**cdpath;
	int		i;

	if (!env_get_value("CDPATH", shell) || path[0] == '/')
		return (false);
	cdpath = ft_split(env_get_value("CDPATH", shell), ':');
	i = 0;
	while (cdpath[i])
	{
		if (cdpath_try(shell, cdpath, path, i++))
			return (true);
	}
	ft_free_array(cdpath);
	return (false);
}

static void	hyphen_cd_print(t_shell *shell, char *pwd)
{
	char	*str;

	if (pwd[0] != '~')                         // check if there is a "~" on the first pwd argument
		ft_putendl_fd(pwd, STDOUT_FILENO);     // if there is not print pwd
	else
	{
		str = ft_strjoin(env_get_value("HOME", shell), &pwd[1]); 
		ft_putendl_fd(str, STDOUT_FILENO);
		free(str);
	}
}

void	ms_cd(t_shell *shell, t_exec *cmd)
{
	if (!cmd->argv[1] || !*cmd->argv[1])   // check if the second argument is valid
	{
		if (!ms_chdir(shell, env_get_value("HOME", shell)))   //if chdir is not successful on obtaining the HOME name in shell envp ...
			print_error(shell, "cd", "HOME not set", 1); // ... print error is called
	}
	else //if argument is valid
	{
		if (cmd->argv[2])  //check if there is a third argument 
			print_error(shell, "cd", "too many arguments", 1); // if there is third argument print error is called, cd does not support arguments
		else if (ft_strcmp(cmd->argv[1], "-") == 0)  // verifies if string compare found a "-" in the second argument. "cd -" gets the last cwd which is recorded on OLDPWD
		{
			if (!ms_chdir(shell, env_get_value("OLDPWD", shell))) // call function to change directory,if an old pwd is recorded on the envp the directory is going to be changed to it
			{
				print_error(shell, "cd", "OLDPWD not set", 1); // if its not recorded prints and error and return
				return ;
			}
			hyphen_cd_print(shell, env_get_value("PWD", shell)); // calls function that verifies if there is an "~" in the second argument, if its true sets HOME as cwd.
		}
		else if (cmd->argv[1][0]
			&& !ms_chdir(shell, cmd->argv[1]) && !cdpath(shell, cmd->argv[1]))
			print_error(shell, "cd: no such file or directory",
				cmd->argv[1], 1);
	}
	if (shell->status == CONTINUE)
		g_exit = 0;
}