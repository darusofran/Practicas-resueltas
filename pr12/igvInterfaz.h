#ifndef __IGVINTERFAZ
#define __IGVINTERFAZ

#include <string>
#include "igvEscena3D.h"
#include "igvCamara.h"

class igvInterfaz {
private:
    int ancho_ventana=900, alto_ventana=700;
    igvEscena3D escena;
    igvCamara camara, camaraAlzado, camaraPlanta, camaraPerfil;
    bool modoCamara=false, cuatroVistas=false, modoRaton=true;
    bool animacion=false, orbitaAutomatica=false;
    bool seleccionPendiente=false, botonRetenido=false;
    int cursorX=0, cursorY=0, ultimaX=0, ultimaY=0;
    int menuId=0;
    static igvInterfaz* _instancia;
    igvInterfaz();
    void dibujarViewport(int x,int y,int w,int h,igvCamara& cam,bool seleccion);
    int vistaEn(int x,int y,int& vx,int& vy,int& vw,int& vh,igvCamara*& cam);
    void renderizar(bool seleccion);
public:
    static igvInterfaz& getInstancia();
    ~igvInterfaz()=default;
    static void keyboardFunc(unsigned char key,int x,int y);
    static void specialKeyFunc(int key,int x,int y);
    static void reshapeFunc(int w,int h);
    static void displayFunc();
    static void mouseFunc(int boton,int estado,int x,int y);
    static void motionFunc(int x,int y);
    static void idleFunc();
    static void menuFunc(int opcion);
    void configura_entorno(int argc,char** argv,int w,int h,int x,int y,std::string titulo);
    void inicializa_callbacks();
    void inicia_bucle_visualizacion();
};

#endif
