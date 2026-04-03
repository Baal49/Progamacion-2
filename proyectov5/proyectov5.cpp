#include <iostream>
#include <locale>
#include <limits>
#include <string>
#include <cstring>
#include <iomanip>
#include <fstream>
#include <vector>
#include "PROVEEDOR.hpp"
#include "HEADER.hpp"
#include "PRODUCTO.hpp"
using namespace std;

class Productoventa{
    public:
    int idt;
    int idprod;
    int cantidad;
    int preciounidad;
};
class Transaccion {
    public:
    int idt=0;
    int idcp=0;
    int tipo;
    float total;
    char fecha[11];
    char descripcion[200];//opcional
};

class Cliente{
public:
 int cedula;
 char nombre[100];
 char correo[100];
 char telefono[20];
 char dirreccion[200];
 char fecharegitro[11]; //formato:YYYY-MM-DD
};
class Tienda {
};
/*void inicializarTienda(Tienda* tienda){
    if(!tienda) return;
    cin.ignore(numeric_limits<streamsize>::max(),'\n');
    cout<<"Ingrese el RIF de la tienda: ";
    cin.getline(tienda->rif,20);

    // Productos
    int cap = 5;
    /*tienda->capacidadProductos = cap;
    tienda->productos = new Producto[tienda->capacidadProductos];
    tienda->siguienteIdProducto = 1;

    // Proveedores (mínimo 1 para permitir crear productos que referencien proveedores)
    tienda->capacidadProveedores = cap;
    tienda->proveedores = new Proveedor[tienda->capacidadProveedores];

    // Transacciones
    tienda->capacidadTransacciones = cap;
    tienda->transacciones = new Transaccion[cap];
    //Clientes
    tienda->capacidadClientes=cap;
    tienda->clientes=new Cliente[cap];
}*/
// Busca transacciones por ID de producto usando fichero binario
void buscarTransaccionesPorProducto(Tienda* tienda, int idProducto){
    if(tienda==nullptr){ cout<<"Tienda no inicializada."<<endl; return; }
    const char* ruta = "transacciones.bin";
    fstream archivo(ruta, ios::binary | ios::in);
    if(!archivo){ cout<<"No se puede abrir el archivo de transacciones: "<<ruta<<". Asegurese de que exista."<<endl; return; }
    archivo.seekg(0, ios::end);
    streampos tam = archivo.tellg();
    if(tam < (streampos)sizeof(Transaccion)){
        cout<<"No hay transacciones registradas en el archivo binario."<<endl;
        return;
    }
    archivo.seekg(0, ios::beg);

    Transaccion t;
    int encontrados = 0;
    cout<<"Transacciones que tienen de referencia la ID "<<idProducto<<":\n"; // mouestra solo transacciones relacionadas con el producto que introdujo el usuario
    while(archivo.read(reinterpret_cast<char*>(&t), sizeof(Transaccion))){
       /* if(t.idRelacionado == idProducto || t.id == idProducto){
            encontrados++;
            cout<<"ID: "<<t.id<<" | Tipo: "<<t.tipo<<" | Cantidad: "<<t.cantidad
                <<" | Precio unitario: "<<t.precioUnitario<<" | Total: "<<t.total
                <<" | Fecha: "<<t.fecha<<" | Descripcion: "<<t.descripcion<<"\n";
        }*/
    }
    archivo.close();
    if(encontrados==0){
        cout<<"No se encontraron transacciones para el producto con ID "<<idProducto<<"."<<endl;
    } else {
        cout<<"Se encontraron "<<encontrados<<" transaccion(es)."<<endl;
    }
}

    void Crearcliente(archivoHeader& cliente,fstream& archivoc){
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

            archivoc.clear();
            Cliente* cp=new Cliente[cliente.cantidadRegistros+1];
            archivoc.seekg(0, ios::beg);
            Cliente c;
            int x=0;
            while(archivoc.read(reinterpret_cast<char*>(&c), sizeof(Cliente))||cliente.cantidadRegistros>x){
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
            archivoc.write(reinterpret_cast<char*>(&temp),sizeof(cliente));
            cout<<"Clientes guardados en archivo binario."<<endl;
            cliente.cantidadRegistros=cliente.cantidadRegistros+1;
            cliente.proximoID=cliente.proximoID+1;
            cliente.registrosActivos=cliente.registrosActivos+1;
        }
        
        
    }
        void Editarcliente (Tienda* tienda, int cedula){
            if(tienda==nullptr){ cout<<"Tienda no inicializada."<<endl; return; }
            const char* ruta = "clientes.bin";
            fstream archivo(ruta, ios::binary | ios::in);
            if(!archivo){ cout<<"No se puede abrir el archivo de clientes: "<<ruta<<". Asegurese de que exista."<<endl; return; }
            vector<Cliente> clientes;
            Cliente c;
            while(archivo.read(reinterpret_cast<char*>(&c), sizeof(Cliente))){ clientes.push_back(c); }
            archivo.close();
            if(clientes.empty()){ cout<<"No hay clientes registrados en el archivo binario."<<endl; return; }

            int indice=-1;
            for(int i=0;i<(int)clientes.size();i++){
                if(clientes[i].cedula==cedula){ indice=i; break; }
            }
            if(indice<0){ cout<<"Cliente con cedula "<<cedula<<" no encontrado."<<endl; return; }

            cout<<"Cliente encontrado: "<<clientes[indice].nombre<<" | Cedula: "<<clientes[indice].cedula<<" | Correo: "<<clientes[indice].correo<<" | Telefono: "<<clientes[indice].telefono<<" | Dirreccion: "<<clientes[indice].dirreccion<<" | Fecha de registro: "<<clientes[indice].fecharegitro<<endl;
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
                    strncpy(clientes[indice].nombre,input.c_str(),sizeof(clientes[indice].nombre)-1);
                    break;
                case 2:
                    cout<<"Ingrese el nuevo correo del cliente (o 'CANCELAR' para cancelar): ";
                    if(!getline(cin,input)) return;
                    if(input=="CANCELAR" || input=="0"){ cout<<"Edición cancelada."<<endl; return; }
                    strncpy(clientes[indice].correo,input.c_str(),sizeof(clientes[indice].correo)-1);
                    break;
                case 3:
                    cout<<"Ingrese el nuevo telefono del cliente (o 'CANCELAR' para cancelar): ";
                    if(!getline(cin,input)) return;
                    if(input=="CANCELAR" || input=="0"){ cout<<"Edición cancelada."<<endl; return; }
                    strncpy(clientes[indice].telefono,input.c_str(),sizeof(clientes[indice].telefono)-1);
                    break;
                case 4:
                    cout<<"Ingrese la nueva dirreccion del cliente (o 'CANCELAR' para cancelar): ";
                    if(!getline(cin,input)) return;
                    if(input=="CANCELAR" || input=="0"){ cout<<"Edición cancelada."<<endl; return; }
                    strncpy(clientes[indice].dirreccion,input.c_str(),sizeof(clientes[indice].dirreccion)-1);
                    break;
                case 5:
                    cout<<"Ingrese la nueva fecha de registro del cliente (o 'CANCELAR' para cancelar): ";
                    if(!getline(cin,input)) return;
                    if(input=="CANCELAR" || input=="0"){ cout<<"Edición cancelada."<<endl; return; }
                    strncpy(clientes[indice].fecharegitro,input.c_str(),sizeof(clientes[indice].fecharegitro)-1);
                    break;
                default:
                    cout<<"Opción inválida. Edición cancelada."<<endl; return;
            }
            fstream salida(ruta, ios::binary | ios::out | ios::trunc);
            if(!salida){ cout<<"No se puede guardar el archivo de clientes."<<endl; return; }
            for(const Cliente &cli : clientes){ salida.write(reinterpret_cast<const char*>(&cli), sizeof(Cliente)); }
            salida.close();
            cout<<"Cliente editado y guardado en archivo binario."<<endl;
        }
    void listarcliente(archivoHeader cliente,fstream* archivoc,int posi){
        if(!archivoc){ cout<<"archivo no inicializado."<<endl; return; }
        if(cliente.cantidadRegistros<=0){ cout<<"No hay Clientes registrados."<<endl; return; }
        cout<<"Listado de Clientes:\n";
        Cliente p;
        int x=0;
        archivoc->clear();
        archivoc->seekg(0,ios::beg);
        while(archivoc->read(reinterpret_cast<char*>(&p),sizeof(Proveedor))||x<cliente.cantidadRegistros){
            cout<<"cedula: "<<p.cedula<<" | Nombre: "<<p.nombre<<" | Telefono: "<<p.telefono<<" | Correo: "<<p.correo<<" | Direccion: "<<p.dirreccion<<" | Fecha de registro: "<<p.fecharegitro<<endl;
            cout<<"X: "<<x<<endl;
            x++;
        }
        archivoc->clear();

}
    void listartransaccion(archivoHeader transaccion,archivoHeader pt,fstream& archivot,fstream& archivopt,int posi){
        if(!archivot){ cout<<"archivo no inicializado."<<endl; return; }
        if(transaccion.cantidadRegistros<=0){ cout<<"No hay transacciones registrados."<<endl; return; }
        cout<<"Listado de transacciones:\n";
        Transaccion p;
        Productoventa ptt;
        int x=0;
        archivopt.clear();
        archivot.clear();
        archivot.seekg(0,ios::beg);
        archivopt.seekg(0,ios::beg);
        while(archivot.read(reinterpret_cast<char*>(&p),sizeof(Proveedor))||x<transaccion.cantidadRegistros){
            cout<<"id: "<<p.idt<<" | Tipo de Transaccion: "<<p.tipo<<" | Sub Total: "<<p.total<<" | Fecha de registro: "<<p.fecha<<endl;
            int y=0;
            cout<<"Productos de la venta con id: "<<p.idt<<endl;
            while(archivopt.read(reinterpret_cast<char*>(&ptt),sizeof(Productoventa))||pt.cantidadRegistros>y){
                cout<<"id: "<<ptt.idprod<<" | Precio de la unidad: "<<ptt.preciounidad<<" | Cantidad: "<<ptt.cantidad<<endl;
                y++;
                archivopt.clear();
            }
            cout<<"X: "<<x<<endl;
            x++;
        }
        archivot.clear();

}
    void Buscartransaccion (archivoHeader&transaccion,archivoHeader pt,fstream& archivot,fstream& archivopt,int id){
        if(!archivot.is_open()){ cout<<"Tienda no inicializada."<<endl; return; }
        /*if(transaccion.cantidadRegistros<=0){
            cout<<"No hay transacciones registradas en el archivo binario."<<endl;
            return;
        }*/
        archivot.clear();
        archivot.seekg(0, ios::beg);
        archivot.seekp(0,ios::beg);
        archivopt.clear();
        archivopt.seekg(0,ios::beg);
        archivopt.seekp(0,ios::beg);
        Transaccion t;
        Productoventa p;
        int x=0;
        bool encontrado = false;
        while(archivot.read(reinterpret_cast<char*>(&t), sizeof(Transaccion))||transaccion.cantidadRegistros>x){
            cout<<"Transacción : ID: "<<t.idt<<" | Tipo: "<<t.tipo<<" | Fecha: "<<t.fecha<<" | Descripcion: "<<t.descripcion<<"\n";
            if(t.idt == id){
                cout<<"Transacción encontrada: ID: "<<t.idt<<" | Tipo: "<<t.tipo<<" | Fecha: "<<t.fecha<<" | Descripcion: "<<t.descripcion<<"\n";
                encontrado = true;
                while(archivopt.read(reinterpret_cast<char*>(&p),sizeof(Productoventa))){
                    if(t.idt==p.idt){
                        cout<<"id producto: "<<p.idprod<<" precio unitario:"<<p.preciounidad<<" cantidad: "<<p.cantidad<<endl;
                    }
                    archivot.clear();
                }
                archivot.clear();
                return;
            }
            archivot.clear();
            x++;
        }
        if(!encontrado){
            cout<<"Transacción con ID "<<id<<" no encontrada."<<endl;
        }
    }

