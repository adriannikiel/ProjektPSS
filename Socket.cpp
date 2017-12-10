#include "Socket.h"
#pragma comment(lib, "ws2_32.lib")
#include "wyjatki.h"

const short int QLEN=10;     		// Dlugosc kolejki

Socket::Socket()
{
	// Initialize Winsock
    if( WSAStartup( MAKEWORD(2, 2), &wsaData ) != NO_ERROR )  //mozliwa zmiana
    {  
		WSACleanup();
		throw SocketException("Socket Initialization: Error with WSAStartup\n");
	}

    //Create a socket
    mySocket = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );

    if ( mySocket == INVALID_SOCKET )
    {
		WSACleanup();
		throw SocketException("Socket Initialization: Error creating socket");
    }
	
	// Ustawienie mozliwosci wielokrotnego wykorzystania portu 
	int tak=1;

	if( setsockopt(mySocket, SOL_SOCKET, SO_REUSEADDR, (const char *)&tak, sizeof(int)) == SOCKET_ERROR) {
          WSACleanup();
		  throw SocketException("Socket Initialization: Error setsockopt");
    }

	myBackup = mySocket;
}

Socket::~Socket()
{
    WSACleanup();
}

bool Socket::SendData( char *buffer )
{
	int SendResult;

	SendResult = send( mySocket, buffer, strlen( buffer ), 0 );
	if ( SendResult== SOCKET_ERROR ){
		//WSACleanup();
		CloseConnection();
		throw SocketException("Socket SendData: Error sending buffer");
	}
    return true;
}

int Socket::RecvData( char *buffer, int size )
{
	int i = recv( mySocket, buffer, size, 0 );
	    
	if( i == SOCKET_ERROR ){
		CloseConnection();
	}
	buffer[i] = '\0';

    return i;
}

void Socket::CloseConnection()
{
    closesocket( mySocket );
    mySocket = myBackup;
}

char *Socket::GetAndSendMessage(char message[STRLEN])
{
    SendData( message );
	return message;
}


void ServerSocket::StartHosting( int port )
{
     Bind( port );
     Listen();
}

void ServerSocket::Bind( int port )
{
    myAddress.sin_family = AF_INET;
	myAddress.sin_addr.s_addr = INADDR_ANY;
    myAddress.sin_port = htons( port );
	memset( &(myAddress.sin_zero), '\0', 8);

	int BindResult;
	BindResult = bind ( mySocket, (SOCKADDR*) &myAddress, sizeof( myAddress) );
    
	if ( BindResult == SOCKET_ERROR )
    {
		WSACleanup();
		CloseConnection();
		throw SocketException("ServerSocket Bind: Failed to connect");
    }
}

void ServerSocket::Listen()
{
        
    if ( listen ( mySocket, QLEN ) == SOCKET_ERROR )
    {
		CloseConnection();
		WSACleanup();
		throw SocketException("ServerSocket Listen: Error listening on socket");
    }
}

SOCKET ServerSocket::Accept()
{
	acceptSocket = accept( myBackup, NULL, NULL );  
  
	if( acceptSocket == INVALID_SOCKET ){
		WSACleanup();
		CloseConnection();
        throw SocketException("ServerSocket Listen: Error listening on socket");
           
    }

    mySocket = acceptSocket;

	return acceptSocket;
}

ClientSocket::ClientSocket(SOCKET newSocket)
{
	myBackup = mySocket = newSocket;
}

void ClientSocket::ConnectToServer( const char *ipAddress, int port )
{
    myAddress.sin_family = AF_INET;
    myAddress.sin_addr.s_addr = inet_addr( ipAddress );
    myAddress.sin_port = htons( port );
   
    int ConnectResult;
	
	ConnectResult = connect( mySocket, (SOCKADDR*) &myAddress, sizeof( myAddress ) );
    if ( ConnectResult == SOCKET_ERROR )
    {
        WSACleanup();
		CloseConnection();
		throw SocketException("ClientSocket ConnectToServer: Failed to connect");
    } 
}









