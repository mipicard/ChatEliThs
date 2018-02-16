#include <iostream>

#include "Serveur.h"

//CLIENT du serveur

Client * createClient(int id,SocketSSL * texte){
	Client * c = new Client();
	c->idClient = id;
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
		ecoute.set_block(false);
		
		//lance le thread d'ecoute de connexion client
		thread_boucle_serveur = std::thread(&Serveur::boucle_serveur,this);
		
		return true;
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

void Serveur::boucle_serveur(){
	std::string messageNouvelleConnexion = "Un nouveau client vient de se connecter...";
	int id = 0;
	while(atomic_exchange(&serveur_up,true)){
		SocketSSL *client = ecoute.accept_connexion_client();
		if(client->get_sock()!=INVALID_SOCKET){//Si il y a eu une connexion client (réussi)
			client->set_block(false);
			send_to_all_client_texte(messageNouvelleConnexion);
			
			Client * c = createClient(id,client);++id;
			c->ecoute_texte = new std::thread(&Serveur::boucle_read_client_texte,this,c);
			
			liste_client._push_back(c);
		}else{
			delete client;
		}
	}
	atomic_exchange(&serveur_up,false);
}

void Serveur::boucle_read_client_texte(Client * client){
	int lecture_entete;
	std::string taille;
	std::string message;
	taille.resize(8);
	while(atomic_exchange(&serveur_up,true)){
		lecture_entete = client->texte->read(taille);
		if(lecture_entete==8){
			std::cout << "Un message de " << client->idClient << " : " << taille << std::endl;
		}else if(client->texte->connexion_terminer(lecture_entete)){
			std::cout << "Un client s'est deconnecter." << std::endl;
			return;
		}
	}
	atomic_exchange(&serveur_up,false);
}

void Serveur::send_to_all_client_texte(const std::string & message){
	std::unique_lock<std::mutex> lock(envoie_global);
	std::cout << "Envoi à tous de : \"" << message << "\"" << std::endl;
}

SocketSSL send_to_client_texte(SocketSSL s,std::pair<std::string,bool> param){
	//s.write(message);
	return s;
}
