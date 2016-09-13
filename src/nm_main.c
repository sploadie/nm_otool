/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm_main.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgauvrit <tgauvrit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/08/28 12:12:46 by tanguy            #+#    #+#             */
/*   Updated: 2016/09/13 16:57:09 by tgauvrit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

static void	read_nm(t_file *file)
{
	int		ret;

	if (file == NULL)
		return (ft_putstr("Could not access file.\n"));
	ret = nm(file);
	if (ret == 0)
		return (ft_putstr("File type not found.\n"));
	else if (ret == 2)
		return (ft_putstr("Fat did not contain known architecture.\n"));
	file_delete(file);
}

int			main(int argc, char **argv)
{
	int i;

	if (argc < 2)
	{
		read_nm(file_create("a.out"));
		return (0);
	}
	i = 0;
	while (++i < argc)
	{
		if (argc > 2)
			multiprint(3, "\n", argv[i], ":\n");
		read_nm(file_create(argv[i]));
	}
	return (0);
}
