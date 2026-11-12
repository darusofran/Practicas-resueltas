#include <math.h>

#include "igvCamara.h"

// M�todos constructores

/**
 * Constructor parametrizado
 * @param _tipo Tipo de c�mara (IGV_PARALELA, IGV_FRUSTUM o IGV_PERSPECTIVA)
 * @param _P0 Posici�n de la c�mara (punto de visi�n)
 * @param _r Punto al que mira la c�mara (punto de referencia)
 * @param _V Vector que indica la vertical
 * @pre Se asume que todos los par�metros tienen valores v�lidos
 * @post Los atributos de la nueva c�mara ser�n iguales a los par�metros que se
 *       le pasan
 */
igvCamara::igvCamara ( tipoCamara _tipo, igvPunto3D _P0, igvPunto3D _r
   , igvPunto3D _V ): P0 ( _P0 ), r ( _r ), V ( _V )
                      , tipo ( _tipo )
{ }

// M�todos p�blicos
/**
 * Define la posici�n de la c�mara
 * @param _P0 Posici�n de la c�mara (punto de visi�n)
 * @param _r Punto al que mira la c�mara (punto de referencia)
 * @param _V Vector que indica la vertical
 * @pre Se asume que todos los par�metros tienen valores v�lidos
 * @post Los atributos de la c�mara cambian a los valores pasados como par�metro
 */
void igvCamara::set ( igvPunto3D _P0, igvPunto3D _r, igvPunto3D _V )
{  P0 = _P0;
   r  = _r;
   V  = _V;
}

/**
 * Define una c�mara de tipo paralela o frustum
 * @param _tipo Tipo de la c�mara (IGV_PARALELA o IGV_FRUSTUM)
 * @param _P0 Posici�n de la c�mara
 * @param _r Punto al que mira la c�mara
 * @param _V Vector que indica la vertical
 * @param _xwmin Coordenada X m�nima del frustum
 * @param _xwmax Coordenada X m�xima del frustum
 * @param _ywmin Coordenada Y m�nima del frustum
 * @param _ywmax Coordenada Y m�xima del frustum
 * @param _znear Distancia de la c�mara al plano Z near
 * @param _zfar Distancia de la c�mara al plano Z far
 * @pre Se asume que todos los par�metros tienen valores v�lidos
 * @post Los atributos de la c�mara cambian a los valores pasados como par�metro
 */
void igvCamara::set ( tipoCamara _tipo, igvPunto3D _P0, igvPunto3D _r
                      , igvPunto3D _V, double _xwmin, double _xwmax, double _ywmin
                      , double _ywmax, double _znear, double _zfar )
{  tipo = _tipo;

   P0 = _P0;
   r = _r;
   V = _V;

   xwmin = _xwmin;
   xwmax = _xwmax;
   ywmin = _ywmin;
   ywmax = _ywmax;
   znear = _znear;
   zfar = _zfar;
}

/**
 * Define una c�mara de tipo perspectiva
 * @param _tipo Tipo de la c�mara (IGV_PERSPECTIVA)
 * @param _P0 Posici�n de la c�mara
 * @param _r Punto al que mira la c�mara
 * @param _V Vector que indica la vertical
 * @param _angulo �ngulo de apertura
 * @param _raspecto Raz�n de aspecto
 * @param _znear Distancia de la c�mara al plano Z near
 * @param _zfar Distancia de la c�mara al plano Z far
 * @pre Se asume que todos los par�metros tienen valores v�lidos
 * @post Los atributos de la c�mara cambian a los valores que se pasan como
 *       par�metros
 */
void igvCamara::set ( tipoCamara _tipo, igvPunto3D _P0, igvPunto3D _r
                      , igvPunto3D _V, double _angulo, double _raspecto
                      , double _znear, double _zfar )
{  tipo = _tipo;

   P0 = _P0;
   r = _r;
   V = _V;

   angulo = _angulo;
   raspecto = _raspecto;
   znear = _znear;
   zfar = _zfar;
}

/**
 * Aplica a los objetos de la escena la transformaci�n de visi�n y la
 * transformaci�n de proyecci�n asociadas a los par�metros de la c�mara
 */
void igvCamara::aplicar ()
{  glMatrixMode ( GL_PROJECTION );
   glLoadIdentity ();

   if ( tipo == IGV_PARALELA )
   {
      glOrtho ( xwmin, xwmax, ywmin, ywmax, znear, zfar );
   }
   if ( tipo == IGV_FRUSTUM )
   {
      glFrustum ( xwmin, xwmax, ywmin, ywmax, znear, zfar );
   }
   if ( tipo == IGV_PERSPECTIVA )
   {
      gluPerspective ( angulo, raspecto, znear, zfar );
   }

   glMatrixMode ( GL_MODELVIEW );
   glLoadIdentity ();
   gluLookAt ( P0[X], P0[Y], P0[Z], r[X], r[Y], r[Z], V[X], V[Y], V[Z] );
}

