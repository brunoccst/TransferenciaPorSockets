#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h> 

int main(int argc, char *argv[])
{
    int sockfd = 0, n = 0;

    char fileName[20];
    int fileSize = 0;
    FILE* file;
    char recvBuff[1024];

    struct sockaddr_in serv_addr; 

    if(argc != 4)
    {
        printf("\n Usage: %s <ip of server> <port> <file name>\n",argv[0]);
        return 1;
    } 
	if(sizeof(argv[3])>20)
	{
        printf("\n Nome do arquivo maximo de 20 caracteres\n");
		return;
	}
    //limpo o buffer de entrada
    memset(recvBuff, 0 ,sizeof(recvBuff));
    memset(fileName, 0 ,sizeof(fileName));

    //o nome do arquivo é escrito na string fileName
    sprintf(fileName,"%s",argv[3]);
    printf("File name: %s \n", fileName);
    
    /* Passo 1 - Criar socket */
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket: ");
        exit(1);
    } 

    /* Passo 2 - Configura struct sockaddr_in */
    memset(&serv_addr, '0', sizeof(serv_addr)); 
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[2])); 
    /* converte Ip em formato string para o formato exigido pela struct sockaddr_in*/
    if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr)<=0)
    {
        printf("\n inet_pton error occured\n");
        exit(1);
    } 

    /* Passo 3 - Conectar ao servidor */
    if(connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
       perror("connect: ");
       exit(1);
    } 

    /* Envia o nome do arquivo para o cliente. */
    send(sockfd, fileName, strlen(fileName)+1, 0); 	

    /* Enviar ou receber dados do servidor */
    /* Enquanto o numero de bytes recebidos for differente de 0*/
    recv(sockfd, &fileSize, sizeof(int), 0);
    printf("\nsize: %d\n",fileSize);
    file = fopen(fileName,"wb");
    while ( (n = recv(sockfd, recvBuff, sizeof(recvBuff), 0)) > 0)
    {
	recvBuff[n] = 0;
	printf("Recebi %d bytes.\n", n);
        if(fputs(recvBuff, file) == EOF)
        {
            printf("\n Error : Fputs error\n");
        }
    }

    printf("Finalizou a transferencia.\n");
    fclose(file);

    return 0;
}
