#ifndef DICCHASHCERRADO_H_
#define DICCHASHCERRADO_H_

#include "Lista.h"
#include <sstream>

#ifndef TAM_TABLA_INI
	#define TAM_TABLA_INI 100
#endif
#ifndef UMBRAL_FC
	#define UMBRAL_FC	  0.75
#endif


namespace aed2 {

template<class S>
class DiccHashCerrado {
public:

	typedef String K;

	DiccHashCerrado();
	~DiccHashCerrado();

	bool 	 Definido(const K& clave) const;
	void 	 Definir(const K& clave, const S& significado);
	S& 		 Significado(const K& clave);
	void 	 Borrar(const K& clave);
	Nat 	 CantClaves() const;

	// solo para test!!
	Lista<K> Claves() const;
	float    factorCarga() const;
	Nat      colisiones() const;

private:

	struct TElem{
			K 	 clave;
			S 	 signif;

			TElem(const K& c, const S& v) : clave(c), signif(v) {}
		};


	Lista<TElem>*   _tabla;
	Nat             _cant_elems;
    Nat             _tam;


    #define A 54059
    #define B 76963
    #define C 37
	Nat fn_hash (const String& str) const {
        unsigned h = C;
        for (int i = 0; i < str.length(); ++i) {
            h = (h * A) ^ (str[i] * B);
        }
//        std::cout << str << ": " << (hash % _tam) << std::endl;
 		return h % _tam;
	}

	Nat charToNat(char c) const {
			return (Nat)(c);
	}


	void redimensionarTabla(){
        int tamAnterior = _tam;
        _tam = 2 * _tam;
        // Copio elementos en la nueva tabla
        Lista<TElem>* tabla = new Lista<TElem>[_tam];
        for (int i = 0; i < tamAnterior; ++i) {
            typename Lista<TElem>::Iterador it = _tabla[i].CrearIt();
            while (it.HaySiguiente()) {
//                std::cout << "tabla[" << fn_hash(it.Siguiente().clave) << "] <-- Significado(" << it.Siguiente().clave << ") = " << it.Siguiente().signif << std::endl;
                tabla[fn_hash(it.Siguiente().clave)].AgregarAtras(it.Siguiente());
                it.EliminarSiguiente();
            }
        }

        // Borro y seteo tabla nueva
        delete [] _tabla;
        _tabla = tabla;
	}

};

/********************************************************************************
 * Implementacion
 ********************************************************************************/

template<class S>
DiccHashCerrado<S>::DiccHashCerrado() 
: _cant_elems(0), _tam(TAM_TABLA_INI) 
{
    _tabla = new Lista<TElem>[TAM_TABLA_INI];
}

template<class S>
DiccHashCerrado<S>::~DiccHashCerrado() {
    delete [] _tabla;
}

template<class S>
bool DiccHashCerrado<S>::Definido(const K& clave) const {
	typename Lista<TElem>::Lista lista = _tabla[fn_hash(clave)];
    typename Lista<TElem>::Iterador it = lista.CrearIt();
    while(it.HaySiguiente()){
        if (it.Siguiente().clave == clave) return true;
		it.Avanzar();
    }
    return false;
}


template<class S>
void DiccHashCerrado<S>::Definir(const K& clave, const S& significado) {
    if(factorCarga() > UMBRAL_FC) redimensionarTabla();
    typename Lista<TElem>::Iterador it = _tabla[fn_hash(clave)].CrearIt();
    while(it.HaySiguiente()){
        if (it.Siguiente().clave == clave) {
            it.Siguiente().signif = significado;
            return;
        }
        it.Avanzar();
    }
    TElem elem (clave, significado);
    it.AgregarComoSiguiente(elem); // El iterador lo agrega por copia
    _cant_elems++;
}


template<class S>
S& DiccHashCerrado<S>::Significado(const K& clave) {
    typename Lista<TElem>::Iterador it = _tabla[fn_hash(clave)].CrearIt();
    while(it.HaySiguiente()){
        if (it.Siguiente().clave == clave) return it.Siguiente().signif;
        it.Avanzar();
    }
    assert(false); // El elemento no existe
}


template<class S>
void DiccHashCerrado<S>::Borrar(const K& clave) {
    typename Lista<TElem>::Iterador it = _tabla[fn_hash(clave)].CrearIt();
    while(it.HaySiguiente()){
        if (it.Siguiente().clave == clave) {
            it.EliminarSiguiente();
            _cant_elems--;
            return;
        }
        it.Avanzar();
    }
}

template<class S>
Nat DiccHashCerrado<S>::CantClaves() const {
	return _cant_elems;
}

// solo para test!!
template<class S>
Lista<typename DiccHashCerrado<S>::K> DiccHashCerrado<S>::Claves() const {
	Lista<K> ret;
	typename Lista<TElem>::const_Iterador it;

	for(Nat i=0; i < _tam; i++){
        for(it = _tabla[i].CrearIt(); it.HaySiguiente(); it.Avanzar()) {
            ret.AgregarAtras(it.Siguiente().clave);
        }
	}

	return ret;
}

// solo para test!!
template<class S>
float DiccHashCerrado<S>::factorCarga() const {

	return float(_cant_elems)/_tam;
}

// solo para test!!
template<class S>
Nat DiccHashCerrado<S>::colisiones() const {

	Nat ret = 0;
	for(Nat i=0; i < _tam; i++){
		if (_tabla[i].Longitud() > 1)
			ret = ret + _tabla[i].Longitud() -1;
	}

	return ret;
}


} /* namespace aed2 */

#endif /* DICCHASHCERRADO_H_ */
