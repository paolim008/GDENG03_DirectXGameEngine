#include "Camera.h"

#include "InputSystem.h"

Camera::Camera(std::string name): AGameObject(name)
{
	this->setPosition(0.0f, 0.0f, -4.0f);
	this->updateViewMatrix();
	InputSystem::get()->addListener(this);
}

Camera::~Camera()
{
	InputSystem::get()->removeListener(this);
}

void Camera::update(float deltaTime)
{
	this->deltaTime = deltaTime;

}

Matrix4x4 Camera::getViewMatrix()
{
	return this->localMatrix;
}

void Camera::updateViewMatrix()
{
	Matrix4x4 worldCam; worldCam.setIdentity();
	Matrix4x4 temp; temp.setIdentity();

	Vector3D localRot = this->getLocalPosition();

	temp.setRotationX(localRot.m_x);
	worldCam = worldCam.multiplyTo(temp);


	temp.setRotationY(localRot.m_y);
	worldCam = worldCam.multiplyTo(temp);


	temp.setTranslation(this->getLocalPosition());
	worldCam = worldCam.multiplyTo(temp);

	worldCam.inverse();
	this->localMatrix = worldCam;


}

void Camera::draw(int width, int height, VertexShader* vertexShader, PixelShader* pixelShader)
{
}

void Camera::onKeyDown(int key)
{
	Vector3D localPos = this->getLocalPosition();
	float x = localPos.m_x;
	float y = localPos.m_y;
	float z = localPos.m_z;
	float moveSpeed = 10.0f;

	if (key == 'W')
	{
		z += deltaTime * moveSpeed;
		this->setPosition(x,y,z);
		this->updateViewMatrix();
	}
	if (key == 'S')
	{
		z -= deltaTime * moveSpeed;
		this->setPosition(x, y, z);
		this->updateViewMatrix();
	}
	if (key == 'A')
	{
		x += deltaTime * moveSpeed;
		this->setPosition(x, y, z);
		this->updateViewMatrix();
	}
	if (key == 'D')
	{
		x -= deltaTime * moveSpeed;
		this->setPosition(x, y, z);
		this->updateViewMatrix();
	}
}

void Camera::onKeyUp(int key)
{
}

void Camera::onMouseMove(const Point& deltaPos)
{
	if (this->mouseDown)
	{
		Vector3D localRot = this->getLocalRotation();
		float x = localRot.m_x;
		float y = localRot.m_y;
		float z = localRot.m_z;

		float speed = 0.005f;
		x += deltaPos.m_y * speed;
		y += deltaPos.m_x * speed;

		this->setRotation(x, y, z);
		this->updateViewMatrix();
	}
}

void Camera::onLeftMouseDown(const Point& mouse_pos)
{
}

void Camera::onRightMouseDown(const Point& mouse_pos)
{
	this->mouseDown = true;
}

void Camera::onLeftMouseUp(const Point& mouse_pos)
{
}

void Camera::onRightMouseUp(const Point& mouse_pos)
{
	this->mouseDown = false;
}
