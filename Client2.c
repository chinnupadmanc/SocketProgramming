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
int main(int g, char *server_data[])
{
    int sock_id, port_no, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    data_packet s;
    char buffer[256];
    if (g < 3) {
        printf("usage %s hostname port\n", server_data[0]);
        exit(0);
    }
    port_no = atoi(server_data[2]);
    /* Creating a socket */
    sock_id = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_id < 0)
    {
        perror("ERROR opening socket");
        exit(1);
    }
 
  /*Finding the ip address of the server from the hostname */
    server = gethostbyname(server_data[1]);
    if (server == NULL) {
        printf("ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,(char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(port_no);

   /* Connecting to server to receive the structure */
    if (connect(sock_id,&serv_addr,sizeof(serv_addr)) < 0)
    {
         perror("ERROR connecting");
         exit(1);
    }
   /* receiving the structure*/
    n=recv(sock_id, &s,sizeof(s) , 0);
    printf("\nMessage: int-%d, float- %f, char-%c\n",s.i,s.f,s.c);
    if (n < 0)
    {
         perror("ERROR writing to socket");
         exit(1);
    }

   /* Sending Ack to the Server*/
    n=write(sock_id,"The Structure received",22);
    if (n < 0)
    {
        perror("ERROR writing to socket");
        exit(1);
    }
    /* Closing the socket*/
    close(sock_id);
    return 0;
 }
