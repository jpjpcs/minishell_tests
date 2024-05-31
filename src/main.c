/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaosilva <joaosilva@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 17:07:27 by joaosilva         #+#    #+#             */
/*   Updated: 2024/05/31 09:10:52 by joaosilva        ###   ########.fr       */
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

static int	to_run(t_shell *shell)
{
	// sig_handler(SIGRESTORE);
	shell->status = STOP;
	shell->exec_cmd = true;
	shell->line = get_prompt(shell);
	if (shell->line && process_line(shell))
	{
		if (parse_cmd(shell))
		{
			// sig_handler(SIGPIPE);
			run_cmd(shell, shell->cmd);
		}
		free_cmd(shell->cmd);
	}
	free(shell->line);
	return (shell->status);
}
// convert_envp_to_linked_lists(envp, shell) 
//para usar nos comando env (ordem criação) e 
//export (ordem alfabética).
//convert_envp_to_char(shell) para criar o char **envp_to_char 
// q é o que será passado para o execve,para que o processo 
// filho tenha acesso às variáveis de ambiente.

static int	init_shell_variables(t_shell *shell, char **envp)
{
	g_exit = 0;
	*shell = (t_shell){0};
	convert_envp_to_linked_lists(envp, shell);
	convert_envp_to_char(shell);
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
		printf("\nMinishell 1.0\n");
	while (to_run(&shell))
		;
	clear_history();
	ft_envlstclear(shell.env_list_unsorted, free);
	ft_envlstclear(shell.env_list_sorted, free);
	if (shell.envp_char)
		// If the shell's environment copy exists...
		ft_free_array(shell.envp_char); // Free the memory allocated for it.
	if (isatty(STDIN_FILENO))
		// If the shell is connected to a terminal... is running in interactive mode
		ft_putendl_fd("exit", 2);
	// Print "exit" to the standard error output.
	return (g_exit);
}
