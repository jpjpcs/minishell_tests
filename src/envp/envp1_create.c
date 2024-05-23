/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp1_create.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaosilva <joaosilva@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 11:26:00 by joaosilva         #+#    #+#             */
/*   Updated: 2024/05/22 10:42:52 by joaosilva        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/* 
Nesta pasta iremos copiar as variáveis de ambiente (env) do sistema (da shell) para o nosso programa (minishell). 
Iremos criar 3 variáveis distintas consoante a utilidade que lhe queremos dar:
Resumindo, shell->env_list (a lista ligada por ordem de criação), a shell->env_sorted (a lista ligada por ordem alfabética) e a shell->envp_char (a char **envp).
1 - A shell->env_list serve para imprimir as variáveis quando executamos o comando env (ou seja, exibi-las por ordem de criação). É criada na função convert_envp_to_linked_lists através da função add_node_to_env_lis (semelhante à append_node do push_swap)
2 - A shell->env_sorted serve para imprimir as variáveis por ordem alfabética quando executamos o comando export (ou seja, exibi-las por ordem alfabética). É criada tb na função convert_envp_to_linked_lists, mas ordenada na função envp_sort_list.
3 - A shell->envp_char ou char **envp é o que será passado para o execve, para que o processo filho tenha acesso às variáveis de ambiente. É criada na função convert_envp_to_char.
As restantes funções são auxiliares a estas.

Sempre que houver uma alteração nas variáveis de ambiente (env), as funções acima (envp_sort_list e convert_envp_to_char) terão de ser chamadas de forma a proceder à atualização da 
...ordenação da linked list e da char **envp_char. A linked list é atualiada in locu (dentro da função add, rm, ou mod), logo não temos que atualizá-la ao criá-la de novo com a 
...função convert_envp_to_linked_lists.
Assim, não precisamos de verificar em primeiro lugar se houve mesmo alguma alteração na lista. Já sabemos que houve. 
E daí não termos nenhuma função chamada check_env_changed, ou env_update (para atualizar as variáveis), uma vez que nós 
já fazemos isso dentro de cada função add, rm, ou mod.

A lista só pode ser alterada através do comando export ou unset. O unset remove a variável (e para tal criamos a função env_rm). Como tal, nessa função as 3 funções acima tem de ser chamadas ()
O export pode:
1 - env_add/criá-la (com a key apenas, com a key e =, com a key e = e valor) quando acompanhado pelo argumento key, key =, ou key =value, respetivamente...
2 - env_mod/... alterá-las (quando colocamos o nome da variável (a sua key) e o novo valor à frente)....
3 - export/....ou imprimir as variáveis por ordem alfabética quando não acompanhado por nenhum argumento. Apresenta as vísiveis e 
não visíveis. As não vísiveis apenas não aparecem quando executamos o *********comando env********. 
No comando env as variáveis aparecem por ordem de criação. 

A add_node_to_env_list é chamada na env_export e na convert_envp_to_linked_lists.
A env_mod é chamada na env_export após verificação por parte da env_get. A env_get é chamada na env_mod e na print_to_envp_tilde.
A env_rm é chamada no ms_unset (nos built-ins).
A env_export é chamada no ms_export (nos built-ins).
A convert_envp_to_linked_lists é chamada no env_export e no init_shell_variables. 
A convert_envp_to_char é chamada no env_export e no init_shell_variables.
A envp_sort_list é chamada no env_export e no convert_envp_to_linked_lists.
*/

