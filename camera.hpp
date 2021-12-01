#ifndef CAMERA_HPP_
#define CAMERA_HPP_

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

class OpenGLWindow;

class Camera {
 public:
  void initializeCamera();

  void computeViewMatrix();
  void computeProjectionMatrix(int width, int height);

  void dolly(float speed);
  void truck(float speed);
  void pan(float speed);
  void tilt(float speed);

 private:
  friend OpenGLWindow;

  float m_gun_z_angle{0};
  float m_gun_y_angle{0};

  glm::vec3 m_eye{glm::vec3(0.0f, 0.0f, 2.5f)};  // Camera position
  glm::vec3 m_at{glm::vec3(0.0f, 0.0f, 0.0f)};   // Look-at point
  glm::vec3 m_up{glm::vec3(0.0f, 1.0f, 0.0f)};   // "up" direction
  glm::vec3 m_atBase{glm::vec3(
      0.0f, 0.0f, 0.0f)};  // m_atBase - m_eye is parallel to the floor

  // Matrix to change from world space to camera space
  glm::mat4 m_viewMatrix;
  // Matrix to change from camera space to clip space
  glm::mat4 m_projMatrix;

  float m_maxDepth{};
};

#endif