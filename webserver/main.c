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


#define BUFF_SIZE 1000

int main ()
{

 int serveur=creer_serveur(8000);

 while(1){
 int socket_client ;
 socket_client=accept(serveur, NULL , NULL);
 if ( socket_client == -1)
 {
 perror ("accept");
 return -1;
 }
 
 const char * message_bienvenue ="Bonjour ,Marhaba , Onaya \n Bienvenu sur le serveur web de C\n Ici nous avons à implementer un long message\n au moins dix lignes !!!\n vous rendez vous comptes\n c'est bquasiment impossible\n enfin sauf si on abuse\n des backslash n hehe\n enore un petit dernier pr la route" ;
 sleep(1);
 write(socket_client,message_bienvenue,strlen(message_bienvenue));
/*int taille;
 
 while ((taille = read(socket_client, buffer, BUFF_SIZE))>0)
{
	write(socket_client, buffer, taille);
}*/
 }
return 0;
}
