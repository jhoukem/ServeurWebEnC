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

 sscanf(ligne,"%s %s %s",arg1,arg2,arg3);
 printf("arg 1 %s , arg2 %s arg3 %s\n",arg1,arg2,arg3);
   if(strcmp(arg1,"GET") != 0)
       return -1;
  if(strlen(arg3) != 8)
       return -1;

  if(strncmp(arg3,"HTTP/1.",7) != 0)
       return -1;

  printf("arg3[7]: %d arg3[8]: %d",arg3[7],arg3[8]);
  if(arg3[7] != '0' && arg3[7] != '1')
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
	int request_ok = 1;
	FILE *fi = fdopen(socket_client,mode);
	fgets(buffer,BUFF_SIZE,fi);
	/* verification de la requete: GET ... HTTP/1.x */
	if(check_first_line(buffer) == -1){
		request_ok = 0;
	}

	/* Passer les entetes */
	while (fgets(buffer,BUFF_SIZE,fi) != NULL)   
	{
		if(strcmp(buffer,"\n") == 0 || strcmp(buffer,"\r\n") == 0)
			break;
		printf("<Cherokee>%s",buffer);
	}
	if (request_ok)
	{
		fprintf(fi, "HTTP/1.1 200 OK\r\n");
		fprintf(fi, "Content-Length: %d\r\n", (int)strlen(message_bienvenue));
		fprintf(fi, "\r\n");
		fprintf(fi, "%s", message_bienvenue);
		fflush(fi);
	}
	else
	{
		const char * msg = "400 Bad request\n";
		fprintf(fi, "HTTP/1.1 400 Bad request\r\n");
		fprintf(fi, "Content-Length: %d\r\n", (int)strlen(msg));
		fprintf(fi, "\r\n");
		fprintf(fi, "%s", msg);
		fflush(fi);		
	}	
	exit(0);
}

/*void send_response (FILE * client , int code ,const char * message_cours ,const char * message_long ){

}*/

