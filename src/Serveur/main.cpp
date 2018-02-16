#include <iostream>
#include <string>

#include "Serveur.h"

int main(){
	
	Serveur s;
	s.start_serveur("8888");

	std::string exit="";
	while(exit!="exit"){std::cin >> exit;}
	
	s.stop_serveur();
	
	return EXIT_SUCCESS;
}
