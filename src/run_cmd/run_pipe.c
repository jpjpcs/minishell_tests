/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_pipe.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crocha-s <crocha-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 17:32:04 by luide-so          #+#    #+#             */
/*   Updated: 2024/05/27 15:43:31 by crocha-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	wait_children(t_shell *shell)
{
	if (waitpid(shell->pid, &g_exit, 0) != -1)
	{
		if (WIFEXITED(g_exit))
			g_exit = WEXITSTATUS(g_exit);
		else if (WIFSIGNALED(g_exit))
			g_exit = WTERMSIG(g_exit) + 128;
	}
	while (wait(0) != -1)
		;
	if (g_exit == 130)
		shell->status = RESTORE;
}


static void	close_fds_and_sig_handler(int fd[2], int sig)
{
	if (sig)
		//sig_handler(sig);
	check(close(fd[0]), "close error", 127);
	check(close(fd[1]), "close error", 127);
}

void	run_pipe(t_shell *shell, t_pipe *cmd)
{
	int		fd[2];

	check(pipe(fd), "pipe error", 127);
	shell->pid = check_fork();
	if (shell->pid == 0)
	{
		check(dup2(fd[1], STDOUT_FILENO), "dup2 error", 127);
		close_fds_and_sig_handler(fd, SIGIGNORE);
		run_cmd(shell, cmd->left);
		free_exit(shell);
	}
	if (cmd->left->type == HERE_DOC)
		wait_children(shell);
	if (shell->status == CONTINUE)
		shell->pid = check_fork();
	if (shell->pid == 0)
	{
		check(dup2(fd[0], STDIN_FILENO), "dup2 error", 127);
		close_fds_and_sig_handler(fd, SIGIGNORE);
		run_cmd(shell, cmd->right);
		free_exit(shell);
	}
	close_fds_and_sig_handler(fd, 0);
	wait_children(shell);
}