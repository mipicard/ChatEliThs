#include <iostream>
#include <cstdlib>
#include <vector>

#include "Serveur.h"

/**
 * @param s string of maximum size of 5, if not, return false
 */
static bool port_correct(const std::string & s){
	if(s.size()>5)
		return false;
	std::vector<int> tab;
	for(unsigned int i=0;i<s.size();++i){
		tab.push_back(((int)s[i]) - 48);
		if(tab[i]<0 || tab[i]>9)
			return false;
	}
	if(tab.size()==5 &&
	   (tab[0]>6 || 
	    (tab[0]==6 && (tab[1]>5 ||
		 (tab[1]==5 && (tab[2]>5 ||
		  (tab[2]==5 && (tab[3]>3 ||
		   (tab[3]==3 && tab[4]>5))))))))){
		return false;
	}
	return true;
}

int main(){
	
	Serveur s;
	
	std::cout << "--- Server waiting for instructions. ---" << std::endl;
	std::cout << "--- type 'help' for help ---" << std::endl;
	std::string exit="";
	while(exit!="exit"){
		std::getline(std::cin,exit);
		if(exit.substr(0,6)=="start "){
			if(exit.size()>6 && port_correct(exit.substr(6))){
				s.start_serveur(exit.substr(6));
				if(s.is_serveur_up())
					std::cout << "--- Server is now running. ---" << std::endl;
				else
					std::cout << "--- Server can't start ---" << std::endl;
			}else
				std::cout << "Port should look like XXXXX where each X is a number!" << std::endl;
		}else if(exit=="stop"){
			s.stop_serveur();
			std::cout << "--- Server is stop. ---" << std::endl;
		}else if(exit.substr(0,8)=="restart"){
			if(exit.size()>8 && port_correct(exit.substr(8))){
				s.restart_serveur(exit.substr(8));
				if(s.is_serveur_up())
					std::cout << "--- Server have been restart. ---" << std::endl;
				else
					std::cout << "--- Server can't be restart ---" << std::endl;
			}else
				std::cout << "Port should look like XXXXX where each X is a number!" << std::endl;
		}else if(exit=="help"){
			std::cout << "--- List of command : ---" << std::endl;
			std::cout << "---> start XXXXX : for start the server" << std::endl;
			std::cout << "---> stop : for stopping the server" << std::endl;
			std::cout << "---> restart XXXXX : for restarting the server" << std::endl;
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
