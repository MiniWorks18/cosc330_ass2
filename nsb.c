<<<<<<< HEAD
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

typedef struct
{
    int *counter, semaphore, id;
    char dir;
} Vehicle;

// Entry point for east bound car threads
void *eastcar(void *arg)
{
    /* 
Car function should act similarly to the reader function
- Lock EC
- Increment EC
- If EC == 1
    - Lock bridge
- Unlock EC
- Drive (sleep(DRIVE_TIME))
- Lock EC
- Decrement EC
- If EC == 0
    - Unlock bridge
- Unlock EC
*/

    sleep(rand() % MAXWAIT);

    Vehicle c = *((Vehicle *)arg);
    int sem = c.semaphore;
    int *counter = c.counter;

    P(sem);
    *counter += 1;
    if (*counter == 1)
        P(bridge_sem);
    printf("Hi, I'm car %d entering the bridge heading %c. Beep Beep!\n", c.id, c.dir);
    V(sem);
    sleep(CROSSINGTIME);
    P(sem);
    *counter -= 1;
    printf("Ok, I'm car %d exiting the bridge heading %c. Thanks!\n", c.id, c.dir);
    if (*counter == 0)
        V(bridge_sem);
    V(sem);
    pthread_exit(NULL);
}

// Entry point for truck threads
void *truckfunc(void *arg)
{
    /*
- Lock bridge
- Drive
- Unlock bridge
*/
    sleep(rand() % MAXWAIT);
    Vehicle t = *((Vehicle *)arg);
    // TODO: Implment this function
    P(bridge_sem);
    printf("Hi, I'm truck %d enering the bridge heading %c.\n", t.id, t.dir);
    sleep(CROSSINGTIME);
    printf("Ok, I'm truck %d exiting the bridge heading %c.\n", t.id, t.dir);
    V(bridge_sem);

    pthread_exit(NULL);
}

void cleanup(void)
{
    rm_sem(e_sem);
    rm_sem(w_sem);
    rm_sem(bridge_sem);
}

