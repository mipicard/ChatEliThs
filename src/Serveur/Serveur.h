#ifndef SERVEUR_H_INCLUDED
#define SERVEUR_H_INCLUDED

#include <atomic>
#include <thread>
#include <vector>

#include "../Socket/SocketSSL.h"
#include "../Moniteur/Moniteur.h"

typedef struct{
	int idClient;
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
		
		std::thread thread_boucle_serveur;
		
		std::mutex envoie_global;
		
		void boucle_serveur();
		
		void boucle_read_client_texte(Client * client);
		
		void send_to_all_client_texte(const std::string & message);
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
