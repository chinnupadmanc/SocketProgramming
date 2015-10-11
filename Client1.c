#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

typedef struct 
{
 int i;
 float f;
 char c;
}data_packet;

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    data_packet s;
    char buffer[256];
    


    if (argc < 3) {
        fprintf(stderr,"usage %s hostname port\n", argv[0]);
        exit(0);
    }
    portno = atoi(argv[2]);

    /* Create a socket point */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
    {
        perror("ERROR opening socket");
        exit(1);
    }

    server = gethostbyname(argv[1]);

    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,(char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);

   /* Now connect to the server */
    if (connect(sockfd,&serv_addr,sizeof(serv_addr)) < 0) 
    {
         perror("ERROR connecting");
         exit(1);
    }

    printf("\n Client successfully connected to the server.");
    printf("\n Please enter the int data:");
    scanf("%d",&s.i);	
    printf("\n Please enter the float data:");
    scanf("%f",&s.f);	
    printf("\n Please enter the char data:");
    //getc(s.c);
    scanf("%s",&s.c);	
    /* Now ask for a message from the user, this message
    * will be read by server
    */
    //printf("Please enter the message: ");
    //bzero(buffer,256);
    //fgets(buffer,255,stdin);

    /* Send message to the server */
    //n = write(sockfd,s,7);
    n=send(sockfd,(char *) &s,sizeof(s) , 0);
    if (n < 0) 
    {
         perror("ERROR writing to socket");
         exit(1);
    }

    /* Now read server response */
    bzero(buffer,256);
    n = read(sockfd,buffer,255);
    if (n < 0) 
    {
         perror("ERROR reading from socket");
         exit(1);
    }

    printf("%s\n",buffer);

    close(sockfd);
    return 0;
}

