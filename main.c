#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>

#include "mynum.h"
#include "mystring.h"

// Create five buckets for the hash table
struct mynum *head01 = create_num();
struct mynum *head23 = create_num();
struct mynum *head45 = create_num();
struct mynum *head67 = create_num();
struct mynum *head89 = create_num();

// Temp variables for threads
int temp_int = 0;
int temp_com = 0;

// Multi-threads locks
pthread_mutex_t mut;

// A thread
void *thread1( void *ptr)
{
      switch(temp_com){
        case 0:
          printf("result: add %d\n", temp_int);
          // Put numbers into buckets
          switch(temp_int%10){
            case 0:
            case 1:
              add_num(head01, temp_int);
              break;
            case 2:
            case 3:
              add_num(head23, temp_int);
              break;
            case 4:
            case 5:
              add_num(head45, temp_int);
              break;
            case 6:
            case 7:
              add_num(head67, temp_int);
              break;
            case 8:
            case 9:
              add_num(head89, temp_int);
              break;
          }
          break;
        case 3:
          printf("result: delete %d\n", temp_int);
          switch(temp_int%10){
            case 0:
            case 1:
              delete_num(head01, temp_int);
              break;
            case 2:
            case 3:
              delete_num(head23, temp_int);
              break;
            case 4:
            case 5:
              delete_num(head45, temp_int);
              break;
            case 6:
            case 7:
              delete_num(head67, temp_int);
              break;
            case 8:
            case 9:
              delete_num(head89, temp_int);
              break;
          }
          break;
        case 18:
          printf("result: search %d\n", temp_int);
          switch(temp_int%10){
            case 0:
            case 1:
              printf("%d\n", search_num(head01, temp_int));
              break;
            case 2:
            case 3:
              printf("%d\n", search_num(head23, temp_int));
              break;
            case 4:
            case 5:
              printf("%d\n", search_num(head45, temp_int));
              break;
            case 6:
            case 7:
              printf("%d\n", search_num(head67, temp_int));
              break;
            case 8:
            case 9:
              printf("%d\n", search_num(head89, temp_int));
              break;
          }
          break;
  }
  // A thread exits
  pthread_exit(0);
}

