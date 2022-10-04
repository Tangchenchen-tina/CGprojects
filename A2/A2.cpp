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
static const float TOLORANCE = 0.001;
static const float FOVSCALE = 5;
static const float PERSCALE = 100;

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

  reset();
}

void A2::reset() {
  curr_button = 0;
  float midpoint_w = m_windowWidth / 2;
  float midpoint_h = m_windowHeight / 2;
  viewleft = (m_windowWidth * 0.05 - midpoint_w) / midpoint_w;
  viewright = (m_windowWidth * 0.95 - midpoint_w) / midpoint_w;
  viewbottom = (m_windowHeight * 0.05 - midpoint_h) / midpoint_h;
  viewtop = (m_windowHeight * 0.95 - midpoint_h) / midpoint_h;
  leftpos = m_windowWidth * 0.05;
  rightpos = m_windowWidth * 0.95;
  toppos = m_windowHeight * 0.95;
  bottompos = m_windowHeight * 0.05;

  // cube
  TranslateCube = mat4(1.0f);
  LastTranslateCube = mat4(1.0f);
  RotateCube = mat4(1.0f);
  ScaleCube = mat4(1.0f);
  LastScaleCube = mat4(1.0f);
  LastRotateCube = mat4(1.0f);
  TransCube = mat4(1.0f);
  LastTransCube = mat4(1.0f);
  // view
  TransView = mat4(1.0f);
  LastTransView = mat4(1.0f);
  // project
  Projection = mat4(1.0f);
  LastProjection = mat4(1.0f);

  near = 5;
  far = 15;
  fov = 30;
  prev_near = 5;
  prev_far = 15;
  prev_fov = 30;
  ratio_x = 0.9;
  ratio_y = 0.9;
  midx = 0;
  midy = 0;

  initGnomonsWorld();
  initGnomonsCube();
  initCube();
  initViewCoord();
  initView();
  initProj();
}

void A2::initProj() {
  float aspect =
      (float)(abs(rightpos - leftpos)) / (float)(abs(toppos - bottompos));

  float cotvalue = cos(radians(fov) / 2) / sin(radians(fov) / 2);
  Projection = mat4(cotvalue / aspect, 0, 0, 0, 0, cotvalue, 0, 0, 0, 0,
                    -(far + near) / (far - near), -1, 0, 0,
                    -2 * far * near / (far - near), 0);
}

mat4 A2::projection(vector<int> idx) {
  float aspect =
      (float)(abs(rightpos - leftpos)) / (float)(abs(toppos - bottompos));
  for (int i = 0; i < idx.size(); i++) {
    int direction = idx[i];
    if (direction == 0) { // left
      fov = prev_fov + move_amount / FOVSCALE;
      fov = clamp(fov, 5.0f, 160.0f);
    } else if (direction == 2) { // middle
      near = std::min(far - 0.1f, (float)(prev_near + move_amount / PERSCALE));
    } else if (direction == 1) { // right
      far = std::max(near + 0.1f, (float)(prev_far + move_amount / PERSCALE));
    }
    float cotvalue = cos(radians(fov) / 2) / sin(radians(fov) / 2);
    Projection = mat4(cotvalue / aspect, 0, 0, 0, 0, cotvalue, 0, 0, 0, 0,
                      -(far + near) / (far - near), -1, 0, 0,
                      -2 * far * near / (far - near), 0);
  }
  return Projection;
}

