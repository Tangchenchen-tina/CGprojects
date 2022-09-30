// Termm--Fall 2022

#include "A2.hpp"
#include "cs488-framework/GlErrorCheck.hpp"

#include <iostream>
using namespace std;

#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/io.hpp>
using namespace glm;

static const float ROTATESCALE = 2048;
static const float TRANSLATESCALE = 200;
static const float SCALE = 400;

//----------------------------------------------------------------------------------------
// Constructor
VertexData::VertexData() : numVertices(0), index(0) {
  positions.resize(kMaxVertices);
  colours.resize(kMaxVertices);
}

//----------------------------------------------------------------------------------------
// Constructor
A2::A2() : m_currentLineColour(vec3(0.0f)) {}

//----------------------------------------------------------------------------------------
// Destructor
A2::~A2() {}

//----------------------------------------------------------------------------------------
/*
 * Called once, at program start.
 */
void A2::init() {
  // Set the background colour.
  glClearColor(0.2, 0.5, 0.3, 1.0);

  createShaderProgram();

  glGenVertexArrays(1, &m_vao);
  glGenVertexArrays(1, &m_world_vao);

  enableVertexAttribIndices();

  generateVertexBuffers();

  mapVboDataToVertexAttributeLocation();

  initGnomonsWorld();
  initGnomonsCube();
  initCube();
  reset();
}

void A2::reset() {
  curr_button = 0;
  float midpoint_w = m_windowWidth / 2;
  float midpoint_h = m_windowHeight / 2;
  viewleft = (m_windowWidth * 0.05 - midpoint_w) / midpoint_w;
  viewright = (m_windowWidth * 0.95 - midpoint_w) / midpoint_w;
  viewbottom = (m_windowHeight * 0.95 - midpoint_h) / midpoint_h;
  viewtop = (m_windowHeight * 0.05 - midpoint_h) / midpoint_h;
  // cube
  TranslateCube = mat4(1.0f);
  LastTranslateCube = mat4(1.0f);
  RotateCube = mat4(1.0f);
  ScaleCube = mat4(1.0f);
  LastScaleCube = mat4(1.0f);
  LastRotateCube = mat4(1.0f);
  TransCube = mat4(1.0f);
  // view
  TransView = mat4(1.0f);
  LastTransView = mat4(1.0f);
}

mat4 A2::rotateCubeOp(int direction) {
  float theta = move_amount / ROTATESCALE;
  // printf("theta val %f\n", theta);
  if (direction == 0) { // left
    RotateCube = glm::rotate(LastRotateCube, theta, vec3(1, 0, 0));
  } else if (direction == 2) { // middle
    RotateCube = glm::rotate(LastRotateCube, theta, vec3(0, 1, 0));
  } else if (direction == 1) { // right
    RotateCube = glm::rotate(LastRotateCube, theta, vec3(0, 0, 1));
  }
  return RotateCube;
}

mat4 A2::scaleCubeOp(int direction) {
  float amount = 1 + move_amount / SCALE;
  if (direction == 0) {
    ScaleCube = glm::scale(LastScaleCube, vec3(amount, 1, 1));
  } else if (direction == 2) {
    ScaleCube = glm::scale(LastScaleCube, vec3(1, amount, 1));
  } else if (direction == 1) {
    ScaleCube = glm::scale(LastScaleCube, vec3(1, 1, amount));
  }
  return ScaleCube;
}

mat4 A2::translateCubeOp(int direction) {
  float amount = move_amount / TRANSLATESCALE;
  if (direction == 0) {
    TranslateCube = glm::translate(LastTranslateCube, vec3(amount, 0, 0));
  } else if (direction == 2) {
    TranslateCube = glm::translate(LastTranslateCube, vec3(0, amount, 0));
  } else if (direction == 1) {
    TranslateCube = glm::translate(LastTranslateCube, vec3(0, 0, amount));
  }
  return TranslateCube;
}

mat4 A2::rotateViewOp(int direction) {
  float theta = move_amount / ROTATESCALE;

  // printf("theta val %f\n", theta);
  if (direction == 0) { // left
    TransView = glm::rotate(LastTransView, theta, vec3(-1, 0, 0));
  } else if (direction == 2) { // middle
    TransView = glm::rotate(LastTransView, theta, vec3(0, -1, 0));
  } else if (direction == 1) { // right
    TransView = glm::rotate(LastTransView, theta, vec3(0, 0, -1));
  }
  return TransView;
}

