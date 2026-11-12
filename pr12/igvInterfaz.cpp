#include "igvInterfaz.h"
#include <cstdlib>
#include <algorithm>

igvInterfaz* igvInterfaz::_instancia=nullptr;

igvInterfaz::igvInterfaz() {
    igvPunto3D centro(0,2,0), arriba(0,1,0);
    camara.set(IGV_PERSPECTIVA,igvPunto3D(8,7,11),centro,arriba,55,1,0.5,100);
    camaraAlzado.set(IGV_PARALELA,igvPunto3D(0,3,16),centro,arriba,-6,6,-4.5,4.5,0.1,100);
    camaraPlanta.set(IGV_PARALELA,igvPunto3D(0,20,0),centro,igvPunto3D(0,0,-1),-6,6,-4.5,4.5,0.1,100);
    camaraPerfil.set(IGV_PARALELA,igvPunto3D(16,3,0),centro,arriba,-6,6,-4.5,4.5,0.1,100);
}
igvInterfaz& igvInterfaz::getInstancia(){ if(!_instancia)_instancia=new igvInterfaz; return *_instancia; }

void igvInterfaz::configura_entorno(int argc,char** argv,int w,int h,int x,int y,std::string titulo){
    ancho_ventana=w; alto_ventana=h; glutInit(&argc,argv); glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH);
    glutInitWindowSize(w,h); glutInitWindowPosition(x,y); glutCreateWindow(titulo.c_str());
    glEnable(GL_DEPTH_TEST); glEnable(GL_LIGHTING); glEnable(GL_LIGHT0); glEnable(GL_NORMALIZE); glEnable(GL_DITHER);
    glClearColor(0.94f,0.96f,0.98f,1); GLfloat l[]={-4,10,8,1}; glLightfv(GL_LIGHT0,GL_POSITION,l);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,GL_TRUE); glShadeModel(GL_SMOOTH);
}
void igvInterfaz::inicializa_callbacks(){
    glutKeyboardFunc(keyboardFunc); glutSpecialFunc(specialKeyFunc); glutReshapeFunc(reshapeFunc); glutDisplayFunc(displayFunc);
    glutMouseFunc(mouseFunc); glutMotionFunc(motionFunc); glutIdleFunc(idleFunc);
    menuId=glutCreateMenu(menuFunc); glutAddMenuEntry("Modo teclado / ratón",1); glutAddMenuEntry("Animación automática",2);
    glutAddMenuEntry("Órbita automática de cámara",3); glutAddMenuEntry("Cambiar sombreado plano / Gouraud",4);
    glutAddMenuEntry("Vista única / cuatro vistas (V)",5); glutAttachMenu(GLUT_RIGHT_BUTTON);
}
void igvInterfaz::inicia_bucle_visualizacion(){ glutMainLoop(); }

