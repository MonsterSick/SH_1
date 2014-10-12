/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_toupper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdiouf <mdiouf@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2013/11/21 16:31:23 by mdiouf            #+#    #+#             */
/*   Updated: 2014/09/28 21:38:57 by mdiouf           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/libft.h"

int	ft_toupper(int c)
{
	if (c <= 'z' && c >= 'a')
	{
		c = c - 32;
	}
	return (c);
}