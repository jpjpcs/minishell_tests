/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp4_sortlist_export_get_print.c                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaosilva <joaosilva@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 14:28:33 by joaosilva         #+#    #+#             */
/*   Updated: 2024/05/31 16:12:24 by joaosilva        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/* Código do luís Balsa *
// This function prints the environment variables in the shell structure.
void	envp_print(t_shell *shell)
{
	t_env	*tmp;
	t_env	*tmp;
	t_env	*tmp;
	t_env	*tmp;
	t_env	*tmp_last;
	t_env	*tmp;

	tmp = shell->env;
		// Set a temporary pointer to the beginning of the shell's environment linked list.
	while (tmp) // Iterate through each environment variable in the linked list.
	{
		if (tmp->visible) // If the environment variable is marked as visible...
			ft_printf("%s=%s\n", tmp->key, tmp->value);
				// Print the key-value pair.
		tmp = tmp->next;
			// Move to the next environment variable in the linked list.
	}
} */
// This function retrieves the value of an environment variable given its key.
/* char	*env_get(char *key, t_shell *shell)
{
	tmp = shell->env;
		// Set a temporary pointer to the beginning of the shell's environment linked list.
	while (tmp) // Iterate through each environment variable in the linked list.
	{
		if (!ft_strcmp(tmp->key, key))
			// If the key of the current environment variable matches the target key...
			return (tmp->value);
				// Return the value of the environment variable.
		tmp = tmp->next;
			// Move to the next environment variable in the linked list.
	}
	return (NULL); // Return NULL if the key is not found.
} */
/* // This function modifies the value of an existing environment variable.
bool	env_mod(t_shell *shell, char *target, char *new_value)
{
	tmp = shell->env;
		// Set a temporary pointer to the beginning of the shell's environment linked list.
	while (tmp) // Iterate through each environment variable in the linked list.
	{
		if (ft_strcmp(target, tmp->key) == 0)
			// If the key of the current variable matches the target key...
		{
			free(tmp->value);
				// Free the memory allocated for the current value.
			tmp->value = ft_strdup(new_value);
				// Duplicate and store the new value.
			tmp->visible = 1; // Set the variable as visible.
			envp_update(shell); // Update the shell's environment copy.
			return (true); // Return true to indicate successful modification.
		}
		tmp = tmp->next;
			// Move to the next environment variable in the linked list.
	}
	return (false); // Return false if the target key is not found.
} */
// This function exports a new environment variable or modifies an existing one.
/* void	env_export(t_shell *shell, char *key, char *value, int visible)
{
	if (env_get(key, shell) && visible)
		// If the key exists and is marked as visible...
		env_mod(shell, key, value);
			// Modify the value of the existing variable.
	else if (!env_get(key, shell)) // If the key does not exist...
		env_add(shell, key, value, visible); // Add a new variable.
} */
/*
// This function removes an environment variable from the shell's environment linked list.
bool	env_rm(char *key, t_shell *shell)
{
	tmp = shell->env;
		// Set a temporary pointer to the beginning of the shell's environment linked list.
	tmp_last = tmp; // Initialize a pointer to the last environment variable.
	while (tmp) // Iterate through each environment variable in the linked list.
	{
		if (!ft_strcmp(tmp->key, key))
			// If the key of the current variable matches the target key...
		{
			tmp_last->next = tmp->next;
				// Update the next pointer of the previous variable to skip the current one.
			if (tmp == shell->env)
				// If the current variable is the first one in the linked list...
				shell->env = tmp->next;
					// Update the beginning of the linked list.
			free(tmp->key); // Free the memory allocated for the key.
			if (tmp->value) // If the value exists...
				free(tmp->value); // Free the memory allocated for the value.
			free(tmp); // Free the memory allocated for the current variable.
			shell->envp_size--; // Decrement the count of environment variables.
			envp_sort(shell);
				// Sort the environment variables alphabetically by key.
			envp_update(shell); // Update the shell's environment copy.
			return (true); // Return true to indicate successful removal.
		}
		tmp_last = tmp; // Update the pointer to the last variable.
		tmp = tmp->next;
			// Move to the next environment variable in the linked list.
	}
	return (false); // Return false if the target key is not found.
} */
// This function prints the environment variables in the shell structure.
void	envp_print(t_shell *shell)
{
	t_env *tmp;
	
	tmp = shell->env_list_unsorted;
		// Set a temporary pointer to the beginning of the shell's environment linked list.
	while (tmp)           
		// Iterate through each environment variable in the linked list.
	{
		if (tmp->visible)                              
			// If the environment variable is marked as visible...
			ft_printf("%s=%s\n", tmp->key, tmp->value);
				// Print the key-value pair.
		tmp = tmp->next;                               
			// Move to the next environment variable in the linked list.
	}
}

