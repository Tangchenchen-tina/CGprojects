// Termm-Fall 2022

#include "A3.hpp"
#include "scene_lua.hpp"
using namespace std;

#include "GeometryNode.hpp"
#include "JointNode.hpp"
#include "Material.hpp"
#include "cs488-framework/GlErrorCheck.hpp"
#include "cs488-framework/MathUtils.hpp"
#include <glm/gtx/string_cast.hpp>

#include <imgui/imgui.h>

#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/io.hpp>
#include <glm/gtx/transform.hpp>
#include <queue>
#include <set>
#include <unordered_map>
#include <vector>

#include <climits>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <queue>
#include <random>
#include <stack>
#include <string.h>

#include <chrono>
#include <cstdint>

#include <AL/al.h>
#include <AL/alc.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

typedef uint32_t DWORD;
typedef int8_t BYTE;

using namespace glm;
using namespace std;

static bool show_gui = true;

const size_t CIRCLE_PTS = 48;
const float TRANSSCALE = 300;
const float ROTATESCALE = 100;
const float TOLORANCE = 0.01;
const float TOTALNODE = 50;
const float PARTICLEMAX = 100;
const float MAXLIFE = 25;

//----------------------------------------------------------------------------------------
// Constructor
A3::A3(const std::string &luaSceneFile)
    : m_luaSceneFile(luaSceneFile), m_positionAttribLocation(0),
      m_normalAttribLocation(0), m_vao_meshData(0), m_vbo_vertexPositions(0),
      m_vbo_vertexNormals(0), m_vao_arcCircle(0), m_vbo_arcCircle(0) {
  transMatrix = mat4(1.0);
  prev_transMatrix = mat4(1.0);
  rotateMatrix = mat4(1.0);
  prev_rotateMatrix = mat4(1.0);
  rotateViewMatrix = mat4(1.0);
  near = true;
  mid = false;
  far = false;
}

//----------------------------------------------------------------------------------------
// Destructor
A3::~A3() {
  alDeleteSources(1, &moveSource);

  device = alcGetContextsDevice(context);
  alcMakeContextCurrent(NULL);
  alcDestroyContext(context);
  alcCloseDevice(device);
}

//----------------------------------------------------------------------------------------
/*
 * Called once, at program start.
 */
void A3::init() {
  // Set the background colour.
  glClearColor(0.7, 0.7, 0.7, 0.8);

  createShaderProgram();

  glGenVertexArrays(1, &m_vao_arcCircle);
  glGenVertexArrays(1, &m_vao_meshData);
  enableVertexShaderInputSlots();

  processLuaSceneFile(m_luaSceneFile);

  // Load and decode all .obj files at once here.  You may add additional .obj
  // files to this list in order to support rendering additional mesh types. All
  // vertex positions, and normals will be extracted and stored within the
  // MeshConsolidator class.
  unique_ptr<MeshConsolidator> meshConsolidator(
      new MeshConsolidator{getAssetFilePath("cube.obj"),
                           getAssetFilePath("sphere.obj"),
                           getAssetFilePath("spherecplx.obj"),
                           getAssetFilePath("suzanne.obj"),
                           getAssetFilePath("mouth.obj"),
                           getAssetFilePath("eyeslash.obj"),
                           getAssetFilePath("nose.obj"),
                           getAssetFilePath("foot.obj"),
                           getAssetFilePath("cylinder.obj"),
                           getAssetFilePath("innerSphere2.obj"),
                           getAssetFilePath("outerSphere.obj"),
                           getAssetFilePath("ship.obj"),
                           getAssetFilePath("shipMidStripe.obj"),
                           getAssetFilePath("starbody.obj"),
                           getAssetFilePath("starmouse.obj"),
                           getAssetFilePath("starpant.obj"),
                           getAssetFilePath("plant1.obj"),
                           getAssetFilePath("plant2.obj"),
                           getAssetFilePath("weapon.obj"),
                           getAssetFilePath("squidhouse.obj")});

  // Acquire the BatchInfoMap from the MeshConsolidator.
  meshConsolidator->getBatchInfoMap(m_batchInfoMap);

  // Take all vertex data within the MeshConsolidator and upload it to VBOs on
  // the GPU.
  uploadVertexDataToVbos(*meshConsolidator);

  mapVboDataToVertexShaderInputLocations();

  initLNodes();

  initPerspectiveMatrix();

  initViewMatrix();

  initLightSources();

  updateUndoStack();

  // initDepthTexture();
  // renderDepth();

  initShadowTexture();
  renderShadow();

  initAnimationNodes();

  initCubeMap();
  loadCubeMap();

  initTextureMapping();

  initParticleSystem();
  setupParticles();

  initMusicSound();

  initTimeQueue("Assets/sound/timeslot.txt", "Assets/sound/timeslotmove.txt");
  cout << "setup particle SUCCESS" << endl;
  cout << glfwGetTime << endl;
}

uint64_t timeSinceEpochMillisec() {
  using namespace std::chrono;
  return duration_cast<milliseconds>(system_clock::now().time_since_epoch())
      .count();
}

void A3::setupParticles() {
  particleSys = ParticleSystem();
  float life = rand() % 8 + 10;
  particleSys.createParticle(0, 5, vec3(-11.2, 35.7, 55), vec3(0, -0.1, 0.1),
                             0.1, life);
  float life2 = rand() % 8 + 10;
  particleSys.createParticle(0, 5, vec3(-9.5, 35.7, 55), vec3(0, -0.1, 0.1),
                             0.1, life2);
  float life3 = rand() % 8 + 10;
  particleSys.createParticle(1, 5, vec3(11.2, 35.7, 55), vec3(0, -0.1, 0.1),
                             0.1, life3 / 5); // set mode to 1
  float life4 = rand() % 8 + 10;
  particleSys.createParticle(1, 5, vec3(9.5, 35.7, 55), vec3(0, -0.1, 0.1), 0.1,
                             life4 / 5); // set mode to 1
}

void A3::initBullet(float angle) {
  if (bullet) {
    m_rootNode.get()->remove_child(bullet);
  }
  GeometryNode *bulletnode = new GeometryNode("sphere", "bullet");
  bulletnode->material = Material(vec3(1, 0, 0), vec3(0.4, 0.4, 0.4), 10.0);
  bulletnode->scale(vec3(0.5, 0.5, 0.5));
  bulletnode->translate(vec3(10.5, 38, 55));
  bulletnode->rotate('x', angle);
  bullet = bulletnode;
  m_rootNode.get()->add_child(bullet);
}

void A3::initParticleSystem() {
  vec3 cubeVertices[] = {// right
                         vec3(1, 1, 1), vec3(1, 1, -1), vec3(1, -1, -1),
                         vec3(1, -1, -1), vec3(1, -1, 1), vec3(1, 1, 1),
                         // left
                         vec3(-1, 1, 1), vec3(-1, -1, 1), vec3(-1, -1, -1),
                         vec3(-1, -1, -1), vec3(-1, 1, -1), vec3(-1, 1, 1),

                         // Up
                         vec3(-1, 1, 1), vec3(-1, 1, -1), vec3(1, 1, -1),
                         vec3(1, 1, -1), vec3(1, 1, 1), vec3(-1, 1, 1),
                         // bottom
                         vec3(-1, -1, 1), vec3(-1, -1, -1), vec3(1, -1, -1),
                         vec3(1, -1, -1), vec3(1, -1, 1), vec3(-1, -1, 1),
                         // front
                         vec3(-1, 1, 1), vec3(1, 1, 1), vec3(1, -1, 1),
                         vec3(1, -1, 1), vec3(-1, -1, 1), vec3(-1, 1, 1),
                         // back
                         vec3(-1, 1, -1), vec3(-1, -1, -1), vec3(1, -1, -1),
                         vec3(1, -1, -1), vec3(1, 1, -1), vec3(-1, 1, -1)};

  glGenVertexArrays(1, &m_vao_particle);
  glBindVertexArray(m_vao_particle);

  GLint poslocation = m_shader_particle.getAttribLocation("position");
  glEnableVertexAttribArray(poslocation);
  GLint offsetAttrib = m_shader_particle.getAttribLocation("offsetSize");
  glEnableVertexAttribArray(offsetAttrib);
  GLint colourAttrib = m_shader_particle.getAttribLocation("colour");
  glEnableVertexAttribArray(colourAttrib);

  glGenBuffers(1, &m_vbo_particle);
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo_particle);
  glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices,
               GL_STATIC_DRAW);
  glVertexAttribPointer(poslocation, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

  glGenBuffers(1, &m_vbo_particlepos);
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo_particlepos);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * particleSys.maxParticles * 4,
               NULL, GL_STREAM_DRAW);
  glVertexAttribPointer(offsetAttrib, 4, GL_FLOAT, GL_TRUE, 0, nullptr);

  glGenBuffers(1, &m_vbo_particlecolour);
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo_particlecolour);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * particleSys.maxParticles * 4,
               NULL, GL_STREAM_DRAW);
  glVertexAttribPointer(colourAttrib, 4, GL_FLOAT, GL_TRUE, 0, nullptr);

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  CHECK_GL_ERRORS;
}

