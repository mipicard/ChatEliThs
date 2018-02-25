#include <algorithm>
#include <iostream>

#include "../Socket/Communication.h"
#include "Serveur.h"

//CLIENT du serveur

Client * createClient(int id,SocketSSL * texte){
	Client * c = new Client();
	
	c->id = id;
	c->lastping = clock();
	
	c->texte = texte;
	c->ecoute_texte = NULL;
	return c;
}

void deleteClient(Client * c){
	//Suppression des threads
	if(c->ecoute_texte!=NULL){
		c->ecoute_texte->join();
		delete c->ecoute_texte;
	}
	
	//Suppression des sockets
	delete c->texte;
	
	delete c;
}

// SERVEUR

//Serveur -> mise en place
Serveur::Serveur(){
	SocketSSL_n::init();
	atomic_exchange(&serveur_up,false);
}

Serveur::~Serveur(){
	SocketSSL_n::end();
}

bool Serveur::start_serveur(const std::string & port){
	if(!atomic_exchange(&serveur_up,true)){
		//Creer un serveur d'ecoute non-bloquant
		ecoute.creer_ecoute_serveur(port);
		if(ecoute.get_sock()!=INVALID_SOCKET){
			ecoute.set_block(false);
			
			//lance l'ecoute de connexion client
			thread_boucle_serveur = std::thread(&Serveur::boucle_serveur,this);
			//lance le garbage collector
			thread_garbage_collector = std::thread(&Serveur::garbage_collector,this);
			//lance l'envoie des signaux de up
			thread_im_up = std::thread(&Serveur::im_up,this);
			
			return true;
		}
		atomic_exchange(&serveur_up,false);
		std::cout << "Error allocating listening socket" << std::endl;
		return false;
	}
	std::cout << "Serveur already running." << std::endl;
	return false;
}

bool Serveur::stop_serveur(){
	if(atomic_exchange(&serveur_up,false)){
		//Suppression des clients et des threads associés
		for(unsigned int i=0,m=liste_client.size();i<m;++i)
			deleteClient(liste_client.at(i));
		liste_client._clear();
		
		//Attend la fin des threads serveurs
		thread_boucle_serveur.join();
		thread_garbage_collector.join();
		thread_im_up.join();
		
		//Detruit le socket d'ecoute
		ecoute.end_and_destroy();
		
		//notifie (à nouveau) le serveur comme down (le serveur a pu être à nouveau notifié à true autre part)
		atomic_exchange(&serveur_up,false);
		
		return true;
	}
	std::cout << "Serveur isn't running." << std::endl;
	return false;
}

bool Serveur::restart_serveur(const std::string & port){
	stop_serveur();
	return start_serveur(port);
}

bool Serveur::is_serveur_up() const{return serveur_up;}

//SocketSSL & Serveur::get_ecoute(){return ecoute;}

//Serveur -> threads serveur
void Serveur::boucle_serveur(){
	std::string messageNouvelleConnexion = "Un nouveau client vient de se connecter...";
	int id = 0;
	while(atomic_exchange(&serveur_up,true)){
		Socket_Portabilite::sleepcp(1000);
		SocketSSL *client = ecoute.accept_connexion_client();
		if(client->get_sock()!=INVALID_SOCKET){//Si il y a eu une connexion client (réussi)
			client->set_block(false);
			send_to_all_client_texte(1,messageNouvelleConnexion);
			
			Client * c = createClient(id,client);++id;
			c->ecoute_texte = new std::thread(&Serveur::boucle_read_client_texte,this,c);
			
			liste_client._push_back(c);
		}else{
			delete client;
		}
	}
	atomic_exchange(&serveur_up,false);
}

void Serveur::garbage_collector(){
	while(atomic_exchange(&serveur_up,true)){
		Socket_Portabilite::sleepcp(1000);
		garbage.lock();
		while(garbage.size()!=0){
			Client *g = garbage.at(0);
			liste_client._pop(g);
			deleteClient(g);
			garbage.pop(g);
		}
		garbage.unlock();
	}
	atomic_exchange(&serveur_up,false);
}

void Serveur::im_up(){
	while(atomic_exchange(&serveur_up,true)){
		Socket_Portabilite::sleepcp((TIME_OUT/CLOCKS_PER_SEC*1000)/10);
		send_to_all_client_texte(0,"");
	}
	atomic_exchange(&serveur_up,false);
}


//Serveur -> partie communication
void Serveur::boucle_read_client_texte(Client * client){
	std::string message;
	while(atomic_exchange(&serveur_up,true)){
		Socket_Portabilite::sleepcp(100);
		int lecture = Communication::read(message,*(client->texte));
		if(lecture == -1){
			send_to_all_client_texte(1,"Error("+std::to_string(client->id)+")");
			garbage._push_back(client);
			return;
		}else if(lecture == 0){
			clock_t actuel = clock();
			if(actuel!=-1 && actuel-client->lastping>TIME_OUT){
				send_to_all_client_texte(1,"Timed out("+std::to_string(client->id)+")");
				garbage._push_back(client);
				return;
			}
		}else if(lecture == 1){
			send_to_all_client_texte(1,"["+std::to_string(client->id)+"]:"+message);
			client->lastping=clock();
		}else if(lecture == 2){
			//std::cout << "Reset time out" << std::endl;
			client->lastping=clock();
		}else if(lecture == 3){
			send_to_all_client_texte(1,"Disconnected("+std::to_string(client->id)+")");
			garbage._push_back(client);
			return;
		}else{
			send_to_all_client_texte(1,"Ignored("+std::to_string(client->id)+")");
		}
	}
	atomic_exchange(&serveur_up,false);
}

static void send_texte(Client * client,const int & cmd,const std::string & message){
	Communication::write(cmd,message,*(client->texte));
}

void Serveur::send_to_all_client_texte(const int & cmd,const std::string & message){
	std::unique_lock<std::mutex> lock(envoie_global);
	if(message.size()>0)
		std::cout << message << std::endl;
	std::vector<std::thread> tab;
	liste_client.lock();
	for(int i=0,nb=liste_client.size();i<nb;++i)
		tab.push_back(std::thread(send_texte,liste_client.at(i),cmd,message));
	liste_client.unlock();
	for(int i=0,m=tab.size();i<m;++i)
		tab[i].join();
}
