#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netinet/ip.h>
#include<sys/types.h>
#include <sys/un.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>


#define BUFF_SIZE 1000


int check_first_line(char ligne[])
{
 int nbMots = 1;
 unsigned int i;
 char get[50];
 char reste[50];
 sscanf(ligne,"%s %s",get,reste);
   if(strcmp(get,"GET") != 0)
       return -1;
   for(i = 0; i < strlen(ligne); ++i){
      if(ligne[i] == ' ')
	  nbMots++;
   }

   if(nbMots == 3)
      return 0;	
 return -1;
}

void traiter_client(int socket_client)
{
  
  const char * message_bienvenue ="Bonjour ,Marhaba , Onaya \n Bienvenu sur le serveur web  de C\n Ici nous avons à implementer un long message\n au moins dix lignes !!!\n vous rendez vous compte\n c'est quasiment impossible\n enfin sauf si on abuse\n des backslash n\n encore un petit dernier pr la route\nsdfsfsfjsdkjfskdjfksjfksjdfjs\nsdjhfgskjgfshdgfsdfhsfh\njsdkhfkjsdhfjksdhfjsdhfjsdhfjsdhfjhsjehehfjsehf\nshfsoieuijfsdjfosdujfioseufoijf\n" ;
  sleep(1);
  
  char buffer[BUFF_SIZE];

  const char *mode = "w+";
  FILE *fi = fdopen(socket_client,mode);
  fgets(buffer,BUFF_SIZE,fi);
  if(check_first_line(buffer) == -1){
     printf("Erreur first line\n");
     exit(0);
   }
   write(socket_client,message_bienvenue,strlen(message_bienvenue));
  while (fgets(buffer,BUFF_SIZE,fi) != NULL)   
  {
   
     printf("<Cherokee>%s",buffer);
  }
exit(0);
}

