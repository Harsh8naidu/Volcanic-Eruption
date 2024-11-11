#ifndef TERRAIN_H
#define TERRAIN_H

#include <assimp/types.h> // Try changing this to #include types.h and type your own code in types.h
#include "basic_glfw_camera.h"
#include "array_2d.h"

#include "triangle_list.h"
#include "terrain_technique.h"

class BaseTerrain {
public:
	BaseTerrain() {}

	void CreateHeightMap(const char* filename, int width, int height);

	void InitTerrain();

	void Render(const BasicCamera& camera);

	void LoadFromFile(const char* filename);

	float GetHeight(int x, int z) const {return m_heightmap.Get(x, z);}

protected:
	void LoadHeightMapFile(const char* filename);

	int m_terrainSize;
	Array2D<float> m_heightmap;
	TriangleList m_triangleList;
	TerrainTechnique m_terrainTech;
};

#endif // TERRAIN_H