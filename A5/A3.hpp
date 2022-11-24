// Termm-Fall 2022

#pragma once

#include "cs488-framework/CS488Window.hpp"
#include "cs488-framework/MeshConsolidator.hpp"
#include "cs488-framework/OpenGLImport.hpp"
#include "cs488-framework/ShaderProgram.hpp"

#include "GeometryNode.hpp"
#include "JointNode.hpp"
#include "SceneNode.hpp"
#include "ParticleSystem.hpp"

#include <glm/glm.hpp>
#include <map>
#include <memory>
#include <set>
#include <stack>
#include <unordered_map>
#include <vector>

using namespace glm;
using namespace std;

struct LightSource {
  glm::vec3 position;
  glm::vec3 rgbIntensity;
};

struct JointNodeInfo {
  float angle_x;
  float angle_y;
  float angle_z;
  mat4 transM;
  bool isSelected;
};

class A3 : public CS488Window {
public:
  A3(const std::string &luaSceneFile);
  virtual ~A3();

protected:
  virtual void init() override;
  virtual void appLogic() override;
  virtual void guiLogic() override;
  virtual void draw() override;
  virtual void cleanup() override;

  //-- Virtual callback methods
  virtual bool cursorEnterWindowEvent(int entered) override;
  virtual bool mouseMoveEvent(double xPos, double yPos) override;
  virtual bool mouseButtonInputEvent(int button, int actions,
                                     int mods) override;
  virtual bool mouseScrollEvent(double xOffSet, double yOffSet) override;
  virtual bool windowResizeEvent(int width, int height) override;
  virtual bool keyInputEvent(int key, int action, int mods) override;

  //-- One time initialization methods:
  void processLuaSceneFile(const std::string &filename);
  void createShaderProgram();
  void enableVertexShaderInputSlots();
  void uploadVertexDataToVbos(const MeshConsolidator &meshConsolidator);
  void mapVboDataToVertexShaderInputLocations();
  void initViewMatrix();
  void initLightSources();
  void initDepthTexture();
  void initShadowTexture();
  void initAnimationNodes();
  void initPerspectiveMatrix();
  void initLNodes();
  void generateChild(SceneNode *parent, LnodeInfo l);

  void uploadCommonSceneUniforms();
  void renderSceneGraph(const SceneNode &node, mat4 modeltrans,
                        mat4 modelScale);
  void renderShadowScene(const SceneNode &node, mat4 modeltrans,
                         mat4 modelScale);
  void renderDepthScene(const SceneNode &node, mat4 modeltrans,
                        mat4 modelScale);
  void renderQuadScene(const SceneNode &node, mat4 modeltrans, mat4 modelScale);
  
  void renderCubeMap();
  void renderShadow();
  void renderDepth();
  void renderParticles();

  void renderArcCircle();

  void rotateTest(SceneNode &root);
  void pickJoints();
  void movePuppetXYZ(float x_amount, float y_amount, vector<int> idx);
  void rotatePuppetXYZ(float curr_x, float curr_y);
  void rotateJoints(float xpos, float ypos, bool rotateZ, bool pos_dirX,
                    bool pos_dirY);
  void rotateHead(float xpos, float ypos);

  void initColorMap();

  void pickingJoints();
  void updateJointSet(int id);

  void resetPos();
  void resetOrin();
  void resetJoints();
  void redo();
  void undo();
  void updateUndoStack();
  void updateRedoStack();
  void apply_map(unordered_map<int, JointNodeInfo> map);
  void rotateHeadAuto();

  // Animation Updates
  void updateSphereAnimation();
  void updateShipAnimation(float Lspeed, float Rspeed);
  void updateStarAnimation();

  glm::mat4 m_perpsective;
  glm::mat4 m_view;

  LightSource m_light;

  //-- GL resources for mesh geometry data:
  GLuint m_vao_meshData;
  GLuint m_vbo_vertexPositions;
  GLuint m_vbo_vertexNormals;
  GLint m_positionAttribLocation;
  GLint m_normalAttribLocation;
  ShaderProgram m_shader;
  GLsizei shadow_width = 1024;
  GLsizei shadow_height = 1024;
  float far_plane = 100;
  float l_near = -30;
  float l_far = 100;

