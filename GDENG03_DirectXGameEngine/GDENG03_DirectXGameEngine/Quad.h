#pragma once
#include "Gameobject.h"
#include "math.h"
#include "Vertex.h"


class Quad : public Gameobject
{

public:
	Quad(vertex* vertexList);
	Quad(Vertex* a, Vertex* b, Vertex* c, Vertex* d);
	~Quad();
	void draw() override;

private:
	vertex m_vertex_list[4];

};