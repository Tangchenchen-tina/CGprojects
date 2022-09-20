// Termm--Fall 2022

#include "A1.hpp"

#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstdio>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "cs488-framework/GlErrorCheck.hpp"

using namespace glm;
using namespace std;

static const size_t DIM = 16;
static const float SCALE = 0.5;

//----------------------------------------------------------------------------------------
// Constructor
A1::A1() : current_col(0), m(DIM) {
  cell_colour[0] = 0.5f;
  cell_colour[1] = 0.5f;
  cell_colour[2] = 0.5f;
  floor_colour[0] = 0.2f;
  floor_colour[1] = 0.2f;
  floor_colour[2] = 0.2f;
  avatar_colour[0] = 0.7f;
  avatar_colour[1] = 0.7f;
  avatar_colour[2] = 0.7f;

  colour[0] = floor_colour[0];
  colour[1] = floor_colour[1];
  colour[2] = floor_colour[2];
}

//----------------------------------------------------------------------------------------
// Destructor
A1::~A1() {}

//----------------------------------------------------------------------------------------
/*
 * Called once, at program start.
 */
void A1::init() {
  // Initialize random number generator
  int rseed = getpid();
  srandom(rseed);
  // Print random number seed in case we want to rerun with
  // same random numbers
  cout << "Random number seed = " << rseed << endl;

  // // DELETE FROM HERE...
  // Maze m(DIM);
  // m.digMaze();
  // m.printMaze();
  // // ...TO HERE

  // Set the background colour.
  glClearColor(0.3, 0.5, 0.7, 1.0);

  // Build the shader
  m_shader.generateProgramObject();
  m_shader.attachVertexShader(getAssetFilePath("VertexShader.vs").c_str());
  m_shader.attachFragmentShader(getAssetFilePath("FragmentShader.fs").c_str());
  m_shader.link();

  // Set up the uniforms
  P_uni = m_shader.getUniformLocation("P");
  V_uni = m_shader.getUniformLocation("V");
  M_uni = m_shader.getUniformLocation("M");
  col_uni = m_shader.getUniformLocation("colour");

  initGrid();
  initCell();
  initFloor();
  initAvatar();

  // digMaze();

  // Set up initial view and projection matrices (need to do this here,
  // since it depends on the GLFW window being set up correctly).
  view = glm::lookAt(glm::vec3(0.0f, 2. * float(DIM) * 2.0 * M_SQRT1_2,
                               float(DIM) * 2.0 * M_SQRT1_2),
                     glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

  proj = glm::perspective(
      glm::radians(30.0f),
      float(m_framebufferWidth) / float(m_framebufferHeight), 1.0f, 1000.0f);
}

void A1::updateAvatar(float x_amount, float y_amount, bool init = false) {
  if (!init) {
    float x = avatar.get_x();
    float y = avatar.get_z();
    float new_x = (x + x_amount);
    float new_y = (y + y_amount);
    printf("new_x: %d\n", (int(new_x + avatar.get_r())));
    printf("float amount new_x %f\n",new_x + avatar.get_r());
    printf("new_y: %d\n", int(new_y));
    if (x_amount > 0 &&
        (m.getValue(int(new_x + avatar.get_r()), int(new_y)) == 1)) {
      if (new_x + avatar.get_r() >= int(new_x + avatar.get_r())) {
        x_amount = int(new_x+avatar.get_r())-x-avatar.get_r();
        printf("x_amount, %f\n", x_amount);
      } else {
        return;
      }
    }
    if (x_amount < 0 &&
        (m.getValue(int(new_x - avatar.get_r()), int(new_y)) == 1)) {
      return;
    }
    if (y_amount > 0 &&
        (m.getValue(int(new_x), int(new_y + avatar.get_r())) == 1)) {
      return;
    }
    if (y_amount < 0 &&
        (m.getValue(int(new_x), int(new_y + avatar.get_r())) == 1)) {
      return;
    }
  }

  avatar.moveAvatar(x_amount, y_amount, init);

  vector<GLfloat> vertices = avatar.getVertices();
  // vector<unsigned int> indices = avatar.getIndices();

  GLfloat v[avatar.getVertices().size()];
  // unsigned int idx[avatar.getIndices().size()];

  copy(vertices.begin(), vertices.end(), v);
  // copy(indices.begin(), indices.end(), idx);

  glBindBuffer(GL_ARRAY_BUFFER, m_avatar_vbo);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(v), v);
  // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
  // (void
  // *)0); glEnableVertexAttribArray(0);

  // // indices
  // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_avatar_ebo);
  // glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(idx), idx);

  CHECK_GL_ERRORS;
}

