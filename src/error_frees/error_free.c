/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_free.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaosilva <joaosilva@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 16:37:39 by joaosilva         #+#    #+#             */
/*   Updated: 2024/05/31 11:48:12 by joaosilva        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	free_exit(t_shell *shell)
{
	if (shell->line)
		free(shell->line);
	free_cmd(shell->cmd);
	ft_envlstclear(shell->env_list_unsorted, free);
	//ft_envlstclear(shell->env_list_sorted, free);
	if (shell->envp_char)
		ft_free_array(shell->envp_char);
	exit(g_exit);
}

// Esta função imprime uma mensagem de erro de sintaxe no stderr.
int	print_error_syntax(t_shell *shell, char *msg, int exit)
{
	ft_putstr_fd(ERROR_TITLE, STDERR_FILENO);                           
		// Imprime o título do erro no stderr.
	ft_putstr_fd("syntax error near unexpected token `", STDERR_FILENO);
		// Imprime uma parte da mensagem de erro no stderr.
	if (*msg == '\0')                                                   
		// Se o primeiro caractere da mensagem for uma nova linha...
		ft_putstr_fd("newline", STDERR_FILENO);                         
			// Imprime "newline" no stderr.
	else
		ft_putchar_fd(*msg, STDERR_FILENO);
			// Imprime o primeiro caractere da mensagem no stderr.
	ft_putendl_fd("'", STDERR_FILENO);     
		// Imprime um apóstrofo e uma nova linha no stderr.
	shell->status = RESTORE;              
		// Define o status da shell como RESTORE.
	g_exit = exit;                          // Define o código de saída global.
	return (1);                            
		// Retorna 1 para indicar que ocorreu um erro.
}

// Esta função imprime uma mensagem de erro no stderr.
int	print_error(t_shell *shell, char *msg, char *msg2, int exit)
{
	ft_putstr_fd(ERROR_TITLE, STDERR_FILENO);
		// Imprime o título do erro no stderr.
	ft_putstr_fd(msg, STDERR_FILENO);        
		// Imprime a primeira parte da mensagem de erro no stderr.
	if (msg2)                                
		// Se a segunda mensagem existir...
		ft_putstr_fd(": ", STDERR_FILENO);   
			// Imprime dois pontos seguido de espaço no stderr.
	else                                      // Caso contrário...
		ft_putchar_fd('\n', STDERR_FILENO);  
			// Imprime uma nova linha no stderr.
	ft_putendl_fd(msg2, STDERR_FILENO);      
		// Imprime a segunda parte da mensagem de erro no stderr,
		// seguida de uma nova linha.
	shell->status = RESTORE;                 
		// Define o status da shell como RESTORE.
	g_exit = exit;                           
		// Define o código de saída global.
	return (1);                              
		// Retorna 1 para indicar que ocorreu um erro.
}

int	error_inside(t_shell *shell, char *cmd, char *arg, int error_code)
{
	char	*err;

	err = ft_strjoin(cmd, arg);
	print_error(shell, err, "not a valid identifier", error_code);
	free(err);
	return (0);
}
