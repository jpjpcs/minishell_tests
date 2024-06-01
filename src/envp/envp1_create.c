/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp1_create.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaosilva <joaosilva@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 11:26:00 by joaosilva         #+#    #+#             */
/*   Updated: 2024/06/01 01:43:22 by joaosilva        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
Nesta pasta iremos copiar as variáveis de ambiente (env) do sistema (da shell) 
// para o nosso programa (minishell).
Iremos criar 3 variáveis distintas consoante a utilidade que lhe queremos dar:
Resumindo, shell->env_list_unsorted (a lista ligada por ordem de criação),
	a shell->env_sorted (a lista ligada por ordem alfabética) e a 
	shell->envp_char (a char **envp).
1
	- A shell->env_list_unsorted serve para imprimir as variáveis quando 
	executamos o comando env (ou seja,
	exibi-las por ordem de criação). É criada na função 
	convert_envp_to_linked_lists 
	através da função add_node_to_env_lis (semelhante à append_node do push_swap)
2
	- A shell->env_sorted serve para imprimir as variáveis por ordem alfabética 
	quando executamos o comando export (ou seja,
	exibi-las por ordem alfabética). É criada tb na função 
	convert_envp_to_linked_lists,
	mas ordenada na função envp_sort_list.
3 - A shell->envp_char ou char **envp é o que será passado para o execve,
	para que o processo filho tenha acesso às variáveis de ambiente. É criada na 
	função convert_envp_to_char.
As restantes funções são auxiliares a estas.

Sempre que houver uma alteração nas variáveis de ambiente (env),
	as funções acima (envp_sort_list e convert_envp_to_char) 
	terão de ser chamadas 
	de forma a proceder à atualização da
...ordenação da linked list e da char **envp_char. A linked list é atualiada in 
	locu (dentro da função add, rm, ou mod), logo não temos que atualizá-la ao 
	criá-la de novo com a função convert_envp_to_linked_lists.
Assim,
	não precisamos de verificar em primeiro lugar se houve 
	mesmo alguma alteração 
	na lista. Já sabemos que houve.
E daí não termos nenhuma função chamada check_env_changed,
	ou env_update (para atualizar as variáveis), uma vez que nós
já fazemos isso dentro de cada função add, rm, ou mod.

A lista só pode ser alterada através do comando export ou unset. 
O unset remove a 
variável (e para tal criamos a função env_rm). Como tal,
	nessa função as 3 funções acima tem de ser chamadas ()
O export pode:
1 - env_add/criá-la (com a key apenas, com a key e =,
		com a key e = e valor) quando acompanhado pelo argumento key, key =,
	ou key =value, respetivamente...
2
	- env_mod/... alterá-las (quando colocamos o nome da variável (a sua key) 
	e o novo valor à frente)....
3
	- export/....ou imprimir as variáveis por ordem 
	alfabética quando não acompanhado 
	por nenhum argumento. Apresenta as vísiveis e
não visíveis. As não vísiveis apenas não aparecem quando executamos o 
*********comando env********.
No comando env as variáveis aparecem por ordem de criação.

A add_node_to_env_list é chamada na env_export e na convert_envp_to_linked_lists.
A env_mod é chamada na env_export após verificação por parte da env_get. 
A env_get é chamada na 
env_mod e na print_to_envp_tilde.
A env_rm é chamada no ms_unset (nos built-ins).
A env_export é chamada no ms_export (nos built-ins).
A convert_envp_to_linked_lists é chamada no env_export e no init_shell_variables.
A convert_envp_to_char é chamada no env_export e no init_shell_variables.
A envp_sort_list é chamada no env_export e no convert_envp_to_linked_lists.
*/

t_env *copy_list(t_env *env_list_unsorted) 
{
    if (!env_list_unsorted)
        return NULL;

    t_env *new_list = malloc(sizeof(t_env));
	new_list->key = env_list_unsorted->key;
	new_list->value = env_list_unsorted->value;
	new_list->visible = env_list_unsorted->visible;
	new_list->next = copy_list(env_list_unsorted->next);
	return new_list;
}

// Função para alocar memória para o char **envp_char.
void	**alocate_memory_to_envp_char(t_shell *shell)
{
	t_env	*tmp;
	int		i;

	i = 0;
	tmp = shell->env_list_unsorted;
	while (tmp)
	{
		if (tmp->visible)
			i++;
		tmp = tmp->next;
	}
	shell->envp_char = (char **)malloc((i + 1) * sizeof(char *));
	if (shell->envp_char == NULL)
		return (NULL);
	else
		return ((void **)shell->envp_char);
}

// Função para criar a char **envp a partir da lista 
// ligada de variáveis de ambiente.
// +2 for the '=' and '\0'
void	convert_envp_to_char(t_shell *shell)
{
	t_env	*tmp;
	int		i;
	char	*buffer;
	size_t	buffer_size;

	alocate_memory_to_envp_char(shell);
	tmp = shell->env_list_unsorted;
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

// Função para ir buscar os valores da key e do value do envp e adicionar 
//à lista ligada de variáveis de ambiente.
static char	*ft_strtok(char *str, const char *delim)
{
	static char	*buffer;
	char		*token;
	char		*ps;
	char		*s;

	if (str)
		buffer = str;
	if (!buffer)
		return (NULL);
	token = buffer;
	ps = buffer;
	while (*ps && (s = ft_strchr(delim, *ps)) == NULL)
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
// shell->env_list_unsorted Cria a linked list por ordem de criação 
// através da função add_node_to_env_list.
void	convert_envp_to_linked_lists(char **envp, t_shell *shell)
{
	int		i;
	char	*key;
	char	*value;

	i = 0;
	while (envp[i])
	{	
		printf ("\n");
		printf("Before strtok: %s\n", envp[i]);
		printf ("\n");
		key = ft_strtok(envp[i], "=");
		value = ft_strtok(NULL, "=");
		printf("Key after strtok: %s\n", key);
		printf("Value after strtok: %s\n", value);
		printf ("\n");
		printf("After strtok: %s\n", envp[i]);
		printf ("\n");
		printf ("\n");
		shell->env_list_unsorted = add_node_to_envp_list(shell, key, value, 1);
		i++;
	}
	shell->env_list_sorted = copy_list(shell->env_list_unsorted); // Crie uma cópia da lista original
	shell->env_list_sorted = env_sorted_list(shell);
}
