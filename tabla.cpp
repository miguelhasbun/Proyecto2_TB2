#include "tabla.h"
#include <stdio.h>
#include <string.h>
#include <iostream>
#include "listcampos.h"
#include "bloquecampo.h"
#include "datafile.h"
#include "bloqueregistro.h"
#include "registro.h"
#include "campodatos.h"
#include "listregistros.h"
#include "manejadordebloques.h"
#include "listbloquetablas.h"
#include "bloquecampo.h"
#include "registro.h"
#include "Idx_Entry.h"
#include "BloqueIndice.h"
using namespace std;

tabla::tabla(char name[20],int i,int pBCampos,int actualBCampos,int pBDatos,int actualBDatos,int nB,DataFile *a,int primerBIndice,int actualBIndice)
{
    archivo=a;
    strncpy(nombre,name,20);
    id=i;
    primerBloqueCampos=pBCampos;
    actualBloqueCampos=actualBCampos;
    primerBloqueDatos=pBDatos;
    actualBloqueDatos=actualBDatos;
    primerBloqueIndice=primerBIndice;
    actualBloqueIndice=actualBIndice;
    nBloque=nB;
    sig=0;
    campos= new ListCampos();
    registros= new ListRegistros();
    indice=new Indice(archivo,primerBloqueIndice,primerBloqueDatos,getTamanoHashTable());
}


char * tabla::toChar()
{
    char * data= new char[52];
    int pos=0;
    memcpy(&data[pos],nombre,20);
    pos+=20;
    memcpy(&data[pos],&id,4);
    pos+=4;
    memcpy(&data[pos],&primerBloqueCampos,4);
    pos+=4;
    memcpy(&data[pos],&actualBloqueCampos,4);
    pos+=4;
    memcpy(&data[pos],&primerBloqueDatos,4);
    pos+=4;
    memcpy(&data[pos],&actualBloqueDatos,4);
    pos+=4;
    memcpy(&data[pos],&primerBloqueIndice,4);
    pos+=4;
    memcpy(&data[pos],&actualBloqueIndice,4);
    pos+=4;
    memcpy(&data[pos],&nBloque,4);
    pos+=4;
    return data;
}

void tabla::charToTabla(char * data)
{
    int pos=0;
    memcpy(nombre,&data[pos],20);
    pos+=20;
    memcpy(&id,&data[pos],4);
    pos+=4;
    memcpy(&primerBloqueCampos,&data[pos],4);
    pos+=4;
    memcpy(&actualBloqueCampos,&data[pos],4);
    pos+=4;
    memcpy(&primerBloqueDatos,&data[pos],4);
    pos+=4;
    memcpy(&actualBloqueDatos,&data[pos],4);
    pos+=4;
    memcpy(&primerBloqueIndice,&data[pos],4);
    pos+=4;
    memcpy(&actualBloqueIndice,&data[pos],4);
    pos+=4;
    memcpy(&nBloque,&data[pos],4);
    pos+=4;
    indice=new Indice(archivo,primerBloqueIndice,actualBloqueIndice,getTamanoHashTable());
}

