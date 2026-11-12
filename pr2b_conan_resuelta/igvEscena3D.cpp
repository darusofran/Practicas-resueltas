#include <cstdlib>
#include <stdio.h>

#include "igvEscena3D.h"

// M�todos constructores

/**
 * Constructor por defecto
 */
igvEscena3D::igvEscena3D()
{
    // TODO: Apartado C: inicializar los atributos para el control de los grados de libertad del modelo
    x = 0;
    y = 0;
    z = 0;
    z2 = 0;
}

/**
 * Destructor
 */
igvEscena3D::~igvEscena3D()
{
}

/**
 * M�todo para pintar los ejes coordenados llamando a funciones de OpenGL
 */
void igvEscena3D::pintar_ejes()
{
    GLfloat rojo[] = {1, 0, 0, 1.0};
    GLfloat verde[] = {0, 1, 0, 1.0};
    GLfloat azul[] = {0, 0, 1, 1.0};

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

void igvEscena3D::pintarCubo()
{
    GLfloat color_cubo[] = {0, 0, 0.5};

    glMaterialfv(GL_FRONT, GL_EMISSION, color_cubo);

    glBegin(GL_QUADS);
    glVertex3f(-0.75f, -0.75f, 0.75f);
    glVertex3f(0.75f, -0.75f, 0.75f);
    glVertex3f(0.75f, 0.75f, 0.75f);
    glVertex3f(-0.75f, 0.75f, 0.75f);
    glEnd();

    glBegin(GL_QUADS);
    glVertex3f(0.75f, -0.75f, -0.75f);
    glVertex3f(-0.75f, -0.75f, -0.75f);
    glVertex3f(-0.75f, 0.75f, -0.75f);
    glVertex3f(0.75f, 0.75f, -0.75f);
    glEnd();

    glBegin(GL_QUADS);
    glVertex3f(-0.75f, -0.75f, -0.75f);
    glVertex3f(-0.75f, -0.75f, 0.75f);
    glVertex3f(-0.75f, 0.75f, 0.75f);
    glVertex3f(-0.75f, 0.75f, -0.75f);
    glEnd();

    glBegin(GL_QUADS);
    glVertex3f(0.75f, -0.75f, 0.75f);
    glVertex3f(0.75f, -0.75f, -0.75f);
    glVertex3f(0.75f, 0.75f, -0.75f);
    glVertex3f(0.75f, 0.75f, 0.75f);
    glEnd();

    glBegin(GL_QUADS);
    glVertex3f(-0.75f, 0.75f, 0.75f);
    glVertex3f(0.75f, 0.75f, 0.75f);
    glVertex3f(0.75f, 0.75f, -0.75f);
    glVertex3f(-0.75f, 0.75f, -0.75f);
    glEnd();

    glBegin(GL_QUADS);
    glVertex3f(0.75f, -0.75f, -0.75f);
    glVertex3f(0.75f, -0.75f, 0.75f);
    glVertex3f(-0.75f, -0.75f, 0.75f);
    glVertex3f(-0.75f, -0.75f, -0.75f);
    glEnd();
}

void igvEscena3D::pintar_tubo(float largo)
{
    GLUquadricObj* tubo;
    GLfloat color_tubo[] = {0, 0, 0.5};

    glMaterialfv(GL_FRONT, GL_EMISSION, color_tubo);


    tubo = gluNewQuadric();
    gluQuadricDrawStyle(tubo, GLU_FILL);

    glPushMatrix();
    glScalef(1, 1, 4);
    glTranslatef(0, 0, -0.5);
    gluCylinder(tubo, 0.25, 0.25, largo, 20, 20);
    glPopMatrix();

    gluDeleteQuadric(tubo);
}

void igvEscena3D::pintar_tubo2(float largo)
{
    GLUquadricObj* tubo;
    GLfloat color_tubo[] = {0, 0, 0.5};


    glMaterialfv(GL_FRONT, GL_EMISSION, color_tubo);


    tubo = gluNewQuadric();
    gluQuadricDrawStyle(tubo, GLU_FILL);

    glPushMatrix();
    glScalef(1, 1, 4);
    glTranslatef(0, 0, -0.5);
    gluCylinder(tubo, 0.25, 0.25, largo, 20, 20);
    glPopMatrix();

    gluDeleteQuadric(tubo);
}

void igvEscena3D::pintar_tubo3(float largo)
{
    GLUquadricObj* tubo;
    GLfloat color_tubo[] = {0, 0, 0.5};

    glMaterialfv(GL_FRONT, GL_EMISSION, color_tubo);

    tubo = gluNewQuadric();
    gluQuadricDrawStyle(tubo, GLU_FILL);

    glPushMatrix();
    glTranslatef(0, 0, -0.5);
    gluCylinder(tubo, 0.25, 0.25, largo, 20, 20);
    glPopMatrix();

    gluDeleteQuadric(tubo);
}


/**
 * M�todo con las llamadas OpenGL para visualizar la escena
 */
void igvEscena3D::visualizar()
{
    // crear luces
    GLfloat luz0[4] = {5.0, 5.0, 5.0, 1}; // luz puntual
    glLightfv(GL_LIGHT0, GL_POSITION, luz0); // la luz se coloca aqu� si permanece fija y no se mueve con la escena
    glEnable(GL_LIGHT0);

    // crear el modelo
    glPushMatrix(); // guarda la matriz de modelado

    // se pintan los ejes
    if (ejes)
    {
        pintar_ejes();
    }

    //glLightfv(GL_LIGHT0,GL_POSITION,luz0); // la luz se coloca aqu� si se mueve junto con la escena (tambi�n habr�a que desactivar la de arriba).


    // TODO: Apartado B: aqu� hay que a�adir la visualizaci�n del �rbol del modelo utilizando la pila de matrices de OpenGL
    //       se recomienda crear una m�todo auxiliar que encapsule el c�digo para la visualizaci�n
    //       del modelo, dejando aqu� s�lo la llamada a ese m�todo, as� como distintas funciones una para cada
    //       parte del modelo.

    float largo = 1;

    //Base
    glRotatef(get_rotarBaseY(), 0, 1, 0);
    pintarCubo();

    //Cuerpo inferior
    glRotatef(get_rotarCuerpoX(), 1, 0, 0);
    glTranslatef(0, 1 * 2, 0);
    glRotatef(-90, 1, 0, 0);
    pintar_tubo(largo);

    //Cuerpo superior
    glRotatef(get_rotarCuerposupZ(), 0, 0, 1);
    glTranslatef(0, 0, largo * 2);
    glRotatef(90, 0, 1, 0);
    pintar_tubo2(largo);

    //Brazo izquierdo
    glRotatef(get_rotarbrazosZ(), 0, 0, 1);
    glPushMatrix();
    glTranslatef(largo / 2, 0, largo * 2);
    glRotatef(-90, 0, 1, 0);
    pintar_tubo3(largo);
    glPopMatrix();

    //Brazo derecho
    glPushMatrix();
    glTranslatef(largo / 2, 0, -largo * 2);
    glRotatef(-90, 0, 1, 0);
    pintar_tubo3(largo);
    glPopMatrix();

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
