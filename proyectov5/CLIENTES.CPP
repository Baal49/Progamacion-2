#include "CLIENTES.hpp"
#include "HEADER.hpp"
#include <iostream>
#include <string>
using namespace std;

 Cliente Cliente::buscarCliente(archivoHeader& cliente,fstream& archivoc,int cedula,string &nombre,string &direccion){
    Cliente c;
    if(cliente.cantidadRegistros<=0){ cout<<"Tienda no inicializada."<<endl; return c; }
    archivoc.seekg(0, ios::end);
    if(cliente.cantidadRegistros<=0){
        cout<<"No hay clientes registrados en el archivo binario."<<endl;
        return c;
    };
    int x=0;
    
    for(int i=0;i<cliente.cantidadRegistros;i++){
        c=cargarArchivo<Cliente>(cliente,archivoc,i);
        if(c.cedula == cedula){
            nombre = c.nombre;
            direccion = c.dirreccion;
            return c;
        }
        x++;
    }

    cout<<"No existe un cliente con cedula "<<cedula<<" en el archivo binario."<<endl;
    return c;
}
    void Cliente::Crearcliente(archivoHeader& cliente,fstream& archivoc){
        if(!archivoc.is_open()){ cout<<"Tienda no inicializada."<<endl; return; }
        string input;
        int cantidad = 0;
        cout<<"Ingrese la cantidad de clientes a crear (o 'CANCELAR'/'0' para cancelar): ";
        while(true){
            if(!getline(cin, input)) return; // Leer linea para permitir espacios
            if(input=="CANCELAR" || input=="0") { cout<<"Creación cancelada."<<endl; return; }
            try{ cantidad = stoi(input); }
            catch(...){ cout<<"Entrada invalida. Ingrese un numero: "; continue; }
            if(cantidad>0) break;
            cout<<"La cantidad debe ser mayor a 0. Intente nuevamente: ";
        }
            Cliente* cp=new Cliente[cliente.cantidadRegistros+1];
            Cliente c;
            int x=0;
            for(int i=0;i<cliente.cantidadRegistros;i++){
                c=cargarArchivo<Cliente>(cliente,archivoc,i);
                cp[x]=c;
                x++;
                archivoc.clear();
            }
        

        bool err = false;
        for(int n=0;n<cantidad;n++){
            Cliente temp;
            while(true){
                cout<<"Ingrese la cedula del cliente (o 'CANCELAR' para cancelar): ";
                if(!getline(cin, input)) return;
                if(input=="CANCELAR" || input=="0"){ cout<<"Creación cancelada."<<endl; return; }
                if(input.empty()){ cout<<"La cedula no puede estar vacía."<<endl; continue; }
                bool soloDigitos=true;
                for(char ch:input){ if(!isdigit(ch)){ soloDigitos=false; break; } }
                if(!soloDigitos){ cout<<"La cedula solo puede contener numeros."<<endl; continue; }
                int cedulaNueva = stoi(input);
                bool existe=false;
                for(int i=0;i<cliente.cantidadRegistros+1;i++){ 
                    if(cp[i].cedula == cedulaNueva){ 
                        existe=true; break; 
                    } 
                }
                if(existe){ cout<<"La cedula ya existe. Ingrese otra."<<endl; continue; }
                temp.cedula = cedulaNueva;
                break;
            }

            cout<<"Ingrese el nombre del cliente (o 'CANCELAR' para cancelar): ";
            if(!getline(cin,input)) return;
            if(input=="CANCELAR" || input=="0"){ cout<<"Creación cancelada."<<endl; return; }
            strncpy(temp.nombre, input.c_str(), sizeof(temp.nombre)-1);

            cout<<"Ingrese el correo del cliente (o 'CANCELAR' para cancelar): ";
            if(!getline(cin,input)) return;
            if(input=="CANCELAR" || input=="0"){ cout<<"Creación cancelada."<<endl; return; }
        
            strncpy(temp.correo, input.c_str(), sizeof(temp.correo)-1);
            cout<<"Ingrese el telefono del cliente (o 'CANCELAR' para cancelar): ";
            if(!getline(cin,input)) return;
            if(input=="CANCELAR" || input=="0"){ cout<<"Creación cancelada."<<endl; return; }
            strncpy(temp.telefono, input.c_str(), sizeof(temp.telefono)-1);

            cout<<"Ingrese la dirreccion del cliente (o 'CANCELAR' para cancelar): ";
            if(!getline(cin,input)) return;
            if(input=="CANCELAR" || input=="0"){ cout<<"Creación cancelada."<<endl; return; }
            strncpy(temp.dirreccion, input.c_str(), sizeof(temp.dirreccion)-1);

            //Fecha de registro
            cout<<"Ingrese la fecha de registro del cliente (o 'CANCELAR' para cancelar): ";
            if(!getline(cin,input)) return;
            if(input=="CANCELAR" || input=="0"){ cout<<"Creación cancelada."<<endl; return; }
            strncpy(temp.fecharegitro, input.c_str(), sizeof(temp.fecharegitro)-1);
            cp[x+1]=temp;
            guardarArchivo(cliente,archivoc,temp);
            cout<<"Clientes guardados en archivo binario."<<endl;
            cliente.cantidadRegistros=cliente.cantidadRegistros+1;
            cliente.proximoID=cliente.proximoID+1;
            cliente.registrosActivos=cliente.registrosActivos+1;
        }
        
        
    }
