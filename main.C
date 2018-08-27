#define _CRT_SECURE_NO_WARNINGS
#undef UNICODE
#define WIN32_LEAN_AND_MEAN
#include<pthread.h>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<string.h>
#include<Windows.h>
#include<process.h>
#include<sys/types.h>
#include"utils.h"
#include"userDataCommands.h"
#include"userOperations.h"
#include"level1Tasks.h"
#include"level3Tasks.h"
#include"level2Tasks.h"

#pragma comment (lib, "Ws2_32.lib")
#define DEFAULT_BUFLEN 1024
#define DEFAULT_PORT "27015"

struct threadPool threadpool;

void main1(void *nargs);

int __cdecl main(void)
{
	pthread_t t1;
	char *queryString = malloc(1); queryString[0] = '\0';
	pthread_create(&t1, NULL, main1, queryString);

	allocate(&threadpool);
	while (1){
		WSADATA wsaData;
		int iResult;

		SOCKET ListenSocket = INVALID_SOCKET;
		SOCKET ClientSocket = INVALID_SOCKET;

		struct addrinfo *result = NULL;
		struct addrinfo hints;

		int iSendResult;
		char recvbuf[DEFAULT_BUFLEN];
		int recvbuflen = DEFAULT_BUFLEN;
		iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;
		hints.ai_flags = AI_PASSIVE;

		iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
		ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

		iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
		freeaddrinfo(result);

		iResult = listen(ListenSocket, SOMAXCONN);
		ClientSocket = accept(ListenSocket, NULL, NULL);
		closesocket(ListenSocket);
		do {

			iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
			if (iResult > 0) {
				if (strcmp(recvbuf, "sync") != 0)
					iSendResult = send(ClientSocket, recvbuf, iResult, 0);
				else
					iSendResult = send(ClientSocket, queryString, iResult, 0);
				recvbuf[iSendResult] = '\0';
				strcat(queryString, recvbuf);
				strcat(queryString, "/");
				//printf("%s", recvbuf);
				processingUsingThreads(recvbuf, &threadpool);
			}

		} while (iResult > 0);
		iResult = shutdown(ClientSocket, SD_SEND);

		WSACleanup();
		//system("cls");
	}
}
void main1(void *nargs)
{
	char name[100], *queryString;
	queryString = (char *)nargs;

	while (1)
	{
		gets(name);
		strcat(queryString, name);
		strcat(queryString, "/");
		processingUsingThreads(&name, &threadpool);
	}
}
