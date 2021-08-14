/* 	sem_ops.c	-	simplified semaphore operations
	in, 'Advanced UNIX Programming', M.J. Rochkind
    Review sem_ops.h for instructions.
*/  
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "sem_ops.h"
  int
semtran (int key)  { int sid;
   if ((sid = semget ((key_t) key, 1, 0666 | IPC_CREAT)) == -1)
    
    {
      printf ("Failure creating semaphore, errno: %d\n", errno);
      exit (EXIT_FAILURE);
    }
  return (sid);
}

 void
semcall (int sid, int op)  { struct sembuf sb;
   sb.sem_num = 0;
  sb.sem_op = op;
  sb.sem_flg = 0;
  if (semop (sid, &sb, 1) == -1)
    
    {
      printf ("Failure locking/unlocking semaphore, errno:%d\n", errno);
      exit (EXIT_FAILURE);
    }
}

 void
P (int sid)  { semcall (sid, -1);
}  void
V (int sid)  { semcall (sid, 1);
}  void
  rm_sem (int sid)  { if (semctl (sid, 0, IPC_RMID, 0) == -1)
      {printf ("Failure removing semaphore, errno: %d\n", errno);
	exit (EXIT_FAILURE);
	}
	}
