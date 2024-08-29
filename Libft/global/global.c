/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   global.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thmouty <thmouty@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 20:00:42 by thmouty           #+#    #+#             */
/*   Updated: 2024/08/14 20:00:42 by thmouty          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "global.h"

t_global	*global(t_global *set, int reset)
{
	static t_global	*global = NULL;

	if (set)
		global = set;
	else if (reset)
		global = NULL;
	return (global);
}

int	add_global(char *name, void *ptr)
{
	t_global	*new;
	t_global	*tmp;

	if (!name)
		return (0);
	if (check_global(name, ptr))
		return (0);
	new = (t_global *)malloc(sizeof(t_global));
	if (!new)
		return (0);
	new->name = strdup(name);
	new->ptr = ptr;
	new->next = NULL;
	new->prev = NULL;
	if (!global(NULL, 0))
		global(new, 0);
	else
	{
		tmp = global(NULL, 0);
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
		new->prev = tmp;
	}
	return (1);
}

void	*get_global(char *name)
{
	t_global	*tmp;

	tmp = global(NULL, 0);
	while (tmp)
	{
		if (!strcmp(tmp->name, name))
			return (tmp->ptr);
		tmp = tmp->next;
	}
	return (NULL);
}

void	set_global(char *name, void *ptr)
{
	t_global	*tmp;

	if (!name || check_global(NULL, ptr))
		return ;
	tmp = global(NULL, 0);
	while (tmp)
	{
		if (!strcmp(tmp->name, name))
		{
			tmp->ptr = ptr;
			return ;
		}
		tmp = tmp->next;
	}
}

void	del_global(char *name)
{
	t_global	*tmp;

	tmp = global(NULL, 0);
	while (tmp)
	{
		if (!strcmp(tmp->name, name))
		{
			if (tmp->prev)
				tmp->prev->next = tmp->next;
			else
				global(tmp->next, 1);
			if (tmp->next)
				tmp->next->prev = tmp->prev;
			free(tmp->name);
			free(tmp->ptr);
			free(tmp);
			return ;
		}
		tmp = tmp->next;
	}
}

// int main()
// {
// 	int *a = (int *)malloc(sizeof(int));
// 	int *b = (int *)malloc(sizeof(int));
// 	int *c = (int *)malloc(sizeof(int));

// 	*a = 1;
// 	*b = 2;
// 	*c = 3;

// 	add_global("a", a);
// 	add_global("aeee", a);
// 	add_global("b", b);
// 	set_global("a", c);
// 	add_global("c", a);
// 	rename_global("c", "d");

// 	printf("a = %d\n", *(int *)get_global("a"));
// 	printf("b = %d\n", *(int *)get_global("b"));
// 	printf("c = %d\n", *(int *)get_global("d"));

// 	printf("\nSuppression de b\n");
// 	print_global();
// 	del_global("b");
// 	printf("a = %d\n", *(int *)get_global("a"));
// 	printf("b = %p\n", get_global("b"));
// 	printf("c = %d\n", *(int *)get_global("d"));

// 	printf("\nSuppression de a\n");
// 	print_global();

// 	del_global("a");
// 	printf("a = %p\n", get_global("a"));
// 	printf("b = %p\n", get_global("b"));
// 	printf("c = %d\n", *(int *)get_global("d"));

// 	printf("\nSuppression de c\n");
// 	print_global();
// 	del_global("c");
// 	printf("a = %p\n", get_global("a"));
// 	printf("b = %p\n", get_global("b"));
// 	printf("c = %p\n", get_global("d"));

// 	free_global();
// 	return (0);
// }
