#ifndef __IGVINTERFAZ
#define __IGVINTERFAZ

#if defined(__APPLE__) && defined(__MACH__)
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else

#include <GL/glut.h>

#endif   // defined(__APPLE__) && defined(__MACH__)

#include <string>
#include <iostream>
#include "igvEscena3D.h"
#include "igvCamara.h"

/**
 * Modos de funcionamiento de la interfaz
 */
enum modoInterfaz
{  IGV_VISUALIZAR ///< En la ventana se va a visualizar de manera normal la escena
   , IGV_SELECCIONAR /**< Se ha hecho clic en la ventana de visualización, y la
                      *   escena se debe visualizar en modo selección
                      */
};

/**
 * Los objetos de esta clase encapsulan la interfaz y el estado de la aplicación
 */
class igvInterfaz
{  private:
      // Atributos
      int ancho_ventana = 0; ///< Ancho de la ventana de visualización
      int alto_ventana = 0;  ///< Alto de la ventana de visualización

      igvEscena3D escena; ///< Escena que se visualiza en la ventana definida por igvInterfaz
      igvCamara camara; ///< Cámara que se utiliza para visualizar la escena

      // TODO: Apartado A: atributos para la selección mediante el ratón
      modoInterfaz modo = IGV_VISUALIZAR; ///< Modo de visualización de la escena
      int cursorX = 0  /**< Coordenada X. Píxel de la pantalla sobre el que
                        *   está situado el ratón, para pulsar o arrastrar
                        */
      , cursorY = 0; /**< Coordenada Y. Píxel de la pantalla sobre el que
                      *   está situado el ratón, para pulsar o arrastrar
                      */

      int objeto_seleccionado = -1; ///< Identificador del objeto seleccionado, -1 si no hay ninguno

      bool boton_retenido = false; ///< Indica si el botón está pulsado (true) o se ha soltado (false)

      // Aplicación del patrón de diseño Singleton
      static igvInterfaz *_instancia; ///< Dirección de memoria del objeto único de la clase
      /// Constructor por defecto
      igvInterfaz () = default;

   public:
      // Aplicación del patrón de diseño Singleton
      static igvInterfaz &getInstancia ();
      // Constructores por defecto y destructor

      /// Destructor
      ~igvInterfaz () = default;

      // Métodos estáticos
      // callbacks de eventos
      static void keyboardFunc ( unsigned char key, int x, int y ); // método para control de eventos del teclado
      static void reshapeFunc ( int w, int h ); // método que define la camara de vision y el viewport
      // se llama automáticamente cuando se cambia el tamaño de la ventana
      static void displayFunc (); // método para visualizar la escena

      // TODO: Apartado A y B: métodos para el control de la pulsación y el arrastre del ratón
      static void mouseFunc ( GLint boton, GLint estado, GLint x, GLint y ); // control de pulsacion del raton
      static void motionFunc ( GLint x, GLint y ); // control del desplazamiento del raton con boton pulsado

      // Métodos
      // crea el mundo que se visualiza en la ventana
      void crear_mundo ( void );

      // inicializa todos los parámetros para crear una ventana de visualización
      void configura_entorno ( int argc, char **argv // parámetros del main
                               , int _ancho_ventana, int _alto_ventana // ancho y alto de la ventana de visualización
                               , int _pos_X, int _pos_Y // posición inicial de la ventana de visualización
                               , std::string _titulo // título de la ventana de visualización
                             );
      void inicializa_callbacks (); // inicializa todos los callbacks
      void inicia_bucle_visualizacion (); // visualiza la escena y espera a eventos sobre la interfaz

      // métodos get_ y set_ de acceso a los atributos
      int get_ancho_ventana ();

      int get_alto_ventana ();

      void set_ancho_ventana ( int _ancho_ventana );

      void set_alto_ventana ( int _alto_ventana );
};

#endif   // __IGVINTERFAZ
