#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream> 
#include <cstdio> 
#include <cstring> 
#include <winsock2.h> 
#include <thread>
#include "string.h"
#pragma comment(lib, "WS2_32.lib")
using namespace std;


DWORD WINAPI serverReceive(LPVOID lpParam) { //Получение данных от клиента
	char login[64] = { 0 };
	char buffer[1024] = { 0 }; //Буфер для данных
	SOCKET client = *(SOCKET*)lpParam; //Сокет для клиента
	if (recv(client, login, sizeof(buffer), 0) == SOCKET_ERROR) {
		//Если не удалось получить данные буфера, сообщить об ошибке и выйти
		cout << "recv function failed with error " << WSAGetLastError() << endl;
		return -1;
	}
	
	while (true) { //Цикл работы сервера

		if (recv(client, buffer, sizeof(buffer), 0) == SOCKET_ERROR) {
			//Если не удалось получить данные буфера, сообщить об ошибке и выйти
			cout << "recv function failed with error " << WSAGetLastError() << endl;
			return -1;
		}
		if (strcmp(buffer, "Close_Dialog") == 1) { //Если клиент отсоединился
			cout << "Client Disconnected." << endl;
			break;
		}
		cout << "Client "<<login <<" - "<< buffer << endl; //Иначе вывести сообщение от клиента из буфера
		memset(buffer, 0, sizeof(buffer)); //Очистить буфер
	}
	return 1;
}

DWORD WINAPI serverSend(LPVOID lpParam) { //Отправка данных клиенту
	char buffer[1024] = { 0 };
	SOCKET client = *(SOCKET*)lpParam;
	while (true) {
		fgets(buffer, 1024, stdin);
		if (send(client, buffer, sizeof(buffer), 0) == SOCKET_ERROR) {
			cout << "send failed with error " << WSAGetLastError() << endl;
			return -1;
		}
		if (strcmp(buffer, "exit\n") == 0) {
			cout << "Thank you for using the application" << endl;
			break;
		}
	}
	return 1;
}