mat4 A2::translateViewOp(int direction) {
  float amount = move_amount / TRANSLATESCALE;
  if (direction == 0) {
    TransView = glm::translate(LastTransView, vec3(-amount, 0, 0));
  } else if (direction == 2) {
    TransView = glm::translate(LastTransView, vec3(0, -amount, 0));
  } else if (direction == 1) {
    TransView = glm::translate(LastTransView, vec3(0, 0, -amount));
  }
  return TranslateCube;
}

void A2::initGnomonsWorld() {
  // GLfloat vertices[] = {
  //     0, 0, 0.2, 0, 0, 0, 0, 0.2, 0, 0, 0, 0 // world coordinate
  // };
  // GLfloat colors[] = {1,   0,   0, 1, 0, 0, 0.7, 0.5, 1,
  //                     0.7, 0.5, 1, 0, 0, 1, 0,   0,   1};
  // glBindVertexArray(m_vao);

  // glBindBuffer(GL_ARRAY_BUFFER, m_world_vbo_positions);
  // glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

  // glBindBuffer(GL_ARRAY_BUFFER, m_world_vbo_colours);
  // glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(colors), colors);
  worldCoordV.push_back(vec4(0, 0, 0, 1));
  worldCoordV.push_back(vec4(0.2, 0, 0, 1));

  worldCoordV.push_back(vec4(0, 0, 0, 1));
  worldCoordV.push_back(vec4(0, 0.2, 0, 1));

  worldCoordV.push_back(vec4(0, 0, 0, 1));
  worldCoordV.push_back(vec4(0, 0, 0.2, 1));
}

void A2::initGnomonsCube() {
  cubeCoordV.push_back(vec4(0, 0, 0, 1));
  cubeCoordV.push_back(vec4(0.2, 0, 0, 1));

  cubeCoordV.push_back(vec4(0, 0, 0, 1));
  cubeCoordV.push_back(vec4(0, 0.2, 0, 1));

  cubeCoordV.push_back(vec4(0, 0, 0, 1));
  cubeCoordV.push_back(vec4(0, 0, 0.2, 1));
}

void A2::normalize(vector<vec4> &vec) {
  for (int i = 0; i < vec.size(); i++) {
    vec[i].x = vec[i].x / vec[i].z;
    vec[i].y = vec[i].y / vec[i].z;
    vec[i].z = vec[i].z / vec[i].z;
  }
}

void A2::normalizePoint(vec4 &vec) {
  vec.x = vec.x / abs(vec.z);
  vec.y = vec.y / abs(vec.z);
  vec.z = vec.z / abs(vec.z);
}

void A2::initCube() {
  // up face
  cubeV.push_back(vec4(-1, 1, 1, 1));
  cubeV.push_back(vec4(-1, 1, -1, 1));

  cubeV.push_back(vec4(-1, 1, -1, 1));
  cubeV.push_back(vec4(1, 1, -1, 1));

  cubeV.push_back(vec4(1, 1, -1, 1));
  cubeV.push_back(vec4(1, 1, 1, 1));

  cubeV.push_back(vec4(1, 1, 1, 1));
  cubeV.push_back(vec4(-1, 1, 1, 1));

  // side face
  cubeV.push_back(vec4(-1, 1, 1, 1));
  cubeV.push_back(vec4(-1, -1, 1, 1));

  cubeV.push_back(vec4(-1, 1, -1, 1));
  cubeV.push_back(vec4(-1, -1, -1, 1));

  cubeV.push_back(vec4(1, 1, -1, 1));
  cubeV.push_back(vec4(1, -1, -1, 1));

  cubeV.push_back(vec4(1, 1, 1, 1));
  cubeV.push_back(vec4(1, -1, 1, 1));

  // botton face
  cubeV.push_back(vec4(-1, -1, 1, 1));
  cubeV.push_back(vec4(-1, -1, -1, 1));

  cubeV.push_back(vec4(-1, -1, -1, 1));
  cubeV.push_back(vec4(1, -1, -1, 1));

  cubeV.push_back(vec4(1, -1, -1, 1));
  cubeV.push_back(vec4(1, -1, 1, 1));

  cubeV.push_back(vec4(1, -1, 1, 1));
  cubeV.push_back(vec4(-1, -1, 1, 1));
}

//----------------------------------------------------------------------------------------
void A2::createShaderProgram() {
  m_shader.generateProgramObject();
  m_shader.attachVertexShader(getAssetFilePath("VertexShader.vs").c_str());
  m_shader.attachFragmentShader(getAssetFilePath("FragmentShader.fs").c_str());
  m_shader.link();
}

