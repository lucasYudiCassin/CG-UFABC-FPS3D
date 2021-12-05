#ifndef OPENGLWINDOW_HPP_
#define OPENGLWINDOW_HPP_

#include <glm/fwd.hpp>

#include "abcg.hpp"
#include "camera.hpp"
#include "gamedata.hpp"
#include "model.hpp"
#include "targets.hpp"

class OpenGLWindow : public abcg::OpenGLWindow {
 protected:
  void handleEvent(SDL_Event &ev) override;
  void initializeGL() override;
  void paintGL() override;
  void paintUI() override;
  void resizeGL(int width, int height) override;
  void terminateGL() override;

 private:
  int m_viewportWidth{};
  int m_viewportHeight{};

  GameData m_gameData;
  Targets m_targets;

  Model m_pistolModel;
  Model m_roomModel;
  Model m_targetModel;

  float m_targetXAxis = {0.0f};
  glm::vec3 m_targetPosition{0.0f};
  float m_targetScale{0.15f};
  float m_targetRadius{0.0897};

  Camera m_camera;
  glm::vec2 m_mouseMovement{0.0f};
  bool m_relativeMouse{true};

  // Shaders
  GLuint m_program{};

  abcg::ElapsedTimer m_gameTimer{};
  static const int GAME_DURATION_SECONDS = 30;

  // Light and material properties
  glm::vec4 m_lightDir{-1.0f, -1.0f, -1.0f, 0.0f};
  glm::vec4 m_Ia{1.0f};
  glm::vec4 m_Id{1.0f};
  glm::vec4 m_Is{1.0f};
  glm::vec4 m_Ka{};
  glm::vec4 m_Kd{};
  glm::vec4 m_Ks{};
  float m_shininess{};

  // Audio
  SDL_AudioDeviceID m_deviceId;
  Uint8 *m_wavBuffer;
  SDL_AudioSpec wavSpec;
  Uint32 wavLength;

  void loadModel(std::string_view path);
  void update();
  glm::vec2 getMouseRotationSpeed();
  void render(glm::mat4 modelMatrix, Model model);

  void restart();
  void shoot();
  void playSound();
};

#endif