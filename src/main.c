/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaosilva <joaosilva@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 17:07:27 by joaosilva         #+#    #+#             */
/*   Updated: 2024/05/07 13:31:54 by joaosilva        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	g_exit;

// Função para obter o prompt da shell
static char	*built_shell_prompt(void)
{
	char	*cwd;
    char    *prompt;

	cwd = getcwd(NULL, 0); // ??????? ONDE FOSTE TU BUSCAR OU LEMBRAR-TE DE IR BUSCAR ESTA FUNÇÃO???Obtém o diretório de trabalho atual. "get current working directory. "The getcwd function retrieves the current working directory path and stores it as a null-terminated string in the provided buffer. Its signature is typically: char *getcwd(char *buf, size_t size); buf: Pointer to a buffer where the current working directory path will be stored. size: Maximum number of characters that can be stored in the buffer. The function returns a pointer to the buffer containing the current working directory path if successful, or NULL if an error occurs.
	if (!cwd) // Verifica se ocorreu algum erro ao obter o diretório atual
    {
        perror("Error getting current working directory");
        EXIT_FAILURE; // Retorna EXIT_FAILURE para indicar que ocorreu um erro.
    }
    prompt = ft_strjoin(PROMPT_BEGIN, cwd); // Concatena o diretório atual com o início do prompt.
	free(cwd); // Libera a memória alocada para o diretório atual.
    cwd = ft_strjoin(prompt, PROMPT_END); // Concatena o resultado anterior com o final do prompt.
	free(prompt); // Libera a memória alocada para o prompt.
    if (!cwd) // Verifica se ocorreu algum erro ao concatenar as strings
    {
        perror("Error creating shell prompt");
        EXIT_FAILURE; // Retorna EXIT_FAILURE para indicar que ocorreu um erro.
    }
    return (cwd); // Retorna o prompt completo.
}

// Função para obter o comando da linha de entrada do usuário
static char *get_cmd(t_shell *shell) 
{
    shell->prompt = built_shell_prompt(); // Cria o prompt da shell
    if (!shell->prompt) // Verifica se ocorreu algum erro ao criar o prompt
    {
        perror("Error creating shell prompt");
        EXIT_FAILURE; // Retorna EXIT_FAILURE para indicar que ocorreu um erro.
    }
    shell->line = readline(shell->prompt); // Lê uma linha de entrada do usuário
    free(shell->prompt);

    if (!shell->line) 
	{
        perror("Error reading user input"); // Mostra uma mensagem de erro
		return NULL; // Se não conseguir ler a linha, retorna NULL, Não sai inesperadamente do programa caso ocorra um erro (no caso de usarmos EXIT_FAILURE ia acontecer isso.)
    }
	else 
		return (shell->line);
}

static void init_shell_variables(t_shell *shell, char **envp) 
{
    // Inicializa a estrutura t_shell
    shell = &((t_shell){0}); // Inicializa todos os campos da estrutura t_shell com 0
  
    // Converte o array de strings de variáveis de ambiente para uma lista encadeada
    envp_to_list(envp, shell);
    // Atualiza as variáveis de ambiente na estrutura t_shell
    envp_update(shell);
    //if(!shell->env) // Verifica se ocorreu um erro ao atualizar as variáveis de ambiente
    //{
    //    perror("Error updating shell environment");
    //    return; // Retorna se ocorrer um erro ao atualizar as variáveis de ambiente
    //}
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	(void)argc; // Ignora o argumento argc para evitar avisos de compilação.
	(void)argv; // Ignora o argumento argv para evitar avisos de compilação.
	init_shell_variables(&shell, envp); // Inicializa a estrutura t_shell com as variáveis de ambiente passadas.
	while ((shell.line = get_cmd(&shell))) // Enquanto a linha de entrada do usuário for obtida com sucesso.../ Loop principal do shell. // get_cmd vai criar o prompt (com os símbolos de ínicio e fim) e fica à espera que o user insira o comando. Após inserção do comando, entra no loop e vai pa função parser para fazer o parser o comando (ou seja interpretação do mesmo) e se for válido, retorna-o para a função run_cmd que o executa)
    {
        ft_printf(("getting command successful\n"));
        //if (parse_cmd(&shell, shell->cmd)) // Realiza o parsing da linha de comando e verifica se foi bem-sucedido
       // {
            //run_cmd(&shell, shell->cmd); // Executa o comando especificado na linha de comando
        //ft_free_array(&shell.line); // Libera a memória alocada para o comando
        //free(shell.prompt); // Libera a memória alocada para o prompt
        //}
    }
	clear_history(); // ?????????? Limpa o histórico de comandos. PQ TEMOS DE LIMPAR O HISTÓRICO E DESTRUIR AS ENVP?
	//envp_destroy(shell.env); // Destrói a lista encadeada contendo as variáveis de ambiente.
	if (shell.envp) // ????????? Se a cópia local das variáveis de ambiente existir... O PONTO NA SHELL.ENVP QUER DIZER O QUÊ?
		ft_free_array(shell.envp); // Libera a memória alocada para armazenar as variáveis de ambiente.
	if (isatty(STDIN_FILENO)) // Verifica se a entrada padrão é um terminal.
		ft_putendl_fd("exit", 2); // Se for um terminal, imprime "exit" na saída de erro.
        // É NECESSÁRIO FAZER TODAS AS VERIFICAÇÕES QUE SÃO FEITAS ACIMA?????
	return (g_exit); // Retorna o status de saída do shell.
}

