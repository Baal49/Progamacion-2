#include "PRODUCTO.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include "HEADER.hpp"
#include "PROVEEDOR.hpp"
#include <cstring>
#include <iomanip>
using namespace std;
    int Producto::getidp(){
        return id;
    }
    const char* Producto::getcodigo(){
        return codigo;
    }
    const char* Producto::getnombre(){
        return nombre;
    }
    const char* Producto::getdeescripcion(){
        return descripcion;
    }
    int Producto::getidProveedor(){
        return idProveedor;
    }
    float Producto::getprecio(){
        return precio;
    }
    int Producto::getstock(){
        return stock;
    }
    const char* Producto::getfechaRegistro(){
        return fechaRegistro;
    }
    const char* Producto::getfechavencimiento(){
        return fechavencimiento;
    }
    bool Producto::getregistroactivo(){
        return registroactivo;
    }

    void Producto::Crearproductos(archivoHeader* proveedores,fstream& archivoproducto,archivoHeader& tienda,fstream& archivoproveedor,int posiprod,int posiprov){
    string input;
    int cantidad = 0;
    if(archivoproducto.is_open()){
        cout<<"Ingrese la cantidad de productos a crear (o 'CANCELAR'/'0' para cancelar): ";
    while(true){
        if(!getline(cin, input)) return;
        if(input=="CANCELAR" || input=="0") { cout<<"Creación cancelada."<<endl; return; }
        try{ cantidad = stoi(input); }
        catch(...){ cout<<"Entrada invalida. Ingrese un numero: "; continue; }
        if(cantidad>0) break;
        cout<<"La cantidad debe ser mayor a 0. Intente nuevamente: ";
    }
    for(int n=0;n<cantidad;n++){
        Producto temp;

        // Codigo (único)
        while(true){
            cout<<"Ingrese el codigo del producto (o 'CANCELAR' para cancelar): ";
            if(!getline(cin, input)) return;
            if(input=="CANCELAR" || input=="0"){ cout<<"Creación cancelada."<<endl; return; }
            if(input.empty()){ cout<<"El codigo no puede estar vacío."<<endl; continue; }
            //if(codigoDuplicado(tienda, input)) { cout<<"Codigo ya existe. Ingrese otro."<<endl; continue; }
            strncpy(temp.codigo, input.c_str(), sizeof(temp.codigo)-1);
            break;
        }

        // Nombre
        cout<<"Ingrese el nombre del producto (o 'CANCELAR' para cancelar): ";
        if(!getline(cin,input)) return;
        if(input=="CANCELAR" || input=="0"){ cout<<"Creación cancelada."<<endl; return; }
        strncpy(temp.nombre, input.c_str(), sizeof(temp.nombre)-1);

        // Descripcion
        cout<<"Ingrese la descripcion del producto (o 'CANCELAR' para cancelar): ";
        if(!getline(cin,input)) return;
        if(input=="CANCELAR" || input=="0"){ cout<<"Creación cancelada."<<endl; return; }
        strncpy(temp.descripcion, input.c_str(), sizeof(temp.descripcion)-1);

        // ID proveedor y validación existencia
        bool proveedorValido = false;
        while(!proveedorValido){
            archivoproveedor.clear();
            archivoproveedor.seekg(posiprov*sizeof(archivoHeader),ios::beg);
            cout<<"Ingrese el id del proveedor del producto (o 'CANCELAR' para cancelar): ";
            if(!getline(cin,input)) return;
            if(input=="CANCELAR" ||input=="cancelar" || input=="Cancelar"|| input=="0"){ cout<<"Creación cancelada."<<endl; return; }
            int idProv = 0;
             idProv = stoi(input); 
             archivoproveedor.clear();
             archivoproveedor.seekg(0,ios::beg);
            try{
                Proveedor p;
                int x=0;
                while(archivoproveedor.read(reinterpret_cast<char*>(&p),sizeof(Proveedor))){
                    cout<<"id del proveedor: "<<p.getid()<<" nombre del proveedor: "<<p.getnombre()<<endl;
                    if(p.getid()==idProv){
                        proveedorValido=true;
                        archivoproveedor.clear();
                        break;
                    }
                    archivoproveedor.clear();
                }
            }
            catch(...){
                if(!proveedorValido) cout<<"El id del proveedor no existe, intentelo nuevamente."<<endl;
            
            }temp.idProveedor = idProv;
            
        }
        archivoproveedor.clear();

        // Precio > 0
        while(true){
            cout<<"Ingrese el precio del producto (mayor a 0) o 'CANCELAR' para cancelar: ";
            if(!getline(cin,input)) return;
            if(input=="CANCELAR" || input=="0"){ cout<<"Creación cancelada."<<endl; return; }
            try{ temp.precio = stof(input); }
            catch(...){ cout<<"Precio invalido. Intente nuevamente."<<endl; continue; }
            if(temp.precio<=0){ cout<<"El precio debe ser mayor a 0."<<endl; continue; }
            break;
        }

        // Stock > 0
        while(true){
            cout<<"Ingrese el stock del producto (mayor a 0) o 'CANCELAR' para cancelar: ";
            if(!getline(cin,input)) return;
            if(input=="CANCELAR" || input=="0"){ cout<<"Creación cancelada."<<endl; return; }
            try{ temp.stock = stoi(input); }
            catch(...){ cout<<"Stock invalido. Intente nuevamente."<<endl; continue; }
            if(temp.stock<=0){ cout<<"El stock debe ser mayor a 0."<<endl; continue; }
            break;
        }

        // Fecha
        cout<<"Ingrese la fecha del registro (YYYY-MM-DD) o 'CANCELAR' para cancelar: ";
        if(!getline(cin,input)) return;
        if(input=="CANCELAR" || input=="0"){ cout<<"Creación cancelada."<<endl; return; }
        strncpy(temp.fechaRegistro, input.c_str(), sizeof(temp.fechaRegistro)-1);
        //Fecha de Vencimiento (opcional)
        cout<<"El producto tiene fecha de vencimiento? (S/N): ";
        if(!getline(cin,input)) return;
        if(input=="S"||input=="s"||input=="Si"||input=="SI"||input=="si"){
            cout<<"Introduzca la fecha de vencimiento (YYYY-MM-DD) o 'CANCELAR' para cancelar: ";
            if(!getline(cin,input)) return;
            if(input=="CANCELAR" || input=="0"){ cout<<"Creación cancelada."<<endl; return; }
            strncpy(temp.fechavencimiento, input.c_str(),sizeof(temp.fechavencimiento)-1);

        }
        // Asignar ID autoincremental
        temp.id = tienda.proximoID+1;

        // Confirmación
        cout<<"\nResumen del producto:"<<endl;
        cout<<"ID: "<<temp.id<<" | Codigo: "<<temp.codigo<<" | Nombre: "<<temp.nombre<<" | Precio: "<<temp.precio<<" | Stock: "<<temp.stock<<" | Proveedor ID: "<<temp.idProveedor<<"\n";
        cout<<"¿Desea guardar este producto? (S/N): ";
        string resp;
        if(!getline(cin,resp)) return;
        if(resp=="S" || resp=="s" || resp=="Si" || resp=="SI" || resp=="si"){
            guardarArchivo(tienda,archivoproducto,temp);
            cout<<"Producto guardado."<<endl;
        } else {
            cout<<"Producto descartado por el usuario."<<endl;
        }
    }
    }
    
}
Producto Producto::buscarProducto(archivoHeader& productosheader,fstream& archivo,int id,string nombre, int opcion){
    // Implementar búsqueda por ID, nombre, código o proveedor
    // Similar a buscarTransaccionesPorProducto pero con criterios diferentes
    Producto p;
    int posicion,i=0;
    if(opcion==1){
    bool encontrado =0;
    try{ id; }
    catch(...){ cout<<"ID invalido."<<endl;  }
    archivo.clear();
    archivo.seekg(0,ios::beg);
    for(int i=0;i<productosheader.cantidadRegistros+1;i++){
        p=cargarArchivo<Producto>(productosheader,archivo,i);
        if(p.id == id){
            posicion=i;
            cout<<"Producto encontrado: ID: "<<p.id<<" | Codigo: "<<p.codigo<<" | Nombre: "<<p.nombre<<" | Precio: "<<p.precio<<" | Stock: "<<p.stock<<" | Proveedor ID: "<<p.idProveedor<<"\n";
            encontrado=1; 
            archivo.clear(); 
            cout<<"ola"<<endl;
            return p;
        }
        else{
            i++;
        }
    }
    archivo.clear();
    if(!encontrado){
        cout<<"Producto no encontrado."<<endl;
    }
    else{
        return p;
    }
    }
    else if(opcion==2){
        string nombre;
        cout<<"introduce el nombre del producto: ";
        getline(cin,nombre);
        int* idencontrados=new int[productosheader.cantidadRegistros];
        int x=0;
        for(int i=0;i<productosheader.cantidadRegistros;i++){
            p=cargarArchivo<Producto>(productosheader,archivo,i);
            string str(p.nombre);
            if(str.find(nombre)){
                idencontrados[x]=p.id;
                x++;
            }
        }
        if(x==0){
            cout<<"no se encontro ningun producto con ese nombre"<<endl;
        }
        else{
            int opcion2;
            Producto* p2= new Producto[productosheader.cantidadRegistros];
            cout<<"se encontraron "<<x<<" coincidencias :"<<endl;
            for(int i=0;i<x;i++){
                while(archivo.read(reinterpret_cast<char*>(&p),sizeof(p))){
                    if(p.id==idencontrados[i]){
                        p2[x]=p;
                        cout<<"Producto "<<i+1<<": id:"<<p.id<<" nombre: "<<p.nombre<<" codigo: "<<p.codigo<<" precio: "<<p.precio<<endl;
                    }
                }   
            }
            do{
            cout<<"Introduzca el numero del producto para seleccionarlo";
            cin>>opcion2;
            opcion2--;
            }while(opcion2<0||opcion2>x);
            return p2[opcion2];
    }
}

return p;
}
bool Producto::codigoDuplicado(archivoHeader* tienda,fstream* archivo ,const string& codigo){
    Producto p;
    for(int i=0;i<tienda->cantidadRegistros;i++){
        p=cargarArchivo<Producto>(*tienda,*archivo,i);
        if(strncmp(p.codigo, codigo.c_str(), sizeof(p.codigo))==0) return true;
    }
    while(archivo->read(reinterpret_cast<char*>(&p),sizeof(p))){
        
    }
    return false;
}
void Producto::listarProductos(fstream* archivop,fstream* archivosprov, archivoHeader productos, archivoHeader provedor,int posiprod,int posiprov){
        if(!archivop){
            cout<<"No se puede abrir el archivo de productos: .\n";
            return;
        }

        Producto p;
        Proveedor prov;
        bool tieneProductos = false;

        // cabecera del cuadro
        cout<<"╔══════════════════════════════════════════════════════════════════════════╗\n";
        cout<<"║                         LISTADO DE PRODUCTOS                             ║\n";
        cout<<"╠════╦═══════════╦══════════════════╦══════════════╦═══════╦════════╦══════╣\n";
        cout<<"║ ID ║  Código   ║     Nombre       ║  Proveedor   ║ Precio║ Stock  ║ Fecha║\n";
        cout<<"╠════╬═══════════╬══════════════════╬══════════════╬═══════╬════════╬══════╣\n";
        for(int i=0;i<productos.cantidadRegistros;i++){
            p=cargarArchivo<Producto>(productos,*archivop,i);
            const char* provName = "N/A";
            for(int j=0;j<provedor.cantidadRegistros;j++){
                prov=cargarArchivo<Proveedor>(provedor,*archivosprov,j);
                if(prov.getid() == p.idProveedor){
                    provName = prov.getnombre();
                    break;
                    tieneProductos=true;
                }
                cout<<"║ "<<setw(2)<<p.id<<" ║ "
                <<setw(9)<<p.codigo<<" ║ "
                <<setw(16)<<p.nombre<<" ║ "
                <<setw(12)<<provName<<" ║ "
                <<setw(6)<<fixed<<setprecision(2)<<p.precio<<"║ "
                <<setw(6)<<p.stock<<" ║ "
                <<setw(10)<<p.fechaRegistro<<" ║\n";
            }

            cout<<"║ "<<setw(2)<<p.id<<" ║ "
                <<setw(9)<<p.codigo<<" ║ "
                <<setw(16)<<p.nombre<<" ║ "
                <<setw(12)<<provName<<" ║ "
                <<setw(6)<<fixed<<setprecision(2)<<p.precio<<"║ "
                <<setw(6)<<p.stock<<" ║ "
                <<setw(10)<<p.fechaRegistro<<" ║\n";
                tieneProductos=true;
        }
        archivop->clear();
        archivosprov->clear();

        if(!tieneProductos){
            cout<<"║                     No hay productos registrados.                      ║\n";
        }
        cout<<"╚════╩═══════════╩══════════════════╩══════════════╩═══════╩════════╩══════╝\n";
}
void Producto::eliminarProducto(archivoHeader &productos,fstream& archivop,int id){
    if(!archivop.is_open()){
        cout<<"La tienda no ha sido creada"<<endl;
        return;
    }
    Producto p;
    int pos;
    bool encontrado = false;
    archivop.seekp(0,ios::beg);
    
    for(int i=0;i<productos.cantidadRegistros;i++){
        p=cargarArchivo<Producto>(productos,archivop,i);
        if(p.id == id){
            encontrado = true;
            p.registroactivo=0;
            pos=archivop.tellg()/sizeof(Producto)-1;
            archivop.clear();
            break;
        }
    }
    guardarArchivo(productos,archivop,p,pos);
    productos.registrosActivos--;
    cout<<"Producto con ID "<<id<<" eliminado correctamente."<<endl;
    if(productos.cantidadRegistros>0){
        cout<<"Producto con id: "<<id<<" eliminado correctamente"<<endl;
    }
}
void Producto::modificarstock(archivoHeader productos,fstream& archivop,int id,int cant,int opt){
    if(!archivop.is_open()){
        cout<<"La tienda no ha sido creada"<<endl;
        return;
    }
    Producto p;
    int pos;
    bool encontrado = false;
    for(int i=0;i<productos.cantidadRegistros;i++){
        p=cargarArchivo<Producto>(productos,archivop,i);
        if(p.id == id&&opt==1){
            encontrado = true;
            p.stock=p.stock-cant;
            pos=archivop.tellg()/sizeof(Producto)-1;
            archivop.clear();
        }
        if(p.id == id&&opt==2){
            encontrado = true;
            p.stock=p.stock+cant;
            pos=archivop.tellg()/sizeof(Producto)-1;
            archivop.clear();
        }
        archivop.clear();
    }
    if(!encontrado){
        cout<<"No se encontró el producto con ID "<<id<<". Ningún cambio aplicado."<<endl;
        return;
    }
    guardarArchivo(productos,archivop,p,pos);
    cout<<"Producto con ID "<<id<<" modificado correctamente."<<endl;
}
void Producto::editarProducto(archivoHeader &producto,archivoHeader &proveedor,fstream &archivo,fstream* archivoprov, int idProducto){
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
    for(int i=0;i<producto.cantidadRegistros;i++){
        p=cargarArchivo<Producto>(producto,archivo,i);
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
                    while(provvaalido==0){
                    cout<<"Ingrese el nuevo ID del proveedor del producto: ";
                    cin>>resp;
                    for(int i=0;i<proveedor.cantidadRegistros;i++){
                        prov=cargarArchivo<Proveedor>(proveedor,*archivoprov,i);
                        if(prov.getid()==resp){
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
                guardarArchivo(producto,archivo,temp,pos);
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
