#include "pipex.h"

void	free_all_stop(t_data *data, int error, char *message)
{
	int	len;

	if (data->limiter)
		unlink(FILE_TEMP);
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
			write(STDERR_FILENO, message, ft_strlen(message));
			if (message[ft_strlen(message) - 1] != '\n')
				write(STDERR_FILENO, "\n", 1);
		}
		if (error == 2)
			free(message);
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}

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
