#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <resolv.h>
#include <unistd.h> //read e write
#include <arpa/inet.h> // inet_atom
#include <stdlib.h>


#define PORT1	9995
#define PORT2	9996
#define MAXBUF		100

int main(int argc, char **argv)
{
	// Socket File descriptor
	int s, client_1; char nome1[MAXBUF];
	int v, client_2; char nome2[MAXBUF];

	// Structure for handling internet addresses
    struct sockaddr_in self1;
    struct sockaddr_in self2;
    struct sockaddr_in client1;
    struct sockaddr_in client2;
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

	// variaveis auxiliares
	int len, port1, port2;
	int addrlen1 = sizeof(client1);
	int addrlen2 = sizeof(client2);

	port1 = PORT1;
	port2 = PORT2;


   	/*
  		█████  ██████  ██████  ███████     ███████  ██████   ██████ ██   ██ ███████ ████████
	 	██   ██ ██   ██ ██   ██ ██          ██      ██    ██ ██      ██  ██  ██         ██
		███████ ██████  ██████  █████       ███████ ██    ██ ██      █████   █████      ██
		██   ██ ██   ██ ██   ██ ██               ██ ██    ██ ██      ██  ██  ██         ██
		██   ██ ██████  ██   ██ ███████     ███████  ██████   ██████ ██   ██ ███████    ██

    domain=AF_INET (Address Family Internet) indica que o dominio de enderecos da internet sera usado IPV4
    type= SOCK_STREAM indica o tipo de servico de transporte TCP (SOCK_STREAM � TCP e SOCK_DGRAM � UDP)
    protocol= protocolo utilizado. 0 � TCP
		*/
   	if ( (s = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
	{
		perror("Socket");
		exit(errno);
	}

		if ( (v = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
	{
		perror("Socket");
		exit(errno);
	}
	//2)******---Inicializa a estrutura com endereco e porta do servidor---*/
  bzero(&self1, sizeof(self1));
  self1.sin_family = AF_INET;
  self1.sin_port = htons(port1);
	self1.sin_addr.s_addr = INADDR_ANY;

	//2)******---Inicializa a estrutura com endereco e porta do servidor---*/
  bzero(&self2, sizeof(self2));
  self2.sin_family = AF_INET;
  self2.sin_port = htons(port2);
	self2.sin_addr.s_addr = INADDR_ANY;

	//3)******---Atribui um numero de porta ao socket---*/
	if ( bind(s, (struct sockaddr*)&self1, sizeof(self1)) != 0 )
	{
		perror("socket--bind");
		exit(errno);
	}
	//3)******---Atribui um numero de porta ao socket---*/
	if ( bind(v, (struct sockaddr*)&self2, sizeof(self2)) != 0 )
	{
		perror("socket--bind");
		exit(errno);
	}
	printf("Veio até aqui\n");


	//4)******---Faz com que ele seja um socket "de escuta"---*/
	if ( listen(s, 1) != 0 )
	{
		perror("socket--listen");
		exit(errno);
	}
	if ( listen(v, 1) != 0 )
	{
		perror("socket--listen");
		exit(errno);
	}



	//5)******---Executa para sempre (at� ser interrompido... ---*/
	fprintf( stderr, "Eu sou o servidor sockets. Estou escutando a porta %d e porta %d\n", PORT1, PORT2);
	//6)******---Aceita a conexão do jogador 01 ---*/
	client_1 = accept(s, (struct sockaddr*)&client1, &addrlen1);

	read (client_1, msg_read, MAXBUF); //Recebe o nome  do cliente
	strcpy(nome1,msg_read);//Salva o nome

	// Cocatena uma mensagem de Boas Vindas
	char bemVindo1[MAXBUF] = "Seja bem-vindo ";
	strcat(bemVindo1, nome1);

	// Apaga o \n no final da entrada
	len = strlen(nome1) - 1;
	if (nome1[len] == '\n') nome1[len] = '\0';

	strcat(bemVindo1, "Você é o jogador 'o'.\n");
	strcat(bemVindo1, "Aguardando o segundo jogador.\n");

	write (client_1, bemVindo1, strlen(bemVindo1));//Envia a mensagem
	printf("O jogador %s se conectou.\n", nome1);


	client_2 = accept(v, (struct sockaddr*)&client2, &addrlen2);

	//6)******---Aceita a conexão do jogador 02 ---*/
	read (client_2, msg_read, MAXBUF); //Recebe a mensagem do cliente
	strcpy(nome2,msg_read);

	char bemVindo2[MAXBUF] = "Seja bem-vindo ";
	strcat(bemVindo2, nome2);

	// Apaga o \n no final da entrada
	len = strlen(nome2) - 1;
	if (nome2[len] == '\n') nome2[len] = '\0';

	strcat(bemVindo2, "Você é o jogador 'x'.\n");
	write (client_2, bemVindo2, strlen(bemVindo2));
	printf("O jogador %s se conectou.\n", nome2);


	// char mensagem[MAXBUF] = "O jogo irá iniciar agora PLayer1.";
	// write (client_1, mensagem, strlen(mensagem));//Envia a mensagem

	char mensagem[MAXBUF]="";
	strcat(mensagem, nome2);
	strcat(mensagem, ", o jogo iniciará agora. Você jogará contra ");
	strcat(mensagem, nome1);

	write (client_2, mensagem, strlen(mensagem));//Envia a mensagem
	bzero(mensagem,MAXBUF);//Zera a variável(Se não fazer isso o STRLEN dá um bug)

	strcat(mensagem, nome1);
	strcat(mensagem, ", o jogo iniciará agora. Você jogará contra ");
	strcat(mensagem, nome2);

	write (client_1, mensagem, strlen(mensagem));//Envia a mensagem
	bzero(mensagem,MAXBUF);//Zera a variável(Se não fazer isso o STRLEN dá um bug)


	mensagem[MAXBUF] = "Você é o primeiro jogador.";
	write (client_1, mensagem, strlen(mensagem));//Envia a mensagem
	bzero(mensagem,MAXBUF);//Zera a variável

	mensagem[MAXBUF] = "Você é o segundo jogador.";
	write (client_2, mensagem, strlen(mensagem));//Envia a mensagem
	bzero(mensagem,MAXBUF);//Zera a variável




	do{
		mensagem[MAXBUF] = "Sua vez";
		write (client_1, mensagem, strlen(mensagem));//Envia a mensagem
 		// read (client_1, msg_read, MAXBUF); //Carrega a jogada
		//
		// // Apaga o \n no final da entrada
		// len = strlen(msg_read) - 1;
		// if (msg_read[len] == '\n') msg_read[len] = '\0';
		//
 		// printf ("Message Rcvd--> %s\n", msg_read);// Imprime a mensagem recebida
		//
 		// write (client_1, "Tudo Certo\n",11);// Envia a mensagem recebida ao cliente
 		// //bzero(msg_read,MAXBUF);
  } while (strcmp(msg_read,"bye"));

	close(client_1);

}
