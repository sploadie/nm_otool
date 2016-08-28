/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tanguy <tanguy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/08/28 12:12:46 by tanguy            #+#    #+#             */
/*   Updated: 2016/08/28 14:05:59 by tanguy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

t_file	*file_create(char *filename)
{
	t_file	*file;
	int		fd;

	file = (t_file*)malloc(sizeof(t_file));
	fd = open(filename, O_RDONLY);
	if (fd < 0 ||
		fstat(fd, &(file->stat)) < 0 ||
		S_ISREG(file->stat.st_mode) == 0 ||
		(file->map = mmap(0, file->stat.st_size, PROT_READ, MAP_PRIVATE, fd, 0))
		== MAP_FAILED)
	{
		free(file);
		return NULL;
	}
	close(fd);
	file->magic = *(unsigned int*)(file->map);
	return file;
}

void	file_delete(t_file *file)
{
	errno = 0;
	munmap(file->map, file->stat.st_size);
	free(file);
}
