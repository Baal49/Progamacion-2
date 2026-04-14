#include "PROVEEDOR.hpp"
#include <iostream>


    // otros campos omitidos para brevedad
    void Proveedor::Crearproveedor(archivoHeader& proveedor, fstream* archivo,int posi){
    cin.ignore(numeric_limits<streamsize>::max(),'\n');
    if(!archivo){ cout<<"El archivo no se a podido abrir"<<endl; return; }
    string input;
    int cantidad = 0;
    cout<<"Ingrese la cantidad de proveedores a crear (o 'CANCELAR'/'0' para cancelar): ";
    while(true){
        if(!getline(cin, input)) return;
        if(input=="CANCELAR" || input=="0") { cout<<"Creación cancelada."<<endl; return; }
        try{ cantidad = stoi(input); }
        catch(...){ cout<<"Entrada invalida. Ingrese un numero: "; continue; }
        if(cantidad>0) break;
        cout<<"La cantidad debe ser mayor a 0. Intente nuevamente: ";
    }
    for(int n=0;n<cantidad;n++){
        Proveedor temp;

        // Codigo (único)
        while(true){
            cout<<"Ingrese el Id del proveedor (o 'CANCELAR' para cancelar): ";
            if(!getline(cin, input)) return;
            if(input=="CANCELAR" || input=="0"){ cout<<"Creación cancelada."<<endl; return; }
            if(input.empty()){ cout<<"El codigo no puede estar vacío."<<endl; continue; }
            //if(codigoDuplicado(tienda, input)) { cout<<"Codigo ya existe. Ingrese otro."<<endl; continue; }
            temp.id=stoi(input);
            break;
        }

        // Nombre
        cout<<"Ingrese el nombre del proveedor (o 'CANCELAR' para cancelar): ";
        if(!getline(cin,input)) return;
        if(input=="CANCELAR" || input=="0"){ cout<<"Creación cancelada."<<endl; return; }
        strncpy(temp.nombre, input.c_str(), sizeof(temp.nombre)-1);

        
        // Asignar ID autoincremental
        //temp.id = tienda->siguienteIdProveedor++;

        // Confirmación
        cout<<"\nResumen del proveedor:"<<endl;
        cout<<"ID: "<<temp.id<<" | Nombre: "<<temp.nombre<<endl;
        cout<<"¿Desea guardar este proveedor? (S/N): ";
        string resp;
        if(!getline(cin,resp)) return;
        if(resp=="S" || resp=="s" || resp=="Si" || resp=="SI" || resp=="si"){
            // si el arreglo está lleno, redimensionar duplicando capacidad
            /*if(tienda->cantidadProveedores >= tienda->capacidadProveedores){
                //redimensionarProveedor(tienda);
                cout<<"Arreglo de proveedores redimensionado a capacidad "<<tienda->capacidadProveedores<<"."<<endl;
            }*/
            
            guardarArchivo(proveedor,*archivo,temp);
            proveedor.cantidadRegistros++;
            proveedor.registrosActivos++;
            proveedor.proximoID++;
            cout<<"cantidadregistros: "<<proveedor.cantidadRegistros<<endl;
            cout<<"registros activos: "<<proveedor.registrosActivos;
            
            cout<<"Proveedor guardado."<<endl;
        } else {
            cout<<"Proveedor descartado por el usuario."<<endl;
        }
    }
}
void Proveedor::listarProveedores(archivoHeader proveedor,fstream* archivoprov,int posi){
        if(!archivoprov){ cout<<"archivo no inicializado."<<endl; return; }
        if(proveedor.cantidadRegistros<=0){ cout<<"No hay proveedores registrados."<<endl; return; }
        cout<<"Listado de proveedores:\n";
        Proveedor p;
        int x=0;
        for(int i=0;i<proveedor.cantidadRegistros;i++){
            p=cargarArchivo<Proveedor>(proveedor,*archivoprov,i);
            if(p.activo){
            cout<<"ID: "<<p.id<<" | Nombre: "<<p.nombre<<"\n";
            cout<<"X: "<<x<<endl;
            x++;
            }
        }
        archivoprov->clear();

}

void Proveedor::eliminarProveedor(archivoHeader proveedores,fstream& archivop,int id){
    if(!archivop){
        cout<<"No se puede abrir el archivo de proveedores: "<<"."<<endl;
        return;
    }

    Proveedor p;
    bool encontrado = false;
    int total = 0,pos;
    for(int i=0;i<proveedores.cantidadRegistros;i++){
        p=cargarArchivo<Proveedor>(proveedores,archivop,i);
        if(p.id == id){
            encontrado = true;
            pos=archivop.tellg()/sizeof(Proveedor)-1;
            archivop.clear();
            continue;
        }
    archivop.clear();
    }
    p.activo=0;
    guardarArchivo(proveedores,archivop,p,pos);
    proveedores.cantidadRegistros--;
    proveedores.registrosActivos--;
    if(proveedores.cantidadRegistros > 0) proveedores.cantidadRegistros--;
    cout<<"Proveedor con ID "<<id<<" eliminado correctamente."<<endl;
}