void tabla::crearRegistro(ManejadordeBloques * mbloques,Registro *r) {
    Idx_Entry *entry;
    int t= mbloques->masterBlock->tamanoBloque;
    if (primerBloqueDatos == -1) {
        Bloque *b = mbloques->asignarNueboBloque();
        BloqueRegistro *br = new BloqueRegistro(archivo, b->nBloque,t);
        br->registros->add(r);
        br->actualizarCantidad();
        br->escribir();
        registros->add(r);
        primerBloqueDatos = b->nBloque;
        actualBloqueDatos = b->nBloque;
        //Lo agrego a la Hash table
        entry = new Idx_Entry(r->campoDatos->get(0)->valor, b->nBloque, 0);
        manejadorBIndice(mbloques);
        indice->insertar(entry, mbloques);
        return;
    }
    int actual=actualBloqueDatos;
    while(actual!=-1)
    {
        BloqueRegistro *br = new BloqueRegistro(archivo,actual,t);
        br->cargar(r->longitudRegistro);
        int maximo=(int) (496/r->longitudRegistro);

        if(br->registros->cantidad < maximo)
        {

            entry= new Idx_Entry(r->campoDatos->get(0)->valor,br->nBloque,br->cantidad);
            if(indice->insertar(entry,mbloques))
            {
                br->registros->add(r);
                br->actualizarCantidad();
                br->escribir();
                registros->add(r);
            }
            else
                cout<<"No se pudo agregar el registro debido al id del registro"<<endl;
            return;
            //Tenqo que guardar la tabla o por lo menos el bloqueTabla como tal
        }
        actual=br->siguiente;
        delete  br;
    }
    entry= new Idx_Entry(r->campoDatos->get(0)->valor,mbloques->masterBlock->sigBloqueDisponible,0);
    if(indice->insertar(entry,mbloques))
    {
        Bloque *b=mbloques->asignarNueboBloque();
        BloqueRegistro * br = new BloqueRegistro(archivo,b->nBloque,t);
        BloqueRegistro * tmp = new BloqueRegistro(archivo,actualBloqueDatos,t);
        tmp->cargar(r->longitudRegistro);
        tmp->siguiente=br->nBloque;
        tmp->escribir();
        br->registros->add(r);
        registros->add(r);
        br->actualizarCantidad();
        br->escribir();
        actualBloqueDatos=b->nBloque;
    }
    else
        cout<<"No se pudo agregar el registro debido al id del registro"<<endl;


}

void tabla::crearCampo(ManejadordeBloques * mbloques,char name[20],int tipo)
{
    int t= mbloques->masterBlock->tamanoBloque;
    campo * c= new campo(name,tipo);
    if(primerBloqueCampos==-1)
    {
        Bloque * b =mbloques->asignarNueboBloque();
        BloqueCampo * bc= new BloqueCampo(archivo,b->nBloque,t);
        bc->campos->add(c);
        bc->cantidad++;
        bc->escribir();
        campos->add(c);
        primerBloqueCampos=bc->nBloque;
        actualBloqueCampos=bc->nBloque;


        //Tengo que guardar la tabla en el archivo o por lo menos el bloqueTabla
        return;
    }
    int actual=primerBloqueCampos;
    while(actual!=-1)
    {
        BloqueCampo *bc = new BloqueCampo(archivo,actual,t);
        bc->cargar();
        int maximo=17;
        if(bc->cantidad<maximo)
        {
            bc->campos->add(c);
            bc->cantidad++;
            bc->escribir();
            campos->add(c);
            return;
            //Tenqo que guardar la tabla o por lo menos el bloqueTabla como tal
        }
        actual=bc->siguiente;

    }
    Bloque *b=mbloques->asignarNueboBloque();
    BloqueCampo * bc = new BloqueCampo(archivo,b->nBloque,t);
    BloqueCampo * tmp = new BloqueCampo(archivo,actualBloqueCampos,t);
    tmp->cargar();
    tmp->siguiente=bc->nBloque;
    tmp->escribir();
    bc->campos->add(c);
    bc->cantidad++;
    bc->escribir();
    actualBloqueCampos=bc->nBloque;
    //Tengo que guardar la tabla o el Bloque Tabla
}

void tabla::cargarCampos()
{
    int actual=primerBloqueCampos;
    while(actual!=-1)
    {
        BloqueCampo *bc= new BloqueCampo(archivo,actual,8192);
        bc->cargar();
        for(int c=0;c<bc->cantidad;c++)
        {
            campos->add(bc->campos->get(c));
        }
        actual=bc->siguiente;
    }
}

