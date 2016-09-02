/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multiprint.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgauvrit <tgauvrit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/08/28 13:14:24 by tanguy            #+#    #+#             */
/*   Updated: 2016/09/02 15:09:08 by tgauvrit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

void	multiprint(int size, ...)
{
	int		i;
	va_list	strings;

	va_start(strings, size);
	i = 0;
	while (i++ < size)
		ft_putstr(va_arg(strings, char*));
	va_end(strings);
}
