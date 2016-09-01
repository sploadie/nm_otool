/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgauvrit <tgauvrit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/08/28 13:59:15 by tanguy            #+#    #+#             */
/*   Updated: 2016/09/01 16:28:10 by tgauvrit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

static void	fill_hex(char *buf, int index, uint64_t num)
{
	if (index > 0)
		fill_hex(buf, index - 1, num >> 4);
	num = num & 0xF;
	if (num < 10)
		num += '0';
	else
		num += 'a' - 10;
	buf[index] = num;
}

static void sort_sym(char **a, size_t size)
{
	size_t i;
	size_t j;

	i = -1;
	while (++i <  size - 1)
	{
		j = -1;
		while (++j < size - i - 1)
		{
			if (ft_strcmp(a[j] + 19, a[j + 1] + 19) > 0)
				ft_memswap(a + j, a + j + 1, sizeof(char*));
		}
	}
}

static char get_type(uint16_t n_type, uint16_t n_sect, uint64_t n_value)
{
	char	c;

	c = n_type;
	if (c & N_STAB)
	    c = '-';
	else
	{
	    c = c & N_TYPE;
	    if (c == N_UNDF && n_value == 0)
			c = 'u';
		else if(c == N_UNDF && n_value != 0)
		    c = 'c';
	    else if (c == N_PBUD)
			c = 'u';
	    else if (c == N_ABS)
		    c = 'a';
		else if (c == N_SECT && n_sect == section_numbers(NULL, 0)->text)
		    c = 't';
		else if (c == N_SECT && n_sect == section_numbers(NULL, 0)->data)
		    c = 'd';
		else if (c == N_SECT && n_sect == section_numbers(NULL, 0)->bss)
		    c = 'b';
		else if (c == N_SECT)
		    c = 's';
	    else if (c == N_INDR)
			c = 'i';
		else
			c = '?';
	}
	if((n_type & N_EXT) && c != '?')
	    c -= 32;
	return (c);
}

static void print_64(char *names, int nsyms, struct nlist_64 *symbols)
{
	int		i;
	char	hex[20];
	char*	strings[nsyms];

	i = -1;
	while (++i < nsyms)
	{
		ft_memset(hex, ' ', 19);
		hex[19] = 0;
		hex[17] = get_type(symbols[i].n_type, symbols[i].n_sect, symbols[i].n_value);
		if (symbols[i].n_type != 0x1)
			fill_hex(hex, 15, symbols[i].n_value);
		strings[i] = ft_strjoin(hex, names + symbols[i].n_un.n_strx);
	}
	sort_sym(strings, nsyms);
	i = -1;
	while (++i < nsyms)
	{
		if (strings[i][17] != '-')
			ft_putendl(strings[i]);
		free(strings[i]);
	}
}

static void	handle_64(struct mach_header_64 *header, struct symtab_command *symtab)
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
	char*	strings[nsyms];

	i = -1;
	while (++i < nsyms)
	{
		ft_memset(hex, ' ', 19);
		hex[19] = 0;
		if (symbols[i].n_type != 0x1)
			fill_hex(hex, 15, symbols[i].n_value);
		hex[17] = get_type(symbols[i].n_type, symbols[i].n_sect, symbols[i].n_value);
		strings[i] = ft_strjoin(hex, names + symbols[i].n_un.n_strx);
	}
	sort_sym(strings, nsyms);
	i = -1;
	while (++i < nsyms)
	{
		ft_putendl(strings[i]);
		free(strings[i]);
	}
}

static void	handle_32(struct mach_header *header, struct symtab_command *symtab)
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
	return (nm(&file));
}

int		nm(t_file *file)
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
