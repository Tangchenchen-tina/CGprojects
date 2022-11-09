// Termm-Fall 2022

#include "A3.hpp"
#include "scene_lua.hpp"
using namespace std;

#include "GeometryNode.hpp"
#include "JointNode.hpp"
#include "cs488-framework/GlErrorCheck.hpp"
#include "cs488-framework/MathUtils.hpp"

#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/io.hpp>
#include <queue>
#include <set>
#include <unordered_map>
#include <vector>

using namespace glm;
using namespace std;

static bool show_gui = true;

const size_t CIRCLE_PTS = 48;
const float TRANSSCALE = 300;
const float ROTATESCALE = 100;
const float TOLORANCE = 0.01;
const float TOTALNODE = 50;

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
}

//----------------------------------------------------------------------------------------
// Destructor
A3::~A3() {}

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
  unique_ptr<MeshConsolidator> meshConsolidator(new MeshConsolidator{
      getAssetFilePath("cube.obj"), getAssetFilePath("sphere.obj"),
      getAssetFilePath("suzanne.obj"), getAssetFilePath("mouth.obj"),
      getAssetFilePath("eyeslash.obj"), getAssetFilePath("nose.obj"),
      getAssetFilePath("foot.obj"), getAssetFilePath("cylinder.obj")});

  // Acquire the BatchInfoMap from the MeshConsolidator.
  meshConsolidator->getBatchInfoMap(m_batchInfoMap);

  // Take all vertex data within the MeshConsolidator and upload it to VBOs on
  // the GPU.
  uploadVertexDataToVbos(*meshConsolidator);

  mapVboDataToVertexShaderInputLocations();

  initPerspectiveMatrix();

  initViewMatrix();

  initLightSources();

  updateUndoStack();

  // initColorMap();

  // printf("colormapsize %d\n", colorMap.size());

  // Exiting the current scope calls delete automatically on meshConsolidator
  // freeing all vertex data resources.  This is fine since we already copied
  // this data to VBOs on the GPU.  We have no use for storing vertex data on
  // the CPU side beyond this point.
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

void A3::initColorMap() {
  queue<const SceneNode *> queue;
  const SceneNode &root = *m_rootNode;
  const SceneNode *snode = (root.children).front();
  queue.push(snode);
  while (!queue.empty()) {
    const SceneNode *topNode = queue.front();
    for (const SceneNode *node : topNode->children) {
      if (node->m_nodeType != NodeType::GeometryNode)
        queue.emplace(node);
      else {
        queue.emplace(node);
        int node_id = node->m_nodeId;
        float val = round(1000 * node_id / TOTALNODE) / 1000;
        colorMap.insert(pair<float, int>(val, node_id));
      }
    }
    queue.pop();
  }
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
  m_shader.attachVertexShader(getAssetFilePath("VertexShader.vs").c_str());
  m_shader.attachFragmentShader(getAssetFilePath("FragmentShader.fs").c_str());
  m_shader.link();

  m_shader_arcCircle.generateProgramObject();
  m_shader_arcCircle.attachVertexShader(
      getAssetFilePath("arc_VertexShader.vs").c_str());
  m_shader_arcCircle.attachFragmentShader(
      getAssetFilePath("arc_FragmentShader.fs").c_str());
  m_shader_arcCircle.link();
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

    // Enable the vertex shader attribute location for "normal" when rendering.
    m_normalAttribLocation = m_shader.getAttribLocation("normal");
    glEnableVertexAttribArray(m_normalAttribLocation);

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

  // Tell GL how to map data from the vertex buffer "m_vbo_vertexPositions" into
  // the "position" vertex attribute location for any bound vertex shader
  // program.
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo_vertexPositions);
  glVertexAttribPointer(m_positionAttribLocation, 3, GL_FLOAT, GL_FALSE, 0,
                        nullptr);

  // Tell GL how to map data from the vertex buffer "m_vbo_vertexNormals" into
  // the "normal" vertex attribute location for any bound vertex shader program.
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo_vertexNormals);
  glVertexAttribPointer(m_normalAttribLocation, 3, GL_FLOAT, GL_FALSE, 0,
                        nullptr);

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
      glm::perspective(degreesToRadians(60.0f), aspect, 0.1f, 100.0f);
}

//----------------------------------------------------------------------------------------
void A3::initViewMatrix() {
  m_view = glm::lookAt(vec3(0.0f, 0.0f, 5.0f), vec3(0.0f, 0.0f, -1.0f),
                       vec3(0.0f, 1.0f, 0.0f));
  // m_view = glm::lookAt(vec3(5.0f, 0.0f, -5.0f), vec3(-5, 0.0f, -5.0),
  // 	vec3(0.0f, 1.0f, 0.0f));
}

