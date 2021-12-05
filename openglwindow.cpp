#include "openglwindow.hpp"

#include <fmt/core.h>
#include <imgui.h>
#include <math.h>

#include <cppitertools/itertools.hpp>
#include <glm/fwd.hpp>
#include <glm/gtc/matrix_inverse.hpp>

#include "SDL_keycode.h"
#include "camera.hpp"
#include "core.h"

void OpenGLWindow::handleEvent(SDL_Event& event) {
  glm::ivec2 mousePosition;
  SDL_GetMouseState(&mousePosition.x, &mousePosition.y);

  if (event.type == SDL_KEYUP) {
    if (event.key.keysym.sym == SDLK_ESCAPE) {
      if (m_gameData.m_state == State::GameOver) {
        SDL_SetRelativeMouseMode(SDL_FALSE);
        SDL_ShowCursor(SDL_TRUE);
        m_relativeMouse = false;
      }
    }

    if (event.key.keysym.sym == SDLK_SPACE) {
      if (m_gameData.m_state == State::GameOver) {
        restart();
      }
      if (m_gameData.m_state == State::Initial) {
        m_gameData.m_state = State::Playing;
        m_gameTimer.restart();
      }
    }
  }

  if (event.type == SDL_MOUSEBUTTONDOWN) {
    if (m_gameData.m_state == State::Playing) {
      shoot();
    }
  }

  if (event.type == SDL_MOUSEMOTION) {
    m_mouseMovement = glm::vec2{
        m_mouseMovement.x + event.motion.xrel,
        m_mouseMovement.y + event.motion.yrel,
    };
  }
}

void OpenGLWindow::initializeGL() {
  abcg::glClearColor(1, 1, 1, 1);
  abcg::glEnable(GL_DEPTH_TEST);

  m_program =
      createProgramFromFile(getAssetsPath() + "shaders/normalmapping.vert",
                            getAssetsPath() + "shaders/normalmapping.frag");

  m_pistolModel.loadModel(m_program, getAssetsPath() + "pistol.obj",
                          getAssetsPath() + "maps/pistol_difusse.jpg",
                          getAssetsPath() + "maps/pistol_normal.jpg");

  m_targetModel.loadModel(m_program, getAssetsPath() + "target.obj",
                          getAssetsPath() + "maps/target_difusse.jpg",
                          getAssetsPath() + "maps/target_normal.jpg");

  m_roomModel.loadModel(m_program, getAssetsPath() + "room.obj",
                        getAssetsPath() + "maps/room_difusse.jpg",
                        getAssetsPath() + "maps/room_normal.jpg");

  // Initialize camera
  m_camera.initializeCamera();

  restart();
}

void OpenGLWindow::restart() {
  if (m_gameData.m_state == State::GameOver) {
    m_gameData.m_state = State::Playing;
    m_gameTimer.restart();
  } else {
    m_gameData.m_state = State::Initial;
  }

  m_gameData.m_score = 0;
  m_gameData.m_shots = 0;
  m_targets.restart();
  SDL_SetRelativeMouseMode(SDL_TRUE);
  SDL_ShowCursor(SDL_FALSE);
  m_relativeMouse = true;
}

void OpenGLWindow::render(glm::mat4 modelMatrix, Model model) {
  const auto program{m_program};
  abcg::glUseProgram(program);

  // Get location of uniform variables
  const GLint viewMatrixLoc{abcg::glGetUniformLocation(program, "viewMatrix")};
  const GLint projMatrixLoc{abcg::glGetUniformLocation(program, "projMatrix")};
  const GLint modelMatrixLoc{
      abcg::glGetUniformLocation(program, "modelMatrix")};
  const GLint normalMatrixLoc{
      abcg::glGetUniformLocation(program, "normalMatrix")};
  const GLint lightDirLoc{
      abcg::glGetUniformLocation(program, "lightDirWorldSpace")};
  const GLint shininessLoc{abcg::glGetUniformLocation(program, "shininess")};
  const GLint IaLoc{abcg::glGetUniformLocation(program, "Ia")};
  const GLint IdLoc{abcg::glGetUniformLocation(program, "Id")};
  const GLint IsLoc{abcg::glGetUniformLocation(program, "Is")};
  const GLint KaLoc{abcg::glGetUniformLocation(program, "Ka")};
  const GLint KdLoc{abcg::glGetUniformLocation(program, "Kd")};
  const GLint KsLoc{abcg::glGetUniformLocation(program, "Ks")};
  const GLint diffuseTexLoc{abcg::glGetUniformLocation(program, "diffuseTex")};
  const GLint normalTexLoc{abcg::glGetUniformLocation(program, "normalTex")};

  // Set uniform variables used by every scene object
  abcg::glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE,
                           &m_camera.m_viewMatrix[0][0]);
  abcg::glUniformMatrix4fv(projMatrixLoc, 1, GL_FALSE,
                           &m_camera.m_projMatrix[0][0]);
  abcg::glUniform1i(diffuseTexLoc, 0);
  abcg::glUniform1i(normalTexLoc, 1);

  // const auto lightDirRotated{m_lightDir};
  abcg::glUniform4fv(lightDirLoc, 1, &m_lightDir.x);
  abcg::glUniform4fv(IaLoc, 1, &m_Ia.x);
  abcg::glUniform4fv(IdLoc, 1, &m_Id.x);
  abcg::glUniform4fv(IsLoc, 1, &m_Is.x);

  // Set uniform variables of the current object
  abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &modelMatrix[0][0]);

  const auto modelViewMatrix{glm::mat3(m_camera.m_viewMatrix * modelMatrix)};
  glm::mat3 normalMatrix{glm::inverseTranspose(modelViewMatrix)};
  abcg::glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, &normalMatrix[0][0]);

  m_Ka = model.getKa();
  m_Kd = model.getKd();
  m_Ks = model.getKs();
  m_shininess = model.getShininess();
  abcg::glUniform1f(shininessLoc, m_shininess);
  abcg::glUniform4fv(KaLoc, 1, &m_Ka.x);
  abcg::glUniform4fv(KdLoc, 1, &m_Kd.x);
  abcg::glUniform4fv(KsLoc, 1, &m_Ks.x);

  model.render();

  abcg::glUseProgram(0);
}

