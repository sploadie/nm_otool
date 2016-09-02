/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   otool_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgauvrit <tgauvrit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/08/28 12:12:46 by tanguy            #+#    #+#             */
/*   Updated: 2016/09/02 16:36:47 by tgauvrit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

static void	handle_arch(void *file, struct load_command *lc, size_t size)
{
	int			ret;
	uint32_t	ncmds;

	ret = 0;
	ncmds = ((struct mach_header*)lc)->ncmds;
	lc = (void*)lc + size;
	while (ncmds-- > 0)
	{
		if (lc->cmd == LC_SEGMENT)
		{
			ret |= otool_32(file, (void*)lc + sizeof(struct segment_command),
				((struct segment_command*)lc)->nsects);
		}
		if (lc->cmd == LC_SEGMENT_64)
		{
			ret |= otool_64(file, (void*)lc + sizeof(struct segment_command_64),
				((struct segment_command_64*)lc)->nsects);
		}
		lc = (void*)lc + lc->cmdsize;
	}
	if (ret == 0)
		ft_putstr("No __text section found.\n");
}

static int	otool_fat(struct fat_header *header, uint32_t nfat_arch)
{
	struct fat_arch			*arch;
	t_file					file;
	struct mach_header		*mach;

	nfat_arch = swap_uint32(header->nfat_arch);
	arch = (void*)(header + 1);
	while (nfat_arch--)
	{
		mach = (void*)((void*)header + swap_uint32(arch->offset));
		if (mach->magic == MH_MAGIC_64 && mach->cputype == CPU_TYPE_X86_64)
		{
			file.map = (void*)header + swap_uint32(arch->offset);
			if (__POINTER_WIDTH__ == 64)
				break ;
		}
		if (mach->magic == MH_MAGIC && mach->cputype == CPU_TYPE_X86)
		{
			file.map = (void*)header + swap_uint32(arch->offset);
			if (__POINTER_WIDTH__ == 32)
				break ;
		}
		arch = arch + 1;
	}
	file.magic = *(unsigned int*)file.map;
	return (otool(&file));
}

int			otool(t_file *file)
{
	if (file->magic == MH_MAGIC_64)
		handle_arch(file, file->map, sizeof(struct mach_header_64));
	else if (file->magic == MH_MAGIC)
		handle_arch(file, file->map, sizeof(struct mach_header));
	else if (file->magic == FAT_CIGAM)
		return (otool_fat(file->map, 0) + 2);
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
		return (1);
	}
	i = 0;
	while (++i < argc)
	{
		multiprint(2, argv[i], ":\n");
		do_otool(file_create(argv[i]));
	}
	return (0);
}
