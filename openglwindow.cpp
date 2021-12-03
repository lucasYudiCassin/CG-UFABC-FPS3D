#include "openglwindow.hpp"

#include <imgui.h>

#include <cppitertools/itertools.hpp>
#include <glm/gtc/matrix_inverse.hpp>

void OpenGLWindow::handleEvent(SDL_Event& event) {
  glm::ivec2 mousePosition;
  SDL_GetMouseState(&mousePosition.x, &mousePosition.y);

  if (event.type == SDL_KEYDOWN) {
    if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_w)
      m_dollySpeed = 1.0f;

    if (event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_s)
      m_dollySpeed = -1.0f;

    if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a)
      m_truckSpeed = -1.0f;

    if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d)
      m_truckSpeed = 1.0f;

    if (event.key.keysym.sym == SDLK_ESCAPE) {
      SDL_SetRelativeMouseMode(SDL_FALSE);
      m_relativeMouse = false;
    }
  }

  if (event.type == SDL_KEYUP) {
    if ((event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_w) &&
        m_dollySpeed > 0)
      m_dollySpeed = 0.0f;

    if ((event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_s) &&
        m_dollySpeed < 0)
      m_dollySpeed = 0.0f;

    if ((event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a) &&
        m_truckSpeed < 0)
      m_truckSpeed = 0.0f;

    if ((event.key.keysym.sym == SDLK_RIGHT ||
         event.key.keysym.sym == SDLK_d) &&
        m_truckSpeed > 0)
      m_truckSpeed = 0.0f;
  }

  if (event.type == SDL_MOUSEBUTTONDOWN) {
    SDL_SetRelativeMouseMode(SDL_TRUE);
    m_relativeMouse = true;
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

  // Create programs
  // for (const auto& name : m_shaderNames) {
  //   const auto path{getAssetsPath() + "shaders/" + name};
  //   const auto program{createProgramFromFile(path + ".vert", path +
  //   ".frag")}; m_programs.push_back(program);
  // }
  m_program =
      createProgramFromFile(getAssetsPath() + "shaders/normalmapping.vert",
                            getAssetsPath() + "shaders/normalmapping.frag");

  // Load default model
  // loadModel(getAssetsPath() + "roman_lamp.obj");
  m_model.loadModel(m_program, getAssetsPath() + "roman_lamp.obj",
                    getAssetsPath() + "maps/roman_lamp_diffuse.png",
                    getAssetsPath() + "maps/roman_lamp_normal.png");

  m_pistolModel.loadModel(m_program, getAssetsPath() + "pistol.obj",
                          getAssetsPath() + "maps/pistol_difusse.jpg",
                          getAssetsPath() + "maps/pistol_normal.jpg");

  // Initialize camera
  m_camera.initializeCamera();
}

void OpenGLWindow::restart() {
  m_gameData.m_state = State::Playing;
}

// void OpenGLWindow::loadModel(std::string_view path) {
//   m_model.terminateGL();
//   m_model.loadDiffuseTexture(getAssetsPath() +
//   "maps/roman_lamp_diffuse.png"); m_model.loadNormalTexture(getAssetsPath() +
//   "maps/roman_lamp_normal.png"); m_model.loadObj(path);
//   m_model.setupVAO(m_programs.at(m_currentProgramIndex));
//   // Use material properties from the loaded model
//   m_Ka = m_model.getKa();
//   m_Kd = m_model.getKd();
//   m_Ks = m_model.getKs();
//   m_shininess = m_model.getShininess();
// }
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

  render(modelMatrix, m_model);

  // RENDER WEAPON
  // glm::mat4 modelMatrix{1.0f};

  // Rotation angle
  modelMatrix = glm::translate(modelMatrix, m_camera.m_eye);

  auto forward = glm::normalize(m_camera.m_at - m_camera.m_eye);

  // Translate horizontally
  glm::vec3 translation =
      0.4f * glm::normalize(m_camera.m_atBase - m_camera.m_eye);

  // Translate gun to right
  translation += -0.1f * glm::cross(m_camera.m_up, forward);

  // Translate gun upward
  translation += -0.1f * m_camera.m_up;

  modelMatrix = glm::translate(modelMatrix, translation);

  modelMatrix = glm::scale(modelMatrix, glm::vec3(0.15f, 0.15f, 0.15f));
  // rotate weapon horizontally
  modelMatrix =
      glm::rotate(modelMatrix, -70.0f + m_camera.m_gun_y_angle, m_camera.m_up);

  // rotate weapon vertically
  modelMatrix = glm::rotate(modelMatrix, 0.7f * m_camera.m_gun_z_angle,
                            glm::vec3{0.0f, 0.0f, 1.0f});

  render(modelMatrix, m_pistolModel);
}

void OpenGLWindow::paintUI() { abcg::OpenGLWindow::paintUI(); }

void OpenGLWindow::resizeGL(int width, int height) {
  m_viewportWidth = width;
  m_viewportHeight = height;

  m_camera.computeProjectionMatrix(width, height);
}

void OpenGLWindow::terminateGL() {
  m_model.terminateGL();
  // for (const auto& program : m_programs) {
  //   abcg::glDeleteProgram(program);
  // }
  abcg::glDeleteProgram(m_program);
}

void OpenGLWindow::update() {
  float deltaTime{static_cast<float>(getDeltaTime())};

  m_camera.dolly(m_dollySpeed * deltaTime);
  m_camera.truck(m_truckSpeed * deltaTime);

  if (m_mouseMovement.x == 0 && m_mouseMovement.y == 0) return;
  glm::vec2 rotationSpeed = getMouseRotationSpeed();
  m_camera.pan(rotationSpeed.x * deltaTime);
  m_camera.tilt(rotationSpeed.y * deltaTime);
}

glm::vec2 OpenGLWindow::getMouseRotationSpeed() {
  if (m_relativeMouse) {
    SDL_WarpMouseInWindow(nullptr, m_viewportWidth / 2, m_viewportHeight / 2);
  }

  float speedScale{5.0f};

  glm::vec2 mouseMovement{m_mouseMovement.x, -m_mouseMovement.y};

  m_mouseMovement = glm::vec2{0, 0};

  return glm::vec2{mouseMovement.x * speedScale, mouseMovement.y * speedScale};
}