void igvInterfaz::dibujarViewport(int x,int y,int w,int h,igvCamara& cam,bool seleccion){
    if(w<1||h<1)return; glViewport(x,y,w,h);
    if(cam.getTipo()==IGV_PERSPECTIVA)
        cam.set(IGV_PERSPECTIVA,cam.getP0(),cam.getR(),cam.getV(),cam.getAngulo(),static_cast<double>(w)/h,cam.getZnear(),cam.getZfar());
    cam.aplicar(); escena.visualizar(seleccion);
}
int igvInterfaz::vistaEn(int x,int y,int& vx,int& vy,int& vw,int& vh,igvCamara*& cam){
    int gy=alto_ventana-1-y;
    if(cuatroVistas){ vw=ancho_ventana/2; vh=alto_ventana/2;
        if(gy>=vh){ if(x<vw){vx=0;vy=vh;cam=&camaraAlzado;return 0;} vx=vw;vy=vh;cam=&camaraPlanta;return 1; }
        if(x<vw){vx=0;vy=0;cam=&camaraPerfil;return 2;} vx=vw;vy=0;cam=&camara;return 3;
    }
    int sw=std::max(1,ancho_ventana/4), sh=std::max(1,alto_ventana/4), sx=ancho_ventana-sw-10, sy=alto_ventana-sh-10;
    if(x>=sx && gy>=sy){vx=sx;vy=sy;vw=sw;vh=sh;cam=&camaraPlanta;return 1;}
    vx=0;vy=0;vw=ancho_ventana;vh=alto_ventana;cam=&camara;return 3;
}
void igvInterfaz::renderizar(bool seleccion){
    if(seleccion){ glDisable(GL_LIGHTING); glDisable(GL_DITHER); glDisable(GL_BLEND); glDisable(GL_TEXTURE_2D); glClearColor(0,0,0,1); glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        int vx,vy,vw,vh; igvCamara* cam=nullptr; vistaEn(cursorX,cursorY,vx,vy,vw,vh,cam); dibujarViewport(vx,vy,vw,vh,*cam,true);
        GLubyte p[3]={0,0,0}; glReadBuffer(GL_BACK); glReadPixels(cursorX,alto_ventana-1-cursorY,1,1,GL_RGB,GL_UNSIGNED_BYTE,p);
        int id=p[0]+(p[1]<<8)+(p[2]<<16); escena.seleccionarParte(id); seleccionPendiente=false;
        glEnable(GL_LIGHTING); glEnable(GL_DITHER); glClearColor(0.94f,0.96f,0.98f,1);
    }
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    if(cuatroVistas){ int w=ancho_ventana/2,h=alto_ventana/2;
        dibujarViewport(0,h,w,alto_ventana-h,camaraAlzado,false); dibujarViewport(w,h,ancho_ventana-w,alto_ventana-h,camaraPlanta,false);
        dibujarViewport(0,0,w,h,camaraPerfil,false); dibujarViewport(w,0,ancho_ventana-w,h,camara,false);
    }else{
        dibujarViewport(0,0,ancho_ventana,alto_ventana,camara,false);
        int w=std::max(1,ancho_ventana/4),h=std::max(1,alto_ventana/4);
        dibujarViewport(ancho_ventana-w-10,alto_ventana-h-10,w,h,camaraPlanta,false);
        glViewport(ancho_ventana-w-10,alto_ventana-h-10,w,h); glMatrixMode(GL_PROJECTION); glPushMatrix(); glLoadIdentity(); glOrtho(0,1,0,1,-1,1); glMatrixMode(GL_MODELVIEW); glPushMatrix(); glLoadIdentity();
        glDisable(GL_LIGHTING); glDisable(GL_DEPTH_TEST); glColor3f(0.08f,0.08f,0.08f); glBegin(GL_LINE_LOOP); glVertex2f(0,0);glVertex2f(1,0);glVertex2f(1,1);glVertex2f(0,1);glEnd();glEnable(GL_DEPTH_TEST);glEnable(GL_LIGHTING);
        glPopMatrix();glMatrixMode(GL_PROJECTION);glPopMatrix();glMatrixMode(GL_MODELVIEW);
    }
    if(cuatroVistas){
        glDisable(GL_LIGHTING); glDisable(GL_DEPTH_TEST);
        glMatrixMode(GL_PROJECTION); glPushMatrix(); glLoadIdentity(); glOrtho(0,ancho_ventana,0,alto_ventana,-1,1);
        glMatrixMode(GL_MODELVIEW); glPushMatrix(); glLoadIdentity(); glColor3f(0.12f,0.12f,0.12f);
        glBegin(GL_LINES); glVertex2i(ancho_ventana/2,0); glVertex2i(ancho_ventana/2,alto_ventana); glVertex2i(0,alto_ventana/2); glVertex2i(ancho_ventana,alto_ventana/2); glEnd();
        glPopMatrix(); glMatrixMode(GL_PROJECTION); glPopMatrix(); glMatrixMode(GL_MODELVIEW);
        glEnable(GL_DEPTH_TEST); glEnable(GL_LIGHTING);
    }
    glutSwapBuffers();
}
void igvInterfaz::displayFunc(){
    igvInterfaz& a=*_instancia;
    if(a.seleccionPendiente && a.modoRaton) a.renderizar(true);
    a.renderizar(false);
}
void igvInterfaz::reshapeFunc(int w,int h){ _instancia->ancho_ventana=std::max(1,w);_instancia->alto_ventana=std::max(1,h);glutPostRedisplay(); }

