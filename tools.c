/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thmouty <theo@student.42.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 14:55:54 by thmouty           #+#    #+#             */
/*   Updated: 2024/07/17 14:56:39 by thmouty          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	free_all_stop(t_data *data, int is_malloc, int error, char *message)
{
	if (data->limiter)
		unlink(FILE_TEMP);
	if (data->path)
		free(data->path);
	if (data->cmd)
		free(data->cmd);
	if (data->pid)
		free(data->pid);
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
		if (is_malloc)
			free(message);
		exit(error);
	}
	exit(EXIT_SUCCESS);
}

void	redirect_output(t_data *data)
{
	int	outfile_fd;

	if (data->limiter)
		outfile_fd = open(data->outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		outfile_fd = open(data->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (outfile_fd == -1)
		free_all_stop(data, 0, 1, "1");
	if (dup2(outfile_fd, STDOUT_FILENO) == -1)
	{
		close(outfile_fd);
		free_all_stop(data, 0, 1, "1");
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
		free_all_stop(data, 0, 1, "Failed to allocate memory\n");
	i = -1;
	while (path[++i])
	{
		create_path(data, cmd, path[i]);
		if (access(data->path, 0) == 0)
			break ;
		free(data->path);
		data->path = NULL;
	}
	path = free_all_split(path);
	if (!data->path)
		free_all_stop(data, 1, 127, ft_strjoin("zsh: command not found: ", cmd,
				0));
}

void	create_path(t_data *data, char *cmd, char *path)
{
	if (ft_strncmp(cmd, "/", 1) == 0 || ft_strncmp(cmd, "./", 2) == 0
		|| ft_strncmp(cmd, "../", 3) == 0)
		data->path = ft_strdup(cmd);
	else
	{
		data->path = ft_strjoin(path, "/", 0);
		data->path = ft_strjoin(data->path, cmd, 1);
	}
}

void	here_doc(t_data *data, int *ac, char ***av)
{
	char	*line;

	data->limiter = av[0][2];
	data->infile = FILE_TEMP;
	*ac -= 1;
	*av += 1;
	data->input_fd = open(FILE_TEMP, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (data->input_fd == -1)
		free_all_stop(data, 0, 1, "1");
	while (1)
	{
		write(1, "pipe heredoc> ", 14);
		line = get_next_line(0);
		if (!line)
			free_all_stop(data, 0, 1, "1");
		if (ft_strncmp(line, data->limiter, ft_strlen(data->limiter)) == 0)
			if (ft_strlen(line) - 1 == ft_strlen(data->limiter))
				break ;
		write(data->input_fd, line, ft_strlen(line));
		free(line);
	}
	free(line);
	close(data->input_fd);
	data->input_fd = open(FILE_TEMP, O_RDONLY);
}
