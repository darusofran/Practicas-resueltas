#include "igvEscena3D.h"
#include <algorithm>
#include <cmath>

namespace { const float PI=3.14159265358979323846f; }

igvEscena3D::igvEscena3D() : mallaPetalo(new igvPetaloParametrico(24,12)) {
    modelos[0].tx=-2.55f; modelos[0].ty=0.9f; modelos[0].rz=18;
    modelos[1].tx=1.0f; modelos[1].ty=-0.25f;
}
igvEscena3D::~igvEscena3D() { delete mallaPetalo; }

void igvEscena3D::pintarEjes() {
    glDisable(GL_LIGHTING); glBegin(GL_LINES);
    glColor3f(0.85f,0.1f,0.1f); glVertex3f(-8,0,0); glVertex3f(8,0,0);
    glColor3f(0.1f,0.65f,0.1f); glVertex3f(0,-1,0); glVertex3f(0,7,0);
    glColor3f(0.1f,0.2f,0.85f); glVertex3f(0,0,-6); glVertex3f(0,0,6);
    glEnd(); glEnable(GL_LIGHTING);
}

void igvEscena3D::material(float r,float g,float b,bool seleccion,int id) {
    if(seleccion) { unsigned int c=static_cast<unsigned int>(id); glColor3ub(c&255,(c>>8)&255,(c>>16)&255); return; }
    if(id==parteSeleccionada) { r=1.0f; g=0.78f; b=0.08f; }
    GLfloat dif[]={r,g,b,1}, amb[]={r*0.25f,g*0.25f,b*0.25f,1}, spec[]={0.35f,0.35f,0.35f,1};
    glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,dif); glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,amb);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,spec); glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,28);
}
void igvEscena3D::cilindro(float rb,float rt,float h,int id,bool sel,int lados) {
    material(0.18f,0.45f,0.19f,sel,id); GLUquadric* q=gluNewQuadric(); gluQuadricNormals(q,GLU_SMOOTH);
    gluCylinder(q,rb,rt,h,lados,4); gluDeleteQuadric(q);
}
void igvEscena3D::esfera(float r,int id,bool sel,int sl,int st) {
    material(0.82f,0.62f,0.18f,sel,id); glutSolidSphere(r,sl,st);
}

void igvEscena3D::dibujarFlor(bool sel) {
    // Nodo 1: maceta; nodo 2: tallo; sus hijos son las dos ramas/hojas.
    glPushMatrix(); glTranslatef(0,0.25f,0);
    material(0.58f,0.22f,0.12f,sel,0); GLUquadric* maceta=gluNewQuadric(); gluQuadricNormals(maceta,GLU_SMOOTH);
    glPushMatrix(); glRotatef(-90,1,0,0); gluCylinder(maceta,0.68,0.48,0.95,32,3); gluDisk(maceta,0,0.68,32,1); glPopMatrix(); gluDeleteQuadric(maceta);
    material(0.18f,0.12f,0.07f,sel,0); glPushMatrix(); glTranslatef(0,0.94f,0); glScalef(0.48f,0.08f,0.48f); glutSolidSphere(1,24,16); glPopMatrix();
    glPopMatrix();

    glPushMatrix(); // nodo tallo con inclinación y traslación vertical
    glTranslatef(0,1.15f+alturaCabeza*0.45f,0); glRotatef(inclinacionZ,0,0,1); glRotatef(inclinacionX,1,0,0);
    glPushMatrix(); glRotatef(-90,1,0,0); cilindro(0.11f,0.075f,2.55f,2,sel,20); glPopMatrix();

    // Rama izquierda: traslación + rotación independiente y hoja elipsoidal.
    glPushMatrix(); glTranslatef(0,1.25f,0); glRotatef(-35+giroHojaIzquierda,0,0,1); glRotatef(-12,1,0,0);
    glPushMatrix(); glTranslatef(0,0.42f,0); glScalef(0.78f,0.08f,0.3f); material(0.13f,0.56f,0.2f,sel,3); glutSolidSphere(1,24,16); glPopMatrix();
    glPopMatrix();
    // Rama derecha (nodo hermano, jerarquía no lineal).
    glPushMatrix(); glTranslatef(0,1.85f,0); glRotatef(35+giroHojaDerecha,0,0,1); glRotatef(10,1,0,0);
    glPushMatrix(); glTranslatef(0,0.42f,0); glScalef(0.72f,0.075f,0.28f); material(0.15f,0.62f,0.22f,sel,4); glutSolidSphere(1,24,16); glPopMatrix();
    glPopMatrix();

    // Nudo de la flor, cabeza giratoria y ocho pétalos articulados.
    glPushMatrix(); glTranslatef(0,2.55f+alturaCabeza,0); glRotatef(giroCabeza,0,1,0);
    glPushMatrix(); glScalef(0.42f,0.3f,0.42f); esfera(1,5,sel,24,18); glPopMatrix();
    for(int i=0;i<8;++i) {
        int id=6+i; float ang=45.0f*i;
        glPushMatrix(); glRotatef(ang,0,1,0); glRotatef(aperturaPetalos,0,0,1); glTranslatef(0.72f,0,0);
        material(0.88f,0.18f+0.035f*(i%3),0.37f,sel,id); // nodo pétalo
        glScalef(0.86f,0.14f,0.38f); GLUquadric* petalo=gluNewQuadric(); gluQuadricNormals(petalo,GLU_SMOOTH); gluSphere(petalo,1.0,24,16); gluDeleteQuadric(petalo); glPopMatrix();
    }
    // Centro de la flor: esfera y pequeños puntos, cada punto es un nodo hijo.
    glPushMatrix(); glTranslatef(0,0.03f,0); glScalef(0.25f,0.25f,0.25f); esfera(1,5,sel,20,14); glPopMatrix();
    for(int i=0;i<6;++i) { glPushMatrix(); float a=2*PI*i/6; glTranslatef(0.27f*std::cos(a),0.05f,0.27f*std::sin(a)); material(0.96f,0.68f,0.08f,sel,5); glutSolidSphere(0.045,10,8); glPopMatrix(); }
    glPopMatrix();
    glPopMatrix();
}

