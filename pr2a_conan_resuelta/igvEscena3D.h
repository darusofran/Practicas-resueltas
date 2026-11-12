#ifndef __IGVESCENA3D
#define __IGVESCENA3D

#if defined(__APPLE__) && defined(__MACH__)
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else

#include <GL/glut.h>

#endif // defined(__APPLE__) && defined(__MACH__)

#include "igvMallaTriangulos.h"

/**
 * Los objetos de esta clase representan escenas 3D para su visualizaci�n
 */
class igvEscena3D
{
private:
    // Atributos
    bool ejes = true; ///< Indica si hay que dibujar los _ejes coordenados o no

    // TODO: Apartado A: A�adir aqu� los atributos con los �ngulos de rotaci�n en X, Y y Z.
    double rotX=0, rotY=0, rotZ=0;
    igvMallaTriangulos* malla = nullptr; ///< Malla de tri�ngulos asociada a la escena
public:
        // Constructores por defecto y destructor
    igvEscena3D();
    ~igvEscena3D();

    // M�todos
    // m�todo con las llamadas OpenGL para visualizar la escena
    void visualizar();

    bool get_ejes();

    void set_ejes(bool _ejes);

    igvMallaTriangulos* getMalla() const
    {
        return malla;
    }

    // Apartado A: métodos para incrementar los ángulos
    void incrX() {
        rotX += 10;
    }

    void incrY() {
        rotY += 10;
    }

    void incrZ() {
        rotZ += 10;
    }

    void decrX() {
        rotX -= 10;
    }

    void decrY() {
        rotY -= 10;
    }

    void decrZ() {
        rotZ -= 10;
    }

private:
    void pintar_ejes();
};

#endif   // __IGVESCENA3D
