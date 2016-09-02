/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_symbols.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgauvrit <tgauvrit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/02 15:13:21 by tgauvrit          #+#    #+#             */
/*   Updated: 2016/09/02 15:20:39 by tgauvrit         ###   ########.fr       */
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

static void	sort_sym(char **a, size_t size)
{
	int		ret;
	size_t	i;
	size_t	j;

	i = -1;
	while (++i < size - 1)
	{
		j = -1;
		while (++j < size - i - 1)
		{
			ret = ft_strcmp(a[j] + 19, a[j + 1] + 19);
			if (ret > 0 || (
				ret == 0 && ft_strncmp(a[j] + 17, a[j + 1] + 17, 1) < 0))
				ft_memswap(a + j, a + j + 1, sizeof(char*));
		}
	}
}

static char	get_type(uint16_t n_type, uint16_t n_sect, uint64_t n_value)
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
		else if (c == N_UNDF && n_value != 0)
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
	if ((n_type & N_EXT) && c != '?')
		c -= 32;
	return (c);
}

void		print_64(char *names, int nsyms, struct nlist_64 *symbols)
{
	int		i;
	char	hex[20];
	char	*strings[nsyms];

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

void		print_32(char *names, int nsyms, struct nlist *symbols)
{
	int		i;
	char	hex[20];
	char	*strings[nsyms];

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
		if (strings[i][17] != '-')
			ft_putendl(strings[i] + 8);
		free(strings[i]);
	}
}
