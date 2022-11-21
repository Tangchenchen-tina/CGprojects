-- Create the top level root node named 'root'.
rootNode = gr.node('root')

bluegreen = gr.material({0.32, 0.87, 0.76}, {0.8, 0.8, 0.8}, 10.0)
orange = gr.material({1, 0.647, 0}, {0.8, 0.8, 0.8}, 10.0)
black = gr.material({0, 0, 0}, {0.8, 0.8, 0.8}, 10.0)
Upscale = 1.2
Forescale = 5

mat1 = gr.material({0.7, 1.0, 0.7}, {0.5, 0.7, 0.5}, 25)
mat2 = gr.material({0.5, 0.5, 0.5}, {0.5, 0.7, 0.5}, 25)
mat3 = gr.material({1.0, 0.6, 0.1}, {0.5, 0.7, 0.5}, 25)
mat4 = gr.material({0.7, 0.6, 1.0}, {0.5, 0.4, 0.8}, 25)
---------------- Torso ------------------
TorsoNode = gr.node('torsenode')
rootNode:add_child(TorsoNode)

Torse = gr.mesh('cylinder', 'torse')
Torse:scale(0.2, 0.3, 0.2)
Torse:translate(0, -1.2+Upscale, 0)
Torse:set_material(orange)
TorsoNode:add_child(Torse)

--------------- Shoulder ----------------
ShoulderNode = gr.node('shouldernode')
ShoulderNode:translate(0.0, 0.2, 0)
TorsoNode:add_child(ShoulderNode)
ShoulderJoint = gr.joint('shoulderJoint',{0, 0, 0}, {-60, 0, 60}, {0, 0, 0})
ShoulderNode:add_child(ShoulderJoint)


cloth1 = gr.mesh('cube', 'cloth1')
cloth1:scale(0.15,0.03,0.3)
cloth1:rotate('z', -70)
cloth1:rotate('x', 25)
cloth1:rotate('y', -10)
cloth1:translate(0.07, 0.45, 0.05)
cloth1:set_material(orange)
ShoulderJoint:add_child(cloth1)

cloth2 = gr.mesh('cube', 'cloth2')
cloth2:scale(0.15,0.03,0.3)
cloth2:rotate('z', 70)
cloth2:rotate('x', 25)
cloth2:rotate('y', -10)
cloth2:translate(0.03, 0.45, 0.05)
cloth2:set_material(orange)
ShoulderJoint:add_child(cloth2)

cloth3 = gr.mesh('cube', 'cloth3')
cloth3:scale(0.15,0.03,0.3)
cloth3:rotate('z', -70)
cloth3:rotate('x', 25)
cloth3:rotate('y', 10)
cloth3:translate(-0.03, 0.45, 0.05)
cloth3:set_material(orange)
ShoulderJoint:add_child(cloth3)

cloth4 = gr.mesh('cube', 'cloth4')
cloth4:scale(0.15,0.03,0.3)
cloth4:rotate('z', 70)
cloth4:rotate('x', 25)
cloth4:rotate('y', 10)
cloth4:translate(-0.07, 0.45, 0.05)
cloth4:set_material(orange)
ShoulderJoint:add_child(cloth4)

cloth5 = gr.mesh('cube', 'cloth5')
cloth5:scale(0.2,0.03,0.2)
cloth5:rotate('z', 90)
cloth5:rotate('y', 90)
cloth5:rotate('x', 30)
cloth5:translate(0, 0.5, -0.1)
cloth5:set_material(orange)
ShoulderJoint:add_child(cloth5)

-- Improve to look like cloth 
UpShoulder = gr.mesh('sphere', 'up_shoulder')
UpShoulder:scale(0.2, 0.15, 0.2)
UpShoulder:translate(0.0, 0.3, 0)
UpShoulder:set_material(orange)
ShoulderJoint:add_child(UpShoulder)

BottomShoulder = gr.mesh('cylinder', 'bottom_shoulder')
BottomShoulder:scale(0.2, 0.1, 0.2)
BottomShoulder:translate(0.0, 0.18, 0)
BottomShoulder:set_material(orange)
ShoulderJoint:add_child(BottomShoulder)


--------------- Neck -----------------
NeckNode = gr.node('necknode')
NeckNode:translate(0.0, 0.4, 0)
ShoulderJoint:add_child(NeckNode)

NeckJoint = gr.joint('neckJoint',{0, 0, 0}, {-20, 0, 20}, {-20, 0, 20})
NeckNode:add_child(NeckJoint)

Neck = gr.mesh('cylinder', 'neck')
Neck:scale(0.06, 0.4, 0.06)
Neck:translate(0.0, 0.2, 0)
Neck:set_material(bluegreen)
NeckJoint:add_child(Neck)

-- -- --------------- Head -----------------

HeadNode = gr.node('headNode')
HeadNode:translate(0.0, 0.6, 0)
NeckJoint:add_child(HeadNode)

HeadJoint = gr.joint('headJoint', {0, 0, 0}, {-30, 0, 30}, {-20, 0, 20})
HeadNode:add_child(HeadJoint)

