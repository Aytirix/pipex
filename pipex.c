#include "pipex.h"

void	execute_pipeline(t_data *data, int cmd_index, int num_cmds,
		int input_fd)
{
	if (cmd_index < num_cmds - 1)
		if (pipe(data->fd) == -1)
			free_all_stop(data, 1, "1");
	data->pid = fork();
	if (data->pid == -1)
		free_all_stop(data, 1, "1");
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
	else
		data->infile = FILE_TEMP;
	data->outfile = av[ac - 1];
	i = -1;
	while (++i < ac - 3)
	{
		data->split = ft_split(av[i + 2], ' ');
		get_path_cmd(data, data->envp, data->split[0]);
		data->split = free_all_split(data->split);
		free(data->path);
		data->path = NULL;
		data->cmd[i] = av[i + 2];
	}
}

void	here_doc(t_data *data, int *ac, char ***av)
{
	data->limiter = av[0][2];
	*ac -= 1;
	*av += 1;
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
	data.envp = envp;
	data.path = NULL;
	data.split = NULL;
	data.cmd = NULL;
	if (ft_strncmp(av[1], "here_doc", 8) == 0)
		here_doc(&data, &ac, &av);
	printf("av[0] = %s\n", av[0]);
	initialize(&data, ac, av);
	printf("infile: %s\noutfile: %s\n", data.infile, data.outfile);
	exit(0);
	input_fd = open(data.infile, O_RDONLY);
	if (input_fd == -1)
		free_all_stop(&data, 1, "1");
	execute_pipeline(&data, 0, ac - 3, input_fd);
	close(input_fd);
	free_all_stop(&data, 0, NULL);
	return (0);
}
