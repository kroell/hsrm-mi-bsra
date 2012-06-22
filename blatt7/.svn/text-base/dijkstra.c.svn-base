/**
 * @brief Dijkstrasche P/V-Operationen auf Basis von Unix Semaphoren
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#include "inc/dijkstra.h"

#if defined(__GNU_LIBRARY__) && !defined(_SEM_SEMUN_UNDEFINED)
/* union semun is defined by including <sys/sem.h> */
#else
/* according to X/OPEN we have to define it ourselves */
union semun {
      int val;                  /* value for SETVAL */
      struct semid_ds *buf;     /* buffer for IPC_STAT, IPC_SET */
      unsigned short *array;    /* array for GETALL, SETALL */
                                /* Linux specific part: */
      struct seminfo *__buf;    /* buffer for IPC_INFO */
};
#endif

static int semid = -1;
 
/**
 * @brief Initialisieren
 * @param num Anzahl benoetigter Semaphore
 */
int PVinit(int num, int ivals[])
{
	key_t mykey;
	union semun arg;
	int i;
	/**
	 * Erzeuge eindeutigen Key
	 */
	if((mykey = ftok("/tmp", 'f')) == -1)
	{
		perror("ftok()");
		return(-1);
	}
	debug(("semkey: %d bzw 0x%x\n", mykey,mykey));
	/**
	 * Versuche, semaphore-set zu oeffnen
	 */
	if((semid = semget(mykey, num, 0666)) == -1)
	{
		if(errno == ENOENT)
		{	/** Semaphore-set existiert noch nicht: erzeugen! */
			if((semid = semget(mykey, num, IPC_CREAT|IPC_EXCL|0666)) == -1)
			{
				if(errno == EEXIST)
				{	/** what the ... ?! Erst existiert er nicht, dann doch ?! */
					fprintf(stderr, "Semaphore deleted while opening. Please retry!\n");
				}
				perror("semget()");
				return(-1);
			}
			else
			{	/** semaphore neu initialisieren */
				arg.val = 0;
				for(i = 0; i < num; i++)
				{
					arg.val = ivals[i];
					debug(("PVinit ivals[i]: %d\n", ivals[i]));
					if (semctl(semid, i, SETVAL, arg) == -1)
					{
						perror("semctl()");
						return(-1);
					}
				}
			}
		}
		else
		{
			perror("semget()");
			return(-1);
		}
	}
	return(0);
}

/* void semterminate()
  {
  	int rc;
 	if((rc = semctl(semid, 0, IPC_RMID)) == -1)
 	{
 		perror("semctl()");
 		exit(1);
 	}
  }
*/

/**
 * @brief Dijkstra's P() operator
 */
void P(int sem)
{
	struct sembuf op;
	op.sem_num = sem;
	op.sem_op  = -1;
	op.sem_flg = 0;
	if(semop(semid, &op, 1) == -1)
	{
		perror("semop()");
		exit(1);
	}
}

/**
 * @brief Dijkstra's V() operator
 */
void V(int sem)
{
	struct sembuf op;
	op.sem_num = sem;
	op.sem_op  = 1;
	op.sem_flg = 0;
	if(semop(semid, &op, 1) == -1)
	{
		perror("semop()");
		exit(1);
	}
}

/**
 * @brief Zaehlerstand eines Semaphors ermitteln
 */
int Stand(int sem)
{
	int rc;
	if((rc = semctl(semid, sem, GETVAL)) == -1)
	{
		perror("semctl()");
		exit(1);
	}
	return(rc);
}

