#include <iostream>

#include "Communication.h"

int Communication::read(std::string & message,const SocketSSL & socket){
	message.clear();
	int ret = 0;
	std::string entete;
	entete.resize(TAILLE_ENTETE);
	int lecture_entete = socket.read(entete);
	if(lecture_entete==TAILLE_ENTETE){
		int ln=bin2int(entete.substr(0,TAILLE_LN)),cmd=bin2int(entete.substr(TAILLE_LN));
		if(ln>0){
			int read_ln = 0,deja=0,tmp_deja=0;
			std::string part_message;
			while(read_ln!=-1 && deja<ln){
				part_message.resize(ln-deja);
				read_ln = socket.read(part_message);
				if(read_ln!=-1){
					message += part_message.substr(0,read_ln);
				}
				tmp_deja+=read_ln;
				if(deja==tmp_deja || read_ln==-1)
					return -1;
				deja=tmp_deja;
			}
		}
		switch(cmd){
			case 1:
				ret = 1;
				break;
			case 0:
				ret = 2;
				break;
			default:
				ret = 9001;
				break;
		}
	}else if(socket.connexion_terminer(lecture_entete)){
		ret = 3;
	}else if(SSL_get_error(socket.get_cssl(),lecture_entete)!=SSL_ERROR_WANT_READ){
		ret = -1;
	}else{
		ret = 0;
	}
	return ret;
}

int Communication::write(const int & commande,const std::string & message, const SocketSSL & socket){
	std::string send = int2bin(message.size(),TAILLE_LN)+int2bin(commande,TAILLE_CMD)+message;
	int read_ln = 0,deja=0,tmp_deja=0,ln=send.size();
	std::string part_message;
	while(read_ln!=-1 && deja<ln){
		part_message = send.substr(deja);
		read_ln = socket.write(part_message);
		tmp_deja+=read_ln;
		if(deja==tmp_deja)
			return 0;
		if(read_ln==-1)
			return -1;
		deja=tmp_deja;
	}
	return ln;
}

int Communication::bin2int(const std::string & bin){
	int ret = 0;
	for(int i=0,m=bin.size();i<m;++i){
		ret *= 2;
		ret += (int)bin[i]-48;
	}
	//std::cout << bin << " -> " << ret << std::endl;
	return ret;
}

std::string Communication::int2bin(const int & i,const int & lenBin){
	std::string ret;
	ret.resize(lenBin);
	int dividende = i, reste = 0;
	for(int b=0;b<lenBin;++b){
		reste = dividende%2;
		dividende /=2;
		ret.at(lenBin-1-b) = (char)(reste+48);
	}
	//std::cout << i << " -> " << ret << std::endl;
	return ret;
}
