
#include <windows.h>
#include <iostream>
#include <locale>
#include <limits>
#include <string>
#include <cstring>
#include <iomanip>
#include <fstream>
#include <vector>
#include "ARCHIVO.hpp"
#include "PROVEEDOR.hpp"
#include "HEADER.hpp"
#include "PRODUCTO.hpp"
#include "TRANSACCION.hpp"
#include "CLIENTES.HPP"
using namespace std;

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
int main(){
    SetConsoleOutputCP(CP_UTF8); 
    SetConsoleCP(CP_UTF8);
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
        cin.ignore((numeric_limits<streamsize>::max)(), '\n');
        int opt=0,id;
        string nombre,direccion,str;
        const char* rutaproductos="C:/Users/reina/Desktop/proyecto2/Progamacion-2/Proyectos/proyectov4/productos.bin";
        const char* rutaproveedores="C:/Users/reina/Desktop/proyecto2/Progamacion-2/Proyectos/proyectov4/proveedores.bin";
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
                        p.buscarProducto(productos,archivoproductos,id,nombre,opt);
                        break;
                    case 3:
                        cout<<"ingrese el id del producto: ";
                        cin>>id;
                        p.editarProducto(productos,proveedores,archivoproductos,&archivoproveedores,id);
                        break;
                    case 4:
                        p.modificarstock(productos,archivoproductos,1,1,1);
                        break;
                    case 5:
                        p.listarProductos(&archivoproductos,&archivoproveedores,productos,proveedores,posiprod,posiprov);
                        cin.ignore((numeric_limits<streamsize>::max)(), '\n');
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
                        cin.ignore((numeric_limits<streamsize>::max)(), '\n');
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
                        prov.editarProveedor(proveedores,archivoproveedores,id);
                        break;
                    case 4:
                        prov.listarProveedores(proveedores,&archivoproveedores,posiprov);
                        break;
                    case 5:
                        cout<<"ingrese el id del del producto que se quiere borrar";
                        cin>>id;
                        prov.eliminarProveedor(proveedores,archivoproveedores, id); 
                        cin.ignore((numeric_limits<streamsize>::max)(), '\n');
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
                        c.Crearcliente(clientes,archivoclientes);
                        break;
                    case 2:{
                        cout<<"Inserte la cedula del cliente";
                        cin>>id;
                        {
                            string nombre, direccion;
                            Cliente c=c.buscarCliente(clientes,archivoclientes,id,nombre,direccion);
                            if(c.getcedula()!=0){
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
                        c.Editarcliente(clientes,archivoclientes,id);
                        break;
                    case 4:
                        c.listarcliente(clientes,&archivoclientes,posic);
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
                        t.compra(productos,proveedores,clientes,transacciones,pt,archivoproductos,archivoproveedores,archivoclientes,archivotransacciones,archivopt);
                        break;
                    case 2:
                        t.venta(productos,transacciones,clientes,pt,archivoproductos,archivoclientes,archivotransacciones,archivopt);
                        break;
                    case 3:
                        cout<<"Introdusca el id de la transaccion: ";
                        cin>>id;
                        t.Buscartransaccion(transacciones,pt,archivotransacciones,archivopt,id);
                        break;
                    case 4:
                        t.listartransaccion(transacciones,pt,archivotransacciones,archivopt,posit);
                        break;
                    case 5:
                    cin.ignore((numeric_limits<streamsize>::max)(), '\n');
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
                productos.actualizarheader(productos,archivoproductos);
                proveedores.actualizarheader(proveedores,archivoproveedores);
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