mat4 A2::rotateCubeOp(vector<int> idx, mat4 lastR) {
  mat4 lastRCube = lastR;
  for (int i = 0; i < idx.size(); i++) {
    float theta = move_amount / ROTATESCALE;
    // printf("theta val %f\n", theta);
    int direction = idx[i];
    if (direction == 0) { // left
      mat4 temp = mat4(1.0f);
      temp[1][1] = cos(theta);
      temp[1][2] = sin(theta);
      temp[2][1] = -sin(theta);
      temp[2][2] = cos(theta);
      TransCube = temp * lastRCube;
      // TransCube = glm::rotate(lastRCube, theta, vec3(1, 0, 0));
    } else if (direction == 2) { // middle
      mat4 temp = mat4(1.0f);
      temp[0][0] = cos(theta);
      temp[0][2] = sin(theta);
      temp[2][0] = -sin(theta);
      temp[2][2] = cos(theta);
      TransCube = temp * lastRCube;
      // TransCube = glm::rotate(lastRCube, theta, vec3(0, 1, 0));
    } else if (direction == 1) { // right
      mat4 temp = mat4(1.0f);
      temp[0][0] = cos(theta);
      temp[0][1] = sin(theta);
      temp[1][0] = -sin(theta);
      temp[1][1] = cos(theta);
      TransCube = temp * lastRCube;
      // TransCube = glm::rotate(lastRCube, theta, vec3(0, 0, 1));
    }
    lastRCube = TransCube;
  }
  return TransCube;
}

mat4 A2::scaleCubeOp(vector<int> idx, mat4 last) {
  mat4 lastCube = last;
  for (int i = 0; i < idx.size(); i++) {
    int direction = idx[i];
    float amount = 1 + move_amount / SCALE;
    if (amount <= 0) {
      amount = 0;
    }
    if (direction == 0) {
      mat4 temp = mat4(1.0f);
      temp[0][0] = amount;
      ScaleCube = lastCube * temp;
      // TransCube = glm::scale(lastCube, vec3(amount, 1, 1));
    } else if (direction == 2) {
      mat4 temp = mat4(1.0f);
      temp[1][1] = amount;
      ScaleCube = lastCube * temp;
      // TransCube = glm::scale(lastCube, vec3(1, amount, 1));
    } else if (direction == 1) {
      mat4 temp = mat4(1.0f);
      temp[2][2] = amount;
      ScaleCube = lastCube * temp;
      // TransCube = glm::scale(lastCube, vec3(1, 1, amount));
    }
    lastCube = ScaleCube;
  }
  return ScaleCube;
}

mat4 A2::translateCubeOp(vector<int> idx, mat4 last) {
  mat4 lastCube = last;
  for (int i = 0; i < idx.size(); i++) {
    int direction = idx[i];
    float amount = move_amount / TRANSLATESCALE;
    if (direction == 0) {
      mat4 temp = mat4(1.0f);
      temp[3][0] = amount;
      TransCube = lastCube * temp;
      // TransCube = glm::translate(lastCube, vec3(amount, 0, 0));
    } else if (direction == 2) {
      mat4 temp = mat4(1.0f);
      temp[3][1] = amount;
      TransCube = lastCube * temp;
      // TransCube = glm::translate(lastCube, vec3(0, amount, 0));
    } else if (direction == 1) {
      mat4 temp = mat4(1.0f);
      temp[3][2] = amount;
      TransCube = lastCube * temp;
      // TransCube = glm::translate(lastCube, vec3(0, 0, amount));
    }
    lastCube = TransCube;
  }
  return TransCube;
}

mat4 A2::rotateViewOp(vector<int> idx, mat4 last) {
  mat4 lastCube = last;
  for (int i = 0; i < idx.size(); i++) {
    int direction = idx[i];
    float theta = move_amount / ROTATESCALE;
    // printf("theta val %f\n", theta);
    if (direction == 0) { // left
      mat4 temp = mat4(1.0f);
      temp[1][1] = cos(theta);
      temp[1][2] = -sin(theta);
      temp[2][1] = sin(theta);
      temp[2][2] = cos(theta);
      TransView = temp * lastCube;
      // TransView = glm::rotate(lastCube, theta, vec3(-1, 0, 0));
    } else if (direction == 2) { // middle
      mat4 temp = mat4(1.0f);
      temp[0][0] = cos(theta);
      temp[0][2] = -sin(theta);
      temp[2][0] = sin(theta);
      temp[2][2] = cos(theta);
      TransView = temp * lastCube;

      // TransView = glm::rotate(lastCube, theta, vec3(0, -1, 0));
    } else if (direction == 1) { // right
      mat4 temp = mat4(1.0f);
      temp[0][0] = cos(theta);
      temp[0][1] = -sin(theta);
      temp[1][0] = sin(theta);
      temp[1][1] = cos(theta);
      TransView = temp * lastCube;

      // TransView = glm::rotate(lastCube, theta, vec3(0, 0, -1));
    }
    lastCube = TransView;
  }
  return TransView;
}

