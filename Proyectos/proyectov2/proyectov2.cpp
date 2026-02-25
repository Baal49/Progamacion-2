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
    tienda->cantidadProductos = capP;
    tienda->siguienteIdProducto = 1;

    // Proveedores (mínimo 1 para permitir crear productos que referencien proveedores)
    int capProv = 0;
    cout<<"Ingrese la capacidad inicial de proveedores: ";
    while(cin>>capProv && capProv<=0){ cout<<"Debe ser mayor que 0. Intente nuevamente: "; }
    tienda->capacidadProveedores = capProv;
    tienda->proveedores = new Proveedor[tienda->capacidadProveedores];
    tienda->cantidadProveedores = capProv;

    // Transacciones
    int capT = 0;
    cout<<"Ingrese la capacidad inicial de transacciones: ";
    while(cin>>capT && capT<=0){ cout<<"Debe ser mayor que 0. Intente nuevamente: "; }
    tienda->capacidadTransacciones = capT;
    tienda->transacciones = new Transaccion[tienda->capacidadTransacciones];
    tienda->cantidadTransacciones = capT;

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
                redimensionarProductos(tienda);
                cout<<"Arreglo de proveedores redimensionado a capacidad "<<tienda->capacidadProveedores<<"."<<endl;
            }
            tienda->proveedores[tienda->cantidadProveedores++] = *temp;
            cout<<"Producto guardado."<<endl;
        } else {
            cout<<"Producto descartado por el usuario."<<endl;
        }
    }
}
void buscarProducto(Tienda* tienda){
    // Implementar búsqueda por ID, nombre, código o proveedor
    // Similar a buscarTransaccionesPorProducto pero con criterios diferentes
    bool encontrado =0;
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
            encontrado=1;
            return;
        }
    }
    if(!encontrado){
        cout<<"Producto no encontrado."<<endl;
    }
    




    cout<<"Desea editar el producto? (S/N): ";
    string resp;
    
    if(!getline(cin,resp)) return;
    if (resp=="N" || resp=="n" || resp=="No" || resp=="NO" || resp=="no"){
        cout<<"Edición cancelada."<<endl;
    }
    else if(resp=="S" || resp=="s" || resp=="Si" || resp=="SI" || resp=="si"){
        editarProducto(tienda, idBuscado);
    }
    
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
        cout <<"9. Guardar cambios"<<endl;
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


int main(){
    Tienda tienda;
    strcpy(tienda.nombre, "Farmacia pipo");
    inicializarTienda(&tienda);

    cout<<"\n--- CREAR PRODUCTOS ---\n";
    Crearproductos(&tienda);

    // Ejemplo de búsqueda (si hay transacciones)
    // buscarTransaccionesPorProducto(&tienda, 1);

    liberarTienda(&tienda);
    return 0;
}