//----------------------------------------------------------------------------------------
void A3::initLightSources() {
  // World-space position
  m_light.position = vec3(5.0f, 0.0f, 10.0f);
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

//----------------------------------------------------------------------------------------
/*
 * Called once per frame, before guiLogic().
 */
void A3::appLogic() {
  // Place per frame, application logic here ...

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
    if (ImGui::Button("Reset Joints (S)")) {
      // reset Joints && clearn undo/redo stack
      resetJoints();
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

  if (ImGui::BeginMenu("Edit")) {
    if (ImGui::Button("Undo (U)")) {
      // Undo
      undo();
    }
    if (ImGui::Button("Redo (R)")) {
      // Redo
      redo();
    }
    ImGui::EndMenu();
  }

  if (ImGui::BeginMenu("Options")) {
    ImGui::Checkbox("Circle (C)", &Circle);
    ImGui::Checkbox("Z-buffer (Z)", &ZBuffer);
    ImGui::Checkbox("Backface culling (B)", &Backface_culling);
    ImGui::Checkbox("Frontface culling (F)", &Frontface_culling);
    ImGui::EndMenu();
  }

  // Create Button, and check if it was clicked:
  ImGui::PushID(0);
  if (ImGui::RadioButton("Position/Orintation (P)", &curr_mode, 0)) {
    pick = false;
    uploadCommonSceneUniforms();
    draw();
  }
  ImGui::PopID();
  ImGui::PushID(1);
  if (ImGui::RadioButton("Joints (J)", &curr_mode, 1)) {
  }
  ImGui::PopID();

  ImGui::Text("Framerate: %.1f FPS", ImGui::GetIO().Framerate);

  ImGui::Text("Undo/Redo Warning: ");
  ImGui::SameLine();
  if (redo_stack.size() > 0 && undo_stack.size() > 1) {
    ImGui::Text("no warning");
  } else if (redo_stack.size() <= 0 && undo_stack.size() <= 1) {
    ImGui::Text("Redo and Undo invalid");
  } else if (redo_stack.size() <= 0) {
    ImGui::Text("Redo invalid");
  } else if (undo_stack.size() <= 1) {
    ImGui::Text("Undo invalid");
  }

  ImGui::End();
}

//----------------------------------------------------------------------------------------
// Update mesh specific shader uniforms:
static void
updateShaderUniforms(const ShaderProgram &shader, const GeometryNode &node,
                     const glm::mat4 &viewMatrix, const glm::mat4 &roottrans,
                     glm::mat4 &transM, glm::mat4 &transScale,
                     glm::mat4 &rotateViewMatrix, int curr_mode, bool pick) {

  shader.enable();
  {
    //-- Set ModelView matrix:
    GLint location = shader.getUniformLocation("ModelView");
    mat4 modelView = viewMatrix * roottrans * transM * node.trans *
                     node.transScale * transScale;
    glUniformMatrix4fv(location, 1, GL_FALSE, value_ptr(modelView));
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
  mat4 modeltrans = mat4(1.0f);
  mat4 modeltransS = mat4(1.0f);
  renderSceneGraph(*m_rootNode, modeltrans, modeltransS);

  glDisable(GL_DEPTH_TEST);
  if (Backface_culling || Frontface_culling) {
    glDisable(GL_CULL_FACE);
  }
  if (Circle) {
    renderArcCircle();
  }
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
                         transScale, rotateViewMatrix, curr_mode, pick);

    // Get the BatchInfo corresponding to the GeometryNode's unique MeshId.
    BatchInfo batchInfo = m_batchInfoMap[geometryNode->meshId];

    //-- Now render the mesh:
    m_shader.enable();
    glDrawArrays(GL_TRIANGLES, batchInfo.startIndex, batchInfo.numIndices);
    m_shader.disable();
  }

  glBindVertexArray(0);

  for (const SceneNode *node : root.children) {
    renderSceneGraph(*node, transM, transScale); // recursion
  }
  CHECK_GL_ERRORS;
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

  if (curr_mode == 0) {
    float x_amount = (xpos - initX) / TRANSSCALE;
    float y_amount = -1 * (ypos - initY) / TRANSSCALE;
    movePuppetXYZ(x_amount, y_amount, idx);

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
  } else {
    bool posdirX = (xpos > prevX) ? true : false;
    bool posdirY = (ypos > prevY) ? true : false;
    if (midSelect && leftSelect) {
      rotateJoints(xpos, ypos, true, posdirX, posdirY);
      status_change = true;
    } else if (midSelect) {
      rotateJoints(xpos, ypos, false, posdirX, posdirY);
      status_change = true;
    } else if (rightSelect) {
      rotateHead(xpos, ypos);
      status_change = true;
    }
  }
  prevX = xpos;
  prevY = ypos;

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
    if (key == GLFW_KEY_S) {
      resetJoints();
    }
    if (key == GLFW_KEY_A) {
      resetPos();
      resetOrin();
      resetJoints();
    }
    if (key == GLFW_KEY_U) {
      undo();
    }
    if (key == GLFW_KEY_R) {
      redo();
    }
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
    if (key == GLFW_KEY_P) {
      curr_mode = 0;
      pick = false;
    }
    if (key == GLFW_KEY_J) {
      curr_mode = 1;
    }
  }
  // Fill in with event handling code...

  return eventHandled;
}
