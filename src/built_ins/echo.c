/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crocha-s <crocha-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 00:36:24 by crocha-s          #+#    #+#             */
/*   Updated: 2024/05/12 17:23:37 by crocha-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	ft_contains_only(char *str, char *characters)
{
	while (*str)
	{
		if (!ft_strchr(characters, *str++))
			return (0);
	}
	return (1);
}

static int has_flag(t_exec *cmd, int *arg_index)
{
   int	flag;

	flag = 0;
	while (cmd->argv[++*arg_index])
	{
		if (cmd->argv[*arg_index][0] == '-')
		{
			if (ft_contains_only(&cmd->argv[*arg_index][1], "n"))
				flag = 1;
			else
				break ;
		}
		else
			break ;
	}
	return (flag);
}

void ms_echo(t_exec *cmd)
{
    int i;
    int flag;
    
    i = 0;
    flag = has_flag(cmd, &i);

    if (cmd->argv[i])
    {
        ft_putstr_fd(cmd->argv[i], STDOUT_FILENO);
        while(cmd->argv[++i])
        {
            ft_putchar_fd(' ', STDOUT_FILENO);
			ft_putstr_fd(cmd->argv[i], STDOUT_FILENO);
        }
       
    }
     if (!flag)
		ft_putchar_fd('\n', STDOUT_FILENO);
    //g_exit = 0;
    
}

