/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mquero <mquero@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 09:36:16 by mquero            #+#    #+#             */
/*   Updated: 2024/12/22 14:17:05 by mquero           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void* incrementCounter(void* arg) 
{
    t_pdata* pdata = (t_pdata*)arg;
    int i;

    i = 0;
    while (pdata->forks >= 2)
    {
        pthread_mutex_lock(&pdata->mutex);
        pdata->forks -= 2;
        pthread_mutex_unlock(&pdata->mutex);
    }
    return NULL;
}

int main(int argc, char **argv) 
{
    pthread_t *philo;
    t_pdata pdata;
    struct timeval tv;

    int i;

    if (argc < 2)
        return (0);
    pthread_mutex_init(&pdata.mutex, NULL);
    pdata.forks = ft_atoi(argv[1]);
    philo = malloc(pdata.forks * sizeof(pthread_t));
    i = 0; 
    while (i < pdata.forks)
    {
        pthread_create(&philo[i], NULL, incrementCounter, &pdata);
        i++;
    }
    i = 0;
    while (i < pdata.forks)
    {
        pthread_join(philo[i], NULL);
        i++;
    }
    if (gettimeofday(&tv, NULL) == 0) {
        printf("Seconds since Epoch: %ld\n", tv.tv_sec);
        printf("Microseconds: %ld\n", tv.tv_usec);
    } else {
        perror("gettimeofday failed");
    }
    pthread_mutex_destroy(&pdata.mutex);
    return 0;
}
