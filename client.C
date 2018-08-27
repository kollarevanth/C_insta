#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN
#include<pthread.h>
#include<dos.h>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")
#define DEFAULT_BUFLEN 1024
#define DEFAULT_PORT "27015"
#include"utils.h"
#include"userDataCommands.h"
#include"userOperations.h"
#include"level1Tasks.h"
#include"level3Tasks.h"
#include"level2Tasks.h"



struct threadPool threadpol;
void startSyncing(char *querie)
{
	
	allocate(&threadpol);
	int numberOfQueries = 0;
	char **queries = NULL;
	queries=stringTokeniser(querie, '/', &numberOfQueries);
	for (int i = 0; i < numberOfQueries; i++)
		processingUsingThreads(queries[i], &threadpol);
}




int __cdecl main(int argc, char **argv)
{
	while (1)
	{

		WSADATA wsaData;
		SOCKET ConnectSocket = INVALID_SOCKET;
		struct addrinfo *result = NULL,
			*ptr = NULL,
			hints;
		char sendbuf[100];
		char recvbuf[DEFAULT_BUFLEN];
		gets(sendbuf);
		int iResult;
		int recvbuflen = DEFAULT_BUFLEN;
		iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;
		iResult = getaddrinfo(argv[1], DEFAULT_PORT, &hints, &result);
		for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {
			ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
				ptr->ai_protocol);
			iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
			if (iResult == SOCKET_ERROR) {
				closesocket(ConnectSocket);
				ConnectSocket = INVALID_SOCKET;
				continue;
			}



			freeaddrinfo(result);
			iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
			iResult = shutdown(ConnectSocket, SD_SEND);
			do {

				iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
				if (strcmp(sendbuf, "sync") == 0)
					startSyncing(recvbuf);
			} while (iResult > 0);
		}
		//sleep(50);
	}
		WSACleanup();

	return 0;
}