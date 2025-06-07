#include "Cube.h"

#include <iostream>
#include <ostream>

#include "DeviceContext.h"
#include "GraphicsEngine.h"
#include "InputSystem.h"

Cube::Cube(string name, void* shaderByteCode, size_t sizeShader) : AGameObject(name)
{

	InputSystem::get()->addListener(this);
	//create buffers

	Vertex quadlist[] = {

		//FRONT FACE
		{Vector3D(-.5f,-.5f,-.5f), Vector3D(1,0,0), Vector3D(0.2f,0,0)},
		{Vector3D(-.5f,.5f,-.5f), Vector3D(1,1,0), Vector3D(0.2f,.2f,0)},
		{Vector3D(.5f,.5f,-.5f), Vector3D(1,1,0), Vector3D(0.2f,.2f,0)},
		{Vector3D(.5f,-.5f,-.5f), Vector3D(1,0,0), Vector3D(0.2f,0,0)},

		//BACK FACE
		{Vector3D(.5f,-.5f,.5f), Vector3D(0,1,0), Vector3D(0, 0.2f,0)},
		{Vector3D(.5f,.5f,.5f), Vector3D(0,1,1), Vector3D(0, 0.2f,0.2f)},
		{Vector3D(-.5f,.5f,.5f), Vector3D(0,1,1), Vector3D(0, 0.2f,0.2f)},
		{Vector3D(-.5f,-.5f,.5f), Vector3D(0,1,0), Vector3D(0, 0.2f,0)},

	};

	this->vertexBuffer = GraphicsEngine::get()->createVertexBuffer();
	this->vertexBuffer->load(quadlist, sizeof(Vertex), ARRAYSIZE(quadlist), shaderByteCode, sizeShader);

	unsigned int indexList[] = {
		//FRONT SIDE
		0,1,2, //FRIST TRIANGLE
		2,3,0, // SECOND TRIANGLE
		//BACK SIDE
		4,5,6,
		6,7,4,
		//TOP SIDE
		1,6,5,
		5,2,1,
		//BOTTOM SIDE
		7,0,3,
		3,4,7,
		//RIGHT SIDE
		3,2,5,
		5,4,3,
		//LEFT SIDE
		7,6,1,
		1,0,7
	};

	this->indexBuffer = GraphicsEngine::get()->createIndexBuffer();
	this->indexBuffer->load(indexList, ARRAYSIZE(indexList));

	//create constant buffer
	CBData cbData = {};
	cbData.time = 0;

	this->constantBuffer = GraphicsEngine::get()->createConstantBuffer();
	this->constantBuffer->load(&cbData, sizeof(CBData));

	cout << "Spawned Cube" << endl;
}

Cube::~Cube()
{
	this->vertexBuffer->release();
	this->indexBuffer->release();
	AGameObject::~AGameObject();
}

void Cube::update(float deltaTime)
{
	this->deltaTime = deltaTime;
}

void Cube::draw(int width, int height, VertexShader* vertexShader, PixelShader* pixelShader)
{
	GraphicsEngine* graphicsEngine = GraphicsEngine::get();
	DeviceContext* deviceContext = graphicsEngine->getImmediateDeviceContext();

	CBData cbData = {};

	if (this->deltaPos > 1.0f)
	{
		this->deltaPos = 0.0f;
	}
	else
	{
		this->deltaPos += this->deltaTime * 0.1f;
	}

	Matrix4x4 allMatrix; allMatrix.setIdentity();
	Matrix4x4 translationMatrix; translationMatrix.setTranslation(this->getLocalPosition());
	Matrix4x4 scaleMatrix; scaleMatrix.setScale(this->getLocalScale());
	Vector3D rotation = this->getLocalRotation();

	Matrix4x4 zMatrix; zMatrix.setRotationZ(rotation.m_z);
	Matrix4x4 xMatrix; xMatrix.setRotationZ(rotation.m_x);
	Matrix4x4 yMatrix; yMatrix.setRotationZ(rotation.m_y);

	//Scale -> Rotate -> Transform
	Matrix4x4 rotMatrix; rotMatrix.setIdentity();
	rotMatrix = rotMatrix.multiplyTo(xMatrix.multiplyTo(yMatrix.multiplyTo(zMatrix)));
	allMatrix = allMatrix.multiplyTo(scaleMatrix.multiplyTo(rotMatrix));
	allMatrix = allMatrix.multiplyTo(translationMatrix);

	cbData.worldMatrix = allMatrix;

	cbData.viewMatrix.setIdentity();
	cbData.projMatrix.setOrthoLH(width / 400.0f, height / 400.0f, -4.f, 4.f);

	this->constantBuffer->update(deviceContext, &cbData);
	deviceContext->setConstantBuffer(vertexShader, this->constantBuffer);
	deviceContext->setConstantBuffer(pixelShader, this->constantBuffer);

	deviceContext->setIndexBuffer(this->indexBuffer);
	deviceContext->setVertexBuffer(this->vertexBuffer);

	deviceContext->drawIndexedTriangleList(this->indexBuffer->getSizeIndexList(), 0, 0);
}

void Cube::setAnimSpeed(float speed)
{
	this->speed = speed;
}

void Cube::onKeyDown(int key)
{
	if (key == 'W')
	{
		this->ticks += deltaTime;
		float rotSpeed = this->ticks * this->speed;
		this->setRotation(rotSpeed, rotSpeed, rotSpeed);
	}
	else if (key == 'E')
	{
		this->setPosition(this->getLocalPosition() + Vector3D(5.f, 0.f, 0.f));
	}
	else if (key == 'S')
	{
		this->ticks -= deltaTime;

		float rotSpeed = this->ticks * this->speed;
		this->setRotation(rotSpeed, rotSpeed, rotSpeed);

	}
}

void Cube::onKeyUp(int key)
{

}

void Cube::onMouseMove(const Point& delta_mouse_pos)
{
}

void Cube::onLeftMouseDown(const Point& mouse_pos)
{
}

void Cube::onLeftMouseUp(const Point& mouse_pos)
{
}

void Cube::onRightMouseDown(const Point& mouse_pos)
{
}

void Cube::onRightMouseUp(const Point& mouse_pos)
{
}

// void Cube::onMiddleMouseDown(const Point& mouse_pos)
// {
// }
//
// void Cube::onMiddleMouseUp(const Point& mouse_pos)
// {
// }