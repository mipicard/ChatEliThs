#include <iostream>

#include "../Socket/SocketSSL.h"

int main(){
	SocketSSL_n::init();
	
	SocketSSL liaison_client;
	liaison_client.creer_liaison_client("localhost","8888");
	
	std::string message;
	message.resize(6);
	liaison_client.read(message);
	std::cout << "Serveur dit : " << message << std::endl;
	
	liaison_client.end_and_destroy();
	
	SocketSSL_n::end();
	
	return EXIT_SUCCESS;
}
