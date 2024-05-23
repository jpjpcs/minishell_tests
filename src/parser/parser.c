/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaosilva <joaosilva@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 19:43:25 by joaosilva         #+#    #+#             */
/*   Updated: 2024/05/23 03:35:44 by joaosilva        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	gettoken_type(t_shell *shell)
{
	int	type;

	type = *shell->ps; // o tipo fica logo igual ao shell->ps. Logo se for > ou < fica logo definido como tal. Só append e heredoc precisam de confirmar se o tipo a seguir igual ao anterior, isto é, se for > o tipo inicial, se tiver outro > a seguir é pq é append. O mm raciocínio para o heredoc. Se for pipe fica só definido como pipe.
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
	/* else if (*shell->ps == '|')
	{
		if (*(shell->ps + 1) == '|')
			type = OR_OP;
	} */
	else if (*shell->ps && !ft_strchr(OPERATORS, *shell->ps))
		type = 'a';
	if (*shell->ps && type != 'a')
??????????		shell->ps += (type == HERE_DOC || type == APPEND) + 1; /// verifica se o tipo é HERE_DOC ou APPEND e incrementa ps em 1
		//|| type == OR_OP || *shell->ps == '&') + 1;
	return (type);
}

int	gettoken(t_shell *sh, char **token)
{
	int		type;

	while (sh->ps < sh->es && !*sh->ps) // Enquanto ps for menor que es e shell->ps for nulo.
		sh->ps++; // assim passa para o proximo token (o próximo argumento a ser avaliado)
	if (token) // Se token existir
		*token = sh->ps; // token recebe o valor de ps
	type = gettoken_type(sh);
	while (type == 'a' && sh->ps < sh->es && *sh->ps)
		sh->ps++;
	while (sh->ps < sh->es && !*sh->ps)
		sh->ps++;
	return (type); //??????? RETORNAS O TIPO E NÃO O RET PORQUÊ?
}

int	peek(t_shell *shell, char *op)
{
	while (shell->ps < shell->es && ft_strchr(op, shell->ps) && !*shell->ps) // Enquanto ps for menor que es e encontrar um caractere em op e não for nulo.
		shell->ps++;
	return (shell->ps && ft_strchr(op, shell->ps)); // Retorna 1 se encontrar um caractere em op, 0 caso contrário
}

t_cmd	*parse_cmd(t_shell *shell)
{
	shell->ps = shell->line;
    shell->es = shell->line + shell->line_len;
    shell->cmd = parsepipe(shell);
    peek(shell, "");
	if (shell->ps != shell->es) // Se ps for diferente de es
	{
		return (!print_error_syntax(shell, shell->ps, 2));
	}
	return shell->cmd; // Retorna o comando
}