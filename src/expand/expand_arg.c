/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_arg.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaosilva <joaosilva@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 10:48:05 by joaosilva         #+#    #+#             */
/*   Updated: 2024/06/04 02:58:25 by joaosilva        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
//Código do Balsa

#include "../../include/minishell.h"

static int	point_to_exp_tilde(t_shell *sh, int point, char *tmp, char **line)
{
	if (!tmp[1] || ft_strchr(NOT_EXP, tmp[1]))
		return (expand(env_get("HOME", sh), point, point + 1, line));
	else if (tmp[1] == '+' && (!tmp[2] || ft_strchr(NOT_EXP, tmp[2])))
		return (expand(env_get("PWD", sh), point, point + 2, line));
	else if (tmp[1] == '-' && (!tmp[2] || ft_strchr(NOT_EXP, tmp[2])))
		return (expand(env_get("OLDPWD", sh), point, point + 2, line));
	else if (*tmp == '$' && tmp[1] == '?')
		return (expand_free(ft_itoa(g_exit), point, point + 2, line));
	return (0);
}

static int	expand_tilde(t_shell *shell, char **line)
{
	int		dquote;
	int		squote;
	char	*tmp;

	dquote = 0;
	squote = 0;
	tmp = *line;
	while (*tmp)
	{
		if (*tmp == '"' && !squote)
			dquote = !dquote;
		if (*tmp == '\'' && !dquote)
			squote = !squote;
		if (*tmp == '~' && !dquote && !squote
			&& (tmp == *line || ft_strchr(SPACES, *(tmp - 1))))
			if (point_to_exp_tilde(shell, tmp - *line, tmp, line))
				tmp = *line;
		if (*tmp)
			tmp++;
	}
	return (0);
}

static int	point_to_expand_env(t_shell *sh, int point, char *tmp, char **line)
{
	char	*key;
	int		len;

	if (tmp[1] == '?')
		return (expand_free(ft_itoa(g_exit), point, point + 2, line));
	else if (tmp[1])
	{
		len = 1;
		while (ft_isalpha(tmp[len]) || tmp[len] == '_')
			len++;
		len += (ft_isalnum(tmp[len]) > 0);
		while (len > 2 && (ft_isalnum(tmp[len]) || tmp[len] == '_'))
			len++;
		key = ft_substr(tmp, 1, len - 1);
		expand(env_get(key, sh), point, point + len, line);
		return (free(key), 1);
	}
	return (0);
}

static void	env_expand(t_shell *shell, char *tmp, char **line)
{
	int	dquote;
	int	squote;

	dquote = 0;
	squote = 0;
	while (*(++tmp))
	{
		if (*tmp == '"' && !squote)
			dquote = !dquote;
		if (*tmp == '\'' && !dquote)
			squote = !squote;
		if (*tmp == '$' && !ft_strchr(NOT_EXP, *(tmp + 1)) && !squote
			&& !((dquote || squote) && (*(tmp + 1) == '"'
					|| *(tmp + 1) == '\'')))
		{
			if (point_to_expand_env(shell, tmp - *line, tmp, line))
			{
				tmp = *line - 1;
				dquote = 0;
				squote = 0;
			}
		}
	}
}

void	expand_arg(t_shell *shell, char **arg)
{
	expand_tilde(shell, arg);
	env_expand(shell, *arg - 1, arg);
	expand_wildcard(arg);
}
*/

