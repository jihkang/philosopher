#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>

typedef struct s_data
{
	pthread_mutex_t	*mtx;
	pthread_t		pid;
	int				idx;
	int				nxt;
	char			*name;
	struct timeval	st;
	int				alive;
}t_data;

void	philo_die(t_data *d)
{
	d->alive = 0;
}

void	*p_func(void *data)
{
	t_data	*d;
	int		idx;
	int		tmp;
	int		nxt;
	int		res;
	int		cnt;
	struct timeval	tval;

	cnt = 0;
	d = (t_data *)data;
	idx = d->idx;
	nxt = d->nxt;
	res = 0;
	while (cnt < 2 && d->alive)
	{
		res = pthread_mutex_lock(&(d->mtx[idx])) == 0 && \
			  pthread_mutex_lock(&(d->mtx[nxt])) == 0;
		if (res)
		{
			gettimeofday(&(d->st), NULL);
			printf("%d philo eat %d times\n", idx, cnt + 1);
			printf("start time%d\n", d->st.tv_usec);
			usleep(5000);
			cnt++;
		}
		pthread_mutex_unlock(&(d->mtx[idx]));
		pthread_mutex_unlock(&(d->mtx[nxt]));
	}
	gettimeofday(&tval, NULL);
	printf("cannot lock cur time : %d", tval.tv_usec);
	return (NULL);
}

int	main(void)
{
	pthread_mutex_t	m[10];
	t_data			d[10];
	for(int i = 0; i < 10; i++)
		pthread_mutex_init(&m[i], NULL);
	for(int i = 0; i < 10; i++)
	{
		d[i].idx = i;
		d[i].nxt = (i + 1) % 10;
		d[i].mtx = m;
		pthread_create(&(d[i].pid), NULL, p_func, &d[i]);
	}
	for(int i = 0; i < 10; i++)
	{
		pthread_join(d[i].pid, (void **)NULL);
	}
	return (0);
}