UpHead = gr.mesh('sphere', 'up_head')
UpHead:scale(0.72, 0.51, 0.6)
UpHead:translate(0.0, 0.8, -0.1)
--UpHead:translate(0.0, 0.5, -0)
UpHead:set_material(bluegreen)
HeadJoint:add_child(UpHead)

LeftEye = gr.mesh('sphere', 'left_eye')
LeftEye:scale(0.15, 0.23, 0.15)
LeftEye:translate(-0.16, 0.5, 0.33)
LeftEye:set_material(gr.material({0.94, 0.9, 0.55}, {0.8, 0.8, 0.8}, 10.0))
HeadJoint:add_child(LeftEye)

RightEye = gr.mesh('sphere', 'right_eye')
RightEye:scale(0.15, 0.23, 0.15)
RightEye:translate(0.16, 0.5, 0.33)
RightEye:set_material(gr.material({0.94, 0.9, 0.55}, {0.8, 0.8, 0.8}, 10.0))
HeadJoint:add_child(RightEye)

LeftEyeCenter = gr.mesh('sphere', 'left_eyecenter')
LeftEyeCenter:scale(0.02, 0.02, 0.04)
LeftEyeCenter:translate(-0.16, 0.4, 0.45)
LeftEyeCenter:set_material(black)
HeadJoint:add_child(LeftEyeCenter)

RightEyeCenter = gr.mesh('sphere', 'right_eyecenter')
RightEyeCenter:scale(0.02, 0.02, 0.04)
RightEyeCenter:translate(0.16, 0.4, 0.45)
RightEyeCenter:set_material(black)
HeadJoint:add_child(RightEyeCenter)


LeftEyeSlash = gr.mesh('eyeslash', 'left_eyeslash')
LeftEyeSlash:scale(0.18, 0.4, 0.25)
LeftEyeSlash:translate(-0.168, 0.48, 0.27)
LeftEyeSlash:set_material(bluegreen)
HeadJoint:add_child(LeftEyeSlash)

RightEyeSlash = gr.mesh('eyeslash', 'right_eyeslash')
RightEyeSlash:scale(0.18, 0.4, 0.25)
RightEyeSlash:translate(0.168, 0.48, 0.27)
RightEyeSlash:set_material(bluegreen)
HeadJoint:add_child(RightEyeSlash)


MidHead = gr.mesh('cylinder', 'mid_head')
MidHead:scale(0.34, 0.45, 0.35)
MidHead:translate(0.0, 0.45, 0)
MidHead:set_material(bluegreen)
HeadJoint:add_child(MidHead)

Mouth = gr.mesh('mouth', 'mouth')
Mouth:scale(0.085, 0.09, 0.07)
Mouth:translate(0.0, -0.05, 0.33)
Mouth:set_material(bluegreen)
HeadJoint:add_child(Mouth)

MouthLine = gr.mesh('mouth', 'mouthline')
MouthLine:scale(0.07, 0.01, 0.03)
MouthLine:translate(0.0, -0.04, 0.4)
MouthLine:set_material(black)
HeadJoint:add_child(MouthLine)

Nose = gr.mesh('nose', 'nose')
Nose:scale(0.11, 0.11, 0.12)
Nose:rotate('x', -8.0)
Nose:rotate('z', -3.0)
Nose:translate(0.01, 0.1, 0.7)
Nose:set_material(bluegreen)
HeadJoint:add_child(Nose)

-- --------------- Left Upper Arm --------------
leftUpperArmNode = gr.node('leftupperarmnode')
leftUpperArmNode:translate(-0.2, 0.25, 0)
ShoulderJoint:add_child(leftUpperArmNode)

leftUpperArmJoint = gr.joint('leftUpperArmJoint',{-45, 0, 45}, {0, 0, 0}, {-100, 0, 20})
leftUpperArmNode:add_child(leftUpperArmJoint)

UpleftUpperArm = gr.mesh('sphere', 'up_leftUpperArm')
UpleftUpperArm:scale(0.1, 0.1, 0.1)
UpleftUpperArm:translate(0, 0, 0)
UpleftUpperArm:set_material(orange)
leftUpperArmJoint:add_child(UpleftUpperArm)

MidleftUpperArm = gr.mesh('cylinder', 'mid_leftUpperArm')
MidleftUpperArm:scale(0.09, 0.16, 0.09)
MidleftUpperArm:rotate('z', -30)
MidleftUpperArm:translate(-0.06, -0.1, 0)
MidleftUpperArm:set_material(orange)
leftUpperArmJoint:add_child(MidleftUpperArm)

BottomleftUpperArm = gr.mesh('cylinder', 'bottom_leftUpperArm')
BottomleftUpperArm:scale(0.06, 0.16, 0.06)
BottomleftUpperArm:rotate('z', -30)
BottomleftUpperArm:translate(-0.22, -0.37, 0)
BottomleftUpperArm:set_material(bluegreen)
leftUpperArmJoint:add_child(BottomleftUpperArm)

