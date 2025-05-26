#pragma once
#include "Vec3.h"

class Vertex
{
public:
	Vertex(float x, float y, float z, float r, float g, float b);
	~Vertex();
public:
	Vec3 position;
	Vec3 color;
};