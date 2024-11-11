#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <cerrno>
#include <string.h>

#include "terrain.h"
#include <iostream>

void BaseTerrain::InitTerrain()
{
	m_terrainTech.Init();

}

void BaseTerrain::LoadFromFile(const char* pFilename)
{
	LoadHeightMapFile(pFilename);

	m_triangleList.CreateTriangleList(m_terrainSize, m_terrainSize, this);
}

void BaseTerrain::LoadHeightMapFile(const char* filename)
{
	int fileSize = 0;
	unsigned char* p = (unsigned char*)ReadBinaryFile(filename, fileSize);
	
	assert(fileSize % sizeof(float) == 0);

	m_terrainSize = (int)sqrt((float)fileSize / sizeof(float));

	m_heightmap.InitArray2D(m_terrainSize, m_terrainSize, p);

	m_heightmap.PrintFloat();
}

void BaseTerrain::Render(const BasicCamera& camera)
{
	Matrix4f VP = camera.GetViewProjMatrix();

	if (m_terrainTech.GetProgram() == 0)
	{
		m_terrainTech.Init();
	}
	m_terrainTech.Enable();
	m_terrainTech.SetVP(VP);

	m_triangleList.Render();
}
