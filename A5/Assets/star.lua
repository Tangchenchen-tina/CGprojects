-- Create the top level root node named 'root'.
rootNode = gr.node('root')

bluegreen = gr.material({0.32, 0.87, 0.76}, {0.8, 0.8, 0.8}, 10.0)
orange = gr.material({1, 0.647, 0}, {0.8, 0.8, 0.8}, 10.0)
black = gr.material({0, 0, 0}, {0.8, 0.8, 0.8}, 10.0)
pink = gr.material({1, 0.5, 0.5}, {0.4, 0.4, 0.4}, 10.0)
red = gr.material({1, 0.1, 0.1}, {0.4, 0.4, 0.4}, 10.0)
lightgreen = gr.material({0.651, 0.8392, 0.03}, {0.4, 0.4, 0.4}, 10.0)


zval = 1
yval = 2.35
xval = 0.04
Sstar = 0.27
---------------- Torso ------------------
StarTorsoNode = gr.node('startorsenode')
rootNode:add_child(StarTorsoNode)

StarTorse = gr.mesh('starbody', 'torse')
StarTorse:scale(0.2, 0.2, 0.2)
StarTorse:translate(0, yval-0.35, zval)
StarTorse:set_material(pink)
StarTorsoNode:add_child(StarTorse)

StarLeftEye = gr.mesh('sphere', 'Starleft_eye')
StarLeftEye:scale(Sstar*0.15, Sstar*0.23, Sstar*0.15)
StarLeftEye:translate(-xval, yval+0.04, zval-0.17)
StarLeftEye:set_material(gr.material({0.94, 0.9, 0.9}, {0.8, 0.8, 0.8}, 10.0))
StarTorsoNode:add_child(StarLeftEye)

StarRightEye = gr.mesh('sphere', 'Starright_eye')
StarRightEye:scale(Sstar*0.15, Sstar*0.23, Sstar*0.15)
StarRightEye:translate(xval, yval+0.04, zval-0.17)
StarRightEye:set_material(gr.material({0.94, 0.9, 0.9}, {0.8, 0.8, 0.8}, 10.0))
StarTorsoNode:add_child(StarRightEye)

StarLeftEyeCenter = gr.mesh('sphere', 'Starleft_eyecenter')
StarLeftEyeCenter:scale(Sstar*0.05, Sstar*0.05, Sstar*0.12)
StarLeftEyeCenter:translate(-xval, yval+0.04, zval-0.15)
StarLeftEyeCenter:set_material(black)
StarTorsoNode:add_child(StarLeftEyeCenter)

StarRightEyeCenter = gr.mesh('sphere', 'Starright_eyecenter')
StarRightEyeCenter:scale(Sstar*0.05, Sstar*0.05, Sstar*0.12)
StarRightEyeCenter:translate(xval, yval+0.04, zval-0.15)
StarRightEyeCenter:set_material(black)
StarTorsoNode:add_child(StarRightEyeCenter)

Starmouse = gr.mesh('starmouse', 'starmouse')
Starmouse:rotate('x',92)
Starmouse:scale(Sstar*0.3, Sstar*0.2, Sstar*0.3)
Starmouse:translate(0, yval-0.08, zval-0.13)
Starmouse:set_material(red)
StarTorsoNode:add_child(Starmouse)


Starpant = gr.mesh('starpant', 'starpant')
Starpant:scale(Sstar*0.95, Sstar*0.85, Sstar*0.95)
Starpant:translate(0, yval-0.35, zval-0.32)
Starpant:set_material(lightgreen)
StarTorsoNode:add_child(Starpant)

------------------------left arm---------------------

StarLeftArmNode = gr.node('leftarmNode')
StarLeftArmNode:translate(-0.16, yval-0.04-0.12, zval-0.2)
StarTorsoNode:add_child(StarLeftArmNode)

StarleftArmJoint = gr.joint('StarleftArmJoint',{-45, 0, 45}, {0, 0, 0}, {-90, 0, 90})
StarLeftArmNode:add_child(StarleftArmJoint)

StarLeftArm = gr.mesh('sphere', 'torse')
StarLeftArm:scale(0.05, 0.2, 0.05)
StarLeftArm:translate(0, 0.12, 0)
StarLeftArm:rotate('x', 90)
StarLeftArm:set_material(pink)
StarleftArmJoint:add_child(StarLeftArm)


------------------------right arm---------------------

StarRightArmNode = gr.node('leftarmNode')
StarRightArmNode:translate(0.16, yval-0.04-0.12, zval-0.2)
StarTorsoNode:add_child(StarRightArmNode)

StarRightArmJoint = gr.joint('StarleftArmJoint',{-45, 0, 45}, {0, 0, 0}, {-90, 0, 90})
StarRightArmNode:add_child(StarRightArmJoint)

StarRightArm = gr.mesh('sphere', 'torse')
StarRightArm:scale(0.05, 0.2, 0.05)
StarRightArm:translate(0, 0.12, 0)
StarRightArm:rotate('x', 90)
StarRightArm:set_material(pink)
StarRightArmJoint:add_child(StarRightArm)


return rootNode