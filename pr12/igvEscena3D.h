#ifndef __IGVESCENA3D
#define __IGVESCENA3D

#if defined(__APPLE__) && defined(__MACH__)
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#endif

#include "igvMallaTriangulos.h"

class igvEscena3D {
public:
    igvEscena3D();
    ~igvEscena3D();
    igvEscena3D(const igvEscena3D&) = delete;
    igvEscena3D& operator=(const igvEscena3D&) = delete;

    void visualizar(bool seleccion = false);
    bool get_ejes() const { return ejes; }
    void set_ejes(bool valor) { ejes = valor; }
    void seleccionarParte(int id);
    void arrastrarParte(int id, int dx, int dy);
    void actualizarAnimacion(float tiempo);
    void cambiarSombreado();
    void rotarModelo(int modelo, char eje, float delta);
    void trasladarModelo(int modelo, char eje, float delta);
    void escalarModelo(int modelo, float factor);
    int getParteSeleccionada() const { return parteSeleccionada; }
    int getModeloSeleccionado() const { return modeloSeleccionado; }

private:
    struct Transformacion { float tx=0, ty=0, tz=0, rx=0, ry=0, rz=0, escala=1; };
    bool ejes = true;
    igvMallaTriangulos* mallaPetalo = nullptr;
    Transformacion modelos[2]; // 0: malla paramétrica, 1: flor completa
    int modeloSeleccionado = 1;
    int parteSeleccionada = 0;
    float inclinacionX = 0, inclinacionZ = 0, giroCabeza = 0;
    float aperturaPetalos = 18, giroHojaIzquierda = 0, giroHojaDerecha = 0, alturaCabeza = 0;
    bool sombreadoSuave = true;

    void pintarEjes();
    void dibujarFlor(bool seleccion);
    void material(float r, float g, float b, bool seleccion, int id);
    void cilindro(float radioBase, float radioSuperior, float altura, int id, bool seleccion, int lados=24);
    void esfera(float radio, int id, bool seleccion, int slices=20, int stacks=16);
};

#endif