//----------------------------------------------------------------------------------------
// Fall 2022
void A2::enableVertexAttribIndices() {
  glBindVertexArray(m_vao);

  // Enable the attribute index location for "position" when rendering.
  GLint positionAttribLocation = m_shader.getAttribLocation("position");
  glEnableVertexAttribArray(positionAttribLocation);

  // Enable the attribute index location for "colour" when rendering.
  GLint colourAttribLocation = m_shader.getAttribLocation("colour");
  glEnableVertexAttribArray(colourAttribLocation);

  // World
  glBindVertexArray(m_world_vao);
  glEnableVertexAttribArray(positionAttribLocation);
  glEnableVertexAttribArray(colourAttribLocation);

  // Restore defaults
  glBindVertexArray(0);

  CHECK_GL_ERRORS;
}

//----------------------------------------------------------------------------------------
void A2::generateVertexBuffers() {
  // Generate a vertex buffer to store line vertex positions
  {
    glGenBuffers(1, &m_vbo_positions);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo_positions);

    // Set to GL_DYNAMIC_DRAW because the data store will be modified
    // frequently.
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * kMaxVertices, nullptr,
                 GL_DYNAMIC_DRAW);

    // Unbind the target GL_ARRAY_BUFFER, now that we are finished using it.
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    CHECK_GL_ERRORS;
  }

  // Generate a vertex buffer to store line colors
  {
    glGenBuffers(1, &m_vbo_colours);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo_colours);

    // Set to GL_DYNAMIC_DRAW because the data store will be modified
    // frequently.
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * kMaxVertices, nullptr,
                 GL_DYNAMIC_DRAW);

    // Unbind the target GL_ARRAY_BUFFER, now that we are finished using it.
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    CHECK_GL_ERRORS;
  }

  // Generate a vertex buffer to store line vertex positions
  {
    glGenBuffers(1, &m_world_vbo_positions);

    glBindBuffer(GL_ARRAY_BUFFER, m_world_vbo_positions);

    // Set to GL_DYNAMIC_DRAW because the data store will be modified
    // frequently.
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * kMaxVertices, nullptr,
                 GL_DYNAMIC_DRAW);

    // Unbind the target GL_ARRAY_BUFFER, now that we are finished using it.
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    CHECK_GL_ERRORS;
  }

  // Generate a vertex buffer to store line colors
  {
    glGenBuffers(1, &m_world_vbo_colours);

    glBindBuffer(GL_ARRAY_BUFFER, m_world_vbo_colours);

    // Set to GL_DYNAMIC_DRAW because the data store will be modified
    // frequently.
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * kMaxVertices, nullptr,
                 GL_DYNAMIC_DRAW);

    // Unbind the target GL_ARRAY_BUFFER, now that we are finished using it.
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    CHECK_GL_ERRORS;
  }
}

