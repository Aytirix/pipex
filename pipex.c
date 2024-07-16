#include "pipex.h"

int	open_file(t_data *data, const char *file)
{
	int	fd;

	fd = open(file, O_RDONLY);
	if (fd == -1)
	{
		perror("open_file: open");
		free_all_stop(data, 1, "1");
	}
	return (fd);
}

void	execute_pipeline(t_data *data, int cmd_index, int num_cmds,
		int input_fd)
{
	if (cmd_index < num_cmds - 1)
	{
		if (pipe(data->fd) == -1)
		{
			perror("pipe");
			free_all_stop(data, 1, "1");
		}
	}
	data->pid = fork();
	if (data->pid == -1)
	{
		perror("fork");
		free_all_stop(data, 1, "1");
	}
	if (data->pid == 0)
		execute_child(data, cmd_index, num_cmds, input_fd);
	else
		execute_parent(data, cmd_index, num_cmds, input_fd);
}

void	execute_child(t_data *data, int cmd_index, int num_cmds, int input_fd)
{
	if (input_fd != STDIN_FILENO)
	{
		if (dup2(input_fd, STDIN_FILENO) == -1)
			free_all_stop(data, 1, "1");
		close(input_fd);
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

void	execute_parent(t_data *data, int cmd_index, int num_cmds, int input_fd)
{
	if (input_fd != STDIN_FILENO)
		close(input_fd);
	if (cmd_index < num_cmds - 1)
	{
		close(data->fd[1]);
		execute_pipeline(data, cmd_index + 1, num_cmds, data->fd[0]);
		close(data->fd[0]);
	}
	waitpid(data->pid, NULL, 0);
}

int	main(int ac, char **av, char **envp)
{
	t_data	data;
	int		input_fd;

	if (((!BONUS && ac != 5) || (BONUS && ac < 5)))
	{
		ft_printf("Error: Not enough arguments. \
			Usage: infile \"command 1\" \"command 2\" output file\n");
		return (1);
	}
	initialize(&data, ac, av, envp);
	input_fd = open_file(&data, data.infile);
	if (input_fd == -1)
		free_all_stop(&data, 1, "1");
	execute_pipeline(&data, 0, ac - 3, input_fd);
	close(input_fd);
	free_all_stop(&data, 0, NULL);
	return (0);
}