void Proveedor::editarProveedor(archivoHeader proveedor,fstream &archivop, int idProveedor){
    
    if(!archivop){
        cout<<"No se puede abrir el archivo de proveedores para edición."<<endl;
        return;
    }
    Proveedor p;
    int posicion;
    for(int i=0;i<proveedor.cantidadRegistros;i++){
        p=cargarArchivo<Proveedor>(proveedor,archivop,i);
        if(p.id==idProveedor){
            archivop.clear();
            posicion=archivop.tellg()/sizeof(Proveedor)-1;
             cout<<"Proveedor encontrado: ID: "<<p.id<<" | Nombre: "<<p.nombre<<"en la posicion: "<<posicion+1<<endl;
            break;
        }
        archivop.clear();
    }  
    
    string nuevaLinea;
    int respuesta;
    do{
        cout<<"Que desea editar del proveedor?\n";
        cout<<"1. Id\n";
        cout<<"2. Nombre\n";
        cout<<"3. Guardar cambios\n";
        cout<<"4. Eliminar proveedor\n";
        cout<<"0. Cancelar\n";
        cin>>respuesta;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch(respuesta){
            case 1:
                cout<<"Ingrese nuevo ID de proveedor (0 para cancelar): ";
                if(!(cin>>p.id)){ cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); cout<<"Entrada inválida."<<endl; break; }
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                if(p.id==0){ cout<<"Edición cancelada."<<endl; break; }
                break;
            case 2:
                cout<<"Ingrese el nuevo nombre del proveedor: ";
                getline(cin, nuevaLinea);
                while(nuevaLinea.empty()){ cout<<"El nombre no puede estar vacío. Ingrese de nuevo: "; getline(cin,nuevaLinea); }
                strncpy(p.nombre, nuevaLinea.c_str(), sizeof(p.nombre)-1);
                break;
            case 3:
                cout<<"Guardando cambios...\n";\
                {
                    guardarArchivo(proveedor, archivop,p);
                }
                cout<<"Proveedor actualizado."<<endl;
                return;
            case 4:
                p.eliminarProveedor(proveedor,archivop, idProveedor);
                return;
            case 0:
                cout<<"Edición cancelada."<<endl;
                return;
            default:
                cout<<"Opción inválida."<<endl;
        }
    } while(respuesta != 0);
}

Proveedor* Proveedor::buscarProveedor(archivoHeader proveedor,fstream* archivo,int id,string nombre,int opcion){
    const char* ruta = "proveedores.bin";
    if(!archivo){
        cout<<"No se puede abrir el archivo de proveedores."<<endl;
        return nullptr;
    }
    int x=0;
    Proveedor p;
    Proveedor* parreglo=new Proveedor[proveedor.cantidadRegistros];
    Proveedor* parregloencontrados=new Proveedor[proveedor.cantidadRegistros];
    for(int i=0;i<proveedor.cantidadRegistros;i++){
        p=cargarArchivo<Proveedor>(proveedor,*archivo,i);
        cout<<"id:"<<p.id<<endl;
        parreglo[x]=p;
        x++;
    }
    if(opcion == 1){
        cout<<"aqui:"<<endl;
        archivo->seekg(0,ios::beg);
        for(int i=0;i<proveedor.cantidadRegistros;i++){
            archivo->read(reinterpret_cast<char*>(&p), sizeof(Proveedor));
            cout<<"id:"<<p.id<<endl;
            if(p.id == id){
                Proveedor* result = &p;
                cout<<"Proveedor encontrado: ID: "<<result->id<<" | Nombre: "<<result->nombre<<"\n";
                return result;
            }
        }
        cout<<"Proveedor con ID "<<id<<" no encontrado."<<endl;
        return nullptr;
    }

    if(opcion == 2){
        x=0;
        for(int i=0; i<proveedor.cantidadRegistros; i++){
            string s=parreglo[i].nombre;
            if(s.find(nombre) != string::npos){
                parregloencontrados[x]=parreglo[i];
                x++;
            }
        }
        if(x==0){
            cout<<"No se encontró proveedor con el nombre "<<nombre<<"."<<endl;
            return nullptr;
        }
        cout<<"Se encontraron "<<x<<" coincidencias:\n";
        for(int i=0; i<x; i++){
            const auto &pr = parregloencontrados[x];
            cout<<i+1<<". ID="<<pr.id<<" Nombre="<<pr.nombre<<"\n";
        }
        int opcion2;
        do{ cout<<"Seleccione número de proveedor (1-"<<x<<") : "; cin>>opcion2; } while(opcion2<1 || opcion2>x);
        Proveedor* result = new Proveedor(parregloencontrados[opcion2-1]);
        return result;
    }

    cout<<"Opción de búsqueda inválida."<<endl;
    return nullptr;
}
