/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jode-jes <jode-jes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 16:40:13 by joaosilva         #+#    #+#             */
/*   Updated: 2024/05/27 13:19:37 by jode-jes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// A função expand_line aloca memória para a nova linha. O comprimento
// será o comprimento da linha original
// mais o comprimento até onde está o espaço menos a diferença entre
// 'j' e 'i' (o comprimento da parte da linha a ser substituída)
// mais 1 para o caractere nulo de terminação.
// Em C, a contagem de índices em um array (ou equivalente,
	como uma string) começa em 0.
// Portanto, se você tem um ponteiro para o início de um array,
	o primeiro elemento
// do array é acessado com ponteiro[0].
// Quando você subtrai dois ponteiros que apontam para locais na
// mesma array (como tmp - sh->line no seu código), o resultado é a diferença
// entre os índices dos dois locais na array. Por exemplo, se tmp aponta para
// o quinto elemento de sh->line, então tmp - sh->line será 4, porque a contagem
// de índices começa em 0.

/*
//Código do Balsa
// Esta função expande a linha inserindo uma chave entre os índices 'i' e 'j'.
int	expand_line(char *key, int i, int j, char **line)
{
	char	*tmp; // Ponteiro temporário para armazenar parte da linha.
	char	*tmp2; // Ponteiro temporário para armazenar parte da linha.
	char	*tmp3; // Ponteiro temporário para armazenar parte da linha.
	tmp = ft_substr(*line, 0, i);
		// Extrai a parte da linha antes do índice 'i'. O indíce i (temp
			-sh->line) é a diferença entre a localização onde está o espaço (indíce 4 por exemplo se o espaço estiver na posição 4)
	tmp2 = ft_substr(*line, j, ft_strlen(*line) - j + 1);
		// Extrai a parte da linha após o índice 'j'.
	tmp3 = *line; // Salva o ponteiro original para a linha.
	free(tmp3); // Libera a memória da linha original.
	*line = ft_strjoin(tmp, key);
		// Concatena a parte da linha antes do índice 'i' com a chave.
	free(tmp); // Libera a memória da parte da linha antes do índice 'i'.
	tmp = *line; // Salva o ponteiro atual para a linha.
	*line = ft_strjoin(*line, tmp2);
		// Concatena a linha com a parte da linha após o índice 'j'.
	free(tmp); // Libera a memória da linha concatenada anterior.
	free(tmp2); // Libera a memória da parte da linha após o índice 'j'.
	return (1); // Retorna 1 para indicar sucesso na expansão da linha.
}
*/

/* int	expand_free(char *key, int i, int j, char **line)
{
	expand(key, i, j, line);
	free(key);
	return (1);
} */
int	expand_line(char *space, int i, int j, char **line)
{
	char *new_line; // Ponteiro para a nova linha.
	int space_len;  // Comprimento da chave.
	int line_len;   // Comprimento da linha original.
	space_len = ft_strlen(space);
	line_len = ft_strlen(*line);
	new_line = (char *)calloc(line_len + space_len - (j - i) + 1, sizeof(char));
	if (!new_line)
		return (0); // Retorna 0 se a alocação de memória falhar.
	// Copia a parte da linha antes do índice 'i' para a nova linha.
	ft_strlcpy(new_line, *line, i + 1);
	// Concatena o espaço à nova linha.
	ft_strlcat(new_line, space, line_len + space_len + 1);
	// Concatena a parte da linha após o índice 'j' à nova linha.
	ft_strlcat(new_line, *line + j, line_len + space_len + 1);
	// Liberta a memória da linha original.
	free(*line);
	// Atualiza o ponteiro para a linha para apontar para a nova linha.
	*line = new_line;
	return (1); // Retorna 1 para indicar sucesso na expansão da linha.
}

int	expand_free(char *key, int i, int j, char **line)
{
	expand_line(key, i, j, line);
	free(key);
	return (1);
}
