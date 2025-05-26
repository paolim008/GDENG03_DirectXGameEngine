#pragma once
class Vec3
{
public:
	Vec3(float x, float y, float z);
	Vec3(Vec3* vector);
	~Vec3();
public:
	float m_x, m_y, m_z;

};