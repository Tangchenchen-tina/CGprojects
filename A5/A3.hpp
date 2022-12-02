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
#include <queue>

#include <AL/al.h>
#include <AL/alc.h>

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
  void moveLboatAnimation(vec3 speed, int mode);
  void hitAnimation(vec3 speed, int pos);
  void hitBulletAnimation();
  bool checkCollision();

  glm::mat4 m_perpsective;
  glm::mat4 m_view;

  LightSource m_light;

  //-- GL resources for mesh geometry data:
  GLuint m_vao_meshData;
  GLuint m_vbo_vertexPositions;
  GLuint m_vbo_vertexNormals;
  GLuint m_vbo_vertexUVs;
  GLint m_positionAttribLocation;
  GLint m_normalAttribLocation;
  GLint m_uvAttribLocation;
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

  // Texture Mapping
  unsigned int sandTexture;
  unsigned int characterTexture;
  void initTextureMapping();

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

  // Left Ship
  SceneNode *shipLNode;
  SceneNode *shipRNode;
  JointNode *weaponNode;
  float LMaxZ = 0.5;
  float LCurrZ = 0;
  float LMinZ = -0.5;
  int Ldir = 0; // down

  float leftbound = -9;
  float origin = 0;
  float rightbound = 7;
  vec3 curr_L_loc = vec3(0,0,0);
  float curr_L_uploc = 0;
  float upbound = 6.5;
  bool moveLeft = false;
  bool moveRight = false;
  bool moveup = false;
  bool movedown = false;
  int Lmode = 0;


  // Right Ship
  float RMaxZ = 0.5;
  float RCurrZ = 0.1;
  float RMinZ = -0.5;
  int Rdir = 1; // up

  bool near;
  bool mid;
  bool far;
  float mid_angle = -15;
  float far_angle = -40;
  float near_angle = 0;
  float curr_angle = 0;
  int curr_loc = 0; // near = 0; mid = 1; far = 2;
  vector<float> angleList = {near_angle, mid_angle, far_angle};
  vector<vec3> curr_Lship_loc;
  void updateCurrShipLoc(vec3 speed);

  //Right Ship Weapon
  float weapon_near_angle = 0;
  float weapon_mid_angle = 40;
  float weapon_far_angle = 70;
  float weapon_curr_angle = 0;

  // Right Ship Bullet
  GeometryNode * bullet;
  bool bulletout = false;
  void initBullet(float angle);
  vec3 curr_bullet_loc;

  // Music
  void loadMusicWAVfile(const char *path, ALuint source, ALuint buffer);
  void initMusicSound();
  ALCdevice * device;
  ALCcontext *context;
  ALuint moveSource;
  ALuint moveBuffer;
  ALuint hitSource;
  ALuint hitBuffer;
  ALuint backgroundSource;
  ALuint backgroundBuffer;

  float framerate;
  uint64_t startTime;
  queue<vec2> timeQueue;
  vec2 currTimeInterval;
  queue<vec3> timeMoveQueue;
  vec3 currTimeMoveInterval;
  uint64_t currTime;
  void initTimeQueue(const char * path, const char * pathmove);
  void updateTimeAnimation();


  // renderContral
  bool cuberender = true;
  bool shadowMaprender = false;
  bool particlerender = false;
  bool texturerender = true;
  bool toonrender = true;
  bool cplxLSystemrender = false;
  bool gameStart = false;

  //ViewMode
  int curr_veiw_mode = 0;
};
