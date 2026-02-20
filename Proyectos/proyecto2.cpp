#include <iostream>
#include <locale>
//1.1 Estructura Producto

struct Producto {
    int id;                    // Identificador único (autoincremental)
    char codigo[20];           // Código del producto (ej: "PROD-001")
    char nombre[100];          // Nombre del producto
    char descripcion[200];     // Descripción del producto
    int idProveedor;           // ID del proveedor asociado
    float precio;              // Precio unitario
    int stock;                 // Cantidad en inventario
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
    int numProductos;
    int capacidadProductos;
    
    Proveedor* proveedores;
    int numProveedores;
    int capacidadProveedores;
    
    Cliente* clientes;
    int numClientes;
    int capacidadClientes;
    
    Transaccion* transacciones;
    int numTransacciones;
    int capacidadTransacciones;
    
    // Contadores para IDs autoincrementales
    int siguienteIdProducto;
    int siguienteIdProveedor;
    int siguienteIdCliente;
    int siguienteIdTransaccion;
};

void inicializarTienda(Tienda* tienda){// Inicia la tienda con su capacidad inicial
    cout<<"Ingrese el nombre de la tienda: ";
    cin.getline(tienda->nombre, 100);
    cout<<"Ingrese el RIF de la tienda: ";
    cin.getline(tienda->rif,20);
    cout<<"Ingrese la cantidad maxima de productos que se puede almacenar: ";
    cin>>tienda->capacidadProductos;
    while((tienda->numProductos<=0)&&(tienda->numProductos>tienda->capacidadProductos)){
        cout<<"Ingrese la cantidad de Productos que desea registrar: ";
        cin>>tienda->numProductos;
    }
    tienda->productos =new Producto[tienda->numProductos];
    cout<<"Ingrese la cantidad maxima de provedores que se puede almacenar: ";
    cin>>tienda->capacidadProveedores;
    while((tienda->numProveedores<=0)&&(tienda->numProveedores>tienda->capacidadProveedores)){
        cout<<"Ingrese la cantidad de Provedores que desea registrar: ";
        cin>>tienda->numProveedores;
    }
    tienda->proveedores =new Proveedor[tienda->numProvedores];
    cout<<"Ingrese la cantidad maxima de clientes que se puede almacenar: ";
    cin>>tienda->capacidadClientes;
    while((tienda->numClientes<=0)&&(tienda->numClientes>tienda->capacidadClientes)){
        cout<<"Ingrese la cantidad de Clientes que desea registrar: ";
        cin>>tienda->numClientes;
    }
    tienda->clientes=new Cliente[tienda->numClientes];
    cout<<"Ingrese la cantidad maxima de Transacciones que se puede almacenar: ";
    cin>>tienda->capacidadTransacciones;
    while((tienda->numTransacciones<=0)&&(tienda->numTransacciones>tienda->capacidadTransacciones)){
        cout<<"Ingrese la cantidad de Transacciones que desea registrar: ";
        cin>>tienda->numTransacciones;
    }
    tienda->transacciones=new Transaccion[tienda->numTransacciones];


} 

void liberarTienda(Tienda* tienda){// Libera la memoria asignada para los arrays dinámicos

}  







int main(){
switch (opcion) {
    case 1:
        
    case 2:
        registrarCompra(&tienda);
            cout << "+===========================================+\n";
            cout << "|   SISTEMA DE GESTION DE INVENTARIO        |\n";
            cout << "|   Tienda: [Nombre de la Tienda]           |\n";
            cout << "+===========================================+\n";
        
            cout << "Ingrese el código del producto: ";
            cin.getline(producto.codigo, 20);
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
        registrarVenta(&tienda);
        break;
    case 4:
        buscarTransacciones(&tienda);
        break;
    case 5:
        listarTransacciones(&tienda);
        break;
    case 6:
        cancelarTransaccion(&tienda);
        break;
    case 0:
        std::cout << "Operación cancelada." << std::endl;
        break;
    default:
        std::cout << "Opción no válida. Intente nuevamente." << std::endl;
}
return 0;
}
