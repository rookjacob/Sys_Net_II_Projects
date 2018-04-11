/*
 * @file bbserver.c
 *
 * @author Jacob Rook
 * @date 02/10/2018
 * @info Course COP 4635
 * Project 1
 */

#include"bbserver.h"

int main(int argc, char *argv[])
{
	//Declare Variables
	int i;

	int sockD;
	int serverPort;
	int numberHosts;


	struct sockaddr_in serverAddr;
	struct sockaddr_in *peerAddrs;

	if(!validateArgv(argc, argv, &serverPort, &numberHosts))
	{
		printf("Program Terminating.");
		return 0;
	}

	createBindSocket(&serverAddr, &serverPort, &sockD);



	peerAddrs = (struct sockaddr_in *)malloc(numberHosts * sizeof(peerAddrs));

	//Receive numberHost peer information
	for(i = 0; i < numberHosts; i++)
	{
		printf("Waiting for peers to join.\n");
		//Receive peer information
		if ((recvfrom(sockD, NULL, 0, 0,(struct sockaddr *)&peerAddrs[i], NULL)) < 0)
		{
			perror("Error: Received Message Error");
			exit(1);
		}
		printf("Peer received with IP address: %lu Port address: %hu \n",
				(unsigned long)peerAddrs[i].sin_addr.s_addr, peerAddrs[i].sin_port);
	}

	createRing(peerAddrs,numberHosts, sockD);

	printf("Ring created. Server Terminating.");

	return 0;
}

void error(const char *msg)
{
	perror(msg);
	exit(1);
}

void validateArgv(int argc, char *argv[], int *serverPort, int *numHosts)
{
	if (argc != 3)
		error("Invalid Parameters.\n"
				"Parameter Format: bbserver <PortNum> <numberHosts>\n");


	*serverPort = atoi(argv[1]);
	*numHosts = atoi(argv[2]);

	if(*numHosts <= 0 || *serverPort < 60000 || *serverPort > 60099)
		error("Invalid Parameter Values.\n"
				"Port Number must be between 60,000 and 60,099\n"
				"Must have one or more hosts to join.\n");

}

void createBindSocket(struct sockaddr_in *serverAddr, int *serverPort, int *socketDescriptor)
{
	//Build local server socket address
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(serverPort);
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	//Create socket
	socketDescriptor = socket(PF_INET,SOCK_DGRAM,0);
	if(socketDescriptor < 0)
	{
		error("Error: Socket Failed");
		exit(1);
	}

	//Bind socket to local address and port
	if((bind(socketDescriptor, (struct sockaddr *)&serverAddr, sizeof(serverAddr))) < 0 )
	{
		error("Error: Bind Failed");
		exit(1);
	}

	printf("Socket was created and binded to local address and port\n\n");
}

void acceptPeers(struct sockaddr_in *peerAddresses, int numberOfPeers, int socketDescriptor)

void createRing(struct sockaddr_in *peerAddresses, int numberOfPeers, int socket)
{
	int i;

	for(i = 0; i < numberOfPeers - 1; i++)
	{
		if ((sendto(socket, &peerAddresses[i + 1], sizeof(peerAddresses[i + 1]), 0,
				(struct sockaddr *)&peerAddresses[i], sizeof(peerAddresses[i]))) < 0 )
		{
			perror("Error: Sending Error");
			exit(1);
		}
	}
	if( (sendto(socket, &peerAddresses[0], sizeof(peerAddresses[0]), 0,
			(struct sockaddr *)&peerAddresses[numberOfPeers - 1],
			sizeof(peerAddresses[numberOfPeers]))) < 0)
	{
		perror("Error: Sending Error");
		exit(1);
	}

}