void A3::initTextureMapping() {
  glGenTextures(1, &sandTexture);
  // glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, sandTexture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  float borderColor[] = {1.0f, 1.0f, 0.0f, 1.0f};
  glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

  int width, height, nrChannels;
  unsigned char *data =
      stbi_load("Assets/texture/metal.jpg", &width, &height, &nrChannels, 0);
  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    cout << "Failed to load sand texture" << endl;
  }

  stbi_image_free(data);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void A3::initCubeMap() {
  vec3 cubeVertices[] = {// right
                         vec3(1, 1, 1), vec3(1, 1, -1), vec3(1, -1, -1),
                         vec3(1, -1, -1), vec3(1, -1, 1), vec3(1, 1, 1),
                         // left
                         vec3(-1, 1, 1), vec3(-1, -1, 1), vec3(-1, -1, -1),
                         vec3(-1, -1, -1), vec3(-1, 1, -1), vec3(-1, 1, 1),

                         // Up
                         vec3(-1, 1, 1), vec3(-1, 1, -1), vec3(1, 1, -1),
                         vec3(1, 1, -1), vec3(1, 1, 1), vec3(-1, 1, 1),
                         // bottom
                         vec3(-1, -1, 1), vec3(-1, -1, -1), vec3(1, -1, -1),
                         vec3(1, -1, -1), vec3(1, -1, 1), vec3(-1, -1, 1),
                         // front
                         vec3(-1, 1, 1), vec3(1, 1, 1), vec3(1, -1, 1),
                         vec3(1, -1, 1), vec3(-1, -1, 1), vec3(-1, 1, 1),
                         // back
                         vec3(-1, 1, -1), vec3(-1, -1, -1), vec3(1, -1, -1),
                         vec3(1, -1, -1), vec3(1, 1, -1), vec3(-1, 1, -1)};
  glGenVertexArrays(1, &m_vao_cubeMap);
  glBindVertexArray(m_vao_cubeMap);

  glGenBuffers(1, &m_vbo_cubeMap);
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo_cubeMap);
  glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices,
               GL_STATIC_DRAW);

  GLint location = m_shader_cubemap.getAttribLocation("position");
  glEnableVertexAttribArray(location);
  glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), nullptr);

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void A3::loadCubeMap() {
  vector<std::string> uwfaces = {"Assets/texture/cartoon_nonclear/right.jpg",
                                 "Assets/texture/cartoon_nonclear/left.jpg",
                                 "Assets/texture/cartoon_nonclear/top.jpg",
                                 "Assets/texture/cartoon_nonclear/bottom.jpg",
                                 "Assets/texture/cartoon_nonclear/front.jpg",
                                 "Assets/texture/cartoon_nonclear/back.jpg"};

  glGenTextures(1, &cubeTextureID);
  glBindTexture(GL_TEXTURE_CUBE_MAP, cubeTextureID);

  for (unsigned int i = 0; i < uwfaces.size(); i++) {
    int w, h, n;
    unsigned char *txt = stbi_load(uwfaces[i].c_str(), &w, &h, &n, 0);
    if (txt) {
      glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, w, h, 0,
                   GL_RGB, GL_UNSIGNED_BYTE, txt);
    } else {
      cout << "load fail!" << endl;
    }
  }
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void A3::initLNodes() {
  srand(time(NULL));
  queue<SceneNode *> queue;
  // jointsSet.clear();
  queue.emplace(m_rootNode.get());
  while (!queue.empty()) {
    SceneNode *topNode = queue.front();
    for (SceneNode *node : topNode->children) {
      if (node->m_nodeType == NodeType::GeometryNode && node->LNode) {
        LnodeInfo l = node->lInfo;
        if (l.horizontal == 0) {
          // vertical type plants
          int layers = l.layer_num;
          int angle = l.angle;
          for (int i = 0; i < layers; i++) {
            Material m = ((GeometryNode *)node)->material;
            GeometryNode *child_plant = new GeometryNode(
                ((GeometryNode *)node)->meshId,
                ((GeometryNode *)node)->m_name + "child" + std::to_string(i));
            child_plant->material = m;

            int scale = (int)7000 / layers;

            float trans_x = (float)(std::rand() % 100) / scale * 10 *
                            (float)(pow(-1, std::rand() % 2));
            float trans_z = (float)(std::rand() % 100) / scale * 10 *
                            (float)(pow(-1, std::rand() % 2));
            float rotate_x = (float)(std::rand() % angle) *
                             (float)(pow(-1, std::rand() % 2));
            float rotate_z = (float)(std::rand() % angle) *
                             (float)(pow(-1, std::rand() % 2));
            float scale_y =
                (float)((float)(std::rand() % 100) / (float)50 + 0.5);

            child_plant->scale(vec3(1, scale_y, 1));
            child_plant->rotate('x', rotate_x);
            child_plant->rotate('z', rotate_z);
            child_plant->translate(vec3(trans_x, 0, trans_z));
            node->add_child(child_plant);
          }
        } else if (l.horizontal == 1) {
          int layers = l.layer_num;
          int angle = l.angle;
          float scale = l.scale;
          float basenum = l.base_num;
          float scaleval = 300 + scale * 20;
          for (int i = 0; i < basenum; i++) {
            Material m = ((GeometryNode *)node)->material;
            GeometryNode *child_plant = new GeometryNode(
                ((GeometryNode *)node)->meshId,
                ((GeometryNode *)node)->m_name + "child" + std::to_string(i));
            child_plant->material = m;

            float trans_y = (float)(std::rand() % 1000) / 1300 * scale;

            float rotate_y =
                (float)(std::rand() % 180) * (float)(pow(-1, std::rand() % 2));
            float rotate_z = (float)(std::rand() % angle) *
                             (float)(pow(-1, std::rand() % 2));
            float rotate_x = (float)(std::rand() % angle) *
                             (float)(pow(-1, std::rand() % 2));
            float scale_y =
                (float)((float)(std::rand() % 100) / (float)scaleval + 0.3);

            child_plant->scale(vec3(1, scale_y, 1));
            child_plant->rotate('z', rotate_z);
            child_plant->rotate('y', rotate_y);
            child_plant->rotate('x', rotate_x);
            child_plant->translate(vec3(0, trans_y, 0));
            node->add_child(child_plant);

            l.layer_num -= 1;
            l.scale *= scale_y;
            l.base_num -= 1;
            l.angle = std::max(30, l.angle - 10);
            generateChild(child_plant, l);
          }
        }
      }
      queue.emplace(node);
    }
    queue.pop();
  }
}

void A3::loadMusicWAVfile(const char *path, ALuint source, ALuint buffer) {
  FILE *fp = fopen(path, "rb");
  char type[4];
  DWORD size, chunkSize;
  DWORD sampleRate, avgBytesPerSec;
  DWORD dataSize;
  short formatType, channels;
  short bytesPerSample, bitsPerSample;

  fread(type, sizeof(char), 4, fp);
  if (type[0] != 'R' || type[1] != 'I' || type[2] != 'F' || type[3] != 'F')
    cout << "Not RIFF" << endl;

  fread(&size, sizeof(DWORD), 1, fp);
  fread(type, sizeof(char), 4, fp);
  if (type[0] != 'W' || type[1] != 'A' || type[2] != 'V' || type[3] != 'E')
    cout << "Not WAVE" << endl;

  fread(type, sizeof(char), 4, fp);
  if (type[0] != 'f' || type[1] != 'm' || type[2] != 't' || type[3] != ' ')
    cout << "Not fmt" << endl;

  fread(&chunkSize, sizeof(DWORD), 1, fp);
  fread(&formatType, sizeof(short), 1, fp);
  fread(&channels, sizeof(short), 1, fp);
  fread(&sampleRate, sizeof(DWORD), 1, fp);
  fread(&avgBytesPerSec, sizeof(DWORD), 1, fp);
  fread(&bytesPerSample, sizeof(short), 1, fp);
  fread(&bitsPerSample, sizeof(short), 1, fp);

  fread(&type, sizeof(char), 4, fp);
  if (type[0] != 'd' || type[1] != 'a' || type[2] != 't' || type[3] != 'a')
    cout << "Not data" << endl;

  fread(&dataSize, sizeof(DWORD), 1, fp);

  unsigned char *buf = new unsigned char[dataSize];
  fread(buf, sizeof(BYTE), dataSize, fp);

  ALuint frequency = sampleRate;
  ALenum format = 0;
  if (bitsPerSample == 8) {
    if (channels == 1) {
      format = AL_FORMAT_MONO8;
      cout << "8" << endl;
    } else if (channels == 2) {
      format = AL_FORMAT_STEREO8;
      cout << "8" << endl;
    }
  } else if (bitsPerSample == 16) {
    if (channels == 1) {
      format = AL_FORMAT_MONO16;
    } else if (channels == 2) {
      format = AL_FORMAT_STEREO16;
    }
  }

  alBufferData(buffer, format, buf, dataSize, frequency);
  alSourcei(source, AL_BUFFER, buffer);
  fclose(fp);
}

void A3::initTimeQueue(const char *path, const char * movepath) {
  timeQueue = queue<vec2>();
  ifstream newfile;
  newfile.open(path, ios::in);
  if (newfile.is_open()) {
    int millsec;
    while (newfile >> millsec) {
      timeQueue.push(vec2(millsec - 50, millsec + 50));
    }
  }

  timeMoveQueue = queue<vec3>();
  ifstream newfile2;
  newfile2.open(movepath, ios::in);
  if(newfile2.is_open()){
    int millsec, idx;
    while(newfile2 >> millsec &&  newfile2 >> idx){
      cout<< millsec << " "<<idx;
      timeMoveQueue.push(vec3(millsec-100, millsec+100, idx));
    }
  }
}

void A3::initMusicSound() {
  alSpeedOfSound(200);
  device = alcOpenDevice(NULL);
  if (!device)
    cout << "No device" << endl;
  context = alcCreateContext(device, NULL);
  if (!alcMakeContextCurrent(context))
    cout << "no context" << endl;

  ALfloat listenerOri[] = {0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f};
  alListener3f(AL_POSITION, 0, 0, 0);
  alListener3f(AL_VELOCITY, 0, 0, 0);
  alListenerfv(AL_ORIENTATION, listenerOri);

  alGenSources((ALuint)1, &moveSource);
  alSourcei(moveSource, AL_SOURCE_RELATIVE, AL_TRUE);
  alSourcef(moveSource, AL_PITCH, 1);
  alSourcef(moveSource, AL_GAIN, 0.4);
  alSource3f(moveSource, AL_POSITION, 0, 0, 0);
  alSource3f(moveSource, AL_VELOCITY, 0, 0, 0);
  alSourcei(moveSource, AL_LOOPING, AL_FALSE);
  alGenBuffers((ALuint)1, &moveBuffer);

  loadMusicWAVfile("Assets/sound/funky-town.wav", moveSource, moveBuffer);
}