mat4 A2::translateViewOp(vector<int> idx, mat4 last) {
  mat4 lastCube = last;
  for (int i = 0; i < idx.size(); i++) {
    int direction = idx[i];
    float amount = move_amount / TRANSLATESCALE;
    if (direction == 0) {
      mat4 temp = mat4(1.0f);
      temp[3][0] = -amount;
      TransView = temp * lastCube;
      // TransView = glm::translate(lastCube, vec3(-amount, 0, 0));
    } else if (direction == 2) {
      mat4 temp = mat4(1.0f);
      temp[3][1] = -amount;
      TransView = temp * lastCube;
      // TransView = glm::translate(lastCube, vec3(0, -amount, 0));
    } else if (direction == 1) {
      mat4 temp = mat4(1.0f);
      temp[3][2] = -amount;
      TransView = temp * lastCube;
      // TransView = glm::translate(lastCube, vec3(0, 0, -amount));
    }
    lastCube = TransView;
  }

  return TransView;
}

void A2::initGnomonsWorld() {
  worldCoordV.clear();
  worldCoordV.push_back(vec4(0, 0, 0, 1));
  worldCoordV.push_back(vec4(0.5, 0, 0, 1));

  worldCoordV.push_back(vec4(0, 0, 0, 1));
  worldCoordV.push_back(vec4(0, 0.5, 0, 1));

  worldCoordV.push_back(vec4(0, 0, 0, 1));
  worldCoordV.push_back(vec4(0, 0, -0.5, 1));
}

void A2::initGnomonsCube() {
  cubeCoordV.clear();
  cubeCoordV.push_back(vec4(0, 0, 0, 1));
  cubeCoordV.push_back(vec4(0.5, 0, 0, 1));

  cubeCoordV.push_back(vec4(0, 0, 0, 1));
  cubeCoordV.push_back(vec4(0, 0.5, 0, 1));

  cubeCoordV.push_back(vec4(0, 0, 0, 1));
  cubeCoordV.push_back(vec4(0, 0, -0.5, 1));
}

void A2::normalize(vector<vec4> &vec) {
  for (int i = 0; i < vec.size(); i++) {
    vec[i].x = vec[i].x / vec[i].z;
    vec[i].y = vec[i].y / vec[i].z;
    vec[i].z = vec[i].z / vec[i].z;
  }
}

void A2::normalizePoint(vec4 &vec) {
  vec.x = vec.x / vec.w;
  vec.y = vec.y / vec.w;
  vec.z = vec.z / vec.w;
}

vec2 getCutPoint(vec4 A, vec4 B, vec2 P, vec2 norm) {
  float t = ((A.x - P.x) * norm.x + (A.y - P.y) * norm.y) /
            ((A.x - B.x) * norm.x + (A.y - B.y) * norm.y);
  vec2 cut = vec2((1 - t) * A.x + t * B.x, (1 - t) * A.y + t * B.y);
  return cut;
}

// 3D clipping, NDC Window
bool A2::clipPoints3D(glm::vec4 &v1, glm::vec4 &v2) {
  // clip left
  vector<float> planes;
  planes.push_back(v1.x + v1.w);
  planes.push_back(v2.x + v2.w);

  planes.push_back(v1.w - v1.x);
  planes.push_back(v2.w - v2.x);

  planes.push_back(v1.y + v1.w);
  planes.push_back(v2.y + v2.w);

  planes.push_back(-v1.y + v1.w);
  planes.push_back(-v2.y + v2.w);

  planes.push_back(v1.z + v1.w);
  planes.push_back(v2.z + v2.w);

  planes.push_back(-v1.z + v1.w);
  planes.push_back(-v2.z + v2.w);

  for (int i = 0; i < planes.size(); i += 2) {
    float b1 = planes[i];
    float b2 = planes[i + 1];
    if (b1 < 0 && b2 < 0) {
      return false;
    } else if (b1 >= 0 && b2 <= 0) {
      float t = b1 / (b1 - b2);
      v2 = (1 - t) * v1 + t * v2;
    } else if (b2 >= 0 && b1 <= 0) {
      float t = b1 / (b1 - b2);
      v1 = (1 - t) * v1 + t * v2;
    } else {
      continue;
    }
  }
  return true;
}

