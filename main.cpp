#include <iostream>
#include "datafile.h"
#include "bloque.h"
#include "manejadrotablas.h"
#include "manejadordebloques.h"
#include "bloquecampo.h"
#include "bloquetabla.h"
#include <stdio.h>
#include "masterblock.h"
#include "tabla.h"
#include "registro.h"
#include "campodatos.h"
#include "bloque.h"
#include "bloquecampo.h"
#include "bloqueregistro.h"
#include "campo.h"
#include "campodatos.h"
#include "BloqueIndice.h"
#include "Database.h"
#include "listcampos.h"

using namespace std;

int main()
{

    Database * db= new Database("database",8192);
   //db->createDataBase(5,"MB");
    ManejadroTablas * mtablas= db->mTablas;
    ManejadordeBloques * mbloques = db->mBloques;
   // cout<<mbloques->masterBlock->sigBloqueDisponible<<endl;
   /* for(int c=0;c<1;c++)
    {
        char * nombre= new char[20];
        nombre[0]='t';
        nombre[1]='a';
        nombre[2]='b';
        nombre[3]='l';
        nombre[4]='a';
        nombre[5]='_';
        char cadena[1000];
        sprintf(cadena, "%d", c);
        for(int x=0;cadena[x]!='\0';x++)
        {
            nombre[x+6]=cadena[x];
        }

        mtablas->crearTabla(nombre,c,mbloques);
        string id="ID";
        string name="Nombre";
        string edad="Edad";
        //ostringstream nom;
        //ostringstream i;
        //ostringstream e;
        char * nom = const_cast<char *>(name.c_str());
        char * i = const_cast<char *>(id.c_str());
        char * e = const_cast<char *>(edad.c_str());

        mtablas->crearCampo(c,i,0,mbloques);
        cout<<"Campos"<<endl;
        mtablas->crearCampo(c,nom,0,mbloques);
        cout<<"Campos"<<endl;
        mtablas->crearCampo(c,e,1,mbloques);
        cout<<"Campos"<<endl;

    }
    for(int c=0;c<5;c++)
    {
        int t=0;
        Registro * r= new Registro(mtablas->buscarTabla(t)->getLongitudRegistros());
        string id="id";
        string name="nombre";
        char edad= (char) (c+48);
        ostringstream nomb;
        stringstream idd;
        ostringstream ed;
        idd<<id<<'_'<<c;
        nomb<<name<<'_'<<c;
        ed<<c;
        char * nom = const_cast<char *>(nomb.str().c_str());
        char * i = const_cast<char *>(idd.str().c_str());
        char * e = const_cast<char *>(ed.str().c_str());
        CampoDatos * d1=new CampoDatos(i,mtablas->buscarTabla(t)->campos->get(0));
        CampoDatos * d2=new CampoDatos(nom,mtablas->buscarTabla(t)->campos->get(1));
        CampoDatos * d3=new CampoDatos(e,mtablas->buscarTabla(t)->campos->get(2));
        r->campoDatos->add(d1);
        r->campoDatos->add(d2);
        r->campoDatos->add(d3);
        mtablas->addRegistro(t,mbloques,r);  //INSERT
    }//*/







    //Select * from con el id de la tabla
    db->selectAll(0);

    //select campos
    /*campo * camp = mtablas->buscarTabla(0)->campos->get(0);
    ListCampos * campos= new ListCampos();
    campos->add(camp);
    db->selectColums(0,campos);*/
    //db->dropDatabase();


    db->cerrarConexion();
    return 0;
}