#include "../../include/minishell.h"


 int	inside_quotes(char *line, char *current_position)
{
	int	quote;

	quote = 0;
	while (*line)
	{
		if (*line == '"' || *line == '\'')
		{
			if (quote == 0)
				quote = *line;
					// Se a quote não existir é definida aqui como existente
			else if (quote == *line)
				quote = 0; // Se a quote existir é definida aqui
		}
		if (&line == &current_position)
			break ;
		line++;
	}
	return (quote);
		// Retorna o status de citação atual se nenhuma das condições acima for verdadeira
}
static int	point_to_exp_tilde(t_shell *sh, int point, char *tmp, char **line)
{
	if (!tmp[1] || ft_strchr(NOT_EXP, tmp[1]))
		return (expand_line(env_get_value("HOME", sh), point, point + 1, line));
	else if (tmp[1] == '+' && (!tmp[2] || ft_strchr(NOT_EXP, tmp[2])))
		return (expand_line(env_get_value("PWD", sh), point, point + 2, line));
	else if (tmp[1] == '-' && (!tmp[2] || ft_strchr(NOT_EXP, tmp[2])))
		return (expand_line(env_get_value("OLDPWD", sh), point, point + 2,
				line));
	return (0);
}

/* static int	expand_tilde(t_shell *shell, char **line)
{
	char	*tmp;

	tmp = *line;
	while (*tmp)
	{
		if (*tmp == '~' && !inside_quotes(shell->line, tmp) && (tmp == *line
				|| ft_strchr(SPACES, *(tmp - 1))))
			if (point_to_exp_tilde(shell, tmp - *line, tmp, line))
				tmp = *line;
		if (*tmp)
			tmp++;
	}
	return (0);
} */
static int	expand_tilde(t_shell *shell, char **line)
{
	int		dquote;
	int		squote;
	char	*tmp;

	dquote = 0;
	squote = 0;
	tmp = *line;
	while (*tmp)
	{
		if (*tmp == '"' && !squote)
			dquote = !dquote;
		if (*tmp == '\'' && !dquote)
			squote = !squote;
		if (*tmp == '~' && !dquote && !squote
			&& (tmp == *line || ft_strchr(SPACES, *(tmp - 1))))
			if (point_to_exp_tilde(shell, tmp - *line, tmp, line))
				tmp = *line;
		if (*tmp)
			tmp++;
	}
	return (0);
}

static int	point_to_expand_env(t_shell *sh, int point, char *tmp, char **line)
{
	char	*key;
	int		len;

	if (tmp[1] == '?')
		return (expand_free(ft_itoa(g_exit), point, point + 2, line));
	else if (tmp[1])
	{
		len = 1;
		while (ft_isalpha(tmp[len]) || tmp[len] == '_')
			len++;
		len += (ft_isalnum(tmp[len]) > 0);
		while (len > 2 && (ft_isalnum(tmp[len]) || tmp[len] == '_'))
			len++;
		key = ft_substr(tmp, 1, len - 1);
		expand_line(env_get_value(key, sh), point, point + len, line);
		return (free(key), 1);
	}
	return (0);
}

/* static void	env_expand(t_shell *shell, char *tmp, char **line)
{
	int quote;

	quote = 0;
	while (*(++tmp))
	{
		if (*tmp == '$' && !ft_strchr(NOT_EXP, *(tmp + 1))
			&& !inside_quotes(shell->line, tmp) && !((*(tmp + 1) == '"' || *(tmp
						+ 1) == '\'')))
		{
			if (point_to_expand_env(shell, tmp - *line, tmp, line))
			{
				tmp = *line - 1;
				quote = 0;
			}
		}
	}
} */

static void	env_expand(t_shell *shell, char *tmp, char **line)
{
	int		dquote;
	int		squote;

	dquote = 0;
	squote = 0;
	while (*(++tmp))
	{
		if (*tmp == '"' && !squote)
			dquote = !dquote;
		if (*tmp == '\'' && !dquote)
			squote = !squote;
		if (*tmp == '$' && !ft_strchr(NOT_EXP, *(tmp + 1)) && !squote
			&& !((dquote || squote) && (*(tmp + 1) == '"'
					|| *(tmp + 1) == '\'')))
		{
			if (point_to_expand_env(shell, tmp - *line, tmp, line))
			{
				tmp = *line - 1;
				dquote = 0;
				squote = 0;
			}
		}
	}
}

void	expand_arg(t_shell *shell, char **arg)
{
	expand_tilde(shell, arg);
	env_expand(shell, *arg - 1, arg);
}
