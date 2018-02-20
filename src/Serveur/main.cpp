#include <iostream>

#include "Serveur.h"

int main(){
	
	Serveur s;
	
	std::cout << "--- Server waiting for instructions. ---" << std::endl;
	std::cout << "--- type 'help' for help ---" << std::endl;
	std::string exit="";
	while(exit!="exit"){
		std::cin >> exit;
		if(exit=="start"){
			s.start_serveur("8888");
			if(s.is_serveur_up())
				std::cout << "--- Server is now running. ---" << std::endl;
			else
				std::cout << "--- Server can't start ---" << std::endl;
		}else if(exit=="stop"){
			s.stop_serveur();
			std::cout << "--- Server is stop. ---" << std::endl;
		}else if(exit=="restart"){
			s.restart_serveur("8888");
			if(s.is_serveur_up())
				std::cout << "--- Server have been restart. ---" << std::endl;
			else
				std::cout << "--- Server can't be restart ---" << std::endl;
		}else if(exit=="help"){
			std::cout << "--- List of command : ---" << std::endl;
			std::cout << "---> start : for start the server" << std::endl;
			std::cout << "---> stop : for stopping the server" << std::endl;
			std::cout << "---> restart : for restarting the server" << std::endl;
			std::cout << "---> exit : for exiting the server (it will stop it)" << std::endl;
			std::cout << "---> help : to read this help" << std::endl;
		}else if(exit!="exit"){
			std::cout << "--- unknow command ---" << std::endl;
		}
	}
	
	if(s.is_serveur_up())
		s.stop_serveur();
	
	return EXIT_SUCCESS;
}