void Cliente::Editarcliente (archivoHeader& cliente, fstream& archivoc, int cedula){
    if(cliente.cantidadRegistros <= 0){ cout<<"No hay clientes registrados."<<endl; return; }
        Cliente c;
        if(cliente.cantidadRegistros<=0){ cout<<"No hay clientes registrados en el archivo binario."<<endl; return; }
        int indice=-1,pos;
       for(int i=0;i<cliente.cantidadRegistros;i++){
        c=cargarArchivo<Cliente>(cliente,archivoc,i);
            if(c.cedula==cedula){
                indice=archivoc.tellg()/sizeof(Cliente);
                cout<<"Cliente encontrado: "<<c.nombre<<" | Cedula: "<<c.cedula<<" | Correo: "<<c.correo<<" | Telefono: "<<c.telefono<<" | Dirreccion: "<<c.dirreccion<<" | Fecha de registro: "<<c.fecharegitro<<" en la posicion: "<<indice+1<<endl;
                pos=archivoc.tellg()/sizeof(Cliente)-1;
                break;
            }
        }
            if(indice<0){ cout<<"Cliente con cedula "<<cedula<<" no encontrado."<<endl; return; }

            cout<<"Cliente encontrado: "<<c.nombre<<" | Cedula: "<<c.cedula<<" | Correo: "<<c.correo<<" | Telefono: "<<c.telefono<<" | Dirreccion: "<<c.dirreccion<<" | Fecha de registro: "<<c.fecharegitro<<endl;
            cout<<"¿Qué desea editar?"<<endl;
            cout<<"1. Nombre"<<endl;
            cout<<"2. Correo"<<endl;
            cout<<"3. Telefono"<<endl;
            cout<<"4. Dirreccion"<<endl;
            cout<<"5. Fecha de registro"<<endl;
            cout<<"0. Cancelar"<<endl;
            int opcion;
            cin>>opcion;
            cin.ignore(numeric_limits<streamsize>::max(),'\n');
            string input;
            switch(opcion){
                case 0:
                    cout<<"Edición cancelada."<<endl; return;
                case 1:
                    cout<<"Ingrese el nuevo nombre del cliente (o 'CANCELAR' para cancelar): ";
                    if(!getline(cin,input)) return;
                    if(input=="CANCELAR" || input=="0"){ cout<<"Edición cancelada."<<endl; return; }
                    strncpy(c.nombre,input.c_str(),sizeof(c.nombre)-1);
                    break;
                case 2:
                    cout<<"Ingrese el nuevo correo del cliente (o 'CANCELAR' para cancelar): ";
                    if(!getline(cin,input)) return;
                    if(input=="CANCELAR" || input=="0"){ cout<<"Edición cancelada."<<endl; return; }
                    strncpy(c.correo,input.c_str(),sizeof(c.correo)-1);
                    break;
                case 3:
                    cout<<"Ingrese el nuevo telefono del cliente (o 'CANCELAR' para cancelar): ";
                    if(!getline(cin,input)) return;
                    if(input=="CANCELAR" || input=="0"){ cout<<"Edición cancelada."<<endl; return; }
                    strncpy(c.telefono,input.c_str(),sizeof(c.telefono)-1);
                    break;
                case 4:
                    cout<<"Ingrese la nueva dirreccion del cliente (o 'CANCELAR' para cancelar): ";
                    if(!getline(cin,input)) return;
                    if(input=="CANCELAR" || input=="0"){ cout<<"Edición cancelada."<<endl; return; }
                    strncpy(c.dirreccion,input.c_str(),sizeof(c.dirreccion)-1);
                    break;
                case 5:
                    cout<<"Ingrese la nueva fecha de registro del cliente (o 'CANCELAR' para cancelar): ";
                    if(!getline(cin,input)) return;
                    if(input=="CANCELAR" || input=="0"){ cout<<"Edición cancelada."<<endl; return; }
                    strncpy(c.fecharegitro,input.c_str(),sizeof(c.fecharegitro)-1);
                    break;
                default:
                    cout<<"Opción inválida. Edición cancelada."<<endl; return;
            }
            guardarArchivo(cliente,archivoc,c,pos);
            cout<<"Cliente editado y guardado en archivo binario."<<endl;
        }
    void Cliente::listarcliente(archivoHeader cliente,fstream* archivoc,int posi){
        if(!archivoc){ cout<<"archivo no inicializado."<<endl; return; }
        if(cliente.cantidadRegistros<=0){ cout<<"No hay Clientes registrados."<<endl; return; }
        cout<<"Listado de Clientes:\n";
        Cliente p;
        int x=0;
       for(int i=0;i<cliente.cantidadRegistros;i++){
        p=cargarArchivo<Cliente>(cliente,*archivoc,i);
            cout<<"cedula: "<<p.cedula<<" | Nombre: "<<p.nombre<<" | Telefono: "<<p.telefono<<" | Correo: "<<p.correo<<" | Direccion: "<<p.dirreccion<<" | Fecha de registro: "<<p.fecharegitro<<endl;
            cout<<"X: "<<x<<endl;
            x++;
        }
        archivoc->clear();

}