void A1::digMaze() {
  m.digMaze();
  m.printMaze();
  int offset = 0;
  for (int x = 0; x < DIM; ++x) {
    for (int y = 0; y < DIM; ++y) {
      if (m.getValue(x, y) == 1) {
        drawCell(x, y, 1, offset);
        offset += 8;
      }
      if (x == 0 && m.getValue(x, y) == 0) {
        updateAvatar(x + 0.5, y + 0.5, true);
      }
    }
  }
}

void A1::initAvatar() {
  Avatar sphere(0.5, 0.5, 0.5, 0.5, 18, 10);
  avatar.updateInfo(sphere);

  glGenVertexArrays(1, &m_avatar_vao);

  vector<GLfloat> vertices = avatar.getVertices();
  vector<unsigned int> indices = avatar.getIndices();

  GLfloat v[avatar.getVertices().size()];
  unsigned int idx[avatar.getIndices().size()];

  copy(vertices.begin(), vertices.end(), v);
  copy(indices.begin(), indices.end(), idx);

  glGenBuffers(1, &m_avatar_vbo);
  glGenBuffers(1, &m_avatar_ebo);

  glBindVertexArray(m_avatar_vao);

  glBindBuffer(GL_ARRAY_BUFFER, m_avatar_vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(v), v, GL_DYNAMIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_avatar_ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(idx), idx, GL_DYNAMIC_DRAW);

  CHECK_GL_ERRORS;
}

void A1::initFloor() {
  glGenVertexArrays(1, &m_floor_vao);
  glGenBuffers(1, &m_floor_vbo);
  glGenBuffers(1, &m_floor_ebo);
  glBindBuffer(GL_ARRAY_BUFFER, m_floor_vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * 4, NULL, GL_DYNAMIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_floor_ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * 3 * 6, NULL,
               GL_DYNAMIC_DRAW);
  drawFloor();
}

void A1::drawFloor() {
  GLfloat vertices[] = {0, 0, 0, 0, 0, DIM, DIM, 0, DIM, DIM, 0, 0};

  unsigned int indices[] = {0, 1, 2, 0, 2, 3};

  glBindVertexArray(m_floor_vao);

  // vertices
  glBindBuffer(GL_ARRAY_BUFFER, m_floor_vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  // indices
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_floor_ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_DYNAMIC_DRAW);
}

void A1::initCell() {
  glGenVertexArrays(1, &m_cell_vao);
  glGenBuffers(1, &m_cell_vbo);
  glGenBuffers(1, &m_cell_ebo);
  glGenBuffers(1, &m_cell_cbo);
  glBindBuffer(GL_ARRAY_BUFFER, m_cell_vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 24 * 256, NULL,
               GL_DYNAMIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, m_cell_cbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 24 * 256, NULL,
               GL_DYNAMIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_cell_ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * 36 * 256, NULL,
               GL_DYNAMIC_DRAW);
}

