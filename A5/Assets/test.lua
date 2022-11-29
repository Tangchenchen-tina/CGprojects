-- Create the top level root node named 'root'.
rootNode = gr.node('root')
rootScale = 3*10;

bluegreen = gr.material({0.32, 0.87, 0.76}, {0.8, 0.8, 0.8}, 10.0)
orange = gr.material({1, 0.4, 0.03}, {0.8, 0.8, 0.8}, 10.0)
black = gr.material({0, 0, 0}, {0.8, 0.8, 0.8}, 10.0)
grass = gr.material({0.1, 0.7, 0.1}, {0.2, 0.2, 0.2}, 0)
sand = gr.material({0.97, 0.90, 0.66}, {0.1,0.1,0.1}, 0)
brown = gr.material({0.6, 0.38, 0.09}, {0.2,0.2,0.2}, 0)
brownyellow = gr.material({0.84, 0.59, 0.24}, {0.2,0.2,0.2}, 0)
red = gr.material({1, 0.12, 0.10}, {0.2,0.2,0.2}, 0)
blue = gr.material({0.1, 0.12, 1}, {0.2,0.2,0.2}, 0)
pink = gr.material({1, 0.5, 0.5}, {0.4, 0.4, 0.4}, 10.0)
darkpink = gr.material({1, 0.4, 0.4}, {0.4, 0.4, 0.4}, 10.0)
red = gr.material({1, 0.1, 0.1}, {0.4, 0.4, 0.4}, 10.0)
lightgreen = gr.material({0.651, 0.8392, 0.03}, {0.4, 0.4, 0.4}, 10.0)

darkred = gr.material({0.698, 0.13, 0.13}, {0.4, 0.4, 0.4}, 10.0)
darkorange = gr.material({0.93, 0.17, 0.17}, {0.4, 0.4, 0.4}, 10.0)
darkpurple = gr.material({1, 0.0, 0.5}, {0.4, 0.4, 0.4}, 10.0)
darkyellow = gr.material({1, 0.8, 0.07}, {0.4, 0.4, 0.4}, 10.0)


scale = 1.05*rootScale
sphere_down = -1.8
sphere_forward = 2

--------------------------------------------------
------------------- Scene-------------------------
--------------------------------------------------
-- bullet = gr.mesh('sphere', 'bullet')
-- bullet:scale(2, 2, 2)  -- d = 2 when scale = 1
-- bullet:translate(-10.5, 37.5, 54)
-- bullet:set_material(red)
-- rootNode:add_child(bullet)

-- bullet2 = gr.mesh('sphere', 'bullet')
-- bullet2:scale(2, 2, 2)  -- d = 2 when scale = 1
-- bullet2:translate(-10.5, 40, 51.5)
-- bullet2:set_material(grass)
-- rootNode:add_child(bullet2)

-- bullet3 = gr.mesh('sphere', 'bullet')
-- bullet3:scale(2, 2, 2)  -- d = 2 when scale = 1
-- bullet3:translate(-10.5, 42, 55)
-- bullet3:set_material(darkyellow)
-- rootNode:add_child(bullet3)

-- ---------------- OuterSphere & ------------------
-- outerSphereNode = gr.node('outerSphereNode')
-- rootNode:add_child(outerSphereNode)

-- outerSphereJoint = gr.joint('outerSphereJoint',{0, 0, 0}, {-20, 0, 20}, {-20, 0, 20})
-- outerSphereNode:add_child(outerSphereJoint)

-- outerSphere = gr.mesh('outerSphere', 'outerSphere')
-- outerSphere:scale(1*scale, 1*scale, 1*scale)
-- outerSphere:translate(0, 0, 0)
-- outerSphere:set_material(grass)
-- outerSphereNode:add_child(outerSphere)

---------------- InnerSphere & ------------------
innerSphereNode = gr.node('innerSphereNode')
rootNode:add_child(innerSphereNode)


house = gr.mesh('squirdhouse', 'house')
house:scale(1.0041*5, 1.0041*5, 1.0041*5)
house:translate(0, 30, -10)
house:set_material(red)
innerSphereNode:add_child(house)

-- house = gr.mesh('cube', 'house')
-- house:scale(1.0041*10, 1.0041*10, 1.0041*10)
-- house:translate(0, 30, -10)
-- house:set_material(red)
-- innerSphereNode:add_child(house)

-- --------------------------------------------------
-- ------------------- Plants---------------------
-- --------------------------------------------------