void A3::generateChild(SceneNode *parent, LnodeInfo l) {
  int layers = l.layer_num;
  int angle = l.angle;
  float scale = l.scale;
  float basenum = l.base_num;
  if (layers > 0) {
    float scaleval = 150 + scale * 20;
    for (int i = 0; i < basenum; i++) {
      Material m = ((GeometryNode *)parent)->material;
      GeometryNode *child_plant = new GeometryNode(
          ((GeometryNode *)parent)->meshId,
          ((GeometryNode *)parent)->m_name + "child" + std::to_string(i));
      child_plant->material = m;

      float trans_y = (float)(std::rand() % 1000) / 1300 * scale;

      float rotate_y =
          (float)(std::rand() % 180) * (float)(pow(-1, std::rand() % 2));
      float rotate_z =
          (float)(std::rand() % angle) * (float)(pow(-1, std::rand() % 2));
      float rotate_x =
          (float)(std::rand() % angle) * (float)(pow(-1, std::rand() % 2));
      float scale_y =
          (float)((float)(std::rand() % 100) / (float)scaleval + 0.3);

      child_plant->scale(vec3(1, scale_y, 1));
      child_plant->rotate('z', rotate_z);
      child_plant->rotate('y', rotate_y);
      child_plant->rotate('x', rotate_x);
      child_plant->translate(vec3(0, trans_y, 0));
      parent->add_child(child_plant);

      l.layer_num -= 1;
      l.scale *= scale_y;
      l.base_num -= 1;
      l.angle = std::max(30, l.angle - 20);
      generateChild(child_plant, l);
    }
  }
}

void A3::initAnimationNodes() {
  queue<SceneNode *> queue;
  // jointsSet.clear();
  queue.emplace(m_rootNode.get());
  while (!queue.empty()) {
    SceneNode *topNode = queue.front();
    for (SceneNode *node : topNode->children) {
      if (node->m_nodeType == NodeType::JointNode) {
        if (node->m_name == "neckJoint") {
          NeckJoint = (JointNode *)node;
        } else if (node->m_name == "leftUpperArmJoint") {
          leftUpperArmJoint = (JointNode *)node;
        } else if (node->m_name == "leftForeArmJoint") {
          leftForeArmJoint = (JointNode *)node;
        } else if (node->m_name == "rightUpperArmJoint") {
          rightUpperArmJoint = (JointNode *)node;
        } else if (node->m_name == "rightForeArmJoint") {
          rightForeArmJoint = (JointNode *)node;
        } else if (node->m_name == "ship2Joint") {
          weaponNode = (JointNode *)node;
        }
      } else if (node->m_nodeType == NodeType::SceneNode) {
        if (node->m_name == "outerSphereNode") {
          outerSphere = node;
        } else if (node->m_name == "shipNode1") {
          shipLNode = node;
        } else if (node->m_name == "shipNode2") {
          shipRNode = node;
        }
      }
      queue.emplace(node);
    }
    queue.pop();
  }

  curr_Lship_loc.emplace_back(vec3(-10.5, 37.5, 54));
  curr_Lship_loc.emplace_back(vec3(-10.5, 40, 51.5));
  curr_Lship_loc.emplace_back(vec3(-10.5, 42, 55));
}

void A3::resetPos() {
  transMatrix = mat4(1.0);
  prev_transMatrix = mat4(1.0);
}

void A3::resetOrin() {
  rotateMatrix = mat4(1.0);
  prev_rotateMatrix = mat4(1.0);
  initViewMatrix();
}

// TODO: clear redo undo stack
void A3::resetJoints() {
  queue<SceneNode *> queue;
  queue.emplace(m_rootNode.get());
  while (!queue.empty()) {
    SceneNode *topNode = queue.front();
    for (SceneNode *node : topNode->children) {
      if (node->m_nodeType == NodeType::JointNode) {
        JointNode *jnode = (JointNode *)node;
        node->set_transform(mat4(1.0));
        jnode->reset();
      }
      //  node->isSelected = false;
      queue.emplace(node);
    }
    queue.pop();
  }
  // jointsSet.clear();
  undo_stack.clear();
  redo_stack.clear();
  updateUndoStack();
}

void A3::apply_map(unordered_map<int, JointNodeInfo> curr_map) {
  queue<SceneNode *> queue;
  // jointsSet.clear();
  queue.emplace(m_rootNode.get());
  while (!queue.empty()) {
    SceneNode *topNode = queue.front();
    for (SceneNode *node : topNode->children) {
      if (node->m_nodeType == NodeType::JointNode) {
        JointNode *jnode = (JointNode *)node;
        int id = node->m_nodeId;
        JointNodeInfo jInfo = curr_map.at(id);
        node->trans = jInfo.transM;
        jnode->angle_x = jInfo.angle_x;
        jnode->angle_y = jInfo.angle_y;
        jnode->angle_z = jInfo.angle_z;
      }
      // node->isSelected = false;
      queue.emplace(node);
    }
    queue.pop();
  }
}

void A3::redo() {
  if (redo_stack.size() > 0) {
    unordered_map<int, JointNodeInfo> redo_map = redo_stack.back();
    redo_stack.pop_back();
    undo_stack.emplace_back(redo_map);
    apply_map(redo_map);
  }
}

void A3::undo() {
  if (undo_stack.size() > 1) {
    unordered_map<int, JointNodeInfo> undo_map = undo_stack.back();
    undo_stack.pop_back();
    redo_stack.emplace_back(undo_map);
    unordered_map<int, JointNodeInfo> curr_map = undo_stack.back();
    apply_map(curr_map);
  }
}

void A3::updateUndoStack() {
  queue<SceneNode *> queue;
  queue.emplace(m_rootNode.get());
  unordered_map<int, JointNodeInfo> undo_map;
  while (!queue.empty()) {
    SceneNode *topNode = queue.front();
    for (SceneNode *node : topNode->children) {
      if (node->m_nodeType == NodeType::JointNode) {
        JointNode *jnode = (JointNode *)node;
        JointNodeInfo jInfo{jnode->angle_x, jnode->angle_y, jnode->angle_z,
                            node->trans, node->isSelected};
        undo_map.insert({node->m_nodeId, jInfo});
        if (node->m_name == "headJoint")
          headJoint = jnode;
      }
      queue.emplace(node);
    }
    queue.pop();
  }
  undo_stack.emplace_back(undo_map);
}

void A3::initShadowTexture() {
  glGenFramebuffers(1, &shadowMapFBO);
  glGenTextures(1, &shadowMap);
  glBindTexture(GL_TEXTURE_2D, shadowMap);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadow_width,
               shadow_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  // Prevents darkness outside the frustrum
  float clampColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
  glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, clampColor);

  glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
                         shadowMap, 0);
  // Needed since we don't touch the color buffer
  glDrawBuffer(GL_NONE);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void A3::updateJointSet(int id) {
  JointNode *jnode = nullptr;
  queue<SceneNode *> queue;
  SceneNode *snode = m_rootNode.get();
  queue.emplace(snode);
  while (!queue.empty()) {
    SceneNode *topNode = queue.front();
    queue.pop();

    for (SceneNode *node : topNode->children) {
      if (node->m_nodeType == NodeType::GeometryNode) {
        if (node->m_nodeId == id &&
            topNode->m_nodeType == NodeType::JointNode) {
          //   cout << "jnode name" << topNode->m_name << endl;
          jnode = (JointNode *)topNode;
          //   cout << "id " << id << endl;
          break;
        }
        queue.emplace(node);
      } else {
        queue.emplace(node);
      }
    }
    if (jnode != nullptr) {
      break;
    }
  }

  if (jnode) {
    if (jointsSet.find(jnode) != jointsSet.end()) {
      jointsSet.erase(jnode);
      jnode->isSelected = false;
      for (SceneNode *child : jnode->children) {
        child->isSelected = false;
      }
    } else {
      jointsSet.insert(jnode);
      jnode->isSelected = true;
      for (SceneNode *child : jnode->children) {
        child->isSelected = true;
      }
    }
  }
}

