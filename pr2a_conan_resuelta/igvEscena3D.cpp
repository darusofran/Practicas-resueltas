#include <cstdlib>
#include <stdio.h>
#include <cmath>
#include <igvCilindro.h>

#include "igvEscena3D.h"
#include "igvMallaTriangulos.h"


// M�todos constructores

/**
 * Constructor por defecto
 */
igvEscena3D::igvEscena3D()
{
    // TODO: Apartado B: Inserta el c�digo para crear un cilindro
    malla = new igvCilindro(1,1,40,5);
}

/**
 * Destructor
 */
igvEscena3D::~igvEscena3D()
{
    if (malla != nullptr)
    {
        delete malla;
        malla = nullptr;
    }
}


// M�todos p�blicos

/**
 * M�todo para pintar los ejes coordenados llamando a funciones de OpenGL
 */
void igvEscena3D::pintar_ejes()
{
    GLfloat rojo[] = {1, 0, 0, 1.0, 1.0};
    GLfloat verde[] = {0, 1, 0, 1.0, 1.0};
    GLfloat azul[] = {0, 0, 1, 1.0, 1.0};

    glMaterialfv(GL_FRONT, GL_EMISSION, rojo);
    glBegin(GL_LINES);
    glVertex3f(1000, 0, 0);
    glVertex3f(-1000, 0, 0);
    glEnd();

    glMaterialfv(GL_FRONT, GL_EMISSION, verde);
    glBegin(GL_LINES);
    glVertex3f(0, 1000, 0);
    glVertex3f(0, -1000, 0);
    glEnd();

    glMaterialfv(GL_FRONT, GL_EMISSION, azul);
    glBegin(GL_LINES);
    glVertex3f(0, 0, 1000);
    glVertex3f(0, 0, -1000);
    glEnd();
}

/**
 * M�todo con las llamadas OpenGL para visualizar la escena
 */
void igvEscena3D::visualizar(void)
{
    GLfloat color_malla[] = {0, 0.25, 0, 1.0};
    // crear luces
    GLfloat luz0[4] = {5, 5, 5, 1}; // luz puntual para visualizar el cubo

    glLightfv(GL_LIGHT0, GL_POSITION, luz0); // la luz se coloca aquí si permanece fija y no se mueve con la escena
    glEnable(GL_LIGHT0);

    // se pintan los ejes
    if (ejes) pintar_ejes();

    // crear el modelo
    glPushMatrix(); // guarda la matriz de modelado

    glRotatef(rotX, 1, 0, 0);
    glRotatef(rotY, 0, 1, 0);
    glRotatef(rotZ, 0, 0, 1);


    //glLightfv(GL_LIGHT0,GL_POSITION,luz0); // la luz se coloca aquí si se mueve junto con la escena
    glMaterialfv(GL_FRONT, GL_EMISSION, color_malla);


    // Apartado B: la siguiente llamada hay que sustituirla por la llamada al método visualizar de la malla
    /*GLUquadric* cyl = gluNewQuadric();
    gluCylinder(cyl, 1, 1, 1, 20, 5);
    gluDeleteQuadric(cyl);
    cyl=nullptr;*/
    malla->visualizar();


    glPopMatrix(); // restaura la matriz de modelado
}

/**
 * M�todo para consultar si hay que dibujar los ejes o no
 * @retval true Si hay que dibujar los ejes
 * @retval false Si no hay que dibujar los ejes
 */
bool igvEscena3D::get_ejes()
{
    return ejes;
}

/**
 * M�todo para activar o desactivar el dibujado de los _ejes
 * @param _ejes Indica si hay que dibujar los ejes (true) o no (false)
 * @post El estado del objeto cambia en lo que respecta al dibujado de ejes,
 *       de acuerdo al valor pasado como par�metro
 */
void igvEscena3D::set_ejes(bool _ejes)
{
    ejes = _ejes;
}
