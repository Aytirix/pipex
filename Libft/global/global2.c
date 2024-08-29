/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   global2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thmouty <thmouty@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 20:00:42 by thmouty           #+#    #+#             */
/*   Updated: 2024/08/14 20:00:42 by thmouty          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "global.h"

int	rename_global(char *name, char *new_name)
{
	t_global	*tmp;

	if (!name || !new_name || check_global(new_name, NULL))
		return (0);
	tmp = global(NULL, 0);
	while (tmp)
	{
		if (!strcmp(tmp->name, name))
		{
			free(tmp->name);
			tmp->name = strdup(new_name);
			return (1);
		}
		tmp = tmp->next;
	}
	return (0);
}

void	print_global(void)
{
	t_global	*tmp;

	tmp = global(NULL, 0);
	while (tmp)
	{
		printf("name: %s\n", tmp->name);
		tmp = tmp->next;
	}
}

int	check_global(char *name, void *ptr)
{
	t_global	*tmp;

	tmp = global(NULL, 0);
	while (tmp)
	{
		if (name && !strcmp(tmp->name, name))
		{
			printf("La variable %s existe déjà\n", name);
			return (1);
		}
		else if (ptr && tmp->ptr == ptr)
		{
			printf("le pointeur %p est déjà utilisé par la variable %s\n", ptr,
				tmp->name);
			return (1);
		}
		tmp = tmp->next;
	}
	return (0);
}

void	free_global(void)
{
	t_global	*tmp;
	t_global	*next;

	tmp = global(NULL, 0);
	while (tmp)
	{
		next = tmp->next;
		free(tmp->name);
		free(tmp->ptr);
		free(tmp);
		tmp = next;
	}
	global(NULL, 1);
}