-- -------------- Plants &------------------------

-- plant1 = gr.mesh('plant1', 'plant1')
-- outerSphereNode:add_child(plant1)
-- plant1:rotate('z', -30)
-- plant1:rotate('x', 40)
-- plant1:translate(2/3*rootScale,4.5/3*rootScale,4.5/3*rootScale)  --outer: 44.5
-- plant1:scale(0.1*scale, 0.1*scale, 0.1*scale)
-- plant1:set_lnode(8, 1, 0, 15, 1)
-- plant1:set_material(darkred)

-- plant2 = gr.mesh('plant1', 'plant2')
-- outerSphereNode:add_child(plant2)
-- plant2:rotate('z', 15)
-- plant2:rotate('x', 40)
-- plant2:translate(-2/3*rootScale,5.5/3*rootScale,2/3*rootScale) -- inner: 38.25
-- plant2:scale(0.15*scale, 0.12*scale, 0.15*scale)
-- plant2:set_lnode(12, 1, 0, 30, 1)
-- plant2:set_material(darkorange)

-- plant3 = gr.mesh('plant1', 'plant3')
-- outerSphereNode:add_child(plant3)
-- plant3:rotate('z', -35)
-- plant3:translate(3/3*rootScale,5.9/3*rootScale,0)
-- plant3:scale(0.1*scale, 0.07*scale, 0.1*scale)
-- plant3:set_lnode(15, 1, 0, 20, 1)
-- plant3:set_material(darkpurple)

-- plant4 = gr.mesh('plant1', 'plant4')
-- outerSphereNode:add_child(plant4)
-- plant4:translate(0/3*rootScale,6/3*rootScale,-1.5/3*rootScale)
-- plant4:scale(0.1*scale, 0.05*scale, 0.1*scale)
-- plant4:set_lnode(10, 1, 0, 15, 1)
-- plant4:set_material(darkred)

-- plant5 = gr.mesh('plant1', 'plant5')
-- outerSphereNode:add_child(plant5)
-- plant5:rotate('z', 15)
-- plant5:rotate('x',-30)
-- plant5:translate(-2/3*rootScale,5/3*rootScale,-3/3*rootScale)
-- plant5:scale(0.1*scale, 0.08*scale, 0.1*scale)
-- plant5:set_lnode(20, 1, 0, 15, 1)
-- plant5:set_material(darkred)

-- plant6 = gr.mesh('plant1', 'plant6')
-- outerSphereNode:add_child(plant6)
-- plant6:rotate('z', -20)
-- plant6:rotate('x',-90)
-- plant6:translate(2.2/3*rootScale,0.5/3*rootScale,-6.1/3*rootScale)
-- plant6:scale(0.06*scale, 0.06*scale, 0.06*scale)
-- plant6:set_lnode(15, 1, 0, 20, 1)
-- plant6:set_material(darkyellow)


-- plant7 = gr.mesh('plant1', 'plant7')
-- outerSphereNode:add_child(plant7)
-- plant7:rotate('z', -20)
-- plant7:rotate('x',-90)
-- plant7:translate(2/3*rootScale,0.5/3*rootScale,-6.2/3*rootScale)
-- plant7:scale(0.1*scale, 0.1*scale, 0.1*scale)
-- plant7:set_lnode(10, 1, 0, 15, 1)
-- plant7:set_material(orange)

-- plant8 = gr.mesh('plant1', 'plant8')
-- outerSphereNode:add_child(plant8)
-- plant8:rotate('z', -20)
-- plant8:rotate('x',-95)
-- plant8:translate(2.6/3*rootScale,0.1/3*rootScale,-6/3*rootScale)
-- plant8:scale(0.12*scale, 0.11*scale, 0.12*scale)
-- plant8:set_lnode(20, 1, 0, 30, 1)
-- plant8:set_material(darkpurple)


-- plant9 = gr.mesh('plant1', 'plant9')
-- outerSphereNode:add_child(plant9)
-- plant9:rotate('z', 30)
-- plant9:rotate('x',-150)
-- plant9:translate(-3.5/3*rootScale,-4.5/3*rootScale,-2.9/3*rootScale)
-- plant9:scale(0.15*scale, 0.13*scale, 0.15*scale)
-- plant9:set_lnode(20, 1, 0, 20, 1)
-- plant9:set_material(darkorange)


