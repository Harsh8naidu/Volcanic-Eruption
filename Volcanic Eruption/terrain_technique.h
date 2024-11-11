#ifndef TERRAIN_TECHNIQUE_H
#define TERRAIN_TECHNIQUE_H

#include "technique.h"
#include "math_3d.h"

class TerrainTechnique : public Technique
{
public:

    TerrainTechnique();

    virtual bool Init();

    void SetVP(const Matrix4f& VP);

private:
    GLuint m_VPLoc = -1;
};

#endif  /* TERRAIN_TECHNIQUE_H */#pragma once
