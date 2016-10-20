#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/types.h>
#include <sys/un.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include "http_request.h"
#include <sys/stat.h>
#include <fcntl.h>


#define BUFF_SIZE 1024

char * fgets_or_exit (char* buffer, int size, FILE * stream)
{	
  while(fgets(buffer,size,stream) != NULL){
    if(strcmp(buffer,"\n") == 0 || strcmp(buffer,"\r\n") == 0)
      break;
    printf("<Cherokee>%s", buffer);
  }
  exit(0);	
}



int parse_http_request(const char *request_line, http_request *request){
  
  int nbMots = 1;
  unsigned int i;
  char arg1[50];
  char arg2[50];
  char arg3[50];
  
  if(strstr(request_line, "../")  != NULL)
    return 2;

  sscanf(request_line, "%s %s %s", arg1, arg2, arg3);
  // printf("arg 1 %s  arg2 %s arg3 %s\n",arg1,arg2,arg3);
  if(strcmp(arg1,"GET") != 0)
    return 0;
  if(strlen(arg3) != 8)
    return 0;
  if(strncmp(arg3,"HTTP/1.",7) != 0)
    return 0;
  // printf("arg3[7]: %d arg3[8]: %d",arg3[7],arg3[8]);
  if(arg3[7] != '0' && arg3[7] != '1')
    return 0;
  for(i = 0; i < strlen(request_line); ++i){
    if(request_line[i] == ' ')
      nbMots++;
  }
  if(nbMots != 3)
    return 0;
  
  request->method = HTTP_GET;
  request->major_version = 1;
  request->minor_version = arg3[7];
  request->url = malloc(strlen(arg2)+1);
  strcpy(request->url, arg2);
  
  
  return 1;
}

void skip_headers(FILE *client){
  char buffer[BUFF_SIZE];
  while (fgets(buffer, BUFF_SIZE, client) != NULL){
    if(strcmp(buffer,"\n") == 0 || strcmp(buffer,"\r\n") == 0)
      break;
  }
}


void send_status(FILE *client, int code, const char *reason_phrase){
  
  fprintf(client, "HTTP/1.1 %d %s\r\n", code, reason_phrase);	
}

void send_response (FILE * client , int code ,const char * reason_phrase  ,const char * message_body){
  send_status(client,code,reason_phrase); 
  fprintf(client, "Content-Length: %d\r\n", (int)strlen(message_body));  
  fprintf(client, "\r\n");
  fprintf(client, "%s", message_body);
  fflush(client);	
}

 int get_file_size(int fd){
  struct stat sb;
  if (fstat(fd, &sb) == -1) {
        return -1;
  }
  return sb.st_size;
 }


int copy(int in, int out){
  char buff[BUFF_SIZE];
  int nb;
  while((nb=read(in,buff,sizeof(buff)))>0){  
    write(out,buff,nb);
  }
  return nb;
}


void send_file(FILE *client, int code, const char * reason_phrase, int fd, char* type){
   send_status(client,code,reason_phrase); 
   fprintf(client, "Content-Length: %d\r\n", get_file_size(fd));  
   fprintf(client, "Content-Type: %s\r\n", type);  
   fprintf(client, "\r\n");
   fflush(client);
   copy(fd, fileno(client));
}




char *rewrite_url(char *url){
  int i;
  int taille = strlen(url);  
  if(taille == 1 && url[0] == '/'){
    return "/index.html";
  }
  for(i=0; i < taille;i++){
    if(url[i] == '?' ){     
      url[i]='\0';   
    }       
  }   
  return url;
}


int check_and_open(const char *url, const char *document_root){

    char ch1[50] = "";
    char ch2[50] = "";
    int ret = -1;
    strcat(ch1, url);
    strcat(ch2, document_root);
    char * chemin = strcat(ch2, ch1);

    struct stat sb;
    if (stat(chemin, &sb) == -1) {
      printf("File not found\n");
      return ret;
    } 
    switch (sb.st_mode & S_IFMT) {
    case S_IFREG: ret = open(chemin, O_RDONLY); printf("file opened\n"); break;
    default: printf("unknown?\n"); break;
    }
    return ret;
}

char * getMimes(char mot[])
{

  char* i = strrchr(mot, '.');
  if(strcmp(i,".html")==0)
    return "text/html";
  if(strcmp(i,".png")==0)
    return "image/png";
  if(strcmp(i,".jpeg")==0)
    return "image/jpeg";

  return i;
}



void traiter_client(int socket_client, const char * dr)
{
  /* const char * message_bienvenue ="Bonjour ,Marhaba , Onaya \n Bienvenu sur le serveur web  de C\n Ici nous avons à implementer un long message\n au moins dix lignes !!!\n vous rendez vous compte\n c'est quasiment impossible\n enfin sauf si on abuse\n des backslash n\n encore un petit dernier pr la route\nsdfsfsfjsdkjfskdjfksjfksjdfjs\nsdjhfgskjgfshdgfsdfhsfh\njsdkhfkjsdhfjksdhfjsdhfjsdhfjsdhfjhsjehehfjsehf\nshfsoieuijfsdjfosdujfioseufoijf\n\r\n" ;*/

  
  char buffer[BUFF_SIZE];
  
  const char *mode = "w+";
  
  int request_ok = 1;
  int fd;	
  FILE *fi = fdopen(socket_client, mode);
  fgets(buffer, BUFF_SIZE, fi);

  /* verification de la requete: GET ... HTTP/1.x */
  http_request requete;
 
  request_ok = parse_http_request(buffer,&requete);
  
  /* Passer les entetes */
  skip_headers(fi);
 
  /*Requete valide*/
  if (!request_ok) {	
    send_response(fi, 400, "Bad Request", "Bad request\r\n");
  }
  else if (requete.method == HTTP_UNSUPPORTED){
    send_response(fi,405,"Method Not Allowed","Method Not Allowed\r\n");
  }	
  else if (request_ok == 2){
    send_response(fi, 403, "Syntax Error ","Syntax Error \r\n");
  }
  else if((fd = check_and_open(rewrite_url(requete.url), dr) ) != -1){
    send_file(fi, 200, "OK", fd, getMimes(rewrite_url(requete.url)));
    //printf("TEST : %d",get_file_size(fd));
    // fileno() return un descripteur ->fichier
    free(requete.url);	
  } else {/*Ressource invalide*/
    send_response(fi,404,"Not Found","Not Found \r\n");
  }
  exit(0);
}




