#include "pipex.h"

void	initialize(t_data *data, int ac, char **av, char **envp)
{
	int	i;

	data->envp = envp;
	data->path = NULL;
	data->cmd = NULL;
	get_path(data, envp);
	data->cmd = ft_calloc(ac - 2, sizeof(char *));
	if (!data->cmd)
		free_all(data, 1, NULL);
	check_file(data, av[1]);
	data->infile = av[1];
	data->outfile = av[ac - 1];
	i = 0;
	while (i < ac - 3)
	{
		data->cmd[i] = av[i + 2];
		i++;
	}
}

void	free_all(t_data *data, int error, char *message)
{
	if (data->path)
		free(data->path);
	if (data->cmd)
		free(data->cmd);
	if (error)
	{
		if (message && *message == '1')
			ft_printf("Error : %s\n", strerror(errno));
		else if (message)
			ft_printf("Error : %s\n", message);
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}

void	check_file(t_data *data, char *path)
{
	if (access(path, F_OK) == -1)
		free_all(data, 1, "1");
	else if (access(path, R_OK) == -1)
		free_all(data, 1, "1");
}

/*
** Redirects the standard output to the output file.
*/
void	redirect_output(t_data *data)
{
	int	outfile_fd;

	outfile_fd = open(data->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (outfile_fd == -1)
		free_all(data, 1, "1");
	if (dup2(outfile_fd, STDOUT_FILENO) == -1)
	{
		close(outfile_fd);
		free_all(data, 1, "1");
	}
	if (close(outfile_fd) == -1)
		perror("Failed to close the file descriptor");
}

void get_path(t_data *data, char **envp)
{
	char	**path;
	int		i;

	i = 0;
	while (envp[i] && ft_strncmp(envp[i], "SHELL=", 6) != 0)
		i++;
	if (!envp[i])
		free_all(data, 1, "PATH not found in environment\n");
	path = ft_split(envp[i] + 6, ':');
	if (!path)
		free_all(data, 1, "Failed to allocate memory\n");
	i = 0;
	while ((path[i] && access(path[i], X_OK) == -1))
		i++;
	if (!path[i])
	{
		path = free_all_split(path);
		free_all(data, 1, "No executable files found in PATH\n");
	}
	data->path = ft_strdup(path[i]);
	path = free_all_split(path);
	if (!data->path)
		free_all(data, 1, "Failed to allocate memory\n");
}