void A1::drawCell(float x, float z, float h, int offset) {
  GLfloat vertices[] = {x, 0, z, 1 + x, 0, z, 1 + x, 0, z + 1, x, 0, z + 1,
                        x, h, z, 1 + x, h, z, 1 + x, h, z + 1, x, h, z + 1};

  unsigned int indices[] = {0, 1, 2, 0, 2, 3, 4, 5, 6, 4, 6, 7,
                            0, 4, 1, 4, 5, 1, 1, 5, 6, 1, 6, 2,
                            6, 2, 3, 6, 3, 7, 3, 7, 0, 0, 4, 7};

  for (int i = 0; i < 12 * 3; i++) {
    indices[i] += offset;
  }

  glBindVertexArray(m_cell_vao);

  // vertices
  glBindBuffer(GL_ARRAY_BUFFER, m_cell_vbo);
  if (offset == 0) {
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) * 256, NULL,
                 GL_DYNAMIC_DRAW);
  }
  glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices) * offset / 8,
                  sizeof(vertices), vertices);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  // indices
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_cell_ebo);
  if (offset == 0) {
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices) * 256, NULL,
                 GL_DYNAMIC_DRAW);
  }
  glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices) * offset / 8,
                  sizeof(indices), indices);

  // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
  // (void
  // *)0); glEnableVertexAttribArray(0);

  // glBindVertexArray(m_cell_vao);
  // glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

  // glBindVertexArray( 0 );
  // glBindBuffer( GL_ARRAY_BUFFER, 0 );
  // glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

  CHECK_GL_ERRORS;
}

void A1::changeWallHeight(bool incH) {
  int length = 8 * 3 * 256;
  float scale = SCALE;
  if (!incH) {
    scale *= -1;
  }
  GLfloat vertices[length] = {};
  glBindBuffer(GL_ARRAY_BUFFER, m_cell_vbo);
  glGetBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * length, vertices);

  for (int i = 0; i < length; ++i) {
    if (int(i / 12) % 2 == 1) {
      vertices[i + 1] += scale;
      vertices[i + 4] += scale;
      vertices[i + 7] += scale;
      vertices[i + 10] += scale;
      if (vertices[i + 1] < 0 && vertices[i + 4] < 0 && vertices[i + 7] < 0 &&
          vertices[i + 10] < 0) {
        vertices[i + 1] = 0;
        vertices[i + 4] = 0;
        vertices[i + 7] = 0;
        vertices[i + 10] = 0;
      }
      i += 12;
    }
  }
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
}

void A1::initGrid() {
  size_t sz = 3 * 2 * 2 * (DIM + 3);

  float *verts = new float[sz];
  size_t ct = 0;
  for (int idx = 0; idx < DIM + 3; ++idx) {
    verts[ct] = -1;
    verts[ct + 1] = 0;
    verts[ct + 2] = idx - 1;
    verts[ct + 3] = DIM + 1;
    verts[ct + 4] = 0;
    verts[ct + 5] = idx - 1;
    ct += 6;

    verts[ct] = idx - 1;
    verts[ct + 1] = 0;
    verts[ct + 2] = -1;
    verts[ct + 3] = idx - 1;
    verts[ct + 4] = 0;
    verts[ct + 5] = DIM + 1;
    ct += 6;
  }

  // Create the vertex array to record buffer assignments.
  glGenVertexArrays(1, &m_grid_vao);
  glBindVertexArray(m_grid_vao);

  // Create the cube vertex buffer
  glGenBuffers(1, &m_grid_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, m_grid_vbo);
  glBufferData(GL_ARRAY_BUFFER, sz * sizeof(float), verts, GL_STATIC_DRAW);

  // Specify the means of extracting the position values properly.
  GLint posAttrib = m_shader.getAttribLocation("position");
  glEnableVertexAttribArray(posAttrib);
  glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

  // Reset state to prevent rogue code from messing with *my*
  // stuff!
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  // OpenGL has the buffer now, there's no need for us to keep a copy.
  delete[] verts;

  CHECK_GL_ERRORS;
}

//----------------------------------------------------------------------------------------
/*
 * Called once per frame, before guiLogic().
 */
void A1::appLogic() {
  // Place per frame, application logic here ...
  m_shader.enable();
  glEnable(GL_DEPTH_TEST);

  glBindVertexArray(m_cell_vao);
  glDrawElements(GL_TRIANGLES, 36 * 256, GL_UNSIGNED_INT, 0);

  // Draw the cubes
  // Highlight the active square.
  m_shader.disable();
}

