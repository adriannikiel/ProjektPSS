/***************************************************************************
 *   Copyright (C) 2009 by Adrian Nikiel   *
 ***************************************************************************/

#ifndef SOCKET_H
#define SOCKET_H

#include "WinSock2.h"
#include <iostream>

using namespace std;

const int STRLEN = 256;

/**
	Klasa do tworzenia i obs�ugi socket�w.
	@author Adrian Nikiel
*/
class Socket
{
public:
	/**
		Konstruktor klasy Socket.
		@throws SocketException
	*/
    Socket(void);

	/**
		Destruktor klasy Socket.
	*/
	~Socket(void);

	/**
		Funkcja wysylajaca dane przez TCP/IP
		@param buffer dane do wyslania
		@return funkcja zwraca true w razie powodzenia trasmisji, w przeciwnym wypadku zwraca false
		@throws SocketException
	*/
        bool SendData( char* buffer);

	/**
		Funkcja odbierajace dane przez TCP/IP
		@param buffer odbierane dane
		@param size rozmiar odbieranych danych
		@return funkcja zwraca ilosc odebranych znak�w
	*/
        int RecvData( char *buffer, int size );

	/**
		Funkcja przygotowuj�ca do zamkni�cia po��czenia
	*/
        void CloseConnection(void);
        
	/**
		Funkcja wywo�uj�ca metod� do wysy�ania danych
		@param recMess wysylana dane
		@return funkcja zwraca wskaznik do wysylanych danych
	*/
	char *GetAndSendMessage(char recMess[STRLEN]);

protected:
    WSADATA wsaData;
    SOCKET mySocket;
    SOCKET myBackup;
    SOCKET acceptSocket;
    sockaddr_in myAddress;
};

/**
	Klasa serwera.
	@author Adrian Nikiel
*/
class ServerSocket : public Socket
{
public:
	/**
		Konstruktor domniemany klasy ServerSocket.
	*/
	ServerSocket() {};
	
	/**
		Destruktor klasy ServerSocket.
	*/
	~ServerSocket() {};

	/**
		Funkcja wywo�uj�ca nas�uchiwanie dla danego socketu
	*/
    void Listen();

	/**
		Funkcja tworz�ca po��czenie na danym porcie
		@param port numer portu 
		@throws SocketException
	*/
    void Bind( int port );

	/**
		Funkcja oczekujaca na po��czenie si� klienta
		@return funkcja zwraca deskryptor do nowego klienta
		@throws SocketException
	*/
	SOCKET Accept();

	/**
		Funkcja wywo�uj�ca metod� bind oraz accept
		@param port numer portu na kt�rym ma by� nas�uchiwanie
	*/
    void StartHosting( int port );
};

/**
	Klasa klienta.
	@author Adrian Nikiel
*/
class ClientSocket : public Socket
{
public:
	/**
		Konstruktor domniemany klasy ClientSocket.
	*/
	ClientSocket() {};

	/**
		Konstruktor klasy ClientSocket.
		@param newSocket numer deskryptora
	*/
	ClientSocket(SOCKET newSocket);

	/**
		Destruktor klasy ClientSocket.
	*/
	~ClientSocket() {};

	/**
		Funkcja umo�liwiaj�ca po��czenie si� z serwerem
		@param ipAddress numer IP serwera
		@param port numer portu
		@throws SocketException
	*/
	void ConnectToServer( const char *ipAddress, int port );
};

#endif







