/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgauvrit <tgauvrit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/02/03 15:35:45 by tgauvrit          #+#    #+#             */
/*   Updated: 2016/09/02 16:43:32 by tgauvrit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NM_H
# define NM_H

# include <stdlib.h>
# include <unistd.h>
# include <stdint.h>
# include <sys/mman.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <mach-o/loader.h>
# include <mach-o/nlist.h>
# include <mach-o/fat.h>
# include <errno.h>
# include <stdarg.h>
# include "libft.h"

typedef struct		s_file
{
	struct stat		stat;
	void			*map;
	unsigned int	magic;
}					t_file;

typedef struct		s_sym
{
	char			*str;
	char			*name;
}					t_sym;

typedef struct		s_section_num
{
	uint32_t		text;
	uint32_t		data;
	uint32_t		bss;
	uint32_t		nsect;
}					t_section_num;

t_file				*file_create(char *filename);
void				file_delete(t_file	*file);

int					nm(t_file *file);
int					otool(t_file *file);

void				print_64(char *names, int nsyms, struct nlist_64 *symbols);
void				print_32(char *names, int nsyms, struct nlist *symbols);

int					otool_64(void *file, struct section_64 *sect,
	uint32_t nsects);
int					otool_32(void *file, struct section *sect, uint32_t nsects);

t_section_num		*section_numbers(struct load_command *lc, uint32_t ncmds);

void				multiprint(int size, ...);

uint16_t			swap_uint16(uint16_t val);
int16_t				swap_int16(int16_t val);
uint32_t			swap_uint32(uint32_t val);
int32_t				swap_int32(int32_t val);

#endif
