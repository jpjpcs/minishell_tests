/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaosilva <joaosilva@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 16:40:13 by joaosilva         #+#    #+#             */
/*   Updated: 2024/04/30 16:41:21 by joaosilva        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Esta função expande a linha inserindo uma chave entre os índices 'i' e 'j'.
int	expand(char *key, int i, int j, char **line)
{
	char	*tmp; // Ponteiro temporário para armazenar parte da linha.
	char	*tmp2; // Ponteiro temporário para armazenar parte da linha.
	char	*tmp3; // Ponteiro temporário para armazenar parte da linha.

	tmp = ft_substr(*line, 0, i); // Extrai a parte da linha antes do índice 'i'.
	tmp2 = ft_substr(*line, j, ft_strlen(*line) - j + 1); // Extrai a parte da linha após o índice 'j'.
	tmp3 = *line; // Salva o ponteiro original para a linha.
	free(tmp3); // Libera a memória da linha original.
	*line = ft_strjoin(tmp, key); // Concatena a parte da linha antes do índice 'i' com a chave.
	free(tmp); // Libera a memória da parte da linha antes do índice 'i'.
	tmp = *line; // Salva o ponteiro atual para a linha.
	*line = ft_strjoin(*line, tmp2); // Concatena a linha com a parte da linha após o índice 'j'.
	free(tmp); // Libera a memória da linha concatenada anterior.
	free(tmp2); // Libera a memória da parte da linha após o índice 'j'.
	return (1); // Retorna 1 para indicar sucesso na expansão da linha.
}

int	expand_free(char *key, int i, int j, char **line)
{
	expand(key, i, j, line);
	free(key);
	return (1);
}
