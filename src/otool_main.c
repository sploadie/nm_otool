/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   otool_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tanguy <tanguy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/08/28 12:12:46 by tanguy            #+#    #+#             */
/*   Updated: 2016/08/29 12:04:36 by tanguy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

static void	print_hex(long num, int depth)
{
	if (depth == 0)
		return ;
	print_hex(num >> 4, depth - 1);
	num -= (num >> 4) << 4;
	if (num < 10)
		num += '0';
	else
		num += 'a' - 10;
	write(1, &num, 1);
}

static void	handle_32(struct mach_header *header, struct segment_command *seg)
{
	unsigned int		i;
	struct load_command	*lc;

	lc = (struct load_command *)(header + 1);
	i = 0;
	while (i++ < header->ncmds)
	{
		if (lc->cmd == LC_SEGMENT)
		{
			seg = (void*)lc;
			break;
		}
		lc = (void*)lc + lc->cmdsize;
	}
	if (seg == NULL)
		return (ft_putstr("No __TEXT segment found.\n"));
	//PRINT
	print_hex(0x32, 4);
	write(1, "\n", 1);
}

static void	handle_64(struct mach_header *header, struct segment_command_64 *seg)
{
	unsigned int		i;
	struct load_command	*lc;

	lc = (struct load_command *)(header + 1);
	i = 0;
	while (i++ < header->ncmds)
	{
		if (lc->cmd == LC_SEGMENT)
		{
			seg = (void*)lc;
			break;
		}
		lc = (void*)lc + lc->cmdsize;
	}
	if (seg == NULL)
		return (ft_putstr("No __TEXT segment found.\n"));
	//PRINT
	print_hex(0x64, 4);
	write(1, "\n", 1);
}

static int	handle_swap_fat(struct fat_header *header)
{
	struct fat_arch	*arch;
	uint32_t		nfat_arch;
	t_file			file;
	uint32_t		magic;

	nfat_arch = swap_uint32(header->nfat_arch);
	arch = (void*)(header + 1);
	while (nfat_arch--)
	{
		magic = *(unsigned int*)((void*)header + swap_uint32(arch->offset));
		if (magic == MH_MAGIC_64 || magic == MH_MAGIC)
		{
			file.magic = magic;
			file.map = (void*)header + swap_uint32(arch->offset);
		}
		arch = arch + 1;
	}
	return (otool(&file));
}

int			otool(t_file *file)
{
	if (file->magic == MH_MAGIC_64)
		handle_64(file->map, NULL);
	else if (file->magic == MH_MAGIC)
		handle_32(file->map, NULL);
	else if (file->magic == FAT_CIGAM)
		return (handle_swap_fat(file->map) + 2);
	else
		return (0);
	return (1);
}

static void	do_otool(t_file *file)
{
	int		ret;

	if (file == NULL)
		return (ft_putstr("Could not access file.\n"));
	ret = otool(file);
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
		ft_putstr("Usage: ./ft_otool FILE1 [FILE2 ...]");
		return 1;
	}
	i = 0;
	while (++i < argc)
	{
		if (argc > 2)
			multiprint(3, "\n", argv[i], ": ");
		do_otool(file_create(argv[i]));
	}
	return 0;
}
