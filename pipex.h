#include "Libft/libft.h"
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#ifndef PIPEX_H
# define PIPEX_H

# ifndef BONUS
#  define BONUS 0
# endif

typedef struct s_data
{
	char	*path;
	char	**envp;
	char	*infile;
	char	*outfile;
	char	**cmd;
	int		fd[2];
	int		pid;
}			t_data;

// pipex.c
int			open_file(t_data *data, const char *file);
void		execute_pipeline(t_data *data, int cmd_index, int num_cmds,
				int input_fd);
void		execute_child(t_data *data, int cmd_index, int num_cmds,
				int input_fd);
void		execute_parent(t_data *data, int cmd_index, int num_cmds,
				int input_fd);

// tools.c
void		initialize(t_data *data, int ac, char **av, char **envp);
void		free_all(t_data *data, int error, char *message);
void		check_file(t_data *data, char *path);
void		redirect_output(t_data *data);
void		get_path(t_data *data, char **envp);

#endif