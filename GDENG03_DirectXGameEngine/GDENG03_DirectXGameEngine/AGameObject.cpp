#include "AGameObject.h"

AGameObject::AGameObject(std::string name)
{
	this->name = name;
	this->localPosition = Vector3D();
	this->localRotation = Vector3D();
	this->localScale = Vector3D(1.f,1.f,1.f);
}

AGameObject::~AGameObject()
{
}

void AGameObject::setPosition(float x, float y, float z)
{
	localPosition = Vector3D(x, y, z);
}

void AGameObject::setPosition(Vector3D pos)
{
	localPosition = pos;
}

Vector3D AGameObject::getLocalPosition()
{
	return this->localPosition;
}

void AGameObject::setScale(float x, float y, float z)
{
	localScale = Vector3D(x, y, z);
}

void AGameObject::setScale(Vector3D scale)
{
	localScale = scale;

}

Vector3D AGameObject::getLocalScale()
{
	return this->localScale;
}

void AGameObject::setRotation(float x, float y, float z)
{
	localRotation = Vector3D(x, y, z);
}

void AGameObject::setRotation(Vector3D rotation)
{
	localRotation = rotation;
}

void AGameObject::setRotationEuler(float x, float y, float z)
{
	localRotation = Vector3D(x / 60, y / 60, z / 60);
}

void AGameObject::setRotationEuler(Vector3D rotation)
{
	Vector3D newRotation = rotation * 0.6;

	localRotation = newRotation;
}

Vector3D AGameObject::getLocalRotation()
{
	return this->localRotation;
}

std::string AGameObject::getName()
{
	return this->name;
}