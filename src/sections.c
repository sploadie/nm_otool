/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sections.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgauvrit <tgauvrit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/01 11:13:55 by tgauvrit          #+#    #+#             */
/*   Updated: 2016/09/02 12:42:20 by tgauvrit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

static void		*preset_sections(t_section_num *secs)
{
	if (secs == NULL)
		secs = malloc(sizeof(t_section_num));
	secs->text = 0;
	secs->data = 0;
	secs->bss = 0;
	secs->nsect = 1;
	return (secs);
}

static void		get_sections(t_section_num *secs, uint32_t i, struct section *s, size_t size)
{
	while (i-- > 0)
	{
		if(ft_strcmp(s->sectname, SECT_TEXT) == 0 && ft_strcmp(s->segname, SEG_TEXT) == 0)
			secs->text = secs->nsect;
		else if(ft_strcmp(s->sectname, SECT_DATA) == 0 && ft_strcmp(s->segname, SEG_DATA) == 0)
			secs->data = secs->nsect;
		else if(ft_strcmp(s->sectname, SECT_BSS) == 0 && ft_strcmp(s->segname, SEG_DATA) == 0)
			secs->bss = secs->nsect;
		s = (void*)s + size;
		secs->nsect++;
	}
}

t_section_num	*section_numbers(struct load_command *lc, uint32_t ncmds)
{
	static t_section_num	*secs = NULL;

	if (lc == NULL)
		return (secs);
	secs = preset_sections(secs);
	while (ncmds-- > 0)
	{
		if (lc->cmd == LC_SEGMENT)
			get_sections(secs, ((struct segment_command*)lc)->nsects, (void*)lc + sizeof(struct segment_command), sizeof(struct section));
		if (lc->cmd == LC_SEGMENT_64)
			get_sections(secs, ((struct segment_command_64*)lc)->nsects, (void*)lc + sizeof(struct segment_command_64), sizeof(struct section_64));
		lc = (void*)lc + lc->cmdsize;
	}
	return (secs);
}