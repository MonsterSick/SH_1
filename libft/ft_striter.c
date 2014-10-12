/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striter.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdiouf <mdiouf@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2013/11/27 23:46:45 by mdiouf            #+#    #+#             */
/*   Updated: 2014/09/28 21:34:39 by mdiouf           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/libft.h"

void	ft_striter(char *s, void (*f)(char *))
{
	int	i;

	i = 0;
	while (s[i] != '\0')
	{
		f(&s[i]);
		i++;
	}
}