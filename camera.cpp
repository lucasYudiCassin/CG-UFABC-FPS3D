#include "camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include "core.h"

void Camera::initializeCamera() { m_maxDepth = 1500; }

void Camera::computeProjectionMatrix(int width, int height) {
  m_projMatrix = glm::mat4(1.0f);
  auto aspect{static_cast<float>(width) / static_cast<float>(height)};
  m_projMatrix =
      glm::perspective(glm::radians(70.0f), aspect, 0.1f, m_maxDepth);
}

void Camera::computeViewMatrix() {
  m_viewMatrix = glm::lookAt(m_eye, m_at, m_up);
}

void Camera::dolly(float speed) {
  if (speed == 0) return;
  // Compute forward vector (view direction)
  glm::vec3 forward = glm::normalize(m_atBase - m_eye);

  // Move eye and center forward (speed > 0) or backward (speed < 0)
  m_eye += forward * speed;
  m_at += forward * speed;
  m_atBase += forward * speed;

  computeViewMatrix();
}

void Camera::truck(float speed) {
  if (speed == 0) return;

  // Compute forward vector (view direction)
  glm::vec3 forward = glm::normalize(m_atBase - m_eye);
  // Compute vector to the left
  glm::vec3 left = glm::cross(m_up, forward);

  // Move eye and center to the left (speed < 0) or to the right (speed > 0)
  m_eye -= left * speed;
  m_at -= left * speed;
  m_atBase -= left * speed;

  computeViewMatrix();
}

void Camera::pan(float speed) {
  if (speed == 0) return;
  glm::mat4 transform{glm::mat4(1.0f)};

  // Rotate camera around its local y axis
  transform = glm::translate(transform, m_eye);
  transform = glm::rotate(transform, -speed, m_up);
  transform = glm::translate(transform, -m_eye);

  m_at = transform * glm::vec4(m_at, 1.0f);
  m_atBase = transform * glm::vec4(m_atBase, 1.0f);

  auto normalizedAt = glm::normalize(m_atBase - m_eye);
  float angle = glm::acos(glm::dot(glm::vec3{0.0f, 0.0f, 1.0f}, normalizedAt));

  if (normalizedAt.x >= 0) {
    m_gun_y_angle = angle;
  } else {
    m_gun_y_angle = -angle;
  }

  computeViewMatrix();
}

void Camera::tilt(float speed) {
  if (speed == 0) return;
  glm::mat4 transform{glm::mat4(1.0f)};

  // Compute forward vector (view direction)
  glm::vec3 forward = glm::normalize(m_at - m_eye);

  // Rotate camera around its local z axis
  transform = glm::translate(transform, m_eye);
  transform = glm::rotate(transform, -speed, glm::cross(m_up, forward));
  transform = glm::translate(transform, -m_eye);

  glm::vec3 newAtPosition = transform * glm::vec4(m_at, 1.0f);

  auto normalizedNewAt = glm::normalize(newAtPosition - m_eye);
  float newViewAngle =
      glm::acos(glm::dot(glm::normalize(m_atBase - m_eye), normalizedNewAt));

  if (newViewAngle < 1.0) {
    m_at = newAtPosition;

    if (normalizedNewAt.y >= 0) {
      m_gun_z_angle = newViewAngle;
    } else {
      m_gun_z_angle = -newViewAngle;
    }

    computeViewMatrix();
  }
}