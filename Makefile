CC=g++
FLAG=-Wall -std=c++11

EXE=ChatEliThs

all : make_dir $(EXE)

$(EXE) : obj/Socket/Socket_Portabilite.o obj/Socket/SocketSSL.o obj/main.o
	$(CC) $^ $(FLAG) -g -o bin/$@.exe

#Création des dossiers de compilation
make_dir :
	@mkdir -p obj bin obj/Socket

#Spécification des dépendances


#Commande globales
obj/%.o : src/%.cpp
	$(CC) $^ $(FLAG) -g -c -o $@ 

.PHONY: clean mrproper doc

clean :
	-rm -f obj/* obj/Socket/*

mrproper :
	-rm -rf obj bin doc/html

doc :
	doxygen doc/config.doxy
	
