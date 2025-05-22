#include <iostream>
#include <queue>
#include <stack>
#include <vector>
#include <string>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <map>

using namespace std;

// Clase para representar un producto
class Producto {
private:
    string nombre;
    double precio;

public:
    Producto(string _nombre, double _precio) : nombre(_nombre), precio(_precio) {}

    string getNombre() const {
        return nombre;
    }

    double getPrecio() const {
        return precio;
    }
};

// Clase para representar un pedido
class Pedido {
private:
    int id;
    string nombreCliente;
    vector<Producto> productos;
    double total;
    string fechaHora;
    bool urgente;

public:
    Pedido(int _id, string _nombreCliente, vector<Producto> _productos, bool _urgente = false)
        : id(_id), nombreCliente(_nombreCliente), productos(_productos), urgente(_urgente) {
        total = calcularTotal();
        fechaHora = obtenerFechaHoraActual();
    }

    // Constructor para cargar desde archivo
    Pedido(int _id, string _nombreCliente, vector<Producto> _productos, double _total, string _fechaHora, bool _urgente = false)
        : id(_id), nombreCliente(_nombreCliente), productos(_productos), total(_total), fechaHora(_fechaHora), urgente(_urgente) {
    }

    int getId() const {
        return id;
    }

    string getNombreCliente() const {
        return nombreCliente;
    }

    vector<Producto> getProductos() const {
        return productos;
    }

    double getTotal() const {
        return total;
    }

    string getFechaHora() const {
        return fechaHora;
    }

    bool esUrgente() const {
        return urgente;
    }

    void setUrgente(bool _urgente) {
        urgente = _urgente;
    }

    double calcularTotal() {
        double suma = 0.0;
        for (const Producto& p : productos) {
            suma += p.getPrecio();
        }
        return suma;
    }

    string obtenerFechaHoraActual() {
        time_t ahora = time(0);
        tm tiempo;

#ifdef _WIN32
        // para tomar la hora y fecha del mi computadora
        localtime_s(&tiempo, &ahora);

#endif

        stringstream ss;
        ss << put_time(&tiempo, "%d/%m/%Y %H:%M:%S");
        return ss.str();
    }

    string toString() const {
        stringstream ss;
        ss << "ID: " << id << " | Cliente: " << nombreCliente;
        if (urgente) {
            ss << " URGENTE";
        }
        ss << " | Fecha: " << fechaHora << " | Total: Q" << fixed << setprecision(2) << total;

        // Agregar productos al resumen
        ss << endl << "    Productos: ";
        for (size_t i = 0; i < productos.size(); i++) {
            ss << productos[i].getNombre();
            if (i < productos.size() - 1) {
                ss << ", ";
            }
        }

        return ss.str();
    }

    string detalleCompleto() const {
        stringstream ss;
        ss << "ID: " << id << " | Cliente: " << nombreCliente;
        if (urgente) {
            ss << " URGENTE";
        }
        ss << " | Fecha: " << fechaHora << endl;
        ss << "Productos:" << endl;

        for (const Producto& p : productos) {
            ss << "  - " << p.getNombre() << ": Q" << fixed << setprecision(2) << p.getPrecio() << endl;
        }

        ss << "Total a pagar: Q" << fixed << setprecision(2) << total;
        return ss.str();
    }
};

// Clase para gestionar los pedidos
class GestorPedidos {
private:
    queue<Pedido> pedidosPendientes;
    stack<Pedido> pedidosCompletados;
    map<string, double> catalogoProductos;

public:
    GestorPedidos() {
        // Inicializar catálogo de productos (precios en quetzales)
        catalogoProductos["Latte"] = 19.50;
        catalogoProductos["Capuchino de vainilla"] = 23.00;
        catalogoProductos["Omelet de jamon y queso"] = 27.50;
        catalogoProductos["Espresso"] = 15.00;
        catalogoProductos["Chocolate con leche de almendras"] = 25.25;
        catalogoProductos["Pan con chilerelleno"] = 15.00;
        catalogoProductos["Encanelados"] = 21.75;
        catalogoProductos["Pastel de tres leches"] = 35.50;
        catalogoProductos["Pastel de almendras"] = 29.75;
        catalogoProductos["Pan dulce relleno de cajeta"] = 11.50;
    }

