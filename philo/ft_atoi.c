/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mquero <mquero@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 14:48:36 by mquero            #+#    #+#             */
/*   Updated: 2024/12/22 13:14:01 by mquero           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_atoi(const char *str)
{
	int		i;
	int		n;
	long	hold;

	i = 0;
	n = 1;
	hold = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i++] == '-')
			n = -1;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		hold = (hold * 10) + (str[i] - '0');
		if (hold < 0 && n == 1)
			return (-1);
		else if (hold < 0 && n == -1)
			return (0);
		i++;
	}
	return ((int)(hold * n));
}
