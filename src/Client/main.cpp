#include <iostream>
#include <string>

#include "Client.h"

void aff(const std::string & message){
	std::cout << message << std::endl;
}

int main(int argv,char **argc){	
	Client c;
	c.set_retour_texte(aff);
	
	std::cout << "--- Client waiting for instructions. ---" << std::endl;
	std::cout << "--- type 'help' for help ---" << std::endl;
	std::string exit="";
	while(exit!="exit"){
		std::getline(std::cin,exit);
		
		if(c.is_waited_to_stop()){
			c.disconnect();
			continue;
		}
		
		if(exit=="connect" || false){
			if(exit=="connect")
				c.connect(argc[1],argc[2]);
			else
				{;}
		}else if(exit=="disconnect"){
			c.disconnect();
		}else if(exit.substr(0,4)=="send"){
			if(exit.size()>5)
				c.send_texte(exit.substr(5));
			else
				std::cout << "--- Blank message unautorized ---" << std::endl;
		}else if(exit=="help"){
			std::cout << "--- List of command : ---" << std::endl;
			std::cout << "---> connect : will ask you for the host and his port, and then try to connect you" << std::endl;
			std::cout << "---> disconnect : will disconnect you from the server" << std::endl;
			std::cout << "---> reconnect : not implemented yet" << std::endl;
			std::cout << "---> send xxxxxxxxxxxxxx : not implemented yet" << std::endl;
			std::cout << "---> exit : for exiting the client" << std::endl;
			std::cout << "---> help : to read this help" << std::endl;
			std::cout << "===> If you are abruptly disconnect from the server, you will to at least hit ENTER to be able to do something else" << std::endl;
		}else if(exit!="exit"){
			std::cout << "--- unknow command ---" << std::endl;
		}
	}
	
	Socket_Portabilite::sleepcp(1000);
	if(c.is_connected() || c.is_waited_to_stop())
		c.disconnect();
		
	return EXIT_SUCCESS;
}