-- plant10 = gr.mesh('plant1', 'plant9')
-- outerSphereNode:add_child(plant10)
-- -- plant10:rotate('z', 30)
-- plant10:rotate('x',-180)
-- plant10:translate(-0.3/3*rootScale,-5.8/3*rootScale,2.5/3*rootScale)
-- plant10:scale(0.08*scale, 0.04*scale, 0.08*scale)
-- plant10:set_lnode(15, 1, 0, 20, 1)
-- plant10:set_material(darkpurple)

-- plant11 = gr.mesh('plant1', 'plant11')
-- outerSphereNode:add_child(plant11)
-- plant11:rotate('z', 40)
-- plant11:rotate('x',120)
-- plant11:translate(-4/3*rootScale, -2/3*rootScale, 4.5/3*rootScale)
-- plant11:scale(0.2*scale, 0.15*scale, 0.2*scale)
-- plant11:set_lnode(10, 1, 0, 20, 1)
-- plant11:set_material(darkred)

-- plant12 = gr.mesh('plant1', 'plant12')
-- outerSphereNode:add_child(plant12)
-- plant12:rotate('z', 40)
-- plant12:rotate('x',120)
-- plant12:translate(-4/3*rootScale, -2/3*rootScale, 4.5/3*rootScale)
-- plant12:scale(0.07*scale, 0.05*scale, 0.07*scale)
-- plant12:set_lnode(30, 1, 0, 20, 1)
-- plant12:set_material(orange)

-- plant13 = gr.mesh('plant1', 'plant13')
-- outerSphereNode:add_child(plant13)
-- plant13:rotate('z', 40)
-- plant13:rotate('x',120)
-- plant13:translate(-4/3*rootScale, -1.7/3*rootScale, 4.5/3*rootScale)
-- plant13:scale(0.1*scale, 0.1*scale, 0.1*scale)
-- plant13:set_lnode(25, 1, 0, 20, 1)
-- plant13:set_material(darkyellow)

-- -------------------- Grass & -------------------

-- grass1 = gr.mesh('plant1', 'grass1')
-- outerSphereNode:add_child(grass1)
-- grass1:rotate('z', 30)
-- grass1:rotate('x', 40)
-- grass1:translate(-2/3*rootScale,4.2/3*rootScale,4.5/3*rootScale)  --outer: 44.5
-- grass1:scale(0.15*scale, 0.25*scale, 0.15*scale)
-- grass1:set_lnode(3, 3, 1, 80, 0.25*scale)
-- grass1:set_material(orange)

-- grass2 = gr.mesh('plant1', 'grass2')
-- outerSphereNode:add_child(grass2)
-- grass2:rotate('z', -30)
-- grass2:rotate('x', 100)
-- grass2:translate(3/3*rootScale,-4.2/3*rootScale,3.7/3*rootScale)  --outer: 44.5
-- grass2:scale(0.15*scale, 0.3*scale, 0.15*scale)
-- grass2:set_lnode(3, 3, 1, 70, 0.3*scale)
-- grass2:set_material(orange)

-- --------------------------------------------------
-- ------------------- Character---------------------
-- --------------------------------------------------

-- xtrans_ship = -0.35*rootScale
-- ytrans_ship = 1.3*rootScale
-- ztrans_ship = 1.7*rootScale

-- ------------------------- Ship 1 &-----------------
-- shipNode1 = gr.node('shipNode1')
-- --shipNode1:translate(0,0,-2)
-- rootNode:add_child(shipNode1)

-- ship1 = gr.mesh('ship', 'ship1')
-- ship1:scale(0.18*rootScale, 0.22*rootScale, 0.18*rootScale)
-- ship1:rotate('y', -90)
-- ship1:rotate('x', 50)
-- ship1:translate(xtrans_ship, ytrans_ship, ztrans_ship)
-- ship1:set_material(brownyellow)
-- shipNode1:add_child(ship1)


-- shipStripeNode1 = gr.node('shipStripeNode1')
-- shipNode1:add_child(shipStripeNode1)

-- ship1Stripe1 = gr.mesh('shipMidStripe', 'ship1MidStripe1')
-- ship1Stripe1:scale(0.19*rootScale, 0.19*rootScale, 0.22*rootScale)
-- ship1Stripe1:rotate('y', -90)
-- ship1Stripe1:rotate('x', 50)
-- ship1Stripe1:translate(xtrans_ship, ytrans_ship, ztrans_ship)
-- ship1Stripe1:set_material(blue)
-- shipStripeNode1:add_child(ship1Stripe1)


