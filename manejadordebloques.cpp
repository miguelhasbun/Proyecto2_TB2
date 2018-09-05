#include "manejadordebloques.h"
#include "bloque.h"
#include "datafile.h"
#include <iostream>
using namespace std;

ManejadordeBloques::ManejadordeBloques(DataFile *a,int t)
{
    archivo=a;
    if(archivo->isVacio()==true)
    {
        formatearDataFile(t);
        asignarNueboBloque();
        actualizarMasterBlock();
    }else
    {
        masterBlock= new MasterBlock(archivo,0,-1,-1,t);
        masterBlock->cargar();
    }
}

void ManejadordeBloques::formatearDataFile(int t)
{
    masterBlock= new MasterBlock(archivo,0,-1,-1,t);
    masterBlock->guardar();
}

Bloque * ManejadordeBloques::asignarNueboBloque()
{
    Bloque * bloque = new Bloque(archivo,masterBlock->sigBloqueDisponible,masterBlock->tamanoBloque,-1);
    masterBlock->sigBloqueDisponible++;
    bloque->escribir();
    masterBlock->guardar();
    return bloque;
}

Bloque * ManejadordeBloques::cargarBloque(int numeroBloque)
{
    Bloque * bloque= new Bloque(numeroBloque,masterBlock->tamanoBloque);
    bloque->cargar();
    return bloque;
}

void ManejadordeBloques::actualizarMasterBlock()
{
    //cout<<"BloqueTabla"<<masterBlock->primerBloqueTabla<<endl;
    masterBlock->guardar();
}

void ManejadordeBloques::createDataBase(int tam,int n) {
    int next=n;
    //Creo que el primer bloque
    Bloque * bloque = new Bloque(archivo,masterBlock->sigBloqueDisponible,masterBlock->tamanoBloque,next);
    masterBlock->ultimoBloque=next;
    bloque->escribir();
    delete bloque;
    int sig=0;
    //Creo el resto de bloques hasta llenar la base de datos
    for(int c=masterBlock->tamanoBloque;c<tam;c+=masterBlock->tamanoBloque){
        sig=next+1;
        if(c+masterBlock->tamanoBloque > tam)
            sig=-2;//para indicar que es el ultimo bloque y cuaando el Masterblock->sigBloqueDisponible ==-2 retornar que la base de datos ya esta llena;
        Bloque * b= new Bloque(archivo,next,masterBlock->tamanoBloque,sig);
        masterBlock->ultimoBloque=next;//Esto por si uso esta funcion varias veces en la clase DataBase ( mas que nada por la cantidad de memoria consumida prefiero hacerlo por partes)
        masterBlock->guardar();
        b->escribir();
        delete b;
        next++;
    }
}

