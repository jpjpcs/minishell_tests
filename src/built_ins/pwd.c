/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jode-jes <jode-jes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 18:49:24 by crocha-s          #+#    #+#             */
/*   Updated: 2024/05/27 15:26:00 by jode-jes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ms_pwd(t_shell *shell, t_exec *cmd)
{
	char	*path;

	if (cmd->argv[1] && cmd->argv[1][0] == '-')
	{
		print_error(shell, cmd->argv[0], "flags not supported", 2);
		return ;
	}
	path = getcwd(NULL, 0);
	if (!path)
		print_error(shell, cmd->argv[0], "couldn't get current directory", 2);
	ft_putendl_fd(path, STDOUT_FILENO);
	free(path);
	if (shell->status == CONTINUE)
		g_exit = 0;
}