/* Código do Luís Balsa *

// This function updates the shell's copy of the environment variables.
void	envp_update(t_shell *shell)
{
	t_env	*tmp;
	char	*env;
	int		i;

	if (shell->envp) // If the shell's environment copy exists...
		ft_free_array(shell->envp); // Free the memory allocated for it.
	if (!shell->env) // If the shell's environment linked list is empty...
	{
		shell->envp = NULL; // Set the shell's environment copy pointer to NULL.
		return ; // Exit the function.
	}
	shell->envp = ft_calloc(shell->envp_size + 1, sizeof(char *)); // Allocate memory for the shell's environment copy.
	tmp = shell->env; // Set a temporary pointer to the beginning of the shell's environment linked list.
	i = 0; // Initialize an index counter.
	while (tmp) // Iterate through each environment variable in the shell's linked list.
	{
		if (tmp->visible) // If the environment variable is marked as visible...
		{
			env = ft_strjoin(tmp->key, "="); // Concatenate the key and "=".
			shell->envp[i++] = ft_strjoin(env, tmp->value); // Concatenate the key-value pair and store it in the shell's environment copy.
			free(env); // Free the memory allocated for the concatenated string.
		}
		tmp = tmp->next; // Move to the next environment variable in the linked list.
	}
}

// This function resets the index of each environment variable in the shell structure to 0.
static void	zero_index(t_shell *shell)
{
	t_env	*tmp;

	tmp = shell->env;
	while (tmp)
	{
		tmp->index = 0; // Resets the index of the current environment variable to 0.
		tmp = tmp->next; // Moves to the next environment variable.
	}
}

// This function sorts the environment variables in the shell structure alphabetically by key.
void	envp_sort(t_shell *shell)
{
	t_env	*tmp;
	t_env	*max;
	char	*max_key;
	int		i;

	zero_index(shell); // Reset the index of each environment variable.
	i = shell->envp_size; // Initialize a counter with the size of the environment list.
	while (i--) // Iterate from the end of the list to the beginning.
	{
		tmp = shell->env; // Set a temporary pointer to the beginning of the shell's environment linked list.
		max = NULL; // Initialize the variable to store the maximum environment variable found so far.
		max_key = ""; // Initialize the variable to store the key of the maximum environment variable found so far.
		while (tmp) // Iterate through each environment variable in the linked list.
		{
			if (ft_strcmp(tmp->key, max_key) > 0 && tmp->index == 0) // If the current key is greater than the maximum key found so far and the variable is not indexed...
			{
				max = tmp; // Update the maximum environment variable found so far.
				max_key = tmp->key; // Update the key of the maximum environment variable found so far.
			}
			tmp = tmp->next; // Move to the next environment variable in the linked list.
		}
		if (max) // If a maximum environment variable was found...
			max->index = i + 1; // Set its index to the current iteration count.
	}
}

// This function adds a new environment variable to the shell's environment linked list.
t_env	*env_add(t_shell *shell, char *key, char *value, int visible)
{
	t_env	*new;
	t_env	*tmp;

	new = malloc(sizeof(t_env)); // Allocate memory for the new environment variable.
	if (!new) // If memory allocation fails...
		return (NULL); // Return NULL.
	shell->envp_size++; // Increment the count of environment variables.
	new->key = ft_strdup(key); // Duplicate and store the key.
	new->value = ft_strdup(value); // Duplicate and store the value.
	new->index = 0; // Set the index of the new variable to 0.
	new->visible = visible; // Set the visibility flag of the new variable.
	new->next = NULL; // Set the next pointer of the new variable to NULL.
	if (!shell->env) // If the shell's environment linked list is empty...
		return (new); // Return the new variable.
	tmp = shell->env; // Set a temporary pointer to the beginning of the shell's environment linked list.
	while (tmp->next) // Iterate through each environment variable in the linked list until the last one is reached.
		tmp = tmp->next; // Move to the next environment variable.
	tmp->next = new; // Append the new variable to the end of the linked list.
	envp_sort(shell); // Sort the environment variables alphabetically by key.
	envp_update(shell); // Update the shell's environment copy.
	return (shell->env); // Return a pointer to the beginning of the linked list.
}
// This function converts the environment variables from the system's format to the shell's linked list format.
void	envp_to_list(char **envp, t_shell *shell)
{
	int		i;
	char	**split;
	char	*value;

	shell->env = NULL; // ???? Initialize the shell's environment linked list. SUPOSTAMENTE JÁ NAO PRECISO FAZER ISTO, UMA VEZ QUE JÁ INICIALIZEI A LISTA TODA NO INIT_SHELL
	i = 0; // Initialize an index counter.
	while (envp[i]) // Iterate through each environment variable in the system's format.
	{
		split = ft_split(envp[i], '='); // ??????? Split the environment variable into key and value. PQ É QUE TEMOS DE FAZER UM SPLIT? NÃO PODÍAMOS IR BUSCAR AS VÁRIÁVEIS COM O KEY, O IGUAL E O VALUE (ASSIM: KEY=VALUE), COPIANDO TUDO NA INTEGRA?
		if (ft_strchr(envp[i], '=')) // If the environment variable contains '='...
			value = ft_strdup(ft_strchr(envp[i], '=') + 1); // Extract the value part.
		else
			value = NULL; // If no '=' is found, set the value to NULL.
		if (split && split[0]) // ?????? If splitting was successful and a key exists...E SE A KEY NÃO EXISTIR? OU EXISTE SEMPRE. NO MAC EXISTEM SEMPRE, QUER A KEY, QUER O VALUE.
			shell->env = env_add(shell, split[0], value, 1); // Add the key-value pair to the shell's environment linked list.
		ft_free_array(split); // Free the memory allocated for the split array.
		if (value) // ?????????? If value exists... ENTÃO VALUE NÃO EXISTE SEMPRE, MESMO QUE SEJA NULL? OU SE FOR NULL CONSIDERA-SE QUE NÃO EXISTE?
			free (value); // Free the memory allocated for value.
		i++; // Move to the next environment variable.
	}
	envp_sort(shell); // ????????? Sort the environment variables alphabetically by key. PQ É QUE TENS DE ORDENAR POR ORDEM ALFABÉTICA? É PARA APLICAR O EXPORT?
} */


