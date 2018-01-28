#include <iostream>

#include "../Socket/SocketSSL.h"

int main(){
	SocketSSL_n::init();
	
	SocketSSL liaison_client;
	liaison_client.creer_liaison_client("localhost","8888");
	liaison_client.end_and_destroy();	
	
	SocketSSL_n::end();
	
	return EXIT_SUCCESS;
}