// 2D clipping
bool A2::clipPoints(glm::vec4 &v1, glm::vec4 &v2) {
  // printf("VEC1: %f, %f\n", v1.x, v1.y);
  // printf("VEC2: %f, %f\n", v2.x, v2.y);
  for (int i = 0; i < 8; i += 2) {
    vec2 P = viewCoordV[i];
    vec2 Q = viewCoordV[i + 1];
    vec2 norm = vec2(-(Q.y - P.y), (Q.x - P.x));
    vec2 mid = vec2((Q.x + P.x) / 2, (Q.y + P.y) / 2);
    float dist1 = (v1.x - mid.x) * norm.x + (v1.y - mid.y) * norm.y;
    float dist2 = (v2.x - mid.x) * norm.x + (v2.y - mid.y) * norm.y;
    // printf("dist1: %f, dist2 %f, idx: %d\n", dist1, dist2, i);
    if (dist1 < 0 && dist2 < 0) {
      // printf("END\n");
      return false;
    } else if (dist1 < 0) {
      vec2 cutpoint = getCutPoint(v1, v2, mid, norm);
      if (cutpoint.x >= viewleft - TOLORANCE &&
          cutpoint.x <= viewright + TOLORANCE &&
          cutpoint.y <= viewtop + TOLORANCE &&
          cutpoint.y >= viewbottom - TOLORANCE) {
        v1.x = cutpoint.x;
        v1.y = cutpoint.y;
      } else {
        continue;
      }
    } else if (dist2 < 0) {
      vec2 cutpoint = getCutPoint(v1, v2, mid, norm);
      if (cutpoint.x >= viewleft - TOLORANCE &&
          cutpoint.x <= viewright + TOLORANCE &&
          cutpoint.y <= viewtop + TOLORANCE &&
          cutpoint.y >= viewbottom - TOLORANCE) {
        v2.x = cutpoint.x;
        v2.y = cutpoint.y;
      } else {
        continue;
      }
    } else {
      continue;
    }
  }
  if ((v1.x >= viewleft - TOLORANCE && v1.x <= viewright + TOLORANCE &&
       v1.y <= viewtop + TOLORANCE && v1.y >= viewbottom - TOLORANCE) &&
      (v2.x >= viewleft - TOLORANCE && v2.x <= viewright + TOLORANCE &&
       v2.y <= viewtop + TOLORANCE && v2.y >= viewbottom - TOLORANCE)) {
    return true;
  } else {
    return false;
  }
}

