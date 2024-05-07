/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaosilva <joaosilva@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 17:08:59 by joaosilva         #+#    #+#             */
/*   Updated: 2024/05/07 17:42:13 by joaosilva        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int check_sintax(t_shell *shell)
{
    int size;
    char *tmp;
    tmp = shell->line;
    shell->line = ft_strtrim(shell->line, SPACES);
    size = ft_strlen(shell->line);
    free(tmp);
    if (shell->line[0] == '\0')
		return (0);
    add_history(shell->line);
    if (ft_strchr("|;&", *shell->line))
    {
        printf("Wrong sintax on the beggining.\n");
        
        return (0);
    }
    if (shell->line[size - 1]  == '|' || shell->line[size - 1]  == '&' )
    {
        printf("Wrong sintax on the end.\n");
       
        return(0);
    }
   
    return (1);
}

// int check_quotes(t_shell *shell)
// {
    
// }

int check_args(t_shell *shell)
{
    if (check_sintax(shell)) // && check_quotes(shell))
    {
        return (1);
    }
   return (0); 
}