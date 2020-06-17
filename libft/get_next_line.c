/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snechaev <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/07 11:45:49 by snechaev          #+#    #+#             */
/*   Updated: 2019/03/20 14:47:40 by snechaev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int	find_end(char **line, char **dep, const int fd)
{
	int		i;
	char	*tmp;

	i = 0;
	while (dep[fd][i] != '\n' && dep[fd][i])
	{
		i++;
	}
	if (dep[fd][i] == '\n')
	{
		*line = ft_strsub(dep[fd], 0, i);
		tmp = ft_strdup(&dep[fd][i + 1]);
		ft_strdel(&dep[fd]);
		dep[fd] = tmp;
	}
	else
	{
		*line = ft_strdup(dep[fd]);
		ft_strdel(&dep[fd]);
	}
	return (1);
}

int	file_read(const int fd, char **line, char **dep)
{
	char	buff[BUFF_SIZE + 1];
	int		nread;
	char	*tmp;

	while ((nread = read(fd, buff, BUFF_SIZE)) > 0)
	{
		buff[nread] = '\0';
		tmp = ft_strjoin(dep[fd], buff);
		ft_strdel(&dep[fd]);
		dep[fd] = tmp;
		if (ft_strchr(buff, '\n'))
			break ;
	}
	if (nread < 0)
		return (-1);
	else if (nread == 0 && dep[fd][0] == '\0')
		return (0);
	return (find_end(line, dep, fd));
}

int	get_next_line(const int fd, char **line)
{
	static char	*dep[MAX_FD];

	if (fd < 0 || !line || BUFF_SIZE <= 0 || fd > MAX_FD)
		return (-1);
	if (!dep[fd])
		dep[fd] = ft_strnew(1);
	return (file_read(fd, line, dep));
}
