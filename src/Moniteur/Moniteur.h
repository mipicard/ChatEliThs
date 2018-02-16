#ifndef MONITEUR_H_INCLUDED
#define MONITEUR_H_INCLUDED

#include <mutex>

template<typename T>
class Moniteur{
	private:
		std::vector<T> data;
		std::mutex atomic_access; //All atomic method begin with an underscore
	public:
		Moniteur();
		
		~Moniteur();
		
		void _push_back(T & d);
		
		void _pop(const T & d);
		
		void _clear();
		
		unsigned int size() const;
		
		T& at(const unsigned int & i);
		
		void lock();
		
		void unlock();
};

template<typename T>
Moniteur<T>::Moniteur(){}

template<typename T>
Moniteur<T>::~Moniteur(){_clear();}

template<typename T>
void Moniteur<T>::_push_back(T & d){
	std::unique_lock<std::mutex> lock(atomic_access);
	data.push_back(d);
}

template<typename T>
void Moniteur<T>::_pop(const T & d){
	std::unique_lock<std::mutex> lock(atomic_access);
	typename std::vector<T>::iterator it = data.begin(), end = data.end();
	while(it!=end && *it!=d){++it;}
	if(it!=end)
		data.erase(it);
}

template<typename T>
void Moniteur<T>::_clear(){
	std::unique_lock<std::mutex> lock(atomic_access);
	data.clear();
}

template<typename T>
T& Moniteur<T>::at(const unsigned int & i){return data.at(i);}

template<typename T>
unsigned int Moniteur<T>::size() const{return data.size();}

template<typename T>
void Moniteur<T>::lock(){atomic_access.lock();}

template<typename T>
void Moniteur<T>::unlock(){atomic_access.unlock();}

#endif //MONITEUR_H_INCLUDED
