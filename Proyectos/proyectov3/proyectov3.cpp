#include <iostream>
#include <locale>
#include <limits>
#include <string>
#include <cstring>
#include <iomanip>
#include <fstream>
using namespace std;

struct archivoHeader{
    int cantidadRegistros;      // Total histórico de registros
    int proximoID;              // Siguiente ID a asignar (Autoincremental)
    int registrosActivos;       // Registros que no están marcados como eliminados
    int version;                // Control de versión del archivo
};
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
struct Productoventa{
    int id;
    int cantidad;
    int preciounidad;
};
struct Transaccion {
    int id;
    int tipo;
    Productoventa* productos=nullptr ;
    int idRelacionado;
    int cantidad;
    float precioUnitario;
    float total;
    char fecha[11];
    char descripcion[200];//opcional
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
    cout<<"Ingrese el RIF de la tienda: ";
    cin.getline(tienda->rif,20);

    // Productos
    int cap = 5;
    tienda->capacidadProductos = cap;
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
        if(tienda->transacciones[i].productos[i].id == idProducto){ matches[encontrados++] = &tienda->transacciones[i]; }
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
void redimensionarTransaccion(Tienda* tienda){
    int nuevaCap = (tienda->cantidadTransacciones>0) ? tienda->capacidadTransacciones*2 : 1;
    Transaccion* nuevo = new Transaccion[nuevaCap];
    // copiar existentes
    for(int i=0;i<tienda->cantidadTransacciones;i++) nuevo[i] = tienda->transacciones[i];
    delete[] tienda->transacciones;
    tienda->transacciones = nuevo;
    tienda->capacidadTransacciones = nuevaCap;
}
void redimensionarProductosventa(Transaccion* transaccion){
    int nuevaCap = (transaccion->cantidad>0) ? transaccion->cantidad++ : 1;
    Productoventa* nuevo = new Productoventa[nuevaCap];
    // copiar existentes
    for(int i=0;i<transaccion->cantidad;i++) nuevo[i] = transaccion->productos[i];
    delete[] transaccion->productos;
    transaccion->productos = nuevo;
    transaccion->cantidad = nuevaCap;
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
            if(input=="CANCELAR" ||input=="cancelar" || input=="Cancelar"|| input=="0"){ cout<<"Creación cancelada."<<endl; return; }
            int idProv = 0;
             idProv = stoi(input); 
            try{
                for(int i=0;i<tienda->cantidadProveedores;i++){
                if(tienda->proveedores[i].id == idProv){ proveedorValido = true;  }

            }
            }
            catch(...){
                if(!proveedorValido) cout<<"El id del proveedor no existe, intentelo nuevamente."<<endl;
            
            }temp.idProveedor = idProv;
            
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
    if(tienda->proveedores==nullptr){ cout<<"Tienda no inicializada."<<endl; return; }
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
        //temp.id = tienda->siguienteIdProveedor++;

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
            tienda->proveedores[tienda->cantidadProveedores] = temp;
            tienda->cantidadProveedores++;
            cout<<"Proveedor guardado."<<endl;
        } else {
            cout<<"Proveedor descartado por el usuario."<<endl;
        }
    }
}
    void Crearcliente(Tienda* tienda){
        if(tienda->clientes!=nullptr){ 
        string input;
        bool cedulaExiste=1;
        int cantidad = 0;
        cout<<"Ingrese la cantidad de clientes a crear (o 'CANCELAR'/'0' para cancelar): ";
        while(true){
            if(!getline(cin, input)) return; // Leer línea completa para permitir espacios
            if(input=="CANCELAR" || input=="0") { cout<<"Creación cancelada."<<endl; return; }
            try{ cantidad = stoi(input); }
            catch(...){ cout<<"Entrada invalida. Ingrese un numero: "; continue; }
            if(cantidad>0) break;
            cout<<"La cantidad debe ser mayor a 0. Intente nuevamente: ";
        }
        for(int n=0;n<cantidad;n++){
            Cliente temp{};

            // Cedula (única)
            while(cedulaExiste){
                cout<<"Ingrese la cedula del cliente (o 'CANCELAR' para cancelar): ";
                if(!getline(cin, input)) return;
                if(input=="CANCELAR" || input=="0"){ cout<<"Creación cancelada."<<endl; return; }
                if(input.empty()){ cout<<"La cedula no puede estar vacía."<<endl; continue; }
                // Validar que la cedula no exista ya
                 cedulaExiste = false;
                for(int i=0;i<tienda->cantidadClientes;i++){
                    if(tienda->clientes[i].cedula == stoi(input)){
                        cedulaExiste = true;
                        break;
                    }
                for (int i=0;i<input.length();i++){
                    if(!isdigit(input[i])){
                        cout<<"La cedula solo puede contener numeros."<<endl;
                        cedulaExiste=true;
                        break;
                    }
                }
                if(cedulaExiste){ cout<<"La cedula ya existe. Ingrese otra."<<endl; continue; }
                tienda->clientes[i].cedula = stoi(input);
                break;
            }
        }
   
            // Nombre
            cout<<"Ingrese el nombre del cliente (o 'CANCELAR' para cancelar): ";
            if(!getline(cin,input)) return;
            if(input=="CANCELAR" || input=="0"){ cout<<"Creación cancelada."<<endl; return; }
            strncpy(temp.nombre, input.c_str(), sizeof(temp.nombre)-1);

            // Correo
            cout<<"Ingrese el correo del cliente (o 'CANCELAR' para cancelar): ";
            if(!getline(cin,input)) return;
            if(input=="CANCELAR" || input=="0"){ cout<<"Creación cancelada."<<endl; return; }
            strncpy(temp.correo, input.c_str(), sizeof(temp.correo)-1);
            for (int i=0;i<strlen(temp.correo);i++){
                if(temp.correo[i]==' '){
                    cout<<"Correo invalido. No se permiten espacios."<<endl;
                    strncpy(temp.correo, "", sizeof(temp.correo)-1);
                    break;
                }
            }
            for (int i=0;i<strlen(temp.correo);i++){
                if(temp.correo[i]=='@'){
                    bool puntoEncontrado=false;
                    for(int j=i+1;j<strlen(temp.correo);j++){
                        if(temp.correo[j]=='.'){
                            puntoEncontrado=true;
                            break;
                        }
                    }
                    if(!puntoEncontrado){
                        cout<<"Correo invalido. Debe contener un punto (.) después del @."<<endl;
                        strncpy(temp.correo, "", sizeof(temp.correo)-1);
                        break;
                    }
                    
                }
            }
            // Telefono
            cout<<"Ingrese el telefono del cliente (o 'CANCELAR' para cancelar): ";
            if(!getline(cin,input)) return;
            if(input=="CANCELAR" || input=="0"){ cout<<"Creación cancelada."<<endl; return; }
            strncpy(temp.telefono, input.c_str(), sizeof(temp.telefono)-1);
            for (int i=0;i<strlen(temp.telefono);i++){
                if(!isdigit(temp.telefono[i]) && temp.telefono[i]!='+' && temp.telefono[i]!='-' && temp.telefono[i]!=' '){
                    cout<<"Telefono invalido. Solo se permiten numeros, espacios, + y -."<<endl;
                    strncpy(temp.telefono, "", sizeof(temp.telefono)-1);
                    break;
                }
            }
             // Dirreccion
            cout<<"Ingrese la dirreccion del cliente (o 'CANCELAR' para cancelar): ";
            if(!getline(cin,input)) return;
            if(input=="CANCELAR" || input=="0"){ cout<<"Creación cancelada."<<endl; return; }
            strncpy(temp.dirreccion, input.c_str(), sizeof(temp.dirreccion)-1);

             // Fecha de registro
            cout<<"Ingrese la fecha de registro del cliente (o 'CANCELAR' para cancelar): ";
            if(!getline(cin,input)) return;
            if(input=="CANCELAR" || input=="0"){ cout<<"Creación cancelada."<<endl; return; }
            strncpy(temp.fecharegitro, input.c_str(), sizeof(temp.fecharegitro)-1);

            // Agregar cliente temporal al array de clientes
            tienda->clientes[tienda->cantidadClientes] = temp;
            tienda->cantidadClientes++;
            cout<< tienda->clientes[tienda->cantidadClientes].cedula<<tienda->clientes[tienda->cantidadClientes].nombre<< endl;
        }
 

        }
    }
        void Editarcliente (Tienda* tienda, int cedula){
            // Implementar función para editar los datos de un cliente existente
            string input;
            for(int i=0;i<tienda->cantidadClientes;i++){
                if(tienda->clientes[i].cedula == cedula){
                    // Encontrado el cliente, mostrar opciones de edición
                    cout<<"Cliente encontrado: "<<tienda->clientes[i].nombre<<" | Cedula: "<<tienda->clientes[i].cedula<<" | Correo: "<<tienda->clientes[i].correo<<" | Telefono: "<<tienda->clientes[i].telefono<<" | Dirreccion: "<<tienda->clientes[i].dirreccion<<" | Fecha de registro: "<<tienda->clientes[i].fecharegitro<<endl;
                    cout<<"¿Qué desea editar?"<<endl;
                    cout<<"1. Nombre"<<endl;
                    cout<<"2. Correo"<<endl;
                    cout<<"3. Telefono"<<endl;
                    cout<<"4. Dirreccion"<<endl;
                    cout<<"5. Fecha de registro"<<endl;
                    cout<<"0. Cancelar"<<endl;
                    int opcion;
                    cin>>opcion;
                    cin.ignore(numeric_limits<streamsize>::max(),'\n'); // Limpiar el buffer después de leer la opción
                    switch(opcion){
                        case 0:
                            cout<<"Edición cancelada."<<endl;
                            return; 
                        case 1:
                            cout<<"Ingrese el nuevo nombre del cliente (o 'CANCELAR' para cancelar): ";
                            
                            if(!getline(cin,input)) return;
                            if(input=="CANCELAR" || input=="0"){ cout<<"Edición cancelada."<<endl; return; }
                            strncpy(tienda->clientes[i].nombre, input.c_str(), sizeof(tienda->clientes[i].nombre)-1);
                            break;
                        case 2:
                            cout<<"Ingrese el nuevo correo del cliente (o 'CANCELAR' para cancelar): ";
                            if(!getline(cin,input)) return;
                            if(input=="CANCELAR" || input=="0"){ cout<<"Edición cancelada."<<endl; return; }
                            strncpy(tienda->clientes[i].correo, input.c_str(), sizeof(tienda->clientes[i].correo)-1);
                            break;
                        case 3:
                            cout<<"Ingrese el nuevo telefono del cliente (o 'CANCELAR' para cancelar): ";
                            if(!getline(cin,input)) return;
                            if(input=="CANCELAR" || input=="0"){ cout<<"Edición cancelada."<<endl; return; }
                            strncpy(tienda->clientes[i].telefono, input.c_str(), sizeof(tienda->clientes[i].telefono)-1);
                            break;
                        case 4:
                            cout<<"Ingrese la nueva dirreccion del cliente (o 'CANCELAR' para cancelar): ";
                            if(!getline(cin,input)) return;
                            if(input=="CANCELAR" || input=="0"){ cout<<"Edición cancelada."<<endl; return; }
                            strncpy(tienda->clientes[i].dirreccion, input.c_str(), sizeof(tienda->clientes[i].dirreccion)-1);
                            break;
                        case 5:
                            cout<<"Ingrese la nueva fecha de registro del cliente (o 'CANCELAR' para cancelar): ";
                            if(!getline(cin,input)) return;
                            if(input=="CANCELAR" || input=="0"){ cout<<"Edición cancelada."<<endl; return; }
                            strncpy(tienda->clientes[i].fecharegitro, input.c_str(), sizeof(tienda->clientes[i].fecharegitro)-1);
                            break;
                        
                        default:
                            cout<<"Opción inválida. Edición cancelada."<<endl;
                            return;

        }
    }
}
        }
    void Listarcliente (Tienda* tienda){
        if(tienda==nullptr){ cout<<"Tienda no inicializada."<<endl; return; }
        if(tienda->clientes==nullptr || tienda->cantidadClientes<=0){ cout<<"No hay clientes registrados."<<endl; return; }
        cout<<"Listado de clientes:\n";
        for(int i=0;i<tienda->cantidadClientes;i++){
            Cliente& c = tienda->clientes[i];
            cout<<"Cedula: "<<c.cedula<<" | Nombre: "<<c.nombre<<" | Correo: "<<c.correo<<" | Telefono: "<<c.telefono<<" | Dirreccion: "<<c.dirreccion<<" | Fecha de registro: "<<c.fecharegitro<<"\n";
        }
        for (int i=0;i<tienda->cantidadClientes;i++){
            cout<<"Cliente "<<i+1<<": Cedula: "<<tienda->clientes[i].cedula<<" | Nombre: "<<tienda->clientes[i].nombre<<" | Correo: "<<tienda->clientes[i].correo<<" | Telefono: "<<tienda->clientes[i].telefono<<" | Dirreccion: "<<tienda->clientes[i].dirreccion<<" | Fecha de registro: "<<tienda->clientes[i].fecharegitro<<"\n";      
        }

    }

    void Creartransaccion(Tienda* tienda){
        if(tienda==nullptr){ cout<<"Tienda no inicializada."<<endl; return; }
        Transaccion temp{};
        string input;
        string resp;
        // Tipo de transacción
        while(true){
            cout<<"Ingrese el tipo de transacción (1=Venta, 2=Compra) o 'CANCELAR' para cancelar: ";
            if(!getline(cin,input)) return;
            if(input=="CANCELAR" || input=="0"){ cout<<"Creación cancelada."<<endl; return; }
            try{ temp.tipo = stoi(input); }
            catch(...){ cout<<"Tipo invalido. Ingrese 1 para Venta o 2 para Compra: "; continue; }
            if(temp.tipo==1 || temp.tipo==2) break;
            cout<<"Tipo invalido. Ingrese 1 para Venta o 2 para Compra: ";
        }
        
        // Fecha
        cout<<"Ingrese la fecha de la transacción (YYYY-MM-DD) o 'CANCELAR' para cancelar: ";
        if(!getline(cin,input)) return;
        if(input=="CANCELAR" || input=="0"){ cout<<"Creación cancelada."<<endl; return; }
        strncpy(temp.fecha, input.c_str(), sizeof(temp.fecha)-1);
        // Descripcion (opcional)
        cout<<"Ingrese una descripcion para la transaccion (opcional, 'CANCELAR' para cancelar): ";
        if(!getline(cin,input)) return;
        if(input=="CANCELAR" || input=="0"){ cout<<"Creación cancelada."<<endl; return; }
        strncpy(temp.descripcion, input.c_str(), sizeof(temp.descripcion)-1);
        //Asignar ID autoincremental
        temp.id = tienda->siguienteIdTransaccion++;

        // Confirmación
        cout<<"\nResumen de la transacción:"<<endl;
        cout<<"ID: "<<temp.id<<" | Tipo: "<<temp.tipo<<" | Fecha: "<<temp.fecha<<" | Descripcion: "<<temp.descripcion<<"\n";
        cout<<"¿Desea guardar esta transacción? (S/N): ";
        cin >> resp ;
        cin.ignore(numeric_limits<streamsize>::max(),'\n'); // Limpiar el buffer después de leer la respuesta
        if(resp=="S" || resp=="s" || resp=="Si" || resp=="SI" || resp=="si"){
            // si el arreglo está lleno, redimensionar duplicando capacidad
            if(tienda->cantidadTransacciones >= tienda->capacidadTransacciones){
                redimensionarTransaccion(tienda);
                cout << "Arreglo de transacciones redimensionado a capacidad " << tienda->capacidadTransacciones << "." << endl;
                for (int i = 0; i < tienda->cantidadTransacciones; i++) {
                    cout << "Transaccion " << i + 1 << ": ID: " << tienda->transacciones[i].id << " | Tipo: " << tienda->transacciones[i].tipo << " | Fecha: " << tienda->transacciones[i].fecha << " | Descripcion: " << tienda->transacciones[i].descripcion << "\n";
                    
}
            tienda->transacciones[tienda->cantidadTransacciones++] = temp;
            cout<<"Transacción guardada."<<endl;
        } else {
            cout<<"Transacción descartada por el usuario."<<endl;
        }
}
}
    void Buscartransaccion (Tienda* tienda, int id){
        if(tienda==nullptr){ cout<<"Tienda no inicializada."<<endl; return; }
        if(tienda->transacciones==nullptr || tienda->cantidadTransacciones<=0){ cout<<"No hay transacciones registradas."<<endl; return; }
        bool encontrado = false;
        for(int i=0;i<tienda->cantidadTransacciones;i++){
            if(tienda->transacciones[i].id == id){
                Transaccion& t = tienda->transacciones[i];
                cout<<"Transacción encontrada: ID: "<<t.id<<" | Tipo: "<<t.tipo<<" | Fecha: "<<t.fecha<<" | Descripcion: "<<t.descripcion<<"\n";
                encontrado = true;
                break;
            }
        }
        if(!encontrado){
            cout<<"Transacción con ID "<<id<<" no encontrada."<<endl;
        }
        for (int i=0;i<tienda->cantidadTransacciones;i++){
            cout<<"Transaccion "<<i+1<<": ID: "<<tienda->transacciones[i].id<<" | Tipo: "<<tienda->transacciones[i].tipo<<" | Fecha: "<<tienda->transacciones[i].fecha<<" | Descripcion: "<<tienda->transacciones[i].descripcion<<"\n";
        }
        for (int i=0;i<tienda->cantidadTransacciones;i++){
            if(tienda->transacciones[i].id==id){
                cout<<"Transaccion encontrada: ID: "<<tienda->transacciones[i].id<<" | Tipo: "<<tienda->transacciones[i].tipo<<" | Fecha: "<<tienda->transacciones[i].fecha<<" | Descripcion: "<<tienda->transacciones[i].descripcion<<"\n";
                return;
            }
        }
        cout<<"Transaccion con ID "<<id<<" no encontrada."<<endl;
    }


