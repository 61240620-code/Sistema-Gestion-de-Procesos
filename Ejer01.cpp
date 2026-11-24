
#include <iostream>
#include <fstream>
#include <string>
#include <limits>
using namespace std;

// LISTA ENLAZADA
struct Proceso {
    int id;
    string nombre;
    int prioridad;
    Proceso *sig;
};

// COLA DE PRIORIDAD
struct Cola {
    int id;
    int prioridad;
    Cola *sig;
};

// PILA DE MEMORIA
struct Pila {
    int id;
    int mb;
    Pila *sig;
};

Proceso *lista = NULL;
Cola *frente = NULL;
Pila *cima = NULL;

// Leer numeros enteros
int leer(string mensaje) {
    int valor;
    cout << mensaje;
    while (!(cin >> valor)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Ingrese un numero valido: ";
    }
    return valor;
}

// Buscar proceso
Proceso *buscar(int id) {
    Proceso *p = lista;
    while (p != NULL) {
        if (p->id == id)
            return p;
        p = p->sig;
    }
    return NULL;
}

// Registrar proceso
void insertar(int id, string nombre, int prioridad) {
    Proceso *p = new Proceso;
    p->id = id;
    p->nombre = nombre;
    p->prioridad = prioridad;
    p->sig = lista;
    lista = p;
    cout << "Proceso registrado correctamente\n";
}

// Buscar en la cola
bool buscarCola(int id) {
    Cola *p = frente;
    while (p != NULL) {
        if (p->id == id)
            return true;
        p = p->sig;
    }
    return false;
}

// Buscar en la pila
bool buscarPila(int id) {
    Pila *p = cima;
    while (p != NULL) {
        if (p->id == id)
            return true;
        p = p->sig;
    }
    return false;
}

// Insertar en la cola segun prioridad
void encolar(int id, int prioridad) {
    Cola *p = new Cola;
    p->id = id;
    p->prioridad = prioridad;
    p->sig = NULL;

    if (frente == NULL || prioridad < frente->prioridad) {
        p->sig = frente;
        frente = p;
    } else {
        Cola *aux = frente;

        while (aux->sig != NULL &&
               aux->sig->prioridad <= prioridad) {
            aux = aux->sig;
        }

        p->sig = aux->sig;
        aux->sig = p;
    }

    cout << "Proceso encolado correctamente\n";
}

// Ejecutar proceso de mayor prioridad
void desencolar() {
    if (frente == NULL) {
        cout << "Cola vacia\n";
        return;
    }

    Cola *p = frente;
    frente = p->sig;

    cout << "Proceso ejecutado: ID " << p->id << "\n";

    delete p;
}

// Asignar memoria: PUSH
void push(int id, int mb) {
    Pila *p = new Pila;
    p->id = id;
    p->mb = mb;
    p->sig = cima;
    cima = p;

    cout << "Memoria asignada correctamente\n";
}

// Liberar memoria: POP
void pop() {
    if (cima == NULL) {
        cout << "Pila vacia\n";
        return;
    }

    Pila *p = cima;
    cima = p->sig;

    cout << "Memoria liberada: " << p->mb << " MB\n";

    delete p;
}

// Eliminar proceso
void eliminar(int id) {
    if (buscarCola(id) || buscarPila(id)) {
        cout << "Primero retire el proceso de cola y pila\n";
        return;
    }

    Proceso *p = lista;
    Proceso *ant = NULL;

    while (p != NULL && p->id != id) {
        ant = p;
        p = p->sig;
    }

    if (p == NULL) {
        cout << "Proceso no encontrado\n";
        return;
    }

    if (ant != NULL)
        ant->sig = p->sig;
    else
        lista = p->sig;

    delete p;

    cout << "Proceso eliminado correctamente\n";
}

// Mostrar las tres estructuras
void mostrar() {
    cout << "\n===== LISTA DE PROCESOS =====\n";
    cout << "ID - Nombre - Prioridad\n";

    for (Proceso *p = lista; p != NULL; p = p->sig) {
        cout << p->id << " - "
             << p->nombre << " - "
             << p->prioridad << "\n";
    }

    cout << "\n===== COLA DE PRIORIDAD =====\n";
    cout << "ID - Prioridad\n";

    for (Cola *p = frente; p != NULL; p = p->sig) {
        cout << p->id << " - "
             << p->prioridad << "\n";
    }

    cout << "\n===== PILA DE MEMORIA =====\n";
    cout << "ID - Memoria\n";

    for (Pila *p = cima; p != NULL; p = p->sig) {
        cout << p->id << " - "
             << p->mb << " MB\n";
    }
}

// Liberar memoria dinamica
void limpiar() {
    while (lista != NULL) {
        Proceso *p = lista;
        lista = p->sig;
        delete p;
    }

    while (frente != NULL) {
        Cola *p = frente;
        frente = p->sig;
        delete p;
    }

    while (cima != NULL) {
        Pila *p = cima;
        cima = p->sig;
        delete p;
    }
}

