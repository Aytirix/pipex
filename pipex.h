/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thmouty <theo@student.42.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 14:55:54 by thmouty           #+#    #+#             */
/*   Updated: 2024/07/17 14:56:39 by thmouty          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "Libft/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

# ifndef BONUS
#  define BONUS 0
# endif

// STRING
# define FILE_TEMP ".here_doc_tmp"

typedef struct s_data
{
	char	*path;
	char	**envp;
	char	**split;
	char	*limiter;
	char	*infile;
	char	*outfile;
	char	**cmd;
	int		fd[2];
	int		input_fd;
	int		pid;
	int		error;
}			t_data;

// pipex.c
void		execute_pipeline(t_data *data, int cmd_index, int num_cmds);
void		execute_child(t_data *data, int cmd_index, int num_cmds);
void		execute_parent(t_data *data, int cmd_index, int num_cmds);

// tools.c
void		free_all_stop(t_data *data, int is_malloc, int error,
				char *message);
void		redirect_output(t_data *data);
void		get_path_cmd(t_data *data, char **envp, char *cmd);
void		create_path(t_data *data, char *cmd, char *path);
void		here_doc(t_data *data, int *ac, char ***av);

#endif