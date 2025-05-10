#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include <stdbool.h>
#include <string.h>

typedef struct s_philo {
    int             id;
    int             meals_eaten;
    long long       last_meal_time;
    pthread_mutex_t *left_fork;
    pthread_mutex_t *right_fork;
    pthread_mutex_t *print_mutex;
    pthread_mutex_t *death_mutex;
    bool            *someone_died;
    int             num_philosophers;
    int             time_to_die;
    int             time_to_eat;
    int             time_to_sleep;
    int             must_eat_count;
    long long       start_time;
} t_philo;

typedef struct s_program {
    t_philo         *philosophers;
    pthread_mutex_t *forks;
    pthread_mutex_t print_mutex;
    pthread_mutex_t death_mutex;
    bool            someone_died;
    int             num_philosophers;
    int             time_to_die;
    int             time_to_eat;
    int             time_to_sleep;
    int             must_eat_count;
    long long       start_time;
} t_program;

// Get current time in milliseconds
long long get_time_ms(void) {
    struct timeval tv;
    
    gettimeofday(&tv, NULL);
    return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

// Print status with timestamp
void print_status(t_philo *philo, char *status) {
    long long current_time;
    
    pthread_mutex_lock(philo->death_mutex);
    if (*philo->someone_died) {
        pthread_mutex_unlock(philo->death_mutex);
        return;
    }
    
    current_time = get_time_ms() - philo->start_time;
    
    pthread_mutex_lock(philo->print_mutex);
    printf("%lld %d %s\n", current_time, philo->id + 1, status);
    pthread_mutex_unlock(philo->print_mutex);
    
    pthread_mutex_unlock(philo->death_mutex);
}

// Sleep for a specific amount of time in milliseconds
void precise_sleep(long long sleep_time) {
    long long start;
    long long current;
    
    start = get_time_ms();
    while (1) {
        current = get_time_ms();
        if (current - start >= sleep_time)
            break;
        usleep(100);
    }
}

// Check if a philosopher has died
void *death_monitor(void *arg) {
    t_philo *philo;
    long long current_time;
    
    philo = (t_philo *)arg;
    while (1) {
        pthread_mutex_lock(philo->death_mutex);
        current_time = get_time_ms();
        
        if (*philo->someone_died) {
            pthread_mutex_unlock(philo->death_mutex);
            break;
        }
        
        if (philo->last_meal_time > 0 && 
            current_time - philo->last_meal_time > philo->time_to_die) {
            *philo->someone_died = true;
            pthread_mutex_lock(philo->print_mutex);
            printf("%lld %d died\n", current_time - philo->start_time, philo->id + 1);
            pthread_mutex_unlock(philo->print_mutex);
            pthread_mutex_unlock(philo->death_mutex);
            break;
        }
        pthread_mutex_unlock(philo->death_mutex);
        usleep(1000);  // Check death condition every 1ms
    }
    return NULL;
}

// Philosopher routine
void *philosopher_routine(void *arg) {
    t_philo *philo;
    pthread_t death_thread;
    
    philo = (t_philo *)arg;
    philo->last_meal_time = get_time_ms();
    
    // Special case for single philosopher
    if (philo->num_philosophers == 1) {
        print_status(philo, "has taken a fork");
        precise_sleep(philo->time_to_die);
        return NULL;
    }
    
    // Create death monitor thread
    if (pthread_create(&death_thread, NULL, death_monitor, philo) != 0) {
        printf("Error creating death monitor thread\n");
        return NULL;
    }
    pthread_detach(death_thread);
    
    // Even philosophers start by taking right fork, odd take left fork
    // This helps prevent deadlock
    if (philo->id % 2 == 0) {
        usleep(100);  // Small delay to prevent deadlock
    }
    
    while (1) {
        // Check if someone died or all meals eaten
        pthread_mutex_lock(philo->death_mutex);
        if (*philo->someone_died || 
            (philo->must_eat_count > 0 && philo->meals_eaten >= philo->must_eat_count)) {
            pthread_mutex_unlock(philo->death_mutex);
            break;
        }
        pthread_mutex_unlock(philo->death_mutex);
        
        // Take forks
        pthread_mutex_lock(philo->left_fork);
        print_status(philo, "has taken a fork");
        pthread_mutex_lock(philo->right_fork);
        print_status(philo, "has taken a fork");
        
        // Eat
        print_status(philo, "is eating");
        pthread_mutex_lock(philo->death_mutex);
        philo->last_meal_time = get_time_ms();
        philo->meals_eaten++;
        pthread_mutex_unlock(philo->death_mutex);
        precise_sleep(philo->time_to_eat);
        
        // Release forks
        pthread_mutex_unlock(philo->right_fork);
        pthread_mutex_unlock(philo->left_fork);
        
        // Sleep
        print_status(philo, "is sleeping");
        precise_sleep(philo->time_to_sleep);
        
        // Think
        print_status(philo, "is thinking");
    }
    
    return NULL;
}

// Initialize program based on arguments
int init_program(t_program *program, int argc, char **argv) {
    if (argc < 5 || argc > 6) {
        printf("Usage: %s number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]\n", argv[0]);
        return 1;
    }
    
    program->num_philosophers = atoi(argv[1]);
    program->time_to_die = atoi(argv[2]);
    program->time_to_eat = atoi(argv[3]);
    program->time_to_sleep = atoi(argv[4]);
    program->must_eat_count = (argc == 6) ? atoi(argv[5]) : -1;
    program->someone_died = false;
    
    if (program->num_philosophers <= 0 || program->time_to_die <= 0 ||
        program->time_to_eat <= 0 || program->time_to_sleep <= 0 ||
        (argc == 6 && program->must_eat_count <= 0)) {
        printf("Invalid arguments\n");
        return 1;
    }
    
    return 0;
}

// Initialize mutexes
int init_mutexes(t_program *program) {
    int i;
    
    program->forks = malloc(sizeof(pthread_mutex_t) * program->num_philosophers);
    if (!program->forks)
        return 1;
        
    for (i = 0; i < program->num_philosophers; i++) {
        if (pthread_mutex_init(&program->forks[i], NULL) != 0) {
            printf("Error initializing fork mutex\n");
            return 1;
        }
    }
    
    if (pthread_mutex_init(&program->print_mutex, NULL) != 0 ||
        pthread_mutex_init(&program->death_mutex, NULL) != 0) {
        printf("Error initializing mutexes\n");
        return 1;
    }
    
    return 0;
}

// Initialize philosophers
int init_philosophers(t_program *program) {
    int i;
    
    program->philosophers = malloc(sizeof(t_philo) * program->num_philosophers);
    if (!program->philosophers)
        return 1;
        
    program->start_time = get_time_ms();
    
    for (i = 0; i < program->num_philosophers; i++) {
        program->philosophers[i].id = i;
        program->philosophers[i].meals_eaten = 0;
        program->philosophers[i].last_meal_time = program->start_time;
        program->philosophers[i].left_fork = &program->forks[i];
        program->philosophers[i].right_fork = &program->forks[(i + 1) % program->num_philosophers];
        program->philosophers[i].print_mutex = &program->print_mutex;
        program->philosophers[i].death_mutex = &program->death_mutex;
        program->philosophers[i].someone_died = &program->someone_died;
        program->philosophers[i].num_philosophers = program->num_philosophers;
        program->philosophers[i].time_to_die = program->time_to_die;
        program->philosophers[i].time_to_eat = program->time_to_eat;
        program->philosophers[i].time_to_sleep = program->time_to_sleep;
        program->philosophers[i].must_eat_count = program->must_eat_count;
        program->philosophers[i].start_time = program->start_time;
    }
    
    return 0;
}

// Start philosopher threads
int start_simulation(t_program *program) {
    int i;
    pthread_t *threads;
    
    threads = malloc(sizeof(pthread_t) * program->num_philosophers);
    if (!threads)
        return 1;
        
    for (i = 0; i < program->num_philosophers; i++) {
        if (pthread_create(&threads[i], NULL, philosopher_routine, &program->philosophers[i]) != 0) {
            printf("Error creating philosopher thread\n");
            return 1;
        }
    }
    
    // Wait for all philosophers to finish
    for (i = 0; i < program->num_philosophers; i++) {
        pthread_join(threads[i], NULL);
    }
    
    free(threads);
    return 0;
}

// Cleanup resources
void cleanup(t_program *program) {
    int i;
    
    // Destroy mutexes
    for (i = 0; i < program->num_philosophers; i++) {
        pthread_mutex_destroy(&program->forks[i]);
    }
    pthread_mutex_destroy(&program->print_mutex);
    pthread_mutex_destroy(&program->death_mutex);
    
    // Free allocated memory
    free(program->forks);
    free(program->philosophers);
}

int main(int argc, char **argv) {
    t_program program;
    
    memset(&program, 0, sizeof(t_program));
    
    if (init_program(&program, argc, argv) != 0)
        return 1;
        
    if (init_mutexes(&program) != 0)
        return 1;
        
    if (init_philosophers(&program) != 0)
        return 1;
        
    if (start_simulation(&program) != 0)
        return 1;
        
    cleanup(&program);
    return 0;
}