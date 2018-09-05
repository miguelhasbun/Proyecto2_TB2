#include "Database.h"


Database::Database(string name,int tBloque) {
    string a="C:\\Users\\Miguel Hasbun\\Desktop\\";
    string b=".data";
    string p= a+name+b;
    string nomb= name+b;
    dataName = new char[nomb.length()];
    path= new char[p.length()];
    strcpy(dataName,nomb.c_str());
    strcpy(path,p.c_str());
    archivo = new DataFile(path);
    iniciarConexion(tBloque);
}

bool Database::dropDatabase() {
    cerrarConexion();
    if( remove( path)!=0 ){
        cout<<"Error al removerlo: "<<strerror(errno)<<endl;
        return false;
    }else{
        cout<<"Base de Datos con el nombre "<< dataName<<" removido con exito"<<endl;
    }
    return true;
}

void Database::cerrarConexion() {
    archivo->cerrar();
}

void Database::iniciarConexion(int t) {
    archivo->abrir();
    mBloques= new ManejadordeBloques(archivo,t);
    mTablas=new ManejadroTablas(archivo,mBloques);
}

void Database::createDataBase(int tamano,string tipo)
{
    if(tipo=="GB")
        for(int c=0;c<tamano;c++)
            mBloques->createDataBase(1073741824,mBloques->masterBlock->ultimoBloque);
    else if(tipo=="MB")
        for (int c = 0; c < tamano; c++)
            mBloques->createDataBase(1048576, mBloques->masterBlock->ultimoBloque);
    else{
        cout<<"No se creo la base de datos"<<endl<<"Error tipo de datos incorrecto, recuerde que solo puede ser(GB o MB)"<<endl;
        return;
    }
    cout<<"Se creo la base de datos exitosamente"<<endl;
}

void Database::selectColums(int id,ListCampos * campos){
    tabla * t = mTablas->buscarTabla(id);
    if(t!=0){
        int sum=0;
        for(int c=0;c<campos->cantidad;c++){
            sum+=campos->get(c)->longitud;
        }
        //Creo la tabla temporal a imprimir
        tabla * tmp= new tabla(t->nombre,t->id,0,0,0,0,t->nBloque,t->archivo,0,0);
        tmp->campos=campos;


            for(int x=0;x< t->registros->cantidad;x++){
                Registro * r = new Registro(sum);
                int actual=0;
                for(int n=0;n<t->registros->get(x)->campoDatos->cantidad;n++){

                    if(t->registros->get(x)->campoDatos->get(n)->defCampos->nombre == campos->get(actual)->nombre ){
                        actual++;
                        r->campoDatos->add(t->registros->get(x)->campoDatos->get(n));
                    }
                }
                tmp->registros->add(r);
                delete r;

            }
         tmp->printTabla();


    }
}

void Database::selectAll(int id){
    mTablas->buscarTabla(id)->printTabla();
}