/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mquero <mquero@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 13:19:39 by mquero            #+#    #+#             */
/*   Updated: 2025/01/08 17:03:25 by mquero           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <stdbool.h>
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/wait.h>
# include <pthread.h>
# include <unistd.h>
#include <sys/time.h>


typedef struct s_pdata
{
    pthread_mutex_t *flag_mutex;
    pthread_mutex_t forks;
    pthread_mutex_t *pr;
    unsigned int    *queue;
    struct timeval tv;
    struct timeval checkifdead;
    int ph_n;
    int pcount;
    int index;
    int next;
    int prev;
    bool    *dead;
    bool    flag;
    bool    thinking;
    unsigned int    timetosleep;
    unsigned int    timetoeat;
    unsigned int    timetodie;
    long long    deathcounter;

}			t_pdata;

char		*ft_strjoin(char const *s1, char const *s2);
void		freesplit(char **strs);
char		**ft_split(char const *s, char c);
size_t		ft_strlcpy(char *dst, const char *src, size_t size);
char		*ft_strdup(const char *s);
int	        ft_atoi(const char *str);
void	parse(char **args);

#endif