/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multiprint.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tanguy <tanguy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/08/28 13:14:24 by tanguy            #+#    #+#             */
/*   Updated: 2016/08/28 13:25:48 by tanguy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

void	multiprint(int size, ...)
{
	int i;
	va_list strings;

	va_start(strings, size);
	i = 0;
	while (i++ < size)
		ft_putstr(va_arg(strings, char*));
	va_end(strings);
}

