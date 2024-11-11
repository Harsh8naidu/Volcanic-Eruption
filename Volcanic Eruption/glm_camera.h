#pragma once

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "math_3d.h"

static bool constexpr CAMERA_LEFT_HANDED = true;

struct CameraMovement {
	bool Forward = false;
	bool Backward = false;
	bool StrafeLeft = false;
	bool StrafeRight = false;
	bool Up = false;
	bool Down = false;
	bool FastSpeed = false;
};

struct MouseState {
	glm::vec2 m_pos = glm::vec2(0.0f);
	bool m_buttonPressed = false;
};

class GLMCameraFirstPerson {

public:

	CameraMovement m_movement;
	MouseState m_mouseState;
	float m_acceleration = 150.0f;
	float m_damping = 5.0f;
	float m_maxSpeed = 10.0f;
	float m_fastCoef = 10.0f;
	float m_mouseSpeed = 4.0f;

	GLMCameraFirstPerson(const glm::vec3& Pos, const glm::vec3& Target,
		const glm::vec3& Up, PersProjInfo& persProjInfo);

	void Update(float dt);

	const glm::mat4& GetProjMatrix() const { return m_persProjection; }

	glm::vec3 GetPosition() const { return m_cameraPos; }

	glm::mat4 GetViewMatrix() const;

	glm::mat4 GetVPMatrix() const;

	glm::mat4 GetVPMatrixNoTranslate() const;

private:

	glm::vec3 CalcAcceleration();
	void CalcVelocity(float dt);
	void CalcCameraOrientation();
	void SetUpVector();

	glm::mat4 m_persProjection = glm::mat4(0.0);
	glm::vec3 m_cameraPos = glm::vec3(0.0f);
	glm::quat m_cameraOrientation = glm::quat(glm::vec3(0.0f));
	glm::vec3 m_velocity = glm::vec3(0.0f);
	glm::vec2 m_oldMousePos = glm::vec2(0.0f);
	glm::vec3 m_up = glm::vec3(0.0f);
};