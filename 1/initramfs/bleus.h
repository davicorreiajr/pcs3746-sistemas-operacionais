#define _GNU_SOURCE
#include <unistd.h>
#include <sys/syscall.h>

#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

int mysem; //ID for a semaphore set, like file handler for a file.
#define KEY 1100 //can be any number

long bleus() {
  create_sem(1);
  init_sem(1);

  if (fork()) {
    while(1) {
      printf("1\n");

      P(0);
      printf("2\n");
      sleep(1);
      printf("3\n");
      sleep(1);
      printf("4\n");
      V(0);

      printf("5\n");
    }
  } else {
    while(1) {
      printf("A\n");

      P(0);
      printf("B\n");
      sleep(1);
      printf("C\n");
      sleep(1);
      printf("D\n");
      V(0);

      printf("E\n");
    }
  }
}

//create a semaphore set with N semaphores using a KEY.
void create_sem(int N) {
  printf("create %d semaphores\n", N);
  mysem = semget(KEY, N, 0666 | IPC_CREAT);
  if(mysem < 0) printf("error create_sem");
} 

//initialize all semaphores value=1
void init_sem(int N) {
  int j;
  int retval;
  union semun
  {
  int val;
  struct semid_ds *buf;
  ushort *array;
  } arg;
  arg.val = 1;
  
  for (j=0; j<N; j++) {
    retval = semctl(mysem, j, SETVAL, arg);
    if(retval < 0) printf("error init_sem: semctl: %d\n", j);
    show_sem(j);
  }
}

// if semaphore value is 0, wait; otherwise, decrement semaphore value.
void P(int i) {
  PV(i, -1);
}

// if any waiters, wake one; otherwise, increment semaphore value.
void V(int i) {
  PV(i, 1);
} 

//P and V operations using semop()
void PV(int i, int act) {
  struct sembuf op;
  int retval;

  op.sem_num = i;
  op.sem_op = act; //1=V, -1=P
  op.sem_flg = 0; //will wait
  retval = semop(mysem, &op, 1);
  if(retval != 0) printf("error PV: semop: %d\n", act);
  show_sem(i);
}

void show_sem(int i) {
  int val;

  val = get_sem(i);
  printf("semaphore[%d]=%d\n", i, val);
}

//get the i-th semaphore value in a semaphore set mysem.
int get_sem(int i) {
  ushort semarr[30];
  union semun
  {
  int val;
  struct semid_ds *buf;
  ushort *array;
  } arg;

  arg.array = semarr;
  semctl(mysem, i, GETALL, arg);
  return semarr[i];
}
