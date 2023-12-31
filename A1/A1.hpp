// Termm--Fall 2022

#pragma once

#include <glm/glm.hpp>

#include "cs488-framework/CS488Window.hpp"
#include "cs488-framework/OpenGLImport.hpp"
#include "cs488-framework/ShaderProgram.hpp"

#include "avatar.hpp"
#include "maze.hpp"

class A1 : public CS488Window {
public:
  A1();
  virtual ~A1();

protected:
  virtual void init() override;
  virtual void appLogic() override;
  virtual void guiLogic() override;
  virtual void draw() override;
  virtual void cleanup() override;

  virtual bool cursorEnterWindowEvent(int entered) override;
  virtual bool mouseMoveEvent(double xPos, double yPos) override;
  virtual bool mouseButtonInputEvent(int button, int actions,
                                     int mods) override;
  virtual bool mouseScrollEvent(double xOffSet, double yOffSet) override;
  virtual bool windowResizeEvent(int width, int height) override;
  virtual bool keyInputEvent(int key, int action, int mods) override;

private:
  void initGrid();
  void initCell();
  void initFloor();
  void initAvatar();
  void initState();
  void drawCell(float x, float y, float h, int offset);
  void drawFloor();
  void digMaze();
  void changeWallHeight(bool incH);
  void updateAvatar(float x, float y, bool init, bool shift);
  void updateMaze(int x,int y);

  // Fields related to the shader and uniforms.
  ShaderProgram m_shader;
  GLint P_uni;   // Uniform location for Projection matrix.
  GLint V_uni;   // Uniform location for View matrix.
  GLint M_uni;   // Uniform location for Model matrix.
  GLint col_uni; // Uniform location for cube colour.

  // Fields related to grid geometry.
  GLuint m_grid_vao; // Vertex Array Object
  GLuint m_grid_vbo; // Vertex Buffer Object

  GLuint m_floor_vao; // Vertex Array Object
  GLuint m_floor_vbo; // Vertex Buffer Object
  GLuint m_floor_ebo; // Vertex Buffer Object

  GLuint m_cell_vao; // Vertex Array Object
  GLuint m_cell_vbo; // Vertex Buffer Object
  GLuint m_cell_ebo; // Vertex Buffer Object
  GLuint m_cell_cbo; // Color Buffer Object

  GLuint m_avatar_vao; // Vertex Array Object
  GLuint m_avatar_vbo; // Vertex Buffer Object
  GLuint m_avatar_ebo; // Vertex Buffer Object

  // Matrices controlling the camera and projection.
  glm::mat4 proj;
  glm::mat4 view;

  float colour[3];
  float cell_colour[3];
  float floor_colour[3];
  float avatar_colour[3];
  int current_col;
  Avatar avatar;
  Maze m;
  float scale;
  float rotate;
  float prev_rotate;
  double mouse_pos;
  bool persistent;
  float rotate_rate;
  double previous_x;
  bool pos_dir;
  bool drag;
};
