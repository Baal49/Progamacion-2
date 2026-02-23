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
    char tipo[10];
    int idProducto;
    int idRelacionado;
    int cantidad;
    float precioUnitario;
    float total;
    char fecha[11];
    char descripcion[200];
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
    while(cin>>capP && capP<=0){ cout<<"Debe ser mayor que 0. Intente nuevamente: "; }
    tienda->capacidadProductos = capP;
    tienda->productos = new Producto[tienda->capacidadProductos];
    tienda->cantidadProductos = 0;
    tienda->siguienteIdProducto = 1;

    // Proveedores (mínimo 1 para permitir crear productos que referencien proveedores)
    int capProv = 0;
    cout<<"Ingrese la capacidad inicial de proveedores: ";
    while(cin>>capProv && capProv<=0){ cout<<"Debe ser mayor que 0. Intente nuevamente: "; }
    tienda->capacidadProveedores = capProv;
    tienda->proveedores = new Proveedor[tienda->capacidadProveedores];
    tienda->cantidadProveedores = 0;

    // Transacciones
    int capT = 0;
    cout<<"Ingrese la capacidad inicial de transacciones: ";
    while(cin>>capT && capT<=0){ cout<<"Debe ser mayor que 0. Intente nuevamente: "; }
    tienda->capacidadTransacciones = capT;
    tienda->transacciones = new Transaccion[tienda->capacidadTransacciones];
    tienda->cantidadTransacciones = 0;

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
}

void buscarProducto(Tienda* tienda){
    // Implementar búsqueda por ID, nombre, código o proveedor
    // Similar a buscarTransaccionesPorProducto pero con criterios diferentes
    cout <<"ingrese el ID del producto a buscar: ";
    string input;
    if(!getline(cin,input)) return;
    int idBuscado = 0;
    try{ idBuscado = stoi(input); }
    catch(...){ cout<<"ID invalido."<<endl; return; }
    for(int i=0;i<tienda->cantidadProductos;i++){
        if(tienda->productos[i].id == idBuscado){
            Producto& p = tienda->productos[i];
            cout<<"Producto encontrado: ID: "<<p.id<<" | Codigo: "<<p.codigo<<" | Nombre: "<<p.nombre<<" | Precio: "<<p.precio<<" | Stock: "<<p.stock<<" | Proveedor ID: "<<p.idProveedor<<"\n";
            return;
        }
    }
    cout<<"Producto no encontrado."<<endl;




    cout<<"Desea editar el producto? (S/N): ";
    string resp;
    if(!getline(cin,resp)) return;
    if (resp=="N" || resp=="n" || resp=="No" || resp=="NO" || resp=="no"){
        cout<<"Edición cancelada."<<endl;
        
        if(resp=="S" || resp=="s" || resp=="Si" || resp=="SI" || resp=="si"){
        editarProducto(tienda, idBuscado);

        cout <<"Que desea editar del producto?"<<endl;
        cout <<"1. Código"<<endl;
        cout <<"2. Nombre"<<endl;
        cout <<"3. Descripción"<<endl;
        cout <<"4. Proveedor"<<endl;
        cout <<"5. Precio"<<endl;
        cout <<"6. Stock"<<endl;
        cout <<"7. Guardar cambios"<<endl;
        cout <<"0. Cancelar sin guardar"<<endl;
        cin>>resp;
        switch(resp){
            case "1":
                // Editar código (validar único)
                cout <<"Ingrese el nuevo código del producto: ";
                cin >> resp;
                   if(codigoDuplicado(tienda, resp)) { cout<<"Codigo ya existe. Edición cancelada."<<endl; break; }
                   if (resp.empty()){ cout<<"El código no puede estar vacío. Edición cancelada."<<endl; break; }
                   if (resp=="CANCELAR" || resp=="0"){ cout<<"Edición cancelada."<<endl; break; }
                // Asignar nuevo código al producto
                tienda->productos[idBuscado-1].codigo = resp;
                cout<<"Código actualizado."<<endl;
                break;
            case "2":
                // Editar nombre
                cout<<"Ingrese el nuevo nombre del producto: ";
                cin >> resp;
                if (resp.empty()){ cout<<"El nombre no puede estar vacío. Edición cancelada."<<endl; break; }
                if (resp.length() >= sizeof(tienda->productos[idBuscado-1].nombre)){ cout<<"El nombre es demasiado largo. Edición cancelada."<<endl; break; }
                // Asignar nuevo nombre al producto
                if (tienda->productos[idBuscado-1].nombre != nullptr) delete[] tienda->productos[idBuscado-1].nombre;
                if (respt=="CANCELAR" || resp=="0"){ cout<<"Edición cancelada."<<endl; break; }
                
                tienda->productos[idBuscado-1].nombre = resp;
                cout<<"Nombre actualizado."<<endl;

                break;
            case "3":
                // Editar descripción
                break;
            case "4":
                // Editar proveedor (validar existencia)
                break;
            case "5":
                // Editar precio (validar > 0)
                break;
            case "6":
                // Editar stock (validar > 0)
                break;
            case "7":
                // Guardar cambios (confirmar antes)
                break;
            case "0":
                cout<<"Edición cancelada sin guardar."<<endl;
                break;
            default:
                cout<<"Opción inválida."<<endl;
        }
    }
    
    }


}



int main(){
    Tienda tienda;
    inicializarTienda(&tienda);

    cout<<"\n--- CREAR PRODUCTOS ---\n";
    Crearproductos(&tienda);

    // Ejemplo de búsqueda (si hay transacciones)
    // buscarTransaccionesPorProducto(&tienda, 1);

    liberarTienda(&tienda);
    return 0;
}
