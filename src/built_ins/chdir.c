/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chdir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crocha-s <crocha-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/07 12:46:43 by achien-k          #+#    #+#             */
/*   Updated: 2024/05/18 22:08:09 by crocha-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

bool	ms_chdir(t_shell *shell, char *path)
{
	char	*tmp_pwd;

	if (!path)                    // checks if the given path is valid
		return (false);
	tmp_pwd = getcwd(NULL, 0);    // gets the current working directory
	if (chdir(path) != 0)         // check if the command to change directory is successful. 0 is for error
	{
		free(tmp_pwd);
		return (false);
	}
	env_export(shell, "OLDPWD", tmp_pwd, 1); // exports an env named OLDPWD with the path before change directory
	free(tmp_pwd);
	tmp_pwd = getcwd(NULL, 0); //get current pwd
	env_export(shell, "PWD", tmp_pwd, 1); // exports an env with the cwd
	free(tmp_pwd);
	return (true);
}