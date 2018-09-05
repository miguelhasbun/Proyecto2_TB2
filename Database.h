
#ifndef PROYECTOED2_MASTER_DATABASE_H
#define PROYECTOED2_MASTER_DATABASE_H


#include "datafile.h"
#include "manejadordebloques.h"
#include "manejadrotablas.h"
#include <iostream>
#include "string.h"
#include "listcampos.h"
#include "tabla.h"

using namespace std;
class Database {
public:
    Database(string name,int tamBloque);
    DataFile * archivo;
    char* dataName;
    char * path;
    int tamanoChar;
    ManejadordeBloques * mBloques;
    ManejadroTablas * mTablas;
    bool dropDatabase();
    void cerrarConexion();
    void iniciarConexion(int t);
    void createDataBase(int tamano,string tipo);
    int getTamanoDatabase();
    void selectColums(int id,ListCampos * campos);
    void selectAll(int id);

};

#endif //PROYECTOED2_MASTER_DATABASE_H