    void mostrarCatalogo() {
        cout << "\n--- Menu de productos ---\n";
        int contador = 1;
        for (const auto& producto : catalogoProductos) {
            cout << contador << ". " << producto.first << " - Q" << fixed << setprecision(2) << producto.second << endl;
            contador++;
        }
    }

    vector<Producto> seleccionarProductos() {
        vector<Producto> productosSeleccionados;
        char continuar;

        do {
            mostrarCatalogo();

            int opcion;
            cout << "\nSeleccione un producto (1-" << catalogoProductos.size() << "): ";
            cin >> opcion;

            if (opcion < 1 || opcion > static_cast<int>(catalogoProductos.size())) {
                cout << "Opcion invalida. Intente de nuevo.\n";
                continue;
            }

            auto it = catalogoProductos.begin();
            advance(it, opcion - 1);

            productosSeleccionados.push_back(Producto(it->first, it->second));
            cout << "Producto anadido: " << it->first << " - Q" << fixed << setprecision(2) << it->second << endl;

            cout << "Desea agregar otro producto? (s/n): ";
            cin >> continuar;
        } while (continuar == 's' || continuar == 'S');

        return productosSeleccionados;
    }

    void agregarPedido() {
        cin.ignore();
        string nombreCliente;
        cout << "\nNombre del cliente: ";
        getline(cin, nombreCliente);

        // Permitir que el usuario especifique el ID
        int id;
        cout << "ID del pedido: ";
        cin >> id;

        vector<Producto> productos = seleccionarProductos();

        char opcionUrgente;
        cout << "Es un pedido urgente? (s/n): ";
        cin >> opcionUrgente;
        bool esUrgente = (opcionUrgente == 's' || opcionUrgente == 'S');

        Pedido nuevoPedido(id, nombreCliente, productos, esUrgente);

        if (esUrgente) {
            // Si es urgente, procesarlo inmediatamente
            cout << "\nPedido URGENTE registrado y procesado inmediatamente.\n";
            cout << "Detalle del pedido:\n" << nuevoPedido.detalleCompleto() << endl;
            pedidosCompletados.push(nuevoPedido);
        }
        else {
            pedidosPendientes.push(nuevoPedido);
            cout << "\nPedido registrado correctamente.\n";
            cout << "Detalle del pedido:\n" << nuevoPedido.detalleCompleto() << endl;
        }
    }

    void procesarPedido() {
        if (pedidosPendientes.empty()) {
            cout << "\nNo hay pedidos pendientes para procesar.\n";
            return;
        }

        Pedido pedido = pedidosPendientes.front();
        pedidosPendientes.pop();

        cout << "\nProcesando pedido:\n" << pedido.detalleCompleto() << endl;

        pedidosCompletados.push(pedido);
        cout << "Pedido completado y movido al historial.\n";
    }

    void mostrarPedidosPendientes() {
        if (pedidosPendientes.empty()) {
            cout << "\nNo hay pedidos pendientes.\n";
            return;
        }

        cout << "\n--- PEDIDOS PENDIENTES ---\n";

        // Crear una copia de la cola para no modificar la original
        queue<Pedido> copia = pedidosPendientes;
        int contador = 1;

        while (!copia.empty()) {
            cout << contador << ". " << copia.front().toString() << endl << endl;
            contador++;
            copia.pop();
        }
    }

    void mostrarPedidosCompletados() {
        if (pedidosCompletados.empty()) {
            cout << "\nNo hay pedidos completados en el historial.\n";
            return;
        }

        cout << "\n--- HISTORIAL DE PEDIDOS COMPLETADOS ---\n";

        // Crear una copia de la pila para no modificar la original
        stack<Pedido> copia = pedidosCompletados;
        vector<Pedido> pedidosOrdenados;

        // Invertir el orden para mostrar de más reciente a más antiguo
        while (!copia.empty()) {
            pedidosOrdenados.push_back(copia.top());
            copia.pop();
        }

        for (size_t i = 0; i < pedidosOrdenados.size(); i++) {
            cout << (i + 1) << ". " << pedidosOrdenados[i].toString() << endl << endl;
        }
    }

