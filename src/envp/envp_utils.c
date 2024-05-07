/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaosilva <joaosilva@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 14:28:33 by joaosilva         #+#    #+#             */
/*   Updated: 2024/04/29 14:28:35 by joaosilva        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// This function retrieves the value of an environment variable given its key.
char	*env_get(char *key, t_shell *shell)
{
	t_env	*tmp;

	tmp = shell->env; // Set a temporary pointer to the beginning of the shell's environment linked list.
	while (tmp) // Iterate through each environment variable in the linked list.
	{
		if (!ft_strcmp(tmp->key, key)) // If the key of the current environment variable matches the target key...
			return (tmp->value); // Return the value of the environment variable.
		tmp = tmp->next; // Move to the next environment variable in the linked list.
	}
	return (NULL); // Return NULL if the key is not found.
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

// This function modifies the value of an existing environment variable.
bool	env_mod(t_shell *shell, char *target, char *new_value)
{
	t_env	*tmp;

	tmp = shell->env; // Set a temporary pointer to the beginning of the shell's environment linked list.
	while (tmp) // Iterate through each environment variable in the linked list.
	{
		if (ft_strcmp(target, tmp->key) == 0) // If the key of the current variable matches the target key...
		{
			free(tmp->value); // Free the memory allocated for the current value.
			tmp->value = ft_strdup(new_value); // Duplicate and store the new value.
			tmp->visible = 1; // Set the variable as visible.
			envp_update(shell); // Update the shell's environment copy.
			return (true); // Return true to indicate successful modification.
		}
		tmp = tmp->next; // Move to the next environment variable in the linked list.
	}
	return (false); // Return false if the target key is not found.
}

// This function exports a new environment variable or modifies an existing one.
void	env_export(t_shell *shell, char *key, char *value, int visible)
{
	if (env_get(key, shell) && visible) // If the key exists and is marked as visible...
		env_mod(shell, key, value); // Modify the value of the existing variable.
	else if (!env_get(key, shell)) // If the key does not exist...
		env_add(shell, key, value, visible); // Add a new variable.
}

// This function removes an environment variable from the shell's environment linked list.
bool	env_rm(char *key, t_shell *shell)
{
	t_env	*tmp;
	t_env	*tmp_last;

	tmp = shell->env; // Set a temporary pointer to the beginning of the shell's environment linked list.
	tmp_last = tmp; // Initialize a pointer to the last environment variable.
	while (tmp) // Iterate through each environment variable in the linked list.
	{
		if (!ft_strcmp(tmp->key, key)) // If the key of the current variable matches the target key...
		{
			tmp_last->next = tmp->next; // Update the next pointer of the previous variable to skip the current one.
			if (tmp == shell->env) // If the current variable is the first one in the linked list...
				shell->env = tmp->next; // Update the beginning of the linked list.
			free(tmp->key); // Free the memory allocated for the key.
			if (tmp->value) // If the value exists...
				free(tmp->value); // Free the memory allocated for the value.
			free(tmp); // Free the memory allocated for the current variable.
			shell->envp_size--; // Decrement the count of environment variables.
			envp_sort(shell); // Sort the environment variables alphabetically by key.
			envp_update(shell); // Update the shell's environment copy.
			return (true); // Return true to indicate successful removal.
		}
		tmp_last = tmp; // Update the pointer to the last variable.
		tmp = tmp->next; // Move to the next environment variable in the linked list.
	}
	return (false); // Return false if the target key is not found.
}
