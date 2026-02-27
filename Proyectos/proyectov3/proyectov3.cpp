#include <iostream>
#include <locale>
#include <limits>
#include <string>
#include <cstring>
using namespace std;

struct Producto {
    int id;                    // Identificador único (autoincremental)
    char codigo[20];           // Código del producto (ej: "PROD-001")
    char nombre[100];          // Nombre del producto
    char descripcion[200];     // Descripción del producto
    int idProveedor;           // ID del proveedor asociado
    float precio=0;            // Precio unitario
    int stock=0;               // Cantidad en inventario
    char fechaRegistro[11];    // Formato: YYYY-MM-DD
    char fechavencimiento[11]; //Formato: YYYY-MM-DD (opcional)
};

struct Proveedor {
    int id;
    char nombre[100];
    // otros campos omitidos para brevedad
};

struct Transaccion {
    int id;
    int tipo;
    int idProducto;
    int idRelacionado;
    int cantidad;
    float precioUnitario;
    float total;
    char fecha[11];
    char descripcion[200];
};
struct Cliente{
 int cedula;
 char nombre[100];
 char correo[100];
 char telefono[20];
 char dirreccion[200];
 char fecharegitro[11]; //formato:YYYY-MM-DD
};
struct Tienda {
    char nombre[100];
    char rif[20];

    Producto* productos = nullptr;
    int capacidadProductos = 0; // capacidad actual del arreglo
    int cantidadProductos = 0;  // cuántos productos están registrados

    Proveedor* proveedores = nullptr;
    int capacidadProveedores = 0;
    int cantidadProveedores = 0;

    Transaccion* transacciones = nullptr;
    int capacidadTransacciones = 0;
    int cantidadTransacciones = 0;

    Cliente* clientes=nullptr;
    int capacidadClientes = 0;
    int cantidadClientes = 0;

    int siguienteIdProducto = 1;
    int siguienteIdProveedor = 1;
    int siguienteIdTransaccion = 1;
};

void inicializarTienda(Tienda* tienda){
    if(!tienda) return;
    cin.ignore(numeric_limits<streamsize>::max(),'\n');
    cout<<"Ingrese el nombre de la tienda: ";
    cin.getline(tienda->nombre,100);
    cout<<"Ingrese el RIF de la tienda: ";
    cin.getline(tienda->rif,20);

    // Productos
    int capP = 0;
    cout<<"Ingrese la capacidad inicial de productos: ";
    while(cin>>capP && capP<=0){ cout<<"Debe ser mayor que 0. Intente nuevamente: "<<endl; }
    tienda->capacidadProductos = capP;
    tienda->productos = new Producto[tienda->capacidadProductos];
    tienda->siguienteIdProducto = 1;

    // Proveedores (mínimo 1 para permitir crear productos que referencien proveedores)
    int capProv = 0;
    cout<<"Ingrese la capacidad inicial de proveedores: ";
    while(cin>>capProv && capProv<=0){ cout<<"Debe ser mayor que 0. Intente nuevamente: "<<endl; }
    tienda->capacidadProveedores = capProv;
    tienda->proveedores = new Proveedor[tienda->capacidadProveedores];

    // Transacciones
    int capT = 0;
    cout<<"Ingrese la capacidad inicial de transacciones: ";
    while(cin>>capT && capT<=0){ cout<<"Debe ser mayor que 0. Intente nuevamente: "<<endl; }
    tienda->capacidadTransacciones = capT;
    tienda->transacciones = new Transaccion[tienda->capacidadTransacciones];
    //Clientes
    int capC=0;
    while(capC<=0){
        cout<<"Ingrese la capacidad inicial de Clientes: ";
        cin>>capC;
        if(capC<=0){
            cout<<"Debe ser mayor que 0. Intente nuevamente: "<<endl;
        }
    }
    tienda->capacidadClientes=capC;
    tienda->clientes=new Cliente[tienda->capacidadClientes];
    cin.ignore(numeric_limits<streamsize>::max(),'\n');
}

void liberarTienda(Tienda* tienda){
    if(!tienda) return;
    delete[] tienda->productos; tienda->productos = nullptr;
    delete[] tienda->proveedores; tienda->proveedores = nullptr;
    delete[] tienda->transacciones; tienda->transacciones = nullptr;
}

