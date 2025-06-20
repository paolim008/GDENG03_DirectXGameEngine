#include "Plane.h"

#include <iostream>

#include "DeviceContext.h"
#include "EngineTime.h"
#include "GraphicsEngine.h"
#include "InputSystem.h"
#include "SceneCameraHandler.h"

Plane::Plane(string name, void* shaderByteCode, size_t sizeShader) : AGameObject(name)
{

	InputSystem::get()->addListener(this);
	//create buffers

	Vertex quadlist[] = {

		//FRONT FACE
		{Vector3D(-0.5f,-0.5f,-0.01f),    Vector3D(1,0.2,1),  Vector3D(0.2,1,1) },
		{Vector3D(-0.5f,0.5f,-0.01f),    Vector3D(1,0.5,0.2), Vector3D(0.2,1,1) },
		{ Vector3D(0.5f,0.5f,-0.01f),   Vector3D(1,0.3,0.2),  Vector3D(0.2,0.3,1) },
		{ Vector3D(0.5f,-0.5f,-0.01f),     Vector3D(1,0,0.2), Vector3D(0.2,0.3,1) },

		//BACK FACE
		{ Vector3D(0.5f,-0.5f,0.01f),    Vector3D(1,1,0.2), Vector3D(0.2,1,1), },
		{ Vector3D(0.5f,0.5f,0.01f),    Vector3D(1,0.5,0.2), Vector3D(0.2,0,1) },
		{ Vector3D(-0.5f,0.5f,0.01f),   Vector3D(1,0.3,0.2),  Vector3D(0.2,0.3,1) },
		{ Vector3D(-0.5f,-0.5f,0.01f),     Vector3D(1,0,0.2),  Vector3D(0.2,0.5,1) }

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

	cout << "Spawned Plane" << endl;
}

Plane::~Plane()
{
	AGameObject::~AGameObject();
}

void Plane::update(float deltaTime)
{
	this->cbData.time = EngineTime::getDeltaTime();

	// Matrix4x4 temp;
	//
	// this->deltaScale += deltaTime * this->speed;
	//
	// this->setRotation(this->deltaScale, this->deltaScale, this->deltaScale);

	this->constantBuffer->update(GraphicsEngine::get()->getImmediateDeviceContext(), &this->cbData);
}


void Plane::draw(int width, int height, VertexShader* vertexShader, PixelShader* pixelShader)
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

void Plane::setAnimSpeed(float speed)
{
	this->speed = speed;
}

void Plane::onKeyDown(int key)
{
	//return;
	if (key == 'W')
	{
		this->setRotation(localRotation + Vector3D(0, 0, 0.1));
	}
	if (key == 'S')
	{
		this->setRotation(localRotation + Vector3D(0, 0, -0.1));
	}
	if (key == 'D')
	{
		this->setRotation(localRotation + Vector3D(0.1, 0, 0));
	}
	if (key == 'A')
	{
		this->setRotation(localRotation + Vector3D(-0.1, 0, 0));
	}
	if (key == 'Q')
	{
		this->setRotation(localRotation + Vector3D(0, 0.1, 0));
	}
	if (key == 'E')
	{
		this->setRotation(localRotation + Vector3D(0, -0.1, 0));
	}
	if (key == 'O')
	{
		this->setRotation(0, 0, 0);
	}
	cout << name << " X: " << localRotation.m_x * 60 << endl;
	cout << name << " Y: " << localRotation.m_y * 60 << endl;
	cout << name << " Z: " << localRotation.m_z * 60 << endl;

	return;
	
	if (key == 'W')
	{
		this->setPosition(localPosition + Vector3D(0, 0, 0.1));
	}
	if (key == 'S')
	{
		this->setPosition(localPosition + Vector3D(0, 0, -0.1));
	}
	if (key == 'D')
	{
		this->setPosition(localPosition + Vector3D(0.1, 0, 0));
	}
	if (key == 'A')
	{
		this->setPosition(localPosition + Vector3D(-0.1, 0, 0));
	}
	if (key == 'Q')
	{
		this->setPosition(localPosition + Vector3D(0, 0.1, 0));
	}
	if (key == 'E')
	{
		this->setPosition(localPosition + Vector3D(0, -0.1, 0));
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

void Plane::onKeyUp(int key)
{

}

void Plane::onMouseMove(const Point& delta_mouse_pos)
{
}

void Plane::onLeftMouseDown(const Point& mouse_pos)
{
}

void Plane::onLeftMouseUp(const Point& mouse_pos)
{
}

void Plane::onRightMouseDown(const Point& mouse_pos)
{
}

void Plane::onRightMouseUp(const Point& mouse_pos)
{
}

// void Cube::onMiddleMouseDown(const Point& mouse_pos)
// {
// }
//
// void Cube::onMiddleMouseUp(const Point& mouse_pos)
// {
// }