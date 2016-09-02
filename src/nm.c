/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgauvrit <tgauvrit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/08/28 13:59:15 by tanguy            #+#    #+#             */
/*   Updated: 2016/09/02 15:24:17 by tgauvrit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

static void	handle_64(struct mach_header_64 *header,
	struct symtab_command *symtab)
{
	unsigned int		i;
	struct load_command	*lc;

	lc = (struct load_command *)(header + 1);
	section_numbers(lc, header->ncmds);
	i = 0;
	while (i++ < header->ncmds)
	{
		if (lc->cmd == LC_SYMTAB)
		{
			symtab = (void*)lc;
			break ;
		}
		lc = (void*)lc + lc->cmdsize;
	}
	if (symtab == NULL)
		return (ft_putstr("No symbol table found.\n"));
	print_64((void*)header + symtab->stroff, symtab->nsyms,
		(void*)header + symtab->symoff);
}

static void	handle_32(struct mach_header *header,
	struct symtab_command *symtab)
{
	unsigned int		i;
	struct load_command	*lc;

	lc = (struct load_command *)(header + 1);
	section_numbers(lc, header->ncmds);
	i = 0;
	while (i++ < header->ncmds)
	{
		if (lc->cmd == LC_SYMTAB)
		{
			symtab = (void*)lc;
			break ;
		}
		lc = (void*)lc + lc->cmdsize;
	}
	if (symtab == NULL)
		return (ft_putstr("No symbol table found.\n"));
	print_32((void*)header + symtab->stroff, symtab->nsyms,
		(void*)header + symtab->symoff);
}

static int	handle_swap_fat(struct fat_header *header, uint32_t nfat_arch)
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
	return (nm(&file));
}

int			nm(t_file *file)
{
	if (file->magic == MH_MAGIC_64)
		handle_64(file->map, NULL);
	else if (file->magic == MH_MAGIC)
		handle_32(file->map, NULL);
	else if (file->magic == FAT_CIGAM)
		return (handle_swap_fat(file->map, 0) + 2);
	else
		return (0);
	return (1);
}
