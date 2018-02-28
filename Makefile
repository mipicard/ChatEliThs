CC=g++
FLAG=-Wall -std=c++11 -fpermissive
LDFLAGS = -L/usr/local/lib
LDLIBS = -lssl -lcrypto -pthread

#Objet CORE
Socket_OBJ=$(patsubst src/%.cpp,obj/%.o,$(wildcard src/Core/Socket/*.cpp))
Serveur_OBJ=$(patsubst src/%.cpp,obj/%.o,$(wildcard src/Core/Serveur/*.cpp))
Client_OBJ=$(patsubst src/%.cpp,obj/%.o,$(wildcard src/Core/Client/*.cpp))


EXE=Client_ChatEliThs_console Serveur_ChatEliThs_console

all : make_dir $(EXE)

Client_ChatEliThs_console : $(Socket_OBJ) $(Client_OBJ) obj/Console/main_client.o
	@$(CC) $^ $(FLAG) -g -o bin/$@.exe $(LDFLAGS) $(LDLIBS)
	@-cp data/ca.crt bin/ca.crt
	
Serveur_ChatEliThs_console : $(Socket_OBJ) $(Serveur_OBJ) obj/Console/main_serveur.o
	@$(CC) $^ $(FLAG) -g -o bin/$@.exe $(LDFLAGS) $(LDLIBS)
	@-cp data/servwiki.crt bin/servwiki.crt
	@-cp data/servwiki.key bin/servwiki.key

#Création des dossiers de compilation
make_dir :
	@mkdir -p obj
		@mkdir -p obj/Core
			@mkdir -p obj/Core/Socket
			@mkdir -p obj/Core/Serveur
			@mkdir -p obj/Core/Client
		@mkdir -p obj/Console
		@mkdir -p obj/Fenetre
	@mkdir -p bin

#Spécification des dépendances


#Commande globales
obj/%.o : src/%.cpp
	@$(CC) $^ $(FLAG) -g -c -o $@ $(LDFLAGS) $(LDLIBS)

.PHONY: clean mrproper doc

clean :
	@-rm -rf obj


mrproper : clean
	@-rm -rf obj bin doc/html

doc :
	@-doxygen doc/config.doxy
	