//----------------------------------------------------------------------------------------
/*
 * Called once per frame, after appLogic(), but before the draw() method.
 */
void A1::guiLogic() {
  // We already know there's only going to be one window, so for
  // simplicity we'll store button states in static local variables.
  // If there was ever a possibility of having multiple instances of
  // A1 running simultaneously, this would break; you'd want to make
  // this into instance fields of A1.
  static bool showTestWindow(false);
  static bool showDebugWindow(true);

  ImGuiWindowFlags windowFlags(ImGuiWindowFlags_AlwaysAutoResize);
  float opacity(0.5f);

  ImGui::Begin("Debug Window", &showDebugWindow, ImVec2(100, 100), opacity,
               windowFlags);
  if (ImGui::Button("Quit Application")) {
    glfwSetWindowShouldClose(m_window, GL_TRUE);
  }

  if (ImGui::Button("Dig")) {
    // Create Maze
    digMaze();
  }

  // Eventually you'll create multiple colour widgets with
  // radio buttons.  If you use PushID/PopID to give them all
  // unique IDs, then ImGui will be able to keep them separate.
  // This is unnecessary with a single colour selector and
  // radio button, but I'm leaving it in as an example.

  // Prefixing a widget name with "##" keeps it from being
  // displayed.

  // Floor Colour
  ImGui::PushID(0);
  ImGui::ColorEdit3("##Colour", colour);
  ImGui::SameLine();
  if (ImGui::RadioButton("Floor Col", &current_col, 0)) {
    // Select this colour.
    colour[0] = floor_colour[0];
    colour[1] = floor_colour[1];
    colour[2] = floor_colour[2];
    // floor_colour[0] = colour[0];
    // floor_colour[1] = colour[1];
    // floor_colour[2] = colour[2];
  }
  ImGui::PopID();

  ImGui::PushID(1);
  ImGui::SameLine();
  if (ImGui::RadioButton("Maze Col", &current_col, 1)) {
    // Select this colour.
    colour[0] = cell_colour[0];
    colour[1] = cell_colour[1];
    colour[2] = cell_colour[2];
    // cell_colour[0] = colour[0];
    // cell_colour[1] = colour[1];
    // cell_colour[2] = colour[2];
  }
  ImGui::PopID();

  ImGui::PushID(2);
  ImGui::SameLine();
  if (ImGui::RadioButton("Avatar Col", &current_col, 2)) {
    // Select this colour.
    colour[0] = avatar_colour[0];
    colour[1] = avatar_colour[1];
    colour[2] = avatar_colour[2];
  }
  ImGui::PopID();

  /*
  // For convenience, you can uncomment this to show ImGui's massive
  // demonstration window right in your application.  Very handy for
  // browsing around to get the widget you want.  Then look in
  // shared/imgui/imgui_demo.cpp to see how it's done.
  if( ImGui::Button( "Test Window" ) ) {
    showTestWindow = !showTestWindow;
  }
  */

  ImGui::Text("Framerate: %.1f FPS", ImGui::GetIO().Framerate);

  ImGui::End();

  if (showTestWindow) {
    ImGui::ShowTestWindow(&showTestWindow);
  }
}

//----------------------------------------------------------------------------------------
/*
 * Called once per frame, after guiLogic().
 */