  //-- GL resources for trackball circle geometry:
  GLuint m_vbo_arcCircle;
  GLuint m_vao_arcCircle;
  GLint m_arc_positionAttribLocation;
  ShaderProgram m_shader_arcCircle;

  ShaderProgram m_shader_toon;
  ShaderProgram m_shader_depth;
  GLuint depthMapFBO = 0;
  GLuint depthCubemap;

  ShaderProgram m_shader_shadow;
  GLuint shadowMapFBO = 0;
  GLuint shadowMap;

  ShaderProgram m_shader_debug;

  mat4 lightProjection;

  vec3 lightPos = glm::vec3(0, 0, 5);

  // BatchInfoMap is an associative container that maps a unique MeshId to a
  // BatchInfo object. Each BatchInfo object contains an index offset and the
  // number of indices required to render the mesh with identifier MeshId.
  BatchInfoMap m_batchInfoMap;

  std::string m_luaSceneFile;

  std::shared_ptr<SceneNode> m_rootNode;

  // CubeMap
  GLuint m_vao_cubeMap;
  GLuint m_vbo_cubeMap;
  ShaderProgram m_shader_cubemap;
  unsigned int cubeTextureID;
  void initCubeMap();
  void loadCubeMap();

  // Particle System
  GLuint m_vao_particle;
  GLuint m_vbo_particle;
  GLuint m_vao_particlepos;
  GLuint m_vbo_particlepos;
  GLuint m_vao_particlecolour;
  GLuint m_vbo_particlecolour;
  ShaderProgram m_shader_particle;
  ParticleSystem particleSys;
  float g_particule_position_size_data[4 * 1000];
	float g_particule_color_data[4 * 1000];
  void initParticleSystem();
  void setupParticles();
  void updateParticles();

  // new fields
  bool Circle;
  bool ZBuffer = true;
  bool Backface_culling;
  bool Frontface_culling;
  int curr_mode;

  float initX;
  float initY;
  float currX;
  float currY;
  float prevX;
  float prevY;

  mat4 transMatrix;
  mat4 prev_transMatrix;

  mat4 rotateMatrix;
  mat4 prev_rotateMatrix;

  mat4 rotateViewMatrix;

  bool pick = false;
  bool status_change;
  bool redo_valid = false;
  bool undo_valid = false;

  JointNode *headJoint;

  vec3 last_normP;

  float angle = 0;
  bool click = false;

  std::set<JointNode *> jointsSet;
  std::map<float, int> colorMap;
  // unordered_map<int, JointNodeInfo> undo_map;
  // unordered_map<int, JointNodeInfo> redo_map;
  vector<unordered_map<int, JointNodeInfo>> undo_stack;
  vector<unordered_map<int, JointNodeInfo>> redo_stack;

  GLuint depthMap;
  // Sphere
  SceneNode *outerSphere;

  // Squird
  JointNode *NeckJoint;
  JointNode *leftUpperArmJoint;
  JointNode *leftForeArmJoint;
  JointNode *rightUpperArmJoint;
  JointNode *rightForeArmJoint;

  // Ship
  SceneNode *shipLNode;
  SceneNode *shipRNode;
  JointNode *weaponNode;
  float LMaxZ = 0.1;
  float LCurrZ = 0;
  float LMinZ = -0.1;
  int Ldir = 0; // down

  float RMaxZ = 0.1;
  float RCurrZ = 0.04;
  float RMinZ = -0.1;
  int Rdir = 1; // up

  bool near_to_mid;
  bool mid_to_far;
  bool near_to_far;
  bool far_to_mid;
  bool far_to_near;
  bool mid_to_near;
  float mid_angle = -10;
  float far_angle = -50;
  float curr_angle = 0;

  // renderContral
  bool cuberender = true;
  bool shadowMaprender = false;
};
