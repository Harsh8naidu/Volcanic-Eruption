#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <cerrno>
#include <string.h>

#include "terrain.h"
#include <iostream>

// In terrain.cpp
void BaseTerrain::CreateHeightMap(const char* filename, int width, int height) {
	// Generate some random float data for a heightmap
	std::vector<float> heightmapData(width * height);

	// Fill with random height values for a mountainous terrain
	srand(static_cast<unsigned>(time(0)));
	for (int i = 0; i < width * height; ++i) {
		heightmapData[i] = static_cast<float>(rand() % 200) / 10.0f; // random height values
	}

	// Write the data to a binary file
	WriteBinaryFile(filename, heightmapData.data(), heightmapData.size() * sizeof(float));
}

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
	
	//assert(fileSize % sizeof(float) == 0);

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
