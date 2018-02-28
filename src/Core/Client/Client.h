#ifndef CLIENT_H_INCLUDED
#define CLIENT_H_INCLUDED

#include <atomic>
#include <chrono>
#include <thread>

#include "../Socket/SocketSSL.h"
#include "../Socket/Communication.h"

class Client{
	private:
		std::atomic<bool> client_connected,client_waited_to_stop;
		std::chrono::time_point<std::chrono::system_clock> lastping;
		SocketSSL texte;
		
		std::thread thread_lecture_texte,thread_im_up;
		void (*retour_texte)(const std::string &);
		
		void im_up();
		
		void lecture_texte();
	public:
		Client();
		
		~Client();
		
		bool connect(const std::string & host,const std::string port);
		
		bool disconnect();
		
		bool reconnect(const std::string & host,const std::string port);
		
		bool is_connected() const;
		
		bool is_waited_to_stop() const;
		
		void set_retour_texte(void (*retour)(const std::string &));
		
		void send_texte(const std::string & message) const;
		
		SocketSSL & get_texte();
};

#endif //CLIENT_H_INCLUDED
