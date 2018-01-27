//#include <openssl/applink.c>
//#include <openssl/bio.h>
//#include <openssl/ssl.h>
//#include <openssl/err.h>

#include <cstring>
#include <iostream>

#include "SocketSSL.h"

void SocketSSL_n::init(){
	Socket_Portabilite::init();
	//SSL_load_error_strings();
	//SSL_library_init();
	//OpenSSL_add_all_algorithms();
}

void SocketSSL_n::end(){
	Socket_Portabilite::end();
}

SocketSSL::SocketSSL() : sock(INVALID_SOCKET),addr("0.0.0.0"),port("0000"){}

SocketSSL::~SocketSSL(){
	end_and_destroy();
}

void SocketSSL::creer_ecoute_serveur(const std::string & port){
	struct addrinfo hints; //pour la demande de port
	struct addrinfo *result, *rp; //pour stocker et lire les résultats
	bool b = false; //Pour savoir si la reservation a finalement pu être possible
	
	//Initialisation de la demande de port
	std::memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;    /* IPv4 ou IPv6 */
	hints.ai_socktype = SOCK_STREAM; /* socket flux connectée */
	hints.ai_flags = AI_PASSIVE;    /* Les signifie que toutes les addresse de la machine seront utilisée */
	hints.ai_protocol = 0;          /* Any protocol */
	hints.ai_addrlen = 0; 
	hints.ai_addr = NULL;           
	hints.ai_canonname = NULL;
	hints.ai_next = NULL;
	
	if(getaddrinfo(NULL,port.c_str(),&hints,&result)!=0){//On recupère les adresses possibles
		std::cout << "Failure in asking for the port : " << port << std::endl;
		exit(EXIT_FAILURE);
	}
	
	rp = result;
	while(rp!=NULL){
		bool yes = true;
		sock = socket(rp->ai_family, rp->ai_socktype,rp->ai_protocol);
		if(sock==INVALID_SOCKET){//Tente l'allocation de la socket
			std::cout << "Impossible de creer un socket. Nouvel essai..." << std::endl;
			rp = rp->ai_next;//On passe au suivant
			continue;
		}
		if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR,&yes,sizeof(int))==-1){//Pour ne pas être jeter par le système (optionnelle)
			std::cout << "setsockop error" << std::endl;
			rp = rp->ai_next;//On passe au suivant
			continue;
		}
		if(bind(sock, rp->ai_addr, rp->ai_addrlen)==0){//On tente le bind de la socket
			b = true;
			if(getnameinfo(rp->ai_addr, rp->ai_addrlen,addr, NI_MAXHOST,this->port, NI_MAXSERV,NI_NUMERICSERV|NI_NUMERICHOST)!=0){
				std::cout << "Erreur lors de la récupération des infos sur le socket créé." << std::endl;
				exit(EXIT_FAILURE);
			}
			//std::cout << "Socket en écoute sur " << addr << ":" << port << std::endl;
			break;
		}else{
			std::cout << "Impossible d'utiliser ce socket (bind). Nouvel essai..." << std::endl;
			close(sock);
			continue;
		}
		rp = rp->ai_next;//On passe au suivant
	}
	
	if(!b){
		std::cout << "Impossible d'utiliser un socket (bind)" << std::endl;
		exit(EXIT_FAILURE);
	}
	
	freeaddrinfo(result); //On libère la structure des addresses possibles
	if(listen(sock,128)==-1){//On se met en ecoute
		std::cout << "Impossible de se mettre en ecoute sur le socket : " << sock << std::endl;
		exit(EXIT_FAILURE);
	}
}

void SocketSSL::creer_liaison_client(const std::string & hostname,const std::string & port){
	
}
			
SocketSSL SocketSSL::accept_connexion_client() const{
	SocketSSL ret;
	return ret;
}

void SocketSSL::end_and_destroy(){
	if(sock!=INVALID_SOCKET){
		//std::cout << "Fermeture du socket" << std::endl;
		close(sock);
		sock=INVALID_SOCKET;
		strcpy(addr,"0.0.0.0");
		strcpy(port,"0000");
	}
}

const SOCKET SocketSSL::get_sock() const{return sock;}

const std::string SocketSSL::get_addr_and_port() const{return (std::string)(addr)+":"+(std::string)(port);}