// Busca transacciones por ID de producto usando un arreglo dinámico de punteros
void buscarTransaccionesPorProducto(Tienda* tienda, int idProducto){
    if(tienda==nullptr){ cout<<"Tienda no inicializada."<<endl; return; }
    if(tienda->transacciones==nullptr || tienda->cantidadTransacciones<=0){ cout<<"No hay transacciones registradas."<<endl; return; }

    Transaccion** matches = new Transaccion*[tienda->cantidadTransacciones];
    int encontrados = 0;
    for(int i=0;i<tienda->cantidadTransacciones;i++){
        if(tienda->transacciones[i].idProducto == idProducto){ matches[encontrados++] = &tienda->transacciones[i]; }
    }
    if(encontrados==0){ cout<<"No se encontraron transacciones para el producto con ID "<<idProducto<<"."<<endl; delete[] matches; return; }
    cout<<"Se encontraron "<<encontrados<<" transaccion(es) para el producto "<<idProducto<<":\n";
    for(int i=0;i<encontrados;i++){
        Transaccion* t = matches[i];
        cout<<"ID: "<<t->id<<" | Tipo: "<<t->tipo<<" | Cantidad: "<<t->cantidad
            <<" | Precio unitario: "<<t->precioUnitario<<" | Total: "<<t->total
            <<" | Fecha: "<<t->fecha<<"\n";
    }
    delete[] matches;
}

// Redimensiona el arreglo de productos (duplica la capacidad)
void redimensionarProductos(Tienda* tienda){
    int nuevaCap = (tienda->capacidadProductos>0) ? tienda->capacidadProductos*2 : 1;
    Producto* nuevo = new Producto[nuevaCap];
    // copiar existentes
    for(int i=0;i<tienda->cantidadProductos;i++) nuevo[i] = tienda->productos[i];
    delete[] tienda->productos;
    tienda->productos = nuevo;
    tienda->capacidadProductos = nuevaCap;
}
void redimensionarProveedor(Tienda* tienda){
    int nuevaCap = (tienda->cantidadProveedores>0) ? tienda->capacidadProveedores*2 : 1;
    Proveedor* nuevo = new Proveedor[nuevaCap];
    // copiar existentes
    for(int i=0;i<tienda->cantidadProveedores;i++) nuevo[i] = tienda->proveedores[i];
    delete[] tienda->proveedores;
    tienda->proveedores = nuevo;
    tienda->capacidadProveedores = nuevaCap;
}

bool codigoDuplicado(Tienda* tienda, const string& codigo){
    for(int i=0;i<tienda->cantidadProductos;i++){
        if(strncmp(tienda->productos[i].codigo, codigo.c_str(), sizeof(tienda->productos[i].codigo))==0) return true;
    }
    return false;
}

