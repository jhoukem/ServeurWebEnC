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
  if(signal(SIGPIPE,SIG_IGN) == SIG_ERR)
   {
     perror("signal");
   }
 struct sigaction sa;
  sa.sa_handler = traitement_signal;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART;
  if (sigaction(SIGCHLD, &sa ,NULL) == -1)
  {
   perror("sigaction(SIGCHLD)");
  }
}

void traitement_signal(int sig)
{ 
  if (sig == SIGCHLD)
  {
    int status;
    while(waitpid(-1,&status,WNOHANG) > 0){
        if (WIFSIGNALED(status))
        {
          printf("Fils tué par signal %d\n", WTERMSIG(status));
        }
    }
      
  }
  printf("Signal %d reçu\n" ,sig);
}



/**
Main !
**/

int main(int argc,char* argv [])
{

if(strcmp(argv[1], "webserver")== 0){

 printf("%d\n",argc);
 initialiser_signaux();
 int serveur=creer_serveur(8000);
  if(serveur == -1){
  perror("creer serveur"); 
  return -1;
  }

 while(1){
   int socket_client ;
   int f;
   socket_client = accept(serveur, NULL , NULL);
  if (socket_client == -1)/*erreur socket client*/
  {
    perror ("accept");
    return -1;
  }
   f = fork();
  if(f == 0)/*dans le fils on traite le client*/
  {
    traiter_client(socket_client);
   }
  else if(f > 0)/*Dans le père, on ferme la socket*/
  {
   close(socket_client);
  }
  else if(f < 0)/*Erreur*/
  {
   perror("erreur fork negatif");
  } 
 }
} // fin premier if
else
perror("erreur parametre");
exit(0);

return 0;
}