-- ztrans_ship2 = 1.81*rootScale
-- -- ------------------------- Ship 2 &-----------------
-- shipNode2 = gr.node('shipNode2')
-- shipNode2:rotate('x', 50)
-- shipNode2:translate(-xtrans_ship, ytrans_ship-0.8, ztrans_ship2+0.18/0.22)
-- rootNode:add_child(shipNode2)
-- ship2Joint = gr.joint('ship2Joint',{0, 0, 0}, {-180, 0, 180}, {0, 0, 0})
-- --ship2Joint:rotate('y',-90)
-- shipNode2:add_child(ship2Joint)


-- ship2 = gr.mesh('ship', 'ship2')
-- ship2:scale(0.18*rootScale, 0.22*rootScale, 0.18*rootScale)
-- ship2:rotate('y', -90)
-- ship2:translate(0,-2.5,-0.11*rootScale)
-- ship2:set_material(brownyellow)
-- shipNode2:add_child(ship2)

-- ship2weapon = gr.mesh('weapon', 'weapon')
-- ship2weapon:scale(0.05*rootScale, 0.05*rootScale, 0.05*rootScale)
-- ship2weapon:rotate('y', -90)
-- -- ship2weapon:rotate('x', 50)
-- ship2weapon:translate(0, 0.066*rootScale-2.5, 0)
-- ship2weapon:set_material(darkpink)
-- ship2Joint:add_child(ship2weapon)

-- ship2weapon2 = gr.mesh('sphere', 'weapon2')
-- ship2weapon2:scale(1,1,1)
-- ship2weapon2:translate(0, 0.066*rootScale-2, 0)
-- ship2weapon2:set_material(darkpink)
-- ship2Joint:add_child(ship2weapon2)



-- shipStripeNode2 = gr.node('shipStripeNode2')
-- shipNode2:add_child(shipStripeNode2)

-- ship2Stripe1 = gr.mesh('shipMidStripe', 'ship1MidStripe2')
-- ship2Stripe1:scale(0.19*rootScale, 0.19*rootScale, 0.22*rootScale)
-- ship2Stripe1:rotate('y', -90)
-- ship2Stripe1:translate(0, -2.5, -0.11*rootScale)
-- ship2Stripe1:set_material(red)
-- shipStripeNode2:add_child(ship2Stripe1)

-- ------------------------------------------------
-- ----------------- Squidward & ---------------------
-- -------------------------------------------------

-- ztrans = 1.75*rootScale
-- ytrans = 1.34*rootScale
-- xtrans = -0.35*rootScale
-- S = 0.08*rootScale

-- Upscale = 1.2
-- Forescale = 5

-- ---------------- Torso ------------------
-- TorsoNode = gr.node('torsenode')
-- TorsoNode:scale(S,S,S)
-- TorsoNode:rotate('y', 180)
-- TorsoNode:rotate('x', 50)
-- TorsoNode:translate(xtrans, ytrans, ztrans)
-- shipNode1:add_child(TorsoNode)

-- Torse = gr.mesh('cylinder', 'torse')
-- Torse:scale(0.2, 0.3, 0.2)
-- Torse:translate(0, S*(-1.2+Upscale), 0)
-- Torse:set_material(orange)
-- TorsoNode:add_child(Torse)

-- --------------- Shoulder ----------------
-- ShoulderNode = gr.node('shouldernode')
-- ShoulderNode:translate(0.0, S*0.2, 0)
-- TorsoNode:add_child(ShoulderNode)
-- ShoulderJoint = gr.joint('shoulderJoint',{0, 0, 0}, {-60, 0, 60}, {0, 0, 0})
-- ShoulderNode:add_child(ShoulderJoint)

-- UpShoulder = gr.mesh('sphere', 'up_shoulder')
-- UpShoulder:scale(0.2, 0.15, 0.2)
-- UpShoulder:translate(0.0, S*0.3, 0)
-- UpShoulder:set_material(orange)
-- ShoulderJoint:add_child(UpShoulder)

-- BottomShoulder = gr.mesh('cylinder', 'bottom_shoulder')
-- BottomShoulder:scale(0.2, 0.1, 0.2)
-- BottomShoulder:translate(0.0, S*0.18, 0)
-- BottomShoulder:set_material(orange)
-- ShoulderJoint:add_child(BottomShoulder)

