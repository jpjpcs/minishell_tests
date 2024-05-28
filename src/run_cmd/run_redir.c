/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_redir.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jode-jes <jode-jes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 16:42:00 by luide-so          #+#    #+#             */
/*   Updated: 2024/05/27 15:28:27 by jode-jes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	expand_file(t_shell *shell, char **file)
{
	int	len;

	expand_arg(shell, file); // expands the envp on the string *file
	len = ft_strlen(*file); // determines the length of file after the expansion
	trim_arg(shell, *file); //trims the blank spaces on each edge of the string
	trim_quotes(*file, &len); //trims quotes and readjust the lenght
	return (1); //returns 1 if it is sucessful
}

void	run_redir(t_shell *shell, t_redir *cmd)
{
	int		fd;  // Variable for the new file descriptor after the redirection
	int		original_fd; // Variable to record the fd for the original file

	original_fd = dup(cmd->fd); // Duplicates the fd of the original files and record it on the original_fd variable, this allows to restore the descriptor of the original file later
	fd = -2; //Initializes fd with an invalid value
	if (expand_file(shell, &cmd->file))  //If the expansion is succesful open file with the especified permissions
		fd = open(cmd->file, cmd->mode, 0644);
	if (fd == -1)  //if it gets an error during the fd creation returns an error and gets back the shell prompt
		print_error(shell, cmd->file, strerror(errno), 1);
	else if (shell->status == CONTINUE) //if shell state is to continue
	{
		dup2(fd, cmd->fd);  //duplicates fd to cmd->fd redirecting the standart in/out
		close(fd); //closes the fd which is no longer necessary
		run_cmd(shell, cmd->cmd); //executes the command associated to the redirection
	}
	check(dup2(original_fd, cmd->fd), "dup2 error", 1); //Restores the original file fd replacing redir
}