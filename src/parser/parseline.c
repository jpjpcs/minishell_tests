/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_parser2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaosilva <joaosilva@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 11:19:59 by joaosilva         #+#    #+#             */
/*   Updated: 2024/05/06 12:47:47 by joaosilva        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	gettoken_type(t_shell *shell)
{
	int	type;

	type = *shell->ps;
	if (*shell->ps == '<')
	{
		if (*(shell->ps + 1) == '<')
			type = HERE_DOC;
	}
	else if (*shell->ps == '>')
	{
		if (*(shell->ps + 1) == '>')
			type = APPEND;
	}
	else if (*shell->ps == '|')
	{
		if (*(shell->ps + 1) == '|')
			type = OR_OP;
	}
	else if (*shell->ps && !ft_strchr(OPERATORS, *shell->ps))
		type = 'a';
	if (*shell->ps && type != 'a')
		shell->ps += (type == HERE_DOC || type == APPEND || type == OR_OP
				|| *shell->ps == '&') + 1;
	return (type);
}

int	gettoken(t_shell *sh, char **token)
{
	int		type;

	while (sh->ps < sh->es && !*sh->ps)
		sh->ps++;
	if (token)
		*token = sh->ps;
	type = gettoken_type(sh);
	while (type == 'a' && sh->ps < sh->es && *sh->ps)
		sh->ps++;
	while (sh->ps < sh->es && !*sh->ps)
		sh->ps++;
	return (type);
}

int	peek(t_shell *shell, char *op, int mode)
{
	while (shell->ps < shell->es && !*shell->ps)
		shell->ps++;
	if (mode == 2)
		return (shell->ps < shell->es && ft_strchr(op, *shell->ps)
			&& *(shell->ps + 1) == *shell->ps);
	return (shell->ps < shell->es && ft_strchr(op, *shell->ps)
		&& *(shell->ps + 1) != *shell->ps);
}

t_cmd	*parseline(t_shell *shell)
{
	t_cmd	*cmd;
	int		type;

	cmd = parsepipeline(shell);
	if (cmd && peek(shell, "&|", 2))
	{
		type = gettoken(shell, NULL);
		if (peek(shell, "|&", 2))
			return (print_error_syntax(shell, shell->ps, 2), cmd);
		if (type == OR_OP)
			cmd = or_cmd(cmd, parseline(shell));
		else if (type == '&')
			cmd = and_cmd(cmd, parseline(shell));
	}
	return (cmd);
}