-- --------------- Neck -----------------
-- NeckNode = gr.node('necknode')
-- NeckNode:translate(0.0, S*0.4, 0)
-- ShoulderJoint:add_child(NeckNode)

-- NeckJoint = gr.joint('neckJoint',{0, 0, 0}, {-20, 0, 20}, {-20, 0, 20})
-- NeckNode:add_child(NeckJoint)

-- Neck = gr.mesh('cylinder', 'neck')
-- Neck:scale(0.06, 0.4, 0.06)
-- Neck:translate(0.0, S*0.2, 0)
-- Neck:set_material(bluegreen)
-- NeckJoint:add_child(Neck)

-- -- -- --------------- Head -----------------

-- HeadNode = gr.node('headNode')
-- HeadNode:translate(0.0, S*0.6, 0)
-- NeckJoint:add_child(HeadNode)

-- HeadJoint = gr.joint('headJoint', {0, 0, 0}, {-30, 0, 30}, {-20, 0, 20})
-- HeadNode:add_child(HeadJoint)

-- UpHead = gr.mesh('sphere', 'up_head')
-- UpHead:scale(0.72, 0.51, 0.6)
-- UpHead:translate(0.0, S*0.8, S*-0.1)
-- --UpHead:translate(0.0, 0.5, -0)
-- UpHead:set_material(bluegreen)
-- HeadJoint:add_child(UpHead)

-- LeftEye = gr.mesh('sphere', 'left_eye')
-- LeftEye:scale(0.15, 0.23, 0.15)
-- LeftEye:translate(S*-0.16, S*0.5, S*0.33)
-- LeftEye:set_material(gr.material({0.94, 0.9, 0.55}, {0.8, 0.8, 0.8}, 10.0))
-- HeadJoint:add_child(LeftEye)

-- RightEye = gr.mesh('sphere', 'right_eye')
-- RightEye:scale(0.15, 0.23, 0.15)
-- RightEye:translate(S*0.16, S*0.5, S*0.33)
-- RightEye:set_material(gr.material({0.94, 0.9, 0.55}, {0.8, 0.8, 0.8}, 10.0))
-- HeadJoint:add_child(RightEye)

-- LeftEyeCenter = gr.mesh('sphere', 'left_eyecenter')
-- LeftEyeCenter:scale(0.02, 0.02, 0.04)
-- LeftEyeCenter:translate(S*-0.16, S*0.4, S*0.45)
-- LeftEyeCenter:set_material(black)
-- HeadJoint:add_child(LeftEyeCenter)

-- RightEyeCenter = gr.mesh('sphere', 'right_eyecenter')
-- RightEyeCenter:scale(0.02, 0.02, 0.04)
-- RightEyeCenter:translate(S*0.16, S*0.4, S*0.45)
-- RightEyeCenter:set_material(black)
-- HeadJoint:add_child(RightEyeCenter)


-- LeftEyeSlash = gr.mesh('eyeslash', 'left_eyeslash')
-- LeftEyeSlash:scale(0.18, 0.4, 0.25)
-- LeftEyeSlash:translate(S*-0.168, S*0.48, S*0.27)
-- LeftEyeSlash:set_material(bluegreen)
-- HeadJoint:add_child(LeftEyeSlash)

-- RightEyeSlash = gr.mesh('eyeslash', 'right_eyeslash')
-- RightEyeSlash:scale(0.18, 0.4, 0.25)
-- RightEyeSlash:translate(S*0.168, S*0.48, S*0.27)
-- RightEyeSlash:set_material(bluegreen)
-- HeadJoint:add_child(RightEyeSlash)


-- MidHead = gr.mesh('cylinder', 'mid_head')
-- MidHead:scale(0.34, 0.45, 0.35)
-- MidHead:translate(S*0.0, S*0.45, 0)
-- MidHead:set_material(bluegreen)
-- HeadJoint:add_child(MidHead)

-- Mouth = gr.mesh('mouth', 'mouth')
-- Mouth:scale(0.085, 0.09, 0.07)
-- Mouth:translate(0.0, S*-0.05, S*0.33)
-- Mouth:set_material(bluegreen)
-- HeadJoint:add_child(Mouth)

-- MouthLine = gr.mesh('mouth', 'mouthline')
-- MouthLine:scale(0.07, 0.01, 0.03)
-- MouthLine:translate(0.0, S*-0.04, S*0.4)
-- MouthLine:set_material(black)
-- HeadJoint:add_child(MouthLine)