void Crearproductos(Tienda* tienda){
    if(tienda==nullptr){ cout<<"Tienda no inicializada."<<endl; return; }
    string input;
    int cantidad = 0;
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
        Producto temp{};

        // Codigo (único)
        while(true){
            cout<<"Ingrese el codigo del producto (o 'CANCELAR' para cancelar): ";
            if(!getline(cin, input)) return;
            if(input=="CANCELAR" || input=="0"){ cout<<"Creación cancelada."<<endl; return; }
            if(input.empty()){ cout<<"El codigo no puede estar vacío."<<endl; continue; }
            if(codigoDuplicado(tienda, input)) { cout<<"Codigo ya existe. Ingrese otro."<<endl; continue; }
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
            cout<<"Ingrese el id del proveedor del producto (o 'CANCELAR' para cancelar): ";
            if(!getline(cin,input)) return;
            if(input=="CANCELAR" || input=="0"){ cout<<"Creación cancelada."<<endl; return; }
            int idProv = 0;
            try{ idProv = stoi(input); }
            catch(...){ cout<<"ID invalido. Intente nuevamente."<<endl; continue; }
            for(int i=0;i<tienda->cantidadProveedores;i++){
                if(tienda->proveedores[i].id == idProv){ proveedorValido = true; break; }
            }
            if(!proveedorValido) cout<<"El id del proveedor no existe, intentelo nuevamente."<<endl;
            else temp.idProveedor = idProv;
        }

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
        temp.id = tienda->siguienteIdProducto++;

        // Confirmación
        cout<<"\nResumen del producto:"<<endl;
        cout<<"ID: "<<temp.id<<" | Codigo: "<<temp.codigo<<" | Nombre: "<<temp.nombre<<" | Precio: "<<temp.precio<<" | Stock: "<<temp.stock<<" | Proveedor ID: "<<temp.idProveedor<<"\n";
        cout<<"¿Desea guardar este producto? (S/N): ";
        string resp;
        if(!getline(cin,resp)) return;
        if(resp=="S" || resp=="s" || resp=="Si" || resp=="SI" || resp=="si"){
            // si el arreglo está lleno, redimensionar duplicando capacidad
            if(tienda->cantidadProductos >= tienda->capacidadProductos){
                redimensionarProductos(tienda);
                cout<<"Arreglo de productos redimensionado a capacidad "<<tienda->capacidadProductos<<"."<<endl;
            }
            tienda->productos[tienda->cantidadProductos++] = temp;
            cout<<"Producto guardado."<<endl;
        } else {
            cout<<"Producto descartado por el usuario."<<endl;
        }
    }
    tienda->cantidadProductos=cantidad;
}
void Crearproveedor(Tienda* tienda){
    if(tienda==nullptr){ cout<<"Tienda no inicializada."<<endl; return; }
    string input;
    int cantidad = 0;
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
        Proveedor temp{};

        // Codigo (único)
        while(true){
            cout<<"Ingrese el Id del proveedor (o 'CANCELAR' para cancelar): ";
            if(!getline(cin, input)) return;
            if(input=="CANCELAR" || input=="0"){ cout<<"Creación cancelada."<<endl; return; }
            if(input.empty()){ cout<<"El codigo no puede estar vacío."<<endl; continue; }
            if(codigoDuplicado(tienda, input)) { cout<<"Codigo ya existe. Ingrese otro."<<endl; continue; }
            temp.id=stoi(input);
            break;
        }

        // Nombre
        cout<<"Ingrese el nombre del proveedor (o 'CANCELAR' para cancelar): ";
        if(!getline(cin,input)) return;
        if(input=="CANCELAR" || input=="0"){ cout<<"Creación cancelada."<<endl; return; }
        strncpy(temp.nombre, input.c_str(), sizeof(temp.nombre)-1);

        
        // Asignar ID autoincremental
        temp.id = tienda->siguienteIdProveedor++;

        // Confirmación
        cout<<"\nResumen del proveedor:"<<endl;
        cout<<"ID: "<<temp.id<<" | Nombre: "<<temp.nombre<<endl;
        cout<<"¿Desea guardar este proveedor? (S/N): ";
        string resp;
        if(!getline(cin,resp)) return;
        if(resp=="S" || resp=="s" || resp=="Si" || resp=="SI" || resp=="si"){
            // si el arreglo está lleno, redimensionar duplicando capacidad
            if(tienda->cantidadProveedores >= tienda->capacidadProveedores){
                redimensionarProveedor(tienda);
                cout<<"Arreglo de proveedores redimensionado a capacidad "<<tienda->capacidadProveedores<<"."<<endl;
            }
            tienda->proveedores[tienda->cantidadProveedores++] = temp;
            cout<<"Proveedor guardado."<<endl;
        } else {
            cout<<"Proveedor descartado por el usuario."<<endl;
        }
    }
    tienda->cantidadProveedores=cantidad;
}
Producto buscarProducto(Tienda* tienda,int id,string nombre, int opcion){
    // Implementar búsqueda por ID, nombre, código o proveedor
    // Similar a buscarTransaccionesPorProducto pero con criterios diferentes
    
    int posicion;
    if(opcion==1){
    bool encontrado =0;
    try{ id }
    catch(...){ cout<<"ID invalido."<<endl; return; }
    for(int i=0;i<tienda->cantidadProductos;i++){
        if(tienda->productos[i].id == id){
            posicion=i;
            Producto& p = tienda->productos[i];
            cout<<"Producto encontrado: ID: "<<p.id<<" | Codigo: "<<p.codigo<<" | Nombre: "<<p.nombre<<" | Precio: "<<p.precio<<" | Stock: "<<p.stock<<" | Proveedor ID: "<<p.idProveedor<<"\n";
            encontrado=1;
            
        }
    }
    if(!encontrado){
        cout<<"Producto no encontrado."<<endl;
    }
    else{
        return tienda->productos[posicion];
    }
    cout<<"Desea editar el producto? (S/N): ";
    string resp;
    
    if(!getline(cin,resp)) return;
    if (resp=="N" || resp=="n" || resp=="No" || resp=="NO" || resp=="no"){
        cout<<"Edición cancelada."<<endl;
    }
    else if(resp=="S" || resp=="s" || resp=="Si" || resp=="SI" || resp=="si"){
        editarProducto(tienda, id);
    }
    }
    else if(opcion==2){
        string nombre;
        cout<<"introduce el nombre del producto: ";
        getline(cin,nombre);
        int* idencontrados=new int[tienda->cantidadProductos];
        int x=0;
        for(int i=0;i<tienda->cantidadProductos;i++){
            string str(tienda->productos->nombre);
            if(str.find(nombre)){
                idencontrados[x]=tienda->productos[i].id;
                x++;
            }
        }
        if(x==0){
            cout<<"no se encontro ningun producto con ese nombre"<<endl;
        }
        else{
            int opcion2;
            cout<<"se encontraron "<<x<<" coincidencias :"<<endl;
            for(int i=0;i<x;i++){
                for(int j=0;j<tienda->cantidadProductos;j++){
                    if(tienda->productos[j].id==idencontrados[i]){
                        cout<<"Producto "<<i+1<<": id:"<<tienda->productos[j].id<<" nombre: "<<tienda->productos[j].nombre<<" codigo: "<<tienda->productos[j].codigo<<" precio: "<<tienda->productos[j].precio<<endl;
                    }
                }   
            }
            do{
            cout<<"Introduzca el numero del producto para seleccionarlo";
            cin>>opcion2;
            opcion2--;
            }while(opcion2<0||opcion2>x);
            cout<<"Desea editar el producto? (S/N): ";
            string resp;
    
            if(!getline(cin,resp)) return;
            if (resp=="N" || resp=="n" || resp=="No" || resp=="NO" || resp=="no"){
                cout<<"Edición cancelada."<<endl;
            }
            else if(resp=="S" || resp=="s" || resp=="Si" || resp=="SI" || resp=="si"){
                editarProducto(tienda, idencontrados[opcion2]);
            }
        }
    }
    
    void listarProductos(Tienda* tienda){
        if(tienda==nullptr){ cout<<"Tienda no inicializada."<<endl; return; }
        if(tienda->productos==nullptr || tienda->cantidadProductos<=0){ cout<<"No hay productos registrados."<<endl; return; }
        cout<<"Listado de productos:\n";
        for(int i=0;i<tienda->cantidadProductos;i++){
            Producto& p = tienda->productos[i];
            cout<<"ID: "<<p.id<<" | Codigo: "<<p.codigo<<" | Nombre: "<<p.nombre<<" | Precio: "<<p.precio<<" | Stock: "<<p.stock<<" | Proveedor ID: "<<p.idProveedor<<"\n";
        }
    }
    void listarProveedores(Tienda* tienda){
        if(tienda==nullptr){ cout<<"Tienda no inicializada."<<endl; return; }
        if(tienda->proveedores==nullptr || tienda->cantidadProveedores<=0){ cout<<"No hay proveedores registrados."<<endl; return; }
        cout<<"Listado de proveedores:\n";
        for(int i=0;i<tienda->cantidadProveedores;i++){
            Proveedor& p = tienda->proveedores[i];
            cout<<"ID: "<<p.id<<" | Nombre: "<<p.nombre<<"\n";
        }

}
void buscarProveedor(Tienda* tienda){
    // Implementar búsqueda por ID, nombre, código o proveedor
    // Similar a buscarTransaccionesPorProducto pero con criterios diferentes
    int opcion=0;
    while(opcion!=1&&opcion!=2){
        cout<<"Quiere hacer la busqueda del proveedor por 1.ID o por 2.Nombre";
        cin>>opcion;
        if(opcion!=1&&opcion!=2){
            cout<<"esa opcion es invalida intentelo de nuevo.";
        }
    }
    
    if(opcion==1){
    bool encontrado =0;
    cout <<"ingrese el ID del producto a buscar: ";
    string input;
    if(!getline(cin,input)) return;
    int idBuscado = 0;
    try{ idBuscado = stoi(input); }
    catch(...){ cout<<"ID invalido."<<endl; return; }
    for(int i=0;i<tienda->cantidadProveedores;i++){
        if(tienda->proveedores[i].id == idBuscado){
            Proveedor& p = tienda->proveedores[i];
            cout<<"proveedor encontrado: ID: "<<p.id<<" | Nombre: "<<p.nombre<<endl;
            encontrado=1;
            return;
        }
    }
    if(!encontrado){
        cout<<"Proveedor no encontrado."<<endl;
    }
    else{
    cout<<"Desea editar el proveedor? (S/N): ";
    string resp;
    
    if(!getline(cin,resp)) return;
    if (resp=="N" || resp=="n" || resp=="No" || resp=="NO" || resp=="no"){
        cout<<"Edición cancelada."<<endl;
    }
    else if(resp=="S" || resp=="s" || resp=="Si" || resp=="SI" || resp=="si"){
        editarProveedor(tienda, idBuscado);
    }
    }
    }
    else if(opcion==2){
        string nombre;
        cout<<"introduce el nombre del provedor: ";
        getline(cin,nombre);
        int* idencontrados=new int[tienda->cantidadProveedores];
        int x=0;
        for(int i=0;i<tienda->cantidadProveedores;i++){
            string str(tienda->proveedores->nombre);
            if(str.find(nombre)){
                idencontrados[x]=tienda->proveedores[i].id;
                x++;
            }
        }
        if(x==0){
            cout<<"no se encontro ningun proveedor con ese nombre"<<endl;
        }
        else{
            int opcion2;
            cout<<"se encontraron "<<x<<" coincidencias :"<<endl;
            for(int i=0;i<x;i++){
                for(int j=0;j<tienda->cantidadProveedores;j++){
                    if(tienda->proveedores[j].id==idencontrados[i]){
                        cout<<"Proveedor "<<i+1<<": id:"<<tienda->proveedores[j].id<<" nombre: "<<tienda->proveedores[j].nombre<<endl;
                    }
                }   
            }
            do{
            cout<<"Introduzca el numero del Proveedor para seleccionarlo";
            cin>>opcion2;
            opcion2--;
            }while(opcion2<0||opcion2>x);
            cout<<"Desea editar el proveedor? (S/N): ";
            string resp;
    
            if(!getline(cin,resp)) return;
            if (resp=="N" || resp=="n" || resp=="No" || resp=="NO" || resp=="no"){
                cout<<"Edición cancelada."<<endl;
            }
            else if(resp=="S" || resp=="s" || resp=="Si" || resp=="SI" || resp=="si"){
                editarProveedor(tienda, idencontrados[opcion2]);
            }
        }
    }
    
}
string buscarCliente(Tienda* tienda,int cedula){
    if(tienda->clientes!=nullptr){
        int posicion=-1;
        for(int i=0;i<tienda->cantidadClientes;i++){
            if(tienda->clientes[i].cedula){
                posicion=i;
                break;
            }
        }
        if(posicion!=-1){
            return tienda->clientes[posicion].nombre,tienda->clientes[posicion].dirreccion;
        }
        else{
            return "no existe ese cliente";
        }
    }

}
void editarProveedor(Tienda* tienda, int idProveedor){
    //Funcion para editar algun aspecto del producto
    int respuesta=0;
    int idBuscado=0;
    bool encontrado=false;
    int resp;
    Proveedor* temp;
    temp->id=idProveedor;
    string respp;
    for(int i=0;i<tienda->cantidadProveedores;i++){
        if(tienda->proveedores[i].id==idProveedor){
            idBuscado=i;
            break;
        }
    }
    if(!encontrado){
        cout<<"Proveedor no encontrado."<<endl;
        return;
    } 
    do{
    cout <<"Que desea editar del proveedor?"<<endl;
        cout <<"1. id"<<endl;
        cout <<"2. Nombre"<<endl;
        cout <<"3. Guardar cambios"<<endl;
        cout <<"4. Eliminar proveedor"<<endl;
        cout <<"0. Cancelar sin guardar"<<endl;
        cin>>respuesta;
        switch(respuesta){
            case 1:
                // Editar código (validar único)
                cout <<"Ingrese el nuevo código del proveedor o 0 para cancelar: ";
                cin>>respuesta;
                   if (respuesta==NULL){ cout<<"El código no puede estar vacío. Edición cancelada."<<endl; break; }
                   if ( respuesta==0){ cout<<"Edición cancelada."<<endl; break; }
                // Asignar nuevo código al producto
                    temp->id=respuesta;
                break;
            case 2:
                // Editar nombre.
                
                cout<<"Ingrese el nuevo nombre del proveedor: ";
                getline(cin,respp);
                while (respp.empty()){ cout<<"El nombre no puede estar vacío."<<endl; getline(cin,respp); }
                while (respp.length() >= sizeof(tienda->proveedores[idBuscado].nombre)){ cout<<"El nombre es demasiado largo. Coloque otro nombre."<<endl; getline(cin,respp); }
                // Asignar nuevo nombre al producto
                if (tienda->proveedores[idBuscado].nombre != nullptr) delete[] tienda->proveedores[idBuscado].nombre;
                if (respp=="CANCELAR" || respp=="0"){ cout<<"Edición cancelada."<<endl; break; }
                strcpy(temp->nombre,respp.c_str());
                respp.clear();
                break;
            case 3:
                // Guardar cambios (confirmar antes)
                cout<<"Producto antes: ID: "<<tienda->proveedores[idBuscado].id<<" | Nombre: "<<tienda->proveedores[idBuscado].nombre<<endl;
                cout<<"Producto Despues: ID: "<<tienda->productos[idBuscado].id<<" | Nombre: "<<temp->nombre<<endl;
                cout<<"¿Desea guardar los cambios realizados al proveedor? (S/N): ";
                tienda->proveedores[idBuscado]=*temp;
                delete temp;
                cout<<"Cambios guardados."<<endl;
                break;
            case 4:
                cout<<"¿Está seguro que desea eliminar este proveedor? Esto también eliminará todos los productos asociados a este proveedor. (S/N): ";
                cin >> resp;
                if(resp=='S'||resp=='s'){
                    eliminarProveedor(tienda, idProveedor);
                }
            case 0:
                cout<<"Edición cancelada sin guardar."<<endl;
                break;
            default:
                cout<<"Opción inválida."<<endl;
        }
    }while(respuesta!=0&&respuesta!=7);

    
     
}
void editarProducto(Tienda* tienda, int idProducto){
    //Funcion para editar algun aspecto del producto
    int respuesta=0;
    int idBuscado=0;
    bool encontrado=false;
    int resp;
    Producto* temp;
    temp->id=idProducto;
    string respp;
    for(int i=0;i<tienda->cantidadProductos;i++){
        if(tienda->productos[i].id==idProducto){
            idBuscado=i;
            break;
        }
    }
    if(!encontrado){
        cout<<"Producto no encontrado."<<endl;
        return;
    } 
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
        cin>>respuesta;
        switch(respuesta){
            case 1:
                // Editar código (validar único)
                cout <<"Ingrese el nuevo código del producto o 0 para cancelar: ";
                getline(cin,respp);
                   while(codigoDuplicado(tienda, respp)) { cout<<"Codigo ya existe Intentelo nuevamente."<<endl; getline(cin,respp); }
                   if (respp.empty()){ cout<<"El código no puede estar vacío. Edición cancelada."<<endl; break; }
                   if ( respp=="0"||respp=="CANCELAR"){ cout<<"Edición cancelada."<<endl; break; }
                // Asignar nuevo código al producto
                strcpy(temp->codigo,respp.c_str());
                break;
            case 2:
                // Editar nombre.
                
                cout<<"Ingrese el nuevo nombre del producto: ";
                getline(cin,respp);
                while (respp.empty()){ cout<<"El nombre no puede estar vacío."<<endl; getline(cin,respp); }
                while (respp.length() >= sizeof(tienda->productos[idBuscado].nombre)){ cout<<"El nombre es demasiado largo. Coloque otro nombre."<<endl; getline(cin,respp); }
                // Asignar nuevo nombre al producto
                if (tienda->productos[idBuscado].nombre != nullptr) delete[] tienda->productos[idBuscado].nombre;
                if (respp=="CANCELAR" || respp=="0"){ cout<<"Edición cancelada."<<endl; break; }
                strcpy(temp->nombre,respp.c_str());
                respp.clear();
                break;
            case 3:
                // Editar descripción
                cout<<"ingrese la nueva descripcion del producto: ";
                getline(cin,respp);
                while(respp.empty()){cout<<"La descripcion no puede estar vacia."<<endl;getline(cin,respp);}
                while(respp.length()>sizeof(tienda->productos[idBuscado].descripcion)){cout<<"La descripcion es demasiado larga."<<endl; getline(cin,respp);}
                if(tienda->productos[idBuscado].descripcion!=nullptr) delete[] tienda->productos[idBuscado].descripcion;
                if (respp=="CANCELAR" || respp=="0"){ cout<<"Edición cancelada."<<endl; break; }
                strcpy(temp->descripcion,respp.c_str());
                cout<<"Descripción actualizada."<<endl;
                break;
            case 4:
                // Editar proveedor (validar existencia)
                    cout<<"Ingrese el nuevo ID del proveedor del producto: ";
                    cin>>resp;
                    bool provvaalido=false;
                    while(!provvaalido){
                    for(int i=0;i<tienda->cantidadProveedores;i++){
                        if(tienda->proveedores[i].id==resp){
                            provvaalido=true;
                            break;
                        }
                    }
                    if(!provvaalido){
                        cout<<"el ID del provedor no existe. Intentelo nuevamente."<<endl;
                        break;
                    }
                }
                temp->idProveedor=resp;
                cout<<"Proveedor actualizado."<<endl;
                break;
            case 5:
                // Editar precio (validar > 0)
                    cout<<"Ingrese el nuevo precio del producto: ";
                    float precio;
                    cin>>precio;
                    while(precio<=0){cout<<"El precio debe ser mayor a 0 Intentelo nuevamente."<<endl; cin>>precio;}
                    temp->precio=precio;
                    cout<<"Precio actualizado."<<endl;
                break;
            case 6:
                // Editar stock (validar > 0)
                cout<<"Ingrese el nuevo stock del producto: ";
                float stock;
                cin>>stock;
                while(stock<=0){cout<<"El stock debe ser mayor a 0 Intentelo nuevamente."<<endl; cin>>stock;}
                temp->stock=stock;
                cout<<"Stock actualizado."<<endl;
                break;
            case 7:
                // Editar fecha de registro
                cout<<"ingrese la nueva fecha de registro del producto con Formato(YYYY-MM-DD): ";
                getline(cin,respp);
                while(respp.empty()){cout<<"La fecha de registro no puede estar vacia."<<endl;getline(cin,respp);}
                while(respp.length()>sizeof(tienda->productos[idBuscado].fechaRegistro)){cout<<"La fecha de registro es demasiado larga."<<endl; getline(cin,respp);}
                if(tienda->productos[idBuscado].fechaRegistro!=nullptr) delete[] tienda->productos[idBuscado].fechaRegistro;
                if (respp=="CANCELAR" || respp=="0"){ cout<<"Edición cancelada."<<endl; break; }
                strcpy(temp->fechaRegistro,respp.c_str());
                cout<<"Fecha de registro actualizada."<<endl;
                break;
            case 8:
                // Editar fecha de vencimiento
                cout<<"ingrese la nueva fecha de vencimiento del producto con Formato(YYYY-MM-DD): ";
                getline(cin,respp);
                while(respp.empty()){cout<<"La fecha de vencimiento no puede estar vacia."<<endl;getline(cin,respp);}
                while(respp.length()>sizeof(tienda->productos[idBuscado].fechavencimiento)){cout<<"La fecha de vencimiento es demasiado larga."<<endl; getline(cin,respp);}
                if(tienda->productos[idBuscado].fechavencimiento!=nullptr) delete[] tienda->productos[idBuscado].fechavencimiento;
                if (respp=="CANCELAR" || respp=="0"){ cout<<"Edición cancelada."<<endl; break; }
                strcpy(temp->fechavencimiento,respp.c_str());
                cout<<"Fecha de vencimiento actualizada."<<endl;
                break;
            
            case 9:
                // Eliminar producto (confirmar antes)
                cout<<"¿Está seguro que desea eliminar este producto? (S/N): ";
                cin >> resp;
                if(resp=='S'||resp=='s'){
                    eliminarProducto(tienda, idProducto);
                    cout<<"Producto eliminado."<<endl;
                    return;
                }
                else{
                    cout<<"Eliminación cancelada."<<endl;
                }
                break;
            
            case 10:
                // Guardar cambios (confirmar antes)
                cout<<"Producto antes: ID: "<<tienda->productos[idBuscado].id<<" | Codigo: "<<tienda->productos[idBuscado].codigo<<" | Nombre: "<<tienda->productos[idBuscado].nombre<<" | Precio: "<<tienda->productos[idBuscado].precio<<" | Stock: "<<tienda->productos[idBuscado].stock<<" | Proveedor ID: "<<tienda->productos[idBuscado].idProveedor<<" | Fecha de Registro: "<<tienda->productos[idBuscado].fechaRegistro<<" | Fecha de Vencimiento: "<<tienda->productos[idBuscado].fechavencimiento<<endl;
                cout<<"Producto Despues: ID: "<<tienda->productos[idBuscado].id<<" | Codigo: "<<temp->codigo<<" | Nombre: "<<temp->nombre<<" | Precio: "<<temp->precio<<" | Stock: "<<temp->stock<<" | Proveedor ID: "<<temp->idProveedor<<" | Fecha de Registro: "<<temp->fechaRegistro<<" | Fecha de Vencimiento: "<<temp->fechavencimiento<<endl;
                cout<<"¿Desea guardar los cambios realizados al producto? (S/N): ";
                tienda->productos[idBuscado]=*temp;
                delete temp;
                cout<<"Cambios guardados."<<endl;
                break;
            case 0:
                cout<<"Edición cancelada sin guardar."<<endl;
                break;
            default:
                cout<<"Opción inválida."<<endl;
        }
    }while(respuesta!=0&&respuesta!=7);
}
void eliminarProveedor(Tienda* tienda,int id){
    if(tienda!=nullptr){
        int posproveedor=-1;
        for(int i;i<tienda->cantidadProveedores;i++){
            if(id==tienda->proveedores[i].id){
                posproveedor=i;
                break;
            }
        }
        if(posproveedor==-1){
            cout<<"no existe ese producto"<<endl;
            return;
        }
        else{
            for(int i=posproveedor+1;i<tienda->cantidadProveedores;i++){
                tienda->proveedores[i-1]=tienda->proveedores[i];
            }
            tienda->proveedores[tienda->cantidadProveedores].id=0;
            strcpy(tienda->proveedores[tienda->cantidadProveedores].nombre,"");
        }
    }
    else{
        cout<<"La tienda no ha sido creada"<<endl;
    }
}
void eliminarProducto(Tienda* tienda,int id){
    if(tienda!=nullptr){
        int posproduct=-1;
        for(int i;i<tienda->cantidadProductos;i++){
            if(id==tienda->productos[i].id){
                posproduct=i;
                break;
            }
        }
        if(posproduct==-1){
            cout<<"no existe ese producto"<<endl;
            return;
        }
        else{
            for(int i=posproduct+1;i<tienda->cantidadProductos;i++){
                tienda->productos[i-1]=tienda->productos[i];
            }
            tienda->productos[tienda->cantidadProductos].id=0;
            strcpy(tienda->productos[tienda->cantidadProductos].nombre,"");
            strcpy(tienda->productos[tienda->cantidadProductos].descripcion,"");
            strcpy(tienda->productos[tienda->cantidadProductos].codigo,"");
            strcpy(tienda->productos[tienda->cantidadProductos].fechaRegistro,"");
            strcpy(tienda->productos[tienda->cantidadProductos].fechavencimiento,"");
            tienda->productos[tienda->cantidadProductos].idProveedor=0;
            tienda->productos[tienda->cantidadProductos].precio=0;
            tienda->productos[tienda->cantidadProductos].stock=0;
        }
    }
    else {
        cout<<"la tienda no ha sido creada"<<endl;
    }
}
void compra(Tienda* tienda){
    Producto* p;
    int cedu;
    char respuesta;
    string nombre,direccion;
    cout<<"Inserte la cedula del cliente";
    cin>>cedu;
    nombre,direccion=buscarCliente(tienda,cedu);
    if(nombre=="no existe ese cliente"){
        cout<<nombre<<" desea registrarlo?"<<endl;
        cout<<"Introduzca S para registrarlo o N para cancelar";
        cin>>respuesta;
        if(respuesta=='S'||respuesta=='s'){
             crearCliente(tienda);
        }
        else{
            return ;
        }
    }
    else{
        cout<<"Nombre del cliente: "<<nombre<<" Cedula: "<<cedu<<" Direccion: "<<direccion;
        int opt,id;
        do{
            cout<<"introduce el id del producto que se va a llevar: ";
            cin>>id;
            *p=buscarProducto(tienda,id,"",1);
            
        }while(opt==1);
    }
}
void venta(Tienda* tienda){

}