    void buscarPedidoPorId() {
        int idBuscado;
        cout << "\nIngrese el ID del pedido a buscar: ";
        cin >> idBuscado;

        // Buscar en pedidos completados
        stack<Pedido> copiaCompletados = pedidosCompletados;
        bool encontrado = false;

        while (!copiaCompletados.empty() && !encontrado) {
            Pedido pedido = copiaCompletados.top();
            if (pedido.getId() == idBuscado) {
                cout << "\nPedido encontrado (completado):\n" << pedido.detalleCompleto() << endl;
                encontrado = true;
            }
            copiaCompletados.pop();
        }

        if (!encontrado) {
            // Buscar en pedidos pendientes
            queue<Pedido> copiaPendientes = pedidosPendientes;

            while (!copiaPendientes.empty() && !encontrado) {
                Pedido pedido = copiaPendientes.front();
                if (pedido.getId() == idBuscado) {
                    cout << "\nPedido encontrado (pendiente):\n" << pedido.detalleCompleto() << endl;
                    encontrado = true;
                }
                copiaPendientes.pop();
            }

            if (!encontrado) {
                cout << "\nNo se encontro ningún pedido con el ID " << idBuscado << ".\n";
            }
        }
    }

    void generarReporteFinanciero() {
        if (pedidosCompletados.empty()) {
            cout << "\nNo hay pedidos completados para generar un reporte.\n";
            return;
        }

        double ingresoTotal = 0.0;
        int cantidadPedidos = 0;
        map<string, int> conteoProductos;

        // Crear una copia de la pila para no modificar la original
        stack<Pedido> copia = pedidosCompletados;

        while (!copia.empty()) {
            Pedido pedido = copia.top();
            ingresoTotal += pedido.getTotal();
            cantidadPedidos++;

            // Contar productos vendidos
            for (const Producto& p : pedido.getProductos()) {
                conteoProductos[p.getNombre()]++;
            }

            copia.pop();
        }

        cout << "\n--- REPORTE FINANCIERO ---\n";
        cout << "Cantidad de pedidos completados: " << cantidadPedidos << endl;
        cout << "Ingreso total: Q" << fixed << setprecision(2) << ingresoTotal << endl;
        cout << "Promedio por pedido: Q" << fixed << setprecision(2) << (ingresoTotal / cantidadPedidos) << endl;

        cout << "\nProductos vendidos:\n";
        for (const auto& item : conteoProductos) {
            cout << "  - " << item.first << ": " << item.second << " unidad(es)" << endl;
        }
    }

    void guardarPedidos() {
        ofstream archivoPendientes("pedidos_pendientes.txt");
        ofstream archivoCompletados("pedidos_completados.txt");

        if (!archivoPendientes.is_open() || !archivoCompletados.is_open()) {
            cout << "\nError al abrir los archivos para guardar los pedidos.\n";
            return;
        }

        // Guardar pedidos pendientes
        queue<Pedido> copiaPendientes = pedidosPendientes;
        while (!copiaPendientes.empty()) {
            Pedido p = copiaPendientes.front();
            archivoPendientes << p.getId() << "|" << p.getNombreCliente() << "|";

            // Guardar productos
            vector<Producto> productos = p.getProductos();
            archivoPendientes << productos.size() << "|";
            for (const Producto& prod : productos) {
                archivoPendientes << prod.getNombre() << "," << prod.getPrecio() << "|";
            }

            archivoPendientes << p.getTotal() << "|" << p.getFechaHora() << "|" << p.esUrgente() << endl;
            copiaPendientes.pop();
        }

        // Guardar pedidos completados
        stack<Pedido> copiaCompletados = pedidosCompletados;
        vector<Pedido> pedidosOrdenados;

        while (!copiaCompletados.empty()) {
            pedidosOrdenados.push_back(copiaCompletados.top());
            copiaCompletados.pop();
        }

        for (int i = pedidosOrdenados.size() - 1; i >= 0; i--) {
            Pedido p = pedidosOrdenados[i];
            archivoCompletados << p.getId() << "|" << p.getNombreCliente() << "|";

            // Guardar productos
            vector<Producto> productos = p.getProductos();
            archivoCompletados << productos.size() << "|";
            for (const Producto& prod : productos) {
                archivoCompletados << prod.getNombre() << "," << prod.getPrecio() << "|";
            }

            archivoCompletados << p.getTotal() << "|" << p.getFechaHora() << "|" << p.esUrgente() << endl;
        }

        archivoPendientes.close();
        archivoCompletados.close();

        cout << "\nPedidos guardados correctamente en archivos.\n";
    }

