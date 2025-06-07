#pragma once
#include <string>

#include "Matrix4x4.h"
#include "Vector3D.h"

class PixelShader;
class VertexShader;

class AGameObject
{
public:
	AGameObject(std::string name);
	virtual ~AGameObject();

	virtual void update(float deltaTime) = 0;
	virtual void draw(int width, int height, VertexShader* vertexShader, PixelShader* pixelShader) = 0;

	void setPosition(float x, float y, float z);
	void setPosition(Vector3D pos);
	Vector3D getLocalPosition();

	void setScale(float x, float y, float z);
	void setScale(Vector3D scale);
	Vector3D getLocalScale();

	void setRotation(float x, float y, float z);
	void setRotation(Vector3D rotation);
	Vector3D getLocalRotation();

	std::string getName();

	struct Vertex
	{
		Vector3D position;
		Vector3D scale;
		Vector3D rotation;
	};

	_declspec(align(16))
		struct CBData
	{
		Matrix4x4 worldMatrix;
		Matrix4x4 viewMatrix;
		Matrix4x4 projMatrix;

		float time;
	};

protected:
	std::string name;
	Vector3D localPosition;
	Vector3D localRotation;
	Vector3D localScale;
	Matrix4x4 localMatrix;


};