/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thmouty <theo@student.42.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 14:55:54 by thmouty           #+#    #+#             */
/*   Updated: 2024/07/17 14:56:39 by thmouty          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	execute_pipeline(t_data *data, int cmd_index, int num_cmds)
{
	if (cmd_index < num_cmds - 1)
		if (pipe(data->fd) == -1)
			free_all_stop(data, 0, 1, "1");
	data->pid[cmd_index] = fork();
	if (data->pid[cmd_index] == -1)
		free_all_stop(data, 0, 1, "1");
	if (data->pid[cmd_index] == 0 && data->input_fd != -5)
		execute_child(data, cmd_index, num_cmds);
	else
		execute_parent(data, cmd_index, num_cmds);
}

void	execute_child(t_data *data, int cmd_index, int num_cmds)
{
	if (data->input_fd != STDIN_FILENO)
	{
		if (dup2(data->input_fd, STDIN_FILENO) == -1)
			free_all_stop(data, 0, 1, "1");
		close(data->input_fd);
	}
	if (cmd_index < num_cmds - 1)
	{
		if (dup2(data->fd[1], STDOUT_FILENO) == -1)
			free_all_stop(data, 0, 1, "1");
		close(data->fd[0]);
		close(data->fd[1]);
	}
	else
		redirect_output(data);
	data->split = ft_split(data->cmd[cmd_index], ' ');
	get_path_cmd(data, data->envp, data->split[0]);
	if (execve(data->path, data->split, data->envp) == -1)
		free_all_stop(data, 0, 1, "1");
	data->split = free_all_split(data->split);
}

void	execute_parent(t_data *data, int cmd_index, int num_cmds)
{
	int	i;

	if (data->input_fd != -5 && data->input_fd != STDIN_FILENO)
		close(data->input_fd);
	if (cmd_index < num_cmds - 1)
	{
		close(data->fd[1]);
		data->input_fd = data->fd[0];
		execute_pipeline(data, cmd_index + 1, num_cmds);
		close(data->fd[0]);
	}
	i = 0;
	while (i <= cmd_index)
		waitpid(data->pid[i++], &data->error, 0);
}

static void	initialize(t_data *data, int *ac, char ***av)
{
	int	i;

	data->cmd = ft_calloc(*ac - 2, sizeof(char *));
	data->pid = ft_calloc(*ac - 2, sizeof(char *));
	if (!data->cmd)
		free_all_stop(data, 0, 1, NULL);
	if (data->limiter == NULL)
	{
		data->infile = (*av)[1];
		data->input_fd = open(data->infile, O_RDONLY);
		if (data->input_fd == -1)
		{
			ft_printf("zsh: aucun fichier ou dossier de ce type: %s\n",
				data->infile);
			data->input_fd = -5;
		}
	}
	data->outfile = (*av)[*ac - 1];
	i = -1;
	while (++i < *ac - 3)
		data->cmd[i] = (*av)[i + 2];
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
	data.error = 0;
	data.envp = envp;
	data.pid = NULL;
	data.path = NULL;
	data.split = NULL;
	data.limiter = NULL;
	data.cmd = NULL;
	if (BONUS && ft_strncmp(av[1], "here_doc", 8) == 0)
		here_doc(&data, &ac, &av);
	initialize(&data, &ac, &av);
	execute_pipeline(&data, 0, ac - 3);
	close(data.input_fd);
	free_all_stop(&data, 0, WEXITSTATUS(data.error), NULL);
	return (0);
}
