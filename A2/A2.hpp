// Termm--Fall 2022

#pragma once

#include "cs488-framework/CS488Window.hpp"
#include "cs488-framework/OpenGLImport.hpp"
#include "cs488-framework/ShaderProgram.hpp"

#include <glm/glm.hpp>

#include <vector>

// Set a global maximum number of vertices in order to pre-allocate VBO data
// in one shot, rather than reallocating each frame.
const GLsizei kMaxVertices = 1000;

// Convenience class for storing vertex data in CPU memory.
// Data should be copied over to GPU memory via VBO storage before rendering.
class VertexData {
public:
  VertexData();

  std::vector<glm::vec2> positions;
  std::vector<glm::vec3> colours;
  GLuint index;
  GLsizei numVertices;
};

enum MODE{
	ROTATE_MODEL = 0,
	TRANSLATE_MODEL = 1,
  SCALE_MODEL = 2,
  ROTATE_VIEW = 3,
  TRANSLATE_VIEW = 4,
  PERSPECTIVE = 5,
  VIEWPORT = 6
};

class A2 : public CS488Window {
public:
  A2();
  virtual ~A2();

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

  void createShaderProgram();
  void enableVertexAttribIndices();
  void generateVertexBuffers();
  void mapVboDataToVertexAttributeLocation();
  void uploadVertexDataToVbos();

  void initLineData();

  void setLineColour(const glm::vec3 &colour);

  void drawLine(const glm::vec2 &v0, const glm::vec2 &v1);

  void initGnomonsWorld();
  void initGnomonsCube();
  void initCube();
  void initView();
  void initViewCoord();
  void initProj();
	void reset();
	void normalize(std::vector<glm::vec4> &vec);
	void normalizePoint(glm::vec4 &v);
  bool clipPoints(glm::vec4 &v1, glm::vec4 &v2);
  bool clipPoints3D(glm::vec4 &v1, glm::vec4 &v2);


  glm::mat4 rotateCubeOp(std::vector<int> idx, glm::mat4 lastRotate);
	glm::mat4 translateCubeOp(std::vector<int> idx, glm::mat4 lastRotate);
  glm::mat4 scaleCubeOp(std::vector<int> idx, glm::mat4 lastRotate);

  glm::mat4 rotateViewOp(std::vector<int> idx, glm::mat4 lastRotate);
  glm::mat4 translateViewOp(std::vector<int> idx, glm::mat4 lastRotate);
  glm::mat4 projection(std::vector<int> idx);

  void createNewView(float xPos, float yPos);

  ShaderProgram m_shader;

  GLuint m_vao;           // Vertex Array Object
  GLuint m_vbo_positions; // Vertex Buffer Object
  GLuint m_vbo_colours;   // Vertex Buffer Object

  // world
	GLuint m_world_vao;           // Vertex Array Object
  GLuint m_world_vbo_positions; // Vertex Buffer Object
  GLuint m_world_vbo_colours;   // Vertex Buffer Object

  VertexData m_vertexData;
  glm::vec3 m_currentLineColour;

  // cube
	std::vector<glm::vec4> cubeV;
	// cube coord
	std::vector<glm::vec4> cubeCoordV;
  // world coord
	std::vector<glm::vec4> worldCoordV;

  std::vector<glm::vec2> viewCoordV;

	glm::mat4 TranslateCube;
	glm::mat4 LastTranslateCube;
	glm::mat4 RotateCube;
	glm::mat4 LastRotateCube;
	glm::mat4 ScaleCube;
  glm::mat4 LastScaleCube;

	glm::mat4 TransCube;
  glm::mat4 LastTransCube;

  glm::mat4 TransView;
  glm::mat4 LastTransView;

  glm::mat4 Projection;
  glm::mat4 LastProjection;
  glm::mat4 view;

	// mouse moving
  double init_mouse_pos;
  double move_amount;

	// view scale
	float viewleft;
	float viewright;
	float viewtop;
	float viewbottom;

  float near;
  float prev_near;
  float far;
  float prev_far;
  float fov;
  float prev_fov;

	int curr_button;
	int mode = 0;

  float prev_xpos;
  float prev_ypos;

  float ratio_x;
  float ratio_y;
  float midx;
  float midy;

  float leftpos;
  float rightpos;
  float toppos;
  float bottompos;
};