    void cargarPedidos() {
        // Limpiar las estructuras actuales
        while (!pedidosPendientes.empty()) pedidosPendientes.pop();
        while (!pedidosCompletados.empty()) pedidosCompletados.pop();

        ifstream archivoPendientes("pedidos_pendientes.txt");
        ifstream archivoCompletados("pedidos_completados.txt");

        if (!archivoPendientes.is_open() || !archivoCompletados.is_open()) {
            cout << "\nNo se encontraron archivos de pedidos para cargar o hubo un error al abrirlos.\n";
            return;
        }

        string linea;

        // Cargar pedidos pendientes
        while (getline(archivoPendientes, linea)) {
            stringstream ss(linea);
            string token;
            vector<string> datos;

            while (getline(ss, token, '|')) {
                datos.push_back(token);
            }

            if (datos.size() < 6) continue; // Formato inválido

            int id = stoi(datos[0]);
            string nombreCliente = datos[1];
            int numProductos = stoi(datos[2]);

            vector<Producto> productos;
            for (int i = 0; i < numProductos; i++) {
                string prodInfo = datos[3 + i];
                stringstream ssProd(prodInfo);
                string nombreProd, precioProd;

                getline(ssProd, nombreProd, ',');
                getline(ssProd, precioProd, ',');

                productos.push_back(Producto(nombreProd, stod(precioProd)));
            }

            double total = stod(datos[3 + numProductos]);
            string fechaHora = datos[4 + numProductos];
            bool urgente = (datos[5 + numProductos] == "1");

            Pedido p(id, nombreCliente, productos, total, fechaHora, urgente);
            pedidosPendientes.push(p);
        }

        // Cargar pedidos completados (en orden inverso para mantener la estructura de pila)
        vector<Pedido> pedidosTmp;

        while (getline(archivoCompletados, linea)) {
            stringstream ss(linea);
            string token;
            vector<string> datos;

            while (getline(ss, token, '|')) {
                datos.push_back(token);
            }

            if (datos.size() < 6) continue; // Formato inválido

            int id = stoi(datos[0]);
            string nombreCliente = datos[1];
            int numProductos = stoi(datos[2]);

            vector<Producto> productos;
            for (int i = 0; i < numProductos; i++) {
                string prodInfo = datos[3 + i];
                stringstream ssProd(prodInfo);
                string nombreProd, precioProd;

                getline(ssProd, nombreProd, ',');
                getline(ssProd, precioProd, ',');

                productos.push_back(Producto(nombreProd, stod(precioProd)));
            }

            double total = stod(datos[3 + numProductos]);
            string fechaHora = datos[4 + numProductos];
            bool urgente = (datos[5 + numProductos] == "1");

            Pedido p(id, nombreCliente, productos, total, fechaHora, urgente);
            pedidosTmp.push_back(p);
        }

        // Cargar en la pila en orden inverso
        for (int i = pedidosTmp.size() - 1; i >= 0; i--) {
            pedidosCompletados.push(pedidosTmp[i]);
        }

        archivoPendientes.close();
        archivoCompletados.close();

        cout << "\nPedidos cargados correctamente desde archivos.\n";
    }