int main(void)
{
    int i;
    // pthread_t e_tid[E_CARS];
    // pthread_t w_tid[W_CARS];
    pthread_t tid[E_CARS + W_CARS];
    pthread_t ttruckid[E_TRUCKS + W_TRUCKS];
    // int truckid[E_TRUCKS + W_TRUCKS];

    // Car e_cars[E_CARS];
    // Car w_cars[W_CARS];

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

    // TODO: Figure out why this doesn't let me use cars[] why it gives segmentation faults

    // for (i = 0; i < E_CARS; i++)
    // {
    //     e_cars[i].id = i;
    //     e_cars[i].dir = 'E';
    //     e_cars[i].counter = &e_counter;
    //     e_cars[i].semaphore = e_sem;
    // }
    // for (i = 0; i < W_CARS; i++)
    // {
    //     // if (i >= E_CARS)
    //     // {
    //     w_cars[i].id = i;
    //     w_cars[i].dir = 'W';
    //     w_cars[i].counter = &w_counter;
    //     w_cars[i].semaphore = w_sem;
    //     // }
    //     // else
    //     // {
    //     // }
    // }
    // for (i = 0; i < TRUCKS; i++)
    //     truckid[i] = i;

    // for (i = 0; i < E_CARS; i++)
    //     pthread_create(&e_tid[i], NULL, eastcar, (void *)&e_cars[i]);

    // for (i = 0; i < W_CARS; i++)
    // {
    //     // if (i >= E_CARS)
    //     // {
    //     pthread_create(&w_tid[i], NULL, eastcar, (void *)&w_cars[i]);
    //     // }
    //     // else
    //     // {
    //     // pthread_create(&e_tid[i], NULL, eastcar, (void *)&cars[i]);
    //     // }
    // }
    // for (i = 0; i < TRUCKS; i++)
    //     pthread_create(&ttruckid[i], NULL, truckfunc, (void *)&truckid[i]);

    // for (i = 0; i < E_CARS; i++)
    //     pthread_join(e_tid[i], NULL);

    // for (i = 0; i < W_CARS; i++)
    // {
    //     // if (i >= E_CARS)
    //     // {
    //     pthread_join(w_tid[i], NULL);
    //     // }
    //     // else
    //     // {
    //     //     pthread_join(e_tid[i], NULL);
    //     // }
    // }

    // for (i = 0; i < E_CARS; i++)
    // {
    //     cars[i].id = i;
    //     cars[i].dir = 'E';
    //     cars[i].counter = &e_counter;
    //     cars[i].semaphore = e_sem;
    // }
    for (i = 0; i < E_CARS + W_CARS; i++)
    {
        cars[i].id = i;
        if (i >= E_CARS)
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

    for (i = 0; i < E_CARS + W_CARS; i++)
    {
        if (i >= E_CARS)
            pthread_create(&tid[i], NULL, eastcar, (void *)&cars[i]);
        else
            pthread_create(&tid[i], NULL, eastcar, (void *)&cars[i]);
    }
    for (i = 0; i < E_TRUCKS + W_TRUCKS; i++)
        pthread_create(&ttruckid[i], NULL, truckfunc, (void *)&trucks[i]);

    for (i = 0; i < E_CARS + W_CARS; i++)
    {
        if (i >= E_CARS)
            pthread_join(tid[i], NULL);
        else
            pthread_join(tid[i], NULL);
    }

    for (i = 0; i < E_TRUCKS + W_TRUCKS; i++)
        pthread_join(ttruckid[i], NULL);
    cleanup();
    return 0;
=======
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

typedef struct
{
    int *counter, semaphore, id;
    char dir;
} Vehicle;

// Entry point for east bound car threads
void *eastcar(void *arg)
{
    /* 
Car function should act similarly to the reader function
- Lock EC
- Increment EC
- If EC == 1
    - Lock bridge
- Unlock EC
- Drive (sleep(DRIVE_TIME))
- Lock EC
- Decrement EC
- If EC == 0
    - Unlock bridge
- Unlock EC
*/

    sleep(rand() % MAXWAIT);

    Vehicle c = *((Vehicle *)arg);
    int sem = c.semaphore;
    int *counter = c.counter;

    P(sem);
    *counter += 1;
    if (*counter == 1)
        P(bridge_sem);
    printf("Hi, I'm car %d entering the bridge heading %c. Beep Beep!\n", c.id, c.dir);
    V(sem);
    sleep(CROSSINGTIME);
    P(sem);
    *counter -= 1;
    printf("Ok, I'm car %d exiting the bridge heading %c. Thanks!\n", c.id, c.dir);
    if (*counter == 0)
        V(bridge_sem);
    V(sem);
    pthread_exit(NULL);
}

// Entry point for truck threads
void *truckfunc(void *arg)
{
    /*
- Lock bridge
- Drive
- Unlock bridge
*/
    sleep(rand() % MAXWAIT);
    Vehicle t = *((Vehicle *)arg);
    // TODO: Implment this function
    P(bridge_sem);
    printf("Hi, I'm truck %d enering the bridge heading %c.\n", t.id, t.dir);
    sleep(CROSSINGTIME);
    printf("Ok, I'm truck %d exiting the bridge heading %c.\n", t.id, t.dir);
    V(bridge_sem);

    pthread_exit(NULL);
}

void cleanup(void)
{
    rm_sem(e_sem);
    rm_sem(w_sem);
    rm_sem(bridge_sem);
}

int main(void)
{
    int i;
    // pthread_t e_tid[E_CARS];
    // pthread_t w_tid[W_CARS];
    pthread_t tid[E_CARS + W_CARS];
    pthread_t ttruckid[E_TRUCKS + W_TRUCKS];
    // int truckid[E_TRUCKS + W_TRUCKS];

    // Car e_cars[E_CARS];
    // Car w_cars[W_CARS];

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

    // TODO: Figure out why this doesn't let me use cars[] why it gives segmentation faults

    // for (i = 0; i < E_CARS; i++)
    // {
    //     e_cars[i].id = i;
    //     e_cars[i].dir = 'E';
    //     e_cars[i].counter = &e_counter;
    //     e_cars[i].semaphore = e_sem;
    // }
    // for (i = 0; i < W_CARS; i++)
    // {
    //     // if (i >= E_CARS)
    //     // {
    //     w_cars[i].id = i;
    //     w_cars[i].dir = 'W';
    //     w_cars[i].counter = &w_counter;
    //     w_cars[i].semaphore = w_sem;
    //     // }
    //     // else
    //     // {
    //     // }
    // }
    // for (i = 0; i < TRUCKS; i++)
    //     truckid[i] = i;

    // for (i = 0; i < E_CARS; i++)
    //     pthread_create(&e_tid[i], NULL, eastcar, (void *)&e_cars[i]);

    // for (i = 0; i < W_CARS; i++)
    // {
    //     // if (i >= E_CARS)
    //     // {
    //     pthread_create(&w_tid[i], NULL, eastcar, (void *)&w_cars[i]);
    //     // }
    //     // else
    //     // {
    //     // pthread_create(&e_tid[i], NULL, eastcar, (void *)&cars[i]);
    //     // }
    // }
    // for (i = 0; i < TRUCKS; i++)
    //     pthread_create(&ttruckid[i], NULL, truckfunc, (void *)&truckid[i]);

    // for (i = 0; i < E_CARS; i++)
    //     pthread_join(e_tid[i], NULL);

    // for (i = 0; i < W_CARS; i++)
    // {
    //     // if (i >= E_CARS)
    //     // {
    //     pthread_join(w_tid[i], NULL);
    //     // }
    //     // else
    //     // {
    //     //     pthread_join(e_tid[i], NULL);
    //     // }
    // }

    // for (i = 0; i < E_CARS; i++)
    // {
    //     cars[i].id = i;
    //     cars[i].dir = 'E';
    //     cars[i].counter = &e_counter;
    //     cars[i].semaphore = e_sem;
    // }
    for (i = 0; i < E_CARS + W_CARS; i++)
    {
        cars[i].id = i;
        if (i >= E_CARS)
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

    for (i = 0; i < E_CARS + W_CARS; i++)
    {
        if (i >= E_CARS)
            pthread_create(&tid[i], NULL, eastcar, (void *)&cars[i]);
        else
            pthread_create(&tid[i], NULL, eastcar, (void *)&cars[i]);
    }
    for (i = 0; i < E_TRUCKS + W_TRUCKS; i++)
        pthread_create(&ttruckid[i], NULL, truckfunc, (void *)&trucks[i]);

    for (i = 0; i < E_CARS + W_CARS; i++)
    {
        if (i >= E_CARS)
            pthread_join(tid[i], NULL);
        else
            pthread_join(tid[i], NULL);
    }

    for (i = 0; i < E_TRUCKS + W_TRUCKS; i++)
        pthread_join(ttruckid[i], NULL);
    cleanup();
    return 0;
>>>>>>> d347a232651367e903cc952c73a097b60d0de136
}