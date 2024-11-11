#include "glm_camera.h"

GLMCameraFirstPerson::GLMCameraFirstPerson(const glm::vec3& Pos, const glm::vec3& Target,
	const glm::vec3& Up, PersProjInfo& persProjInfo)
{
	m_cameraPos = Pos;
	m_up = Up;

	float ar = (float)persProjInfo.Width / (float)persProjInfo.Height;

	if (CAMERA_LEFT_HANDED) {
		m_cameraOrientation = glm::lookAtLH(Pos, Target, Up);
		m_persProjection = glm::perspectiveLH(persProjInfo.FOV, ar,
			persProjInfo.zNear, persProjInfo.zFar);
	}
	else {
		m_cameraOrientation = glm::lookAtRH(Pos, Target, Up);
		m_persProjection = glm::perspectiveRH(persProjInfo.FOV, ar,
			persProjInfo.zNear, persProjInfo.zFar);
	}
}


void GLMCameraFirstPerson::Update(float dt)
{
	if (m_mouseState.m_buttonPressed) {
		CalcCameraOrientation();
	}

	m_oldMousePos = m_mouseState.m_pos;

	CalcVelocity(dt);

	m_cameraPos += m_velocity * dt;
}



void GLMCameraFirstPerson::CalcCameraOrientation()
{
	glm::vec2 DeltaMouse = m_mouseState.m_pos - m_oldMousePos;

	glm::quat DeltaQuat = glm::quat(glm::vec3(m_mouseSpeed * DeltaMouse.y,
		m_mouseSpeed * DeltaMouse.x, 0.0f));

	m_cameraOrientation = glm::normalize(DeltaQuat * m_cameraOrientation);

	SetUpVector();
}


void GLMCameraFirstPerson::CalcVelocity(float dt)
{
	glm::vec3 Acceleration = CalcAcceleration();

	if (Acceleration == glm::vec3(0.0f)) {
		m_velocity -= m_velocity * std::min(dt * m_damping, 1.0f);
	}
	else {
		m_velocity += Acceleration * m_acceleration * dt;
		float MaxSpeed = m_movement.FastSpeed ? m_maxSpeed * m_fastCoef : m_maxSpeed;

		if (glm::length(m_velocity) > m_maxSpeed) {
			m_velocity = glm::normalize(m_velocity) * m_maxSpeed;
		}
	}
}


glm::vec3 GLMCameraFirstPerson::CalcAcceleration()
{
	glm::mat4 v = glm::mat4_cast(m_cameraOrientation);

	glm::vec3 Forward, Up;

	glm::vec3 Right = glm::vec3(v[0][0], v[1][0], v[2][0]);

	if (CAMERA_LEFT_HANDED) {
		Forward = glm::vec3(v[0][2], v[1][2], v[2][2]);
		Up = glm::cross(Forward, Right);
	}
	else {
		Forward = -glm::vec3(v[0][2], v[1][2], v[2][2]);
		Up = glm::cross(Right, Forward);
	}

	glm::vec3 Acceleration = glm::vec3(0.0f);

	if (m_movement.Forward) {
		Acceleration += Forward;
	}

	if (m_movement.Backward) {
		Acceleration += -Forward;
	}

	if (m_movement.StrafeLeft) {
		Acceleration += -Right;
	}

	if (m_movement.StrafeRight) {
		Acceleration += Right;
	}

	if (m_movement.Up) {
		Acceleration += Up;
	}

	if (m_movement.Down) {
		Acceleration += -Up;
	}

	if (m_movement.FastSpeed) {
		Acceleration *= m_fastCoef;
	}

	return Acceleration;
}


glm::mat4 GLMCameraFirstPerson::GetViewMatrix() const
{
	glm::mat4 t = glm::translate(glm::mat4(1.0), -m_cameraPos);

	glm::mat4 r = glm::mat4_cast(m_cameraOrientation);

	glm::mat4 res = r * t;

	return res;
}


glm::mat4 GLMCameraFirstPerson::GetVPMatrix() const
{
	glm::mat4 View = GetViewMatrix();

	glm::mat4 VP = m_persProjection * View;

	return VP;
}


glm::mat4 GLMCameraFirstPerson::GetVPMatrixNoTranslate() const
{
	glm::mat4 View = glm::mat4_cast(m_cameraOrientation);

	glm::mat4 VP = m_persProjection * View;

	return VP;
}


void GLMCameraFirstPerson::SetUpVector()
{
	glm::mat4 View = GetViewMatrix();

	glm::vec3 Forward = glm::vec3(View[0][2], View[1][2], View[2][2]);

	if (CAMERA_LEFT_HANDED) {
		m_cameraOrientation = glm::lookAtLH(m_cameraPos, m_cameraPos + Forward, m_up);
	}
	else {
		m_cameraOrientation = glm::lookAtRH(m_cameraPos, m_cameraPos - Forward, m_up);
	}
}