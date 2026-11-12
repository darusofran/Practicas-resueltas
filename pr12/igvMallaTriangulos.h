#ifndef __IGVMALLATRIANGULOS
#define __IGVMALLATRIANGULOS

#include <cstddef>

class igvMallaTriangulos {
protected:
    long num_vertices = 0;
    float* vertices = nullptr;
    float* normales = nullptr;
    long num_triangulos = 0;
    unsigned int* triangulos = nullptr;
public:
    igvMallaTriangulos() = default;
    igvMallaTriangulos(long nv, const float* v, long nt, const unsigned int* t);
    virtual ~igvMallaTriangulos();
    igvMallaTriangulos(const igvMallaTriangulos&) = delete;
    igvMallaTriangulos& operator=(const igvMallaTriangulos&) = delete;
    virtual void visualizar() const;
};

// Hoja/pétalo paramétrico con normales calculadas desde sus triángulos.
class igvPetaloParametrico : public igvMallaTriangulos {
public:
    igvPetaloParametrico(int divisionesLargo=24, int divisionesAncho=12);
};

#endif
