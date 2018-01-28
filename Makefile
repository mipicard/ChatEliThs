CC=g++
FLAG=-Wall -std=c++11

EXE=Client_ChatEliThs Serveur_ChatEliThs

all : make_dir $(EXE)

Client_ChatEliThs : obj/Socket/Socket_Portabilite.o obj/Socket/SocketSSL.o obj/Client/main.o
	$(CC) $^ $(FLAG) -g -o bin/$@.exe
	
Serveur_ChatEliThs : obj/Socket/Socket_Portabilite.o obj/Socket/SocketSSL.o obj/Serveur/main.o
	$(CC) $^ $(FLAG) -g -o bin/$@.exe

#Création des dossiers de compilation
make_dir :
ifeq ($(OS),Windows_NT)
	if not exist obj mkdir obj
	if not exist obj\Socket mkdir obj\Socket
	if not exist obj\Serveur mkdir obj\Serveur
	if not exist obj\Client mkdir obj\Client
	if not exist bin mkdir bin
else
	@mkdir -p obj bin obj/Socket obj/Serveur obj/Client
endif

#Spécification des dépendances


#Commande globales
obj/%.o : src/%.cpp
	$(CC) $^ $(FLAG) -g -c -o $@ 

.PHONY: clean mrproper doc

clean :
ifeq ($(OS),Windows_NT)
	del /Q obj\*\*
else
	-rm -f obj/*/*
endif

mrproper : clean
ifeq ($(OS),Windows_NT)
	del /S /Q obj bin doc\html
else
	-rm -rf obj bin doc/html
endif

doc :
	doxygen doc/config.doxy
	