void A3::pickingJoints() {
  uploadCommonSceneUniforms();
  glClearColor(1.0, 1.0, 1.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClearColor(0.85, 0.85, 0.85, 1.0);

  draw();
  pick = true;

  double xpos, ypos;
  glfwGetCursorPos(m_window, &xpos, &ypos);
  GLubyte buffer[4] = {0, 0, 0, 0};
  // A bit ugly -- don't want to swap the just-drawn false colours
  // to the screen, so read from the back buffer.
  // Actually read the pixel at the mouse location.
  xpos *= double(m_framebufferWidth) / double(m_windowWidth);
  // WTF, don't know why I have to measure y relative to the bottom of
  // the window in this case.
  ypos = m_windowHeight - ypos;
  ypos *= double(m_framebufferHeight) / double(m_windowHeight);
  glReadBuffer(GL_BACK);
  glReadPixels(int(xpos), int(ypos), 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
  // printf("xpos and ypos: %f, %f\n", xpos, ypos);
  // printf("rgb value: %d, %d, %d\n", buffer[0], buffer[1], buffer[2]);
  CHECK_GL_ERRORS;

  unsigned int node_id = buffer[0] + (buffer[1] << 8) + (buffer[2] << 16);
  // printf("node id %d \n", node_id);

  if (node_id <= 255) {
    updateJointSet(node_id);
  }
  pick = false;
}

//----------------------------------------------------------------------------------------
void A3::processLuaSceneFile(const std::string &filename) {
  // This version of the code treats the Lua file as an Asset,
  // so that you'd launch the program with just the filename
  // of a puppet in the Assets/ directory.
  // std::string assetFilePath = getAssetFilePath(filename.c_str());
  // m_rootNode = std::shared_ptr<SceneNode>(import_lua(assetFilePath));

  // This version of the code treats the main program argument
  // as a straightforward pathname.
  m_rootNode = std::shared_ptr<SceneNode>(import_lua(filename));
  if (!m_rootNode) {
    std::cerr << "Could Not Open " << filename << std::endl;
  }
}

//----------------------------------------------------------------------------------------
void A3::createShaderProgram() {
  m_shader.generateProgramObject();
  m_shader.attachVertexShader(
      getAssetFilePath("shader/VertexShader.vs").c_str());
  // m_shader.attachGeometryShader(getAssetFilePath("GeometryShader.gs").c_str());
  m_shader.attachFragmentShader(
      getAssetFilePath("shader/FragmentShader.fs").c_str());
  m_shader.link();

  m_shader_arcCircle.generateProgramObject();
  m_shader_arcCircle.attachVertexShader(
      getAssetFilePath("shader/arc_VertexShader.vs").c_str());
  m_shader_arcCircle.attachFragmentShader(
      getAssetFilePath("shader/arc_FragmentShader.fs").c_str());
  m_shader_arcCircle.link();

  // m_shader_toon.generateProgramObject();
  // m_shader_toon.attachVertexShader(getAssetFilePath("VertexShader.vs").c_str());
  // m_shader_toon.attachFragmentShader(getAssetFilePath("CartoonShader.fs").c_str());
  // m_shader_toon.link();

  m_shader_depth.generateProgramObject();
  m_shader_depth.attachVertexShader(
      getAssetFilePath("shader/DepthShader.vs").c_str());
  m_shader_depth.attachFragmentShader(
      getAssetFilePath("shader/DepthShader.fs").c_str());
  m_shader_depth.attachGeometryShader(
      getAssetFilePath("shader/DepthShader.gs").c_str());
  m_shader_depth.link();

  m_shader_shadow.generateProgramObject();
  m_shader_shadow.attachVertexShader(
      getAssetFilePath("shader/ShadowShader.vs").c_str());
  m_shader_shadow.attachFragmentShader(
      getAssetFilePath("shader/ShadowShader.fs").c_str());
  m_shader_shadow.link();

  m_shader_cubemap.generateProgramObject();
  m_shader_cubemap.attachVertexShader(
      getAssetFilePath("shader/CubeMapShader.vs").c_str());
  m_shader_cubemap.attachFragmentShader(
      getAssetFilePath("shader/CubeMapShader.fs").c_str());
  m_shader_cubemap.link();

  m_shader_particle.generateProgramObject();
  m_shader_particle.attachVertexShader(
      getAssetFilePath("shader/ParticleShader.vs").c_str());
  m_shader_particle.attachFragmentShader(
      getAssetFilePath("shader/ParticleShader.fs").c_str());
  m_shader_particle.link();

  m_shader_debug.generateProgramObject();
  m_shader_debug.attachVertexShader(
      getAssetFilePath("shader/QuadShader.vs").c_str());
  m_shader_debug.attachFragmentShader(
      getAssetFilePath("shader/QuadShader.fs").c_str());
  m_shader_debug.link();
}

//----------------------------------------------------------------------------------------
void A3::enableVertexShaderInputSlots() {
  //-- Enable input slots for m_vao_meshData:
  {
    glBindVertexArray(m_vao_meshData);

    // Enable the vertex shader attribute location for "position" when
    // rendering.
    m_positionAttribLocation = m_shader.getAttribLocation("position");
    glEnableVertexAttribArray(m_positionAttribLocation);

    // Enable the vertex shader attribute location for "normal" when
    // rendering.
    m_normalAttribLocation = m_shader.getAttribLocation("normal");
    glEnableVertexAttribArray(m_normalAttribLocation);

    m_uvAttribLocation = m_shader.getAttribLocation("tileUV");
    glEnableVertexAttribArray(m_uvAttribLocation);

    CHECK_GL_ERRORS;
  }

  //-- Enable input slots for m_vao_arcCircle:
  {
    glBindVertexArray(m_vao_arcCircle);

    // Enable the vertex shader attribute location for "position" when
    // rendering.
    m_arc_positionAttribLocation =
        m_shader_arcCircle.getAttribLocation("position");
    glEnableVertexAttribArray(m_arc_positionAttribLocation);

    CHECK_GL_ERRORS;
  }

  // Restore defaults
  glBindVertexArray(0);
}

//----------------------------------------------------------------------------------------
void A3::uploadVertexDataToVbos(const MeshConsolidator &meshConsolidator) {
  // Generate VBO to store all vertex position data
  {
    glGenBuffers(1, &m_vbo_vertexPositions);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo_vertexPositions);

    glBufferData(GL_ARRAY_BUFFER, meshConsolidator.getNumVertexPositionBytes(),
                 meshConsolidator.getVertexPositionDataPtr(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    CHECK_GL_ERRORS;
  }

  // Generate VBO to store all vertex normal data
  {
    glGenBuffers(1, &m_vbo_vertexNormals);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo_vertexNormals);

    glBufferData(GL_ARRAY_BUFFER, meshConsolidator.getNumVertexNormalBytes(),
                 meshConsolidator.getVertexNormalDataPtr(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    CHECK_GL_ERRORS;
  }

  // Generate VBO to store all UV data
  {
    glGenBuffers(1, &m_vbo_vertexUVs);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo_vertexUVs);

    glBufferData(GL_ARRAY_BUFFER, meshConsolidator.getNumVertexUVBytes(),
                 meshConsolidator.getVertexUVDataPtr(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    CHECK_GL_ERRORS;
  }

  // Generate VBO to store the trackball circle.
  {
    glGenBuffers(1, &m_vbo_arcCircle);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo_arcCircle);

    float *pts = new float[2 * CIRCLE_PTS];
    for (size_t idx = 0; idx < CIRCLE_PTS; ++idx) {
      float ang = 2.0 * M_PI * float(idx) / CIRCLE_PTS;
      pts[2 * idx] = cos(ang);
      pts[2 * idx + 1] = sin(ang);
    }

    glBufferData(GL_ARRAY_BUFFER, 2 * CIRCLE_PTS * sizeof(float), pts,
                 GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    CHECK_GL_ERRORS;
  }
}

//----------------------------------------------------------------------------------------
void A3::mapVboDataToVertexShaderInputLocations() {
  // Bind VAO in order to record the data mapping.
  glBindVertexArray(m_vao_meshData);

  // Tell GL how to map data from the vertex buffer "m_vbo_vertexPositions"
  // into the "position" vertex attribute location for any bound vertex shader
  // program.
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo_vertexPositions);
  glVertexAttribPointer(m_positionAttribLocation, 3, GL_FLOAT, GL_FALSE, 0,
                        nullptr);

  // Tell GL how to map data from the vertex buffer "m_vbo_vertexNormals" into
  // the "normal" vertex attribute location for any bound vertex shader
  // program.
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo_vertexNormals);
  glVertexAttribPointer(m_normalAttribLocation, 3, GL_FLOAT, GL_FALSE, 0,
                        nullptr);

  glBindBuffer(GL_ARRAY_BUFFER, m_vbo_vertexUVs);
  glVertexAttribPointer(m_uvAttribLocation, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

  //-- Unbind target, and restore default values:
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  CHECK_GL_ERRORS;

  // Bind VAO in order to record the data mapping.
  glBindVertexArray(m_vao_arcCircle);

  // Tell GL how to map data from the vertex buffer "m_vbo_arcCircle" into the
  // "position" vertex attribute location for any bound vertex shader program.
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo_arcCircle);
  glVertexAttribPointer(m_arc_positionAttribLocation, 2, GL_FLOAT, GL_FALSE, 0,
                        nullptr);

  //-- Unbind target, and restore default values:
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  CHECK_GL_ERRORS;
}

//----------------------------------------------------------------------------------------
void A3::initPerspectiveMatrix() {
  float aspect = ((float)m_windowWidth) / m_windowHeight;
  m_perpsective =
      glm::perspective(degreesToRadians(65.0f), aspect, 0.1f, 100.0f);
}

//----------------------------------------------------------------------------------------
void A3::initViewMatrix() {
  // ------------ Shadow test puppet mode
  // m_view = glm::lookAt(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f),
  //                      vec3(0.0f, 1.0f, 0.0f));
  //--------------J mode----------
  // m_view = glm::lookAt(vec3(0.0f, 0.0f, 130.0f), vec3(0.0f, 0.0f, -1.0f),
  //                      vec3(0.0f, 1.0f, 0.0f));
  //--------------P mode
  m_view = glm::lookAt(vec3(0.0f, 45.0f, 85.0f), vec3(0.0f, 60.0f, -1.0f),
                       vec3(0.0f, 1.0f, 0.0f));
}

//----------------------------------------------------------------------------------------
void A3::initLightSources() {
  // World-space position
  // -----------Normal mode--------------
  m_light.position = vec3(3.0f, 2 * 30.0f, 2 * 30.0f);
  //----------- Shadow test puppet mode-------------
  //  m_light.position = vec3(0.0f, 0.0f, 15.0f);
  m_light.rgbIntensity = vec3(0.7f); // light
}

//----------------------------------------------------------------------------------------
void A3::uploadCommonSceneUniforms() {
  m_shader.enable();
  {
    //-- Set Perpsective matrix uniform for the scene:
    GLint location = m_shader.getUniformLocation("Perspective");
    glUniformMatrix4fv(location, 1, GL_FALSE, value_ptr(m_perpsective));
    CHECK_GL_ERRORS;

    location = m_shader.getUniformLocation("picking");
    glUniform1i(location, pick ? 1 : 0);
    CHECK_GL_ERRORS;

    location = m_shader.getUniformLocation("toonrender");
    glUniform1i(location, toonrender ? 1 : 0);
    CHECK_GL_ERRORS;

    if (curr_mode == 0) {
      //-- Set LightSource uniform for the scene:
      {
        location = m_shader.getUniformLocation("light.position");
        glUniform3fv(location, 1, value_ptr(m_light.position));
        location = m_shader.getUniformLocation("light.rgbIntensity");
        glUniform3fv(location, 1, value_ptr(m_light.rgbIntensity));
        CHECK_GL_ERRORS;
      }

      //-- Set background light ambient intensity
      {
        location = m_shader.getUniformLocation("ambientIntensity");
        vec3 ambientIntensity(0.2f);
        glUniform3fv(location, 1, value_ptr(ambientIntensity));
        CHECK_GL_ERRORS;
      }
    }
  }
  m_shader.disable();
}

void A3::updateParticles() {
  particleSys.particlesCount = 0;
  for (int i = 0; i < particleSys.maxParticles; i++) {
    Particle &p = particleSys.particlesContainer[i];
    if (p.life > 0.0f) {
      p.life -= 1.0f;
      if (p.life > 0.0f) {
        p.pos += p.speed;
        p.col.a = p.life / MAXLIFE + 0.4;
        if (p.col.a >= 1) {
          p.col.a = 1;
        }

        g_particule_position_size_data[4 * particleSys.particlesCount + 0] =
            p.pos.x;
        g_particule_position_size_data[4 * particleSys.particlesCount + 1] =
            p.pos.y;
        g_particule_position_size_data[4 * particleSys.particlesCount + 2] =
            p.pos.z;
        g_particule_position_size_data[4 * particleSys.particlesCount + 3] =
            p.size;

        g_particule_color_data[4 * particleSys.particlesCount + 0] = p.col.r;
        g_particule_color_data[4 * particleSys.particlesCount + 1] = p.col.g;
        g_particule_color_data[4 * particleSys.particlesCount + 2] = p.col.b;
        g_particule_color_data[4 * particleSys.particlesCount + 3] = p.col.a;
      } else {
        if (p.mode == 0) {
          vec3 newpos = vec3(p.initpos.x + curr_L_loc.x,
                             p.initpos.y + LCurrZ + curr_L_loc.y,
                             p.initpos.z + curr_L_loc.z);
          p.reActive(newpos);
        } else if (p.mode == 1) {
          float angle = 0;
          if (curr_loc == 0) {
            angle = 0;
          } else if (curr_loc == 1) {
            angle = mid_angle;
          } else if (curr_loc == 2) {
            angle = far_angle;
          }
          mat4 rot_matrix_base =
              glm::rotate(degreesToRadians(angle), vec3(1, 0, 0));
          vec3 newpos = vec3(rot_matrix_base * vec4(p.initpos, 0));

          mat4 rot_matrix =
              glm::rotate(degreesToRadians(curr_angle), vec3(1, 0, 0));
          newpos = vec3(rot_matrix * vec4(p.initpos, 0));
          p.reActive(newpos);
        }
      }
      particleSys.particlesCount++;
    }
  }
}

//----------------------------------------------------------------------------------------
/*
 * Called once per frame, before guiLogic().
 */
void A3::appLogic() {
  // Place per frame, application logic here ...
  updateParticles();

  uploadCommonSceneUniforms();
  draw();
  if (!pick) {
    // glClearColor(0.2, 0.5, 0.3, 1.0);
    glClearColor(0.7, 0.7, 0.7, 0.8);
  }
}

//----------------------------------------------------------------------------------------
/*
 * Called once per frame, after appLogic(), but before the draw() method.
 */
void A3::guiLogic() {
  if (!show_gui) {
    return;
  }

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
  if (ImGui::BeginMenu("Application")) {
    if (ImGui::Button("Reset Position (I)")) {
      // reset position
      resetPos();
    }
    if (ImGui::Button("Reset Orintation (O)")) {
      // reset orintation
      resetOrin();
    }
    if (ImGui::Button("Reset All (A)")) {
      // reset Joints && clearn undo/redo stack
      resetPos();
      resetOrin();
      resetJoints();
    }
    if (ImGui::Button("Quit Application (Q)")) {
      glfwSetWindowShouldClose(m_window, GL_TRUE);
    }
    ImGui::EndMenu();
  }

  if (ImGui::BeginMenu("Basic Options")) {
    ImGui::Checkbox("Circle (C)", &Circle);
    ImGui::Checkbox("Z-buffer (Z)", &ZBuffer);
    ImGui::Checkbox("Backface culling (B)", &Backface_culling);
    ImGui::Checkbox("Frontface culling (F)", &Frontface_culling);
    ImGui::EndMenu();
  }

  if (ImGui::BeginMenu("Premium Options")) {
    ImGui::Checkbox("Toon Shader (T)", &toonrender);
    ImGui::Checkbox("Shadow (S)", &shadowMaprender);
    ImGui::Checkbox("Complex L-System (L)", &cplxLSystemrender);
    ImGui::Checkbox("Particle System (P)", &particlerender);
    ImGui::Checkbox("Texture Mapping (M)", &texturerender);
    ImGui::EndMenu();
  }

  // Create Button, and check if it was clicked:
  ImGui::Text("Mode Selection:");
  ImGui::PushID(0);
  if (ImGui::RadioButton("Game Mode (G)", &curr_mode, 0)) {
    pick = false;
    m_view = glm::lookAt(vec3(0.0f, 45.0f, 85.0f), vec3(0.0f, 60.0f, -1.0f),
                         vec3(0.0f, 1.0f, 0.0f));
    uploadCommonSceneUniforms();
    draw();
  }
  ImGui::PopID();
  ImGui::PushID(1);
  if (ImGui::RadioButton("Travel Mode (J)", &curr_mode, 1)) {
    m_view = glm::lookAt(vec3(0.0f, 0.0f, 130.0f), vec3(0.0f, 0.0f, -1.0f),
                         vec3(0.0f, 1.0f, 0.0f));
  }
  ImGui::PopID();

  if (ImGui::Button("Start Game")) {
    // start the game
    if (!gameStart) {
      gameStart = true;
      alSourcePlay(moveSource);
      cout << "sound effect" << endl;
      startTime = timeSinceEpochMillisec();
      curr_mode = 0;
    } else {
  initTimeQueue("Assets/sound/timeslot.txt", "Assets/sound/timeslotmove.txt");
      alSourceStop(moveSource);
      gameStart = false;
    }
  }

  framerate = ImGui::GetIO().Framerate;
  ImGui::Text("Framerate: %.1f FPS", framerate);

  // ImGui::Text("Undo/Redo Warning: ");
  // ImGui::SameLine();
  // if (redo_stack.size() > 0 && undo_stack.size() > 1) {
  //   ImGui::Text("no warning");
  // } else if (redo_stack.size() <= 0 && undo_stack.size() <= 1) {
  //   ImGui::Text("Redo and Undo invalid");
  // } else if (redo_stack.size() <= 0) {
  //   ImGui::Text("Redo invalid");
  // } else if (undo_stack.size() <= 1) {
  //   ImGui::Text("Undo invalid");
  // }

  ImGui::End();
}

//----------------------------------------------------------------------------------------
// Update mesh specific shader uniforms:
static void updateShaderUniforms(
    const ShaderProgram &shader, const GeometryNode &node,
    const glm::mat4 &viewMatrix, const glm::mat4 &roottrans, glm::mat4 &transM,
    glm::mat4 &transScale, glm::mat4 &rotateViewMatrix, int curr_mode,
    bool pick, float farplane, mat4 lightProjection, GLuint shadowMap,
    GLuint sandTexture, bool shadowrender, bool texturerender) {

  shader.enable();
  {
    //-- Set ModelView matrix:
    GLint location = shader.getUniformLocation("ModelView");
    mat4 modelView = viewMatrix * roottrans * transM * node.trans *
                     node.transScale * transScale;
    glUniformMatrix4fv(location, 1, GL_FALSE, value_ptr(modelView));
    CHECK_GL_ERRORS;

    glUniform1i(shader.getUniformLocation("shadowMap"), 0);
    glUniform1i(shader.getUniformLocation("textureMap"), 1);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, shadowMap);
    // location = shader.getUniformLocation("shadowMap");
    // glUniform1i(location, shadowMap);
    CHECK_GL_ERRORS;

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, sandTexture);
    // location = shader.getUniformLocation("shadowMap");
    // glUniform1i(location, shadowMap);
    CHECK_GL_ERRORS;

    location = shader.getUniformLocation("Model");
    mat4 model = roottrans * transM * node.trans * node.transScale * transScale;
    glUniformMatrix4fv(location, 1, GL_FALSE, value_ptr(model));
    CHECK_GL_ERRORS;

    location = shader.getUniformLocation("lightProjection");
    mat4 proj = lightProjection;
    glUniformMatrix4fv(location, 1, GL_FALSE, value_ptr(proj));
    CHECK_GL_ERRORS;

    location = shader.getUniformLocation("shadowflag");
    int shadow = shadowrender;
    glUniform1i(location, shadow);
    CHECK_GL_ERRORS;

    location = shader.getUniformLocation("textureflag");
    int text = texturerender;
    glUniform1i(location, text);
    CHECK_GL_ERRORS;

    if (!pick) {
      //-- Set NormMatrix:
      location = shader.getUniformLocation("NormalMatrix");
      mat3 normalMatrix = glm::transpose(glm::inverse(mat3(modelView)));
      glUniformMatrix3fv(location, 1, GL_FALSE, value_ptr(normalMatrix));
      CHECK_GL_ERRORS;

      //-- Set Material values:
      location = shader.getUniformLocation("material.kd");
      vec3 kd = node.material.kd;
      if (node.isSelected) {
        kd *= 0.5;
      }
      glUniform3fv(location, 1, value_ptr(kd));

      CHECK_GL_ERRORS;

    } else if (curr_mode == 1 && pick) {
      int node_id = node.m_nodeId;
      float r = float(node_id & 0xff) / 255.0f;
      float g = float((node_id >> 8) & 0xff) / 255.0f;
      float b = float((node_id >> 16) & 0xff) / 255.0f;

      location = shader.getUniformLocation("material.kd");
      glUniform3f(location, r, g, b);
      CHECK_GL_ERRORS;
    }
  }
  shader.disable();
}

void A3::updateSphereAnimation() {
  if (curr_mode == 0) {
    outerSphere->rotate('x', 0.3);
  }
}

void A3::updateStarAnimation() {
  // shipRNode->rotate('x', -15);
  // shipRNode->rotate('x', -50);
  if (mid && curr_loc != 1) {
    if (curr_angle > mid_angle) {
      shipRNode->rotate('x', -0.5);
      weaponNode->rotate('y', 1);
      curr_angle -= 0.5;
      if (curr_angle <= mid_angle) {
        mid = false;
        curr_loc = 1;
      }
    } else {
      shipRNode->rotate('x', 0.5);
      weaponNode->rotate('y', -1);

      curr_angle += 0.5;
      if (curr_angle >= mid_angle) {
        mid = false;
        curr_loc = 1;
      }
    }
  } else if (far && curr_loc != 2) {
    if (curr_angle > far_angle) {
      shipRNode->rotate('x', -0.5);
      weaponNode->rotate('y', 1);
      curr_angle -= 0.5;
      if (curr_angle <= far_angle) {
        far = false;
        curr_loc = 2;
      }
    }
  } else if (near && curr_loc != 0) {
    if (curr_angle < near_angle) {
      shipRNode->rotate('x', 0.5);
      weaponNode->rotate('y', -1);

      curr_angle += 0.5;
      if (curr_angle >= near_angle) {
        near = false;
        curr_loc = 0;
      }
    }
  }
}

void A3::updateShipAnimation(float L_speed, float R_speed) {
  if (Ldir == 1) {
    shipLNode->translate(vec3(0.0f, L_speed, 0.0f));
    LCurrZ += L_speed;
    if (LCurrZ >= LMaxZ) {
      Ldir = 0;
    }
  } else if (Ldir == 0) {
    shipLNode->translate(vec3(0.0f, -L_speed, 0.0f));
    LCurrZ -= L_speed;
    if (LCurrZ <= LMinZ) {
      Ldir = 1;
    }
  }

  if (Rdir == 1) {
    shipRNode->translate(vec3(0.0f, R_speed, 0.0f));
    RCurrZ += R_speed;
    if (RCurrZ >= RMaxZ) {
      Rdir = 0;
    }
  } else if (Rdir == 0) {
    shipRNode->translate(vec3(0.0f, -R_speed, 0.0f));
    RCurrZ -= R_speed;
    if (RCurrZ <= RMinZ) {
      Rdir = 1;
    }
  }
}

void A3::updateCurrShipLoc(vec3 speed) {
  for (auto &v : curr_Lship_loc) {
    v += speed;
  }
}

void A3::moveLboatAnimation(vec3 L_speed, int mode) {
  // mode 0, left first
  if (mode == 0) {
    if (moveLeft) {
      shipLNode->translate(-1.0f * L_speed);
      updateCurrShipLoc(-1.0f * L_speed);
      curr_L_loc -= L_speed;
      if (curr_L_loc.x <= leftbound) {
        moveLeft = false;
        moveRight = true;
      }
    } else if (moveRight) {
      shipLNode->translate(L_speed);
      updateCurrShipLoc(L_speed);

      curr_L_loc += L_speed;
      if (curr_L_loc.x >= origin) {
        moveRight = false;
      }
    }
  } else if (mode == 1) {
    if (moveRight) {
      shipLNode->translate(L_speed);
      updateCurrShipLoc(L_speed);

      curr_L_loc += L_speed;
      if (curr_L_loc.x >= rightbound) {
        moveRight = false;
        moveLeft = true;
      }
    } else if (moveLeft) {
      shipLNode->translate(-1.0f * L_speed);
      updateCurrShipLoc(-1.0f * L_speed);

      curr_L_loc -= L_speed;
      if (curr_L_loc.x <= origin) {
        moveLeft = false;
      }
    }
  } else if (mode == 2) {
    if (moveup) {
      shipLNode->translate(L_speed);
      updateCurrShipLoc(L_speed);

      curr_L_loc += L_speed;
      if (curr_L_loc.y >= upbound) {
        moveup = false;
        movedown = true;
      }
    } else if (movedown) {
      shipLNode->translate(-1.0f * L_speed);
      updateCurrShipLoc(-1.0f * L_speed);

      curr_L_loc -= L_speed;
      if (curr_L_loc.y <= origin) {
        movedown = false;
      }
    }
  }
}

bool A3::checkCollision() {
  for (auto locV : curr_Lship_loc) {
    if (abs(locV.x - curr_bullet_loc.x) <= 2 &&
        abs(locV.z - curr_bullet_loc.z) <= 2 &&
        abs(locV.y - curr_bullet_loc.y) <= 2) {
      return true;
    }
  }
  return false;
}

void A3::hitAnimation(vec3 speed, int pos) {
  if (bullet) {
    bullet->translate(speed);
    curr_bullet_loc = vec3(bullet->trans * vec4(0, 0, 0, 1));
    bool collide = checkCollision();
    if (collide) {
      particleSys.createParticle(2, 10, curr_bullet_loc, vec3(0, 0.2, 0), 0.1,
                                 10); // set mode to 1
      m_rootNode.get()->remove_child(bullet);
    }
  }
}

void A3::hitBulletAnimation() {
  vec3 speed;
  if (curr_loc == 0) {
    speed = vec3(-0.3, 0, 0);
  } else if (curr_loc == 1) {
    float scale = 0.2;
    speed = vec3(-1.8*scale, -0.8*scale, 0.5*scale);
  } else if (curr_loc == 2) {
    if (curr_bullet_loc.x >= 0)
      speed = vec3(-0.4, -0.3, 0.7);
    else
      speed = vec3(-0.4, -0.7, 0.7);
  }
  hitAnimation(speed, 0);
}

void A3::updateTimeAnimation(){
    currTimeInterval = timeQueue.front();
    currTimeMoveInterval = timeMoveQueue.front();
    currTime = timeSinceEpochMillisec() - startTime;
    // cout<<to_string(currTimeInterval) << " "<<currTime<<endl;
    if (currTime >= currTimeInterval.x && currTime <= currTimeInterval.y) {
      initBullet(angleList[curr_loc]);
      bulletout = true;
      timeQueue.pop();
    }
    if(currTime >= currTimeMoveInterval.x && currTime <= currTimeMoveInterval.y){
      near = false;
      far = false;
      mid = false;
      if(currTimeMoveInterval.z == 0)
        near = true;
      else if(currTimeMoveInterval.z == 1)
        mid = true;
      else if(currTimeMoveInterval.z == 2)
        far = true;
    }
}

//----------------------------------------------------------------------------------------
/*
 * Called once per frame, after guiLogic().
 */
void A3::draw() {
  if (ZBuffer) {
    glEnable(GL_DEPTH_TEST);
  }
  if (Backface_culling || Frontface_culling) {
    glEnable(GL_CULL_FACE);
    if (Backface_culling && Frontface_culling) {
      glCullFace(GL_FRONT_AND_BACK);
    } else if (Backface_culling) {
      glCullFace(GL_BACK);
    } else if (Frontface_culling) {
      glCullFace(GL_FRONT);
    }
  }

  if (gameStart) 
    updateTimeAnimation();

  updateSphereAnimation();
  // updateShipAnimation(0.005, 0.006);
  updateStarAnimation();
  if (Lmode == 0)
    moveLboatAnimation(vec3(0.5, 0, 0), 0);
  else if (Lmode == 1)
    moveLboatAnimation(vec3(0.5, 0, 0), 1);
  else if (Lmode == 2) {
    moveLboatAnimation(vec3(0, 0.3, 0.3), 2);
  }

  if (bulletout)
    hitBulletAnimation();

  mat4 modeltrans = mat4(1.0f);
  mat4 modeltransS = mat4(1.0f);

  /*
  Shadow Map and Debug
  */

  if (shadowMaprender) {
    m_shader_shadow.enable();
    glEnable(GL_DEPTH_TEST);
    glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);

    renderShadowScene(*m_rootNode, modeltrans, modeltransS);
    m_shader_shadow.disable();
    glViewport(0, 0, 1024, 768);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }

  // -----------------------------------------
  // --------------Debug shadow map-----------
  // ---------------------------------------
  // glDisable(GL_DEPTH_TEST);
  // glViewport(0, 0, 1024, 1024);
  // renderQuadScene(*m_rootNode, modeltrans, modeltransS);
  // glViewport(0, 0, 1024, 768);

  //-------------------------------------------
  //------------ Normal render & CubeMap
  //------------------------------------------
  // glEnable(GL_DEPTH_TEST);

  if (particlerender) {
    renderParticles();
  }

  renderSceneGraph(*m_rootNode, modeltrans, modeltransS);

  renderCubeMap();

  glBindVertexArray(0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glClear(GL_DEPTH_BUFFER_BIT);
  glDisable(GL_DEPTH_TEST);
  glBindTexture(GL_TEXTURE_2D, 0);

  if (Backface_culling || Frontface_culling) {
    glDisable(GL_CULL_FACE);
  }
  // update Circle
  if (Circle) {
    renderArcCircle();
  }
}

void A3::renderShadow() {
  glm::mat4 l_perspective = ortho(-60.0f, 60.0f, -60.0f, 60.0f, l_near, l_far);
  glm::mat4 lightView =
      glm::lookAt(m_light.position, glm::vec3(0.0f, 0.0f, 0.0f),
                  glm::vec3(0.0f, 1.0f, 0.0f));
  lightProjection = l_perspective * lightView;
  // glm::mat4 lightProjection2 = m_perpsective * m_view;
  m_shader_shadow.enable();
  glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
  GLint m_location = m_shader_shadow.getUniformLocation("lightProjection");
  glUniformMatrix4fv(m_location, 1, GL_FALSE, value_ptr(lightProjection));
  m_shader_shadow.disable();
}

unsigned int quadVAO = 0;
unsigned int quadVBO;
void A3::renderQuadScene(const SceneNode &root, mat4 modeltrans,
                         mat4 modelScale) {
  m_shader_debug.enable();
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, shadowMap);
  if (quadVAO == 0) {
    float quadVertices[] = {
        // positions        // texture Coords
        -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        1.0f,  1.0f, 0.0f, 1.0f, 1.0f, 1.0f,  -1.0f, 0.0f, 1.0f, 0.0f,
    };
    // setup plane VAO
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices,
                 GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                          (void *)(3 * sizeof(float)));
  }
  glBindVertexArray(quadVAO);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  m_shader_debug.disable();
  glBindVertexArray(0);
}

