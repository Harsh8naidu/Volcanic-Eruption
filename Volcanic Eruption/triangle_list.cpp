#include <stdio.h>
#include <vector>

#include "math_3d.h"
#include "triangle_list.h"
#include "terrain.h"

TriangleList::TriangleList()
{
}

void TriangleList::CreateTriangleList(int Width, int Depth, const BaseTerrain* pTerrain)
{
	m_width = Width;
	m_depth = Depth;

	CreateGLState();

	PopulateBuffers(pTerrain);

	// Unbind the VAO
	glBindVertexArray(0); // Unbind the VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind the VBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // Unbind the IBO
}

void TriangleList::CreateGLState()
{
	glGenVertexArrays(1, &m_vao);

	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vb);

	glBindBuffer(GL_ARRAY_BUFFER, m_vb);

	int POS_LOC = 0; // Position location

	glEnableVertexAttribArray(POS_LOC);

	size_t NumFloats = 0;
	glVertexAttribPointer(POS_LOC, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(NumFloats * sizeof(float)));
	NumFloats += 3;
}

void TriangleList::PopulateBuffers(const BaseTerrain* pTerrain)
{
	std::vector<Vertex> Vertices;
	Vertices.resize(m_width * m_depth);

	InitVertices(pTerrain, Vertices);

	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices[0]) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);
}

void TriangleList::Vertex::InitVertex(const BaseTerrain* pTerrain, int x, int z)
{
	float y = pTerrain->GetHeight(x, z);
	Pos = Vector3f(x, y, z); // Height map is in the y axis
}

void TriangleList::InitVertices(const BaseTerrain* pTerrain, std::vector<Vertex>& Vertices)
{
	int index = 0;

	for (int z = 0; z < m_depth; z++) {
		for (int x = 0; x < m_width; x++) {
			assert(index < Vertices.size());
			Vertices[index].InitVertex(pTerrain, x, z);
			index++;
		}
	}
}

void TriangleList::Render()
{
	glBindVertexArray(m_vao);

	glDrawArrays(GL_POINTS, 0, m_width * m_depth); // Draw the points

	glBindVertexArray(0); // Unbind the VAO
}
