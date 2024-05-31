/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp3_clear_modify.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaosilva <joaosilva@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 18:07:07 by joaosilva         #+#    #+#             */
/*   Updated: 2024/05/30 22:08:29 by joaosilva        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ft_envlstclear(t_env *lst, void (*del)(void *))
{
	t_env	*tmp;

	if (!lst || !del)
		return ;
	while (lst)
	{
		tmp = (lst)->next;
		ft_envlstdelone(lst, del);
		lst = tmp;
	}
}

bool	env_mod(t_shell *shell, char *target, char *new_value)
{
	t_env	*tmp;

	tmp = shell->env_list_unsorted;
		// Set a temporary pointer to the beginning of the shell's environment linked list.
	while (tmp)           
		// Iterate through each environment variable in the linked list.
	{
		if (ft_strcmp(target, tmp->key) == 0)
			// If the key of the current variable matches the target key...
		{
			free(tmp->value);                             
				// Free the memory allocated for the current value.
			tmp->value = malloc(ft_strlen(new_value) + 1);
				// Allocate memory for the new value.
			if (tmp->value)                               
				// If the memory allocation was successful...
			{
				ft_strlcpy(tmp->value, new_value, ft_strlen(new_value) + 1);
					// Copy the new value to the allocated memory.
				tmp->visible = 1;                                           
					// Set the variable as visible.
				convert_envp_to_char(shell);                                
					// Update the shell's environment copy.
				return (true);                                              
					// Return true to indicate successful modification.
			}
		}
		tmp = tmp->next;
			// Move to the next environment variable in the linked list.
	}
	return (false); // Return false if the target key is not found.
}