--------------- Left ForeArm ------------------------
leftForeArmNode = gr.node('leftforearmnode')
leftForeArmNode:translate(-0.31, -0.53, 0)
leftUpperArmJoint:add_child(leftForeArmNode)

leftForeArmJoint = gr.joint('leftForeArmJoint',{-45, 0, 45}, {0, 0, 0}, {-90, 0, 90})
leftForeArmNode:add_child(leftForeArmJoint)

UpleftForeArm = gr.mesh('sphere', 'up_leftForeArm')
UpleftForeArm:scale(0.08, 0.08, 0.08)
UpleftForeArm:translate(0, 0, 0)
UpleftForeArm:set_material(bluegreen)
leftForeArmJoint:add_child(UpleftForeArm)

MidleftForeArm = gr.mesh('cylinder', 'mid_leftForeArm')
MidleftForeArm:scale(0.06, 0.2, 0.06)
MidleftForeArm:translate(0, -0.25, 0)
MidleftForeArm:set_material(bluegreen)
leftForeArmJoint:add_child(MidleftForeArm)

-- -------------- Left hand 1 --------------------
HandNode1 = gr.node('hand')
HandNode1:translate(-0, -0.5, 0)
leftForeArmJoint:add_child(HandNode1)
HandJoint1 = gr.joint('handjoint1',{-30, 0, 30}, {0, 0, 0}, {-90, 0, 30})
HandNode1:add_child(HandJoint1)

hand1 = gr.mesh('foot', 'hand1')
hand1:scale(0.15, 0.15, 0.15)
hand1:rotate('x', 90)
hand1:rotate('y', -90)
hand1:translate(-0.05, -0.2, 0)
hand1:set_material(bluegreen)
HandJoint1:add_child(hand1)



--------------- Right Upper Arm --------------
rightUpperArmNode = gr.node('rightupperarmnode')
rightUpperArmNode:translate(0.2, 0.25, 0)
ShoulderJoint:add_child(rightUpperArmNode)

rightUpperArmJoint = gr.joint('rightUpperArmJoint',{-45, 0, 45}, {0, 0, 0}, {-20, 0, 100})
rightUpperArmNode:add_child(rightUpperArmJoint)

UprightUpperArm = gr.mesh('sphere', 'up_rightUpperArm')
UprightUpperArm:scale(0.1, 0.1, 0.1)
UprightUpperArm:translate(0, 0, 0)
UprightUpperArm:set_material(orange)
rightUpperArmJoint:add_child(UprightUpperArm)

MidrightUpperArm = gr.mesh('cylinder', 'mid_rightUpperArm')
MidrightUpperArm:scale(0.09, 0.16, 0.09)
MidrightUpperArm:rotate('z', 30)
MidrightUpperArm:translate(0.06, -0.1, 0)
MidrightUpperArm:set_material(orange)
rightUpperArmJoint:add_child(MidrightUpperArm)

BottomrightUpperArm = gr.mesh('cylinder', 'bottom_rightUpperArm')
BottomrightUpperArm:scale(0.06, 0.16, 0.06)
BottomrightUpperArm:rotate('z', 30)
BottomrightUpperArm:translate(0.22, -0.37, 0)
BottomrightUpperArm:set_material(bluegreen)
rightUpperArmJoint:add_child(BottomrightUpperArm)

-- --------------- right ForeArm ------------------------
rightForeArmNode = gr.node('rightforearmnode')
rightForeArmNode:translate(0.31, -0.53, 0)
rightUpperArmJoint:add_child(rightForeArmNode)

rightForeArmJoint = gr.joint('rightForeArmJoint',{-45, 0, 45}, {0, 0, 0}, {-90, 0, 90})
rightForeArmNode:add_child(rightForeArmJoint)

UprightForeArm = gr.mesh('sphere', 'up_rightForeArm')
UprightForeArm:scale(0.08, 0.08, 0.08)
UprightForeArm:translate(0, 0, 0)
UprightForeArm:set_material(bluegreen)
rightForeArmJoint:add_child(UprightForeArm)

MidrightForeArm = gr.mesh('cylinder', 'mid_rightForeArm')
MidrightForeArm:scale(0.06, 0.22, 0.06)
MidrightForeArm:translate(0, -0.25, 0)
MidrightForeArm:set_material(bluegreen)
rightForeArmJoint:add_child(MidrightForeArm)

-------------- Right hand 2 --------------------
HandNode2 = gr.node('hand2')
HandNode2:translate(0, -0.5, 0)
rightForeArmJoint:add_child(HandNode2)
HandJoint2 = gr.joint('handjoint2',{-30, 0, 30}, {0, 0, 0}, {-30, 0, 90})
HandNode2:add_child(HandJoint2)

hand2 = gr.mesh('foot', 'hand2')
hand2:scale(0.15, 0.15, 0.15)
hand2:rotate('x', 90)
hand2:rotate('y', 90)
hand2:translate(0.05, -0.2, 0)
hand2:set_material(bluegreen)
HandJoint2:add_child(hand2)

return rootNode