// ÓRBITA

void igvCamara::orbitar(double grados)
{
    double rad = grados * M_PI / 180.0;

    double dx = P0[X] - r[X];
    double dz = P0[Z] - r[Z];

    double nuevoX =
            dx * cos(rad) - dz * sin(rad);

    double nuevoZ =
            dx * sin(rad) + dz * cos(rad);

    P0[X] = r[X] + nuevoX;
    P0[Z] = r[Z] + nuevoZ;
}

// CABECEO

void igvCamara::cabecear(double grados)
{
    double rad = grados * M_PI / 180.0;

    // Vector desde la cámara al punto de referencia
    double dx = r[X] - P0[X];
    double dy = r[Y] - P0[Y];
    double dz = r[Z] - P0[Z];

    // Eje X local de la cámara
    double rx = V[Y] * dz - V[Z] * dy;
    double ry = V[Z] * dx - V[X] * dz;
    double rz = V[X] * dy - V[Y] * dx;

    double longitud =
            sqrt(rx * rx + ry * ry + rz * rz);

    if (longitud < 0.000001)
        return;

    rx /= longitud;
    ry /= longitud;
    rz /= longitud;


    // Rotación de Rodrigues
    double cosA = cos(rad);
    double sinA = sin(rad);

    double nuevoX =
            dx * cosA +
            (ry * dz - rz * dy) * sinA +
            rx * (rx * dx + ry * dy + rz * dz) * (1 - cosA);

    double nuevoY =
            dy * cosA +
            (rz * dx - rx * dz) * sinA +
            ry * (rx * dx + ry * dy + rz * dz) * (1 - cosA);

    double nuevoZ =
            dz * cosA +
            (rx * dy - ry * dx) * sinA +
            rz * (rx * dx + ry * dy + rz * dz) * (1 - cosA);


    r[X] = P0[X] + nuevoX;
    r[Y] = P0[Y] + nuevoY;
    r[Z] = P0[Z] + nuevoZ;
}


// ROTACIÓN SOBRE EL EJE Y DE LA CÁMARA


void igvCamara::rotarY(double grados)
{
    double rad = grados * M_PI / 180.0;

    double dx = r[X] - P0[X];
    double dz = r[Z] - P0[Z];

    double nuevoX =
            dx * cos(rad) - dz * sin(rad);

    double nuevoZ =
            dx * sin(rad) + dz * cos(rad);

    r[X] = P0[X] + nuevoX;
    r[Z] = P0[Z] + nuevoZ;
}

void igvCamara::zoom(double factor)
{
    // Convertimos porcentaje a factor.
    // Por ejemplo:
    // 10  -> 1.10
    // -10 -> 0.90

    double f = 1.0 + factor / 100.0;


    if (tipo == IGV_PARALELA ||
        tipo == IGV_FRUSTUM)
    {
        xwmin *= f;
        xwmax *= f;

        ywmin *= f;
        ywmax *= f;
    }
    else
    {
        angulo *= f;

        // Evitamos valores absurdos
        if (angulo < 10)
            angulo = 10;

        if (angulo > 120)
            angulo = 120;
    }
}

// RECORTE DEL PLANO DELANTERO

void igvCamara::moverZnear(double cantidad)
{
    znear += cantidad;

    // Nunca puede superar al plano trasero
    if (znear < 0.1)
        znear = 0.1;

    if (znear >= zfar - 0.1)
        znear = zfar - 0.1;
}

// RECORTE DEL PLANO TRASERO

void igvCamara::moverZfar(double cantidad)
{
    zfar += cantidad;

    // Nunca puede estar antes del plano delantero
    if (zfar <= znear + 0.1)
        zfar = znear + 0.1;
}

tipoCamara igvCamara::getTipo() const {
    return tipo;
}

GLdouble igvCamara::getXwmin() const {
    return xwmin;
}

GLdouble igvCamara::getXwmax() const {
    return xwmax;
}

GLdouble igvCamara::getYwmin() const {
    return ywmin;
}

GLdouble igvCamara::getYwmax() const {
    return ywmax;
}

GLdouble igvCamara::getAngulo() const {
    return angulo;
}

GLdouble igvCamara::getRaspecto() const {
    return raspecto;
}

GLdouble igvCamara::getZnear() const {
    return znear;
}

GLdouble igvCamara::getZfar() const {
    return zfar;
}

const igvPunto3D &igvCamara::getP0() const {
    return P0;
}

const igvPunto3D &igvCamara::getR() const {
    return r;
}

const igvPunto3D &igvCamara::getV() const {
    return V;
}

void igvCamara::setZnear(GLdouble valor)
{
    znear = valor;

    if (znear < 0.1) {
        znear = 0.1;
    }

    if (znear >= zfar) {
        znear = zfar - 0.1;
    }
}

void igvCamara::setZfar(GLdouble valor)
{
    zfar = valor;

    if (zfar <= znear) {
        zfar = znear + 0.1;
    }

}