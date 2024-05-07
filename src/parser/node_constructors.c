/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node_constructors_1.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaosilva <joaosilva@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 19:32:08 by joaosilva         #+#    #+#             */
/*   Updated: 2024/04/26 11:19:53 by joaosilva        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_cmd	*block_cmd(t_cmd *cmd)
{
	t_block	*new;

	if (!cmd)
		return (NULL);
	new = (t_block *)ft_calloc(1, sizeof(t_block));
	new->type = BLOCK;
	new->cmd = cmd;
	return ((t_cmd *)new);
}

t_cmd	*redir_cmd(t_cmd *cmd, char *file, int mode, int fd)
{
	t_redir	*redir;
	t_cmd	*tmp;
	t_cmd	*tmp2;

	redir = (t_redir *)ft_calloc(1, sizeof(t_redir));
	redir->type = REDIR;
	redir->file = ft_strdup(file);
	redir->mode = mode;
	redir->fd = fd;
	if (cmd->type == EXEC || cmd->type == BLOCK)
		redir->cmd = cmd;
	else
	{
		tmp = cmd;
		while (tmp->type != EXEC && tmp->type != BLOCK)
		{
			tmp2 = tmp;
			tmp = ((t_redir *)tmp)->cmd;
		}
		((t_redir *)tmp2)->cmd = (t_cmd *)redir;
		redir->cmd = tmp;
		return (cmd);
	}
	return ((t_cmd *)redir);
}

t_cmd	*pipe_cmd(t_cmd *left, t_cmd *right)
{
	t_pipe	*cmd;

	cmd = (t_pipe *)ft_calloc(1, sizeof(t_pipe));
	cmd->type = PIPE;
	cmd->left = left;
	cmd->right = right;
	return ((t_cmd *)cmd);
}

t_cmd	*exec_cmd(void)
{
	t_exec	*cmd;

	cmd = (t_exec *)ft_calloc(1, sizeof(t_exec));
	cmd->type = EXEC;
	return ((t_cmd *)cmd);
}