void igvInterfaz::keyboardFunc(unsigned char key,int,int){
    igvInterfaz& a=*_instancia; int parte=a.escena.getParteSeleccionada();
    switch(key){
        case 27: case 'q': case 'Q': std::exit(0);
        case 'e': case 'E': a.escena.set_ejes(!a.escena.get_ejes()); break;
        case 'v': case 'V': a.cuatroVistas=!a.cuatroVistas; break;
        case 'm': case 'M': a.modoRaton=!a.modoRaton; break;
        case 'a': case 'A': a.animacion=!a.animacion; break;
        case 'o': case 'O': a.orbitaAutomatica=!a.orbitaAutomatica; break;
        case 'g': case 'G': a.escena.cambiarSombreado(); break;
        case 'c': case 'C': a.modoCamara=!a.modoCamara; break;
        case '1': a.escena.seleccionarParte(2); break; // tallo
        case '2': a.escena.seleccionarParte(3); break; // hoja izquierda
        case '3': a.escena.seleccionarParte(4); break; // hoja derecha
        case '4': a.escena.seleccionarParte(5); break; // cabeza
        case '5': case '6': case '7': case '8': a.escena.seleccionarParte(6+(key-'5')); break; // pétalos
        case '0': a.escena.seleccionarParte(0); break;
        case '9': a.escena.seleccionarParte(5); break;
        case 'x': a.escena.rotarModelo(a.escena.getModeloSeleccionado(),'x',5); break;
        case 'X': a.escena.rotarModelo(a.escena.getModeloSeleccionado(),'x',-5); break;
        case 'y': a.escena.rotarModelo(a.escena.getModeloSeleccionado(),'y',5); break;
        case 'Y': a.escena.rotarModelo(a.escena.getModeloSeleccionado(),'y',-5); break;
        case 'z': a.escena.rotarModelo(a.escena.getModeloSeleccionado(),'z',5); break;
        case 'Z': a.escena.rotarModelo(a.escena.getModeloSeleccionado(),'z',-5); break;
        case 'u': a.escena.trasladarModelo(a.escena.getModeloSeleccionado(),'y',0.1f); break;
        case 'U': a.escena.trasladarModelo(a.escena.getModeloSeleccionado(),'y',-0.1f); break;
        case 's': a.escena.escalarModelo(a.escena.getModeloSeleccionado(),1.05f); break;
        case 'S': a.escena.escalarModelo(a.escena.getModeloSeleccionado(),0.95f); break;
        case '+': a.camara.zoom(-8); break; case '-': a.camara.zoom(8); break;
        case 'f': a.camara.moverZnear(-0.2); break; case 'F': a.camara.moverZnear(0.2); break;
        case 'b': a.camara.moverZfar(2); break; case 'B': a.camara.moverZfar(-2); break;
    }
    glutPostRedisplay();
}
void igvInterfaz::specialKeyFunc(int key,int,int){
    igvInterfaz& a=*_instancia;
    if(a.modoCamara){ switch(key){case GLUT_KEY_LEFT:a.camara.orbitar(-5);break;case GLUT_KEY_RIGHT:a.camara.orbitar(5);break;case GLUT_KEY_UP:a.camara.cabecear(3);break;case GLUT_KEY_DOWN:a.camara.cabecear(-3);break;} }
    else { int id=a.escena.getParteSeleccionada();
        if(id>=2) { switch(key){case GLUT_KEY_LEFT:a.escena.arrastrarParte(id,-5,0);break;case GLUT_KEY_RIGHT:a.escena.arrastrarParte(id,5,0);break;case GLUT_KEY_UP:a.escena.arrastrarParte(id,0,5);break;case GLUT_KEY_DOWN:a.escena.arrastrarParte(id,0,-5);break;} }
        else { int m=a.escena.getModeloSeleccionado(); switch(key){case GLUT_KEY_LEFT:a.escena.trasladarModelo(m,'x',-0.1f);break;case GLUT_KEY_RIGHT:a.escena.trasladarModelo(m,'x',0.1f);break;case GLUT_KEY_UP:a.escena.trasladarModelo(m,'z',-0.1f);break;case GLUT_KEY_DOWN:a.escena.trasladarModelo(m,'z',0.1f);break;} }
    } glutPostRedisplay();
}
void igvInterfaz::mouseFunc(int boton,int estado,int x,int y){
    igvInterfaz& a=*_instancia; if(!a.modoRaton||boton!=GLUT_LEFT_BUTTON)return;
    a.cursorX=x;a.cursorY=y;
    if(estado==GLUT_DOWN){a.botonRetenido=true;a.seleccionPendiente=true;glutPostRedisplay();}
    else if(estado==GLUT_UP)a.botonRetenido=false;
}
void igvInterfaz::motionFunc(int x,int y){
    igvInterfaz& a=*_instancia; if(!a.modoRaton||!a.botonRetenido)return;
    int dx=x-a.cursorX,dy=a.cursorY-y; int id=a.escena.getParteSeleccionada(); if(id>=2)a.escena.arrastrarParte(id,dx,dy);
    a.cursorX=x;a.cursorY=y;glutPostRedisplay();
}
void igvInterfaz::idleFunc(){
    static int ultimo=glutGet(GLUT_ELAPSED_TIME); int ahora=glutGet(GLUT_ELAPSED_TIME); float dt=(ahora-ultimo)*0.001f;ultimo=ahora;
    igvInterfaz& a=*_instancia; if(a.animacion)a.escena.actualizarAnimacion(ahora*0.001f); if(a.orbitaAutomatica)a.camara.orbitar(12*dt);
    if(a.animacion||a.orbitaAutomatica)glutPostRedisplay();
}
void igvInterfaz::menuFunc(int opcion){
    igvInterfaz& a=*_instancia; switch(opcion){case 1:a.modoRaton=!a.modoRaton;break;case 2:a.animacion=!a.animacion;break;case 3:a.orbitaAutomatica=!a.orbitaAutomatica;break;case 4:a.escena.cambiarSombreado();break;case 5:a.cuatroVistas=!a.cuatroVistas;break;} glutPostRedisplay();
}
