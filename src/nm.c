/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tanguy <tanguy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/08/28 13:59:15 by tanguy            #+#    #+#             */
/*   Updated: 2016/08/29 11:51:39 by tanguy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"
#include <stdio.h> // FIXME

static void	fill_hex(char *buf, int index, uint64_t num)
{
	if (index > 0)
		fill_hex(buf, index - 1, num >> 4);
	num -= (num >> 4) << 4;
	if (num < 10)
		num += '0';
	else
		num += 'a' - 10;
	buf[index] = num;
}

static void sort_sym(t_sym *a, size_t size)
{
	size_t i;
	size_t j;

	i = -1;
	while (++i <  size - 1 )
	{
		j = -1;
		while (++j < size - i - 1)
		{
			if (ft_strcmp(a[j].name, a[j + 1].name) > 0)
				ft_memswap(a + j, a + j + 1, sizeof(t_sym));
		}
	}
}

void	print_hex(long num, int depth) // DEBUG ONLY
{
	if (depth == 0)
	{
		write(1, "0x", 2);
		return ;
	}
	print_hex(num >> 4, depth - 1);
	num -= (num >> 4) << 4;
	if (num < 10)
		num += '0';
	else
		num += 55;
	write(1, &num, 1);
}

static char get_type(uint16_t n_type, uint16_t n_sect)
{
	uint16_t type;

	type = (n_type << 8) | n_sect;
	// print_hex(type, 4);
	// write(1, " ", 1);
	if (type == 0x0100)
		return 'U';
	if (type == 0x0e01)
		return 't';
	if (type == 0x0e05)
		return 's';
	if (type == 0x0e09)
		return 's';
	if (type == 0x0e0b)
		return 's';
	if (type == 0x0e0c)
		return 'b';
	if (type == 0x0e0d)
		return 'd';
	if (type == 0x0e0e)
		return 'b';
	if (type == 0x0f01)
		return 'T';
	if (type == 0x0f05)
		return 'S';
	if (type == 0x0f09)
		return 'd';
	if (type == 0x0f0b)
		return 'S';
	if (type == 0x0f0d)
		return 'D';
	return '?';
}

static void print_64(char *names, int nsyms, struct nlist_64 *symbols)
{
	int		i;
	char	hex[20];
	t_sym	strings[nsyms];

	i = -1;
	while (++i < nsyms)
	{
		ft_memset(hex, ' ', 19);
		hex[19] = 0;
		if (symbols[i].n_type != 0x1)
			fill_hex(hex, 15, symbols[i].n_value);
		hex[17] = get_type(symbols[i].n_type, symbols[i].n_sect);
		strings[i].str = ft_strjoin(hex, names + symbols[i].n_un.n_strx);
		strings[i].name = names + symbols[i].n_un.n_strx;
		// ft_putstr(hex);
		// ft_putendl(names + symbols[i].n_un.n_strx);
		// // printf("%c %s\n", get_type(symbols[i].n_type, symbols[i].n_sect), names + symbols[i].n_un.n_strx);
		// // printf("%c %u %u %u %s\n", get_type(symbols[i].n_type, symbols[i].n_type, symbols[i].n_sect, symbols[i].n_desc, names + symbols[i].n_un.n_strx);
		// // multiprint(2, names + symbols[i].n_un.n_strx, "\n");
	}
	sort_sym(strings, nsyms);
	i = -1;
	while (++i < nsyms)
	{
		ft_putendl(strings[i].str);
		free(strings[i].str);
	}
}

static void	handle_64(struct mach_header_64 *header, struct symtab_command *symtab)
{
	unsigned int		i;
	struct load_command	*lc;

	// printf("64\n");
	lc = (struct load_command *)(header + 1);
	i = 0;
	while (i++ < header->ncmds)
	{
		if (lc->cmd == LC_SYMTAB)
		{
			symtab = (void*)lc;
			break;
		}
		lc = (void*)lc + lc->cmdsize;
	}
	if (symtab == NULL)
		return (ft_putstr("No symbol table found.\n"));
	print_64((void*)header + symtab->stroff, symtab->nsyms, (void*)header + symtab->symoff);
}

static void print_32(char *names, int nsyms, struct nlist *symbols)
{
	int		i;
	char	hex[20];
	t_sym	strings[nsyms];

	i = -1;
	while (++i < nsyms)
	{
		ft_memset(hex, ' ', 19);
		hex[19] = 0;
		if (symbols[i].n_type != 0x1)
			fill_hex(hex, 15, symbols[i].n_value);
		hex[17] = get_type(symbols[i].n_type, symbols[i].n_sect);
		strings[i].str = ft_strjoin(hex, names + symbols[i].n_un.n_strx);
		strings[i].name = names + symbols[i].n_un.n_strx;
		// ft_putstr(hex);
		// ft_putendl(names + symbols[i].n_un.n_strx);
		// printf("%c %s\n", get_type(symbols[i].n_type, symbols[i].n_sect), names + symbols[i].n_un.n_strx);
		// printf("%c %u %u %u %s\n", get_type(symbols[i].n_type, symbols[i].n_type, symbols[i].n_sect, symbols[i].n_desc, names + symbols[i].n_un.n_strx);
		// multiprint(2, names + symbols[i].n_un.n_strx, "\n");
	}
	sort_sym(strings, nsyms);
	i = -1;
	while (++i < nsyms)
	{
		ft_putendl(strings[i].str);
		free(strings[i].str);
	}
}

static void	handle_32(struct mach_header *header, struct symtab_command *symtab)
{
	unsigned int		i;
	struct load_command	*lc;

	// printf("32\n");
	lc = (struct load_command *)(header + 1);
	i = 0;
	while (i++ < header->ncmds)
	{
		if (lc->cmd == LC_SYMTAB)
		{
			symtab = (void*)lc;
			break;
		}
		lc = (void*)lc + lc->cmdsize;
	}
	if (symtab == NULL)
		return (ft_putstr("No symbol table found.\n"));
	print_32((void*)header + symtab->stroff, symtab->nsyms, (void*)header + symtab->symoff);
}

static int	handle_swap_fat(struct fat_header *header)
{
	struct fat_arch	*arch;
	uint32_t		nfat_arch;
	t_file			file;
	uint32_t		magic;

	nfat_arch = swap_uint32(header->nfat_arch);
	// printf("FAT, magic number %x, %u entries\n", header->magic, nfat_arch);
	arch = (void*)(header + 1);
	while (nfat_arch--)
	{
		// printf("%u %u %u\n", swap_uint32(arch->offset), swap_uint32(arch->size), swap_uint32(arch->align));
		magic = *(unsigned int*)((void*)header + swap_uint32(arch->offset));
		if (magic == MH_MAGIC_64 || magic == MH_MAGIC)
		{
			file.magic = magic;
			file.map = (void*)header + swap_uint32(arch->offset);
		}
		// if (nm(&file))
		// 	break;
		arch = arch + 1;
	}
	return (nm(&file));
}

int		nm(t_file *file)
{
	// printf("%x\n", file->magic);
	if (file->magic == MH_MAGIC_64)
		handle_64(file->map, NULL);
	else if (file->magic == MH_MAGIC)
		handle_32(file->map, NULL);
	else if (file->magic == FAT_CIGAM)
		return (handle_swap_fat(file->map) + 2);
	else
		return (0);
	return (1);
	// usr/include/mach-o/loader.h
	// usr/include/mach-o/nlist.h
	// Add 32-bit handling
}