void A3::renderShadowScene(const SceneNode &root, mat4 modeltrans,
                           mat4 modelScale) {

  glBindVertexArray(m_vao_meshData);
  glViewport(0, 0, shadow_width, shadow_height);
  // glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
  // glClear(GL_DEPTH_BUFFER_BIT);
  mat4 transM = modeltrans * root.trans;
  mat4 transScale = modelScale * root.transScale;
  for (const SceneNode *node : root.children) {
    bool change_color = false;

    if (node->m_nodeType != NodeType::GeometryNode)
      continue;

    const GeometryNode *geometryNode = static_cast<const GeometryNode *>(node);

    glm::mat4 root_trans = transMatrix * rotateMatrix;

    //  m_shader_shadow.enable();
    GLint location = m_shader_shadow.getUniformLocation("model");
    mat4 model =
        root_trans * transM * node->trans * node->transScale * transScale;
    glUniformMatrix4fv(location, 1, GL_FALSE, value_ptr(model));
    CHECK_GL_ERRORS;

    // Get the BatchInfo corresponding to the GeometryNode's unique MeshId.
    BatchInfo batchInfo = m_batchInfoMap[geometryNode->meshId];

    //-- Now render the mesh:
    glDrawArrays(GL_TRIANGLES, batchInfo.startIndex, batchInfo.numIndices);
    // m_shader_shadow.disable();
  }

  for (const SceneNode *node : root.children) {
    renderShadowScene(*node, transM, transScale); // recursion
  }
  // glBindFramebuffer(GL_FRAMEBUFFER, 0);
  CHECK_GL_ERRORS;
}

