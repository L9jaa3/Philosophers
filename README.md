# Dining Philosophers

## Project Overview

This project implements the classic Dining Philosophers problem - a fundamental computer science problem that demonstrates synchronization issues and techniques in concurrent programming.

The problem consists of N philosophers sitting around a circular table with N forks. Each philosopher needs two forks to eat (the ones to their immediate left and right). The challenge is to design a solution where no philosopher starves, while avoiding deadlocks and resource conflicts.

![Dining Philosophers Illustration](https://upload.wikimedia.org/wikipedia/commons/7/7b/An_illustration_of_the_dining_philosophers_problem.png)

## How to Use

### Compilation

```bash
make
```

### Running the Program

```bash
./philo [number_of_philosophers] [time_to_die] [time_to_eat] [time_to_sleep] [number_of_times_each_philosopher_must_eat]
```

- `number_of_philosophers`: The number of philosophers (and forks)
- `time_to_die`: Time in milliseconds after which a philosopher dies if they haven't started eating
- `time_to_eat`: Time in milliseconds it takes for a philosopher to eat
- `time_to_sleep`: Time in milliseconds a philosopher spends sleeping
- `number_of_times_each_philosopher_must_eat`: (Optional) If all philosophers eat this many times, the program stops

### Examples

```bash
# 5 philosophers, 800ms to die, 200ms to eat, 200ms to sleep, no meal limit
./philo 5 800 200 200

# 4 philosophers, 410ms to die, 200ms to eat, 200ms to sleep, must each eat 5 times
./philo 4 410 200 200 5
```

## Implementation Details

### Core Data Structures

#### Philosopher Structure (`t_philo`)
```c
typedef struct s_philo
{
    int         philo_id;
    pthread_t   thread;
    long        meals_nb;
    bool        it_is_full;
    time_t      last_meal;
    int         right_fork;
    int         left_fork;
    struct s_global *args;
} t_philo;
```

#### Global Data Structure (`t_global`)
```c
typedef struct s_global
{
    long                philosophers_nb;
    time_t              time_to_die;
    time_t              time_to_eat;
    time_t              time_to_sleep;
    int                 must_eat;
    time_t              start;
    pthread_mutex_t     meal;
    pthread_mutex_t     print;
    pthread_mutex_t     status;
    pthread_mutex_t     forks[200];
    t_philo             philosophers[200];
    bool                simulation_end;
} t_global;
```

### Concurrency Model

- Each philosopher is simulated by a separate thread
- Forks are represented by mutexes
- Additional mutexes protect shared data:
  - `meal`: Protects meal counters and last meal timestamps
  - `print`: Ensures atomic printing operations
  - `status`: Guards the simulation status flag

### Philosopher Lifecycle

1. **Initialization**: Each philosopher is assigned an ID and references to their left and right forks
2. **Thinking**: Initial state where philosopher is waiting for forks
3. **Taking Forks**:
   - Even-numbered philosophers grab right fork first
   - Odd-numbered philosophers have a small delay to prevent deadlocks
4. **Eating**: When a philosopher has two forks, they eat for `time_to_eat` milliseconds
5. **Sleeping**: After eating, philosopher sleeps for `time_to_sleep` milliseconds
6. **Repeat**: The cycle continues until simulation ends

### Simulation Monitoring

A monitoring system regularly checks:
1. If any philosopher has gone too long without eating (starvation)
2. If all philosophers have eaten the required number of times (optional termination condition)

The simulation ends when either condition is met.

## Key Files

- `main.c`: Program entry point, argument parsing, initialization
- `philosophers.c`: Core philosopher routine and thread creation
- `monitoring.c`: Death and meal completion monitoring
- `utils.c`: Utility functions (time, display, conversion)
- `protections.c`: Thread and mutex safety wrappers
- `mutex_funs.c`: Mutex initialization and cleanup
- `helpers.c`: Helper functions for simulation state
- `philosophers.h`: Header containing structures and function declarations

## Thread Synchronization Techniques

### Deadlock Prevention

This implementation uses several methods to prevent deadlocks:
1. **Asymmetric approach**: Even-numbered philosophers take right fork first, odd ones take left fork first
2. **Staggered start**: Odd-numbered philosophers wait briefly before starting
3. **Resource hierarchy**: Resources are acquired in a consistent order
4. **Status checking**: Threads check if simulation has ended before locking resources

### Mutex Error Handling

Custom wrappers around pthread functions provide detailed error messages and consistent error handling:

```c
void ft_mutex_error_handler(pthread_mutex_t *mutex, t_mutex_codes opcode)
{
    // Handles all mutex operations with appropriate error checking
}

void ft_thread_error_handler(pthread_t *thread, void *(*routine)(void *),
    void *arg, t_thread_code opcode)
{
    // Handles thread operations with error checking
}
```

## Common Issues & Solutions

### Deadlocks
- **Problem**: Circular waiting for resources leads to system-wide halt
- **Solution**: Implemented staggered start times and asymmetric fork acquisition

### Race Conditions
- **Problem**: Unpredictable results from concurrent access to shared data
- **Solution**: All shared resources protected by appropriate mutexes

### Timing Accuracy
- **Problem**: Standard sleep functions are not precise enough
- **Solution**: Custom `ft_usleep` function with active waiting for short durations

### Resource Cleanup
- **Problem**: Potential memory and resource leaks
- **Solution**: Careful tracking and freeing of all allocated resources

## Performance Considerations

- Busy-waiting is used sparingly and with small sleep intervals
- Thread creation overhead is minimized by creating only necessary threads
- Mutex operations are optimized to reduce contention

## Learning Outcomes

This project demonstrates:
- Thread creation and synchronization
- Mutex usage for protecting shared resources
- Deadlock prevention techniques
- Race condition avoidance
- Concurrent program design
- Resource allocation in multi-threaded environments

## References

1. Dijkstra, E. W. (1971). "Hierarchical ordering of sequential processes". Acta Informatica. 1 (2): 115–138.
2. Silberschatz, A., Galvin, P. B., and Gagne, G. (2018). "Operating System Concepts" (10th ed.). Wiley.
3. POSIX Threads Programming: https://computing.llnl.gov/tutorials/pthreads/
