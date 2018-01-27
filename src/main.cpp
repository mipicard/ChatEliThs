#include <iostream>

#include "Socket/SocketSSL.h"

int main(){
	SocketSSL_n::init();
	
	SocketSSL ecoute_serveur;
	std::cout << "Socket serveur " << ecoute_serveur.get_sock() << " sur l'addresse:port " << ecoute_serveur.get_addr_and_port() << std::endl;
	ecoute_serveur.creer_ecoute_serveur("8888");
	std::cout << "Socket serveur : " << ecoute_serveur.get_sock() << " sur l'addresse:port " << ecoute_serveur.get_addr_and_port() << std::endl;
	ecoute_serveur.end_and_destroy();
	std::cout << "Socket serveur : " << ecoute_serveur.get_sock() << " sur l'addresse:port " << ecoute_serveur.get_addr_and_port() << std::endl;	
	
	SocketSSL_n::end();
	
	return EXIT_SUCCESS;
}
