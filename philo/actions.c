/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mquero <mquero@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 14:44:04 by mquero            #+#    #+#             */
/*   Updated: 2025/01/09 16:08:06 by mquero           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	thinking(t_pdata *pdata, long long ms)
{
	pthread_mutex_lock(pdata->pr);
	ms = get_elapsed_time_ms(pdata->tv);
	if (*pdata->dead)
	{
		pthread_mutex_unlock(pdata->pr);
		return (false);
	}
	printf("%lld ", ms);
	printf("%d is thinking\n", pdata->ph_n);
	pthread_mutex_unlock(pdata->pr);
	return (true);
}

bool	waiting_to_eat(t_pdata *pdata, long long *deathcounter)
{
	int	forks_in_hand;

	forks_in_hand = 0;
	while (forks_in_hand != 2)
	{
		if (death_checker(pdata, deathcounter))
			return (false);
		pthread_mutex_lock(pdata->pr);
		if (pdata->lfork == false)
		{
			pdata->lfork = true;
			forks_in_hand++;
		}
		pthread_mutex_unlock(pdata->pr);
		pthread_mutex_lock(pdata->pr);
		if (*pdata->rfork == false)
		{
			*pdata->rfork = true;
			forks_in_hand++;
		}
		pthread_mutex_unlock(pdata->pr);
	}
	return (true);
}

bool	pr_eat_action(t_pdata *pdata, long long ms)
{
	pthread_mutex_lock(pdata->pr);
	if (*pdata->dead == true)
	{
		pdata->lfork = false;
		*pdata->rfork = false;
		pthread_mutex_unlock(pdata->pr);
		return (false);
	}
	ms = get_elapsed_time_ms(pdata->tv);
	printf("%lld ", ms);
	printf("%d has taken a fork\n", pdata->ph_n);
	printf("%lld ", ms);
	printf("%d has taken a fork\n", pdata->ph_n);
	printf("%lld ", ms);
	printf("%d is eating\n", pdata->ph_n);
	pthread_mutex_unlock(pdata->pr);
	return (true);
}

bool	eat(t_pdata *pdata, long long *deathcounter, int chunks_of_eat)
{
	if (chunks_of_eat <= pdata->timetodie)
	{
		usleep(pdata->timetoeat * 1000);
		if (death_checker(pdata, deathcounter))
		{
			pthread_mutex_unlock(&pdata->left_fork);
			pthread_mutex_unlock(pdata->right_fork);
			return (false);
		}
	}
	else
	{
		while (chunks_of_eat >= 0)
		{
			chunks_of_eat = chunks_of_eat - pdata->timetodie;
			usleep(pdata->timetodie * 1000);
			if (death_checker(pdata, deathcounter))
			{
				pthread_mutex_unlock(&pdata->left_fork);
				pthread_mutex_unlock(pdata->right_fork);
				return (false);
			}
		}
	}
	return (true);
}

bool	sleep_ac(t_pdata *pdata, long long *deathcounter, int chunks_of_sleep)
{
	if (pdata->timetodie > (pdata->timetoeat + pdata->timetosleep))
	{
		usleep(pdata->timetosleep * 1000);
		if (death_checker(pdata, deathcounter))
			return (false);
	}
	else
	{
		while (chunks_of_sleep >= 0)
		{
			chunks_of_sleep = chunks_of_sleep - *deathcounter;
			usleep(*deathcounter * 1000);
			if (death_checker(pdata, deathcounter))
				return (false);
		}
	}
	return (true);
}