//----------------------------------------------------------------------------------------
void A2::mapVboDataToVertexAttributeLocation() {
  // Bind VAO in order to record the data mapping.
  glBindVertexArray(m_vao);

  // Tell GL how to map data from the vertex buffer "m_vbo_positions" into the
  // "position" vertex attribute index for any bound shader program.
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo_positions);
  GLint positionAttribLocation = m_shader.getAttribLocation("position");
  glVertexAttribPointer(positionAttribLocation, 2, GL_FLOAT, GL_FALSE, 0,
                        nullptr);

  // Tell GL how to map data from the vertex buffer "m_vbo_colours" into the
  // "colour" vertex attribute index for any bound shader program.
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo_colours);
  GLint colorAttribLocation = m_shader.getAttribLocation("colour");
  glVertexAttribPointer(colorAttribLocation, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

  // World vao
  glBindVertexArray(m_world_vao);

  glBindBuffer(GL_ARRAY_BUFFER, m_world_vbo_positions);
  glVertexAttribPointer(positionAttribLocation, 2, GL_FLOAT, GL_FALSE, 0,
                        nullptr);

  glBindBuffer(GL_ARRAY_BUFFER, m_world_vbo_colours);
  glVertexAttribPointer(colorAttribLocation, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

  //-- Unbind target, and restore default values:
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  CHECK_GL_ERRORS;
}

//---------------------------------------------------------------------------------------
void A2::initLineData() {
  m_vertexData.numVertices = 0;
  m_vertexData.index = 0;
}

//---------------------------------------------------------------------------------------
void A2::setLineColour(const glm::vec3 &colour) {
  m_currentLineColour = colour;
}

//---------------------------------------------------------------------------------------
void A2::drawLine(const glm::vec2 &V0, // Line Start (NDC coordinate)
                  const glm::vec2 &V1  // Line End (NDC coordinate)
) {

  m_vertexData.positions[m_vertexData.index] = V0;
  m_vertexData.colours[m_vertexData.index] = m_currentLineColour;
  ++m_vertexData.index;
  m_vertexData.positions[m_vertexData.index] = V1;
  m_vertexData.colours[m_vertexData.index] = m_currentLineColour;
  ++m_vertexData.index;

  m_vertexData.numVertices += 2;
}

//----------------------------------------------------------------------------------------
/*
 * Called once per frame, before guiLogic().
 */
void A2::appLogic() {
  // Place per frame, application logic here ...

  // Call at the beginning of frame, before drawing lines:
  initLineData();

  float scale = 0.5;

  // Draw view square:

  // printf("vewleft: %f\n", viewleft);

  setLineColour(vec3(0.2f, 1.0f, 1.0f));
  drawLine(vec2(viewleft, viewtop), vec2(viewright, viewtop));
  drawLine(vec2(viewleft, viewtop), vec2(viewleft, viewbottom));
  drawLine(vec2(viewleft, viewbottom), vec2(viewright, viewbottom));
  drawLine(vec2(viewright, viewbottom), vec2(viewright, viewtop));

  setLineColour(vec3(1.0f, 0.7f, 0.8f));
  for (int i = 0; i < cubeV.size(); i += 2) {
    vec4 firstV = TransView * ScaleCube * TranslateCube * RotateCube * cubeV[i];
    vec4 secondV = TransView * ScaleCube * TranslateCube * RotateCube * cubeV[i + 1];
    drawLine(vec2(firstV.x * scale, firstV.y * scale),
             vec2(secondV.x * scale, secondV.y * scale));
  }
  for (int i = 0; i < cubeCoordV.size(); i += 2) {
    setLineColour(vec3(i * 0.2 + 0.2f, i * 0.2 + 0.2f, i * 0.2 + 0.2f));
    vec4 firstV = TransView * ScaleCube * TranslateCube * RotateCube * cubeCoordV[i];
    vec4 secondV = TransView * ScaleCube * TranslateCube * RotateCube * cubeCoordV[i + 1];
    // normalizePoint(firstV);
    // normalizePoint(secondV);
    drawLine(vec2(firstV.x * scale, firstV.y * scale),
             vec2(secondV.x * scale, secondV.y * scale));
  }
    for (int i = 0; i < worldCoordV.size(); i += 2) {
    setLineColour(vec3(i * 0.2 + 0.2f, i * 0.2 + 0.2f, i * 0.2 + 0.2f));
    vec4 firstV = TransView  * worldCoordV[i];
    vec4 secondV = TransView * worldCoordV[i + 1];
    // normalizePoint(firstV);
    // normalizePoint(secondV);
    drawLine(vec2(firstV.x * scale, firstV.y * scale),
             vec2(secondV.x * scale, secondV.y * scale));
  }
}

//----------------------------------------------------------------------------------------
/*
 * Called once per frame, after appLogic(), but before the draw() method.
 */
void A2::guiLogic() {
  static bool firstRun(true);
  if (firstRun) {
    ImGui::SetNextWindowPos(ImVec2(50, 50));
    firstRun = false;
  }

  static bool showDebugWindow(true);
  ImGuiWindowFlags windowFlags(ImGuiWindowFlags_AlwaysAutoResize);
  float opacity(0.5f);

  ImGui::Begin("Properties", &showDebugWindow, ImVec2(100, 100), opacity,
               windowFlags);

  // Add more gui elements here here ...

  // Create Button, and check if it was clicked:
  if (ImGui::Button("Quit Application")) {
    glfwSetWindowShouldClose(m_window, GL_TRUE);
  }

  // rotate view
  ImGui::PushID(3);
  if (ImGui::RadioButton("Rotate View", &curr_button, 3)) {
    mode = 3;
  }
  ImGui::PopID();

    // Translate view
  ImGui::PushID(4);
  if (ImGui::RadioButton("Translate View", &curr_button, 4)) {
    mode = 4;
  }
  ImGui::PopID();

    // Perspective
  ImGui::PushID(5);
  if (ImGui::RadioButton("Perspective", &curr_button, 5)) {
    mode = 5;
  }
  ImGui::PopID();

  // Rotate Model
  ImGui::PushID(0);
  if (ImGui::RadioButton("Rotate Model", &curr_button, 0)) {
    mode = 0;
  }
  ImGui::PopID();

  ImGui::PushID(1);
  if (ImGui::RadioButton("Translate Model", &curr_button, 1)) {
    mode = 1;
  }
  ImGui::PopID();

  ImGui::PushID(2);
  if (ImGui::RadioButton("Scale Model", &curr_button, 2)) {
    mode = 2;
  }
  ImGui::PopID();

  ImGui::Text("Framerate: %.1f FPS", ImGui::GetIO().Framerate);

  ImGui::End();
}

//----------------------------------------------------------------------------------------
void A2::uploadVertexDataToVbos() {

  //-- Copy vertex position data into VBO, m_vbo_positions:
  {
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo_positions);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec2) * m_vertexData.numVertices,
                    m_vertexData.positions.data());
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    CHECK_GL_ERRORS;
  }

  //-- Copy vertex colour data into VBO, m_vbo_colours:
  {
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo_colours);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec3) * m_vertexData.numVertices,
                    m_vertexData.colours.data());
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    CHECK_GL_ERRORS;
  }
}

