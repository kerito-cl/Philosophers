/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_time.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mquero <mquero@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 15:02:34 by mquero            #+#    #+#             */
/*   Updated: 2025/01/09 16:08:58 by mquero           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long long	get_elapsed_time_ms(struct timeval start_time)
{
	struct timeval	current_time;
	long long		elapsed_microseconds;
	long long		elapsed_seconds;

	gettimeofday(&current_time, NULL);
	elapsed_seconds = current_time.tv_sec - start_time.tv_sec;
	elapsed_microseconds = current_time.tv_usec - start_time.tv_usec;
	return ((elapsed_seconds * 1000) + (elapsed_microseconds / 1000));
}

bool	death_checker(t_pdata *pdata, long long *deathcounter)
{
	long long	ms_death;
	long long	ms;

	ms_death = get_elapsed_time_ms(pdata->checkifdead);
	*deathcounter = pdata->timetodie;
	*deathcounter = *deathcounter - ms_death;
	if (*deathcounter <= 0)
	{
		ms = get_elapsed_time_ms(pdata->tv);
		pthread_mutex_lock(pdata->pr);
		if (*pdata->dead == false)
		{
			*pdata->dead = true;
			printf("%lld ", ms);
			printf("%d died\n", pdata->ph_n);
		}
		pthread_mutex_unlock(pdata->pr);
		return (true);
	}
	return (false);
}