-- Nose = gr.mesh('nose', 'nose')
-- Nose:scale(0.11, 0.11, 0.12)
-- Nose:rotate('x', -8.0)
-- Nose:rotate('z', -3.0)
-- Nose:translate(S*0.01, S*0.1, S*0.7)
-- Nose:set_material(bluegreen)
-- HeadJoint:add_child(Nose)


-- -- --------------- Left Upper Arm --------------
-- leftUpperArmNode = gr.node('leftupperarmnode')
-- leftUpperArmNode:translate(S*-0.2, S*0.25, 0)
-- ShoulderJoint:add_child(leftUpperArmNode)

-- leftUpperArmJoint = gr.joint('leftUpperArmJoint',{-45, 0, 45}, {0, 0, 0}, {-100, 0, 20})
-- leftUpperArmNode:add_child(leftUpperArmJoint)

-- UpleftUpperArm = gr.mesh('sphere', 'up_leftUpperArm')
-- UpleftUpperArm:scale(0.1, 0.1, 0.1)
-- UpleftUpperArm:translate(0, 0, 0)
-- UpleftUpperArm:set_material(orange)
-- leftUpperArmJoint:add_child(UpleftUpperArm)

-- MidleftUpperArm = gr.mesh('cylinder', 'mid_leftUpperArm')
-- MidleftUpperArm:scale(0.09, 0.16, 0.09)
-- MidleftUpperArm:rotate('z', -30)
-- MidleftUpperArm:translate(S*-0.06, S*-0.1, 0)
-- MidleftUpperArm:set_material(orange)
-- leftUpperArmJoint:add_child(MidleftUpperArm)

-- BottomleftUpperArm = gr.mesh('cylinder', 'bottom_leftUpperArm')
-- BottomleftUpperArm:scale(0.06, 0.16, 0.06)
-- BottomleftUpperArm:rotate('z', -30)
-- BottomleftUpperArm:translate(S*-0.22, S*-0.37, 0)
-- BottomleftUpperArm:set_material(bluegreen)
-- leftUpperArmJoint:add_child(BottomleftUpperArm)

-- --------------- Left ForeArm ------------------------
-- leftForeArmNode = gr.node('leftforearmnode')
-- leftForeArmNode:rotate('x', -90)
-- leftForeArmNode:rotate('y', 45)
-- leftForeArmNode:translate(S*-0.31, S*-0.53, 0)
-- leftUpperArmJoint:add_child(leftForeArmNode)

-- leftForeArmJoint = gr.joint('leftForeArmJoint',{-45, 0, 45}, {0, 0, 0}, {-90, 0, 90})
-- leftForeArmNode:add_child(leftForeArmJoint)

-- UpleftForeArm = gr.mesh('sphere', 'up_leftForeArm')
-- UpleftForeArm:scale(0.08, 0.08, 0.08)
-- UpleftForeArm:translate(0, 0, 0)
-- UpleftForeArm:set_material(bluegreen)
-- leftForeArmJoint:add_child(UpleftForeArm)

-- MidleftForeArm = gr.mesh('cylinder', 'mid_leftForeArm')
-- MidleftForeArm:scale(0.06, 0.2, 0.06)
-- MidleftForeArm:translate(0, S*-0.25, 0)
-- MidleftForeArm:set_material(bluegreen)
-- leftForeArmJoint:add_child(MidleftForeArm)

-- -- -------------- Left hand 1 --------------------
-- HandNode1 = gr.node('hand')
-- HandNode1:translate(-0, S*-0.5, 0)
-- leftForeArmJoint:add_child(HandNode1)
-- HandJoint1 = gr.joint('handjoint1',{-30, 0, 30}, {0, 0, 0}, {-90, 0, 30})
-- HandNode1:add_child(HandJoint1)

-- hand1 = gr.mesh('foot', 'hand1')
-- hand1:scale(0.15, 0.15, 0.15)
-- hand1:rotate('x', 90)
-- hand1:rotate('y', -90)
-- hand1:translate(S*-0.05, S*-0.2, 0)
-- hand1:set_material(bluegreen)
-- HandJoint1:add_child(hand1)



-- --------------- Right Upper Arm --------------
-- rightUpperArmNode = gr.node('rightupperarmnode')
-- rightUpperArmNode:translate(S*0.2, S*0.25, 0)
-- ShoulderJoint:add_child(rightUpperArmNode)

