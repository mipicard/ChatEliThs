#ifndef SOCKET_PORTABILITE_H_INCLUDED
#define SOCKET_PORTABILITE_H_INCLUDED

#if defined(_WIN32) || defined(_WIN64)
	#include <winsock2.h>
	typedef int socklen_t;
//#elif defined(__linux__)
#else
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <unistd.h> /* close */
	#include <netdb.h> /* gethostbyname */
	#include <signal.h> /* pour gérer le SIGPIPE sous linux */
	#define INVALID_SOCKET -1
	#define SOCKET_ERROR -1
	#define closesocket(s) close(s)
	typedef int SOCKET;
	typedef struct sockaddr_in SOCKADDR_IN;
	typedef struct sockaddr SOCKADDR;
	typedef struct in_addr IN_ADDR;
//#else
	//#error not defined for this platform
#endif

namespace Socket_Portabilite{
	/**
	 * @brief Initialise le systeme de sockets inter-OS
	 */
	void init();
	
	/**
	 * @brief Termine le systeme de sockets inter-OS
	 */
	void end();
	
	/**
	 * @brief Met en pause le programme pendant x millisecondes
	 * @param [in] milliseconds le temps de pause en millisecondes
	 * 
	 * Met en pause le programme pendant x millisecondes, indépendamment de l'OS
	 */
	void sleepcp(int milliseconds);
}

#endif //SOCKET_PORTABILITE_H_INCLUDED
