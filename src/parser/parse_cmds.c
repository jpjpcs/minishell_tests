/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmds.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaosilva <joaosilva@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 12:48:55 by joaosilva         #+#    #+#             */
/*   Updated: 2024/05/23 02:36:10 by joaosilva        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static t_cmd	*parseredir(t_cmd *cmd, t_shell *shell)
{
	int		type;
	char	*token; // Token é o argumento. O argumento é o que vem depois do operador de redirecionamento "<" ou ">".

	while (peek(shell, "<>")) // Verifica se o próximo token é "<" ou ">".
	{
		type = gettoken(shell, NULL); // Obtém o tipo do token. se é "<" ou ">".
		if (gettoken(shell, &token) != 'a') // Se o próximo token não for argumento.
		{
			if (type != '<' || (type == '<' && ft_strcmp(token, ">"))) // Se o tipo for diferente de < ou se o tipo for < e o token for diferente de ">".
				return (print_error_syntax(shell, token, 2), cmd);
			else
				if (gettoken(shell, &token) != 'a') // Se o próximo token não for argumento.
					return (print_error_syntax(shell, token, 2), cmd);
		}
		if (type == '<')
			cmd = redir_cmd(cmd, token, O_RDONLY, 0);
		else if (type == '>')
			cmd = redir_cmd(cmd, token, O_WRONLY | O_CREAT | O_TRUNC, 1);
		else if (type == APPEND)
			cmd = redir_cmd(cmd, token, O_WRONLY | O_CREAT | O_APPEND, 1);
		else if (type == HERE_DOC)
			cmd = here_cmd(cmd, token);
	}
	return (cmd);
}

static t_cmd	*parseexec(t_shell *shell)
{
	t_cmd	*ret;
	t_exec	*cmd;
	char	*token;
	int		type;

	/* if (peek(shell, "(", 1) || peek(shell, "(", 2))
		return (parseblock(shell)); */
	ret = exec_cmd();
	cmd = (t_exec *)ret;
	ret = parseredir(ret, shell);
	while (!peek(shell, "|"))
	{
		type = gettoken(shell, &token);
		if (!type)
			break ;
		if (type != 'a')
			return (print_error_syntax(shell, token, 2), ret);
		if (cmd->argv[0])
			cmd->argv[0] = ft_strjoin_free_s1(cmd->argv[0], " ");
		cmd->argv[0] = ft_strjoin_free_s1(cmd->argv[0], token);
		ret = parseredir(ret, shell);
	}
	return (ret);
}

static t_cmd	*parsepipe(t_shell *shell)
{
	t_cmd	*cmd;

	cmd = parseexec(shell);
	if (cmd && peek(shell, "|"))
	{
		gettoken(shell, NULL);
		cmd = pipe_cmd(cmd, parsepipe(shell));
	}
	return (cmd);
}