// //----------------------------------------------------------------------------------------
void A3::renderSceneGraph(const SceneNode &root, mat4 modeltrans,
                          mat4 modelScale) {

  // Bind the VAO once here, and reuse for all GeometryNode rendering below.
  glBindVertexArray(m_vao_meshData);

  // This is emphatically *not* how you should be drawing the scene graph in
  // your final implementation.  This is a non-hierarchical demonstration
  // in which we assume that there is a list of GeometryNodes living directly
  // underneath the root node, and that we can draw them in a loop.  It's
  // just enough to demonstrate how to get geometry and materials out of
  // a GeometryNode and onto the screen.

  // You'll want to turn this into recursive code that walks over the tree.
  // You can do that by putting a method in SceneNode, overridden in its
  // subclasses, that renders the subtree rooted at every node.  Or you
  // could put a set of mutually recursive functions in this class, which
  // walk down the tree from nodes of different types.

  // cout<<jointsSet.size()<<endl;
  mat4 transM = modeltrans * root.trans;
  mat4 transScale = modelScale * root.transScale;
  // mat4 transM = modeltrans *root.trans;
  for (const SceneNode *node : root.children) {
    bool change_color = false;

    mat4 rotateM = mat4(1.0f);
    // if(root.m_nodeType == NodeType::JointNode){
    //   rotateM = root.trans;
    //   transM = modeltrans;
    // }else{
    //   transM = modeltrans * root.trans;
    // }

    if (node->m_nodeType != NodeType::GeometryNode)
      continue;

    const GeometryNode *geometryNode = static_cast<const GeometryNode *>(node);

    // glm::mat4 root_trans =
    //     transMatrix * m_view * rotateMatrix * inverse(m_view);
    glm::mat4 root_trans = transMatrix * rotateMatrix;
    updateShaderUniforms(m_shader, *geometryNode, m_view, root_trans, transM,
                         transScale, rotateViewMatrix, curr_mode, pick,
                         far_plane, lightProjection, shadowMap, sandTexture,
                         shadowMaprender, texturerender);

    // Get the BatchInfo corresponding to the GeometryNode's unique MeshId.
    BatchInfo batchInfo = m_batchInfoMap[geometryNode->meshId];
    if (cplxLSystemrender) {
      if (geometryNode->meshId == "plant1") {
        batchInfo = m_batchInfoMap["plant2"];
      }
    }

    //-- Now render the mesh:
    m_shader.enable();
    glDrawArrays(GL_TRIANGLES, batchInfo.startIndex, batchInfo.numIndices);
    m_shader.disable();
  }

  for (const SceneNode *node : root.children) {
    renderSceneGraph(*node, transM, transScale); // recursion
  }
  CHECK_GL_ERRORS;
}

