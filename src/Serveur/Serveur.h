#ifndef SERVEUR_H_INCLUDED
#define SERVEUR_H_INCLUDED

#include <atomic>
#include <ctime>
#include <thread>
#include <vector>

#include "../Socket/Communication.h"
#include "../Moniteur/Moniteur.h"

typedef struct{
	int id;
	clock_t lastping;
	
	SocketSSL* texte;
	std::thread * ecoute_texte;
} Client;
Client * createClient(int id,SocketSSL * texte);
void deleteClient(Client * c);

class Serveur{
	private:
		SocketSSL ecoute;
		std::atomic<bool> serveur_up;
		Moniteur<Client *> liste_client;
		Moniteur<Client *> garbage;
		
		std::thread thread_boucle_serveur,thread_garbage_collector,thread_im_up;
		
		std::mutex envoie_global;
		
		void boucle_serveur();
		
		void garbage_collector();
		
		void im_up();
		
		void boucle_read_client_texte(Client * client);
		
		void send_to_all_client_texte(const int & cmd,const std::string & message);
	public:
		Serveur();
		
		~Serveur();
		
		bool start_serveur(const std::string & port);
		
		bool stop_serveur();
		
		bool restart_serveur(const std::string & port);
		
		bool is_serveur_up() const;
		
		//SocketSSL & get_ecoute();
};

#endif //SERVEUR_H_INCLUDED
