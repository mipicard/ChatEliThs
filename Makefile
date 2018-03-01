CC=g++
FLAG=-Wall -std=c++11

#Objet CORE
Socket_OBJ=$(patsubst src/%.cpp,obj/%.o,$(wildcard src/Core/Socket/*.cpp))
Serveur_OBJ=$(patsubst src/%.cpp,obj/%.o,$(wildcard src/Core/Serveur/*.cpp))
Client_OBJ=$(patsubst src/%.cpp,obj/%.o,$(wildcard src/Core/Client/*.cpp))

#Bibliotheque
ifeq ($(OS),Windows_NT)
	INCLUDE=-Lextern/Windows \
			-Iextern/Windows
	SSL=    -lssl -lcrypto
	THREAD= 
else
	INCLUDE=-L/usr/local/lib
	SSL=-lssl -lcrypto
	THREAD=-pthread
endif

EXE=Client_ChatEliThs_console Serveur_ChatEliThs_console

all : make_dir $(EXE)

Client_ChatEliThs_console : $(Socket_OBJ) $(Client_OBJ) obj/Console/main_client.o
	@$(CC) $^ $(FLAG) $(INCLUDE) $(SSL) $(THREAD) -g -o bin/$@.exe
ifeq ($(OS),Windows_NT)
	@-copy data\ca.crt bin
else
	@-cp data/ca.crt bin/ca.crt
endif
	
Serveur_ChatEliThs_console : $(Socket_OBJ) $(Serveur_OBJ) obj/Console/main_serveur.o
	@$(CC) $^ $(FLAG) $(INCLUDE) $(SSL) $(THREAD) -g -o bin/$@.exe
ifeq ($(OS),Windows_NT)
	@-copy data\servwiki.crt bin
	@-copy data\servwiki.key bin
else 
	@-cp data/servwiki.crt bin
	@-cp data/servwiki.key bin
endif

#Création des dossiers de compilation
make_dir :
ifeq ($(OS),Windows_NT)
	@if not exist obj mkdir obj
		@if not exist obj\Core mkdir obj\Core
			@if not exist obj\Core\Socket mkdir obj\Core\Socket
			@if not exist obj\Core\Serveur mkdir obj\Core\Serveur
			@if not exist obj\Core\Client mkdir obj\Core\Client
		@if not exist obj\Console mkdir obj\Console
		@if not exist obj\Fenetre mkdir obj\Fenetre
	@if not exist bin mkdir bin
else
	@mkdir -p obj
		@mkdir -p obj/Core
			@mkdir -p obj/Core/Socket
			@mkdir -p obj/Core/Serveur
			@mkdir -p obj/Core/Client
		@mkdir -p obj/Console
		@mkdir -p obj/Fenetre
	@mkdir -p bin
endif

#Spécification des dépendances


#Commande globales
obj/%.o : src/%.cpp
	@$(CC) $^ $(FLAG) $(INCLUDE) $(SSL) $(THREAD) -g -c -o $@ 

.PHONY: clean mrproper doc

clean :
ifeq ($(OS),Windows_NT)
	@-del /S /Q obj
else
	@-rm -rf obj
endif

mrproper : clean
ifeq ($(OS),Windows_NT)
	@-del /S /Q bin doc\html
else
	@-rm -rf obj bin doc/html
endif

doc :
ifeq ($(OS),Windows_NT)
	@-echo NOT IMPLEMENTED FOR WINDOWS
else
	@-doxygen doc/config.doxy
endif