void tabla::cargarRegistros()
{
    int actual=primerBloqueDatos;
    while(actual!=-1)
    {
        BloqueRegistro *br= new BloqueRegistro(archivo,actual,8192);
        int longitudReg=this->getLongitudRegistros();
        br->cargar(longitudReg);

        for(int c=0;c< br->cantidad;c++)
        {
            registros->add(interpretarRegistro(br->registros->get(c)->toChar(),longitudReg));
        }
        actual=br->siguiente;
    }
}

Registro * tabla::interpretarRegistro(char * data,int longitud)
{
    int pos=0;
    Registro * reg= new Registro(longitud);
    for(int c=0;c<campos->cantidad;c++)
    {
        CampoDatos * campDatos= new CampoDatos("",0);
        campo * defCampo= campos->get(c);
        campDatos->defCampos=defCampo;
        memcpy(campDatos->valor,&data[pos],20);
        //campDatos->valor=&data[pos];
        //memcpy(campDatos->valor,&data[pos],defCampo->longitud);
        pos+=defCampo->longitud;
        reg->campoDatos->add(campDatos);
    }
    return reg;
}

int tabla::getLongitudRegistros()
{
    int sum=0;
    for(int c=0;c<campos->cantidad;c++)
    {
        sum+=campos->get(c)->longitud;
    }
    return sum;

}

void tabla::toString()
{
    cout<<"Nombre: "<<nombre<<"  ID: "<<id<<"  PrimerBloqueCampo: "<<primerBloqueCampos<<"  ActualBloqueCampo: "<<actualBloqueCampos<<"  PrimerBloqueDatos: "<<primerBloqueDatos<<"  ActualBloqueDatos: "<<actualBloqueDatos<<"  Numero de Bloque: "<<nBloque<<" pbloqueindice "<<primerBloqueIndice<<" actualbloqueindice"<<actualBloqueIndice<<endl<<endl;
}

void tabla::printTabla()
{
    cout<<"Nombre de la Tabla: "<<nombre<<endl;
    for(int c=0;c<campos->cantidad;c++)
    {
        cout<<" "<<campos->get(c)->nombre<<"    ";
    }
    cout<<endl;
    for(int c=0;c<registros->cantidad;c++)
    {
        Registro * r= registros->get(c);
        for(int x=0;x<r->campoDatos->cantidad;x++)
        {
            CampoDatos * camDatos=r->campoDatos->get(x);
            //camDatos->printValor();
            if(camDatos->defCampos->tipo==1)
            {
                cout<<"   ";
            }
            cout<<" "<<camDatos->valor;
            cout<<"     ";
             if(camDatos->defCampos->tipo==1)
            {
                cout<<"   ";
            }

        }
        cout<<endl;
    }
}

int tabla::getTamanoHashTable() {
    int sum=0;
    int actual=primerBloqueIndice;
    while(actual!=-1)
    {
        BloqueIndice * bloque= new BloqueIndice(archivo,actual,8192);
        bloque->cargar();
        sum+=62;
        actual=bloque->siguiente;
        delete bloque;
    }
    return sum;
}

Registro * tabla::buscarRegistro(char *id) {
    Idx_Entry * entry = indice->buscar(id);
    if(entry==0)
        return 0;
    BloqueRegistro * bloque= new BloqueRegistro(archivo,entry->numeroBloque,8192);
    Registro * registro= bloque->getRegsitro(entry->numeroRR,getLongitudRegistros());
    for(int c=0;c<campos->cantidad;c++)
    {
        campo * defCampo = campos->get(c);
        registro->campoDatos->get(c)->defCampos=defCampo;
    }
    return registro;
}

void tabla::manejadorBIndice(ManejadordeBloques * mB)
{
    int n= mB->asignarNueboBloque()->nBloque;
    BloqueIndice * bi = new BloqueIndice(archivo,n,8192);
    bi->escribir();
    if(primerBloqueIndice==-1)
        primerBloqueIndice=n;
    this->actualBloqueIndice=n;
    indice->actualizarIndice(primerBloqueIndice,actualBloqueIndice,getTamanoHashTable());
}

