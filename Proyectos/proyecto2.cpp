#include <iostream>
#include <locale>
#include <limits>
using namespace std;
//1.1 Estructura Producto

struct Producto {
    int id;                    // Identificador único (autoincremental)
    char codigo[20];           // Código del producto (ej: "PROD-001")
    char nombre[100];          // Nombre del producto
    char descripcion[200];     // Descripción del producto
    int idProveedor;           // ID del proveedor asociado
    float precio=0;              // Precio unitario
    int stock=0;                 // Cantidad en inventario
    char fechaRegistro[11];    // Formato: YYYY-MM-DD

};

//1.2 Estructura Proveedor

struct Proveedor {
    int id;                    // Identificador único (autoincremental)
    char nombre[100];          // Nombre del proveedor
    char rif[20];              // RIF o identificación fiscal
    char telefono[20];         // Teléfono de contacto
    char email[100];           // Correo electrónico
    char direccion[200];       // Dirección física
    char fechaRegistro[11];    // Formato: YYYY-MM-DD
};

// 1.3 Estructura Cliente

struct Cliente {
    int id;                    // Identificador único (autoincremental)
    char nombre[100];          // Nombre completo del cliente
    char cedula[20];           // Cédula o RIF
    char telefono[20];         // Teléfono de contacto
    char email[100];           // Correo electrónico
    char direccion[200];       // Dirección física
    char fechaRegistro[11];    // Formato: YYYY-MM-DD
};

//1.4 Estructura Transacción (CASO ESPECIAL: Esta estructura puede separarse como se comentó en clase, tienen libertad de hacerlo.)

struct Transaccion {
    int id;                    // Identificador único (autoincremental)
    char tipo[10];             // "COMPRA" o "VENTA"
    int idProducto;            // ID del producto involucrado
    int idRelacionado;         // ID del proveedor (compra) o cliente (venta)
    int cantidad;              // Cantidad de unidades
    float precioUnitario;      // Precio por unidad en esta transacción
    float total;               // cantidad * precioUnitario
    char fecha[11];            // Formato: YYYY-MM-DD
    char descripcion[200];     // Notas adicionales (opcional)
};

//1.5 Estructura Principal: Tienda

struct Tienda {
    char nombre[100];          // Nombre de la tienda
    char rif[20];              // RIF de la tienda
    
    // Arrays dinámicos de entidades
    Producto* productos;
    int numProductos=0;
    int capacidadProductos=0;
    
    Proveedor* proveedores;
    int numProveedores=0;
    int capacidadProveedores=0;
    
    Cliente* clientes;
    int numClientes=0;
    int capacidadClientes=0;
    
    Transaccion* transacciones;
    int numTransacciones=0;
    int capacidadTransacciones=0;
    
    // Contadores para IDs autoincrementales
    int siguienteIdProducto;
    int siguienteIdProveedor;
    int siguienteIdCliente;
    int siguienteIdTransaccion;
};

void inicializarTienda(Tienda* tienda){// Inicia la tienda con su capacidad inicial
    cin.ignore(numeric_limits<streamsize>::max(),'\n');
    cout<<"Ingrese el nombre de la tienda: ";
    cin.getline(tienda->nombre, 100);
    cout<<"Ingrese el RIF de la tienda: ";
    cin.getline(tienda->rif,20);
    cout<<"Ingrese la cantidad maxima de productos que se puede almacenar: ";
    cin>>tienda->capacidadProductos;
    while((tienda->numProductos<=0)||(tienda->numProductos>tienda->capacidadProductos)){
        cout<<"Ingrese la cantidad de Productos que desea registrar: ";
        cin>>tienda->numProductos;
    }
    tienda->productos =new Producto[tienda->numProductos];
    cout<<"Ingrese la cantidad maxima de provedores que se puede almacenar: ";
    cin>>tienda->capacidadProveedores;
    while((tienda->numProveedores<=0)||(tienda->numProveedores>tienda->capacidadProveedores)){
        cout<<"Ingrese la cantidad de Provedores que desea registrar: ";
        cin>>tienda->numProveedores;
    }
    tienda->proveedores =new Proveedor[tienda->numProveedores];
    cout<<"Ingrese la cantidad maxima de clientes que se puede almacenar: ";
    cin>>tienda->capacidadClientes;
    while((tienda->numClientes<=0)||(tienda->numClientes>tienda->capacidadClientes)){
        cout<<"Ingrese la cantidad de Clientes que desea registrar: ";
        cin>>tienda->numClientes;
    }
    tienda->clientes=new Cliente[tienda->numClientes];
    cout<<"Ingrese la cantidad maxima de Transacciones que se puede almacenar: ";
    cin>>tienda->capacidadTransacciones;
    while((tienda->numTransacciones<=0)||(tienda->numTransacciones>tienda->capacidadTransacciones)){
        cout<<"Ingrese la cantidad de Transacciones que desea registrar: ";
        cin>>tienda->numTransacciones;
    }
    tienda->transacciones=new Transaccion[tienda->numTransacciones];


} 

