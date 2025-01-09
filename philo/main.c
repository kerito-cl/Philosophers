/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mquero <mquero@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 09:36:16 by mquero            #+#    #+#             */
/*   Updated: 2025/01/09 16:09:20 by mquero           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char **argv)
{
	pthread_t		*philo;
	t_pdata			*pdata;
	pthread_mutex_t	pr;
	bool			death;

	if (argc > 6 || argc < 5)
		return (0);
	parse(argv);
	pdata = malloc(sizeof(t_pdata) * ft_atoi(argv[1]) + 1);
	if (pdata == NULL)
		return (1);
	pdata->pcount = ft_atoi(argv[1]);
	pthread_mutex_init(&pr, NULL);
	init_philo(pdata, &pr, argv, argc);
	philo = malloc(pdata->pcount * sizeof(pthread_t) + 1);
	if (philo == NULL)
	{
		free(pdata);
		return (1);
	}
	death = false;
	create_thread(philo, pdata, &death);
	pthread_mutex_destroy(&pr);
	return (0);
}
