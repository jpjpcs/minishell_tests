/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaosilva <joaosilva@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 17:07:27 by joaosilva         #+#    #+#             */
/*   Updated: 2024/05/07 21:12:33 by joaosilva        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int			g_exit;

// Função para obter o prompt da shell
static char	*show_prompt(void)
{
	char	*cwd;
	char	*prompt;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("Error getting current working directory");
		return (NULL);
	}
	prompt = ft_strjoin(PROMPT_BEGIN, cwd);
	free(cwd);
	cwd = ft_strjoin(prompt, PROMPT_END);
	free(prompt);
	if (!cwd)
	{
		perror("Error creating shell prompt");
		return (NULL);
	}
	return (cwd);
}

static char	*get_prompt(t_shell *shell)
{
	shell->prompt = show_prompt();
	if (!shell->prompt)
	{
		perror("Error creating shell prompt");
		return (NULL);
	}
	shell->line = readline(shell->prompt);
	free(shell->prompt);
	if (!shell->line)
	{
		perror("Error reading user input");
		return (NULL);
	}
	else if (ft_strnstr(shell->line, "exit", ft_strlen(shell->line)))
		exit(0);
	else
		return (shell->line);
}

void	welcome_screen(void)
{
	printf("\nMinishell 1.0\n");
}

static int	init_shell_variables(t_shell *shell, char **envp)
{
	*shell = (t_shell){0};
	envp_to_list(envp, shell);
	envp_update(shell);
	return (1);
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	(void)argv;
	if (!init_shell_variables(&shell, envp))
		return (0);
	if (argc != 1)
	{
		printf("Minishell need no arguments. Try './minishell'.\n");
		return (0);
	}
	else
		welcome_screen();
	while ((1))
	{
		shell.line = get_prompt(&shell);
		if (!shell.line)
			break;
		if (check_args(&shell))
			ft_printf(("args ok, parser entra aqui\n"));
		free(shell.line);
	}
	clear_history();
	if (shell.envp)
		ft_free_array(shell.envp);
	if (isatty(STDIN_FILENO))
		ft_putendl_fd("exit", 2);
	return (g_exit);
}
