#pragma once
#include "PixelShader.h"
#include "VertexBuffer.h"
#include "VertexShader.h"
#include "math.h"

class Gameobject
{
public:
	Gameobject();

	void initializeShaders(vertex* data);
	void update();
	virtual void draw() = 0;
	virtual ~Gameobject();
protected:
	VertexBuffer* m_vb;
	VertexShader* m_vs;
	PixelShader* m_ps;
};