// The main procedure: parent and child processes
int main ()
{
  int result = -1, nbytes;
  // From child process to parent process
  int fd[2];
  // From parent process to child process
  // int fd_back[2];
  int result1 = -1;

  pid_t pid;
  char readbuffer[20];
  
  int *write_fd = &fd[1];
  int *read_fd= &fd[0];

  // int *write_fd_back = &fd_back[1];
  // int *read_fd_back = &fd_back[0];

  result = pipe(fd);
  // We may fail to create pipe
  if (-1 == result)
  {
    printf("fail to create pipe\n");
    return -1;
  }
  // result1 = pipe(fd_back);
  // if (-1 == result1)
  // {
  // 	printf("fail to create pipe again\n");
  // 	return -1;
  // }

  pid = fork();
  // We may fail to fork
  if (-1 == pid)
  {
    printf("fail to fork\n");
    return -1;
  }
  if (0 == pid) // Child process
  {
    // Close child read fd
    close(*read_fd);
   
    // Child process gets input  
    while(1)
    {
      // If we use gets, we will get warnings. We use fgets, but the end of the string will be \n
      fgets(readbuffer, 20, stdin);
      printf("the input are: %s", readbuffer);
      // Child writes, parent reads
      write(*write_fd, readbuffer, sizeof(readbuffer));
      // Stop if meets exec
      if(!strcmp("exec\n", readbuffer))
      {
        break;
      }
      // Clear the buffer
      strcpy(readbuffer, "");
    }
    close(*write_fd);
    
    // Receive information from parent process
    // close(*write_fd_back);
    // char receives[20];
    // for(int i=0; i<3; i++)
    // {
    //   read(*read_fd, receives, sizeof(receives));
    //   // Stop if meets over
    //   if(!strcmp("over\n", receives))
    //   {
    //     break;
    //   }
    //   printf("Parent process says: %s\n", receives);
    //   strcpy(receives, "");
    // }
    // close(*read_fd_back);
  }
  else{ // Parent process
    close(*write_fd);
    // Create a chain to keep commands
    struct mystring *command = create_string();
    // Trace the number of user-input commands
    int command_number = 0;
    while(1)
    {
      nbytes=read(*read_fd, readbuffer, sizeof(readbuffer));
      printf("the parent receive %d bytes data: %s\n", nbytes, readbuffer);
      // Stop if meets exec
      if(!strcmp("exec\n", readbuffer))
      {
        break;
      }
      // Keep all these commands
      add_string(command, readbuffer);
      command_number ++;
      // Clean the buffer
      strcpy(readbuffer, "");
    }
    close(*read_fd);
    // Traverse commands, change strings into hash table
    struct mystring * temp_command = command;
    // Create threads
    pthread_t threads[command_number];
    memset(&threads, 0, sizeof(threads));
    pthread_mutex_init(&mut, NULL);
    // Trace the number of current threads
    int current_thread_num = 0;

    while(temp_command->next != NULL)
    {
      printf("The string is %s\n", temp_command->next->str);
      // Parse strings
      char delims[] = " ";
      char *res = NULL;  
      res = strtok(temp_command->next->str, delims);
      res = strtok(NULL, delims);
      temp_int = atoi(res);
      temp_com = temp_command->next->str[0]-'a';
      // Create threads to do the tasks
      pthread_create(&threads[current_thread_num], NULL, thread1, (void *) NULL);
      // Wait for threads to finish
      pthread_join(threads[current_thread_num], NULL);
      current_thread_num++;
      // Next command
      temp_command = temp_command->next;    
    }
    printf("--bucket 1---\n");
    traverse_num(head01);
    printf("--bucket 2---\n");
    traverse_num(head23);
    printf("--bucket 3---\n");
    traverse_num(head45);
    printf("--bucket 4---\n");
    traverse_num(head67);
    printf("--bucket 5---\n");
    traverse_num(head89);

    // Send results to the child process
   //  close(*read_fd_back);
   //  // head01
   //  struct mynum * send_to_child = head01;
   //  char change_into[25];
  	// while(send_to_child->next != NULL)
   //  {
   //    printf("hihihi\n");
   //    sprintf(change_into, "%d", send_to_child->next->num);
   //    write(*write_fd_back, change_into, sizeof(change_into));
   //    send_to_child = send_to_child->next;    
  	// }
  	// // head23
   //  send_to_child = head23;
  	// while(send_to_child->next != NULL)
   //  {
   //    sprintf(change_into, "%d", send_to_child->next->num);
   //    write(*write_fd_back, change_into, sizeof(change_into));
   //    send_to_child = send_to_child->next;    
  	// }
   //  // head45
   //  send_to_child = head45;
  	// while(send_to_child->next != NULL)
   //  {
   //    sprintf(change_into, "%d", send_to_child->next->num);
   //    write(*write_fd_back, change_into, sizeof(change_into));
   //    send_to_child = send_to_child->next;     
  	// }
  	// // head67
   //  send_to_child = head67;
  	// while(send_to_child->next != NULL)
   //  {
   //    sprintf(change_into, "%d", send_to_child->next->num);
   //    write(*write_fd_back, change_into, sizeof(change_into));
   //    send_to_child = send_to_child->next;     
  	// }
  	// // head89
   //  send_to_child = head89;
  	// while(send_to_child->next != NULL)
   //  {
   //    sprintf(change_into, "%d", send_to_child->next->num);
   //    write(*write_fd_back, change_into, sizeof(change_into));
   //    send_to_child = send_to_child->next;    
  	// }
  	// write(*write_fd_back, "over\n", sizeof("over\n"));
   //  close(*write_fd_back);
  }
  return 0;
}