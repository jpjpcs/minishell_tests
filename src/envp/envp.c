/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaosilva <joaosilva@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 11:26:00 by joaosilva         #+#    #+#             */
/*   Updated: 2024/04/30 14:18:00 by joaosilva        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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

// This function prints the environment variables in the shell structure.
void	envp_print(t_shell *shell)
{
	t_env	*tmp;

	tmp = shell->env; // Set a temporary pointer to the beginning of the shell's environment linked list.
	while (tmp) // Iterate through each environment variable in the linked list.
	{
		if (tmp->visible) // If the environment variable is marked as visible...
			ft_printf("%s=%s\n", tmp->key, tmp->value); // Print the key-value pair.
		tmp = tmp->next; // Move to the next environment variable in the linked list.
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
}