// Função para alocar memória para o char **envp_char.
void **alocate_memory_to_envp_char(t_shell *shell)
{
	t_env *tmp;
	int i;

	i = 0;
	tmp = shell->env_list;
	while (tmp)
	{
		if (tmp->visible)
			i++;
		tmp = tmp->next;
	}
	shell->envp_char = (char **)malloc((i + 1) * sizeof(char *));
	if (shell->envp_char == NULL)
		return NULL;
	else
		return (void**)shell->envp_char;
}

// Função para criar a char **envp a partir da lista ligada de variáveis de ambiente.
// +2 for the '=' and '\0'
void convert_envp_to_char(t_shell *shell)
{
	t_env *tmp;
	int i;
	char *buffer;
	size_t buffer_size;
	
	alocate_memory_to_envp_char (shell);
	tmp = shell->env_list;
	i = 0;
	while (tmp)
	{
		if (tmp->visible)
		{
			buffer_size = ft_strlen(tmp->key) + ft_strlen(tmp->value) + 2; 
			buffer = (char *)malloc(buffer_size);
			if (buffer == NULL)
				return ;
			ft_strlcpy(buffer, tmp->key, buffer_size);
			ft_strlcat(buffer, "=", buffer_size);
			ft_strlcat(buffer, tmp->value, buffer_size);
			shell->envp_char[i++] = buffer;
		}
		tmp = tmp->next;
	}
	shell->envp_char[i] = NULL;
}

// Função para ir buscar os valores da key e do value do envp e adicionar à lista ligada de variáveis de ambiente.
static char *ft_strtok(char *str, const char *delim)
{
	static char *buffer;
	char *token;
	char *ps;
	char *s;
	
	token = buffer;
	ps = buffer;
	
	if (str != NULL)
		buffer = str;
	if (buffer == NULL)
		return NULL;
	
	while(*ps && (s = ft_strchr(delim, *ps)) == NULL)
		ps++;
	
	if (*ps)
	{
		*ps = 0;
		buffer = ps + 1;
	}
	else
		buffer = NULL;
	
	return (token);
}

// Função para converter o envp em uma lista ligada de variáveis de ambiente.
void convert_envp_to_linked_lists(char **envp, t_shell *shell)
{
	int i;
	char *key;
	char *value;

	i = 0;
	while(envp[i])
	{
		key = ft_strtok (envp[i], "=");
		if (!key)
		{
			perror("Error getting key envp");
			return ;
		}
		value = NULL;
		if (key && ft_strchr(envp[i], '='))
			value = ft_strtok(NULL, "=");
		shell->env_list = add_node_to_envp_list(shell, key, value, 1); // Cria a linked list por ordem de criação.
		i++;
	}
	shell->env_list_sorted = envp_to_sort_list(shell); // Cria a linked list por ordem alfabética.
}

void convert_envp(char **envp, t_shell *shell)
{
	convert_envp_to_linked_lists(envp, shell); // para usar nos comando env (ordem criação) e export (ordem alfabética).
	convert_envp_to_char(shell); // char **envp_to_char é o que será passado para o execve, para que o processo filho tenha acesso às variáveis de ambiente.
}









 










