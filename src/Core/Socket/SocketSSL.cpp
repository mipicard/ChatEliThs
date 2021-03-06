#include <cstring>
#include <iostream>
#include <cstdio>
#ifdef WIN32

#else
	#include <fcntl.h>
#endif
#include <errno.h>


#include "SocketSSL.h"

void SocketSSL_n::init(){
	Socket_Portabilite::init();
	SSL_load_error_strings();
	SSL_library_init();
	//OpenSSL_add_all_algorithms();
}

void SocketSSL_n::end(){
	Socket_Portabilite::end();
}

SocketSSL::SocketSSL() : sock(INVALID_SOCKET),addr("0.0.0.0"),port("00000"),cssl(NULL){}

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
		end_and_destroy();
		return;
		//exit(EXIT_FAILURE);
	}
	
	rp = result;
	while(rp!=NULL){
		char yes;
		sock = socket(rp->ai_family, rp->ai_socktype,rp->ai_protocol);
		if(sock==INVALID_SOCKET){//Tente l'allocation de la socket
			//std::cout << "Impossible de creer un socket. Nouvel essai..." << std::endl;
			rp = rp->ai_next;//On passe au suivant
			continue;
		}
		if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR,&yes,sizeof(int))==-1){//Pour ne pas être jeter par le système (optionnelle)
			//std::cout << "setsockop error" << std::endl;
			rp = rp->ai_next;//On passe au suivant
			continue;
		}
		if(bind(sock, rp->ai_addr, rp->ai_addrlen)==0){//On tente le bind de la socket
			b = true;
			if(getnameinfo(rp->ai_addr, rp->ai_addrlen,addr, NI_MAXHOST,this->port, NI_MAXSERV,NI_NUMERICSERV|NI_NUMERICHOST)!=0){
				std::cout << "Erreur lors de la récupération des infos sur le socket créé." << std::endl;
				end_and_destroy();
				return;
				//exit(EXIT_FAILURE);
			}
			//std::cout << "Socket en écoute sur " << addr << ":" << port << std::endl;
			break;
		}else{
			//std::cout << "Impossible d'utiliser ce socket (bind). Nouvel essai..." << std::endl;
			close(sock);
			rp = rp->ai_next;//On passe au suivant
			continue;
		}
	}
	
	if(!b){
		std::cout << "Impossible d'utiliser un socket (bind)" << std::endl;
		end_and_destroy();
		return;
		//exit(EXIT_FAILURE);
	}
	
	freeaddrinfo(result); //On libère la structure des addresses possibles
	if(listen(sock,128)==-1){//On se met en ecoute
		std::cout << "Impossible de se mettre en ecoute sur le socket."<< std::endl;
		end_and_destroy();
		return;
		//exit(EXIT_FAILURE);
	}
}

void SocketSSL::creer_liaison_client(const std::string & hostname,const std::string & port){
	struct addrinfo hints; //pour la demande de connexion
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
	
	if(getaddrinfo(hostname.c_str(),port.c_str(),&hints,&result)!=0){//On recupère les adresses possibles
		std::cout << "Failure in asking for the port : " << port << std::endl;
		end_and_destroy();
		return;
		//exit(EXIT_FAILURE);
	}
	
	rp = result;
	while (rp != NULL) {
		if(getnameinfo(rp->ai_addr, rp->ai_addrlen,addr, NI_MAXHOST,this->port, NI_MAXSERV,NI_NUMERICSERV|NI_NUMERICHOST)!=0){
			std::cout << "Erreur lors de la récupération des infos sur le socket testé." << std::endl;
			end_and_destroy();
			return;
			//exit(EXIT_FAILURE);
		}
		//std::cout << "On tente l'adresse " << addr << " sur le port " << this->port << "..." << std::endl;
	
		sock = socket(rp->ai_family, rp->ai_socktype,rp->ai_protocol);
		if(sock==INVALID_SOCKET){//Tente l'allocation de la socket
			//std::cout << "Impossible de creer un socket. Nouvel essai..." << std::endl;
			rp = rp->ai_next;//On passe au suivant
			continue;
		}
		
		if(connect(sock, rp->ai_addr, rp->ai_addrlen)==0){
			b = true;
			//std::cout << "Connexion effectué." << std::endl;
			break; 
		}else{
			//std::cout << "Impossible d'utiliser ce socket (bind). Nouvel essai..." << std::endl;
			close(sock);
			rp = rp->ai_next;//On passe au suivant
			continue;
		}
	}

	if(!b){
		std::cout << "Impossible d'utiliser un socket (bind)" << std::endl;
		end_and_destroy();
		return;
		//exit(EXIT_FAILURE);
	}
	
	freeaddrinfo(result); //On libère la structure des addresses possibles
	//std::cout << get_addr_and_port() << std::endl;
	
	//PARTIE SSL
	SSL_CTX *sslctx = SSL_CTX_new(SSLv23_client_method());
	if(SSL_CTX_use_certificate_file(sslctx,"servwiki.crt",SSL_FILETYPE_PEM)<=0){
		std::cout << "Erreur lors du chargement du certificat publique serveur." << std::endl;
		end_and_destroy();
		return;
		//exit(EXIT_FAILURE);
	}
	if(SSL_CTX_use_PrivateKey_file(sslctx,"servwiki.key",SSL_FILETYPE_PEM)<=0){
		std::cout << "Erreur lors du chargement de la clé privée serveur." << std::endl;
		end_and_destroy();
		return;
		//exit(EXIT_FAILURE);
	}
	if(SSL_CTX_load_verify_locations(sslctx,"ca.crt",NULL)==0){
		std::cout << "Erreur lors du chargement du certification publique de validation" << std::endl;
		end_and_destroy();
		return;
		//exit(EXIT_FAILURE);
	}
	cssl = SSL_new(sslctx);
	SSL_CTX_free(sslctx);
	SSL_set_fd(cssl, sock);
	if(SSL_connect(cssl)!=1){
		printf("Error: %s\n", ERR_reason_error_string(ERR_get_error()));
		//std::cout << "Handshake unsuccessfull : link refused." << std::endl;
		end_and_destroy();
		return;
		//exit(EXIT_FAILURE);
	}
}
			