void A3::renderParticles() {
  m_shader_particle.enable();
  glEnable(GL_DEPTH_TEST);

  GLint p_location = m_shader_particle.getUniformLocation("P");
  GLint v_location = m_shader_particle.getUniformLocation("V");
  GLint m_location = m_shader_particle.getUniformLocation("M");

  glUniformMatrix4fv(p_location, 1, GL_FALSE, value_ptr(m_perpsective));
  glUniformMatrix4fv(v_location, 1, GL_FALSE, value_ptr(m_view));
  glUniformMatrix4fv(m_location, 1, GL_FALSE,
                     value_ptr(transMatrix * rotateMatrix));
  glBindVertexArray(m_vao_particle);

  glBindBuffer(GL_ARRAY_BUFFER, m_vbo_particlepos);
  glBufferData(GL_ARRAY_BUFFER, particleSys.maxParticles * 4 * sizeof(GLfloat),
               NULL, GL_STREAM_DRAW);
  glBufferSubData(GL_ARRAY_BUFFER, 0,
                  particleSys.particlesCount * 4 * sizeof(GLfloat),
                  g_particule_position_size_data);

  glBindBuffer(GL_ARRAY_BUFFER, m_vbo_particlecolour);
  glBufferData(GL_ARRAY_BUFFER, particleSys.maxParticles * 4 * sizeof(GLfloat),
               NULL, GL_STREAM_DRAW);
  glBufferSubData(GL_ARRAY_BUFFER, 0,
                  particleSys.particlesCount * 4 * sizeof(GLfloat),
                  g_particule_color_data);

  GLint posAttrib = m_shader_particle.getAttribLocation("position");
  GLint offsetAttrib = m_shader_particle.getAttribLocation("offsetSize");
  GLint colourAttrib = m_shader_particle.getAttribLocation("colour");

  glVertexAttribDivisor(posAttrib, 0);
  glVertexAttribDivisor(offsetAttrib, 1);
  glVertexAttribDivisor(colourAttrib, 1);

  glDrawArraysInstanced(GL_TRIANGLES, 0, 36, particleSys.particlesCount);

  m_shader_particle.disable();

  glBindVertexArray(0);
  CHECK_GL_ERRORS;
}

void A3::renderCubeMap() {
  m_shader_cubemap.enable();
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);

  GLint location = m_shader_cubemap.getUniformLocation("P");
  glUniformMatrix4fv(location, 1, GL_FALSE, value_ptr(m_perpsective));
  location = m_shader_cubemap.getUniformLocation("V");
  glUniformMatrix4fv(location, 1, GL_FALSE, value_ptr(m_view));
  location = m_shader_cubemap.getUniformLocation("render_textures");
  glUniform1i(location, cuberender);

  glBindVertexArray(m_vao_cubeMap);
  glBindTexture(GL_TEXTURE_CUBE_MAP, cubeTextureID);
  glDrawArrays(GL_TRIANGLES, 0, 36);
  glDepthFunc(GL_LESS);

  m_shader_cubemap.disable();
}

//----------------------------------------------------------------------------------------
// Draw the trackball circle.
void A3::renderArcCircle() {
  glBindVertexArray(m_vao_arcCircle);

  m_shader_arcCircle.enable();
  GLint m_location = m_shader_arcCircle.getUniformLocation("M");
  float aspect = float(m_framebufferWidth) / float(m_framebufferHeight);
  glm::mat4 M;
  if (aspect > 1.0) {
    M = glm::scale(glm::mat4(), glm::vec3(0.5 / aspect, 0.5, 1.0));
  } else {
    M = glm::scale(glm::mat4(), glm::vec3(0.5, 0.5 * aspect, 1.0));
  }
  glUniformMatrix4fv(m_location, 1, GL_FALSE, value_ptr(M));
  glDrawArrays(GL_LINE_LOOP, 0, CIRCLE_PTS);
  m_shader_arcCircle.disable();

  glBindVertexArray(0);
  CHECK_GL_ERRORS;
}

//----------------------------------------------------------------------------------------
/*
 * Called once, after program is signaled to terminate.
 */
void A3::cleanup() {}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles cursor entering the window area events.
 */
bool A3::cursorEnterWindowEvent(int entered) {
  bool eventHandled(false);

  // Fill in with event handling code...

  return eventHandled;
}

void A3::rotateTest(SceneNode &root) {
  for (SceneNode *node : root.children) {
    if (node->m_name == "neckJoint") {
      // printf("reachhere\n");
      node->rotate('z', 10);
    } else {
      rotateTest(*node);
    }
  }
}

void A3::movePuppetXYZ(float x_amount, float y_amount, vector<int> idx) {
  mat4 lastM = prev_transMatrix;
  for (int i = 0; i < idx.size(); i++) {
    int direction = idx[i];
    if (direction == 0) {
      transMatrix = glm::translate(lastM, vec3(x_amount, y_amount, 0));
    } else if (direction == 2) {
      transMatrix = glm::translate(lastM, vec3(0, 0, -y_amount));
    }
    lastM = transMatrix;
  }
}

