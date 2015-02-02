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
 char arg1[50];
 char arg2[50];
 char arg3[50];
 char http[10];
 sscanf(ligne,"%s %s %s",arg1,arg2,arg3);
 printf("arg 1 %s , arg2 %s arg3 %s\n",arg1,arg2,arg3);
   if(strcmp(arg1,"GET") != 0)
       return -1;
  if(strlen(arg3) != 8)
       return -1;
  strncat(http,arg3,6);  	
  if(strcmp(http,"HTTP/1") != 0)
       return -1;
	printf("arg3[7]: %d arg3[8]: %d",arg3[7],arg3[8]);
  if(arg3[7] != 49 || (arg3[8] != 0 && arg3[8] != 1))
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
  char buffer2[BUFF_SIZE*5];

  const char *mode = "w+";
  FILE *fi = fdopen(socket_client,mode);
 /* fgets(buffer,BUFF_SIZE,fi);

  if(check_first_line(buffer) == -1){
     printf("Erreur first line\n");
     exit(0);
   }*/
   write(socket_client,message_bienvenue,strlen(message_bienvenue));
  while (fgets(buffer,BUFF_SIZE,fi) != NULL)   
  {
   if(strcmp(buffer,"\n") == 0 || strcmp(buffer,"\r\n") == 0)
     printf("<Cherokee>%s",buffer2);
   else{
     strcat(buffer2,buffer);
     const char * msg = "Chaque message doit se terminer par '\\r\\n'\n";
     write(socket_client,msg,strlen(msg));
   }
     
  }
exit(0);
}

