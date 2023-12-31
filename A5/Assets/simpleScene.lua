-- Simple Scene:
-- An extremely simple scene that will render right out of the box
-- with the provided skeleton code.  It doesn't rely on hierarchical
-- transformations.

-- Create the top level root node named 'root'.
S = 1.3
rootNode = gr.node('root')
rootNode:scale(S,S,S)

-- background = gr.mesh('cube', 'name-of-cube')
-- background:scale(30.0, 30.0, 1.0)
-- background:translate(0.0, -4*S, -20.0*S)
-- background:set_material(gr.material({0, 1, 0.2}, {0.8, 0.8, 0.8}, 10.0))
-- rootNode:add_child(background)

-- Create a GeometryNode with MeshId = 'cube', and name = 'torso'.
-- MeshId's must reference a .obj file that has been previously loaded into
-- the MeshConsolidator instance within A5::init().
cubeMesh = gr.mesh('cube', 'name-of-cube')
cubeMesh:scale(1.0, 1.0, 1.0)
cubeMesh:rotate('y', 45.0)
cubeMesh:translate(0.0, -0.5*S, -5.0*S)
cubeMesh:set_material(gr.material({0.8, 0.2, 0.2}, {0.8, 0.8, 0.8}, 10.0))

-- Add the cubeMesh GeometryNode to the child list of rootnode.
rootNode:add_child(cubeMesh)

-- Create a GeometryNode with MeshId = 'sphere', and name = 'head'.
sphereMesh = gr.mesh('sphere', 'name-of-sphere')
sphereMesh:scale(0.5, 0.5, 0.5)
sphereMesh:translate(0.0, 0.5*S, -5.0*S)
sphereMesh:set_material(gr.material({0.4, 0.8, 0.4}, {0.8, 0.8, 0.8}, 50.0))

-- Add the sphereMesh GeometryNode to the child list of rootnode.
rootNode:add_child(sphereMesh)


-- Return the root with all of it's childern.  The SceneNode A5::m_rootNode will be set
-- equal to the return value from this Lua script.
return rootNode
