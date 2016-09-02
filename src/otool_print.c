/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   otool_print.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgauvrit <tgauvrit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/02 16:34:08 by tgauvrit          #+#    #+#             */
/*   Updated: 2016/09/02 16:41:34 by tgauvrit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

static void	print_hex(uint64_t num, int depth)
{
	if (depth == 0)
		return ;
	print_hex(num >> 4, depth - 1);
	num = num & 0xF;
	if (num < 10)
		num += '0';
	else
		num += 'a' - 10;
	write(1, &num, 1);
}

static void	set_sect_data(struct section_64 *data, struct section *sect,
	size_t size)
{
	ft_memcpy(data, sect, size);
	if (size == sizeof(struct section_64))
		return ;
	data->addr = sect->addr;
	data->size = sect->size;
	data->offset = sect->offset;
	data->align = sect->align;
	data->reloff = sect->reloff;
	data->nreloc = sect->nreloc;
	data->flags = sect->flags;
}

static void	print_section(t_file *file, struct section *sect, size_t size)
{
	void				*start;
	uint32_t			offset;
	uint32_t			i;
	uint8_t				byte_word;
	struct section_64	data;

	set_sect_data(&data, sect, size);
	multiprint(5, "(", data.segname, ",", data.sectname, ") section\n");
	start = (void*)file->map + data.offset;
	offset = 0;
	while (offset < data.size)
	{
		print_hex((uint64_t)(data.addr + offset),
			(file->magic == MH_MAGIC ? 8 : 16));
		i = 0 * (int)write(1, " ", 1);
		while (i < 16 * sizeof(char) && offset + i < data.size)
		{
			byte_word = *(uint8_t*)(start + offset + i);
			print_hex(byte_word, 2);
			write(1, " ", 1);
			i += sizeof(char);
		}
		write(1, "\n", 1);
		offset += i;
	}
}

int			otool_64(void *file, struct section_64 *sect, uint32_t nsects)
{
	while (nsects--)
	{
		if (ft_strcmp(sect->sectname, SECT_TEXT) == 0 &&
			ft_strcmp(sect->segname, SEG_TEXT) == 0)
		{
			print_section(file, (void*)sect, sizeof(*sect));
			return (1);
		}
		sect += 1;
	}
	return (0);
}

int			otool_32(void *file, struct section *sect, uint32_t nsects)
{
	while (nsects--)
	{
		if (ft_strcmp(sect->sectname, SECT_TEXT) == 0 &&
			ft_strcmp(sect->segname, SEG_TEXT) == 0)
		{
			print_section(file, (void*)sect, sizeof(*sect));
			return (1);
		}
		sect += 1;
	}
	return (0);
}
