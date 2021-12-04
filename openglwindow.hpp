#ifndef OPENGLWINDOW_HPP_
#define OPENGLWINDOW_HPP_

#include <glm/fwd.hpp>

#include "abcg.hpp"
#include "camera.hpp"
#include "gamedata.hpp"
#include "model.hpp"

class OpenGLWindow : public abcg::OpenGLWindow {
 protected:
  void handleEvent(SDL_Event& ev) override;
  void initializeGL() override;
  void paintGL() override;
  void paintUI() override;
  void resizeGL(int width, int height) override;
  void terminateGL() override;

 private:
  int m_viewportWidth{};
  int m_viewportHeight{};

  GameData m_gameData;

  Model m_pistolModel;
  Model m_targetModel;
  Model m_roomModel;

  float m_targetXAxis = {0.0f};
  glm::vec3 m_targetPosition{0.0f};
  float m_targetScale{0.15f};
  float m_targetOriginalRadius{10.0f};
  float m_targetRadius{m_targetScale * m_targetOriginalRadius};

  Camera m_camera;
  float m_dollySpeed{0.0f};
  float m_truckSpeed{0.0f};
  glm::vec2 m_mouseMovement{0.0f};
  bool m_relativeMouse{true};
  abcg::ElapsedTimer m_mouseTimer{};

  // Shaders
  GLuint m_program{};

  // Mapping mode
  // 0: triplanar; 1: cylindrical; 2: spherical; 3: from mesh
  // int m_mappingMode{};

  // Light and material properties
  glm::vec4 m_lightDir{-1.0f, -1.0f, -1.0f, 0.0f};
  glm::vec4 m_Ia{1.0f};
  glm::vec4 m_Id{1.0f};
  glm::vec4 m_Is{1.0f};
  glm::vec4 m_Ka{};
  glm::vec4 m_Kd{};
  glm::vec4 m_Ks{};
  float m_shininess{};

  void loadModel(std::string_view path);
  void update();
  glm::vec2 getMouseRotationSpeed();
  void render(glm::mat4 modelMatrix, Model model);

  void restart();
};

#endif