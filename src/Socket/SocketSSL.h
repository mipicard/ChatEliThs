#ifndef SOCKETSSL_H_INCLUDED
#define SOCKETSSL_H_INCLUDED

#include <string>

#include "Socket_Portabilite.h"

namespace SocketSSL_n{	
	void init();
	
	void end();
}

class SocketSSL{
		private :
			SOCKET sock;
			char addr[NI_MAXHOST], port[NI_MAXSERV];
			//SSL *cssl;
			
		public :
			SocketSSL();
			
			~SocketSSL();
			
			void creer_ecoute_serveur(const std::string & port);
			
			void creer_liaison_client(const std::string & hostname,const std::string & port);
			
			SocketSSL accept_connexion_client() const;
			
			void end_and_destroy();
			
			const SOCKET get_sock() const;
			
			const std::string get_addr_and_port() const;
};

#endif //SOCKETSSL_H_INCLUDED
