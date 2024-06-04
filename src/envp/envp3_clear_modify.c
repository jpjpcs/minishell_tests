/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp3_clear_modify.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaosilva <joaosilva@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 18:07:07 by joaosilva         #+#    #+#             */
/*   Updated: 2024/06/04 04:02:23 by joaosilva        ###   ########.fr       */
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
	{
		if (ft_strcmp(target, tmp->key) == 0)
		{
			free(tmp->value);
			tmp->value = ft_strdup(new_value);
			tmp->visible = 1;
			shell->env_list_sorted = copy_list(shell->env_list_unsorted);
			return (true);
		}
		tmp = tmp->next;
	}
	return (false);
}
