#include <iostream>
#include <fstream>
#include "DiccHashCerrado.hpp"
#include "mini_test.h"

using namespace std;
using namespace aed2;

bool mismosElementos(const Lista<String> &l1, Lista<String> l2){
    if (l1.Longitud() != l2.Longitud()) return false;

    Lista<String>::const_Iterador itL1 = l1.CrearIt();
    while(itL1.HaySiguiente()) {
        Lista<String>::Iterador itL2 = l2.CrearIt();
        bool esta = false;
        while (itL2.HaySiguiente()) {
            if (itL2.Siguiente() == itL1.Siguiente()) {
                itL2.EliminarSiguiente();
                esta = true;
                break;
            }
            itL2.Avanzar();
        }
        if (!esta) return false;
        itL1.Avanzar();
    }
    return true;
}

void test_Definido () {
	DiccHashCerrado<int> dicc;
    ASSERT(!dicc.Definido("Dios")); // DIOS NO EXISTE

	dicc.Definir("Jorge", 14);
	ASSERT(dicc.Definido("Jorge"));
	ASSERT(!dicc.Definido("Dios")); // DIOS NO EXISTE
}

void test_Definir () {
	DiccHashCerrado<int> dicc;
	dicc.Definir("Papurro", 14);
	dicc.Definir("Papu", 15);
	dicc.Definir("papu", 16);

	ASSERT(dicc.Definido("Papurro"));
	ASSERT(dicc.Definido("papu"));
	ASSERT(dicc.Definido("Papu"));

}

void test_String_Vacio () {
	DiccHashCerrado<int> dicc;
	dicc.Definir("", 14);

	ASSERT(dicc.Definido(""));
	ASSERT(dicc.Significado("") == 14);
}

void test_Significado () {
	DiccHashCerrado<int> dicc;
	dicc.Definir("", 1);
	dicc.Definir("Papurro", 14);
	dicc.Definir("Papu", 15);
	dicc.Definir("papu", 16);

	ASSERT(dicc.Significado("") == 1);
	ASSERT(dicc.Significado("Papurro") == 14);
	ASSERT(dicc.Significado("Papu") == 15);
	ASSERT(dicc.Significado("papu") == 16);
}

void test_Borrar () {

	DiccHashCerrado<int> dicc;
	dicc.Definir("", 1);
	dicc.Definir("Papurro", 14);
	dicc.Definir("Papu", 15);
	dicc.Definir("papu", 16);

	ASSERT(dicc.Definido(""));
	ASSERT(dicc.Definido("Papurro"));
	ASSERT(dicc.Definido("papu"));
	ASSERT(dicc.Definido("Papu"));

	dicc.Borrar("Papurro");
	ASSERT(dicc.Definido(""));
	ASSERT(!dicc.Definido("Papurro"));
	ASSERT(dicc.Definido("papu"));
	ASSERT(dicc.Definido("Papu"));

	dicc.Borrar("papu");
	ASSERT(dicc.Definido(""));
	ASSERT(!dicc.Definido("Papurro"));
	ASSERT(!dicc.Definido("papu"));
	ASSERT(dicc.Definido("Papu"));

	dicc.Borrar("Papu");
	ASSERT(dicc.Definido(""));
	ASSERT(!dicc.Definido("Papurro"));
	ASSERT(!dicc.Definido("papu"));
	ASSERT(!dicc.Definido("Papu"));

	dicc.Borrar("");
	ASSERT(!dicc.Definido(""));
	ASSERT(!dicc.Definido("Papurro"));
	ASSERT(!dicc.Definido("papu"));
	ASSERT(!dicc.Definido("Papu"));

}

void test_CantClaves () {

	DiccHashCerrado<int> dicc;
	ASSERT(dicc.CantClaves() == 0);

	dicc.Definir("", 1);
	dicc.Definir("Papurro", 14);
	dicc.Definir("Papu", 15);
	dicc.Definir("papu", 16);
	ASSERT(dicc.CantClaves() == 4);

	dicc.Borrar("");
	ASSERT(dicc.CantClaves() == 3);

	dicc.Borrar("papu");
	ASSERT(dicc.CantClaves() == 2);

	dicc.Borrar("Dios");
	ASSERT(dicc.CantClaves() == 2);

	dicc.Borrar("Papu");
	ASSERT(dicc.CantClaves() == 1);

	dicc.Borrar("Papurro");
	ASSERT(dicc.CantClaves() == 0);
}