SocketSSL* SocketSSL::accept_connexion_client() const{
	SocketSSL* ret = new SocketSSL();
	struct sockaddr_storage tadr;
	socklen_t tadr_len = sizeof(struct sockaddr_storage);
	char host[NI_MAXHOST], service[NI_MAXSERV];
	
	ret->set_sock(accept(sock, (struct sockaddr*) &tadr, &tadr_len));
	if(ret->get_sock()==INVALID_SOCKET){
		if(errno!=EAGAIN){
			std::cout << "Probleme lors de l'allocation d'un socket de discussion" << std::endl;
			exit(EXIT_FAILURE);
		}
		ret->end_and_destroy();
		return ret;
	}
	if(getnameinfo((struct sockaddr*) &tadr, tadr_len, host, NI_MAXHOST, service, NI_MAXSERV, NI_NUMERICSERV)!=0){
		std::cout << "Erreur lors de la récupération des infos sur le socket de discussion." << std::endl;
		ret->end_and_destroy();
		return ret;
		//exit(EXIT_FAILURE);
	}
	std::string addr(host),port(service);
	ret->set_addr_and_port(addr,port);
	
	//std::cout << "log : connexion depuis " << host << ":" << service << " ";
	
	//PARTIE SSL
	SSL_CTX *sslctx = SSL_CTX_new(SSLv23_server_method());
	if(SSL_CTX_use_certificate_file(sslctx,"servwiki.crt",SSL_FILETYPE_PEM)<=0){
		std::cout << "Erreur lors du chargement du certificat publique serveur." << std::endl;
		ret->end_and_destroy();
		return ret;
		//exit(EXIT_FAILURE);
	}
	if(SSL_CTX_use_PrivateKey_file(sslctx,"servwiki.key",SSL_FILETYPE_PEM)<=0){
		std::cout << "Erreur lors du chargement de la clé privée serveur." << std::endl;
		ret->end_and_destroy();
		return ret;
		//exit(EXIT_FAILURE);
	}
	ret->set_cssl(SSL_new(sslctx));
	SSL_CTX_free(sslctx);
	SSL_set_fd(ret->get_cssl(),ret->get_sock());
	if(SSL_accept(ret->get_cssl())!=1){
		printf("Error: %s\n", ERR_reason_error_string(ERR_get_error()));
		//std::cout << "Handshake unsuccessfull : link refused." << std::endl;
		ret->end_and_destroy();
		return ret;
		//exit(EXIT_FAILURE);
	}
	
	//std::cout << "et SSL en place." << std::endl;
	
	return ret;
}

void SocketSSL::end_and_destroy(){
	if(cssl!=NULL){
		SSL_shutdown(cssl);
		SSL_free(cssl);
		cssl = NULL;
	}
	if(sock!=INVALID_SOCKET){
		//std::cout << "Fermeture du socket" << std::endl;
		close(sock);
		sock=INVALID_SOCKET;
		strcpy(addr,"0.0.0.0");
		strcpy(port,"00000");
	}
}

const SOCKET SocketSSL::get_sock() const{return sock;}

void SocketSSL::set_sock(const SOCKET & s){
	sock = s;
}

const std::string SocketSSL::get_addr_and_port() const{return (std::string)(addr)+":"+(std::string)(port);}

void SocketSSL::set_addr_and_port(const std::string & addr,const std::string & port){
	strcpy(this->addr,addr.c_str());
	strcpy(this->port,port.c_str());
}

SSL * SocketSSL::get_cssl() const{return cssl;}
			
void SocketSSL::set_cssl(SSL * cssl){
	this->cssl = cssl;
}

int SocketSSL::read(std::string &s) const{
	if(cssl!=NULL){
		char * r = new char[s.size()];
		int ret = SSL_read(cssl,r,s.size());
		if(ret>0)
			s.assign(r,ret);
		delete[] r;
		return ret;
	}
	return -1;
}
			
int SocketSSL::write(const std::string &s) const{
	if(cssl!=NULL)
		return SSL_write(cssl,s.data(),s.size());
	return -1;
}

void SocketSSL::set_block(const bool block){
	if(sock!=INVALID_SOCKET){
		if(block){
#ifdef WIN32
			
#else
			
#endif
		}else{
#ifdef WIN32
		
#else
			int oldattr = fcntl(sock, F_GETFL);
			if(oldattr==-1){
				std::cout <<"Erreur fnctl" << std::endl;
				exit(EXIT_FAILURE);
			}
			int r = fcntl(sock, F_SETFL, oldattr | O_NONBLOCK);
			if(r==-1){
				std::cout <<"Erreur fnctl" << std::endl;
				exit(EXIT_FAILURE);
			}
#endif
		}
	}
}

bool SocketSSL::connexion_terminer(int retour) const{
	return SSL_get_error(cssl,retour)==SSL_ERROR_ZERO_RETURN;
}

bool SocketSSL::operator == (const SocketSSL & s) const{
	return this->sock==s.get_sock();
}
			
bool SocketSSL::operator != (const SocketSSL & s) const{
	return !(*this==s);
}