    void verPedidosGuardados() {
        ifstream archivoPendientes("pedidos_pendientes.txt");
        ifstream archivoCompletados("pedidos_completados.txt");

        if (!archivoPendientes.is_open() || !archivoCompletados.is_open()) {
            cout << "\nNo se encontraron archivos de pedidos para visualizar o hubo un error al abrirlos.\n";
            return;
        }

        cout << "\n--- PEDIDOS GUARDADOS EN ARCHIVOS ---\n";

        // Mostrar pedidos pendientes
        cout << "\nPEDIDOS PENDIENTES:\n";
        string linea;
        int contador = 1;

        while (getline(archivoPendientes, linea)) {
            stringstream ss(linea);
            string token;
            vector<string> datos;

            while (getline(ss, token, '|')) {
                datos.push_back(token);
            }

            if (datos.size() < 6) continue; // Formato inválido

            int id = stoi(datos[0]);
            string nombreCliente = datos[1];
            int numProductos = stoi(datos[2]);

            vector<Producto> productos;
            for (int i = 0; i < numProductos; i++) {
                string prodInfo = datos[3 + i];
                stringstream ssProd(prodInfo);
                string nombreProd, precioProd;

                getline(ssProd, nombreProd, ',');
                getline(ssProd, precioProd, ',');

                productos.push_back(Producto(nombreProd, stod(precioProd)));
            }

            double total = stod(datos[3 + numProductos]);
            string fechaHora = datos[4 + numProductos];
            bool urgente = (datos[5 + numProductos] == "1");

            Pedido p(id, nombreCliente, productos, total, fechaHora, urgente);
            cout << contador << ". " << p.toString() << endl << endl;
            contador++;
        }

        if (contador == 1) {
            cout << "No hay pedidos pendientes guardados.\n";
        }

        // Mostrar pedidos completados
        cout << "\nPEDIDOS COMPLETADOS:\n";
        contador = 1;

        while (getline(archivoCompletados, linea)) {
            stringstream ss(linea);
            string token;
            vector<string> datos;

            while (getline(ss, token, '|')) {
                datos.push_back(token);
            }

            if (datos.size() < 6) continue; // Formato inválido

            int id = stoi(datos[0]);
            string nombreCliente = datos[1];
            int numProductos = stoi(datos[2]);

            vector<Producto> productos;
            for (int i = 0; i < numProductos; i++) {
                string prodInfo = datos[3 + i];
                stringstream ssProd(prodInfo);
                string nombreProd, precioProd;

                getline(ssProd, nombreProd, ',');
                getline(ssProd, precioProd, ',');

                productos.push_back(Producto(nombreProd, stod(precioProd)));
            }

            double total = stod(datos[3 + numProductos]);
            string fechaHora = datos[4 + numProductos];
            bool urgente = (datos[5 + numProductos] == "1");

            Pedido p(id, nombreCliente, productos, total, fechaHora, urgente);
            cout << contador << ". " << p.toString() << endl << endl;
            contador++;
        }

        if (contador == 1) {
            cout << "No hay pedidos completados guardados.\n";
        }

        archivoPendientes.close();
        archivoCompletados.close();
    }

