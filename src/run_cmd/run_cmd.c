/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaosilva <joaosilva@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 11:25:05 by joaosilva         #+#    #+#             */
/*   Updated: 2024/05/03 11:25:19 by joaosilva        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"


void	run_cmd(t_shell *shell, t_cmd *cmd)
{
	if (cmd->type == EXEC && shell->exec_cmd == true)
		run_exec(shell, (t_exec *)cmd);
	else if (cmd->type == REDIR && shell->exec_cmd == true)
		run_redir(shell, (t_redir *)cmd);
	else if (cmd->type == HERE_DOC)
		run_heredoc(shell, (t_here *)cmd);
	else if (cmd->type == PIPE)
		run_pipe(shell, (t_pipe *)cmd);
/* 	else if (cmd->type == AND)
		run_and(shell, (t_lrn *)cmd);
	else if (cmd->type == OR_OP)
		run_or(shell, (t_lrn *)cmd); */
	else if (cmd->type == BLOCK && shell->exec_cmd == true)
		run_block(shell, (t_block *)cmd);
}