-- rightUpperArmJoint = gr.joint('rightUpperArmJoint',{-45, 0, 45}, {0, 0, 0}, {-20, 0, 100})
-- rightUpperArmNode:add_child(rightUpperArmJoint)

-- UprightUpperArm = gr.mesh('sphere', 'up_rightUpperArm')
-- UprightUpperArm:scale(0.1, 0.1, 0.1)
-- UprightUpperArm:translate(0, 0, 0)
-- UprightUpperArm:set_material(orange)
-- rightUpperArmJoint:add_child(UprightUpperArm)

-- MidrightUpperArm = gr.mesh('cylinder', 'mid_rightUpperArm')
-- MidrightUpperArm:scale(0.09, 0.16, 0.09)
-- MidrightUpperArm:rotate('z', 30)
-- MidrightUpperArm:translate(S*0.06, S*-0.1, 0)
-- MidrightUpperArm:set_material(orange)
-- rightUpperArmJoint:add_child(MidrightUpperArm)

-- BottomrightUpperArm = gr.mesh('cylinder', 'bottom_rightUpperArm')
-- BottomrightUpperArm:scale(0.06, 0.16, 0.06)
-- BottomrightUpperArm:rotate('z', 30)
-- BottomrightUpperArm:translate(S*0.22, S*-0.37, 0)
-- BottomrightUpperArm:set_material(bluegreen)
-- rightUpperArmJoint:add_child(BottomrightUpperArm)

-- -- --------------- right ForeArm ------------------------
-- rightForeArmNode = gr.node('rightforearmnode')
-- rightForeArmNode:rotate('x', -90)
-- rightForeArmNode:rotate('y', -45)
-- rightForeArmNode:translate(S*0.31, S*-0.53, 0)
-- rightUpperArmJoint:add_child(rightForeArmNode)

-- rightForeArmJoint = gr.joint('rightForeArmJoint',{-45, 0, 45}, {0, 0, 0}, {-90, 0, 90})
-- rightForeArmNode:add_child(rightForeArmJoint)

-- UprightForeArm = gr.mesh('sphere', 'up_rightForeArm')
-- UprightForeArm:scale(0.08, 0.08, 0.08)
-- UprightForeArm:translate(0, 0, 0)
-- UprightForeArm:set_material(bluegreen)
-- rightForeArmJoint:add_child(UprightForeArm)

-- MidrightForeArm = gr.mesh('cylinder', 'mid_rightForeArm')
-- MidrightForeArm:scale(0.06, 0.22, 0.06)
-- MidrightForeArm:translate(0, S*-0.25, 0)
-- MidrightForeArm:set_material(bluegreen)
-- rightForeArmJoint:add_child(MidrightForeArm)

-- -------------- Right hand 2 --------------------
-- HandNode2 = gr.node('hand2')
-- HandNode2:translate(0, S*-0.5, 0)
-- rightForeArmJoint:add_child(HandNode2)
-- HandJoint2 = gr.joint('handjoint2',{-30, 0, 30}, {0, 0, 0}, {-30, 0, 90})
-- HandNode2:add_child(HandJoint2)

-- hand2 = gr.mesh('foot', 'hand2')
-- hand2:scale(0.15, 0.15, 0.15)
-- hand2:rotate('x', 90)
-- hand2:rotate('y', 90)
-- hand2:translate(S*0.05, S*-0.2, 0)
-- hand2:set_material(bluegreen)
-- HandJoint2:add_child(hand2)


-- ----------------------- Patrick Star & ---------------

-- zval = 1
-- yval = 2.35
-- xval = 0.04
-- Sstar = 0.27
-- S_star = 0.2*rootScale
-- ---------------- Torso ------------------
-- StarTorsoNode = gr.node('startorsenode')
-- StarTorsoNode:rotate('y',180)
-- StarTorsoNode:scale(S_star,S_star,S_star)
-- StarTorsoNode:translate(0.35*rootScale+xtrans_ship,0.96*rootScale-ytrans_ship-2.5,1.56*rootScale-ztrans_ship2+0.27*rootScale)
-- shipNode2:add_child(StarTorsoNode)

-- StarTorse = gr.mesh('starbody', 'startorse')
-- StarTorse:scale(0.2, 0.2, 0.2)
-- StarTorse:translate(0, S_star*(yval-0.35),  S_star*zval)
-- StarTorse:set_material(pink)
-- StarTorsoNode:add_child(StarTorse)

