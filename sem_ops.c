/* 	sem_ops.c	-	simplified semaphore operations
	in, 'Advanced UNIX Programming', M.J. Rochkind
    Review sem_ops.h for instructions.
*/  
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "sem_ops.h"
  
semtran (int key) 
  
    
    {
      
      
    
  



semcall (int sid, int op) 
  
  
  
  
    
    {
      
      
    



P (int sid) 

V (int sid) 

  rm_sem (int sid) 
      
	
	
	