//----------------------------------------------------------------------------------------
/*
 * Called once per frame, after guiLogic().
 */
void A2::draw() {
  uploadVertexDataToVbos();

  glBindVertexArray(m_vao);

  m_shader.enable();
  glDrawArrays(GL_LINES, 0, m_vertexData.numVertices);
  // glBindVertexArray(m_world_vao);
  // glDrawArrays(GL_LINES, 0, 6);

  m_shader.disable();

  // Restore defaults
  glBindVertexArray(0);

  CHECK_GL_ERRORS;
}

//----------------------------------------------------------------------------------------
/*
 * Called once, after program is signaled to terminate.
 */
void A2::cleanup() {}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles cursor entering the window area events.
 */
bool A2::cursorEnterWindowEvent(int entered) {
  bool eventHandled(false);

  // Fill in with event handling code...

  return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles mouse cursor movement events.
 */
bool A2::mouseMoveEvent(double xPos, double yPos) {
  bool eventHandled(false);

  // Fill in with event handling code...
  if (ImGui::IsMouseDragging(0)) {
    move_amount = (xPos - init_mouse_pos);
    // printf("move amount %f\n", move_amount);
    if (curr_button == (int)ROTATE_MODEL) {
      rotateCubeOp(0);
    } else if (curr_button == (int)TRANSLATE_MODEL) {
      translateCubeOp(0);
    } else if (curr_button == (int)SCALE_MODEL) {
      scaleCubeOp(0);
    } else if (curr_button == (int)ROTATE_VIEW){
      rotateViewOp(0);
    }
  } else if (ImGui::IsMouseDragging(1)) {
    move_amount = (xPos - init_mouse_pos);
    if (curr_button == (int)ROTATE_MODEL) {
      rotateCubeOp(1);
    } else if (curr_button == (int)TRANSLATE_MODEL) {
      translateCubeOp(1);
    } else if (curr_button == (int)SCALE_MODEL) {
      scaleCubeOp(1);
    } else if (curr_button == (int)ROTATE_VIEW){
      rotateViewOp(1);
    }
  } else if (ImGui::IsMouseDragging(2)) {
    move_amount = (xPos - init_mouse_pos);
    if (curr_button == (int)ROTATE_MODEL) {
      rotateCubeOp(2);
    } else if (curr_button == (int)TRANSLATE_MODEL) {
      translateCubeOp(2);
    } else if (curr_button == (int)SCALE_MODEL) {
      scaleCubeOp(2);
    } else if (curr_button == (int)ROTATE_VIEW){
      rotateViewOp(2);
    }
  }

  return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles mouse button events.
 */
bool A2::mouseButtonInputEvent(int button, int actions, int mods) {
  bool eventHandled(false);

  // Fill in with event handling code...
  if (!ImGui::IsMouseHoveringAnyWindow()) {
    // The user clicked in the window.  If it's the left
    // mouse button, initiate a rotation.
    double xpos, ypos;
    if (actions == GLFW_PRESS) {
      glfwGetCursorPos(m_window, &xpos, &ypos);
      init_mouse_pos = xpos;
      LastRotateCube = RotateCube;
    }
    if (actions == GLFW_RELEASE) {
      move_amount = 0;
      LastRotateCube = RotateCube;
      LastTranslateCube = TranslateCube;
      LastScaleCube = ScaleCube;
      LastTransView = TransView;
    }
  }

  return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles mouse scroll wheel events.
 */
bool A2::mouseScrollEvent(double xOffSet, double yOffSet) {
  bool eventHandled(false);

  // Fill in with event handling code...

  return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles window resize events.
 */
bool A2::windowResizeEvent(int width, int height) {
  bool eventHandled(false);

  // Fill in with event handling code...

  return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles key input events.
 */
bool A2::keyInputEvent(int key, int action, int mods) {
  bool eventHandled(false);

  // Fill in with event handling code...

  return eventHandled;
}
