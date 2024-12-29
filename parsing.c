/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mquero <mquero@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 12:30:02 by mquero            #+#    #+#             */
/*   Updated: 2024/12/29 13:30:10 by mquero           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_errors(char **str, int i, int *j)
{
	if (str[i][*j] == '-' || str[i][*j] == '+')
		*j = *j + 1;
	if (((str[i][*j] < '0' || str[i][*j] > '9')) || *j > 11
		|| ft_atoi(str[i]) > INT_MAX || ft_atoi(str[i]) < INT_MIN)
		return (0);
	else
		return (1);
}

int	check_nums_case(char **str)
{
	int	i;
	int	j;

	i = 0;
	while (str[i + 1])
	{
		j = 0;
		while (str[i + 1][j])
		{
			if (check_errors(str, i + 1, &j) == 0)
				return (0);
			j++;
		}
		i++;
	}
	return (1);
}

void	parse(char **args)
{
    if (check_nums_case(args) == 0)
    {
        write(2, "Error\n", 6);
        exit(1);
    }
}
