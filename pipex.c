#include "pipex.h"

void	execute_pipeline(t_data *data, int cmd_index, int num_cmds)
{
	if (cmd_index < num_cmds - 1)
		if (pipe(data->fd) == -1)
			free_all_stop(data, 1, "1");
	data->pid = fork();
	if (data->pid == -1)
		free_all_stop(data, 1, "1");
	if (data->pid == 0)
		execute_child(data, cmd_index, num_cmds);
	else
		execute_parent(data, cmd_index, num_cmds);
}

void	execute_child(t_data *data, int cmd_index, int num_cmds)
{
	if (data->input_fd != STDIN_FILENO)
	{
		if (dup2(data->input_fd, STDIN_FILENO) == -1)
			free_all_stop(data, 1, "1");
		close(data->input_fd);
	}
	if (cmd_index < num_cmds - 1)
	{
		if (dup2(data->fd[1], STDOUT_FILENO) == -1)
			free_all_stop(data, 1, "1");
		close(data->fd[0]);
		close(data->fd[1]);
	}
	else
		redirect_output(data);
	data->split = ft_split(data->cmd[cmd_index], ' ');
	get_path_cmd(data, data->envp, data->split[0]);
	if (execve(data->path, data->split, data->envp) == -1)
		free_all_stop(data, 1, "1");
	data->split = free_all_split(data->split);
}

void	execute_parent(t_data *data, int cmd_index, int num_cmds)
{
	if (data->input_fd != STDIN_FILENO)
		close(data->input_fd);
	if (cmd_index < num_cmds - 1)
	{
		close(data->fd[1]);
		data->input_fd = data->fd[0];
		execute_pipeline(data, cmd_index + 1, num_cmds);
		close(data->fd[0]);
	}
	waitpid(data->pid, NULL, 0);
}

static void	initialize(t_data *data, int ac, char **av)
{
	int	i;

	data->cmd = ft_calloc(ac - 2, sizeof(char *));
	if (!data->cmd)
		free_all_stop(data, 1, NULL);
	if (data->limiter == NULL)
	{
		if (access(av[1], F_OK) == -1 || access(av[1], R_OK) == -1)
			free_all_stop(data, 1, "1");
		data->infile = av[1];
	}
	data->outfile = av[ac - 1];
	i = -1;
	while (++i < ac - 3)
		data->cmd[i] = av[i + 2];
}

void	here_doc(t_data *data, int *ac, char ***av)
{
	char	*line;
	int		fd;

	data->infile = FILE_TEMP;
	data->limiter = av[0][2];
	*ac -= 1;
	*av += 1;
	fd = open(FILE_TEMP, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		free_all_stop(data, 1, "1");
	while (1)
	{
		write(1, "pipe heredoc> ", 14);
		line = get_next_line(0);
		if (!line)
			free_all_stop(data, 1, "1");
		if (ft_strncmp(line, data->limiter, ft_strlen(data->limiter)) == 0)
			if (ft_strlen(line) == 1 && ft_strlen(data->limiter) == 0)
				break ;
		write(fd, line, ft_strlen(line));
		free(line);
	}
	free(line);
	close(fd);
}

int	main(int ac, char **av, char **envp)
{
	t_data	data;

	if (((!BONUS && ac != 5) || (BONUS && ac < 5)))
	{
		ft_printf("Error: Not enough arguments. \
			Usage: infile \"command 1\" \"command 2\" output file\n");
		return (1);
	}
	data.envp = envp;
	data.path = NULL;
	data.split = NULL;
	data.limiter = NULL;
	data.cmd = NULL;
	if (BONUS && ft_strncmp(av[1], "here_doc", 8) == 0)
		here_doc(&data, &ac, &av);
	initialize(&data, ac, av);
	data.input_fd = open(data.infile, O_RDONLY);
	if (data.input_fd == -1)
		free_all_stop(&data, 1, "1");
	execute_pipeline(&data, 0, ac - 3);
	close(data.input_fd);
	free_all_stop(&data, 0, NULL);
	return (0);
}