Producto buscarProducto(Tienda* tienda,int id,string nombre, int opcion){
    // Implementar búsqueda por ID, nombre, código o proveedor
    // Similar a buscarTransaccionesPorProducto pero con criterios diferentes
    
    int posicion;
    if(opcion==1){
    bool encontrado =0;
    try{ id; }
    catch(...){ cout<<"ID invalido."<<endl;  }
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
            return tienda->productos[opcion2];
    }
}
return tienda->productos[posicion];
}

    void listarProductos(Tienda* tienda){
        if(tienda==nullptr){ cout<<"Tienda no inicializada."<<endl; return; }
        if(tienda->productos==nullptr || tienda->cantidadProductos<=0){ cout<<"No hay productos registrados."<<endl; return; }
                 
        // cabecera del cuadro
        cout<<"╔══════════════════════════════════════════════════════════════════════════╗\n";
        cout<<"║                         LISTADO DE PRODUCTOS                             ║\n";
        cout<<"╠════╦═══════════╦══════════════════╦══════════════╦═══════╦════════╦══════╣\n";
        cout<<"║ ID ║  Código   ║     Nombre       ║  Proveedor   ║ Precio║ Stock  ║ Fecha║\n";
        cout<<"╠════╬═══════════╬══════════════════╬══════════════╬═══════╬════════╬══════╣\n";

        for(int i=0;i<tienda->cantidadProductos;i++){
            Producto& p = tienda->productos[i];
            const char* provName = "N/A";
            for(int j=0;j<tienda->cantidadProveedores;j++){
                if(tienda->proveedores[j].id == p.idProveedor){
                    provName = tienda->proveedores[j].nombre;
                    break;
                }
            }
            cout<<"║ "<<setw(2)<<p.id<<" ║ "
                <<setw(9)<<p.codigo<<" ║ "
                <<setw(16)<<p.nombre<<" ║ "
                <<setw(12)<<provName<<" ║ "
                <<setw(6)<<fixed<<setprecision(2)<<p.precio<<"║ "
                <<setw(6)<<p.stock<<" ║ "
                <<setw(4)<<p.fechaRegistro<<" ║\n";
        }
        cout<<"╚════╩═══════════╩══════════════════╩══════════════╩═══════╩════════╩══════╝\n";
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
void editarProveedor(Tienda* tienda, int idProveedor){
    //Funcion para editar algun aspecto del producto
    int respuesta=0;
    int idBuscado=0;
    bool encontrado=false;
    int resp;
    Proveedor temp;
    
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
    temp=tienda->proveedores[idBuscado];
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
                   if ( respuesta==0){ cout<<"Edición cancelada."<<endl; break; }
                // Asignar nuevo código al producto
                    temp.id=respuesta;
                break;
            case 2:
                // Editar nombre.
                
                cout<<"Ingrese el nuevo nombre del proveedor: ";
                getline(cin,respp);
                while (respp.empty()){ cout<<"El nombre no puede estar vacío."<<endl; getline(cin,respp); }
                while (respp.length() >= sizeof(tienda->proveedores[idBuscado].nombre)){ cout<<"El nombre es demasiado largo. Coloque otro nombre."<<endl; getline(cin,respp); }
                // Asignar nuevo nombre al producto
                if (respp=="CANCELAR" || respp=="0"){ cout<<"Edición cancelada."<<endl; break; }
                strcpy(temp.nombre,respp.c_str());
                respp.clear();
                break;
            case 3:
                // Guardar cambios (confirmar antes)
                cout<<"Producto antes: ID: "<<tienda->proveedores[idBuscado].id<<" | Nombre: "<<tienda->proveedores[idBuscado].nombre<<endl;
                cout<<"Producto Despues: ID: "<<tienda->productos[idBuscado].id<<" | Nombre: "<<temp.nombre<<endl;
                cout<<"¿Desea guardar los cambios realizados al proveedor? (S/N): ";
                tienda->proveedores[idBuscado]=temp;
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

Proveedor* buscarProveedor(Tienda* tienda,int id,string nombre,int opcion){
    // Implementar búsqueda por ID, nombre, código o proveedor
    // Similar a buscarTransaccionesPorProducto pero con criterios diferentes
    if(tienda->proveedores!=nullptr){
    if(opcion==1){
    bool encontrado =0;
    cin.ignore(numeric_limits<streamsize>::max(),'\n');
    Proveedor* p;
    int idBuscado = 0;
    try{ idBuscado = id; }
    catch(...){ cout<<"ID invalido."<<endl;  }
    for(int i=0;i<tienda->cantidadProveedores;i++){
        cout<<"id: "<<tienda->proveedores[i].id<<endl;
        if( tienda->proveedores[i].id== id){
             *p = tienda->proveedores[i];
            cout<<"proveedor encontrado: ID: "<<p->id<<" | Nombre: "<<p->nombre<<endl;
            encontrado=1;
            return p;
        }
    }
    if(!encontrado){
        cout<<"Proveedor no encontrado."<<endl;
        return nullptr;
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
        }
    }
}
}
string buscarCliente(Tienda* tienda,int cedula){
    cout<<"Si entre aqui"<<endl;
    if(tienda->clientes!=nullptr){
        int posicion=-1;
        for(int i=0;i<tienda->cantidadClientes;i++){
            cout<<"Hoaa"<<endl;
            if(tienda->clientes[i].cedula==cedula){
                cout<<"adios"<<endl;
                posicion=i;
                break;
            }
        }
        if(posicion!=-1){
            cout<<"nombre del cliente: "<<tienda->clientes[posicion].nombre;
            return tienda->clientes[posicion].nombre,tienda->clientes[posicion].dirreccion;
        }
        else{
            
        }
    }
    else{
        cout<<"Tienda es null ptr:"<<tienda->clientes[0].cedula;
        return "no existe ese cliente";
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
void editarProducto(Tienda* tienda, int idProducto){
    //Funcion para editar algun aspecto del producto
    
    int idBuscado=0;
    int provvaalido=0;
    bool encontrado=false;
    int resp;
    Producto temp;
    cout<<"id producto: "<<idProducto;
    
    string respp;
    for(int i=0;i<tienda->cantidadProductos;i++){
        if(tienda->productos[i].id==idProducto){
            idBuscado=i;
            encontrado=true;
            break;
        }
    }
    if(!encontrado){
        cout<<"Producto no encontrado."<<endl;
        return;
    } 
    temp = tienda->productos[idBuscado];
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
                   while(codigoDuplicado(tienda, respp)) { cout<<"Codigo ya existe Intentelo nuevamente."<<endl; getline(cin,respp); }
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
                while (respp.length() >= sizeof(tienda->productos[idBuscado].nombre)){ cout<<"El nombre es demasiado largo. Coloque otro nombre."<<endl; getline(cin,respp); }
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
                while(respp.length()>sizeof(tienda->productos[idBuscado].descripcion)){cout<<"La descripcion es demasiado larga."<<endl; getline(cin,respp);}
                if (respp=="CANCELAR" || respp=="0"){ cout<<"Edición cancelada."<<endl; break; }
                strcpy(temp.descripcion,respp.c_str());
                cout<<"Descripción actualizada."<<endl;
                break;
            case 4:
                // Editar proveedor (validar existencia)
                    cout<<"Ingrese el nuevo ID del proveedor del producto: ";
                    cin>>resp;
                    
                    while(provvaalido==0){
                    for(int i=0;i<tienda->cantidadProveedores;i++){
                        if(tienda->proveedores[i].id==resp){
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
                while(respp.length()>sizeof(tienda->productos[idBuscado].fechaRegistro)){cout<<"La fecha de registro es demasiado larga."<<endl; getline(cin,respp);}
                if (respp=="CANCELAR" || respp=="0"){ cout<<"Edición cancelada."<<endl; break; }
                strcpy(temp.fechaRegistro,respp.c_str());
                cout<<"Fecha de registro actualizada."<<endl;
                break;
            case 8:
                // Editar fecha de vencimiento
                cout<<"ingrese la nueva fecha de vencimiento del producto con Formato(YYYY-MM-DD): ";
                getline(cin,respp);
                while(respp.empty()){cout<<"La fecha de vencimiento no puede estar vacia."<<endl;getline(cin,respp);}
                while(respp.length()>sizeof(tienda->productos[idBuscado].fechavencimiento)){cout<<"La fecha de vencimiento es demasiado larga."<<endl; getline(cin,respp);}
                if (respp=="CANCELAR" || respp=="0"){ cout<<"Edición cancelada."<<endl; break; }
                strcpy(temp.fechavencimiento,respp.c_str());
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
                cout<<"Producto Despues: ID: "<<tienda->productos[idBuscado].id<<" | Codigo: "<<temp.codigo<<" | Nombre: "<<temp.nombre<<" | Precio: "<<temp.precio<<" | Stock: "<<temp.stock<<" | Proveedor ID: "<<temp.idProveedor<<" | Fecha de Registro: "<<temp.fechaRegistro<<" | Fecha de Vencimiento: "<<temp.fechavencimiento<<endl;
                cout<<"¿Desea guardar los cambios realizados al producto? (S/N): ";
                tienda->productos[idBuscado]=temp;
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


void venta(Tienda* tienda){
    Producto* p;
    int cedu;
    string input;
    string respuesta;
    string nombre,direccion;
    cout<<"Inserte la cedula del cliente";
    cin>>cedu;
    nombre,direccion=buscarCliente(tienda,cedu);
    if(direccion=="no existe ese cliente"||nombre=="no existe ese cliente"){
        cout<<nombre<<" desea registrarlo?"<<endl;
        cout<<"Introduzca S para registrarlo o N para cancelar";
        cin>>respuesta;
        if(respuesta=="S"||respuesta=="s"){
           
           
            Crearcliente(tienda);
        }
        else if(respuesta=="CANCELAR" || respuesta=="0") {
            cout<<"Creación cancelada."<<endl; return; 
            return ;
        }
    }
        
    

    else{
        cout<<"Nombre del cliente: "<<nombre<<" Cedula: "<<cedu<<" Direccion: "<<direccion;
        int opt,id,sub,cantp=1;
        bool cantver=0;
        tienda->transacciones[tienda->siguienteIdTransaccion-1].productos=new Productoventa[cantp];
        do{
            int cant;
            cout<<"introduce el id del producto que se va a llevar: ";
            cin>>id;
            *p=buscarProducto(tienda,id,"",1);
            cout<<"El precio del producto es: "<<p->precio<<endl;
            do{
            cout<<"introduce la cantidad del producto que se va a llevar: ";
            cin>>cant;
            if(cant>p->stock){
                cout<<"el producto no tiene esa cantidad intentelo de nuevo La existencia del producto es: "<<p->stock;
                cantver=1;
            }
        }while(cantver==1);
        sub+=(cant*p->precio);
        string input;
        tienda->transacciones[tienda->siguienteIdTransaccion-1].productos[cantp-1].id=p->id;
        tienda->transacciones[tienda->siguienteIdTransaccion-1].productos[cantp-1].cantidad=cant;
        tienda->transacciones[tienda->siguienteIdTransaccion-1].productos[cantp-1].preciounidad=p->precio;
        do{
        cout<<"desea incluir otro producto? S para si N para no";
        getline(cin,input);
        if(input=="S"||input=="s"){
            opt=1;
            redimensionarProductosventa(tienda->transacciones);
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
        tienda->transacciones[tienda->siguienteIdTransaccion-1].id=tienda->siguienteIdTransaccion;
        tienda->transacciones[tienda->siguienteIdTransaccion-1].tipo=1;
        tienda->transacciones[tienda->siguienteIdTransaccion-1].idRelacionado=cedu;
        tienda->transacciones[tienda->siguienteIdTransaccion-1].total=sub;
        cout<<"introduzca la fecha de la transaccion en formato YYYY-MM-DD: ";
        strncpy(tienda->transacciones[tienda->siguienteIdTransaccion-1].fecha, input.c_str(), sizeof(tienda->transacciones[tienda->siguienteIdTransaccion-1].fecha)-1);

    }
}
void compra(Tienda* tienda){
    Producto* p;
    Proveedor* prov;
    int idprov;
    char respuesta;
    string nombre,direccion;
    cout<<"Inserte el id del proveedor: ";
    cin>>idprov;
    *prov=*buscarProveedor(tienda,idprov,"",1);
    if(nombre=="no existe ese proveedor"){
        cout<<nombre<<" desea registrarlo?"<<endl;
        cout<<"Introduzca S para registrarlo o N para cancelar";
        cin>>respuesta;
        if(respuesta=='S'||respuesta=='s'){
             Crearproveedor(tienda);
        }
        else{
            return ;
        }
    }
    else{
        cout<<"Nombre del proveedor: "<<nombre<<" ID: "<<idprov;
        int opt,id,sub,cantp=1;
        bool cantver=0;
        tienda->transacciones[tienda->siguienteIdTransaccion-1].productos=new Productoventa[cantp];
        do{
            int cant;
            cout<<"introduce el id del producto que se va a comprar: ";
            cin>>id;
            *p=buscarProducto(tienda,id,"",1);
            //if(p->precio>0){
            
            cout<<"El precio del producto es: "<<p->precio<<endl;
            do{
            cout<<"introduce la cantidad del producto que se va a comprar: ";
            cin>>cant;
            if(cant<=0){
                cout<<"la cantidad debe ser mayor a 0 intentelo de nuevo.";
                cantver=1;
            }
        }while(cantver==1);
        sub+=(cant*p->precio);
        string input;
        tienda->transacciones[tienda->siguienteIdTransaccion-1].productos[cantp-1].id=p->id;
        tienda->transacciones[tienda->siguienteIdTransaccion-1].productos[cantp-1].cantidad=cant;
        tienda->transacciones[tienda->siguienteIdTransaccion-1].productos[cantp-1].preciounidad=p->precio;
        do{
        cout<<"desea incluir otro producto? S para si N para no";
        getline(cin,input);
        if(input=="S"||input=="s"){
            opt=1;
            redimensionarProductosventa(tienda->transacciones);
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
        tienda->transacciones[tienda->siguienteIdTransaccion-1].id=tienda->siguienteIdTransaccion;
        tienda->transacciones[tienda->siguienteIdTransaccion-1].tipo=2;
        tienda->transacciones[tienda->siguienteIdTransaccion-1].idRelacionado=idprov;
        tienda->transacciones[tienda->siguienteIdTransaccion-1].total=sub;
        cout<<"introduzca la fecha de la transaccion en formato YYYY-MM-DD: ";
        string input;
        strncpy(tienda->transacciones[tienda->siguienteIdTransaccion-1].fecha, input.c_str(), sizeof(tienda->transacciones[tienda->siguienteIdTransaccion-1].fecha)-1);

    

}
}

int main(){
     Tienda tienda;
    strcpy(tienda.nombre, "Farmacia pipo");
    inicializarTienda(&tienda);
    
    
        int opcion;
    do{
        int opt=0,id;
    string nombre,direccion;
   
        cout <<"╔═══════════════════════════════════════════╗"<<endl;
        cout <<"║   SISTEMA DE GESTIÓN DE INVENTARIO        ║"<<endl;
        cout <<"║   Tienda: "<<tienda.nombre<<          "\t\t    ║"<<endl;
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
                        Crearproductos(&tienda);
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
                        buscarProducto(&tienda,id,nombre,opt);
                        break;
                    case 3:
                        cout<<"ingrese el id del producto: ";
                        cin>>id;
                        editarProducto(&tienda,id);
                        break;
                    case 4:
                        //actualizarStock(&tienda);
                        break;
                    case 5:
                        listarProductos(&tienda);
                        break;
                    case 6:
                        cout<<"ingrese el id del del producto que se quiere borrar";
                        cin>>id;
                        eliminarProducto(&tienda, id); 
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
                        *buscarProveedor(&tienda,id,nombre,opcion);
                        break;
                    case 3:
                        cout<<"ingrese el id del del producto que se quiere editar";
                        cin>>id;
                        editarProveedor(&tienda,id);
                        break;
                    case 4:
                        listarProveedores(&tienda);
                        break;
                    case 5:
                        cout<<"ingrese el id del del producto que se quiere borrar";
                        cin>>id;
                        eliminarProveedor(&tienda, id); 
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
                        Crearcliente(&tienda);
                        break;
                    case 2:
                        cout<<"Inserte la cedula del cliente";
                        cin>>id;
                        id,nombre,direccion=buscarCliente(&tienda,id);
                        break;
                    case 3:
                        cout<<"Inserte la cedula del cliente";
                        cin>>id;
                        Editarcliente(&tienda,id);
                        break;
                    case 4:
                        //listarClientes(&tienda);
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
                        compra(&tienda);
                        break;
                    case 2:
                        venta(&tienda);
                        break;
                    case 3:
                        //editarCliente(&tienda);
                        break;
                    case 4:
                        //listarClientes(&tienda);
                        break;
                    case 5:
                        //eliminarCliente(&tienda, 1); 
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
                liberarTienda(&tienda);
                break;
            default:
                cout<<"Opción inválida. Intente nuevamente."<<endl;
        }


    cout<<"\n--- CREAR PRODUCTOS ---\n";
    /*Crearproductos(&tienda);
    buscarProducto(&tienda);*/
    // Ejemplo de búsqueda (si hay transacciones)
    // buscarTransaccionesPorProducto(&tienda, 1);

    
    
}while(opcion!=5);

}

