#include <stdio.h>
#include <string.h>
#include <string.h>
#include "socket.h"
#include<sys/socket.h>
#include<netinet/in.h>
#include<netinet/ip.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>




int main ()
{
 initialiser_signaux();
 int serveur=creer_serveur(8000);
  if(serveur == -1){
  perror("creer serveur"); 
  return -1;
  }

 while(1){
   int socket_client ;
   int f;
   socket_client=accept(serveur, NULL , NULL);
   f = fork();
  if(f==0)
  {
    traiter_client(socket_client);
   }
  else if(f > 0)
  {

   close(socket_client);
  }
  else if(f<0)
  {
   perror("erreur fork negatif");
  }
  if ( socket_client == -1)
  {
   perror ("accept");
   return -1;
  }
 
 }
return 0;
}