    // Nueva función para eliminar pedidos guardados en archivos
    void eliminarPedido() {
        int opcion;
        cout << "\nQue tipo de pedido desea eliminar?" << endl;
        cout << "1. Pedido pendiente" << endl;
        cout << "2. Pedido completado" << endl;
        cout << "Ingrese opcion: ";
        cin >> opcion;

        if (opcion != 1 && opcion != 2) {
            cout << "\nOpcion invalida. Regresando al menu principal." << endl;
            return;
        }

        string tipoArchivo = (opcion == 1) ? "pendientes" : "completados";
        string nombreArchivo = "pedidos_" + tipoArchivo + ".txt";

        // Leer archivo a memoria
        ifstream archivoEntrada(nombreArchivo);
        if (!archivoEntrada.is_open()) {
            cout << "\nNo se pudo abrir el archivo de pedidos " << tipoArchivo << "." << endl;
            return;
        }

        // Leer todas las líneas del archivo
        vector<string> lineas;
        string linea;
        vector<Pedido> pedidosLeidos;

        while (getline(archivoEntrada, linea)) {
            // Parsear la línea para obtener ID y mostrar al usuario
            stringstream ss(linea);
            string token;
            vector<string> datos;

            while (getline(ss, token, '|')) {
                datos.push_back(token);
            }

            if (datos.size() < 6) continue; // Formato inválido

            int id = stoi(datos[0]);
            string nombreCliente = datos[1];
            int numProductos = stoi(datos[2]);

            vector<Producto> productos;
            for (int i = 0; i < numProductos; i++) {
                string prodInfo = datos[3 + i];
                stringstream ssProd(prodInfo);
                string nombreProd, precioProd;

                getline(ssProd, nombreProd, ',');
                getline(ssProd, precioProd, ',');

                productos.push_back(Producto(nombreProd, stod(precioProd)));
            }

            double total = stod(datos[3 + numProductos]);
            string fechaHora = datos[4 + numProductos];
            bool urgente = (datos[5 + numProductos] == "1");

            Pedido p(id, nombreCliente, productos, total, fechaHora, urgente);
            pedidosLeidos.push_back(p);
            lineas.push_back(linea);
        }

        archivoEntrada.close();

        if (pedidosLeidos.empty()) {
            cout << "\nNo hay pedidos " << tipoArchivo << " para eliminar." << endl;
            return;
        }

        // Mostrar los pedidos al usuario
        cout << "\n--- Pedidos " << tipoArchivo << " disponibles para eliminar ---\n";
        for (size_t i = 0; i < pedidosLeidos.size(); i++) {
            cout << (i + 1) << ". " << pedidosLeidos[i].toString() << endl << endl;
        }

        // Solicitar al usuario que seleccione el pedido a eliminar
        int seleccion;
        cout << "Seleccione el numero del pedido a eliminar (1-" << pedidosLeidos.size() << "): ";
        cin >> seleccion;

        if (seleccion < 1 || seleccion > static_cast<int>(pedidosLeidos.size())) {
            cout << "\nSelección invalida. Operacion cancelada." << endl;
            return;
        }

        // Eliminar el pedido seleccionado
        lineas.erase(lineas.begin() + seleccion - 1);

        // Guardar los cambios en el archivo
        ofstream archivoSalida(nombreArchivo);
        if (!archivoSalida.is_open()) {
            cout << "\nError al abrir el archivo para guardar los cambios." << endl;
            return;
        }

        for (const string& l : lineas) {
            archivoSalida << l << endl;
        }

        archivoSalida.close();

        cout << "\nPedido eliminado correctamente del archivo de pedidos " << tipoArchivo << "." << endl;

        // Actualizar las estructuras en memoria si están cargadas
        cargarPedidos();
    }
};

// Función principal
int main() {
    GestorPedidos gestor;
    int opcion;

    cout << "\n=== Cafeteria el buen sabor ===\n";

    do {
        cout << "\nMenu Principal:\n";
        cout << "1. Agregar nuevo pedido\n";
        cout << "2. Procesar pedido pendiente \n";
        cout << "3. Mostrar pedidos pendientes\n";
        cout << "4. Mostrar historial de pedidos completados\n";
        cout << "5. Buscar pedido por ID\n";
        cout << "6. Generar reporte financiero\n";
        cout << "7. Guardar pedidos en archivos\n";
        cout << "8. Cargar pedidos desde archivos\n";
        cout << "9. Ver pedidos guardados en archivos\n";
        cout << "10. Eliminar pedido guardado en archivo\n";
        cout << "0. Salir\n";
        cout << "Ingrese una opcion: ";
        cin >> opcion;

        switch (opcion) {
        case 1:
            gestor.agregarPedido();
            break;

        case 2:
            gestor.procesarPedido();
            break;

        case 3:
            gestor.mostrarPedidosPendientes();
            break;

        case 4:
            gestor.mostrarPedidosCompletados();
            break;

        case 5:
            gestor.buscarPedidoPorId();
            break;

        case 6:
            gestor.generarReporteFinanciero();
            break;

        case 7:
            gestor.guardarPedidos();
            break;

        case 8:
            gestor.cargarPedidos();
            break;

        case 9:
            gestor.verPedidosGuardados();
            break;

        case 10:
            gestor.eliminarPedido();
            break;

        case 0:
            cout << "\n Gracias por su compra vuelve pronto a el buen sabor\n";
            break;

        default:
            cout << "\nOpcion invalida. Intente de nuevo.\n";
        }

    } while (opcion != 0);

    return 0;
}