void A3::rotatePuppetXYZ(float curr_x, float curr_y) {
  float diff_x = curr_x - initX;
  float diff_y = curr_y - initY;
  float center_x = m_framebufferWidth / 2;
  float center_y = m_framebufferHeight / 2;
  float radius = std::min(center_x / 2, center_y / 2);
  bool inCircle = false;
  // if (curr_x - center_x >= -1 * radius - TOLORANCE &&
  //     curr_x - center_x <= radius + TOLORANCE &&
  //     curr_y - center_y >= -1 * radius - TOLORANCE &&
  //     curr_y - center_y <= radius + TOLORANCE) {
  //   inCircle = true;
  // }

  float xy_sqr = pow((curr_x - center_x), 2) + pow((curr_y - center_y), 2);
  if (xy_sqr < radius * radius) {
    inCircle = true;
  }

  float zval = sqrt(radius * radius - xy_sqr);
  if (!inCircle) {
    zval = 0;
  }
  // vec3 P = vec3(initX - center_x, -initY + center_y, zval);
  vec3 P = vec3(curr_x - center_x, -curr_y + center_y, zval);

  vec3 norm_P = P / radius;
  // vec3 D = vec3(diff_x, -diff_y, 0);
  vec3 D = norm_P - last_normP;
  if (glm::l2Norm(D) > 0) {
    vec3 norm_D = D / glm::l2Norm(D);
    vec3 axis = cross(last_normP, norm_D);
    float angle = length(D) / (radius * 2) * PI;
    float angleZ =
        PI * sqrt(pow(diff_x, 2) + pow(diff_y, 2)) /
        sqrt(pow(m_framebufferWidth, 2) + pow(m_framebufferHeight, 2));
    angleZ = length(D) / (radius * 2) * PI;

    angle = 2 * glm::l2Norm(norm_P - last_normP);
    // cout << "last_normP" << last_normP << endl;
    // cout << "last_normP" << norm_P << endl;
    // cout << "norm_D" << norm_D << endl << endl;;

    // cout<<"prevrotate" << rotateMatrix << endl;

    if (inCircle) {
      // printf("axis x, y, z: %f, %f, %f\n", axis.x, axis.y, axis.z);
      // rotateMatrix = glm::rotate(prev_rotateMatrix, angle, axis);
      if (axis != vec3(0.0f)) {
        rotateMatrix = glm::rotate(rotateMatrix, angle, axis);
        // cout<<"currentrotate" << rotateMatrix << endl << endl;
      }

    } else {
      // rotateMatrix = glm::rotate(prev_rotateMatrix, angleZ, vec3(0, 0, 1));
      if (axis != vec3(0.0f)) {
        m_view = glm::rotate(m_view, angle / 2, axis);
      }
    }
    last_normP = norm_P;
  }
}

void A3::rotateJoints(float curr_x, float curr_y, bool rotateZ, bool posDirX,
                      bool posDirY) {
  float diffy = -1 * (curr_y - prevY);
  float diffx = -1 * (curr_x - prevX);
  for (JointNode *jnode : jointsSet) {
    if (rotateZ) {
      bool updateZ = jnode->updateAngle(diffy * 0.1, 2);
      // cout<<jnode->m_joint_z.min << ' '<< jnode->m_joint_z.max <<endl;
      if (updateZ) {
        jnode->rotate('z', diffy * 0.1);
      }
    } else {
      bool updateX = jnode->updateAngle(diffy * 0.1, 0);
      if (updateX)
        jnode->rotate('x', diffy * 0.1);

      bool updateY = jnode->updateAngle(diffy * 0.1, 1);
      if (updateY)
        jnode->rotate('y', diffy * 0.1);
    }
  }
}

void A3::rotateHead(float xpos, float ypos) {
  float diffy = -1 * (ypos - prevY);
  if (headJoint) {
    bool updateY = headJoint->updateAngle(diffy * 0.1, 1);
    if (updateY)
      headJoint->rotate('y', diffy * 0.1);
  }
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles mouse cursor movement events.
 */
bool A3::mouseMoveEvent(double xPos, double yPos) {
  bool eventHandled(false);
  double xpos, ypos;
  glfwGetCursorPos(m_window, &xpos, &ypos);
  bool leftSelect = false;
  bool rightSelect = false;
  bool midSelect = false;

  vector<int> idx;
  if (ImGui::IsMouseDragging(0)) {
    // rotateTest(*m_rootNode);
    idx.push_back(0);
    leftSelect = true;
  }
  if (ImGui::IsMouseDragging(1)) {
    idx.push_back(1);
    rightSelect = true;
  }
  if (ImGui::IsMouseDragging(2)) {
    idx.push_back(2);
    midSelect = true;
  }

  float x_amount = (xpos - initX) / TRANSSCALE;
  float y_amount = -1 * (ypos - initY) / TRANSSCALE;
  if (curr_mode == 0) {
    movePuppetXYZ(x_amount, y_amount, idx);
  }

  float center_x = m_framebufferWidth / 2;
  float center_y = m_framebufferHeight / 2;
  float radius = std::min(center_x / 2, center_y / 2);
  bool inCircle = false;
  float xy_sqr = pow((xpos - center_x), 2) + pow((ypos - center_y), 2);
  if (xy_sqr < radius * radius) {
    inCircle = true;
  }

  float zval = sqrt(radius * radius - xy_sqr);
  if (!inCircle) {
    zval = 0;
  }

  vec3 P = vec3(xpos - center_x, -ypos + center_y, zval);
  vec3 norm_P = P / radius;

  float x_angle = 0;
  float y_angle = 0;
  float z_angle = 0;
  if (rightSelect) {
    Circle = true;
    rotatePuppetXYZ(xpos, ypos);
  }
  last_normP = norm_P;

  return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles mouse button events.
 */
bool A3::mouseButtonInputEvent(int button, int actions, int mods) {
  bool eventHandled(false);

  // Fill in with event handling code...
  double xpos, ypos;
  if (!ImGui::IsMouseHoveringAnyWindow()) {
    if (actions == GLFW_PRESS) {
      glfwGetCursorPos(m_window, &xpos, &ypos);
      initX = xpos;
      initY = ypos;
      status_change = false;
      if (curr_mode == 1 && (button == GLFW_MOUSE_BUTTON_LEFT)) {
        pick = true;
        pickingJoints();
      }
    }
    if (actions == GLFW_RELEASE) {
      prev_transMatrix = transMatrix;
      prev_rotateMatrix = rotateMatrix;
      if (status_change) {
        // add to undo list
        updateUndoStack();
        status_change = true;
      }
    }
  }

  return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles mouse scroll wheel events.
 */
bool A3::mouseScrollEvent(double xOffSet, double yOffSet) {
  bool eventHandled(false);

  // Fill in with event handling code...

  return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles window resize events.
 */
bool A3::windowResizeEvent(int width, int height) {
  bool eventHandled(false);
  initPerspectiveMatrix();
  return eventHandled;
}

void A3::rotateHeadAuto() {
  if (click && angle < 90 && glfwGetTime() > 0.1f) {
    headJoint->rotate('y', 1);
    angle += 1;
  } else if (angle == 90) {
    click = false;
    angle = 0;
  }
  // glfwSetTime(0.0f);
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles key input events.
 */
bool A3::keyInputEvent(int key, int action, int mods) {
  bool eventHandled(false);

  if (action == GLFW_PRESS) {
    if (key == GLFW_KEY_M) {
      show_gui = !show_gui;
      eventHandled = true;
    }
    if (key == GLFW_KEY_Q) {
      glfwSetWindowShouldClose(m_window, GL_TRUE);
    }
    if (key == GLFW_KEY_I) {
      resetPos();
    }
    if (key == GLFW_KEY_O) {
      resetOrin();
    }
    // if (key == GLFW_KEY_S) {
    //   resetJoints();
    // }
    if (key == GLFW_KEY_A) {
      resetPos();
      resetOrin();
      resetJoints();
      curr_mode = 0;
    }
    // if (key == GLFW_KEY_U) {
    //   undo();
    // }
    // if (key == GLFW_KEY_R) {
    //   redo();
    // }
    if (key == GLFW_KEY_C) {
      Circle = (Circle) ? false : true;
    }
    if (key == GLFW_KEY_Z) {
      ZBuffer = (ZBuffer) ? false : true;
    }
    if (key == GLFW_KEY_B) {
      Backface_culling = (Backface_culling) ? false : true;
    }
    if (key == GLFW_KEY_F) {
      Frontface_culling = (Frontface_culling) ? false : true;
    }
    if (key == GLFW_KEY_G) {
      curr_mode = 0;
      pick = false;
      m_view = glm::lookAt(vec3(0.0f, 45.0f, 85.0f), vec3(0.0f, 60.0f, -1.0f),
                           vec3(0.0f, 1.0f, 0.0f));
    }
    if (key == GLFW_KEY_J) {
      curr_mode = 1;
      m_view = glm::lookAt(vec3(0.0f, 0.0f, 130.0f), vec3(0.0f, 0.0f, -1.0f),
                           vec3(0.0f, 1.0f, 0.0f));
    }
    if (key == GLFW_KEY_1) {
      near = true;
      mid = false;
      far = false;
      // rotateHeadAuto();
    }
    if (key == GLFW_KEY_2) {
      mid = true;
      near = false;
      far = false;
      // rotateHeadAuto();
    }
    if (key == GLFW_KEY_3) {
      far = true;
      near = false;
      mid = false;
      // rotateHeadAuto();
    }
    if (key == GLFW_KEY_LEFT) {
      moveLeft = true;
      Lmode = 0;
    }
    if (key == GLFW_KEY_RIGHT) {
      moveRight = true;
      Lmode = 1;
    }
    if (key == GLFW_KEY_DOWN) {
      initBullet(angleList[curr_loc]);
      bulletout = true;
    }
    if (key == GLFW_KEY_UP) {
      moveup = true;
      Lmode = 2;
    }
  }
  // Fill in with event handling code...

  return eventHandled;
}
