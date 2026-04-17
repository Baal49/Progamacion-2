#include "TRANSACCION.hpp"
#include <fstream>
#include <iostream>
#include "HEADER.hpp"
#include "CLIENTES.HPP"
#include "PROVEEDOR.hpp"
#include "PRODUCTO.hpp"
#include "PRODUCTOVENTA.hpp"
using namespace std;
void Transaccion::venta(archivoHeader& producto,archivoHeader transaccion,archivoHeader& cliente,archivoHeader& pt,fstream& archivop,fstream& archivoc,fstream& archivot,fstream& archivopt){
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
    c=c.buscarCliente(cliente,archivoc, cedu, nombre, direccion);
    
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
            p=p.buscarProducto(producto,archivop,id,"",1);
            cout<<"El precio del producto es: "<<p.getprecio()<<endl;
            venta.idprod=p.getidp();
            venta.preciounidad=p.getprecio();
            do{
            cout<<"introduce la cantidad del producto que se va a llevar: ";
            cin>>cant;
            if(cant>p.getstock()){
                cout<<"el producto no tiene esa cantidad intentelo de nuevo La existencia del producto es: "<<p.getstock();
                cantver=1;
            }
        }while(cantver==1);
        venta.cantidad=cant;
        venta.idt=transaccion.proximoID++;
        sub+=(cant*p.getprecio());
        string input;
        guardarArchivo(pt,archivopt,venta);
        p.modificarstock(producto,archivop,p.getidp(),cant,1);
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
        guardarArchivo(transaccion,archivot,t);
    }
};
void Transaccion::compra(archivoHeader producto, archivoHeader proveedor,archivoHeader cliente,archivoHeader& transaccion,archivoHeader& pt,fstream& archivop,fstream& archivoprov,fstream& archivoc,fstream& archivot,fstream& archivopt){
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
        t.idcp=prov->getid();
        do{
            int cant;
            cout<<"introduce el id del producto que se va a comprar: ";
            cin>>id;
            p=p.buscarProducto(producto,archivop,id,"",1);
            //if(p->precio>0){
            venta.idprod=p.getidp();
            cout<<"El precio del producto es: "<<p.getprecio()<<endl;
            venta.preciounidad=p.getprecio();
            do{
            cout<<"introduce la cantidad del producto que se va a comprar: ";
            cin>>cant;
            if(cant<=0){
                cout<<"la cantidad debe ser mayor a 0 intentelo de nuevo.";
                cantver=1;
            }
        }while(cantver==1);
        venta.cantidad=cant;
        sub+=(cant*p.getprecio());
        string input;
        venta.idt=transaccion.proximoID++;
        guardarArchivo(pt,archivopt,venta);
        p.modificarstock(producto,archivop,p.getidp(),cant,2);
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
        guardarArchivo(transaccion,archivot,t);
        cout<<"Transaccion aceptada Id de la transaccion es: "<<t.idt<<endl;
        transaccion.cantidadRegistros++;
        transaccion.proximoID++;
        transaccion.registrosActivos++;
}   cout<<"cantidad de transacciones: "<<transaccion.cantidadRegistros;
};
void Transaccion::buscarTransaccionesPorProducto(archivoHeader& transaccion,fstream& archivot,int idProducto){
    if(!archivot.is_open()){ cout<<"Tienda no inicializada."<<endl; return; }
    archivot.seekg(0, ios::end);

    if(transaccion.cantidadRegistros<=0){
        cout<<"No hay transacciones registradas en el archivo binario."<<endl;
        return;
    }

    Transaccion t;
    int encontrados = 0;
    cout<<"Transacciones que tienen de referencia la ID "<<idProducto<<":\n"; // mouestra solo transacciones relacionadas con el producto que introdujo el usuario
    for(int i=0;i<transaccion.cantidadRegistros;i++){
        t=cargarArchivo<Transaccion>(transaccion,archivot,i);
        if(t.idcp == idProducto ){
            encontrados++;
            cout<<"ID: "<<t.idt<<" | Tipo: "<<t.tipo
                <<" | Total: "<<t.total
                <<" | Fecha: "<<t.fecha<<" | Descripcion: "<<t.descripcion<<"\n";
        }
    }
    if(encontrados==0){
        cout<<"No se encontraron transacciones para el producto con ID "<<idProducto<<"."<<endl;
    } else {
        cout<<"Se encontraron "<<encontrados<<" transaccion(es)."<<endl;
    }
}
 void Transaccion::Buscartransaccion (archivoHeader&transaccion,archivoHeader pt,fstream& archivot,fstream& archivopt,int id){
        if(!archivot.is_open()){ cout<<"Tienda no inicializada."<<endl; return; }
        Transaccion t;
        Productoventa p;
        int x=0;
        bool encontrado = false;
        for(int i=0;i<transaccion.cantidadRegistros;i++){
            t=cargarArchivo<Transaccion>(transaccion,archivot,i);
            cout<<"Transacción : ID: "<<t.idt<<" | Tipo: "<<t.tipo<<" | Fecha: "<<t.fecha<<" | Descripcion: "<<t.descripcion<<"\n";
            if(t.idt == id){
                cout<<"Transacción encontrada: ID: "<<t.idt<<" | Tipo: "<<t.tipo<<" | Fecha: "<<t.fecha<<" | Descripcion: "<<t.descripcion<<"\n";
                encontrado = true;
                for(int i=0;i<pt.cantidadRegistros;i++){
                    p=cargarArchivo<Productoventa>(pt,archivopt,i);
                    if(t.idt==p.idt){
                        cout<<"id producto: "<<p.idprod<<" precio unitario:"<<p.preciounidad<<" cantidad: "<<p.cantidad<<endl;
                    }
                }
                return;
            }
            x++;
        }
        if(!encontrado){
            cout<<"Transacción con ID "<<id<<" no encontrada."<<endl;
        }
    }
void Transaccion::listartransaccion(archivoHeader transaccion,archivoHeader pt,fstream& archivot,fstream& archivopt,int posi){
        if(!archivot){ cout<<"archivo no inicializado."<<endl; return; }
        if(transaccion.cantidadRegistros<=0){ cout<<"No hay transacciones registrados."<<endl; return; }
        cout<<"Listado de transacciones:\n";
        Transaccion p;
        Productoventa ptt;
        int x=0;
        for(int i=0;i<transaccion.cantidadRegistros;i++){
            p=cargarArchivo<Transaccion>(transaccion,archivot,i);
            cout<<"id: "<<p.idt<<" | Tipo de Transaccion: "<<p.tipo<<" | Sub Total: "<<p.total<<" | Fecha de registro: "<<p.fecha<<endl;
            int y=0;
            cout<<"Productos de la venta con id: "<<p.idt<<endl;
            for(int j=0;j<pt.cantidadRegistros;j++){
                ptt=cargarArchivo<Productoventa>(pt,archivopt,j);
                if(ptt.idt==p.idt){
                    cout<<"id: "<<ptt.idprod<<" | Precio de la unidad: "<<ptt.preciounidad<<" | Cantidad: "<<ptt.cantidad<<endl;
                }
                y++;
            }
            cout<<"X: "<<x<<endl;
            x++;
        }
}
