/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaosilva <joaosilva@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 19:43:25 by joaosilva         #+#    #+#             */
/*   Updated: 2024/05/03 14:30:49 by joaosilva        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// This function expands the command line by inserting spaces around certain characters.
static void	expand_line(t_shell *sh, char *tmp)
{
	int		dquote;
	int		squote;

	dquote = 0; // Flag to track if inside double quotes
	squote = 0; // Flag to track if inside single quotes
	while (*(++tmp)) // Loop through each character in the line
	{
		if (*tmp == '"' && !squote) // If encountering double quotes and not inside single quotes
			dquote = !dquote; // Toggle the double quote flag. "Toggle" significa "alternar" ou "trocar". Portanto, "Toggle the double quote flag" pode ser traduzido para o português como "Alternar o sinalizador de aspas duplas". Isso implica que o sinalizador está sendo invertido, ou seja, se estava ativado, será desativado, e vice-versa. Isso significa que o valor da variável Dquote, que representa o sinalizador de aspas duplas (double quote), está sendo invertido. Se dquote estiver atualmente definido como verdadeiro (true), essa linha de código o definirá como falso (false), e vice-versa. Em outras palavras, o sinalizador está sendo alternado entre verdadeiro e falso.
		if (*tmp == '\'' && !dquote) // If encountering single quotes and not inside double quotes
			squote = !squote; // Toggle the single quote flag. Isso significa que o valor da variável squote, que representa o sinalizador de aspas simples (single quote), está sendo invertido. Se squote estiver atualmente definido como verdadeiro (true), essa linha de código o definirá como falso (false), e vice-versa. Em outras palavras, o sinalizador está sendo alternado entre verdadeiro e falso.
		if (ft_strchr(OPERATORS, *tmp) && !dquote && !squote) // If encountering an operator and not inside quotes
		{
			if (tmp != sh->line && !ft_strchr(" |><&()", *(tmp - 1))) // If the character is not at the beginning of the line and the previous character is not an operator
			{
				if (expand(" ", tmp - sh->line, tmp - sh->line, &sh->line)) // Insert a space before the operator
					tmp = sh->line - 1; // Move the temporary pointer back to the beginning of the line
			}
			else if (!ft_strchr(" |><&()", *(tmp + 1))) // If the character is not at the end of the line and the next character is not an operator
				if (expand(" ", tmp - sh->line + 1,
						tmp - sh->line + 1, &sh->line)) // Insert a space after the operator
					tmp = sh->line - 1; // Move the temporary pointer back to the beginning of the line
		}
	}
	sh->line_len = ft_strlen(sh->line); // Update the length of the line
}

// This function trims leading and trailing spaces from the command line.
static void	trim_line(t_shell *shell)
{
	char	*tmp;
	int		squote;
	int		dquote;

	dquote = 0; // Flag to track if inside double quotes
	squote = 0; // Flag to track if inside single quotes
	tmp = shell->line; // Set a temporary pointer to the beginning of the line
	while (*tmp) // Loop through each character in the line
	{
		if (*tmp == '"' && !squote) // If encountering double quotes and not inside single quotes
			dquote = !dquote; // Toggle the double quote flag
		if (*tmp == '\'' && !dquote) // If encountering single quotes and not inside double quotes
			squote = !squote; // Toggle the single quote flag
		if (ft_strchr(SPACES, *tmp) && !squote && !dquote) // If encountering a space and not inside quotes
			*tmp = '\0'; // Replace the space with null character
		tmp++; // Move to the next character
	}
}

// This function checks for syntax errors in the command line.
static void	check_syntax_errors(t_shell *shell)
{
	char	*tmp;
	
	int dquote = 0; // Inicializa a flag de aspas duplas como 0 para indicar que o comando não está dentro de aspas duplas. Flag to track if the command is inside double quotes (0 = false, 1 = true).
	int squote = 0; // Inicializa a flag de aspas simples como 0 para indicar que o comando não está dentro de aspas simples. Flag to track if the command is inside single quotes (0 = false, 1 = true).
	tmp = shell->line - 1; // Set a temporary pointer to the beginning of the line
	if (ft_strchr("|;&", *shell->line)) // If the first character is one of '|', ';' or '&'
		return (print_error_syntax(shell, shell->line, 2)); // Print syntax error and return 2
	if (shell->line[ft_strlen(shell->line) - 1] == '|' // If the last character is '|'
		|| shell->line[ft_strlen(shell->line) - 1] == '&') // Or the last character is '&'
		return (print_error(shell,
				"Open | or || or && not supported", NULL, 2)); // Print error message and return 2
	while (*++tmp) // Loop through each character in the line
	{
		if (*tmp == '"' && !squote) // If encountering double quotes and not inside single quotes
			dquote = !dquote; // Toggle the double quote flag
		if (*tmp == '\'' && !dquote) // If encountering single quotes and not inside double quotes
			squote = !squote; // Toggle the single quote flag
		if (*tmp == '&' && !dquote && !squote) // If encountering '&' and not inside quotes
			if (*(tmp + 1) != '&' && *(tmp - 1) != '&') // If '&' is not followed or preceded by another '&'
				return (print_error(shell, "no support for single &", NULL, 2)); // Print error message and return 2
	}
	if (dquote || squote) // If there are unmatched quotes
		return (print_error(shell, ERROR_QUOTE, NULL, 2)); // Print error message and return 2
	//return (0); // Se não tem erros, então retorna 0 (falso), de forma a que o if que chama a função (o check_syntax_errors na função treat_cmd_line) não seja executado, continuando o programa e a análise da linha de comando.
}

// Esta função inicializa a linha de comando removendo os espaços à frente e atrás, verificando erros de sintaxe, expandindo a linha e removendo espaços. This function initializes the command line by trimming leading and trailing spaces, checking syntax errors, expanding line, and trimming spaces.
static int	treat_cmd_line(t_shell *shell)
{
	char	*tmp;

	//shell->status = CONTINUE; // Set the shell status to CONTINUE
	//tmp = shell->line; // ????????????? Store the pointer to the original line ENTÃO CRIAS UM FICHEIRO temp E LIBERTA-LO A SEGUIR SEM NUNCA O TERES USADO PARA NADA!!!?
	shell->line = ft_strtrim(shell->line, SPACES); // ???????????? Trim leading and trailing spaces from the line. ESPACÇOS SÃO RETIRADOS NO INÍCIO E NO FIM DA STRING. E NO MEIO?
	//free(tmp); // Free the memory allocated for the original line
	if (shell->line[0] == '\0') // If the line is empty after trimming
		return (0); // ??????????? Return 0. PORQUE TEMOS DE RETORNAR ZERO AQUI, SE A STRTRIM JÁ RETORNA UMA CÓPIA VAZIA DA S1 NESSE CASO?
	add_history(shell->line); // É uma função da biblioteca readline que adiciona a linha ao histórico de comandos. Adds the line to the command history.
	check_syntax_errors(shell); // ?????? RETIREI A INICIALIZAÇÃO DA SQUOTE E DA DQUOTE E RETIREI O IF (NÃO FAZIA SENTIDO) E COLOQUEI A FUNÇÃO VOID. Verifica se há erros de sintaxe na linha de comando, como caracteres inesperados ou falta de aspas. Check for syntax errors in the command line, such as unexpected characters/unsupported operators and missing/unmatched quotes.
	expand_line(shell, shell->line - 1); // Expand line by inserting spaces around certain characters
	trim_line(shell); // Trim spaces from the line
	return (1); // Return 1
}

t_cmd	*parse_cmd(t_shell *shell)
{
	if (!treat_cmd_line(shell)) // Trata a linha de comando, retirando espaços, verificando erros de escrita (se no início ou final da palavra existem os símbolos &;|, e verificando as quotes), etc. acrescentei a init_line aqui e mudei-lhe o nome pq o que ela faz é tratar os dados. If there is an error in the command line, return NULL. if (!treat_cmd_line(shell)) verifica se treat_cmd_line(shell) retorna 0, o que indica um erro na linha de comando. Se houver um erro, o código dentro do bloco if será executado, que libera a memória alocada para a linha e retorna NULL
		return (NULL); // Return NULL
	{
		free(shell->line); // Free the memory allocated for the line
		return (NULL); // Return NULL
	}
	shell->ps = shell->line;
    shell->es = shell->line + shell->line_len;
    shell->cmd = parseline(shell);
    peek(shell, "", 0);
	
	// Não verificamos o erro de sintaxe aqui
    
	return shell->cmd; // Retorna o comando
}
/*
// Esta função remove os caracteres especificados em 'set' das extremidades da string 's1'.
char	*ft_strtrim(char const *s1, char const *set)
{
	int		beguin; // Índice para o início da string após a remoção dos caracteres especificados.
	int		end; // Índice para o final da string após a remoção dos caracteres especificados.
	char	*c; // Ponteiro para a nova string após a remoção dos caracteres.

	end = ft_strlen(s1) - 1; // Define o índice do último caractere de 's1'.
	beguin = 0; // Define o índice inicial como 0.
	while (beguin <= end && ft_strchr(set, s1[beguin])) // Avança 'beguin' enquanto os caracteres em 'set' estiverem presentes no início de 's1'.
		beguin++;
	if (beguin > end) // Se 'beguin' ultrapassar 'end', significa que 's1' consiste apenas em caracteres em 'set'.
		return (ft_strdup(&s1[end + 1])); // Retorna uma cópia vazia de 's1'.
	while (end >= 0 && ft_strchr(set, s1[end])) // Retrocede 'end' enquanto os caracteres em 'set' estiverem presentes no final de 's1'.
		end--;
	c = (char *) malloc(sizeof(char) * (end - beguin + 1) + 1); // Aloca memória para a nova string, considerando a quantidade de caracteres remanescentes.
	if (!c) // Verifica se a alocação de memória foi bem-sucedida.
		return (NULL); // Retorna NULL se a alocação de memória falhar.
	ft_strlcpy(c, &s1[beguin], (end - beguin + 1) + 1); // Copia os caracteres remanescentes de 's1' para 'c'.
	return (c); // Retorna a nova string sem os caracteres especificados em 'set'.
}
*/