void igvEscena3D::visualizar(bool seleccion) {
    if(!seleccion && ejes) pintarEjes();
    glPushMatrix();
    Transformacion& m=modelos[0]; glTranslatef(m.tx,m.ty,m.tz); glRotatef(m.rx,1,0,0); glRotatef(m.ry,0,1,0); glRotatef(m.rz,0,0,1); glScalef(m.escala,m.escala,m.escala);
    if(seleccion) glColor3ub(0,0,0); else material(0.91f,0.35f,0.48f,false,1);
    if(!seleccion) glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,35);
    mallaPetalo->visualizar(); glPopMatrix();
    glPushMatrix();
    Transformacion& f=modelos[1]; glTranslatef(f.tx,f.ty,f.tz); glRotatef(f.rx,1,0,0); glRotatef(f.ry,0,1,0); glRotatef(f.rz,0,0,1); glScalef(f.escala,f.escala,f.escala);
    dibujarFlor(seleccion); glPopMatrix();
    if(!seleccion) glShadeModel(sombreadoSuave?GL_SMOOTH:GL_FLAT);
}

void igvEscena3D::seleccionarParte(int id) { parteSeleccionada=(id>=2 && id<=13)?id:0; modeloSeleccionado=parteSeleccionada?1:0; }
void igvEscena3D::arrastrarParte(int id,int dx,int dy) {
    const float k=0.45f;
    if(id==2){ inclinacionZ=std::max(-35.0f,std::min(35.0f,inclinacionZ+dx*k)); inclinacionX=std::max(-30.0f,std::min(30.0f,inclinacionX+dy*k)); }
    else if(id==3) giroHojaIzquierda=std::max(-30.0f,std::min(30.0f,giroHojaIzquierda+dy*k));
    else if(id==4) giroHojaDerecha=std::max(-30.0f,std::min(30.0f,giroHojaDerecha+dy*k));
    else if(id==5) giroCabeza+=dx*k;
    else if(id>=6&&id<=11) aperturaPetalos=std::max(-15.0f,std::min(65.0f,aperturaPetalos+dy*k));
}
void igvEscena3D::actualizarAnimacion(float t) {
    inclinacionX=8*std::sin(t*0.7f); inclinacionZ=11*std::sin(t*0.47f);
    giroCabeza=24*std::sin(t*0.8f); aperturaPetalos=18+15*(0.5f+0.5f*std::sin(t*1.2f));
    giroHojaIzquierda=8*std::sin(t*0.9f); giroHojaDerecha=-8*std::sin(t*0.9f); alturaCabeza=0.12f*std::sin(t*1.2f);
    modelos[0].ry=18*std::sin(t*0.55f);
}
void igvEscena3D::cambiarSombreado(){ sombreadoSuave=!sombreadoSuave; glShadeModel(sombreadoSuave?GL_SMOOTH:GL_FLAT); }
void igvEscena3D::rotarModelo(int idx,char eje,float d){ Transformacion& m=modelos[idx]; if(eje=='x')m.rx+=d; if(eje=='y')m.ry+=d; if(eje=='z')m.rz+=d; }
void igvEscena3D::trasladarModelo(int idx,char eje,float d){ Transformacion& m=modelos[idx]; if(eje=='x')m.tx+=d; if(eje=='y')m.ty+=d; if(eje=='z')m.tz+=d; }
void igvEscena3D::escalarModelo(int idx,float f){ modelos[idx].escala=std::max(0.35f,std::min(2.5f,modelos[idx].escala*f)); }