void A2::createNewView(float xPos, float yPos) {
  float left, right, top, bottom;
  if (xPos > prev_xpos && yPos > prev_ypos) {
    // top-left to bottom-right
    xPos = clamp(xPos, prev_xpos, (float)m_framebufferWidth);
    yPos = clamp(yPos, prev_ypos, (float)m_framebufferHeight);
    left = prev_xpos;
    right = xPos;
    top = prev_ypos;
    bottom = yPos;
  } else if (xPos > prev_xpos && yPos < prev_ypos) {
    // buttom-left to top-right
    xPos = clamp(xPos, prev_xpos, (float)m_framebufferWidth);
    yPos = clamp(yPos, 0.0f, prev_ypos);
    left = prev_xpos;
    right = xPos;
    top = yPos;
    bottom = prev_ypos;
  } else if (xPos < prev_xpos && yPos > prev_ypos) {
    // top-right to bottom-left
    xPos = clamp(xPos, 0.0f, prev_xpos);
    yPos = clamp(yPos, prev_ypos, (float)m_framebufferHeight);
    left = xPos;
    right = prev_xpos;
    top = prev_ypos;
    bottom = yPos;
  } else if (xPos < prev_xpos && yPos < prev_ypos) {
    // bottom-right to top-left
    xPos = clamp(xPos, 0.0f, prev_xpos);
    yPos = clamp(yPos, 0.0f, prev_ypos);
    left = xPos;
    right = prev_xpos;
    top = yPos;
    bottom = prev_ypos;
  }
  ratio_x = (right - left) / m_framebufferWidth;
  ratio_y = (bottom - top) / m_framebufferHeight;
  // leftpos = left;
  // rightpos = right;
  // toppos = top;
  // bottompos = bottom;
  // vector<int> vec{3};
  // projection(vec);

  float midpoint_w = m_windowWidth / 2;
  float midpoint_h = m_windowHeight / 2;
  left = (left - midpoint_w) / midpoint_w;
  right = (right - midpoint_w) / midpoint_w;
  bottom = (m_windowHeight - bottom - midpoint_h) / midpoint_h;
  top = (m_windowHeight - top - midpoint_h) / midpoint_h;

  midx = (right + left) / 2;
  midy = (bottom + top) / 2;

  viewleft = left;
  viewright = right;
  viewbottom = bottom;
  viewtop = top;
  initViewCoord();
}

void A2::initViewCoord() {
  viewCoordV.clear();

  viewCoordV.push_back(vec2(viewleft, viewbottom));
  viewCoordV.push_back(vec2(viewright, viewbottom));

  viewCoordV.push_back(vec2(viewleft, viewtop));
  viewCoordV.push_back(vec2(viewleft, viewbottom));

  viewCoordV.push_back(vec2(viewright, viewtop));
  viewCoordV.push_back(vec2(viewleft, viewtop));

  viewCoordV.push_back(vec2(viewright, viewbottom));
  viewCoordV.push_back(vec2(viewright, viewtop));
}

void A2::initView() {

  // change basis
  // assume camera is at 0,0,6
  vec3 cubeBase1 = vec3(1.0f, 0.0f, 0.0f);
  vec3 cubeBase2 = vec3(0.0f, 1.0f, 0.0f);
  vec3 cubeBase3 = vec3(0.0f, 0.0f, 1.0f);

  vec3 viewBase1 = vec3(1.0f, 0.0f, 0.0f);
  vec3 viewBase2 = vec3(0.0f, 1.0f, 0.0f);
  vec3 viewBase3 = vec3(0.0f, 0.0f, -1.0f);
  vec3 viewBaseOrigin = vec3(0.0f, 0.0f, 10.0f);

  mat4 Matrix_CtoV =
      mat4(dot(cubeBase1, viewBase1), dot(cubeBase1, viewBase2),
           dot(cubeBase1, viewBase3), 0, dot(cubeBase2, viewBase1),
           dot(cubeBase2, viewBase2), dot(cubeBase2, viewBase3), 0,
           dot(cubeBase3, viewBase1), dot(cubeBase3, viewBase2),
           dot(cubeBase3, viewBase3), 0, dot(viewBaseOrigin, viewBase1),
           dot(viewBaseOrigin, viewBase2), dot(viewBaseOrigin, viewBase3), 1);
  view = Matrix_CtoV;
}

