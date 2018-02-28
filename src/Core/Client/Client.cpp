#include <iostream>

#include "Client.h"

//Client -> mise en place
Client::Client()
	: retour_texte(NULL){
	SocketSSL_n::init();
	atomic_exchange(&client_connected,false);
	atomic_exchange(&client_waited_to_stop,false);
}

Client::~Client(){
	SocketSSL_n::end();
	retour_texte=NULL;
}

bool Client::connect(const std::string & host,const std::string port){
	if(!atomic_exchange(&client_connected,true)){
		texte.creer_liaison_client(host,port);
		if(texte.get_cssl()!=NULL){
			texte.set_block(false);
			
			lastping = std::chrono::system_clock::now();
			
			//lance l'ecoute texte
			thread_lecture_texte = std::thread(&Client::lecture_texte,this);
			//lance l'envoie des signaux de up
			thread_im_up = std::thread(&Client::im_up,this);
			
			return true;
		}
		atomic_exchange(&client_connected,false);
		std::cout << "Error on connection." << std::endl;
		return false;
	}
	std::cout << "Client is already connected." << std::endl;
	return false;
}

bool Client::disconnect(){
	if(texte.get_cssl()!=NULL){
		atomic_exchange(&client_connected,false);
		
		//Attend la fin des threads clients
		thread_lecture_texte.join();
		thread_im_up.join();
		
		//Detruit les sockets
		texte.end_and_destroy();
		
		//notifie (à nouveau) le client comme down (le client a pu être à nouveau notifié à true autre part)
		atomic_exchange(&client_connected,false);
		atomic_exchange(&client_waited_to_stop,false);
		
		return true;
	}
	std::cout << "Client isn't connected yet." << std::endl;
	return false;
}

bool Client::reconnect(const std::string & host,const std::string port){
	disconnect();
	return connect(host,port);
}

bool Client::is_connected() const{return client_connected;}

bool Client::is_waited_to_stop() const{return client_waited_to_stop;}

void Client::set_retour_texte(void (*retour)(const std::string &)){retour_texte=retour;}

SocketSSL & Client::get_texte(){return texte;}

//Client -> threads client et ecoute
void Client::im_up(){
	while(atomic_exchange(&client_connected,true)){
		Socket_Portabilite::sleepcp(TIME_OUT/4);
		Communication::write(0,"",texte);
	}
	atomic_exchange(&client_connected,false);
}

void Client::lecture_texte(){
	std::string message;
	while(atomic_exchange(&client_connected,true)){
		Socket_Portabilite::sleepcp(100);
		int lecture = Communication::read(message,texte);
		if(lecture == -1){
			retour_texte("Error from Server.");
			atomic_exchange(&client_waited_to_stop,true);
			atomic_exchange(&client_connected,false);
			return;
		}else if(lecture == 0){
			std::chrono::time_point<std::chrono::system_clock> actuel = std::chrono::system_clock::now();
			if(std::chrono::duration_cast<std::chrono::milliseconds>(actuel-lastping).count()>TIME_OUT){
				retour_texte("Server timed out.");
				atomic_exchange(&client_waited_to_stop,true);
				atomic_exchange(&client_connected,false);
				return;
			}
		}else if(lecture == 1){
			retour_texte(message);
			lastping=std::chrono::system_clock::now();
		}else if(lecture == 2){
			//retour_texte("Reset time out");
			lastping=std::chrono::system_clock::now();
		}else if(lecture == 3){
			retour_texte("Server closed connection. You are disconnected.");
			atomic_exchange(&client_waited_to_stop,true);
			atomic_exchange(&client_connected,false);
			return;
		}else{
			//~ send_to_all_client_texte(1,"Ignored("+std::to_string(client->id)+")");
			retour_texte("Should never happen...");
		}
	}
	atomic_exchange(&client_connected,false);
}

//Client -> envoie
void Client::send_texte(const std::string & message) const{
	Communication::write(1,message,texte);
}
