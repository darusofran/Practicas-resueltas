#include "igvMallaTriangulos.h"
#include <cmath>
#include <cstring>
#include <vector>
#include <algorithm>
#include <initializer_list>
#if defined(__APPLE__) && defined(__MACH__)
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

igvMallaTriangulos::igvMallaTriangulos(long nv, const float* v, long nt, const unsigned int* t)
    : num_vertices(nv), num_triangulos(nt) {
    if (nv > 0) { vertices = new float[nv*3]; std::memcpy(vertices, v, sizeof(float)*nv*3); }
    if (nt > 0) { triangulos = new unsigned int[nt*3]; std::memcpy(triangulos, t, sizeof(unsigned int)*nt*3); }
    normales = new float[nv*3]();
    // Promedia las normales de las caras vecinas para suavizado de Gouraud.
    for (long i=0; i<nt; ++i) {
        unsigned int a=triangulos[i*3], b=triangulos[i*3+1], c=triangulos[i*3+2];
        const float* A=&vertices[a*3]; const float* B=&vertices[b*3]; const float* C=&vertices[c*3];
        float ux=B[0]-A[0], uy=B[1]-A[1], uz=B[2]-A[2];
        float vx=C[0]-A[0], vy=C[1]-A[1], vz=C[2]-A[2];
        float nx=uy*vz-uz*vy, ny=uz*vx-ux*vz, nz=ux*vy-uy*vx;
        for (unsigned int idx : {a,b,c}) { normales[idx*3]+=nx; normales[idx*3+1]+=ny; normales[idx*3+2]+=nz; }
    }
    for (long i=0; i<nv; ++i) {
        float* n=&normales[i*3]; float d=std::sqrt(n[0]*n[0]+n[1]*n[1]+n[2]*n[2]);
        if (d>1e-7f) { n[0]/=d; n[1]/=d; n[2]/=d; } else n[2]=1;
    }
}

igvMallaTriangulos::~igvMallaTriangulos() { delete[] vertices; delete[] normales; delete[] triangulos; }

void igvMallaTriangulos::visualizar() const {
    if (!vertices || !triangulos) return;
    glEnableClientState(GL_VERTEX_ARRAY); glEnableClientState(GL_NORMAL_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices); glNormalPointer(GL_FLOAT, 0, normales);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(num_triangulos*3), GL_UNSIGNED_INT, triangulos);
    glDisableClientState(GL_NORMAL_ARRAY); glDisableClientState(GL_VERTEX_ARRAY);
}

igvPetaloParametrico::igvPetaloParametrico(int nl, int na) {
    nl=std::max(2,nl); na=std::max(2,na);
    const long nv=static_cast<long>(nl+1)*(na+1), nt=static_cast<long>(nl)*na*2;
    std::vector<float> v(nv*3); std::vector<unsigned int> t(nt*3);
    for (int i=0; i<=nl; ++i) {
        float u=static_cast<float>(i)/nl;
        float ancho=0.56f*std::pow(std::sin(3.14159265f*u),0.72f);
        for (int j=0; j<=na; ++j) {
            float w=2.0f*static_cast<float>(j)/na-1.0f; long k=(static_cast<long>(i)*(na+1)+j)*3;
            v[k]=1.65f*u; v[k+1]=ancho*w; v[k+2]=0.16f*std::sin(3.14159265f*u)*(1.0f-w*w);
        }
    }
    long k=0;
    for (int i=0;i<nl;++i) for (int j=0;j<na;++j) {
        unsigned int a=i*(na+1)+j, b=(i+1)*(na+1)+j, c=b+1, d=a+1;
        t[k++]=a; t[k++]=b; t[k++]=c; t[k++]=a; t[k++]=c; t[k++]=d;
    }
    num_vertices=nv; num_triangulos=nt;
    vertices=new float[v.size()]; std::copy(v.begin(),v.end(),vertices);
    triangulos=new unsigned int[t.size()]; std::copy(t.begin(),t.end(),triangulos);
    normales=new float[nv*3]();
    for (long i=0;i<nt;++i) {
        unsigned int a=triangulos[i*3],b=triangulos[i*3+1],c=triangulos[i*3+2];
        const float* A=&vertices[a*3]; const float* B=&vertices[b*3]; const float* C=&vertices[c*3];
        float ux=B[0]-A[0],uy=B[1]-A[1],uz=B[2]-A[2],vx=C[0]-A[0],vy=C[1]-A[1],vz=C[2]-A[2];
        float n[3]={uy*vz-uz*vy,uz*vx-ux*vz,ux*vy-uy*vx};
        for (unsigned int idx : {a,b,c}) for(int q=0;q<3;++q) normales[idx*3+q]+=n[q];
    }
    for(long i=0;i<nv;++i){float* n=&normales[i*3];float d=std::sqrt(n[0]*n[0]+n[1]*n[1]+n[2]*n[2]);if(d>1e-7f){n[0]/=d;n[1]/=d;n[2]/=d;}else n[2]=1;}
}

