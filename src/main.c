/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaosilva <joaosilva@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 17:07:27 by joaosilva         #+#    #+#             */
/*   Updated: 2024/05/26 10:26:01 by joaosilva        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* 

código do veho

int main(void)
{
	static char buf [100];
	int fd;
	
	while (fd = open ("console", O_RDWR) >= 0)
	{
		if (fd >= 3)
		{
			close (fd);
			break;
		}
	}
	while (getcmd(buf, sizeof(buf)) >= 0)
	{
		if (buf[0] == 'c' && buf [1] == 'd' && buf[2] == ' ')
		{
			buf [strlen(buf) - 1] = 0;
			if (chdir (buf + 3) < 0)
				fprintf (2, "cannot cd %s\n", buf+3);
			continue;
		}
		if (fork1() == 0)
			run_cmd (parse_cmd, (buf));
		wait (0);
	}
	exit(0);
}

void panic (char *s)
{
	fprintf (2, "%s\n", s);
	exit(1);
}

int for1(void)
{
	int pid;
	pid = fork();
	if (pid == -1)
	{
		panic("fork");
	}
	return (pid);
}
*/

// O minishell saí do programa em duas situações:
// 1 - qd escrevemos "exit" na prompt ou 
// 2 - qd fazemos ctrl + D.
// Assim, o minishell é um programa que corre em 
// loop até que o utilizador escreva "exit" ou faça ctrl + D. 
// Contudo, isso levaria a que o minishell corresse o run_cmd
// até ao fim até entender que o comando estava errado (o que
// iria entender uma vez que ao longo do run_pipe, run_exec, 
// e run_redir é feita análise sintática. Logo, os erros no 
// comando seriam detetados e o minishell iria imprimir o erro.
// Mas para não "obrigar" o minishell a correr todo o programa
// até chegar aí, nós preferimos que após o process_line (a
// primeira análise sintática realizada), o programa terminasse 
// na segunda análise sintática realizada (que é feita no parse_cmd).
// O parse_cmd contempla o parsepipe, que por sua vez contempla 
// o parseexec, que por sua vez contempla o parseredir. Em todos 
// estas funções é feita análise sintática (a parte do print_error_syntax
// e do print_error que está em cada uma dessas funções caso o comando
// seja errado). Assim, se o comando for errado nem sequer corremos o
// programa até ao fim, ou seja, não passamos do parser_cmd para
// o run_cmd. Assim, o minishell termina após a segunda análise
// sintática realizada, tornando o programa mais eficiente.
// Outra abordagem poderia ter sido adotada de forma a correr o programa
// até ao fim e só terminar após escrevermos exit ou fazermos ctrl + D..
// Nós optamos por esta abordagem porque achamos que é mais eficiente.
// Para tal utilizamos uma flag chamada status (shell->status) que é
// alterada em cada análise sintática realizada, fazendo com que o loop
// principal (chamado na função while(to_run) no main termine ou 
// continue. Como? Se a análise sintática não for bem sucedida,
// a flag é alterada para STOP. Se a análise sintática
// for bem sucedida, a flag é alterada para CONTINUE. Se a flag for RESTORE, 
// o minishell restaura o estado anterior. Assim, o minishell corre até 
// que a flag seja STOP. Se a flag for CONTINUE ou RESTORE, o minishell
// continua a correr.

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

static int to_run( t_shell *shell)
{
	
	sig_handler(SIGRESTORE);
	shell->status = STOP;
	//shell->exec_cmd = true;
	shell->line = get_prompt(&shell);
	if (shell->line && process_line(&shell))
	{
		if (parse_cmd(&shell))
		{
			sig_handler(SIGPIPE);
			run_cmd(&shell, shell->cmd);
		}
		free_cmd(shell->cmd);
	}
	free(shell->line);
	return (shell->status);
}
void	welcome_screen(void)
{
	printf("\nMinishell 1.0\n");
}

static int	init_shell_variables(t_shell *shell, char **envp)
{
	*shell = (t_shell){0};
	convert_envp(envp, shell);
	return (1);
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;
	// g_exit = 0;
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
		
	while (to_run(&shell));
	clear_history();
	ft_envlstclear (shell.env_list, free);
	if (shell.envp_char) // If the shell's environment copy exists...
		ft_free_array(shell.envp_char);  // Free the memory allocated for it.
	if (isatty(STDIN_FILENO)) // If the shell is connected to a terminal... is running in interactive mode
		ft_putendl_fd("exit", 2); // Print "exit" to the standard error output.
	return (g_exit);
}


