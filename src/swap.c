/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   swap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tanguy <tanguy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/08/28 19:41:24 by tanguy            #+#    #+#             */
/*   Updated: 2016/08/28 19:56:37 by tanguy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

/*
 * Byte swap unsigned short
*/
uint16_t	swap_uint16(uint16_t val)
{
	return (val << 8) | (val >> 8 );
}

/*
 * Byte swap short
*/
int16_t		swap_int16(int16_t val)
{
	return (val << 8) | ((val >> 8) & 0xFF);
}

/*
 * Byte swap unsigned int
*/
uint32_t	swap_uint32(uint32_t val)
{
	val = ((val << 8) & 0xFF00FF00 ) | ((val >> 8) & 0xFF00FF ); 
	return (val << 16) | (val >> 16);
}

/*
 * Byte swap int
*/
int32_t		swap_int32(int32_t val)
{
	val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF ); 
	return (val << 16) | ((val >> 16) & 0xFFFF);
}
