#ifndef SOCKET_PORTABILITE_H_INCLUDED
#define SOCKET_PORTABILITE_H_INCLUDED

#ifdef WIN32
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
	void init();
	void end();
}

#endif //SOCKET_PORTABILITE_H_INCLUDED
