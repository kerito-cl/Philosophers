/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mquero <mquero@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 09:36:16 by mquero            #+#    #+#             */
/*   Updated: 2025/01/09 09:47:03 by mquero           ###   ########.fr       */
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


bool death_checker(t_pdata *pdata, long long *deathcounter) 
{
    long long ms_death;
    long long ms;

    ms_death = get_elapsed_time_ms(pdata->checkifdead);
    *deathcounter = pdata->timetodie;
    *deathcounter = *deathcounter - ms_death;
    if (*deathcounter == 0)
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
        return(true);
    }
    return (false);
}

bool    waiting_to_eat(t_pdata *pdata, long long *deathcounter)
{
    int     forks_in_hand;
    
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
bool    pr_eat_action(t_pdata *pdata, long long ms)
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

bool    eat(t_pdata *pdata, long long *deathcounter, int chunks_of_eat)
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
bool    sleep_ac(t_pdata *pdata, long long *deathcounter, int chunks_of_sleep)
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
bool eat_and_sleep(t_pdata *pdata, long long ms, long long *deathcounter)
{
    int     chunks_of_sleep;
    int     chunks_of_eat;

    chunks_of_sleep = pdata->timetosleep;
    chunks_of_eat = pdata->timetoeat;

    if (!waiting_to_eat(pdata, deathcounter))
        return (false);
    pthread_mutex_lock(&pdata->left_fork);
    pthread_mutex_lock(pdata->right_fork);
    if (!pr_eat_action(pdata, ms))
    {
        pthread_mutex_unlock(&pdata->left_fork);
        pthread_mutex_unlock(pdata->right_fork);
        return (false);
    }
    ms = get_elapsed_time_ms(pdata->tv);
    gettimeofday(&pdata->checkifdead, NULL);
    if (!eat(pdata, deathcounter, chunks_of_eat))
        return (false);
    pdata->eating_counter--;
    pthread_mutex_lock(pdata->pr);
    pdata->lfork = false;
    *pdata->rfork = false;
    pthread_mutex_unlock(pdata->pr);

    pthread_mutex_lock(pdata->pr);
    if (pdata->eating_counter == 0 || *pdata->dead)
    {
        pthread_mutex_unlock(&pdata->left_fork);
        pthread_mutex_unlock(pdata->right_fork);
        pthread_mutex_unlock(pdata->pr);
        return (false);
    }
    ms = get_elapsed_time_ms(pdata->tv);
    printf("%lld ", ms);
    printf("%d is sleeping\n", pdata->ph_n);
    pthread_mutex_unlock(pdata->pr);
    pthread_mutex_unlock(&pdata->left_fork);
    pthread_mutex_unlock(pdata->right_fork);

    if (!sleep_ac(pdata, deathcounter, chunks_of_sleep))
        return (false);
    return (true);
}


void* start_thread(void* arg) 
{
    t_pdata* pdata = (t_pdata*)arg;
    long long ms;
    long long deathcounter;

    deathcounter = pdata->timetodie;
    gettimeofday(&pdata->tv, NULL);
    gettimeofday(&pdata->checkifdead, NULL);
    while (true)
    {
        pthread_mutex_lock(pdata->pr);
        ms = get_elapsed_time_ms(pdata->tv);
        if (*pdata->dead)
        {
            pthread_mutex_unlock(pdata->pr);
            return (NULL);
        }
        printf("%lld ", ms);
        printf("%d is thinking\n", pdata->ph_n);
        pthread_mutex_unlock(pdata->pr);
        if (pdata->ph_n % 2 == 1)
        {
            if (!eat_and_sleep(pdata, ms, &deathcounter))
                return (NULL);
        }
        else
        {
            usleep(500);
            if (!eat_and_sleep(pdata, ms, &deathcounter))
                return (NULL);
        }
    }
    return NULL;
}
void    create_thread(pthread_t *philo ,t_pdata *pdata, bool *death)
{
    int i;

    i = 0;
    while (i < pdata->pcount)
    {
        pdata[i].dead = death;
        pthread_create(&philo[i], NULL, start_thread, pdata + i);
        i++;
    }
    i = 0;
    while (i < pdata->pcount)
    {
        pthread_join(philo[i], NULL);
        i++;
    }
    i = 0;
    while (i < pdata->pcount)
    {
        pthread_mutex_destroy(&pdata[i].left_fork);
        i++;
    }
    free(pdata);
    free(philo);
}
void init_philo(t_pdata *pdata, pthread_mutex_t *pr, char **argv, int argc)
{
    int i;
    int counter;

    i = 0;
    counter = pdata->pcount;
    while (counter > 0)
    {
        if (argc == 6)
            pdata[i].eating_counter = ft_atoi(argv[5]);
        else
            pdata[i].eating_counter = 100000;
        pthread_mutex_init(&pdata[i].left_fork, NULL);
        pdata[i].right_fork = &pdata[i + 1].left_fork;
        pdata[i].lfork = false;
        pdata[i].rfork = &pdata[i + 1].lfork;
        pdata[i].pr = pr;
        pdata[i].ph_n = i + 1;
        pdata[i].timetosleep = ft_atoi(argv[4]);
        pdata[i].timetoeat = ft_atoi(argv[3]);
        pdata[i].timetodie = ft_atoi(argv[2]);
        i++;
        counter--;
    }
    pdata[i - 1].rfork = &pdata[0].lfork;
    pdata[i - 1].right_fork = &pdata[0].left_fork;
}

int main(int argc, char **argv) 
{
    pthread_t *philo;
    t_pdata *pdata;
    pthread_mutex_t pr;
    bool    death;

    if (argc > 6 || argc < 5)
        return (0);
    parse(argv);
    pdata = malloc(sizeof(t_pdata) * ft_atoi(argv[1]) + 1);
    if (pdata == NULL)
        return (1);
    pdata->pcount = ft_atoi(argv[1]);;
    pthread_mutex_init(&pr, NULL);
    init_philo(pdata, &pr, argv, argc);
    philo = malloc(pdata->pcount * sizeof(pthread_t) + 1);
    if (philo == NULL)
        return (1);
    death = false;
    create_thread(philo, pdata, &death);
    pthread_mutex_destroy(&pr);
    return 0;
}
