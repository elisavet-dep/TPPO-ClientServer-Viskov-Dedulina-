#include "common.h"

int main() {
	setlocale(LC_ALL, "Russian");
	// Создание переменных и сервера
	WSADATA WSAData; //Данные 
	SOCKET server, client1,client2; //Сокеты сервера и клиента
	SOCKADDR_IN serverAddr, clientAddr1, clientAddr2; //Адреса сокетов
	WSAStartup(MAKEWORD(2, 0), &WSAData);
	server = socket(AF_INET, SOCK_STREAM, 0); //Создали сервер
	if (server == INVALID_SOCKET) {
		cout << "Ошибка иницилизации" << WSAGetLastError() << endl;
		return -1;
	}
	serverAddr.sin_addr.s_addr = INADDR_ANY;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(5555);
	if (bind(server, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
		cout << "Ошибка иницилизации" << WSAGetLastError() << endl;
		return -1;
	}

	if (listen(server, 0) == SOCKET_ERROR) { //Если не удалось получить запрос
		cout << "Ошибка прослушивания" << WSAGetLastError() << endl;
		return -1;
	}
	cout << "Отслеживание входящих подключений. . . ." << endl;

	char buffer[1024]; //Создать буфер для данных
	int clientAddrSize1 = sizeof(clientAddr1);
	int clientAddrSize2 = sizeof(clientAddr2);
	if (((client1 = accept(server, (SOCKADDR*)&clientAddr1, &clientAddrSize1)) != INVALID_SOCKET) && ((client2 = accept(server, (SOCKADDR*)&clientAddr2, &clientAddrSize2)) != INVALID_SOCKET)) {
		
		cout << "Оба клиента подключены успешно!" << endl;

		DWORD tid1; //Идентификатоh
		HANDLE t1 = CreateThread(NULL, 0, serverReceive, &client1, 0, & tid1); //Создание потока для получения данных
		if (t1 == NULL) { //Ошибка создания потока
			cout << "Thread Creation Error: " << WSAGetLastError() << endl;
		}
		DWORD tid2; //Идентификатор
		HANDLE t2 = CreateThread(NULL, 0, serverReceive, &client2, 0, &tid2); //Создание потока для получения данных
		if (t1 == NULL) { //Ошибка создания потока
			cout << "Thread Creation Error: " << WSAGetLastError() << endl;
		}
		
		HANDLE t3 = CreateThread(NULL, 0, serverSend, &client2, 0, &tid1); //Создание потока для отправки данных
		if (t2 == NULL) {
			cout << "Thread Creation Error: " << WSAGetLastError() << endl;
		}
		HANDLE t4 = CreateThread(NULL, 0, serverSend, &client1, 0, &tid2); //Создание потока для отправки данных
		if (t2 == NULL) {
			cout << "Thread Creation Error: " << WSAGetLastError() << endl;
		}

		WaitForSingleObject(t1, INFINITE);
		WaitForSingleObject(t2, INFINITE);
		WaitForSingleObject(t3, INFINITE);
		WaitForSingleObject(t4, INFINITE);

		closesocket(client1); //Закрыть сокет
		if (closesocket(server) == SOCKET_ERROR) { //Ошибка закрытия сокета
			cout << "Close socket failed with error: " << WSAGetLastError() << endl;
			return -1;
		}
		closesocket(client2); //Закрыть сокет
		if (closesocket(server) == SOCKET_ERROR) { //Ошибка закрытия сокета
			cout << "Close socket failed with error: " << WSAGetLastError() << endl;
			return -1;
		}
		WSACleanup();
	}
}