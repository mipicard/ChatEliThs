#include <iostream>
#include <mutex>
#include <string>

#include "../Core/Client/Client.h"

std::mutex lock_clean_client;

void aff(const std::string & message){
	std::cout << message << std::endl;
}

void clean_client(Client & c,bool & run){
	while(run){
		Socket_Portabilite::sleepcp(100);
		if(c.is_waited_to_stop()){
			lock_clean_client.lock();
			c.disconnect();
			std::cout << "--- Client is disconnect ---" << std::endl;
			lock_clean_client.unlock();
		}
	}
}

int main(int argv,char **argc){	
	Client c;
	c.set_retour_texte(aff);
	bool run = true;
	
	std::thread t_clean = std::thread(clean_client,std::ref(c),std::ref(run));
	
	std::cout << "--- Client waiting for instructions. ---" << std::endl;
	std::cout << "--- type 'help' for help ---" << std::endl;
	std::string exit="";
	while(exit!="exit"){
		std::getline(std::cin,exit);
		
		if(c.is_waited_to_stop()){
			lock_clean_client.lock();
			c.disconnect();
			lock_clean_client.unlock();
			continue;
		}
		
		if(exit=="connect" || false){
			if(exit=="reconnect")
				{;}
			c.connect(argc[1],argc[2]);
			if(c.is_connected())
				std::cout << "--- Client is connect ---" << std::endl;
			else
				std::cout << "--- Client can't connect ---" << std::endl;
		}else if(exit=="disconnect"){
			c.disconnect();
			std::cout << "--- Client is disconnect ---" << std::endl;
		}else if(exit.substr(0,4)=="send"){
			if(exit.size()>5 && c.is_connected())
				c.send_texte(exit.substr(5));
			else
				std::cout << "--- Blank message unautorized or you are disconnected ---" << std::endl;
		}else if(exit=="help"){
			std::cout << "--- List of command : ---" << std::endl;
			std::cout << "---> connect : will ask you for the host and his port, and then try to connect you" << std::endl;
			std::cout << "---> disconnect : will disconnect you from the server" << std::endl;
			std::cout << "---> reconnect : not implemented yet" << std::endl;
			std::cout << "---> send xxxxxxxxxxxxxx : not implemented yet" << std::endl;
			std::cout << "---> exit : for exiting the client" << std::endl;
			std::cout << "---> help : to read this help" << std::endl;
		}else if(exit!="exit"){
			std::cout << "--- unknow command ---" << std::endl;
		}
	}
	
	run = false;
	t_clean.join();
	
	if(c.is_connected() || c.is_waited_to_stop())
		c.disconnect();
		
	return EXIT_SUCCESS;
}
