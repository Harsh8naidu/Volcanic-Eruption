#ifndef TRIANGLE_LIST_H
#define TRIANGLE_LIST_H

#include <glew.h>
#include <vector>

#include "math_3d.h"

// Important for assignment:
// This class will create and render a grid of triangles
// while taking their height from the heightmap.

// this header is included in the terrain.h so we have
// a forward declaration of the BaseTerrain class

class BaseTerrain;

class TriangleList
{
public:
	TriangleList();

	void CreateTriangleList(int Width, int Depth, const BaseTerrain* pTerrain);

	void Render();

private:
	struct Vertex {
		Vector3f Pos;

		void InitVertex(const BaseTerrain* pTerrain, int x, int z); // Initialize the vertex position
	};

	void CreateGLState();

	void PopulateBuffers(const BaseTerrain* pTerrain);


	void InitVertices(const BaseTerrain* pTerrain, std::vector<Vertex>& Vertices);

	int m_width = 0;
	int m_depth = 0;
	GLuint m_vao; // Vertex Array Object
	GLuint m_vb; // Vertex Buffer Object
	GLuint m_ib; // Index Buffer Object
};

#endif // TRIANGLE_LIST_H