// Guardar datos en archivo
void guardar() {
    ofstream f("procesos.txt");

    if (!f) {
        cout << "Error de archivo\n";
        return;
    }

    for (Proceso *p = lista; p != NULL; p = p->sig) {
        f << 'L' << ' ' << p->id << ' '
          << p->prioridad << ' '
          << p->nombre << '\n';
    }

    for (Cola *p = frente; p != NULL; p = p->sig) {
        f << 'C' << ' ' << p->id << ' '
          << p->prioridad << '\n';
    }

    for (Pila *p = cima; p != NULL; p = p->sig) {
        f << 'P' << ' ' << p->id << ' '
          << p->mb << '\n';
    }

    f.close();

    if (f)
        cout << "Datos guardados correctamente\n";
    else
        cout << "Error al guardar\n";
}

// Cargar datos conservando el orden
void cargar() {
    ifstream f("procesos.txt");

    if (!f) {
        cout << "No existe archivo guardado\n";
        return;
    }

    limpiar();

    char tipo;
    int id, valor;
    string nombre;

    Proceso **finL = &lista;
    Cola **finC = &frente;
    Pila **finP = &cima;

    while (f >> tipo >> id >> valor) {

        if (tipo == 'L') {
            if (!(f >> nombre))
                break;

            *finL = new Proceso;
            (*finL)->id = id;
            (*finL)->prioridad = valor;
            (*finL)->nombre = nombre;
            (*finL)->sig = NULL;
            finL = &((*finL)->sig);

        } else if (tipo == 'C') {
            *finC = new Cola;
            (*finC)->id = id;
            (*finC)->prioridad = valor;
            (*finC)->sig = NULL;
            finC = &((*finC)->sig);

        } else if (tipo == 'P') {
            *finP = new Pila;
            (*finP)->id = id;
            (*finP)->mb = valor;
            (*finP)->sig = NULL;
            finP = &((*finP)->sig);
        }
    }

    f.close();

    cout << "Carga finalizada\n";
}

// PROGRAMA PRINCIPAL
int main() {
    int op, id, pr, mb;
    string nombre;
    Proceso *p;

    do {
        cout << "\n====================================\n";
        cout << "   SISTEMA DE GESTION DE PROCESOS\n";
        cout << "====================================\n";
        cout << "1. Registrar proceso\n";
        cout << "2. Buscar proceso\n";
        cout << "3. Eliminar proceso\n";
        cout << "4. Modificar prioridad\n";
        cout << "5. Encolar proceso\n";
        cout << "6. Ejecutar proceso\n";
        cout << "7. Asignar memoria (Push)\n";
        cout << "8. Liberar memoria (Pop)\n";
        cout << "9. Mostrar estructuras\n";
        cout << "10. Guardar datos\n";
        cout << "11. Cargar datos\n";
        cout << "0. Salir\n";
        cout << "====================================\n";

        op = leer("Ingrese una opcion: ");

        switch (op) {

        case 1:
            id = leer("ID: ");

            if (id <= 0 || buscar(id)) {
                cout << "ID invalido o repetido\n";
                break;
            }

            cout << "Nombre (sin espacios): ";
            cin >> nombre;

            pr = leer("Prioridad (1-10): ");

            if (pr >= 1 && pr <= 10)
                insertar(id, nombre, pr);
            else
                cout << "Prioridad invalida\n";

            break;

        case 2:
            id = leer("ID: ");
            p = buscar(id);

            if (p != NULL) {
                cout << "Nombre: " << p->nombre << "\n";
                cout << "Prioridad: " << p->prioridad << "\n";
            } else {
                cout << "Proceso no encontrado\n";
            }

            cout << "En cola: "
                 << (buscarCola(id) ? "Si" : "No") << "\n";

            cout << "En pila: "
                 << (buscarPila(id) ? "Si" : "No") << "\n";

            break;

        case 3:
            id = leer("ID: ");
            eliminar(id);
            break;

        case 4:
            id = leer("ID: ");
            p = buscar(id);

            if (p == NULL || buscarCola(id)) {
                cout << "Proceso no disponible\n";
                break;
            }

            pr = leer("Nueva prioridad: ");

            if (pr >= 1 && pr <= 10) {
                p->prioridad = pr;
                cout << "Prioridad modificada\n";
            } else {
                cout << "Prioridad invalida\n";
            }

            break;

        case 5:
            id = leer("ID: ");
            p = buscar(id);

            if (p != NULL && !buscarCola(id))
                encolar(id, p->prioridad);
            else
                cout << "ID no disponible\n";

            break;

        case 6:
            desencolar();
            break;

        case 7:
            id = leer("ID: ");
            mb = leer("Cantidad de memoria (MB): ");

            if (buscar(id) && !buscarPila(id)
                && mb > 0 && mb <= 4096) {
                push(id, mb);
            } else {
                cout << "Datos invalidos\n";
            }

            break;

        case 8:
            pop();
            break;

        case 9:
            mostrar();
            break;

        case 10:
            guardar();
            break;

        case 11:
            cargar();
            break;

        case 0:
            cout << "Programa finalizado\n";
            break;

        default:
            cout << "Opcion invalida\n";
        }

    } while (op != 0);

    limpiar();

    return 0;
}
