/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_joinf.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thmouty <thmouty@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 17:25:25 by thmouty           #+#    #+#             */
/*   Updated: 2024/05/13 03:36:12 by thmouty          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

static char	*checker_buffer(char *arg)
{
	char	*buffer;

	if (arg == NULL)
		buffer = ft_strdup("(null)");
	else
		buffer = ft_strdup(arg);
	return (buffer);
}

static char	*checker_buffer_free(char **arg, int dollar)
{
	char	*buffer;

	if (!arg || *arg == NULL)
		buffer = ft_strdup("(null)");
	else
		buffer = ft_strdup(*arg);
	if (dollar && *arg)
	{
		free(*arg);
		*arg = NULL;
	}
	return (buffer);
}

static char	*checker(const char *str, va_list args, int dollar)
{
	if (!dollar && *str == 'c')
		return (ft_strjoinc(va_arg(args, int)));
	else if (*str == 's')
	{
		if (dollar)
			return (checker_buffer_free(va_arg(args, char **), dollar));
		return (checker_buffer(va_arg(args, char *)));
	}
	else if (!dollar && (*str == 'd' || *str == 'i'))
		return (ft_itoa(va_arg(args, int)));
	else if (!dollar && *str == 'u')
		return (ft_itoa(va_arg(args, unsigned int)));
	else if (!dollar && *str == 'x')
		return (ft_itoa_base(va_arg(args, unsigned int), "0123456789abcdef"));
	else if (!dollar && *str == 'X')
		return (ft_itoa_base(va_arg(args, unsigned int), "0123456789ABCDEF"));
	return (0);
}

/**
 * @param[in] str La chaîne de format contenant des spécificateurs de formatage.
 * @param[in] ... Les arguments correspondant aux spécificateurs de format.
 * @return Nouvelle chaîne allouée dynamiquement contenant le résultat formaté.
 *
 * Cette fonction fonctionne de manière similaire à `printf`, sauf qu'au lieu
 * d'afficher le résultat, elle retourne la chaîne formatée. Elle prend en
 * charge les spécificateurs de format standard (`%s`, `%d`, etc.) et un
 * spécificateur spécial `$s`. Lorsqu'un `$s` est utilisé, la chaîne
 * correspondante est non seulement insérée dans le résultat, mais elle
 * est également libérée (`free`) après l'insertion, et le pointeur
 * d'origine est mis à `NULL`.
 *
 * Exemple d'utilisation :
 * @code
 * char *str1 = strdup("world");
 * char *result = ft_joinf("%s, $s!", "Hello", &str1);
 * // result contient "Hello, world!" et `str1` a été libéré et mis à NULL.
 * @endcode
 */
char	*ft_joinf(const char *str, ...)
{
	char	*line;
	char	*tmp;
	va_list	args;

	va_start(args, str);
	line = ft_strdup("");
	tmp = NULL;
	while (*str)
	{
		if (*str == '%' && *(++str) != '%')
			tmp = checker(str, args, 0);
		else if (*str == '$' && *(++str) != '$')
			tmp = checker(str, args, 1);
		else
			tmp = ft_strjoinc(*str);
		line = ft_strjoin(line, tmp, 3);
		str++;
	}
	va_end(args);
	return (line);
}

int	ft_fprintf(int fd, const char *str, ...)
{
	int		count;
	char	*line;
	char	*tmp;
	va_list	args;

	va_start(args, str);
	line = ft_strdup("");
	tmp = NULL;
	while (*str)
	{
		if (*str == '%' && *(++str) != '%')
			tmp = checker(str, args, 0);
		else if (*str == '$' && *(++str) != '$')
			tmp = checker(str, args, 1);
		else
			tmp = ft_strjoinc(*str);
		line = ft_strjoin(line, tmp, 3);
		str++;
	}
	va_end(args);
	count = write(fd, line, ft_strlen(line));
	free(line);
	return (count);
}
