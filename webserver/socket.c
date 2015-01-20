#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netinet/ip.h>
#include<sys/types.h>
#include <sys/un.h>
#include <string.h>



int creer_serveur (int port)
{
  
  int socket_serveur ;
  socket_serveur = socket(AF_INET,SOCK_STREAM,0);
  //socket(SERVEUR, PF_INET, SOCK_STREAM, getprotobyname('http'));
  //bind(SERVEUR, $adresse)
  if (socket_serveur == -1)
    {
      perror ("socket_serveur");
      return -1;
    }
  struct sockaddr_in saddr;
  saddr.sin_family=AF_INET; /* Socket ipv4 */
  saddr.sin_port=htons(port); /* Port d ’ écoute */
  saddr.sin_addr.s_addr=INADDR_ANY; /* écoute sur toutes les interfaces */
  if (bind(socket_serveur,(struct sockaddr *) &saddr , sizeof(saddr)) == -1)
    {
      perror ( "bind socker_serveur");
      return -1;
      /* traitement de l ’ erreur */
    }

  if(listen(socket_serveur,10) == -1)
    {
      perror("listen socket_serveur");
      return -1;
      /* traitement d ’ erreur */
    }

  return socket_serveur;
}




