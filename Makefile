CC=g++
FLAG=-Wall -std=c++11
LDFLAGS = -L/usr/local/lib
LDLIBS = -lssl -lcrypto

#Objet
Socket_OBJ=$(patsubst src/%.cpp,obj/%.o,$(wildcard src/Socket/*.cpp))
Serveur_OBJ=$(patsubst src/%.cpp,obj/%.o,$(wildcard src/Serveur/*.cpp))
Client_OBJ=$(patsubst src/%.cpp,obj/%.o,$(wildcard src/Client/*.cpp))


EXE=Client_ChatEliThs Serveur_ChatEliThs

all : make_dir $(EXE)

Client_ChatEliThs : $(Socket_OBJ) $(Client_OBJ)
	$(CC) $^ $(FLAG) -g -o bin/$@.exe $(LDFLAGS) $(LDLIBS)
	
Serveur_ChatEliThs : $(Socket_OBJ) $(Serveur_OBJ)
	$(CC) $^ $(FLAG) -g -o bin/$@.exe $(LDFLAGS) $(LDLIBS)

#Création des dossiers de compilation
make_dir :
	@mkdir -p obj bin obj/Socket obj/Serveur obj/Client

#Spécification des dépendances


#Commande globales
obj/%.o : src/%.cpp
	$(CC) $^ $(FLAG) -g -c -o $@ $(LDFLAGS) $(LDLIBS)

.PHONY: clean mrproper doc

clean :
	-rm -f obj/*/*


mrproper : clean
	-rm -rf obj bin doc/html

doc :
	doxygen doc/config.doxy
	
