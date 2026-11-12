#include <cstdlib>
#include "igvInterfaz.h"

int main(int argc, char** argv) {
    igvInterfaz::getInstancia().configura_entorno(argc, argv, 1000, 760, 100, 80,
        "IGV Practica 2 - Flor mecanica");
    igvInterfaz::getInstancia().inicializa_callbacks();
    igvInterfaz::getInstancia().inicia_bucle_visualizacion();
    return 0;
}