void A2::initCube() {
  cubeV.clear();
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

  float scale = 1;

  // Draw view square:

  // printf("vewleft: %f\n", viewleft);

  setLineColour(vec3(0.2f, 1.0f, 1.0f));
  drawLine(vec2(viewleft, viewtop), vec2(viewright, viewtop));
  drawLine(vec2(viewleft, viewtop), vec2(viewleft, viewbottom));
  drawLine(vec2(viewleft, viewbottom), vec2(viewright, viewbottom));
  drawLine(vec2(viewright, viewbottom), vec2(viewright, viewtop));

  setLineColour(vec3(1.0f, 0.7f, 0.8f));
  for (int i = 0; i < cubeV.size(); i += 2) {
    // vec4 firstV = TransView * ScaleCube * TranslateCube * RotateCube *
    // cubeV[i]; vec4 secondV = TransView * ScaleCube * TranslateCube *
    // RotateCube * cubeV[i + 1];
    vec4 firstV =
        Projection * TransView * view * TransCube * ScaleCube * cubeV[i];
    vec4 secondV =
        Projection * TransView * view * TransCube * ScaleCube * cubeV[i + 1];
    // printf("firstV: %f %f %f %f\n", firstV.x, firstV.y, firstV.z, firstV.w);
    if (clipPoints3D(firstV, secondV)) {

      normalizePoint(firstV);
      normalizePoint(secondV);

      // printf("firstV.x, y %f  %f\n", firstV.x,firstV.y);
      // printf("Second.x, y %f   %f\n", secondV.x,secondV.y);
      // printf("radio x %f, radio y %f\n", ratio_x, ratio_y);

      firstV.x = firstV.x * ratio_x + midx;
      firstV.y = firstV.y * ratio_y + midy;
      secondV.x = secondV.x * ratio_x + midx;
      secondV.y = secondV.y * ratio_y + midy;

      if (clipPoints(firstV, secondV)) {
        drawLine(vec2(firstV.x, firstV.y), vec2(secondV.x, secondV.y));
      }
    }
  }
  for (int i = 0; i < cubeCoordV.size(); i += 2) {
    setLineColour(vec3(i * 0.2 + 0.2f, i * 0.2 + 0.2f, i * 0.2 + 0.2f));
    // vec4 firstV = TransView * ScaleCube * TranslateCube * RotateCube *
    // cubeCoordV[i]; vec4 secondV = TransView * ScaleCube * TranslateCube *
    // RotateCube * cubeCoordV[i + 1];
    vec4 firstV =
        Projection * TransView * view * TransCube * ScaleCube * cubeCoordV[i];
    vec4 secondV = Projection * TransView * view * TransCube * ScaleCube *
                   cubeCoordV[i + 1];
    // normalizePoint(firstV);
    // normalizePoint(secondV);36
    if (clipPoints3D(firstV, secondV)) {

      normalizePoint(firstV);
      normalizePoint(secondV);

      firstV.x = firstV.x * ratio_x + midx;
      firstV.y = firstV.y * ratio_y + midy;
      secondV.x = secondV.x * ratio_x + midx;
      secondV.y = secondV.y * ratio_y + midy;

      if (clipPoints(firstV, secondV)) {
        drawLine(vec2(firstV.x, firstV.y), vec2(secondV.x, secondV.y));
      }
    }
  }
  for (int i = 0; i < worldCoordV.size(); i += 2) { // }

    setLineColour(vec3(i * 0.2 + 0.2f, i * 0.2 + 0.2f, i * 0.2 + 0.2f));
    vec4 firstV = Projection * TransView * view * ScaleCube * worldCoordV[i];
    vec4 secondV =
        Projection * TransView * view * ScaleCube * worldCoordV[i + 1];
    // normalizePoint(firstV);
    // normalizePoint(secondV);
    normalizePoint(firstV);
    normalizePoint(secondV);
    drawLine(vec2(firstV.x * ratio_x + midx, firstV.y * ratio_y + midy),
             vec2(secondV.x * ratio_x + midx, secondV.y * ratio_y + midy));
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
  if (ImGui::Button("Quit Application (Q)")) {
    glfwSetWindowShouldClose(m_window, GL_TRUE);
  }

  if (ImGui::Button("Reset (A)")) {
    reset();
  }

  // rotate view
  ImGui::PushID(3);
  if (ImGui::RadioButton("Rotate View (O)", &curr_button, 3)) {
    mode = 3;
  }
  ImGui::PopID();

  // Translate view
  ImGui::PushID(4);
  if (ImGui::RadioButton("Translate View (E)", &curr_button, 4)) {
    mode = 4;
  }
  ImGui::PopID();

  // Perspective
  ImGui::PushID(5);
  if (ImGui::RadioButton("Perspective (P)", &curr_button, 5)) {
    mode = 5;
  }
  ImGui::PopID();

  // Rotate Model
  ImGui::PushID(0);
  if (ImGui::RadioButton("Rotate Model (R)", &curr_button, 0)) {
    mode = 0;
  }
  ImGui::PopID();

  ImGui::PushID(1);
  if (ImGui::RadioButton("Translate Model (T)", &curr_button, 1)) {
    mode = 1;
  }
  ImGui::PopID();

  ImGui::PushID(2);
  if (ImGui::RadioButton("Scale Model (S)", &curr_button, 2)) {
    mode = 2;
  }
  ImGui::PopID();

  ImGui::PushID(6);
  if (ImGui::RadioButton("ViewPort (V)", &curr_button, 6)) {
    mode = 6;
  }
  ImGui::PopID();

  ImGui::Text("Framerate: %.1f FPS", ImGui::GetIO().Framerate);

  ImGui::Text("Near: %.1f", near);
  ImGui::Text("Far: %.1f", far);

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
  vector<int> idx;

  if (ImGui::IsMouseDragging(0)) {
    idx.push_back(0);
  }
  if (ImGui::IsMouseDragging(1)) {
    idx.push_back(1);
  }
  if (ImGui::IsMouseDragging(2)) {
    idx.push_back(2);
  }

  move_amount = (xPos - init_mouse_pos);

  if (curr_button == (int)ROTATE_MODEL) {
    rotateCubeOp(idx, LastTransCube);
  }

  else if (curr_button == (int)TRANSLATE_MODEL) {
    translateCubeOp(idx, LastTransCube);
  }

  else if (curr_button == (int)SCALE_MODEL) {
    scaleCubeOp(idx, LastScaleCube);
  }

  else if (curr_button == (int)ROTATE_VIEW) {
    rotateViewOp(idx, LastTransView);
  }

  else if (curr_button == (int)TRANSLATE_VIEW) {
    translateViewOp(idx, LastTransView);
  }

  else if (curr_button == (int)PERSPECTIVE) {
    projection(idx);
  }

  if (curr_button == (int)VIEWPORT && ImGui::IsMouseDragging(0)) {
    createNewView(xPos, yPos);
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
      prev_xpos = xpos;
      prev_ypos = ypos;
    }
    if (actions == GLFW_RELEASE) {
      move_amount = 0;
      LastRotateCube = RotateCube;
      LastTranslateCube = TranslateCube;
      LastScaleCube = ScaleCube;
      LastTransView = TransView;
      LastTransCube = TransCube;
      prev_fov = fov;
      prev_near = near;
      prev_far = far;
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
  if (action == GLFW_PRESS) {
    if (key == GLFW_KEY_A) {
      reset();
      eventHandled = true;
    } else if (key == GLFW_KEY_Q) {
      glfwSetWindowShouldClose(m_window, GL_TRUE);
      eventHandled = true;
    } else if (key == GLFW_KEY_O) {
      curr_button = (int)ROTATE_VIEW;
      eventHandled = true;
    } else if (key == GLFW_KEY_E) {
      curr_button = (int)TRANSLATE_VIEW;
      eventHandled = true;
    } else if (key == GLFW_KEY_P) {
      curr_button = (int)PERSPECTIVE;
      eventHandled = true;
    } else if (key == GLFW_KEY_R) {
      curr_button = (int)ROTATE_MODEL;
      eventHandled = true;
    } else if (key == GLFW_KEY_T) {
      curr_button = (int)TRANSLATE_MODEL;
      eventHandled = true;
    } else if (key == GLFW_KEY_S) {
      curr_button = (int)SCALE_MODEL;
      eventHandled = true;
    } else if (key == GLFW_KEY_V) {
      curr_button = (int)VIEWPORT;
      eventHandled = true;
    } 
  }

  return eventHandled;
}
