#include "Socket_Portabilite.h"

void Socket_Portabilite::init(){
#ifdef WIN32
	WSADATA wsa;
	int err = WSAStartup(MAKEWORD(2,2),&wsa);
	if(err<0){
		puts("WSAStartup failed !");
		exit(EXIT_FAILURE);
	}
#else
	signal(SIGPIPE,SIG_IGN);
#endif
}

void Socket_Portabilite::end(){
#ifdef WIN32
	WSACleanup();
#endif
}

