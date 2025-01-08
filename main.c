/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mquero <mquero@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 09:36:16 by mquero            #+#    #+#             */
/*   Updated: 2025/01/08 17:31:01 by mquero           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long long get_elapsed_time_ms(struct timeval start_time) 
{
    struct timeval current_time;
    long long elapsed_microseconds;
    long long elapsed_seconds;
    
    gettimeofday(&current_time, NULL);
    elapsed_seconds = current_time.tv_sec - start_time.tv_sec;
    elapsed_microseconds = current_time.tv_usec - start_time.tv_usec;
    return (elapsed_seconds * 1000) + (elapsed_microseconds / 1000);
}


void death_checker(t_pdata *pdata, long long *deathcounter) 
{
    long long ms_death;
    long long ms;

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
            exit(1);
        }
        pthread_mutex_unlock(pdata->pr);
    }
}

void    pr_eat_action(t_pdata *pdata, long long ms)
{
        pthread_mutex_lock(pdata->pr);
        pthread_mutex_lock(pdata->flag_mutex);
        pdata->thinking = false;
        pthread_mutex_unlock(pdata->flag_mutex);
        pdata->thinking = false;
        ms = get_elapsed_time_ms(pdata->tv);
        printf("%lld ", ms);
        printf("%d has taken a fork\n", pdata->ph_n);
        printf("%lld ", ms);
        printf("%d has taken a fork\n", pdata->ph_n);
        printf("%lld ", ms);
        printf("%d is eating\n", pdata->ph_n);
        pthread_mutex_unlock(pdata->pr);
}

void eat(t_pdata *pdata, long long ms, long long *deathcounter, unsigned int queue)
{
    int     chunks_of_sleep;
    int     chunks_of_eat;

    chunks_of_sleep = pdata->timetosleep;
    chunks_of_eat = pdata->timetoeat;
    
    pthread_mutex_lock(pdata->flag_mutex);
    while(pdata[pdata->next].thinking || pdata[pdata->prev].thinking)
    {
        
        pthread_mutex_unlock(pdata->flag_mutex);
        death_checker(pdata, deathcounter);
        pthread_mutex_lock(pdata->flag_mutex);
    }
    pthread_mutex_unlock(pdata->flag_mutex);

    pthread_mutex_lock(pdata->flag_mutex);
    while(pdata[pdata->next].flag || pdata[pdata->prev].flag)
    {
        
        pthread_mutex_unlock(pdata->flag_mutex);
        death_checker(pdata, deathcounter);
        pthread_mutex_lock(pdata->flag_mutex);
    }
    pthread_mutex_unlock(pdata->flag_mutex);
    
    pthread_mutex_lock(pdata->flag_mutex);
    pdata->flag = true;
    pthread_mutex_unlock(pdata->flag_mutex);


    pthread_mutex_lock(&pdata->forks);
    pthread_mutex_lock(&pdata[pdata->next].forks);

    pr_eat_action(pdata, ms);

    ms = get_elapsed_time_ms(pdata->tv);
    gettimeofday(&pdata->checkifdead, NULL);
    if (chunks_of_eat <= pdata->timetodie)
    {
        usleep(pdata->timetoeat * 1000);
        death_checker(pdata, deathcounter);
    }
    else
    {
        while (chunks_of_eat >= 0)
        {
            chunks_of_eat = chunks_of_eat - pdata->timetodie;
            usleep(pdata->timetodie * 1000);
            death_checker(pdata, deathcounter);
        }
    }

    pthread_mutex_lock(pdata->flag_mutex);
    pdata->flag = false;
    pthread_mutex_unlock(pdata->flag_mutex);

    pthread_mutex_lock(pdata->pr);
    ms = get_elapsed_time_ms(pdata->tv);
    printf("%lld ", ms);
    printf("%d is sleeping\n", pdata->ph_n);
    pthread_mutex_unlock(pdata->pr);
    pthread_mutex_unlock(&pdata[pdata->next].forks);
    pthread_mutex_unlock(&pdata->forks);

    if (pdata->timetodie > (pdata->timetoeat + pdata->timetosleep))
            usleep(pdata->timetosleep * 1000);
    else
    {
        while (chunks_of_sleep >= 0)
        {
            chunks_of_sleep = chunks_of_sleep - *deathcounter;
            usleep(*deathcounter * 1000);
            death_checker(pdata, deathcounter);
        }
    }
}


void* start_thread(void* arg) 
{
    t_pdata* pdata = (t_pdata*)arg;
    long long ms;
    bool    flag;
    long long deathcounter;
    unsigned int     queue;

    deathcounter = pdata->timetodie;
    queue = 0;
    gettimeofday(&pdata->tv, NULL);
    gettimeofday(&pdata->checkifdead, NULL);
    while (true)
    {
        ms = get_elapsed_time_ms(pdata->tv);
        pthread_mutex_lock(pdata->pr);
        printf("%lld ", ms);
        printf("%d is thinking\n", pdata->ph_n);
        pthread_mutex_unlock(pdata->pr);
        if (pdata->ph_n % 2 == 1)
            eat(pdata, ms, &deathcounter, queue);
        else
        {
            usleep(200);
            eat(pdata, ms, &deathcounter, queue);
        }
        pdata->thinking = true;
    }
    return NULL;
}

void init_philo(t_pdata *pdata, bool *death, pthread_mutex_t *pr, char **argv)
{
    int i;
    int counter;

    i = 0;
    counter = pdata->pcount;
    while (counter > 0)
    {
        pthread_mutex_init(&pdata[i].forks, NULL);
        pdata[i].pr = pr;
        pdata[i].ph_n = i + 1;
        pdata[i].next = 1;
        pdata[i].prev = -1;
        pdata[i].dead = death;
        pdata[i].flag = false;
        pdata[i].thinking = false;
        pdata[i].timetosleep = ft_atoi(argv[4]);
        pdata[i].timetoeat = ft_atoi(argv[3]);
        pdata[i].timetodie = ft_atoi(argv[2]);
        i++;
        counter--;
    }
    pdata[0].prev = i - 1;
    pdata[i - 1].next = 1 - i;
}

int main(int argc, char **argv) 
{
    pthread_t *philo;
    t_pdata *pdata;
    pthread_mutex_t pr;
    pthread_mutex_t flag;
    pthread_mutex_t unlock_flag;
    bool    death;
    unsigned int queue;
    int i;
    int pcount;

    if (argc > 6 || argc < 5)
        return (0);
    parse(argv);
    pcount = ft_atoi(argv[1]);
    pdata = malloc(sizeof(t_pdata) * pcount);
    pdata->pcount = pcount;
    pthread_mutex_init(&pr, NULL);
    pthread_mutex_init(&flag, NULL);
    init_philo(pdata, &death, &pr, argv);
    philo = malloc(pcount * sizeof(pthread_t) + 1);
    queue = 0;

    death = false;
    i = 0; 
    while (i < pcount)
    {
        pdata[i].flag_mutex = &flag;
        pdata[i].queue = &queue;
        pthread_create(&philo[i], NULL, start_thread, pdata + i);
        i++;
    }
    i = 0;
    while (i < pcount)
    {
        pthread_join(philo[i], NULL);
        i++;
    }
    i = 0;
    while (i < pcount)
    {
        pthread_mutex_destroy(&pdata[i].forks);
        i++;
    }
    pthread_mutex_destroy(&pr);
    //pthread_mutex_destroy(&pdata->lfork);
    return 0;
}
