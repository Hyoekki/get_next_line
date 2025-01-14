/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhyokki <jhyokki@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 13:35:26 by jhyokki           #+#    #+#             */
/*   Updated: 2025/01/09 20:09:33 by jhyokki          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "get_next_line.h"

static char	*read_to_buffer(int fd, char *buffer)
{
	char	temp[BUFFER_SIZE + 1];
	ssize_t	bytes_read;
	char	*new_buffer;

	if (!buffer)
		buffer = ft_strdup("");
	bytes_read = 1;
	while (!ft_strchr(buffer, '\n') && bytes_read > 0)
	{
		bytes_read = read(fd, temp, BUFFER_SIZE);
		if (bytes_read > 0)
		{
			temp[bytes_read] = '\0';
			new_buffer = ft_strjoin(buffer, temp);
			free(buffer);
			buffer = new_buffer;
		}
		else if (bytes_read == -1)
		{
			free(buffer);
			return (NULL);
		}
	}
	return (buffer);
}

static char	*get_line(char *buffer, size_t len)
{
	char	*line;

	line = malloc(sizeof(line) * (len + 1));
	if (!line)
		return (NULL);
	ft_memmove(line, buffer, len);
	line[len] = '\0';
	return (line);
}

static void	update_buffer(char **buffer, char *newline_pos)
{
	char	*remaining;

	remaining = ft_strdup(newline_pos + 1);
	free(*buffer);
	*buffer = remaining;
}

static char	*extract_line(char **buffer)
{
	char	*newline_pos;
	size_t	len;
	char	*line;

	newline_pos = ft_strchr(*buffer, '\n');
	if (newline_pos)
		len = newline_pos - *buffer + 1;
	else
		len = ft_strlen(*buffer);
	line = get_line(*buffer, len);
	if (!line)
		return (NULL);
	if (newline_pos)
		update_buffer(buffer, newline_pos);
	else
	{
		free(*buffer);
		*buffer = (NULL);
	}
	return (line);
}

char	*get_next_line(int fd)
{
	static char	*buffer;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	buffer = read_to_buffer(fd, buffer);
	if (!buffer || *buffer == '\0')
	{
		free(buffer);
		buffer = NULL;
		return (NULL);
	}
	return (extract_line(&buffer));
}
