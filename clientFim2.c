/*
     ██  ██████   ██████   █████  ██████   ██████  ██████      ██████   ██████  ██ ███████
     ██ ██    ██ ██       ██   ██ ██   ██ ██    ██ ██   ██     ██   ██ ██    ██ ██ ██
     ██ ██    ██ ██   ███ ███████ ██   ██ ██    ██ ██████      ██   ██ ██    ██ ██ ███████
██   ██ ██    ██ ██    ██ ██   ██ ██   ██ ██    ██ ██   ██     ██   ██ ██    ██ ██      ██
 █████   ██████   ██████  ██   ██ ██████   ██████  ██   ██     ██████   ██████  ██ ███████
*/


#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <resolv.h>

#include <unistd.h> //read e write
#include <arpa/inet.h> // inet_atom
#include <stdlib.h>

#define PORT	9996
#define SERVER_ADDR     "127.0.0.1"
#define MAXBUF		100

int main(int argc, char **argv)
{
	// Socket File descriptor
	int s;

	// Structure for handling internet addresses
    struct sockaddr_in dest;
 /*
    struct sockaddr_in
	{
    	short            sin_family;   // e.g. AF_INET
    	unsigned short   sin_port;     // e.g. htons(3490)
    	struct in_addr   sin_addr;     // see struct in_addr, below
    	char             sin_zero[8];  // zero this if you want to
	};
	struct in_addr
	{
    	unsigned long s_addr;  // load with inet_aton()
	};
*/

    // buffer to in/out messages
    char msg_write[MAXBUF];
	char msg_read[MAXBUF];

    //address
    char server_addr[50];
	int server_port;


	server_port = PORT;
	strcpy (server_addr, SERVER_ADDR);

    //1)******---Abre socket orientado a conex�es---
    // int socket(int domain, int type, int protocol)
    //domain=AF_INET (Address Family Internet) indica que o dominio de endere�os da internet ser� usado IPV4
    //type= SOCK_STREAM indica o tipo de servi�o de transporte (SOCK_STREAM � TCP e SOCK_DGRAM � UDP)
    //protocol= protocolo utilizado. 0 � TCP
    if ( (s = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
    {
		perror("Socket");
		exit(errno);
	}

	//2)******---Inicializa a estrutura com endere�o e porta---*/
    bzero(&dest, sizeof(dest));
    dest.sin_family = AF_INET;
    dest.sin_port = htons(server_port);
    if ( inet_aton(server_addr, &dest.sin_addr) == 0 ) { //inet_aton(server_addr, &dest.sin_addr.s_addr)
       	perror(server_addr);
       	exit(errno);
    }

    //3)*****---Conecta com o servidor---*/
    if ( connect(s, (struct sockaddr*)&dest, sizeof(dest)) != 0 ) {
       	perror("Connect ");
       	exit(errno);
    }

		printf( "Eu sou o cliente. Conectei com o servidor na porta %d\n", PORT );
		printf( "Para terminar a conexao, digite bye.\n" );
		printf( "Digite seu nome.\n" );

		fgets(msg_write, MAXBUF , stdin);//Escaneia o nome do jogador
		write (s, msg_write, strlen(msg_write)+1);//Envia a mensagem do jogador

		read (s, msg_read, MAXBUF);//Espera uma resposta do servidor
		printf ("%s\n", msg_read);

		read (s, msg_read, MAXBUF);//Espera uma resposta do servidor
		printf ("%s\n", msg_read);

		read (s, msg_read, MAXBUF);//Espera uma resposta do servidor
		printf ("%s\n", msg_read);


	/*
		 ██████  ██████  ███    ███ ██    ██ ███    ██ ██  ██████  █████   ██████  █████   ██████
		██      ██    ██ ████  ████ ██    ██ ████   ██ ██ ██      ██   ██ ██      ██   ██ ██    ██
		██      ██    ██ ██ ████ ██ ██    ██ ██ ██  ██ ██ ██      ███████ ██      ███████ ██    ██
		██      ██    ██ ██  ██  ██ ██    ██ ██  ██ ██ ██ ██      ██   ██ ██      ██   ██ ██    ██
		 ██████  ██████  ██      ██  ██████  ██   ████ ██  ██████ ██   ██  ██████ ██   ██  ██████
		*/
    do
		{

    } while (strcmp(msg_read,"bye"));
   	close(s);//Fecha Sockets
}
