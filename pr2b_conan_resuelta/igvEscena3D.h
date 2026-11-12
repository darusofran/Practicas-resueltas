#ifndef __IGVESCENA3D
#define __IGVESCENA3D

#if defined(__APPLE__) && defined(__MACH__)
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else

#include <GL/glut.h>

#endif // defined(__APPLE__) && defined(__MACH__)

/**
 * Partes del modelo
 */
enum parte
{
    basex ///< Identifica la base del modelo
    , cuerpoinferior ///< Identifica el cuerpo inferior del modelo
    , cuerposuperior ///< Identifica el cuerpo superior del modelo
    , brazo ///< Identifica el brazo del modelo
};

/**
 * Los objetos de esta clase representan escenas 3D para su visualizaci�n
 */
class igvEscena3D
{
private:
    // Atributos
    // TODO: Apartado C: a�adir qu� los atributos para el control de los grados de libertad del modelo
    float x, y, z, z2;

    // Otros atributos
    bool ejes = true; ///< Indica si hay que dibujar los ejes coordenados o no

public:
    // Constructores por defecto y destructor
    igvEscena3D();
    ~igvEscena3D();

    // m�todo con las llamadas OpenGL para visualizar la escena
    void visualizar();

    // TODO: Apartado B: M�todos para visualizar cada parte del modelo
    void pintarCubo();
    void pintar_tubo(float largo);
    void pintar_tubo2(float largo);
    void pintar_tubo3(float largo);

    // TODO: Apartado C: a�adir aqu� los m�todos para modificar los grados de libertad del modelo
    void rotarBaseY(float _xbase) { x = x + _xbase; }
    float get_rotarBaseY() { return x; };

    void rotarCuerpoX(float _xcuerpo) { y = y + _xcuerpo; }
    float get_rotarCuerpoX() { return y; };

    void rotarCuerposupZ(float _zcuerposup) { z = z + _zcuerposup; }
    float get_rotarCuerposupZ() { return z; };

    void rotarbrazosZ(float _zbrazos) { z2 = z2 + _zbrazos; }
    float get_rotarbrazosZ() { return z2; };

    bool get_ejes();
    void set_ejes(bool _ejes);

private:
    void pintar_ejes();
};

#endif   // __IGVESCENA3D
