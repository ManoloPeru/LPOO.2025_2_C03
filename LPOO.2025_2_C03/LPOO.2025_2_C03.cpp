#include "pch.h"
#include <iostream>
#include <string>

using namespace System;
using namespace System::Collections::Generic;

// Clase Estacion con los atributos requeridos
public ref class Estacion {
private:
    int id;
    String^ nombre;
    String^ tipo;
    String^ zona;

public:
    // Constructor
    Estacion(int id, String^ nombre, String^ tipo, String^ zona) {
        this->id = id;
        this->nombre = nombre;
        this->tipo = tipo;
        this->zona = zona;
    }

    // Propiedades
    int getId() { return id; }
    String^ getNombre() { return nombre; }
    String^ getTipo() { return tipo; }
    String^ getZona() { return zona; }

    // Método para mostrar información de la estación
    String^ ToString() override {
        return String::Format("[{0}] {1} (Zona {2})", id, nombre, zona);
    }
};

// Clase principal del sistema de rutas
public ref class SistemaRutasRobots {
private:
    Dictionary<int, Estacion^>^ estaciones;
    Dictionary<int, List<int>^>^ grafo;
    int contadorId;

public:
    // Constructor
    SistemaRutasRobots() {
        estaciones = gcnew Dictionary<int, Estacion^>();
        grafo = gcnew Dictionary<int, List<int>^>();
        contadorId = 1;
    }

    // Método para registrar una nueva estación
    void RegistrarEstacion() {
        String^ nombre, ^ tipo, ^ zona;

        // Solicitar nombre
        do {
            Console::Write("Nombre de la estación: ");
            nombre = Console::ReadLine();
            if (!String::IsNullOrEmpty(nombre)) {
                break;
            }
            Console::WriteLine("Error: El nombre no puede estar vacío.");
        } while (true);

        // Solicitar tipo
        do {
            Console::Write("Tipo: ");
            tipo = Console::ReadLine();
            if (!String::IsNullOrEmpty(tipo)) {
                break;
            }
            Console::WriteLine("Error: El tipo no puede estar vacío.");
        } while (true);

        // Solicitar zona
        do {
            Console::Write("Zona: ");
            zona = Console::ReadLine();
            if (!String::IsNullOrEmpty(zona)) {
                break;
            }
            Console::WriteLine("Error: La zona no puede estar vacía.");
        } while (true);

        // Crear y agregar la estación
        Estacion^ nuevaEstacion = gcnew Estacion(contadorId, nombre, tipo, zona);
        estaciones->Add(contadorId, nuevaEstacion);
        grafo->Add(contadorId, gcnew List<int>()); // Inicializar lista de adyacencia

        Console::WriteLine(">> Estación registrada con ID: {0}", contadorId);
        contadorId++;
    }

    // Método para crear una ruta entre estaciones
    void CrearRuta() {
        if (estaciones->Count < 2) {
            Console::WriteLine("Error: Debe haber al menos 2 estaciones registradas.");
            return;
        }

        int idOrigen, idDestino;

        // Solicitar ID de origen con validación
        do {
            Console::Write("Ingrese ID de la estación origen: ");
            String^ input = Console::ReadLine();
            if (Int32::TryParse(input, idOrigen)) {
                if (estaciones->ContainsKey(idOrigen)) {
                    break;
                }
            }
            Console::WriteLine("Error: ID de origen no válido.");
        } while (true);

        // Solicitar ID de destino con validación
        do {
            Console::Write("Ingrese ID de la estación destino: ");
            String^ input = Console::ReadLine();
            if (Int32::TryParse(input, idDestino)) {
                if (estaciones->ContainsKey(idDestino)) {
                    if (idDestino != idOrigen) {
                        break;
                    }
                    else {
                        Console::WriteLine("Error: No se puede crear una ruta a la misma estación.");
                    }
                }
            }
            Console::WriteLine("Error: ID de destino no válido.");
        } while (true);

        // Verificar si la ruta ya existe
        List<int>^ rutasSalientes = grafo[idOrigen];
        if (rutasSalientes->Contains(idDestino)) {
            Console::WriteLine("Error: La ruta ya existe.");
            return;
        }

        // Crear la ruta
        rutasSalientes->Add(idDestino);
        Console::WriteLine(">> Ruta creada: {0} --> {1}",
            estaciones[idOrigen]->ToString(),
            estaciones[idDestino]->ToString());
    }

    // Método para mostrar rutas salientes desde una estación
    void MostrarRutasSalientes() {
        if (estaciones->Count == 0) {
            Console::WriteLine("Error: No hay estaciones registradas.");
            return;
        }

        int idEstacion;

        // Solicitar ID de estación con validación
        do {
            Console::Write("Ingrese ID de la estación: ");
            String^ input = Console::ReadLine();
            if (Int32::TryParse(input, idEstacion)) {
                if (estaciones->ContainsKey(idEstacion)) {
                    break;
                }
            }
            Console::WriteLine("Error: ID de estación no válido.");
        } while (true);

        // Obtener la estación y sus rutas salientes
        Estacion^ estacion = estaciones[idEstacion];
        List<int>^ destinos = grafo[idEstacion];

        Console::WriteLine(">> Estación {0} conecta con:", estacion->ToString());

        if (destinos->Count == 0) {
            Console::WriteLine("   - No tiene rutas salientes");
        }
        else {
            for each (int idDestino in destinos) {
                Estacion^ destino = estaciones[idDestino];
                Console::WriteLine("   - {0}", destino->ToString());
            }
        }
    }

    // Método para mostrar todas las estaciones registradas (útil para debugging)
    void MostrarEstaciones() {
        if (estaciones->Count == 0) {
            Console::WriteLine("No hay estaciones registradas.");
            return;
        }

        Console::WriteLine("=== ESTACIONES REGISTRADAS ===");
        for each (KeyValuePair<int, Estacion^> ^ par in estaciones) {
            Estacion^ estacion = par->Value;
            Console::WriteLine("{0} - Tipo: {1}",
                estacion->ToString(), estacion->getTipo());
        }
    }

    // Método para ejecutar el sistema
    void Ejecutar() {
        int opcion;

        Console::WriteLine("=== RED DE RUTAS DE ROBOTS MÓVILES ===");

        do {
            Console::WriteLine();
            Console::WriteLine("1. Registrar nueva estación");
            Console::WriteLine("2. Crear ruta entre estaciones");
            Console::WriteLine("3. Mostrar rutas salientes");
            Console::WriteLine("4. Mostrar todas las estaciones");
            Console::WriteLine("5. Salir");
            Console::Write("Seleccione una opción: ");

            String^ input = Console::ReadLine();
            if (Int32::TryParse(input, opcion)) {
                switch (opcion) {
                case 1:
                    RegistrarEstacion();
                    break;
                case 2:
                    CrearRuta();
                    break;
                case 3:
                    MostrarRutasSalientes();
                    break;
                case 4:
                    MostrarEstaciones();
                    break;
                case 5:
                    Console::WriteLine(">> Finalizando programa...");
                    return;
                default:
                    Console::WriteLine("Opción no válida. Por favor, seleccione 1-5.");
                    break;
                }
            }
            else {
                Console::WriteLine("Entrada no válida. Por favor, ingrese un número.");
            }
        } while (true);
    }
};

// Función principal
int main() {
    SistemaRutasRobots^ sistema = gcnew SistemaRutasRobots();
    sistema->Ejecutar();
    return 0;
}