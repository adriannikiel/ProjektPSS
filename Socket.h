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
	Klasa do tworzenia i obs³ugi socketów.
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
		@return funkcja zwraca ilosc odebranych znaków
	*/
        int RecvData( char *buffer, int size );

	/**
		Funkcja przygotowuj¹ca do zamkniêcia po³¹czenia
	*/
        void CloseConnection(void);
        
	/**
		Funkcja wywo³uj¹ca metodê do wysy³ania danych
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
		Funkcja wywo³uj¹ca nas³uchiwanie dla danego socketu
	*/
    void Listen();

	/**
		Funkcja tworz¹ca po³¹czenie na danym porcie
		@param port numer portu 
		@throws SocketException
	*/
    void Bind( int port );

	/**
		Funkcja oczekujaca na po³¹czenie siê klienta
		@return funkcja zwraca deskryptor do nowego klienta
		@throws SocketException
	*/
	SOCKET Accept();

	/**
		Funkcja wywo³uj¹ca metodê bind oraz accept
		@param port numer portu na którym ma byæ nas³uchiwanie
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
		Funkcja umo¿liwiaj¹ca po³¹czenie siê z serwerem
		@param ipAddress numer IP serwera
		@param port numer portu
		@throws SocketException
	*/
	void ConnectToServer( const char *ipAddress, int port );
};

#endif







