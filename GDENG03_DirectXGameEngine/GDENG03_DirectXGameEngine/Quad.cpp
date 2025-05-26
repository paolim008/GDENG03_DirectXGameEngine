#include "Quad.h"

#include "DeviceContext.h"
#include "GraphicsEngine.h"
#include "math.h"

//Initialize Quad with a list of vertices
Quad::Quad(vertex* vertexList)
{
	initializeShaders(vertexList);
}

//Initialize Quad with 4 vertices
Quad::Quad(Vertex* a, Vertex* b, Vertex* c, Vertex* d)
{
	vertex vertexList[] = {
		a->position.m_x, a->position.m_y, a->position.m_z, a->color.m_x, a->color.m_y, a->color.m_z,
		b->position.m_x, b->position.m_y, b->position.m_z, b->color.m_x, b->color.m_y, b->color.m_z,
		c->position.m_x, c->position.m_y, c->position.m_z, c->color.m_x, c->color.m_y, c->color.m_z,
		d->position.m_x, d->position.m_y, d->position.m_z, d->color.m_x, d->color.m_y, d->color.m_z
	};

	initializeShaders(vertexList);
}

Quad::~Quad()
{
}

void Quad::draw()
{
	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexBuffer(m_vb);
	GraphicsEngine::get()->getImmediateDeviceContext()->drawTriangleStrip(m_vb->getSizeVertexList(), 0);
}