void A1::draw() {
  // Create a global transformation for the model (centre it).
  mat4 W;
  W = glm::translate(W, vec3(-float(DIM) / 2.0f, 0, -float(DIM) / 2.0f));

  m_shader.enable();
  glEnable(GL_DEPTH_TEST);

  glUniformMatrix4fv(P_uni, 1, GL_FALSE, value_ptr(proj));
  glUniformMatrix4fv(V_uni, 1, GL_FALSE, value_ptr(view));
  glUniformMatrix4fv(M_uni, 1, GL_FALSE, value_ptr(W));

  // Just draw the grid for now.
  glBindVertexArray(m_grid_vao);
  glUniform3f(col_uni, 1, 1, 1);
  glDrawArrays(GL_LINES, 0, (3 + DIM) * 4);

  glBindVertexArray(m_floor_vao);
  if (current_col == 0) {
    floor_colour[0] = colour[0];
    floor_colour[1] = colour[1];
    floor_colour[2] = colour[2];
  }
  glUniform3f(col_uni, floor_colour[0], floor_colour[1], floor_colour[2]);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

  glBindVertexArray(m_avatar_vao);
  if (current_col == 2) {
    avatar_colour[0] = colour[0];
    avatar_colour[1] = colour[1];
    avatar_colour[2] = colour[2];
  }
  glUniform3f(col_uni, avatar_colour[0], avatar_colour[1], avatar_colour[2]);
  glDrawElements(GL_TRIANGLES, (unsigned int)avatar.getIndices().size(),
                 GL_UNSIGNED_INT, 0);

  glBindVertexArray(m_cell_vao);
  if (current_col == 1) {
    cell_colour[0] = colour[0];
    cell_colour[1] = colour[1];
    cell_colour[2] = colour[2];
  }
  glUniform3f(col_uni, cell_colour[0], cell_colour[1], cell_colour[2]);
  glDrawElements(GL_TRIANGLES, 36 * 256, GL_UNSIGNED_INT, 0);

  // Draw the cubes
  // Highlight the active square.
  m_shader.disable();

  // Restore defaults
  glBindVertexArray(0);

  CHECK_GL_ERRORS;
}

//----------------------------------------------------------------------------------------
/*
 * Called once, after program is signaled to terminate.
 */
void A1::cleanup() {}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles cursor entering the window area events.
 */
bool A1::cursorEnterWindowEvent(int entered) {
  bool eventHandled(false);

  // Fill in with event handling code...

  return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles mouse cursor movement events.
 */
bool A1::mouseMoveEvent(double xPos, double yPos) {
  bool eventHandled(false);

  if (!ImGui::IsMouseHoveringAnyWindow()) {
    // Put some code here to handle rotations.  Probably need to
    // check whether we're *dragging*, not just moving the mouse.
    // Probably need some instance variables to track the current
    // rotation amount, and maybe the previous X position (so
    // that you can rotate relative to the *change* in X.
  }

  return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles mouse button events.
 */
bool A1::mouseButtonInputEvent(int button, int actions, int mods) {
  bool eventHandled(false);

  if (!ImGui::IsMouseHoveringAnyWindow()) {
    // The user clicked in the window.  If it's the left
    // mouse button, initiate a rotation.
  }

  return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles mouse scroll wheel events.
 */
bool A1::mouseScrollEvent(double xOffSet, double yOffSet) {
  bool eventHandled(false);

  // Zoom in or out.

  return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles window resize events.
 */
bool A1::windowResizeEvent(int width, int height) {
  bool eventHandled(false);

  // Fill in with event handling code...

  return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles key input events.
 */
bool A1::keyInputEvent(int key, int action, int mods) {
  bool eventHandled(false);

  // Fill in with event handling code...
  if (action == GLFW_PRESS) {
    // Respond to some key events.
    if (key == GLFW_KEY_Q) {
      glfwSetWindowShouldClose(m_window, GL_TRUE);
    }
    if (key == GLFW_KEY_SPACE) {
      // increase wall
      changeWallHeight(true);
    }
    if (key == GLFW_KEY_BACKSPACE) {
      // decrease wall
      changeWallHeight(false);
    }
    if (key == GLFW_KEY_UP) {
      updateAvatar(0, -0.2);
    }
    if (key == GLFW_KEY_DOWN) {
      updateAvatar(0, 0.2);
    }
    if (key == GLFW_KEY_LEFT) {
      updateAvatar(-0.2, 0);
    }
    if (key == GLFW_KEY_RIGHT) {
      updateAvatar(0.2, 0);
    }
  }

  return eventHandled;
}
