// Nordvik Suspension Bridge solution using threads and semaphores
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include "sem_ops.h"
#include <string.h>
#define E_CARS 5
#define W_CARS 5
#define E_TRUCKS 3
#define W_TRUCKS 2
#define MAXWAIT 20
#define CROSSINGTIME 4

int e_counter, w_counter = 0; // Initialise counters
int e_sem, w_sem, bridge_sem; // Declare semaphores

/* Struct of a Vehicle and the properties it can hold
    int counter - The counter integer for how many vehicles are on the bridge
    int semaphore - The mutex semaphore providing exclusive access to 
                    the counter
    int id - The id of this particular car
    car dir - The direction that the car is travelling (e.g 'E' or 'W')
*/
typedef struct
{
    int *counter, semaphore, id;
    char dir;
} Vehicle;

void *carfunc(void *arg) // Entry point for east bound car threads
{
    sleep(rand() % MAXWAIT); // Delay rand amount of time

    Vehicle c = *((Vehicle *)arg);
    int sem = c.semaphore;
    int *counter = c.counter;

    P(sem); // Lock counter
    *counter += 1;
    if (*counter == 1) // Lock bridge if first car
        P(bridge_sem);
    printf("Hi, I'm car %d entering the bridge heading %c. Beep Beep!\n",
           c.id, c.dir);
    V(sem);              // Unlock counter
    sleep(CROSSINGTIME); // Drive
    P(sem);              // Lock counter
    *counter -= 1;
    printf("Ok, I'm car %d exiting the bridge heading %c. Thanks!\n",
           c.id, c.dir);
    if (*counter == 0) // Unlock bridge if last car
        V(bridge_sem);
    V(sem);
    pthread_exit(NULL);
}

void *truckfunc(void *arg) // Entry point for truck threads
{
    sleep(rand() % MAXWAIT);

    Vehicle t = *((Vehicle *)arg);

    P(bridge_sem); // Lock bridge for exclusive access
    printf("Hi, I'm truck %d enering the bridge heading %c.\n", t.id, t.dir);
    sleep(CROSSINGTIME); // Drive
    printf("Ok, I'm truck %d exiting the bridge heading %c.\n", t.id, t.dir);
    V(bridge_sem); // Unlock bridge
    pthread_exit(NULL);
}

void cleanup(void) // Remove semaphores after completion
{
    rm_sem(e_sem);
    rm_sem(w_sem);
    rm_sem(bridge_sem);
}

int main(void)
{
    int i;
    pthread_t tid[E_CARS + W_CARS];
    pthread_t ttruckid[E_TRUCKS + W_TRUCKS];

    // Vehcile array declaration
    Vehicle cars[E_CARS + W_CARS];
    Vehicle trucks[E_TRUCKS + W_TRUCKS];

    // Initialise semaphores
    e_sem = semtran(IPC_PRIVATE);
    w_sem = semtran(IPC_PRIVATE);
    bridge_sem = semtran(IPC_PRIVATE);

    // Initialise to 1 (binary semaphores)
    V(e_sem);
    V(w_sem);
    V(bridge_sem);

    // Loop for each vehicle, initialise properties
    for (i = 0; i < E_CARS + W_CARS; i++)
    {
        cars[i].id = i;
        if (i >= E_CARS) // If cars above the E_CARS index
        {
            cars[i].dir = 'W';
            cars[i].counter = &w_counter;
            cars[i].semaphore = w_sem;
        }
        else
        {
            cars[i].dir = 'E';
            cars[i].counter = &e_counter;
            cars[i].semaphore = e_sem;
        }
    }
    for (i = 0; i < E_TRUCKS + W_TRUCKS; i++)
    {
        trucks[i].id = i;
        if (i >= E_TRUCKS)
            trucks[i].dir = 'W';
        else
            trucks[i].dir = 'E';
    }

    // Create pthreads for each vehicle
    for (i = 0; i < E_CARS + W_CARS; i++)
    {
        if (i >= E_CARS)
            pthread_create(&tid[i], NULL, carfunc, (void *)&cars[i]);
        else
            pthread_create(&tid[i], NULL, carfunc, (void *)&cars[i]);
    }
    for (i = 0; i < E_TRUCKS + W_TRUCKS; i++)
        pthread_create(&ttruckid[i], NULL, truckfunc, (void *)&trucks[i]);

    // Join to each thread and wait until each have exited
    for (i = 0; i < E_CARS + W_CARS; i++)
    {
        if (i >= E_CARS)
            pthread_join(tid[i], NULL);
        else
            pthread_join(tid[i], NULL);
    }
    for (i = 0; i < E_TRUCKS + W_TRUCKS; i++)
        pthread_join(ttruckid[i], NULL);

    // Cleanup semaphores and exit
    cleanup();
    return 0;
}