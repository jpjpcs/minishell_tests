/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp2_add_rm.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaosilva <joaosilva@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 02:29:21 by joaosilva         #+#    #+#             */
/*   Updated: 2024/06/04 03:59:47 by joaosilva        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ft_envlstdelone(t_env *lst, void (*del)(void *))
{
	if (!lst || !del)
		return ;
	if (lst)
	{
		del(lst->key);
		if (lst->value)
			del(lst->value);
		del(lst);
		lst = NULL;
	}
}

bool	env_rm(char *key, t_shell *shell)
{
	t_env	*tmp;
	t_env	*tmp_last;

	tmp = shell->env_list_unsorted;
	tmp_last = NULL;
	while (tmp)
	{
		if (!ft_strcmp(tmp->key, key))
		{
			if (tmp_last)
				tmp_last->next = tmp->next;
			if (tmp == shell->env_list_unsorted)
				shell->env_list_unsorted = tmp->next;
			ft_envlstdelone(tmp, free);
			shell->envp_size--;
			//env_sorted_list(shell);
			shell->env_list_sorted = copy_list(shell->env_list_unsorted); // Crie uma cópia da lista original
			shell->env_list_sorted = env_sorted_list(shell);
			convert_envp_to_char(shell);
			return (true);
		}
		tmp_last = tmp;
		tmp = tmp->next;
	}
	return (false);
}

bool	env_lstadd_back(t_env **lst, t_env *new)
{
	t_env	*temp;

	if (!lst || !new)
		return (false);
	if (!*lst)
	{
		*lst = new;
		return (true);
	}
	temp = *lst;
	while (temp->next)
		temp = temp->next;
	temp->next = new;
	return (true);
}

// Função para criar um nó. É semelhante à ft_lstnew,
//mas além do valor/conteúdo, tb tem o visivel,
t_env	*env_lstnew(char *key, char *value, int visible)
{
	t_env	*new;

	new = (t_env *)malloc(sizeof(t_env));
	if (!new)
		return (NULL);
	new->key = ft_strdup(key);
	new->value = ft_strdup(value);
	new->visible = visible;
	new->next = NULL;
	return (new);
}

// Função semelhante à lstadd_back,
//	para adicionar um novo nó ao final da lista 
// ligada de variáveis de ambiente.
t_env	*add_node_to_envp_list(t_shell *shell, char *key, char *value,
		int visible)
{
	t_env	*new_node;

	new_node = env_lstnew(key, value, visible);
	if (!new_node)
		return (NULL);
	if (env_lstadd_back(&shell->env_list_unsorted, new_node))
		shell->envp_size++;
	shell->env_list_sorted = copy_list(shell->env_list_unsorted); // Crie uma cópia da lista original
	shell->env_list_sorted = env_sorted_list(shell);
	//env_sorted_list(shell);
	convert_envp_to_char(shell);
	//free(new_node);
	return (shell->env_list_unsorted);
}
