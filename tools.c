#include "pipex.h"

void	initialize(t_data *data, int ac, char **av, char **envp)
{
	int	i;

	data->envp = envp;
	data->path = NULL;
	data->split = NULL;
	data->cmd = NULL;
	data->cmd = ft_calloc(ac - 2, sizeof(char *));
	if (!data->cmd)
		free_all_stop(data, 1, NULL);
	check_file(data, av[1]);
	data->infile = av[1];
	data->outfile = av[ac - 1];
	i = 0;
	while (i < ac - 3)
	{
		data->split = ft_split(av[i + 2], ' ');
		get_path_cmd(data, envp, data->split[0]);
		data->split = free_all_split(data->split);
		data->cmd[i] = av[i + 2];
		free(data->path);
		data->path = NULL;
		i++;
	}
}

void	free_all_stop(t_data *data, int error, char *message)
{
	int	len;

	if (data->path)
		free(data->path);
	if (data->cmd)
		free(data->cmd);
	if (data->split)
		data->split = free_all_split(data->split);
	if (error)
	{
		if (message && (*message == '1' || *message == '2'))
			write(STDERR_FILENO, strerror(errno), ft_strlen(strerror(errno)));
		else if (message)
		{
			len = ft_strlen(message);
			write(STDERR_FILENO, message, len);
			if (message[len - 1] != '\n')
				write(STDERR_FILENO, "\n", 1);
		}
		if (error == 2)
			free(message);
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}

void	check_file(t_data *data, char *path)
{
	if (access(path, F_OK) == -1)
		free_all_stop(data, 1, "1");
	else if (access(path, R_OK) == -1)
		free_all_stop(data, 1, "1");
}

/*
** Redirects the standard output to the output file.
*/
void	redirect_output(t_data *data)
{
	int	outfile_fd;

	outfile_fd = open(data->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (outfile_fd == -1)
		free_all_stop(data, 1, "1");
	if (dup2(outfile_fd, STDOUT_FILENO) == -1)
	{
		close(outfile_fd);
		free_all_stop(data, 1, "1");
	}
	if (close(outfile_fd) == -1)
		perror("Failed to close the file descriptor");
}

void	get_path_cmd(t_data *data, char **envp, char *cmd)
{
	char	**path;
	int		i;

	i = 0;
	while (envp[i] && ft_strncmp(envp[i], "PATH=", 5) != 0)
		i++;
	path = ft_split(envp[i] + 5, ':');
	if (!path)
		free_all_stop(data, 1, "Failed to allocate memory\n");
	i = -1;
	while (path[++i])
	{
		data->path = ft_strjoin(path[i], "/", 0);
		data->path = ft_strjoin(data->path, cmd, 1);
		if (access(data->path, 0) == 0)
			break ;
		free(data->path);
		data->path = NULL;
	}
	path = free_all_split(path);
	if (!data->path)
		free_all_stop(data, 2, ft_strjoin("zsh: command not found: ", cmd, 0));
}