void OpenGLWindow::paintGL() {
  update();

  abcg::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  abcg::glViewport(0, 0, m_viewportWidth, m_viewportHeight);

  // RENDER ROOM
  glm::mat4 modelMatrix{1.0f};

  // RENDER WEAPON
  // Rotation angle
  modelMatrix = glm::translate(glm::mat4{1.0f}, m_camera.m_eye);

  auto forward = glm::normalize(m_camera.m_at - m_camera.m_eye);

  // Translate horizontally
  glm::vec3 translation =
      0.4f * glm::normalize(m_camera.m_atBase - m_camera.m_eye);

  // Translate gun to right
  translation += -0.15f * glm::cross(m_camera.m_up, forward);

  // Translate gun upward
  translation += glm::vec3{0.0f, -0.1f, 0.0f};

  translation += 0.1f * forward;

  modelMatrix = glm::translate(modelMatrix, translation);

  modelMatrix = glm::scale(modelMatrix, glm::vec3(0.15f, 0.15f, 0.15f));
  // rotate weapon horizontally
  modelMatrix =
      glm::rotate(modelMatrix, -glm::radians(90.0f) + m_camera.m_gun_y_angle,
                  m_camera.m_up);

  // rotate weapon vertically
  modelMatrix = glm::rotate(modelMatrix, 0.7f * m_camera.m_gun_z_angle,
                            glm::vec3{0.0f, 0.0f, 1.0f});

  render(modelMatrix, m_pistolModel);

  if (!m_targets.m_targets.empty()) {
    for (auto target : m_targets.m_targets) {
      auto translation = m_targets.allowedTranslations.at(target);
      modelMatrix = glm::translate(glm::mat4{1.0f}, translation);
      modelMatrix = glm::scale(modelMatrix, glm::vec3{m_targetScale});

      render(modelMatrix, m_targetModel);
    }
  }

  // Rotation angle
  modelMatrix = glm::translate(glm::mat4{1.0f}, glm::vec3(0.0f, 0.0f, 1.7f));
  modelMatrix = glm::scale(modelMatrix, glm::vec3(2.0f, 1.0f, 2.0f));
  modelMatrix = glm::rotate(modelMatrix, -glm::radians(90.0f), m_camera.m_up);

  render(modelMatrix, m_roomModel);
}

