#include <stdio.h>
#include <string.h>
#include <string.h>
#include "socket.h"
#include "traiter_client.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>



void initialiser_signaux(void)
{
  if(signal(SIGPIPE, SIG_IGN) == SIG_ERR){
    perror("signal");
  }
  struct sigaction sa;
  sa.sa_handler = traitement_signal;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART;
  if (sigaction(SIGCHLD, &sa ,NULL) == -1){
    perror("sigaction(SIGCHLD)");
  }
}

void traitement_signal(int sig)
{ 
  if (sig == SIGCHLD){
    int status;
    while(waitpid(-1, &status, WNOHANG) > 0){
      if (WIFSIGNALED(status)){
	printf("Fils tué par signal %d\n", WTERMSIG(status));
      }
    }
  }
  printf("Signal %d reçu\n" ,sig);
}

int main(int argc, char* argv [])
{

  if(argc < 2){
    printf("Error bad usage: %s [path to the served folder]\n", argv[0]);
    return -1;
  }

  printf("Le serveur sers les fichiers du dossier %s\n", argv[1]);
  printf("Allez sur \"http://localhost:8000\"dans votre navigateur web\n");
  initialiser_signaux();
  int serveur = creer_serveur(8000);
  if(serveur == -1){
    perror("creer serveur"); 
    return -1;
  }
  
  while(1){
    int socket_client;
    socket_client = accept(serveur, NULL , NULL);
    if(socket_client == -1){
      perror ("accept()");
      return -1;
    }
    switch(fork()){
    case -1: perror("fork"); exit(0);
    case 0: traiter_client(socket_client, argv[1]); break;
    default: close(socket_client); break;
    }
  }
  return 0;
}