void test_claves(){
	DiccHashCerrado<int> dicc;
	dicc.Definir("", 1);
	dicc.Definir("Papurro", 14);
	dicc.Definir("Papu", 15);
	dicc.Definir("papu", 16);

    Lista<String> claves;
	claves.AgregarAtras("");
	claves.AgregarAtras("Papurro");
	claves.AgregarAtras("Papu");
	claves.AgregarAtras("papu");

    ASSERT(mismosElementos(dicc.Claves(), claves));
	Lista<String>::Iterador itClaves = claves.CrearIt();

	dicc.Borrar("");
	itClaves.EliminarSiguiente();
    ASSERT(mismosElementos(dicc.Claves(), claves));

    dicc.Borrar("Papurro");
    itClaves.EliminarSiguiente();
    ASSERT(mismosElementos(dicc.Claves(), claves));

    dicc.Borrar("Papu");
    itClaves.EliminarSiguiente();
    ASSERT(mismosElementos(dicc.Claves(), claves));

    dicc.Borrar("papu");
    itClaves.EliminarSiguiente();
    ASSERT(mismosElementos(dicc.Claves(), claves));
}

void test_colisiones_uniforme(){

    DiccHashCerrado<int> dh;

	std::string archnom = "data/uniforme.txt";
//	std::string archnom = "data/gauss.txt";
//    std::string archnom = "data/libretas.txt";


    int val = 0;
    string str;

    ifstream archin(archnom.c_str());
    ASSERT(archin.good());

    while(getline(archin,str) && archin.good()){

        //cout << "Definiendo: " << str << endl;
        dh.Definir(str, val);
        val++;

        //if (val == 100) break;
    }
    archin.close();

    ASSERT_EQ(dh.CantClaves(),val);

    cout << "Cant claves: " << val << endl;
    cout << "Factor de carga: " << dh.factorCarga() << endl;
    cout << "Colisiones: " << dh.colisiones() << endl;

}

void test_colisiones_gauss(){

    DiccHashCerrado<int> dh;

//    std::string archnom = "data/uniforme.txt";
	std::string archnom = "data/gauss.txt";
//    std::string archnom = "data/libretas.txt";


    int val = 0;
    string str;

    ifstream archin(archnom.c_str());
    ASSERT(archin.good());

    while(getline(archin,str) && archin.good()){

        //cout << "Definiendo: " << str << endl;
        dh.Definir(str, val);
        val++;

        //if (val == 100) break;
    }
    archin.close();

	ASSERT_EQ(dh.CantClaves(),val);

	cout << "Cant claves: " << val << endl;
	cout << "Factor de carga: " << dh.factorCarga() << endl;
	cout << "Colisiones: " << dh.colisiones() << endl;

}

void test_colisiones_libretas(){

    DiccHashCerrado<int> dh;

//    std::string archnom = "data/uniforme.txt";
//	std::string archnom = "data/gauss.txt";
    std::string archnom = "data/libretas.txt";


    int val = 0;
    string str;

    ifstream archin(archnom.c_str());
    ASSERT(archin.good());

    while(getline(archin,str) && archin.good()){

        //cout << "Definiendo: " << str << endl;
        dh.Definir(str, val);
        val++;

        //if (val == 100) break;
    }
    archin.close();

    ASSERT_EQ(dh.CantClaves(),val);

    cout << "Cant claves: " << val << endl;
    cout << "Factor de carga: " << dh.factorCarga() << endl;
    cout << "Colisiones: " << dh.colisiones() << endl;

}

// Setear TAM_TABLA_INI en 5 para testear
void test_redimensionado(){
    DiccHashCerrado<int> dicc;
    dicc.Definir("", 1);
    dicc.Definir("Papurro", 14);
    dicc.Definir("Papu", 15);
    dicc.Definir("Papu1", 15);
    dicc.Definir("Papu2", 15);
    dicc.Definir("Papu3", 15);
    dicc.Definir("Papu4", 15);
    ASSERT(dicc.Definido(""));
    ASSERT(dicc.Significado("") == 1);

    ASSERT(dicc.Definido("Papurro"));
    ASSERT(dicc.Significado("Papurro") == 14);

    ASSERT(dicc.Definido("Papu"));
    ASSERT(dicc.Significado("Papu") == 15);

    ASSERT(dicc.Definido("Papu1"));
    ASSERT(dicc.Significado("Papu1") == 15);

    ASSERT(dicc.Definido("Papu2"));
    ASSERT(dicc.Significado("Papu2") == 15);

    ASSERT(dicc.Definido("Papu3"));
    ASSERT(dicc.Significado("Papu3") == 15);

    ASSERT(dicc.Definido("Papu4"));
    ASSERT(dicc.Significado("Papu4") == 15);

}


int main() {

	RUN_TEST(test_Definido);
	RUN_TEST(test_Definir);
	RUN_TEST(test_String_Vacio);
	RUN_TEST(test_Significado);
	RUN_TEST(test_Borrar);
	RUN_TEST(test_CantClaves);
	RUN_TEST(test_claves);
	RUN_TEST(test_redimensionado);
	RUN_TEST(test_colisiones_uniforme);
	RUN_TEST(test_colisiones_gauss);
	RUN_TEST(test_colisiones_libretas);

	return 0;
}