int main(){
    Tienda tienda;
    strcpy(tienda.nombre, "Farmacia pipo");
    inicializarTienda(&tienda);
    
    cout <<"╔═══════════════════════════════════════════╗"<<endl;
    cout <<"║   SISTEMA DE GESTIÓN DE INVENTARIO        ║"<<endl;
    cout <<"║   Tienda: [Nombre de la Tienda]           ║"<<endl;
    cout <<"╚═══════════════════════════════════════════╝"<<endl;
    
    cout <<"1. Gestión de Productos" <<endl;
     cout <<"2. Gestión de Proveedores" <<endl;
     cout <<"3. Gestión de Clientes" <<endl;
     cout <<"4. Gestión de Transacciones" <<endl;
     cout <<"5. Salir"<<endl;
        int opcion;
    do{
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
                        Crearproductos(&tienda);
                        break;
                    case 2:
                        buscarProducto(&tienda);
                        break;
                    case 3:
                        int opcion=0;
                         while(opcion!=1&&opcion!=2){
                            cout<<"Quiere hacer la busqueda del producto por 1.ID o por 2.Nombre";
                            cin>>opcion;
                            if(opcion!=1&&opcion!=2){
                            cout<<"esa opcion es invalida intentelo de nuevo.";
                            }
                        }
                        editarProducto(&tienda);
                        break;
                    case 4:
                        actualizarStock(&tienda);
                        break;
                    case 5:
                        listarProductos(&tienda);
                        break;
                    case 6:
                        eliminarProducto(&tienda, 1); // Example ID for deletion
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
                        Crearproveedor(&tienda);
                        break;
                    case 2:
                        buscarProveedor(&tienda);
                        break;
                    case 3:
                        editarProveedor(&tienda);
                        break;
                    case 4:
                        listarProveedores(&tienda);
                        break;
                    case 5:
                        eliminarProveedor(&tienda, 1); // Example ID for deletion
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
                        Crearclientes(&tienda);
                        break;
                    case 2:
                        buscarCliente(&tienda);
                        break;
                    case 3:
                        editarCliente(&tienda);
                        break;
                    case 4:
                        listarClientes(&tienda);
                        break;
                    case 5:
                        eliminarCliente(&tienda, 1); // Example ID for deletion
                        break;
                    case 0:
                        cout<<"Volviendo al menú principal..."<<endl;
                        break;
                    default:
                        cout<<"Opción inválida. Intente nuevamente."<<endl;
                }
                break;
            case 4:
                //CrearTransacciones(&tienda);
                break;
            case 5:
                cout<<"Saliendo del programa..."<<endl;
                break;
            default:
                cout<<"Opción inválida. Intente nuevamente."<<endl;
        }


    cout<<"\n--- CREAR PRODUCTOS ---\n";
    Crearproductos(&tienda);
    buscarProducto(&tienda);
    // Ejemplo de búsqueda (si hay transacciones)
    // buscarTransaccionesPorProducto(&tienda, 1);

    liberarTienda(&tienda);
    return 0;
}
}