// This function retrieves the value of an environment variable given its key.
char	*env_get_value(char *key, t_shell *shell)
{
	t_env	*tmp;

	tmp = shell->env_list_unsorted;
		// Set a temporary pointer to the beginning of the shell's environment linked list.
	while (tmp)           
		// Iterate through each environment variable in the linked list.
	{
		if (!ft_strcmp(tmp->key, key))
			// If the key of the current environment variable matches the target key...
			return (tmp->value);      
				// Return the value of the environment variable.
		tmp = tmp->next;              
			// Move to the next environment variable in the linked list.
	}
	return (NULL); // Return NULL if the key is not found.
}

void	env_export(t_shell *shell, char *key, char *value, int visible)
{
	if (!env_get_value(key, shell))                       
		// If the key does not exist...
		add_node_to_envp_list(shell, key, value, visible);
			// Add a new variable.
	else if (env_get_value(key, shell) && visible)        
		// If the key exists and is marked as visible...
		env_mod(shell, key, value);                       
			// Modify the value of the existing variable.
}

// Função para ordenar a lista ligada de variáveis de ambiente por ordem alfabética.

/* t_env	*env_sorted_list(t_shell *shell)
{
	t_env	*tmp_lst = NULL;
	t_env	*dummy = NULL;
	
	if (!shell->env_list_sorted)
		return (NULL);
	tmp_lst = shell->env_list_sorted;
	dummy = shell->env_list_sorted;
	
	while (shell->env_list_sorted && shell->env_list_sorted->next)
	{
		if (ft_strcmp(shell->env_list_sorted->key, shell->env_list_sorted->next->key) > 0)
		{
			if (shell->env_list_sorted == tmp_lst)
			{
				tmp_lst = shell->env_list_sorted->next;
				shell->env_list_sorted->next = tmp_lst->next;
				tmp_lst->next = shell->env_list_sorted;
				dummy = tmp_lst;
			}
			else
			{
				shell->env_list_sorted = shell->env_list_sorted->next;
				dummy->next->next = shell->env_list_sorted->next;
				shell->env_list_sorted->next = dummy->next;
				dummy->next = shell->env_list_sorted;
				shell->env_list_sorted = tmp_lst;
			}  
		}
		else
		{
			dummy = shell->env_list_sorted;
			shell->env_list_sorted = shell->env_list_sorted->next;
		}
	}
	return (tmp_lst);
} */

t_env *swap_nodes(t_env *dummy, t_env **env_list_sorted, t_env **tmp_lst)
{
    if (*env_list_sorted == *tmp_lst)
    {
        *tmp_lst = (*env_list_sorted)->next;
        (*env_list_sorted)->next = (*tmp_lst)->next;
        (*tmp_lst)->next = *env_list_sorted;
        dummy = *tmp_lst;
    }
    else
    {
        *env_list_sorted = (*env_list_sorted)->next;
        dummy->next->next = (*env_list_sorted)->next;
        (*env_list_sorted)->next = dummy->next;
        dummy->next = *env_list_sorted;
        *env_list_sorted = *tmp_lst;
    }
    return dummy;
}

t_env *env_sorted_list(t_shell *shell)
{
    t_env *tmp_lst = NULL;
    t_env *dummy = NULL;

    if (!shell->env_list_sorted)
        return (NULL);
    tmp_lst = shell->env_list_sorted;
    dummy = shell->env_list_sorted;

    while (shell->env_list_sorted && shell->env_list_sorted->next)
    {
        if (ft_strcmp(shell->env_list_sorted->key, shell->env_list_sorted->next->key) > 0)
        {
            dummy = swap_nodes(dummy, &(shell->env_list_sorted), &tmp_lst);
        }
        else
        {
            dummy = shell->env_list_sorted;
            shell->env_list_sorted = shell->env_list_sorted->next;
        }
    }
    return (tmp_lst);
}