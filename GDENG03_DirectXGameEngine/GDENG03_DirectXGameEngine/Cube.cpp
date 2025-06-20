#include "Cube.h"

#include <iostream>

#include "DeviceContext.h"
#include "EngineTime.h"
#include "GraphicsEngine.h"
#include "InputSystem.h"
#include "SceneCameraHandler.h"

Cube::Cube(string name, void* shaderByteCode, size_t sizeShader) : AGameObject(name)
{

	InputSystem::get()->addListener(this);
	//create buffers

	Vertex quadlist[] = {

		
		//FRONT FACE
		{Vector3D(-0.5f,-0.5f,-0.5f),Vector3D(1,0,0),  Vector3D(1,0,0) },
		{Vector3D(-0.5f,0.5f,-0.5f),Vector3D(0,1,0), Vector3D(0,1,0) },
		{ Vector3D(0.5f,0.5f,-0.5f),Vector3D(0,0,1),  Vector3D(0,0,1) },
		{ Vector3D(0.5f,-0.5f,-0.5f),Vector3D(1,1,1), Vector3D(1,1,0) },

		//BACK FACE
		{ Vector3D(0.5f,-0.5f,0.5f),Vector3D(1,0,0), Vector3D(1,0,0), },
		{ Vector3D(0.5f,0.5f,0.5f),  Vector3D(0,1,0), Vector3D(0,1,0) },
		{ Vector3D(-0.5f,0.5f,0.5f), Vector3D(0,0,1),  Vector3D(0,0,1) },
		{ Vector3D(-0.5f,-0.5f,0.5f), Vector3D(1,1,0),  Vector3D(1,1,0) }

		// //FRONT FACE
		// {Vector3D(-0.5f,-0.5f,-0.5f),Vector3D(1,1,1),  Vector3D(1,1,1) },
		// {Vector3D(-0.5f,0.5f,-0.5f),Vector3D(1,1,1), Vector3D(1,1,1) },
		// { Vector3D(0.5f,0.5f,-0.5f),Vector3D(1,1,1),  Vector3D(1,1,1) },
		// { Vector3D(0.5f,-0.5f,-0.5f),Vector3D(1,1,1), Vector3D(1,1,1) },
		//
		// //BACK FACE
		// { Vector3D(0.5f,-0.5f,0.5f),Vector3D(1,1,1), Vector3D(1,1,1), },
		// { Vector3D(0.5f,0.5f,0.5f),  Vector3D(1,1,1), Vector3D(1,1,1) },
		// { Vector3D(-0.5f,0.5f,0.5f), Vector3D(1,1,1),  Vector3D(1,1,1) },
		// { Vector3D(-0.5f,-0.5f,0.5f), Vector3D(1,1,1),  Vector3D(1,1,1) }

	};

	this->vertexBuffer = GraphicsEngine::get()->createVertexBuffer();
	this->vertexBuffer->load(quadlist, sizeof(Vertex), ARRAYSIZE(quadlist), shaderByteCode, sizeShader);

	unsigned int indexList[] = {
		//FRONT SIDE
		0,1,2,  //FIRST TRIANGLE
		2,3,0,  //SECOND TRIANGLE
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
	this->cbData = {};

	this->constantBuffer = GraphicsEngine::get()->createConstantBuffer();
	this->constantBuffer->load(&cbData, sizeof(CBData));

	cout << "Spawned Cube" << endl;
}

Cube::~Cube()
{
	// this->vertexBuffer->release();
	// this->indexBuffer->release();
	// this->constantBuffer->release();
	AGameObject::~AGameObject();
}

void Cube::update(float deltaTime)
{
	this->cbData.time = EngineTime::getDeltaTime();
	

	//this->deltaPos += deltaTime / 10.0f;
	//if (this->deltaPos > 1.0f)
		//this->deltaPos = 0;


	Matrix4x4 temp;

	//this->deltaScale += deltaTime * this->speed;

	//this->setRotation(this->deltaScale, this->deltaScale, this->deltaScale);

	this->constantBuffer->update(GraphicsEngine::get()->getImmediateDeviceContext(), &this->cbData);


	//this->ticks += deltaTime;

	//float rotSpeed = this->ticks * this->speed * 50;
	//this->setRotation(rotSpeed, rotSpeed, rotSpeed);
	//
	// if (localScale.m_x > 40.f) return;
	//
	//
	// float scaleSpeed = this->ticks * this->speed/2;
	// this->setScale(localScale.m_x + scaleSpeed, localScale.m_y + scaleSpeed, 1.f);



}


void Cube::draw(int width, int height, VertexShader* vertexShader, PixelShader* pixelShader)
{
	if (this->deltaPos > 1.0f) {
		this->deltaPos = 0.0f;
	}
	else {
		this->deltaPos += this->deltaTime * 0.1f;
	}

	Matrix4x4 allMatrix;
	Matrix4x4 temp;

	allMatrix.setIdentity();
	allMatrix.setScale(this->localScale);

	temp.setIdentity();
	temp.setRotationZ(this->localRotation.m_z);
	allMatrix *= temp;

	temp.setIdentity();
	temp.setRotationY(this->localRotation.m_y);
	allMatrix *= temp;

	temp.setIdentity();
	temp.setRotationX(this->localRotation.m_x);
	allMatrix *= temp;

	temp.setIdentity();
	temp.setTranslation(this->localPosition);
	allMatrix *= temp;

	this->cbData.worldMatrix = allMatrix;

	Matrix4x4 cameraMatrix = SceneCameraHandler::get()->getSceneCameraViewMatrix();
	this->cbData.viewMatrix = cameraMatrix;

	float aspectRatio = (float)width / (float)height;
	this->cbData.projMatrix.setPerspectiveFovLH(aspectRatio, aspectRatio, 0.0f, 1000.0f);

	this->constantBuffer->update(GraphicsEngine::get()->getImmediateDeviceContext(), &this->cbData);

	GraphicsEngine::get()->getImmediateDeviceContext()->setConstantBuffer(vertexShader, this->constantBuffer);
	GraphicsEngine::get()->getImmediateDeviceContext()->setConstantBuffer(pixelShader, this->constantBuffer);

	GraphicsEngine::get()->getImmediateDeviceContext()->setIndexBuffer(this->indexBuffer);
	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexBuffer(this->vertexBuffer);

	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexShader(vertexShader);
	GraphicsEngine::get()->getImmediateDeviceContext()->setPixelShader(pixelShader);

	GraphicsEngine::get()->getImmediateDeviceContext()->drawIndexedTriangleList(this->indexBuffer->getSizeIndexList(), 0, 0);
}

void Cube::setAnimSpeed(float speed)
{
	this->speed = speed;
}

void Cube::onKeyDown(int key)
{
	return;
	/*
	if (key == 'W')
	{
		this->setRotation(localRotation + Vector3D(0, 0, 0.01));
	}
	if (key == 'S')
	{
		this->setRotation(localRotation + Vector3D(0, 0, -0.01));
	}
	if (key == 'D')
	{
		this->setRotation(localRotation + Vector3D(0.01, 0, 0));
	}
	if (key == 'A')
	{
		this->setRotation(localRotation + Vector3D(-0.01, 0, 0));
	}
	if (key == 'Q')
	{
		this->setRotation(localRotation + Vector3D(0, 0.01, 0));
	}
	if (key == 'E')
	{
		this->setRotation(localRotation + Vector3D(0, -0.01, 0));
	}
	cout << "X: " << localRotation.m_x << endl;
	cout << "Y: " << localRotation.m_y << endl;
	cout << "Z: " << localRotation.m_z << endl;

	return;*/

	if (key == 'W')
	{
		this->setPosition(localPosition + Vector3D(0, 0, 0.2));
	}
	if (key == 'S')
	{
		this->setPosition(localPosition + Vector3D(0, 0, -0.2));
	}
	if (key == 'D')
	{
		this->setPosition(localPosition + Vector3D(0.2, 0, 0));
	}
	if (key == 'A')
	{
		this->setPosition(localPosition + Vector3D(-0.2, 0, 0));
	}
	if (key == 'Q')
	{
		this->setPosition(localPosition + Vector3D(0, 0.2, 0));
	}
	if (key == 'E')
	{
		this->setPosition(localPosition + Vector3D(0, -0.2, 0));
	}
	cout << "X: " << localPosition.m_x << endl;
	cout << "Y: " << localPosition.m_y << endl;
	cout << "Z: " << localPosition.m_z << endl;

	/*if (key == 'W')
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

	}*/

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