-- StarLeftEye = gr.mesh('sphere', 'Starleft_eye')
-- StarLeftEye:scale(Sstar*0.15, Sstar*0.23, Sstar*0.15)
-- StarLeftEye:translate( S_star*-xval,  S_star*(yval+0.04),  S_star*(zval-0.17))
-- StarLeftEye:set_material(gr.material({0.94, 0.9, 0.9}, {0.8, 0.8, 0.8}, 10.0))
-- StarTorsoNode:add_child(StarLeftEye)

-- StarRightEye = gr.mesh('sphere', 'Starright_eye')
-- StarRightEye:scale(Sstar*0.15, Sstar*0.23, Sstar*0.15)
-- StarRightEye:translate( S_star*xval,  S_star*(yval+0.04),  S_star*(zval-0.17))
-- StarRightEye:set_material(gr.material({0.94, 0.9, 0.9}, {0.8, 0.8, 0.8}, 10.0))
-- StarTorsoNode:add_child(StarRightEye)

-- StarLeftEyeCenter = gr.mesh('sphere', 'Starleft_eyecenter')
-- StarLeftEyeCenter:scale(Sstar*0.05, Sstar*0.05, Sstar*0.12)
-- StarLeftEyeCenter:translate( S_star*-xval,  S_star*(yval+0.04),  S_star*(zval-0.15))
-- StarLeftEyeCenter:set_material(black)
-- StarTorsoNode:add_child(StarLeftEyeCenter)

-- StarRightEyeCenter = gr.mesh('sphere', 'Starright_eyecenter')
-- StarRightEyeCenter:scale(Sstar*0.05, Sstar*0.05, Sstar*0.12)
-- StarRightEyeCenter:translate( S_star*xval,  S_star*(yval+0.04),  S_star*(zval-0.15))
-- StarRightEyeCenter:set_material(black)
-- StarTorsoNode:add_child(StarRightEyeCenter)

-- Starmouse = gr.mesh('starmouse', 'starmouse')
-- Starmouse:rotate('x',92)
-- Starmouse:scale(Sstar*0.3, Sstar*0.2, Sstar*0.3)
-- Starmouse:translate(0,  S_star*(yval-0.08),  S_star*(zval-0.13))
-- Starmouse:set_material(red)
-- StarTorsoNode:add_child(Starmouse)


-- Starpant = gr.mesh('starpant', 'starpant')
-- Starpant:scale(Sstar*0.95, Sstar*0.85, Sstar*0.95)
-- Starpant:translate(0,  S_star*(yval-0.35),  S_star*(zval-0.32))
-- Starpant:set_material(lightgreen)
-- StarTorsoNode:add_child(Starpant)

-- ------------------------left arm---------------------

-- StarLeftArmNode = gr.node('starleftarmNode')
-- StarLeftArmNode:translate( S_star*(-0.16),  S_star*(yval-0.04-0.12),  S_star*(zval-0.2))
-- StarTorsoNode:add_child(StarLeftArmNode)

-- StarleftArmJoint = gr.joint('StarleftArmJoint',{-45, 0, 45}, {0, 0, 0}, {-90, 0, 90})
-- StarLeftArmNode:add_child(StarleftArmJoint)

-- StarLeftArm = gr.mesh('sphere', 'starleftarm')
-- StarLeftArm:scale(0.05, 0.2, 0.05)
-- StarLeftArm:translate(0,  S_star*0.12, 0)
-- StarLeftArm:rotate('x', 90)
-- StarLeftArm:set_material(pink)
-- StarleftArmJoint:add_child(StarLeftArm)


-- ------------------------right arm---------------------

-- StarRightArmNode = gr.node('rightarmNode')
-- StarRightArmNode:translate( S_star*0.16,  S_star*(yval-0.04-0.12),  S_star*(zval-0.2))
-- StarTorsoNode:add_child(StarRightArmNode)

-- StarRightArmJoint = gr.joint('StarrightArmJoint',{-45, 0, 45}, {0, 0, 0}, {-90, 0, 90})
-- StarRightArmNode:add_child(StarRightArmJoint)

-- StarRightArm = gr.mesh('sphere', 'starrightarm')
-- StarRightArm:scale(0.05, 0.2, 0.05)
-- StarRightArm:translate(0,  S_star*0.12, 0)
-- StarRightArm:rotate('x', 90)
-- StarRightArm:set_material(pink)
-- StarRightArmJoint:add_child(StarRightArm)


return rootNode