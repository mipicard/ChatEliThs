#include <iostream>
#include <string>

#include "../Socket/SocketSSL.h"
#include "../Socket/Communication.h"

int main(){
	SocketSSL_n::init();
	
	SocketSSL liaison_client;
	liaison_client.creer_liaison_client("localhost","8888");
	
	std::cout << "Je suis connecter." << std::endl;
	std::cout << "Attente de 2secondes..." << std::endl;
	sleep(2);
	Communication::write(1,"Je suis un client test, permettant de vérifier le bon fonctionnement de ce serveur.",liaison_client);
	std::cout << "Attente de 2secondes..." << std::endl;
	sleep(2);
	std::cout << "Je pars." << std::endl;
	
	liaison_client.end_and_destroy();
	
	SocketSSL_n::end();
	
	return EXIT_SUCCESS;
}