void liberarTienda(Tienda* tienda){// Libera la memoria asignada para los arrays dinámicos
// Busca transacciones por ID de producto usando punteros dinámicos
void buscarTransaccionesPorProducto(Tienda* tienda, int idProducto){
    if(tienda==nullptr){
        cout<<"Tienda no inicializada."<<endl;
        return;
    }
    if(tienda->transacciones==nullptr || tienda->numTransacciones<=0){
        cout<<"No hay transacciones registradas."<<endl;
        return;
    }

    // Reservamos un arreglo dinámico de punteros a Transaccion
    Transaccion** matches = new Transaccion*[tienda->numTransacciones];
    int encontrados = 0;

    for(int i=0;i<tienda->numTransacciones;i++){
        if(tienda->transacciones[i].idProducto == idProducto){
            matches[encontrados++] = &tienda->transacciones[i];
        }
    }

    if(encontrados==0){
        cout<<"No se encontraron transacciones para el producto con ID "<<idProducto<<"."<<endl;
        delete[] matches;
        return;
    }

    cout<<"Se encontraron "<<encontrados<<" transaccion(es) para el producto "<<idProducto<<":\n";
    for(int i=0;i<encontrados;i++){
        Transaccion* t = matches[i];
        cout<<"ID: "<<t->id<<" | Tipo: "<<t->tipo<<" | Cantidad: "<<t->cantidad
            <<" | Precio unitario: "<<t->precioUnitario<<" | Total: "<<t->total
            <<" | Fecha: "<<t->fecha<<"\n";
    }

    // Liberamos el arreglo dinámico de punteros
    delete[] matches;
}

void listarTransacciones(Tienda* tienda){
    if(tienda==nullptr){
        cout<<"Tienda no inicializada."<<endl;
        return;
    }
    if(tienda->transacciones==nullptr || tienda->numTransacciones<=0){
        cout<<"No hay transacciones registradas."<<endl;
        return;
    }  

    cout<<"Listado de Transacciones:\n";
    for(int i=0;i<tienda->numTransacciones;i++){
        Transaccion* t = &tienda->transacciones[i];
        cout<<"ID: "<<t->id<<" | Tipo: "<<t->tipo<<" | Producto ID: "<<t->idProducto<<" | Relacionado ID: "<<t->idRelacionado
            <<" | Cantidad: "<<t->cantidad<<" | Precio unitario: "<<t->precioUnitario
            <<" | Total: "<<t->total<<" | Fecha: "<<t->fecha<<"\n";


int main(){
    delete[] tienda->proveedores;
    delete[] tienda->clientes;
    delete[] tienda->transacciones;
    delete tienda;
    tienda = nullptr;
}  
void Crearproductos(Tienda* tienda){// Crea uno o varios productos de la tienda
    int x=1;
    if(tienda->productos!=nullptr){
        int *cantprodcreados=new int;
        while(*cantprodcreados<=0&&*cantprodcreados>tienda->numProductos){
            cout<<"Ingrese la cantidad de Productos que desea registrar: ";
            cin>>*cantprodcreados;
        }
        for(int i=0;i<*cantprodcreados;i++){
            cout<<"Ingrese el id del producto: ";
            cin>>tienda->productos[i].id;
            cin.ignore(numeric_limits<streamsize>::max(),'\n');
            cout<<"Ingrese el nombre del producto: ";
            cin.getline(tienda->productos[i].nombre,100);
            cout<<"ingrese la descripcion del producto: ";
            cin.getline(tienda->productos[i].descripcion,200);
            while(x==1){
                cout<<"ingrese el id del provedor del producto: ";
                cin>>tienda->productos[i].idProveedor;
                for(int j=0;j<tienda->numProveedores;j++){
                    if(tienda->proveedores[j].id==tienda->productos[i].idProveedor){
                        x=0;
                    }
                }
                if(x==1){
                    cout<<"El id del proveedor no existe, intentelo nuevamente."<<endl;
                }
            }
            while(tienda->productos[i].precio<=0){
                cout<<"ingrese el precio del producto: ";
                cin>>tienda->productos[i].precio;
                if(tienda->productos[i].precio<=0){
                    cout<<"El precio debe ser mayor a 0, por favor intentelo mas tarde."<<endl;
                }
            }
            while(tienda->productos[i].stock<=0){
                cout<<"ingrese el stock del producto: ";
                cin>>tienda->productos[i].stock;
                if(tienda->productos[i].stock<=0){
                    cout<<"El stock debe ser mayor a 0, por favor intentelo mas tarde."<<endl;
                }
            }
            cout<<"Ingrese la fecha del registro del producto con formato YYYY-MM-DD: ";
            cin.getline(tienda->productos[i].fechaRegistro,11);
        }
    }
}







int main(){
    *tienda tienda ;
    int opcion=0;
    do{
        cout << "+===========================================+"<<endl;
        cout << "║   SISTEMA DE GESTIÓN DE INVENTARIO       ║"<<endl;
        cout << "║   Tienda: "<<tienda.nombre<<"            ║"<<endl;
        cout << "+===========================================+"<<endl;
        cout << "1. Crear Tienda" << endl;
        cout << "2. Menu Productos" << endl;
        cout << "3. Menu Venta" << endl;
        cout << "4. Menu Transacciones" << endl;
        cout << "5. Menu Transacciones" << endl;
        cin >> opcion;
        switch (opcion) {
    case 1:
        inicializarTienda(&tienda);
        break;
        
    case 2:
        *registrarCompra(&tienda);
            cout << "+===========================================+\n";
            cout << "|   SISTEMA DE GESTION DE INVENTARIO        |\n";
            cout << "|   Tienda: " << tienda->nombre << "           |\n";
            cout << "+===========================================+\n";
        
            cout << "Ingrese el código del producto: ";
            cin.getline(tienda->productos.codigo, 20);
            cout << "ingrese la descripción del producto: ";
            cin.getline(producto.descripcion, 200);
            cout << "Ingrese el ID del proveedor: ";
            cin >> producto.idProveedor;
            cout << "Ingrese el precio del producto: ";
            cin >> producto.precio;
            cout << "Ingrese el stock del producto: ";
            cin >> producto.stock;
        break;
    case 3:
        //registrarVenta(&tienda);
            *registrarVenta(&tienda);
            cout << "+===========================================+\n";
            cout << "|   SISTEMA DE GESTION DE INVENTARIO        |\n";
            cout << "|   Tienda: " << tienda->nombre << "           |\n";
            cout << "+===========================================+\n";
            cout << "Ingrese el código del producto: ";
            cin.getline(tienda->productos.codigo, 20);
            cout << "Ingrese el ID del cliente: ";
            cin >> producto.idProveedor;

        break;
    case 4:
        //buscarTransacciones(&tienda);

            new *buscarTransacciones(&tienda);
             cout << "+===========================================+\n"; 
            cout << "|   SISTEMA DE GESTION DE INVENTARIO        |\n";
            cout << "|   Tienda: " << tienda->nombre << "           |\n";
            cout << "+===========================================+\n";
            cout << "Ingrese el ID del producto para buscar transacciones: ";
            cin >> producto.id;
            buscarTransaccionesPorProducto(&tienda, producto.id);

        break;
    case 5:
        //listarTransacciones(&tienda);

            *listarTransacciones(&tienda);
             cout << "+===========================================+\n";
            cout << "|   SISTEMA DE GESTION DE INVENTARIO        |\n";
            cout << "|   Tienda: [Nombre de la Tienda]           |\n";
            cout << "+===========================================+\n";
            listarTransacciones(&tienda);


        break;
    case 6:
       // cancelarTransaccion(&tienda);
        break;
    case 0:
        cout << "Operación cancelada." << endl;
        break;
    default:
        cout << "Opción no válida. Intente nuevamente." << endl;
}
    }while(opcion!=0);
return 0;
}
