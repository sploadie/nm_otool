/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm_main.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tanguy <tanguy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/08/28 12:12:46 by tanguy            #+#    #+#             */
/*   Updated: 2016/08/29 11:09:44 by tanguy           ###   ########.fr       */
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
		ft_putstr("Usage: ./nm FILE1 [FILE2 ...]");
		return 1;
	}
	i = 0;
	while (++i < argc)
	{
		if (argc > 2)
			multiprint(3, "\n", argv[i], ": ");
		read_nm(file_create(argv[i]));
	}
	return 0;
}
