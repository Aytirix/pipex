/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   global.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thmouty <thmouty@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 20:00:42 by thmouty           #+#    #+#             */
/*   Updated: 2024/08/14 20:00:42 by thmouty          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GLOBAL_H

# define GLOBAL_H
# include <stddef.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>

typedef struct s_global
{
	char			*name;
	void			*ptr;
	struct s_global	*next;
	struct s_global	*prev;
}					t_global;

// global.c
t_global			*global(t_global *set, int reset);
int					add_global(char *name, void *ptr);
void				*get_global(char *name);
void				set_global(char *name, void *ptr);
void				del_global(char *name);

// global2.c
void				free_global(void);
void				print_global(void);
int					check_global(char *name, void *ptr);
int					rename_global(char *name, char *new_name);

#endif