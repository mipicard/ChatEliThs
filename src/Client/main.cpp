#include <iostream>

#include "../Socket/SocketSSL.h"

int main(){
	SocketSSL_n::init();
	
	SocketSSL liaison_client;
	liaison_client.creer_liaison_client("localhost","8888");
	
	std::cout << "Je suis connecter." << std::endl;
	for(unsigned int i=0;i<5;++i){
		std::cout << "Attente de 2secondes..." << std::endl;
		sleep(2);
		liaison_client.write("Salut!!!");
	}
	std::cout << "Je pars." << std::endl;
	
	liaison_client.end_and_destroy();
	
	SocketSSL_n::end();
	
	return EXIT_SUCCESS;
}
