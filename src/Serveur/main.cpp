#include <iostream>

#include "Serveur.h"

int main(){
	
	Serveur s;
	s.start_serveur("8888");
	
	std::cout << "Socket serveur : " << s.get_ecoute().get_sock() << " sur l'addresse:port " << s.get_ecoute().get_addr_and_port() << std::endl;

	sleep(300);
	
	s.stop_serveur();
	
	return EXIT_SUCCESS;
}
