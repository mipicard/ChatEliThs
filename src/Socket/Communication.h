#ifndef COMMUNICATION_H_INCLUDED
#define COMMUNICATION_H_INCLUDED

#include "SocketSSL.h"

#define TIME_OUT 10*CLOCKS_PER_SEC

//TAILLE_ENTETE = TAILLE_LN + TAILLE_CMD
#define TAILLE_ENTETE 16
#define TAILLE_LN 11
#define TAILLE_CMD 5

/**
 * @note structure des messages : TAILLE_LN caractere + TAILLE_CMD caractere en binaire pour la longueur du message et la commande correspondante.
 * n:0 sera un message anti-time-out (le reste du message est ignoré)
 * n:1 sera un message client de longueur n
 * 
 */

namespace Communication{
/** 
 * @return La fonction read renvoie different type de code en fonction du message lu :
 * -1 : erreur de lecteur inconnu
 *  0 : pas de message mais socket ouvert
 *  1 : message reçu, stocker dans message
 *  2 : still-connected
 *  3 : disconnected
 * 
 */
int read(std::string & message,const SocketSSL & socket);

int write(const int & commande,const std::string & message, const SocketSSL & socket);

int bin2int(const std::string & bin);

std::string int2bin(const int & i,const int & lenBin);

}


#endif //COMMUNICATION_H_INCLUDED
