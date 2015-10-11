#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <math.h>
struct data_packet
{
	int i;
	float f;
	char c;
};

int main( int argc, char *argv[] )
{
    int socket_id,socket_id1, new_sock_id,new_sock_id1, portno,portno1, clilen,ibuff;
    char buffer[256];
    struct sockaddr_in serv_addr, client_addr,client_addr1;
    int  n;
    char *ptr1,*ptr2;
    char temp[4];
    char tmp;
    float fbuff;
    struct data_packet s,s1;

    /* creating a Socket */
    socket_id = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_id < 0) 
    {
        perror("ERROR opening socket");
        exit(1);
    }
    /* Clearing the address structure and initialising with port no,address family */
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
 
    /* The Socket id and host ip address is binded together with bind function*/
    if (bind(socket_id, (struct sockaddr *) &serv_addr,
                          sizeof(serv_addr)) < 0)
    {
         perror("ERROR on binding");
         exit(1);
    }

    /* Listening*/
    listen(socket_id,5);
    clilen = sizeof(client_addr);

   /* Accept the connection request from the client */
    new_sock_id = accept(socket_id, (struct sockaddr *)&client_addr, 
                                &clilen);
    if (new_sock_id < 0) 
    {
        perror("ERROR on accept");
        exit(1);
    }
    /* If connection is established then start communicating */
    bzero(buffer,256);
    n=recv(new_sock_id, &s, sizeof(s), 0);
    if (n < 0)
    {
        perror("ERROR reading from socket");
        exit(1);
    }
    ptr1=(char *) &s.f;
    ptr2=(char *) &fbuff;
    *(ptr2+3)=*(ptr1);
    *(ptr2+2)=*(ptr1+1);
    *(ptr2+1)=*(ptr1+2);
    *(ptr2)=*(ptr1+3);
    ptr1=(char *) &s.i;
    ptr2=(char *) &ibuff;
    *(ptr2+3)=*(ptr1);
    *(ptr2+2)=*(ptr1+1);
    *(ptr2+1)=*(ptr1+2);
    *(ptr2)=*(ptr1+3);


    printf("\n Received Structure :int- %d, float- %f, char- %c \n",ibuff,fbuff,s.c);

    /* Acknowledging the client */
    n = write(new_sock_id,"The Structure received",18);
    if (n < 0)
    {
        perror("ERROR writing to socket");
        exit(1);
    }
    s1.i=ibuff*2;
    s1.f=fbuff+1;
    s1.c= (++s.c ==123) ? 'a':s.c;
    s1.c= s1.c ==91 ? 'A':s1.c;
    printf("\n Modified Structure: int- %d,float- %f,char- %c \n",s1.i,s1.f,s1.c);
    close(new_sock_id);
    close(socket_id);
   

    /* New Socket created to send the data to Client 2*/
    socket_id1 = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_id1 < 0) 
    {
        perror("ERROR opening socket");
        exit(1);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = atoi(argv[2]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if (bind(socket_id1, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
    {
         perror("ERROR on binding");
         exit(1);
    }

    listen(socket_id1,5);
    clilen = sizeof(client_addr1);

    new_sock_id1 = accept(socket_id1, (struct sockaddr *)&client_addr1,&clilen);
    if (new_sock_id1 < 0) 
    {
        perror("ERROR on accept");
        exit(1);
    }
    /* If connection is established then start communicating */
    bzero(buffer,256);
    n=send(new_sock_id1,(char *) &s1, sizeof(s), 0);
    if (n < 0)
    {
        perror("ERROR reading from socket");
        exit(1);
    }
   
    /* Write a response to the client */
    n = write(new_sock_id,"I got your message",18);
    if (n < 0)
    {
        perror("ERROR writing to socket");
        exit(1);
    }
    close(new_sock_id1);
    close(socket_id1);
    return 0; 
}