Cliente buscarCliente(archivoHeader& cliente,fstream& archivoc,int cedula,string &nombre,string &direccion){
    Cliente c;
    if(cliente.cantidadRegistros<=0){ cout<<"Tienda no inicializada."<<endl; return c; }
    const char* ruta = "clientes.bin";
    fstream archivo(ruta, ios::binary | ios::in);
    if(!archivo){
        cout<<"No se puede abrir el archivo de clientes: "<<ruta<<". Asegurese de que exista."<<endl;
        return c;
    }
    archivo.seekg(0, ios::end);
    streampos tam = archivo.tellg();
    if(cliente.cantidadRegistros<=0){
        cout<<"No hay clientes registrados en el archivo binario."<<endl;
        return c;
    }
    archivo.seekg(0, ios::beg);
    int x=0;
    
    while(archivo.read(reinterpret_cast<char*>(&c), sizeof(Cliente))||cliente.cantidadRegistros>x){
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


void editarProducto(archivoHeader &producto,fstream &archivo,fstream* archivoprov, int idProducto){
    //Funcion para editar algun aspecto del producto
    
    int idBuscado=0;
    int provvaalido=0,pos;
    bool encontrado=false;
    int resp;
    Producto temp,p;
    Proveedor prov;
    cout<<"id producto: "<<idProducto;
    int i=0;
    string respp;
    archivo.clear();
    archivo.seekg(0,ios::beg);
    while(archivo.read(reinterpret_cast<char*>(&p),sizeof(Producto))){
        if(p.id==idProducto){
            idBuscado=i;
            pos=archivo.tellg()/sizeof(Producto);
            archivo.clear();
            encontrado=true;
            break;
        }
        else{
            archivo.clear();
            i++;
        }
    }
    if(!encontrado){
        cout<<"Producto no encontrado."<<endl;
        return;
    } 
    temp = p;
    int ola=0;
    do{
    
    cout <<"Que desea editar del producto?"<<endl;
        cout <<"1. Código"<<endl;
        cout <<"2. Nombre"<<endl;
        cout <<"3. Descripción"<<endl;
        cout <<"4. Proveedor"<<endl;
        cout <<"5. Precio"<<endl;
        cout <<"6. Stock"<<endl;
        cout <<"7. Fecha de Registro"<<endl;
        cout <<"8. Fecha de vencimiento"<<endl;
        cout <<"9. Eliminar producto"<<endl;
        cout <<"10. Guardar cambios"<<endl;
        cout <<"0. Cancelar sin guardar"<<endl;
        cin>>ola;
        cin.ignore(numeric_limits<streamsize>::max(),'\n');
        switch(ola){
            case 1:
                // Editar código (validar único)
                cout <<"Ingrese el nuevo código del producto o 0 para cancelar: ";
                getline(cin,respp);
                   while(p.codigoDuplicado(&producto,&archivo, respp)) { cout<<"Codigo ya existe Intentelo nuevamente."<<endl; getline(cin,respp); }
                   if (respp.empty()){ cout<<"El código no puede estar vacío. Edición cancelada."<<endl; break; }
                   if ( respp=="0"||respp=="CANCELAR"){ cout<<"Edición cancelada."<<endl; break; }
                // Asignar nuevo código al producto
                strcpy(temp.codigo,respp.c_str());
                break;
            case 2:
                // Editar nombre.
                
                cout<<"Ingrese el nuevo nombre del producto: ";
                getline(cin,respp);
                while (respp.empty()){ cout<<"El nombre no puede estar vacío."<<endl; getline(cin,respp); }
                while (respp.length() >= sizeof(temp.nombre)){ cout<<"El nombre es demasiado largo. Coloque otro nombre."<<endl; getline(cin,respp); }
                // Asignar nuevo nombre al producto
                if (respp=="CANCELAR" || respp=="0"){ cout<<"Edición cancelada."<<endl; break; }
                strcpy(temp.nombre,respp.c_str());
                respp.clear();
                break;
            case 3:
                // Editar descripción
                cout<<"ingrese la nueva descripcion del producto: ";
                getline(cin,respp);
                while(respp.empty()){cout<<"La descripcion no puede estar vacia."<<endl;getline(cin,respp);}
                while(respp.length()>sizeof(temp.descripcion)){cout<<"La descripcion es demasiado larga."<<endl; getline(cin,respp);}
                if (respp=="CANCELAR" || respp=="0"){ cout<<"Edición cancelada."<<endl; break; }
                strcpy(temp.descripcion,respp.c_str());
                cout<<"Descripción actualizada."<<endl;
                break;
            case 4:
                // Editar proveedor (validar existencia)
                    cout<<"Ingrese el nuevo ID del proveedor del producto: ";
                    cin>>resp;
                    
                    while(provvaalido==0){
                    while(archivoprov->read(reinterpret_cast<char*>(&prov),sizeof(Proveedor))){
                        if(prov.id==resp){
                            provvaalido=1;
                            break;
                        }
                    }
                    if(provvaalido==0){
                        cout<<"el ID del provedor no existe. Intentelo nuevamente."<<endl;
                        break;
                    }
                }
                temp.idProveedor=resp;
                cout<<"Proveedor actualizado."<<endl;
                break;
            case 5:
                // Editar precio (validar > 0)
                    cout<<"Ingrese el nuevo precio del producto: ";
                    float precio;
                    cin>>precio;
                    while(precio<=0){cout<<"El precio debe ser mayor a 0 Intentelo nuevamente."<<endl; cin>>precio;}
                    temp.precio=precio;
                    cout<<"Precio actualizado."<<endl;
                break;
            case 6:
                // Editar stock (validar > 0)
                cout<<"Ingrese el nuevo stock del producto: ";
                float stock;
                cin>>stock;
                while(stock<=0){cout<<"El stock debe ser mayor a 0 Intentelo nuevamente."<<endl; cin>>stock;}
                temp.stock=stock;
                cout<<"Stock actualizado."<<endl;
                break;
            case 7:
                // Editar fecha de registro
                cout<<"ingrese la nueva fecha de registro del producto con Formato(YYYY-MM-DD): ";
                getline(cin,respp);
                while(respp.empty()){cout<<"La fecha de registro no puede estar vacia."<<endl;getline(cin,respp);}
                while(respp.length()>sizeof(temp.fechaRegistro)){cout<<"La fecha de registro es demasiado larga."<<endl; getline(cin,respp);}
                if (respp=="CANCELAR" || respp=="0"){ cout<<"Edición cancelada."<<endl; break; }
                strcpy(temp.fechaRegistro,respp.c_str());
                cout<<"Fecha de registro actualizada."<<endl;
                break;
            case 8:
                // Editar fecha de vencimiento
                cout<<"ingrese la nueva fecha de vencimiento del producto con Formato(YYYY-MM-DD): ";
                getline(cin,respp);
                while(respp.empty()){cout<<"La fecha de vencimiento no puede estar vacia."<<endl;getline(cin,respp);}
                while(respp.length()>sizeof(temp.fechavencimiento)){cout<<"La fecha de vencimiento es demasiado larga."<<endl; getline(cin,respp);}
                if (respp=="CANCELAR" || respp=="0"){ cout<<"Edición cancelada."<<endl; break; }
                strcpy(temp.fechavencimiento,respp.c_str());
                cout<<"Fecha de vencimiento actualizada."<<endl;
                break;
            
            case 9:
                // Eliminar producto (confirmar antes)
                cout<<"¿Está seguro que desea eliminar este producto? (S/N): ";
                cin >> resp;
                if(resp=='S'||resp=='s'){
                    //eliminarProducto(tienda, idProducto);
                    cout<<"Producto eliminado."<<endl;
                    return;
                }
                else{
                    cout<<"Eliminación cancelada."<<endl;
                }
                break;
            
            case 10:
                // Guardar cambios (confirmar antes)
                cout<<"Producto antes: ID: "<<p.id<<" | Codigo: "<<p.codigo<<" | Nombre: "<<p.nombre<<" | Precio: "<<p.precio<<" | Stock: "<<p.stock<<" | Proveedor ID: "<<p.idProveedor<<" | Fecha de Registro: "<<p.fechaRegistro<<" | Fecha de Vencimiento: "<<p.fechavencimiento<<endl;
                cout<<"Producto Despues: ID: "<<temp.id<<" | Codigo: "<<temp.codigo<<" | Nombre: "<<temp.nombre<<" | Precio: "<<temp.precio<<" | Stock: "<<temp.stock<<" | Proveedor ID: "<<temp.idProveedor<<" | Fecha de Registro: "<<temp.fechaRegistro<<" | Fecha de Vencimiento: "<<temp.fechavencimiento<<endl;
                cout<<"¿Desea guardar los cambios realizados al producto? (S/N): ";
                //tienda->productos[idBuscado]=temp;
                archivo.clear();
                archivo.seekp((pos-1)*sizeof(Producto),ios::beg);
                archivo.write(reinterpret_cast<char*>(&temp),sizeof(Producto));
                archivo.clear();
                cout<<"Cambios guardados."<<endl;
                break;
            case 0:
                cout<<"Edición cancelada sin guardar."<<endl;
                break;
            default:
                cout<<"Opción inválida."<<endl;
        }
    }while(ola!=0&&ola!=7);
}

void actualizarheader(archivoHeader header,fstream& archivo){
    int x=0;
    if(!archivo.is_open()){
        cout<<"No se pudo abrir el archivo"<<endl;
        return;
    }
        archivo.clear();
        archivo.seekg(0,ios::beg);
        archivo.write(reinterpret_cast<char*>(&header),sizeof(archivoHeader));
        archivo.clear();

}
void venta(archivoHeader& producto,archivoHeader transaccion,archivoHeader& cliente,archivoHeader& pt,fstream& archivop,fstream& archivoc,fstream& archivot,fstream& archivopt){
    Producto p;
    Cliente c;
    int cedu;
    Productoventa venta;
    Transaccion t;
    string input;
    string respuesta;
    string nombre, direccion;
    cout<<"Inserte la cedula del cliente";
    cin>>cedu;
    c=buscarCliente(cliente,archivoc, cedu, nombre, direccion);
    archivopt.clear();
    if(pt.cantidadRegistros>0){
        archivopt.seekp((transaccion.cantidadRegistros+1)*sizeof(Productoventa),ios::beg);
    }
    else{
        archivopt.seekp((0)*sizeof(Productoventa),ios::beg);
    }
    if(c.cedula==0){
        cout<<"Cliente no encontrado. Intente de nuevo o Cree otro cliente";
            cout<<"Venta cancelada."<<endl;
            return;
        }
    else {
        cout<<"Nombre del cliente: "<<nombre<<" Cedula: "<<cedu<<" Direccion: "<<direccion<<"\n";
        int opt,id,sub,cantp=1;
        bool cantver=0;
        do{
            int cant;
            cout<<"introduce el id del producto que se va a llevar: ";
            cin>>id;
            p=p.buscarProducto(&producto,archivop,id,"",1);
            cout<<"El precio del producto es: "<<p.precio<<endl;
            venta.idprod=p.id;
            venta.preciounidad=p.precio;
            do{
            cout<<"introduce la cantidad del producto que se va a llevar: ";
            cin>>cant;
            if(cant>p.stock){
                cout<<"el producto no tiene esa cantidad intentelo de nuevo La existencia del producto es: "<<p.stock;
                cantver=1;
            }
        }while(cantver==1);
        venta.cantidad=cant;
        venta.idt=transaccion.proximoID++;
        sub+=(cant*p.precio);
        string input;
        archivopt.write(reinterpret_cast<char*>(&venta),sizeof(Productoventa));
        archivopt.flush();
        archivopt.clear();
        p.modificarstock(producto,archivop,p.id,cant,1);
        pt.cantidadRegistros++;
        pt.proximoID++;
        do{
        cin.ignore(numeric_limits<streamsize>::max(),'\n');
        cout<<"desea incluir otro producto? S para si N para no";
        getline(cin,input);
        if(input=="S"||input=="s"){
            opt=1;
        }
        else if(input=="N"||input=="n"){
            opt=0;
        }
        else{
            cout<<"Opcion no valida intentelo de nuevo."<<endl;
            opt=2;
        }
        cantp++;
    }while(opt!=1&&opt!=2);
        }while(opt==1);
        t.idt=transaccion.proximoID++;
        t.total=sub;
        t.tipo=2;
        //cin.ignore(numeric_limits<streamsize>::max(),'\n');
        string input,input2;
        cout<<"introduzca la fecha de la transaccion en formato YYYY-MM-DD: ";
        cin >> input;
        
        strncpy(t.fecha, input.c_str(), sizeof(t.fecha)-1);
        cout<<"Quieres ponerle descripcion a la transaccion? S para si N para no"<<endl;
        cin>>input2;
        if(input2=="S"||input=="s"){
            cout<<"Introduzca la descripcion de la transaccion"<<endl;
            string input3;
            strncpy(t.descripcion,input3.c_str(),sizeof(t.descripcion));
        }
        archivot.clear();
        if(transaccion.cantidadRegistros<=0){
            archivot.seekp((transaccion.cantidadRegistros+1)*sizeof(transaccion),ios::beg);
        }
        else{
            archivot.seekp(1*sizeof(transaccion),ios::beg);
        }
        archivot.write(reinterpret_cast<char*>(&t),sizeof(transaccion));
        archivot.clear();
    }
}
void compra(Tienda* tienda,archivoHeader producto, archivoHeader proveedor,archivoHeader cliente,archivoHeader& transaccion,archivoHeader& pt,fstream& archivop,fstream& archivoprov,fstream& archivoc,fstream& archivot,fstream& archivopt){
    Producto p;
    Proveedor* prov;
    Productoventa venta;
    Transaccion t;
    int idprov;
    char respuesta;
    string nombre,direccion;
    string input;
    cout<<"Inserte el id del proveedor: ";
    cin>>idprov;
    prov=prov->buscarProveedor(proveedor,&archivoprov,idprov,"",1);
    if(nombre=="no existe ese proveedor"){
        cout<<nombre<<" desea registrarlo?"<<endl;
        cout<<"Introduzca S para registrarlo o N para cancelar";
        cin>>respuesta;
        if(respuesta=='S'||respuesta=='s'){
        }
        else{
            return ;
        }
    }
    else{
        cout<<"Nombre del proveedor: "<<nombre<<" ID: "<<idprov;
        int opt,id,sub,cantp=1;
        bool cantver=0;
        int x=0;
        t.idcp=prov->id;
        archivopt.clear();
        if(pt.cantidadRegistros>0){
            archivopt.seekp((pt.cantidadRegistros+1)*sizeof(Productoventa),ios::beg);
        }
        else{
            archivopt.seekp((1)*sizeof(Productoventa),ios::beg);
        }
        do{
            int cant;
            cout<<"introduce el id del producto que se va a comprar: ";
            cin>>id;
            p=p.buscarProducto(&producto,archivop,id,"",1);
            //if(p->precio>0){
            venta.idprod=p.id;
            cout<<"El precio del producto es: "<<p.precio<<endl;
            venta.preciounidad=p.precio;
            do{
            cout<<"introduce la cantidad del producto que se va a comprar: ";
            cin>>cant;
            if(cant<=0){
                cout<<"la cantidad debe ser mayor a 0 intentelo de nuevo.";
                cantver=1;
            }
        }while(cantver==1);
        venta.cantidad=cant;
        sub+=(cant*p.precio);
        string input;
        venta.idt=transaccion.proximoID++;
        archivopt.write(reinterpret_cast<char*>(&venta),sizeof(Productoventa));
        archivopt.flush();
        archivopt.clear();
        p.modificarstock(producto,archivop,p.id,cant,2);
        pt.cantidadRegistros++;
        pt.proximoID++;
        do{
        cin.ignore(numeric_limits<streamsize>::max(),'\n');
        cout<<"desea incluir otro producto? S para si N para no"<<endl;
        getline(cin,input);
        if(input=="S"||input=="s"||input=="SI"||input=="si"){
            opt=1;
        }
        else if(input=="N"||input=="n"||input=="NO"||input=="no"){
            opt=2;
        }
        else{
            cout<<"Opcion no valida intentelo de nuevo."<<endl;
            opt=0;
        }
        cantp++;
    }while(opt!=1&&opt!=2);
        }while(opt==1);
        t.idt=transaccion.proximoID++;
        t.total=sub;
        t.tipo=1;
        //cin.ignore(numeric_limits<streamsize>::max(),'\n');
        string input,input2;
        cout<<"introduzca la fecha de la transaccion en formato YYYY-MM-DD: ";
        getline(cin,input);
        strncpy(t.fecha, input.c_str(), sizeof(t.fecha)-1);
        cout<<"Quieres ponerle descripcion a la transaccion? S para si N para no"<<endl;
        cin>>input2;
        if(input2=="S"||input=="s"){
            cout<<"Introduzca la descripcion de la transaccion"<<endl;
            string input3;
            getline(cin,input3);
            strncpy(t.descripcion,input3.c_str(),sizeof(t.descripcion));
        }
        archivot.clear();
        if(transaccion.cantidadRegistros>0){
            archivot.seekp((transaccion.cantidadRegistros+1)*sizeof(transaccion),ios::beg);
           cout<<">0"<<endl; 
        }
        else{
            archivot.seekp((0)*sizeof(transaccion),ios::beg);
            cout<<"=0"<<endl;
        }
        archivot.write(reinterpret_cast<char*>(&t),sizeof(transaccion));
        archivot.flush();
        archivot.clear();
        cout<<"Transaccion aceptada Id de la transaccion es: "<<t.idt<<endl;
        transaccion.cantidadRegistros++;
        transaccion.proximoID++;
        transaccion.registrosActivos++;
}   cout<<"cantidad de transacciones: "<<transaccion.cantidadRegistros;
}

int main(){
    Tienda tienda;
    bool existp,existprov,existc,existt,existpt;
    archivoHeader productos,proveedores,clientes,transacciones,pt;
    Producto p;
    Proveedor prov;
    Cliente c;
    Transaccion t;
    int posiprod=0,posiprov=0,posic=0,posit=0,positp=0;
    fstream archivoproductos("C:/Users/reina/Desktop/proyecto2/Progamacion-2/Proyectos/proyectov4/productos.bin",ios::binary | ios::in|ios::out);
    archivoproductos.open("C:/Users/reina/Desktop/proyecto2/Progamacion-2/Proyectos/proyectov4/productos.bin", ios::binary | ios::out);
    archivoproductos.close();
    archivoproductos.open("C:/Users/reina/Desktop/proyecto2/Progamacion-2/Proyectos/proyectov4/productos.bin", ios::binary | ios::in | ios::out);
    fstream archivoproveedores("C:/Users/reina/Desktop/proyecto2/Progamacion-2/Proyectos/proyectov4/proveedores.bin",ios::binary|ios::in|ios::out);
    archivoproveedores.open("C:/Users/reina/Desktop/proyecto2/Progamacion-2/Proyectos/proyectov4/proveedores.bin", ios::binary | ios::out);
    archivoproveedores.close();
    archivoproveedores.open("C:/Users/reina/Desktop/proyecto2/Progamacion-2/Proyectos/proyectov4/proveedores.bin",ios::binary|ios::in|ios::out);
    archivoproveedores.clear(); 
    archivoproveedores.seekp(0, ios::beg);
    archivoproveedores.seekg(0, ios::beg);
    archivoproveedores.read(reinterpret_cast<char*>(&proveedores),sizeof(archivoHeader));
    posiprov=archivoproveedores.tellg()/sizeof(archivoHeader);
    if(posiprov<=0){
        proveedores.cantidadRegistros=0;
        proveedores.proximoID=1;
        proveedores.registrosActivos=0;
        proveedores.version=0;
        existprov=0;
        cout<<"posicion actual de proveedores"<<posiprov<<endl;
        cout<<"No se encontraron proveedores."<<endl;
    }
    else{
        existprov=1;
        cout<<"posicion actual de proveedores"<<posiprov<<endl;
        cout<<"Se encontraron proveedores."<<endl;
    }
    
    archivoproveedores.clear(); 
    cout<<"El valor de proveedores es: "<<proveedores.cantidadRegistros<<endl;
    cout << "Nueva posicion de lectura: " << archivoproveedores.tellg() <<"Nueva posicion de escritura: "<<archivoproveedores.tellp()<< endl;
    fstream archivoclientes("C:/Users/reina/Desktop/proyecto2/Progamacion-2/Proyectos/proyectov4/clientes.bin",ios::binary|ios::in|ios::out);
    archivoclientes.open("C:/Users/reina/Desktop/proyecto2/Progamacion-2/Proyectos/proyectov4/clientes.bin",ios::binary|ios::out);
    archivoclientes.close();
    archivoclientes.open("C:/Users/reina/Desktop/proyecto2/Progamacion-2/Proyectos/proyectov4/clientes.bin",ios::binary|ios::in|ios::out);
    archivoclientes.clear(); 
    archivoclientes.seekp(0, ios::beg);
    archivoclientes.seekg(0, ios::beg);
    archivoclientes.read(reinterpret_cast<char*>(&clientes),sizeof(archivoHeader));
    posic=archivoclientes.tellg()/sizeof(archivoHeader);
    archivoclientes.clear(); 
    if(posic<=0){
        existc=0;
        clientes.cantidadRegistros=0;
        clientes.proximoID=0;
        clientes.registrosActivos=0;
        clientes.version=0;
        cout<<"No se encontraron clientes."<<endl;
    }
    else{
        existc=1;
        cout<<"Se encontraron clientes."<<endl;
        cout<<"El valor de clientes es: "<<clientes.cantidadRegistros<<endl;
        cout << "Nueva posicion de lectura: " <<posic<< endl;
    }
    
    fstream archivotransacciones("C:/Users/reina/Desktop/proyecto2/Progamacion-2/Proyectos/proyectov4/transacciones.bin",ios::binary|ios::in|ios::out);
    archivotransacciones.clear();
    archivotransacciones.seekg(0,ios::beg);
    archivotransacciones.seekp(0,ios::beg);
    archivotransacciones.read(reinterpret_cast<char*>(&transacciones),sizeof(archivoHeader));
    posit=archivotransacciones.tellg()/sizeof(archivoHeader);
    if(posit<=0){
        existt=0;
        transacciones.cantidadRegistros=0;
        transacciones.proximoID=0;
        transacciones.registrosActivos=0;
        transacciones.version=0;
        cout<<"No se encontraron transacciones."<<endl;
    }
    else{
        existt=1;
        cout<<"Se encontraron transacciones."<<endl<<"La cantidad de transacciones es: "<<transacciones.cantidadRegistros;
    }
    archivoproductos.clear(); 
    archivoproductos.seekg(0, ios::beg);
    archivoproductos.seekp(0,ios::beg);
    archivoproductos.read(reinterpret_cast<char*>(&productos),sizeof(archivoHeader));
    posiprod=archivoproductos.tellg()/sizeof(archivoHeader);
    archivoproductos.clear(); 
    if(posiprod<=0){
        existp=0;
        productos.cantidadRegistros=0;
        productos.proximoID=0; 
        productos.registrosActivos=0;
        productos.version=0;
        posiprod=archivoproductos.tellg()/sizeof(archivoHeader);
        cout << "Nueva posicion de lectura: " << posiprod << endl;
        cout<<"La cantidad de productos es: "<<productos.cantidadRegistros<<endl;
        cout<<"No se encontraron productos."<<endl;
        archivoproductos.clear(); 
    }
    else{
        existp=1;
        posiprod=archivoproductos.tellg()/sizeof(archivoHeader);
        cout << "Nueva posicion de lectura: " << posiprod << endl;
        cout<<"La cantidad de productos es: "<<productos.cantidadRegistros<<endl;
        archivoproductos.clear(); 
    }
    fstream archivopt("C:/Users/reina/Desktop/proyecto2/Progamacion-2/Proyectos/proyectov4/productostransaccion.bin",ios::binary|ios::in|ios::out);
    archivopt.clear();
    archivopt.seekg(0,ios::beg);
    archivopt.seekp(0,ios::beg);
    archivopt.read(reinterpret_cast<char*>(&pt),sizeof(archivoHeader));
    archivopt.clear();
    positp=archivopt.tellg()/sizeof(archivoHeader);
    if(positp<=0){
        pt.cantidadRegistros=0;
        pt.proximoID=0;
        pt.registrosActivos=0;
        pt.version=0;
        cout<<"no se encontraron productos de transacciones"<<endl;
    }
    else{
        cout<<"Se encontraron Productos de transacciones."<<endl;
    }
    int opcion;
    do{
        cin.ignore(numeric_limits<streamsize>::max(),'\n');
        int opt=0,id;
        string nombre,direccion,str;
         char* rutaproductos="C:/Users/reina/Desktop/proyecto2/Progamacion-2/Proyectos/proyectov4/productos.bin";
         char* rutaproveedores="C:/Users/reina/Desktop/proyecto2/Progamacion-2/Proyectos/proyectov4/proveedores.bin";
        cout <<"╔═══════════════════════════════════════════╗"<<endl;
        cout <<"║   SISTEMA DE GESTIÓN DE INVENTARIO        ║"<<endl;
        cout <<"║   Tienda: Farmacia pipo                   ║"<<endl;
        cout <<"╚═══════════════════════════════════════════╝"<<endl;
    
        cout <<"1. Gestión de Productos" <<endl;
        cout <<"2. Gestión de Proveedores" <<endl;
        cout <<"3. Gestión de Clientes" <<endl;
        cout <<"4. Gestión de Transacciones" <<endl;
        cout <<"5. Salir"<<endl;
        cout<<"Seleccione una opción: ";
        cin>>opcion;
        switch(opcion){
            case 1:
                cout <<"╔═══════════════════════════════════════════╗"<<endl;
                cout <<"║        GESTIÓN DE PRODUCTOS              ║"<<endl;
                cout <<"╚═══════════════════════════════════════════╝"<<endl;

                cout <<"1. Registrar nuevo producto"<<endl;
                cout <<"2. Buscar producto"<<endl;
                cout <<"3. Actualizar producto"<<endl;
                cout <<"4. Actualizar stock manualmente"<<endl;
                cout <<"5. Listar todos los productos"<<endl;
                cout <<"6. Eliminar producto"<<endl;
                cout <<"0. Volver al menú principal"<<endl;
                 cin>>opcion;
                 switch(opcion){
                    case 1:
                        p.Crearproductos(&proveedores,archivoproductos,productos,archivoproveedores,posiprod,posiprov);
                        break;
                    case 2:
                    
                        do{
                            cout<<"Quiere hacer la busqueda del producto por 1.ID o por 2.Nombre";
                            cin>>opt;
                            if(opt!=1&&opt!=2){
                            cout<<"esa opcion es invalida intentelo de nuevo.";
                            }
                        } while(opt!=1&&opt!=2);
                        if(opt==1){
                            cout<<"ingrese el id: ";
                            cin>>id;
                        }
                        else{
                            cout<<"ingrese el nombre: ";
                            getline(cin,nombre);
                        }
                        p.buscarProducto(&productos,archivoproductos,id,nombre,opt);
                        break;
                    case 3:
                        cout<<"ingrese el id del producto: ";
                        cin>>id;
                        editarProducto(productos,archivoproductos,&archivoproveedores,id);
                        break;
                    case 4:
                        p.modificarstock(productos,archivoproductos,1,1,1);
                        break;
                    case 5:
                        p.listarProductos(&archivoproductos,&archivoproveedores,productos,proveedores,posiprod,posiprov);
                        cin.ignore(numeric_limits<streamsize>::max(),'\n');
                        opt=0;
                        opcion=0;
                        break;
                    case 6:
                        cout<<"ingrese el id del del producto que se quiere borrar";
                        cin>>id;
                        p.eliminarProducto(productos,archivoproductos, id); 
                        break;
                    case 0:
                        cout<<"Volviendo al menú principal..."<<endl;
                        break;
                    default:
                        cout<<"Opción inválida. Intente nuevamente."<<endl;
                 }
                break;
            case 2:
                cout <<"╔═══════════════════════════════════════════╗"<<endl;
                cout <<"║       GESTIÓN DE PROVEEDORES             ║"<<endl;
                cout <<"╚═══════════════════════════════════════════╝"<<endl;

                cout <<"1. Registrar proveedor"<<endl;
                cout <<"2. Buscar proveedor"<<endl;
                cout <<"3. Actualizar proveedor"<<endl;
                cout <<"4. Listar proveedores"<<endl;
                cout <<"5. Eliminar proveedor"<<endl;
                cout <<"0. Volver al menú principal"<<endl;
                cout<<"Seleccione una opción: ";
                cin>>opcion;
                
                switch(opcion){
                    case 1:
                        prov.Crearproveedor(proveedores,&archivoproveedores,posiprov);
                        cout<<"cantidad de registros"<<proveedores.cantidadRegistros<<endl;
                        break;
                    case 2:
                        cin.ignore(numeric_limits<streamsize>::max(),'\n');
                        opcion=0;
                        while(opcion!=1&&opcion!=2){
                            cout<<"Quiere hacer la busqueda del proveedor por 1.ID o por 2.Nombre";
                            cin>>opcion;
                            if(opcion!=1&&opcion!=2){
                                cout<<"esa opcion es invalida intentelo de nuevo.";
                            }
                        }
                        if(opcion==1){
                            cout<<"ingrese el id del proveedor a buscar";
                            cin>>id;
                        }
                        if(opcion==2){
                            cout<<"ingrese el nombre del proveedor a buscar";
                            getline(cin,nombre);
                        }
                        prov.buscarProveedor(productos,&archivoproductos,id,nombre,opcion);
                        break;
                    case 3:
                        cout<<"ingrese el id del del producto que se quiere editar";
                        cin>>id;
                        prov.editarProveedor(proveedores,&archivoproveedores,id);
                        break;
                    case 4:
                        prov.listarProveedores(proveedores,&archivoproveedores,posiprov);
                        break;
                    case 5:
                        cout<<"ingrese el id del del producto que se quiere borrar";
                        cin>>id;
                        prov.eliminarProveedor(proveedores,&archivoproveedores, id); 
                        cin.ignore(numeric_limits<streamsize>::max(),'\n');
                        opt=0;
                        opcion=0;
                        break;
                    case 0:
                        cout<<"Volviendo al menú principal..."<<endl;
                        break;
                    default:
                        cout<<"Opción inválida. Intente nuevamente."<<endl;
                }
                break;
            case 3:
                cout <<"╔═══════════════════════════════════════════╗"<<endl;
                cout <<"║         GESTIÓN DE CLIENTES              ║"<<endl;
                cout <<"╚═══════════════════════════════════════════╝"<<endl;

                cout <<"1. Registrar cliente"<<endl;
                cout <<"2. Buscar cliente"<<endl;
                cout <<"3. Actualizar cliente"<<endl;
                cout <<"4. Listar clientes"<<endl;
                cout <<"5. Eliminar cliente"<<endl;
                cout <<"0. Volver al menú principal"<<endl;
                cout <<"Seleccione una opción: ";
                cin  >>opcion;
                
                switch(opcion){
                    case 1:
                        Crearcliente(clientes,archivoclientes);
                        break;
                    case 2:{
                        cout<<"Inserte la cedula del cliente";
                        cin>>id;
                        {
                            string nombre, direccion;
                            Cliente c=buscarCliente(clientes,archivoclientes,id,nombre,direccion);
                            if(c.cedula!=0){
                                cout<<"Cliente encontrado: "<<nombre<<" | Cedula: "<<id<<" | Direccion: "<<direccion<<"\n";
                            } else {
                                cout<<"Cliente no encontrado."<<endl;
                            }
                        }
                        break;
                    }
                    case 3:
                        cout<<"Inserte la cedula del cliente";
                        cin>>id;
                        Editarcliente(&tienda,id);
                        break;
                    case 4:
                        listarcliente(clientes,&archivoclientes,posic);
                        break;
                    case 5:
                       /* cout<<"Introduzca el id del cliente que quiere borrar";
                        cin>>id;
                        eliminarCliente(&tienda, id); // Example ID for deletion*/
                        break;
                    case 0:
                        cout<<"Volviendo al menú principal..."<<endl;
                        break;
                    default:
                        cout<<"Opción inválida. Intente nuevamente."<<endl;
                }
                break;
            case 4:
                cout <<"╔═══════════════════════════════════════════╗"<<endl;
                cout <<"║         GESTIÓN DE Transacciones          ║"<<endl;
                cout <<"╚═══════════════════════════════════════════╝"<<endl;
                cout <<"1. Registrar compra"<<endl;
                cout <<"2. Registrar venta"<<endl;
                cout <<"3. Buscar transaccion"<<endl;
                cout <<"4. Actualizar transaccion"<<endl;
                cout <<"5. Listar transacciones"<<endl;
                cout <<"6. Eliminar transacciones"<<endl;
                cout <<"0. Volver al menú principal"<<endl;
                cout <<"Seleccione una opción: ";
                cin  >>opcion;
                switch(opcion){
                    case 1:
                        compra(&tienda,productos,proveedores,clientes,transacciones,pt,archivoproductos,archivoproveedores,archivoclientes,archivotransacciones,archivopt);
                        break;
                    case 2:
                        venta(productos,transacciones,clientes,pt,archivoproductos,archivoclientes,archivotransacciones,archivopt);
                        break;
                    case 3:
                        cout<<"Introdusca el id de la transaccion: ";
                        cin>>id;
                        Buscartransaccion(transacciones,pt,archivotransacciones,archivopt,id);
                        break;
                    case 4:
                        listartransaccion(transacciones,pt,archivotransacciones,archivopt,posit);
                        break;
                    case 5:
                    cin.ignore(numeric_limits<streamsize>::max(),'\n');
                        //eliminarCliente(&tienda, 1); 
                        opcion=0;
                        break;
                    case 0:
                        cout<<"Volviendo al menú principal..."<<endl;
                        break;
                    default:
                        cout<<"Opción inválida. Intente nuevamente."<<endl;
                }
                break;
            case 5:
                cout<<"Saliendo del programa..."<<endl;
                actualizarheader(productos,archivoproductos);
                actualizarheader(proveedores,archivoproveedores);
                archivoclientes.seekg(0,ios::beg);
                archivoclientes.write(reinterpret_cast<char*>(&clientes),sizeof(archivoHeader));
                archivoclientes.close();
                archivoproductos.close();
                archivoproveedores.close();
                archivotransacciones.close();
                break;
            default:
                cout<<"Opción inválida. Intente nuevamente."<<endl;
        }
}while(opcion!=5);

}

