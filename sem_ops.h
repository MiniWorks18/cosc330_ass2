/* 	sem_ops.h	-	simplified semaphore operations
	in, 'Advanced UNIX Programming', M.J. Rochkind
*/

/*	semtran		will return the semaphore id (system wide) of the
			semaphore number you give. If no semaphore has been
			established for this number, one is created	*/
int semtran (int key);

/*	semcall		applies the operation to the semaphore */
void semcall (int sid, int op);

/*	P		the semaphore signal operation. sid must be the system
			wide semaphore number returned by semtran above	*/
void P (int sid);

/*	V		the semaphore release operation. sid must be the system 
			wide semaphore number returned by semtran above. */
void V (int sid);

/*	rm_sem		remove a semaphore from the system. sid must be the
			system wide semaphore returned from semtran	*/
void rm_sem (int sid);
