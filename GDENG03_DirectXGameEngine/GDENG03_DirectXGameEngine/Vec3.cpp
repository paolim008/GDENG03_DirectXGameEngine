#include "Vec3.h"

Vec3::Vec3(float x, float y, float z) : m_x(x), m_y(y), m_z(z)
{
}

Vec3::Vec3(Vec3* vector) : m_x(vector->m_x), m_y(vector->m_y), m_z(vector->m_z)
{
}

Vec3::~Vec3()
{
}