void OpenGLWindow::paintUI() {
  if (m_gameData.m_state == State::Playing) {
    const auto size{ImVec2(300, 85)};
    // const auto position{ImVec2((1 - size.x), (1 - size.y))};
    const auto position{ImVec2(0, 0)};
    ImGui::SetNextWindowPos(position);
    ImGui::SetNextWindowSize(size);
    ImGuiWindowFlags flags{ImGuiWindowFlags_NoBackground |
                           ImGuiWindowFlags_NoTitleBar |
                           ImGuiWindowFlags_NoInputs};
    ImGui::Begin(" ", nullptr, flags);

    ImGui::Text("Time elapsed: %.f", m_gameTimer.elapsed());
    ImGui::End();
    flags = {ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar |
             ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoScrollbar};

    ImGui::SetNextWindowSize(
        ImVec2{(float)m_viewportWidth, (float)m_viewportHeight});
    ImGui::Begin("Aim", nullptr, flags);
    {
      ImDrawList* draw_list = ImGui::GetWindowDrawList();
      auto height = (float)m_viewportHeight / 2;
      auto width = (float)m_viewportWidth / 2;

      draw_list->AddLine(ImVec2{width - 1, height}, ImVec2{width - 5, height},
                         ImColor(0.0f, 1.0f, 0.0f));
      draw_list->AddLine(ImVec2{width + 1, height}, ImVec2{width + 5, height},
                         ImColor(0.0f, 1.0f, 0.0f));
      draw_list->AddLine(ImVec2{width, height - 1}, ImVec2{width, height - 5},
                         ImColor(0.0f, 1.0f, 0.0f));
      draw_list->AddLine(ImVec2{width, height + 1}, ImVec2{width, height + 5},
                         ImColor(0.0f, 1.0f, 0.0f));
    }

    ImGui::End();
    return;
  }
  if (m_gameData.m_state == State::Initial) {
    ImGui::SetNextWindowPos(
        ImVec2{(float)m_viewportWidth / 4, (float)m_viewportHeight / 4});
    ImGui::SetNextWindowSize(
        ImVec2{(float)m_viewportWidth / 2, (float)m_viewportHeight / 4});
    ImGui::Begin("instruction", nullptr, ImGuiWindowFlags_NoDecoration);
    {
      ImGui::Text("Duration of the game: %d seconds", GAME_DURATION_SECONDS);
      ImGui::Text("try to hit all targets in %d seconds",
                  GAME_DURATION_SECONDS);
      ImGui::Text("Press any mouse button to shot");
      ImGui::Spacing();
      ImGui::Text("press space to start");
    }
    ImGui::End();
    if (ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_Space))) {
      m_gameData.m_state = State::Playing;
      m_gameTimer.restart();
    }
    return;
  }
  if (m_gameData.m_state == State::GameOver) {
    ImGui::SetNextWindowPos(
        ImVec2{(float)m_viewportWidth / 4, (float)m_viewportHeight / 4});
    ImGui::SetNextWindowSize(
        ImVec2{(float)m_viewportWidth / 2, (float)m_viewportHeight / 4});
    ImGui::Begin("Statistics", nullptr, ImGuiWindowFlags_NoDecoration);
    {
      ImGui::Text("Shots fired: %d", m_gameData.m_shots);
      ImGui::Text("Targets hit: %d", m_gameData.m_score);
      ImGui::Text("Accuracy: %.2f%%",
                  100 * static_cast<float>(m_gameData.m_score) /
                      static_cast<float>(m_gameData.m_shots));
      ImGui::Text(
          "Targets hit per second: %.3f",
          static_cast<float>(m_gameData.m_score) / GAME_DURATION_SECONDS);

      ImGui::Spacing();
      ImGui::Spacing();
      ImGui::Text("press space to restart");
      ImGui::Text("press ESC to enable cursor");
    }
    ImGui::End();
    if (ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_Escape))) {
      SDL_SetRelativeMouseMode(SDL_FALSE);
      SDL_ShowCursor(SDL_TRUE);
      m_relativeMouse = false;
    }

    if (ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_Space))) {
      restart();
    }

    return;
  }
}

void OpenGLWindow::resizeGL(int width, int height) {
  m_viewportWidth = width;
  m_viewportHeight = height;

  m_camera.computeProjectionMatrix(width, height);
}

void OpenGLWindow::terminateGL() { abcg::glDeleteProgram(m_program); }

void OpenGLWindow::update() {
  if (m_gameData.m_state == State::GameOver ||
      m_gameData.m_state == State::Initial) {
    return;
  }

  if (m_gameTimer.elapsed() > GAME_DURATION_SECONDS) {
    m_gameData.m_state = State::GameOver;
    return;
  }

  float deltaTime{static_cast<float>(getDeltaTime())};

  if (m_mouseMovement.x == 0 && m_mouseMovement.y == 0) return;
  glm::vec2 rotationSpeed = getMouseRotationSpeed();
  m_camera.pan(rotationSpeed.x * deltaTime);
  m_camera.tilt(rotationSpeed.y * deltaTime);
}

glm::vec2 OpenGLWindow::getMouseRotationSpeed() {
  if (m_relativeMouse) {
    SDL_WarpMouseInWindow(nullptr, m_viewportWidth / 2, m_viewportHeight / 2);
  }

  float speedScale{0.6f};

  glm::vec2 mouseMovement{m_mouseMovement.x, -m_mouseMovement.y};

  m_mouseMovement = glm::vec2{0, 0};

  return glm::vec2{mouseMovement.x * speedScale, mouseMovement.y * speedScale};
}

void OpenGLWindow::shoot() {
  m_gameData.m_shots++;
  if (m_targets.m_targets.empty()) {
    return;
  }
  if (m_camera.m_at.z >= 2.0f) {
    return;
  }
  auto v = m_camera.m_at - m_camera.m_eye;
  auto targets = m_targets.m_targets;

  for (auto target : targets) {
    auto targetTranslation = m_targets.allowedTranslations.at(target);
    auto num = glm::cross(m_camera.m_eye - targetTranslation, v);
    auto distance = sqrt((num.x * num.x) + (num.y * num.y) + (num.z * num.z)) /
                    sqrt((v.x * v.x) + (v.y * v.y) + (v.z * v.z));

    if (distance < m_targetRadius) {
      m_gameData.m_score++;
      m_targets.removeTarget(target);
      break;
    }
  }
}