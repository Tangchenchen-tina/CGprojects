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
pink = gr.material({1, 0.55, 0.55}, {0.4, 0.4, 0.4}, 10.0)
darkpink = gr.material({1, 0.4, 0.4}, {0.4, 0.4, 0.4}, 10.0)
red = gr.material({1, 0.1, 0.1}, {0.4, 0.4, 0.4}, 10.0)
lightgreen = gr.material({0.651, 0.8392, 0.03}, {0.4, 0.4, 0.4}, 10.0)
gray = gr.material({0.5, 0.5, 0.5}, {0.4, 0.4, 0.4}, 10.0)
white = gr.material({1, 1, 1}, {0.4, 0.4, 0.4}, 10.0)
lightblue = gr.material({0.3, 0.8, 1}, {0.4, 0.4, 0.4}, 20.0)
darkblue = gr.material({0, 0.2, 0.4}, {0.4, 0.4, 0.4}, 20.0)

darkred = gr.material({0.698, 0.13, 0.13}, {0.4, 0.4, 0.4}, 10.0)
darkorange = gr.material({0.93, 0.17, 0.17}, {0.4, 0.4, 0.4}, 10.0)
darkpurple = gr.material({1, 0.0, 0.5}, {0.4, 0.4, 0.4}, 10.0)
darkyellow = gr.material({1, 0.8, 0.07}, {0.4, 0.4, 0.4}, 10.0)
ddyellow = gr.material({1, 0.6, 0.07}, {0.4, 0.4, 0.4}, 10.0)


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

---------------- OuterSphere & ------------------
outerSphereNode = gr.node('outerSphereNode')
rootNode:add_child(outerSphereNode)

outerSphereJoint = gr.joint('outerSphereJoint',{0, 0, 0}, {-20, 0, 20}, {-20, 0, 20})
outerSphereNode:add_child(outerSphereJoint)

outerSphere = gr.mesh('outerSphere', 'outerSphere')
outerSphere:scale(1*scale, 1*scale, 1*scale)
outerSphere:translate(0, 0, 0)
outerSphere:set_material(grass)
outerSphereNode:add_child(outerSphere)

---------------- InnerSphere & ------------------
innerSphereNode = gr.node('innerSphereNode')
outerSphereNode:add_child(innerSphereNode)

innerSphere = gr.mesh('innerSphere', 'innerSphere')
innerSphere:scale(1.0041*scale, 1.0041*scale, 1.0041*scale)
innerSphere:translate(0, 0, 0)
innerSphere:set_material(sand)
innerSphereNode:add_child(innerSphere)

--------------------------------------------------
------------------- Jelly fish ---------------------
--------------------------------------------------
jellyFishNode1 = gr.node('jellyfish')
innerSphereNode:add_child(jellyFishNode1)

jellyfish1 = gr.mesh('jellyfish', 'j1')
jellyfish1:scale(2,2,2)
jellyfish1:rotate('y', 30)
jellyfish1:rotate('z', 30)
jellyfish1:translate(-2,75,0)
jellyfish1:set_material(pink)
jellyFishNode1:add_child(jellyfish1)

jellyfish2 = gr.mesh('jellyfish', 'j2')
jellyfish2:scale(2.5,2.5,2.5)
jellyfish2:rotate('y', 10)
jellyfish2:rotate('z', 10)
jellyfish2:rotate('x', 20)
jellyfish2:translate(-7,85,1)
jellyfish2:set_material(pink)
jellyFishNode1:add_child(jellyfish2)

jellyfish3 = gr.mesh('jellyfish', 'j3')
jellyfish3:scale(3,3,3)
jellyfish3:rotate('y', 80)
jellyfish3:rotate('z', -20)
jellyfish3:rotate('x', 50)
jellyfish3:translate(5,95,-4)
jellyfish3:set_material(pink)
jellyFishNode1:add_child(jellyfish3)

jellyfish4 = gr.mesh('jellyfish', 'j3')
jellyfish4:scale(1,1,1)
jellyfish4:rotate('y', 80)
jellyfish4:rotate('z', -10)
jellyfish4:rotate('x', 5)
jellyfish4:translate(-18,49,68)
jellyfish4:set_material(pink)
jellyFishNode1:add_child(jellyfish4)

jellyfish5 = gr.mesh('jellyfish', 'j3')
jellyfish5:scale(2,2,2)
jellyfish5:rotate('y', 80)
jellyfish5:rotate('z', -40)
jellyfish5:rotate('x', 40)
jellyfish5:translate(50,100,-4)
jellyfish5:set_material(darkpink)
jellyFishNode1:add_child(jellyfish5)

-------------

jellyFishNode2 = gr.node('jellyfish')
innerSphereNode:add_child(jellyFishNode2)

jellyfish6 = gr.mesh('jellyfish', 'j1')
jellyfish6:scale(2,2,2)
jellyfish6:rotate('y', 30)
jellyfish6:rotate('z', 30)
jellyfish6:translate(15,90,0)
jellyfish6:rotate('x', 200)
jellyfish6:set_material(pink)
jellyFishNode2:add_child(jellyfish6)

jellyfish7 = gr.mesh('jellyfish', 'j1')
jellyfish7:scale(4,4,4)
jellyfish7:rotate('y', 30)
jellyfish7:rotate('z', -40)
jellyfish7:translate(-20,80,10)
jellyfish7:rotate('x', 170)
jellyfish7:set_material(pink)
jellyFishNode2:add_child(jellyfish7)

jellyfish8 = gr.mesh('jellyfish', 'j1')
jellyfish8:scale(4,4,4)
jellyfish8:rotate('y', 30)
jellyfish8:rotate('z', 20)
jellyfish8:rotate('x', 30)
jellyfish8:translate(30,75,-5)
jellyfish8:rotate('x', 160)
jellyfish8:set_material(pink)
jellyFishNode2:add_child(jellyfish8)

-----------

jellyFishNode3 = gr.node('jellyfish')
innerSphereNode:add_child(jellyFishNode3)

jellyfish9 = gr.mesh('jellyfish', 'j1')
jellyfish9:scale(2,2,2)
jellyfish9:rotate('y', 30)
jellyfish9:rotate('z', 30)
jellyfish9:translate(-40,70,0)
jellyfish9:rotate('x', 90)
jellyfish9:set_material(pink)
jellyFishNode3:add_child(jellyfish9)

jellyfish10 = gr.mesh('jellyfish', 'j1')
jellyfish10:scale(2,2,2)
jellyfish10:rotate('y', 30)
jellyfish10:rotate('z', 10)
jellyfish10:rotate('x', 45)
jellyfish10:translate(-35,65,0)
jellyfish10:rotate('x', 90)
jellyfish10:set_material(pink)
jellyFishNode3:add_child(jellyfish10)

---------------

jellyFishNode4 = gr.node('jellyfish')
innerSphereNode:add_child(jellyFishNode4)

jellyfish11 = gr.mesh('jellyfish', 'j1')
jellyfish11:scale(1,1,1)
jellyfish11:rotate('y', 30)
jellyfish11:rotate('z', 10)
jellyfish11:rotate('x', 45)
jellyfish11:translate(0,70,0)
jellyfish11:rotate('x', -65)
jellyfish11:rotate('y', -30)
jellyfish11:set_material(pink)
jellyFishNode4:add_child(jellyfish11)

jellyfish12 = gr.mesh('jellyfish', 'j1')
jellyfish12:scale(1,1,1)
jellyfish12:rotate('y', 45)
jellyfish12:rotate('z', 20)
jellyfish12:rotate('x', 20)
jellyfish12:translate(3,68,2)
jellyfish12:rotate('x', -65)
jellyfish12:rotate('y', -30)
jellyfish12:set_material(pink)
jellyFishNode4:add_child(jellyfish12)

jellyfish13 = gr.mesh('jellyfish', 'j1')
jellyfish13:scale(1.5,1.5,1.5)
jellyfish13:rotate('y', 45)
jellyfish13:rotate('z', 20)
jellyfish13:rotate('x', -20)
jellyfish13:translate(-4,73,2)
jellyfish13:rotate('x', -65)
jellyfish13:rotate('y', -30)
jellyfish13:set_material(pink)
jellyFishNode4:add_child(jellyfish13)

jellyfish14 = gr.mesh('jellyfish', 'j1')
jellyfish14:scale(1.5,1.5,1.5)
jellyfish14:rotate('y', 80)
jellyfish14:rotate('z', -10)
jellyfish14:rotate('x', -10)
jellyfish14:translate(5,73,-4)
jellyfish14:rotate('x', -65)
jellyfish14:rotate('y', -30)
jellyfish14:set_material(pink)
jellyFishNode4:add_child(jellyfish14)

jellyfish15 = gr.mesh('jellyfish', 'j1')
jellyfish15:scale(2,2,2)
jellyfish15:rotate('y', 85)
jellyfish15:rotate('z', 0)
jellyfish15:rotate('x', -10)
jellyfish15:translate(3,80,-1)
jellyfish15:rotate('x', -65)
jellyfish15:rotate('y', -30)
jellyfish15:set_material(pink)
jellyFishNode4:add_child(jellyfish15)



--------------------------------------------------
------------------- stones---------------------
--------------------------------------------------
stone1 = gr.mesh('smallarch', 'stone1')
stone1:scale(0.2,0.2,0.2)
stone1:translate(0,70,0)
stone1:rotate('y', -45)
stone1:rotate('z', -30)
stone1:rotate('x', 65)
stone1:set_material(gray)
innerSphereNode:add_child(stone1)

stone2 = gr.mesh('smallarch', 'stone2')
stone2:scale(0.1,0.12,0.1)
stone2:translate(0,65,0)
stone2:rotate('y', 45)
stone2:rotate('z', 25)
stone2:rotate('x', -85)
stone2:set_material(gray)
innerSphereNode:add_child(stone2)

stone5 = gr.mesh('smallarch', 'stone2')
stone5:scale(0.2,0.3,0.2)
stone5:translate(0,65,0)
stone5:rotate('y', 40)
stone5:rotate('z', 28)
stone5:rotate('x', -55)
stone5:set_material(gray)
innerSphereNode:add_child(stone5)

stone6 = gr.mesh('smallarch', 'stone2')
stone6:scale(0.11,0.16,0.11)
stone6:translate(0,65,0)
stone6:rotate('y', 20)
stone6:rotate('z', 25)
stone6:rotate('x', -30)
stone6:set_material(gray)
innerSphereNode:add_child(stone6)

stone3 = gr.mesh('rockset', 'stone3')
stone3:scale(0.6,0.7,0.6)
stone3:translate(0,65,0)
stone3:rotate('y', -90)
stone3:rotate('z', -90)
stone3:rotate('y', -30)
stone3:set_material(gray)
innerSphereNode:add_child(stone3)

stone4 = gr.mesh('rockset', 'stone4')
stone4:scale(0.5,1,0.5)
stone4:translate(0,62,0)
stone4:rotate('z', -65)
stone4:rotate('y', 60)
stone4:set_material(gray)
innerSphereNode:add_child(stone4)

stone8 = gr.mesh('rockset', 'stone4')
stone8:scale(0.3,0.4,0.3)
stone8:translate(0,62,0)
stone8:rotate('x', 80)
stone8:set_material(gray)
innerSphereNode:add_child(stone8)

stone7 = gr.mesh('rockset2', 'stone4')
stone7:scale(0.25,0.7,0.25)
stone7:rotate('y', 50)
stone7:translate(0,62,0)
stone7:rotate('x', 130)
stone7:rotate('y', -20)
stone7:set_material(gray)
innerSphereNode:add_child(stone7)

stone9 = gr.mesh('rockset2', 'stone4')
stone9:scale(0.28,1,0.28)
stone9:rotate('y', -230)
stone9:translate(0,62,0)
stone9:rotate('x', 190)
stone9:rotate('z', 20)
stone9:set_material(gray)
innerSphereNode:add_child(stone9)

--------------------------------------------------
------------------- boards---------------------
--------------------------------------------------
boardNode1 = gr.node('board1')
innerSphereNode:add_child(boardNode1)

board1 = gr.mesh('board', 'board1')
board1:scale(3,3,3)
board1:rotate('y', -120)
board1:rotate('x', 30)
board1:rotate('z', -10)
board1:translate(20, 62, 30)
board1:set_material(brown)
boardNode1:add_child(board1)

boardbottom1 = gr.mesh('cube', 'board1')
boardbottom1:scale(0.75,4.6,7.5)
boardbottom1:rotate('y', -120)
boardbottom1:rotate('x', 30)
boardbottom1:rotate('z', -10)
boardbottom1:translate(19.8, 62, 30.4)
boardbottom1:set_material(white)
boardbottom1:set_texture(1)
boardNode1:add_child(boardbottom1)


--------------------------------------------------
------------------- houses---------------------
--------------------------------------------------
house1Node = gr.node('house1')
innerSphereNode:add_child(house1Node)

house = gr.mesh('squirdhouse', 'house')
house:scale(1.0041*4, 1.0041*4, 1.0041*4)
house:rotate('y', -60)
house:rotate('z', 25)
house:rotate('x', 15)
house:translate(-40, 55, 20)
house:set_material(gray)
house1Node:add_child(house)

door1 = gr.mesh('door', 'd1')
door1:scale(0.25,0.25,0.25)
door1:rotate('y', 180)
door1:translate(6, -5, 0)
door1:set_material(brown)
house:add_child(door1)

house1w1 = gr.mesh('window', 'w1')
house1w1:scale(0.4,0.4,0.4)
house1w1:translate(5,4,-2.5)
house1w1:set_material(lightblue)
house:add_child(house1w1)

house1w2 = gr.mesh('window', 'w1')
house1w2:scale(0.4,0.4,0.4)
house1w2:translate(5,4,3)
house1w2:set_material(lightblue)
house:add_child(house1w2)

-----------------------
house2Node = gr.node('house2')
innerSphereNode:add_child(house2Node)

house2 = gr.mesh('sphere', 'house')
house2:scale(8, 8, 8)
house2:rotate('z', -20)
house2:translate(25, 57, 20) -- x=25 55 20
house2:rotate('x', 20)
house2:rotate('x', 60)
house2:set_material(ddyellow)
house2Node:add_child(house2)

door2 = gr.mesh('door', 'd1')
door2:scale(0.2,0.15,0.15)
door2:rotate('y', 110)
door2:translate(3, -3, 7)
door2:set_material(lightblue)
house2:add_child(door2)

house2w1 = gr.mesh('window', 'w1')
house2w1:scale(0.2,0.2,0.2)
house2w1:rotate('y', -90)
house2w1:rotate('x', -45)
house2w1:rotate('z', -45)
house2w1:translate(5,4,5)
house2w1:set_material(lightblue)
house2:add_child(house2w1)

house2w2 = gr.mesh('window', 'w1')
house2w2:scale(0.3,0.3,0.3)
house2w2:rotate('y', -90)
house2w2:rotate('x', -20)
house2w2:rotate('z', 60)
house2w2:translate(-2,3,7)
house2w2:set_material(lightblue)
house2:add_child(house2w2)


house3Node = gr.node('house3')
innerSphereNode:add_child(house3Node)

house3 = gr.mesh('squirdhouse', 'house')
house3:scale(3, 3, 3)
house3:rotate('y', 150)
house3:rotate('x',20)
--house3:rotate('x',10)
house3:translate(0, 65, 20)
house3:set_material(darkblue)
house3:rotate('z', -210)
house3Node:add_child(house3)

door3 = gr.mesh('door', 'd1')
door3:scale(0.15,0.15,0.15)
door3:rotate('y', 180)
door3:translate(4.5, -3, 0)
door3:set_material(brown)
house3:add_child(door3)

house3w1 = gr.mesh('window', 'w1')
house3w1:scale(0.4,0.4,0.4)
house3w1:translate(4,3.5,-2)
house3w1:set_material(lightblue)
house3:add_child(house3w1)

house3w2 = gr.mesh('window', 'w1')
house3w2:scale(0.4,0.4,0.4)
house3w2:translate(4,3.5,2)
house3w2:set_material(lightblue)
house3:add_child(house3w2)

---- spongbob
house4Node = gr.node('house4')
innerSphereNode:add_child(house4Node)

house4 = gr.mesh('bobhouse', 'house')
house4:scale(15, 15, 15)
house4:translate(0, 71, 0)
house4:rotate('y', -40)
house4:rotate('z', -30)
house4:rotate('x', 20)
house4:set_material(orange)
house4Node:add_child(house4)

house4leave = gr.mesh('bobleave', 'house')
house4leave:scale(0.8/15, 0.8/15, 0.8/15)
house4leave:translate(0, 10, 0)
house4leave:set_material(grass)
house4:add_child(house4leave)

door4 = gr.mesh('door', 'd3')
door4:scale(0.1,0.07,0.07) -- z x y
door4:rotate('y', 90)
door4:rotate('x', 10)
door4:translate(0, -4, 8.8)
door4:set_material(lightblue)
house4:add_child(door4)

house4w1 = gr.mesh('window', 'w1')
house4w1:scale(0.1,0.1,0.1)
house4w1:rotate('y', -100)
house4w1:translate(-3,3.5,8.5)
house4w1:set_material(lightblue)
house4:add_child(house4w1)

house4w2 = gr.mesh('window', 'w1')
house4w2:scale(0.1,0.1,0.1)
house4w2:rotate('y', -70)
house4w2:translate(4,1,8.5)
house4w2:set_material(lightblue)
house4:add_child(house4w2)

house4w3 = gr.mesh('window', 'w3')
house4w3:scale(0.05,0.05,0.05)
house4w3:rotate('y', -90)
house4w3:translate(0,-3,9.8)
house4w3:set_material(lightblue)
house4:add_child(house4w3)


---- spongbob2
house5Node = gr.node('house5')
innerSphereNode:add_child(house5Node)

house5 = gr.mesh('bobhouse', 'house')
house5:scale(10, 10, 10)
house5:translate(0, 67, 0)
house5:rotate('y', -60)
house5:rotate('z', -20)
house5:rotate('x', -140)
house5:set_material(orange)
house5Node:add_child(house5)

house5leave = gr.mesh('bobleave', 'house')
house5leave:scale(0.8/15, 0.8/15, 0.8/15)
house5leave:translate(0, 7, 0)
house5leave:set_material(grass)
house5:add_child(house5leave)

door5 = gr.mesh('door', 'd3')
door5:scale(0.1,0.07,0.07) -- z x y
door5:rotate('y', 90)
door5:rotate('x', 10)
door5:translate(0, -3, 6)
door5:set_material(lightblue)
house5:add_child(door5)

house5w1 = gr.mesh('window', 'w1')
house5w1:scale(0.1,0.1,0.1)
house5w1:rotate('y', -100)
house5w1:translate(-3,3.5,4.7)
house5w1:set_material(lightblue)
house5:add_child(house5w1)

house5w2 = gr.mesh('window', 'w1')
house5w2:scale(0.1,0.1,0.1)
house5w2:rotate('y', -70)
house5w2:translate(4,1,5)
house5w2:set_material(lightblue)
house5:add_child(house5w2)

house5w3 = gr.mesh('window', 'w3')
house5w3:scale(0.05,0.05,0.05)
house5w3:rotate('y', -90)
house5w3:translate(0,-3,6.8)
house5w3:set_material(lightblue)
house5:add_child(house5w3)


--------------------------------------------------
------------------- Plants---------------------
--------------------------------------------------


-------------- Plants &------------------------

plant1 = gr.mesh('plant1', 'plant1')
outerSphereNode:add_child(plant1)
plant1:rotate('z', -30)
plant1:rotate('x', 40)
plant1:translate(2/3*rootScale,4.5/3*rootScale,4.5/3*rootScale)  --outer: 44.5
plant1:scale(0.1*scale, 0.08*scale, 0.1*scale)
plant1:set_lnode(15, 1, 0, 25, 1)
plant1:set_material(darkyellow)

plant1_1 = gr.mesh('plant1', 'plant1')
outerSphereNode:add_child(plant1_1)
plant1_1:rotate('z', -30)
plant1_1:rotate('x', 40)
plant1_1:translate(2/3*rootScale+2,4.5/3*rootScale-3,4.5/3*rootScale+2)  --outer: 44.5
plant1_1:scale(0.08*scale, 0.05*scale, 0.08*scale)
plant1_1:set_lnode(20, 1, 0, 30, 1)
plant1_1:set_material(darkpurple)

plant2 = gr.mesh('plant1', 'plant2')
outerSphereNode:add_child(plant2)
plant2:rotate('z', 15)
plant2:rotate('x', 40)
plant2:translate(-2/3*rootScale,5.5/3*rootScale,2/3*rootScale) -- inner: 38.25
plant2:scale(0.15*scale, 0.12*scale, 0.15*scale)
plant2:set_lnode(12, 1, 0, 30, 1)
plant2:set_material(darkorange)

plant3 = gr.mesh('plant1', 'plant3')
outerSphereNode:add_child(plant3)
plant3:rotate('z', -35)
plant3:translate(3/3*rootScale,5.9/3*rootScale,0)
plant3:scale(0.1*scale, 0.07*scale, 0.1*scale)
plant3:set_lnode(15, 1, 0, 20, 1)
plant3:set_material(darkpurple)

plant4 = gr.mesh('plant1', 'plant4')
outerSphereNode:add_child(plant4)
plant4:translate(0/3*rootScale,6/3*rootScale,-1.5/3*rootScale)
plant4:scale(0.1*scale, 0.05*scale, 0.1*scale)
plant4:set_lnode(10, 1, 0, 15, 1)
plant4:set_material(darkred)

plant5 = gr.mesh('plant1', 'plant5')
outerSphereNode:add_child(plant5)
plant5:rotate('z', 15)
plant5:rotate('x',-30)
plant5:translate(-2/3*rootScale,5/3*rootScale,-3/3*rootScale)
plant5:scale(0.1*scale, 0.08*scale, 0.1*scale)
plant5:set_lnode(20, 1, 0, 15, 1)
plant5:set_material(darkred)

plant6 = gr.mesh('plant1', 'plant6')
outerSphereNode:add_child(plant6)
plant6:rotate('z', -20)
plant6:rotate('x',-90)
plant6:translate(2.2/3*rootScale,0.5/3*rootScale,-6.1/3*rootScale)
plant6:scale(0.06*scale, 0.06*scale, 0.06*scale)
plant6:set_lnode(15, 1, 0, 20, 1)
plant6:set_material(darkyellow)


plant7 = gr.mesh('plant1', 'plant7')
outerSphereNode:add_child(plant7)
plant7:rotate('z', -20)
plant7:rotate('x',-90)
plant7:translate(2/3*rootScale,0.5/3*rootScale,-6.2/3*rootScale)
plant7:scale(0.1*scale, 0.1*scale, 0.1*scale)
plant7:set_lnode(10, 1, 0, 15, 1)
plant7:set_material(orange)

plant8 = gr.mesh('plant1', 'plant8')
outerSphereNode:add_child(plant8)
plant8:rotate('z', -20)
plant8:rotate('x',-95)
plant8:translate(2.6/3*rootScale,0.1/3*rootScale,-6/3*rootScale)
plant8:scale(0.12*scale, 0.11*scale, 0.12*scale)
plant8:set_lnode(20, 1, 0, 30, 1)
plant8:set_material(darkpurple)


plant9 = gr.mesh('plant1', 'plant9')
outerSphereNode:add_child(plant9)
plant9:rotate('z', 30)
plant9:rotate('x',-150)
plant9:translate(-3.5/3*rootScale,-4.5/3*rootScale,-2.9/3*rootScale)
plant9:scale(0.15*scale, 0.13*scale, 0.15*scale)
plant9:set_lnode(20, 1, 0, 20, 1)
plant9:set_material(darkorange)


plant10 = gr.mesh('plant1', 'plant9')
outerSphereNode:add_child(plant10)
-- plant10:rotate('z', 30)
plant10:rotate('x',-180)
plant10:translate(-0.3/3*rootScale,-5.8/3*rootScale,2.5/3*rootScale)
plant10:scale(0.08*scale, 0.04*scale, 0.08*scale)
plant10:set_lnode(15, 1, 0, 20, 1)
plant10:set_material(darkpurple)

plant11 = gr.mesh('plant1', 'plant11')
outerSphereNode:add_child(plant11)
plant11:rotate('z', 40)
plant11:rotate('x',120)
plant11:translate(-4/3*rootScale, -2/3*rootScale, 4.5/3*rootScale)
plant11:scale(0.2*scale, 0.15*scale, 0.2*scale)
plant11:set_lnode(10, 1, 0, 20, 1)
plant11:set_material(darkred)

plant12 = gr.mesh('plant1', 'plant12')
outerSphereNode:add_child(plant12)
plant12:rotate('z', 40)
plant12:rotate('x',120)
plant12:translate(-4/3*rootScale, -2/3*rootScale, 4.5/3*rootScale)
plant12:scale(0.07*scale, 0.05*scale, 0.07*scale)
plant12:set_lnode(30, 1, 0, 20, 1)
plant12:set_material(orange)

plant13 = gr.mesh('plant1', 'plant13')
outerSphereNode:add_child(plant13)
plant13:rotate('z', 40)
plant13:rotate('x',120)
plant13:translate(-4/3*rootScale, -1.7/3*rootScale, 4.5/3*rootScale)
plant13:scale(0.1*scale, 0.1*scale, 0.1*scale)
plant13:set_lnode(25, 1, 0, 20, 1)
plant13:set_material(darkyellow)

-- pair 14 15
plant14 = gr.mesh('plant1', 'grass2')
outerSphereNode:add_child(plant14)
plant14:translate(0,4.2/3*rootScale*1.5-2,0)  --outer: 44.5
plant14:scale(0.1*scale, 0.07*scale, 0.1*scale)
plant14:rotate('y', -45)
--plant14:rotate('z', -30)
plant14:rotate('x', 85)
plant14:set_lnode(30, 1, 0, 30, 1)
plant14:set_material(orange)

plant15 = gr.mesh('plant1', 'grass2')
outerSphereNode:add_child(plant15)
plant15:translate(-4,4.2/3*rootScale*1.5-2,3)  --outer: 44.5
plant15:scale(0.15*scale, 0.06*scale, 0.15*scale)
plant15:rotate('y', -45)
--plant15:rotate('z', -30)
plant15:rotate('x', 80)
plant15:set_lnode(40, 1, 0, 35, 1)
plant15:set_material(darkyellow)

-- pair 16 17 18
plant16 = gr.mesh('plant1', 'grass2')
outerSphereNode:add_child(plant16)
plant16:translate(0,4.2/3*rootScale*1.5,0)  --outer: 44.5
plant16:scale(0.1*scale, 0.12*scale, 0.1*scale)
plant16:rotate('y', -90)
plant16:rotate('z', -95)
plant16:rotate('y', -30)
plant16:set_lnode(20, 1, 0, 30, 1)
plant16:set_material(orange)

plant17 = gr.mesh('plant1', 'grass2')
outerSphereNode:add_child(plant17)
plant17:translate(-4,4.2/3*rootScale*1.5,3)  --outer: 44.5
plant17:scale(0.15*scale, 0.13*scale, 0.15*scale)
plant17:rotate('y', -90)
plant17:rotate('z', -95)
plant17:rotate('y', -30)
plant17:set_lnode(30, 1, 0, 20, 1)
plant17:set_material(darkyellow)

plant18 = gr.mesh('plant1', 'grass2')
outerSphereNode:add_child(plant18)
plant18:translate(-1,4.2/3*rootScale*1.5,7)  --outer: 44.5
plant18:scale(0.15*scale, 0.14*scale, 0.15*scale)
plant18:rotate('y', -90)
plant18:rotate('z', -95)
plant18:rotate('y', -30)
plant18:set_lnode(25, 1, 0, 20, 1)
plant18:set_material(red)

-------------------- Grass & -------------------

grass1 = gr.mesh('plant1', 'grass1')
outerSphereNode:add_child(grass1)
grass1:rotate('z', 30)
grass1:rotate('x', 40)
grass1:translate(-2/3*rootScale-4,4.2/3*rootScale-1,4.5/3*rootScale)  --outer: 44.5
grass1:scale(0.15*scale, 0.25*scale, 0.15*scale)
grass1:set_lnode(3, 3, 1, 80, 0.25*scale)
grass1:set_material(orange)

grass2 = gr.mesh('plant1', 'grass2')
outerSphereNode:add_child(grass2)
grass2:rotate('z', -30)
grass2:rotate('x', 100)
grass2:translate(3/3*rootScale,-4.2/3*rootScale,3.7/3*rootScale)  --outer: 44.5
grass2:scale(0.15*scale, 0.3*scale, 0.15*scale)
grass2:set_lnode(3, 3, 1, 70, 0.3*scale)
grass2:set_material(orange)


grass3 = gr.mesh('plant1', 'grass2')
outerSphereNode:add_child(grass3)
grass3:translate(0,4.2/3*rootScale*1.5,0)  --outer: 44.5
grass3:scale(0.15*scale, 0.3*scale, 0.15*scale)
grass3:rotate('z', -210)
grass3:set_lnode(4, 4, 1, 40, 0.3*scale)
grass3:set_material(red)

grass4 = gr.mesh('plant1', 'grass2')
outerSphereNode:add_child(grass4)
grass4:translate(-2,4.2/3*rootScale*1.5,3)  --outer: 44.5
grass4:scale(0.15*scale, 0.3*scale, 0.15*scale)
grass4:rotate('z', -210)
grass4:set_lnode(3, 3, 1, 70, 0.3*scale)
grass4:set_material(darkyellow)

--------------------------------------------------
------------------- Character---------------------
--------------------------------------------------

xtrans_ship = -0.35*rootScale
ytrans_ship = 1.3*rootScale
ztrans_ship = 1.7*rootScale

------------------------- Ship 1 &-----------------
shipNode1 = gr.node('shipNode1')
--shipNode1:translate(0,0,-2)
rootNode:add_child(shipNode1)

ship1 = gr.mesh('ship', 'ship1')
ship1:scale(0.18*rootScale, 0.22*rootScale, 0.18*rootScale)
ship1:rotate('y', -90)
ship1:rotate('x', 50)
ship1:translate(xtrans_ship, ytrans_ship, ztrans_ship)
ship1:set_material(brownyellow)
shipNode1:add_child(ship1)


shipStripeNode1 = gr.node('shipStripeNode1')
shipNode1:add_child(shipStripeNode1)

ship1Stripe1 = gr.mesh('shipMidStripe', 'ship1MidStripe1')
ship1Stripe1:scale(0.19*rootScale, 0.19*rootScale, 0.22*rootScale)
ship1Stripe1:rotate('y', -90)
ship1Stripe1:rotate('x', 50)
ship1Stripe1:translate(xtrans_ship, ytrans_ship, ztrans_ship)
ship1Stripe1:set_material(blue)
shipStripeNode1:add_child(ship1Stripe1)


ztrans_ship2 = 1.81*rootScale
-- ------------------------- Ship 2 &-----------------
shipNode2 = gr.node('shipNode2')
shipNode2:rotate('x', 50)
shipNode2:translate(-xtrans_ship, ytrans_ship-0.8, ztrans_ship2+0.18/0.22)
rootNode:add_child(shipNode2)
ship2Joint = gr.joint('ship2Joint',{0, 0, 0}, {-180, 0, 180}, {0, 0, 0})
--ship2Joint:rotate('y',-90)
shipNode2:add_child(ship2Joint)


ship2 = gr.mesh('ship', 'ship2')
ship2:scale(0.18*rootScale, 0.22*rootScale, 0.18*rootScale)
ship2:rotate('y', -90)
ship2:translate(0,-2.5,-0.11*rootScale)
ship2:set_material(brownyellow)
shipNode2:add_child(ship2)

ship2weapon = gr.mesh('weapon', 'weapon')
ship2weapon:scale(0.05*rootScale, 0.05*rootScale, 0.05*rootScale)
ship2weapon:rotate('y', -90)
-- ship2weapon:rotate('x', 50)
ship2weapon:translate(0, 0.066*rootScale-2.5, 0)
ship2weapon:set_material(darkpink)
ship2Joint:add_child(ship2weapon)

ship2weapon2 = gr.mesh('sphere', 'weapon2')
ship2weapon2:scale(1,1,1)
ship2weapon2:translate(0, 0.066*rootScale-2, 0)
ship2weapon2:set_material(darkpink)
ship2Joint:add_child(ship2weapon2)



shipStripeNode2 = gr.node('shipStripeNode2')
shipNode2:add_child(shipStripeNode2)

ship2Stripe1 = gr.mesh('shipMidStripe', 'ship1MidStripe2')
ship2Stripe1:scale(0.19*rootScale, 0.19*rootScale, 0.22*rootScale)
ship2Stripe1:rotate('y', -90)
ship2Stripe1:translate(0, -2.5, -0.11*rootScale)
ship2Stripe1:set_material(red)
shipStripeNode2:add_child(ship2Stripe1)

------------------------------------------------
----------------- Squidward & ---------------------
-------------------------------------------------

ztrans = 1.75*rootScale
ytrans = 1.34*rootScale
xtrans = -0.35*rootScale
S = 0.08*rootScale

Upscale = 1.2
Forescale = 5

---------------- Torso ------------------
TorsoNode = gr.node('torsenode')
TorsoNode:scale(S,S,S)
TorsoNode:rotate('y', 180)
TorsoNode:rotate('x', 50)
TorsoNode:translate(xtrans, ytrans, ztrans)
shipNode1:add_child(TorsoNode)

Torse = gr.mesh('cylinder', 'torse')
Torse:scale(0.2, 0.3, 0.2)
Torse:translate(0, S*(-1.2+Upscale), 0)
Torse:set_material(orange)
TorsoNode:add_child(Torse)

--------------- Shoulder ----------------
ShoulderNode = gr.node('shouldernode')
ShoulderNode:translate(0.0, S*0.2, 0)
TorsoNode:add_child(ShoulderNode)
ShoulderJoint = gr.joint('shoulderJoint',{0, 0, 0}, {-60, 0, 60}, {0, 0, 0})
ShoulderNode:add_child(ShoulderJoint)

UpShoulder = gr.mesh('sphere', 'up_shoulder')
UpShoulder:scale(0.2, 0.15, 0.2)
UpShoulder:translate(0.0, S*0.3, 0)
UpShoulder:set_material(orange)
ShoulderJoint:add_child(UpShoulder)

BottomShoulder = gr.mesh('cylinder', 'bottom_shoulder')
BottomShoulder:scale(0.2, 0.1, 0.2)
BottomShoulder:translate(0.0, S*0.18, 0)
BottomShoulder:set_material(orange)
ShoulderJoint:add_child(BottomShoulder)

--------------- Neck -----------------
NeckNode = gr.node('necknode')
NeckNode:translate(0.0, S*0.4, 0)
ShoulderJoint:add_child(NeckNode)

NeckJoint = gr.joint('neckJoint',{0, 0, 0}, {-20, 0, 20}, {-20, 0, 20})
NeckNode:add_child(NeckJoint)

Neck = gr.mesh('cylinder', 'neck')
Neck:scale(0.06, 0.4, 0.06)
Neck:translate(0.0, S*0.2, 0)
Neck:set_material(bluegreen)
NeckJoint:add_child(Neck)

-- -- --------------- Head -----------------

HeadNode = gr.node('headNode')
HeadNode:translate(0.0, S*0.6, 0)
NeckJoint:add_child(HeadNode)

HeadJoint = gr.joint('headJoint', {0, 0, 0}, {-30, 0, 30}, {-20, 0, 20})
HeadNode:add_child(HeadJoint)

UpHead = gr.mesh('sphere', 'up_head')
UpHead:scale(0.72, 0.51, 0.6)
UpHead:translate(0.0, S*0.8, S*-0.1)
--UpHead:translate(0.0, 0.5, -0)
UpHead:set_material(bluegreen)
HeadJoint:add_child(UpHead)

LeftEye = gr.mesh('sphere', 'left_eye')
LeftEye:scale(0.15, 0.23, 0.15)
LeftEye:translate(S*-0.16, S*0.5, S*0.33)
LeftEye:set_material(gr.material({0.94, 0.9, 0.55}, {0.8, 0.8, 0.8}, 10.0))
HeadJoint:add_child(LeftEye)

RightEye = gr.mesh('sphere', 'right_eye')
RightEye:scale(0.15, 0.23, 0.15)
RightEye:translate(S*0.16, S*0.5, S*0.33)
RightEye:set_material(gr.material({0.94, 0.9, 0.55}, {0.8, 0.8, 0.8}, 10.0))
HeadJoint:add_child(RightEye)

LeftEyeCenter = gr.mesh('sphere', 'left_eyecenter')
LeftEyeCenter:scale(0.02, 0.02, 0.04)
LeftEyeCenter:translate(S*-0.16, S*0.4, S*0.45)
LeftEyeCenter:set_material(black)
HeadJoint:add_child(LeftEyeCenter)

RightEyeCenter = gr.mesh('sphere', 'right_eyecenter')
RightEyeCenter:scale(0.02, 0.02, 0.04)
RightEyeCenter:translate(S*0.16, S*0.4, S*0.45)
RightEyeCenter:set_material(black)
HeadJoint:add_child(RightEyeCenter)


LeftEyeSlash = gr.mesh('eyeslash', 'left_eyeslash')
LeftEyeSlash:scale(0.18, 0.4, 0.25)
LeftEyeSlash:translate(S*-0.168, S*0.48, S*0.27)
LeftEyeSlash:set_material(bluegreen)
HeadJoint:add_child(LeftEyeSlash)

RightEyeSlash = gr.mesh('eyeslash', 'right_eyeslash')
RightEyeSlash:scale(0.18, 0.4, 0.25)
RightEyeSlash:translate(S*0.168, S*0.48, S*0.27)
RightEyeSlash:set_material(bluegreen)
HeadJoint:add_child(RightEyeSlash)


MidHead = gr.mesh('cylinder', 'mid_head')
MidHead:scale(0.34, 0.45, 0.35)
MidHead:translate(S*0.0, S*0.45, 0)
MidHead:set_material(bluegreen)
HeadJoint:add_child(MidHead)

Mouth = gr.mesh('mouth', 'mouth')
Mouth:scale(0.085, 0.09, 0.07)
Mouth:translate(0.0, S*-0.05, S*0.33)
Mouth:set_material(bluegreen)
HeadJoint:add_child(Mouth)

MouthLine = gr.mesh('mouth', 'mouthline')
MouthLine:scale(0.07, 0.01, 0.03)
MouthLine:translate(0.0, S*-0.04, S*0.4)
MouthLine:set_material(black)
HeadJoint:add_child(MouthLine)

Nose = gr.mesh('nose', 'nose')
Nose:scale(0.11, 0.11, 0.12)
Nose:rotate('x', -8.0)
Nose:rotate('z', -3.0)
Nose:translate(S*0.01, S*0.1, S*0.7)
Nose:set_material(bluegreen)
HeadJoint:add_child(Nose)


-- --------------- Left Upper Arm --------------
leftUpperArmNode = gr.node('leftupperarmnode')
leftUpperArmNode:translate(S*-0.2, S*0.25, 0)
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
MidleftUpperArm:translate(S*-0.06, S*-0.1, 0)
MidleftUpperArm:set_material(orange)
leftUpperArmJoint:add_child(MidleftUpperArm)

BottomleftUpperArm = gr.mesh('cylinder', 'bottom_leftUpperArm')
BottomleftUpperArm:scale(0.06, 0.16, 0.06)
BottomleftUpperArm:rotate('z', -30)
BottomleftUpperArm:translate(S*-0.22, S*-0.37, 0)
BottomleftUpperArm:set_material(bluegreen)
leftUpperArmJoint:add_child(BottomleftUpperArm)

--------------- Left ForeArm ------------------------
leftForeArmNode = gr.node('leftforearmnode')
leftForeArmNode:rotate('x', -90)
leftForeArmNode:rotate('y', 45)
leftForeArmNode:translate(S*-0.31, S*-0.53, 0)
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
MidleftForeArm:translate(0, S*-0.25, 0)
MidleftForeArm:set_material(bluegreen)
leftForeArmJoint:add_child(MidleftForeArm)

-- -------------- Left hand 1 --------------------
HandNode1 = gr.node('hand')
HandNode1:translate(-0, S*-0.5, 0)
leftForeArmJoint:add_child(HandNode1)
HandJoint1 = gr.joint('handjoint1',{-30, 0, 30}, {0, 0, 0}, {-90, 0, 30})
HandNode1:add_child(HandJoint1)

hand1 = gr.mesh('foot', 'hand1')
hand1:scale(0.15, 0.15, 0.15)
hand1:rotate('x', 90)
hand1:rotate('y', -90)
hand1:translate(S*-0.05, S*-0.2, 0)
hand1:set_material(bluegreen)
HandJoint1:add_child(hand1)



--------------- Right Upper Arm --------------
rightUpperArmNode = gr.node('rightupperarmnode')
rightUpperArmNode:translate(S*0.2, S*0.25, 0)
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
MidrightUpperArm:translate(S*0.06, S*-0.1, 0)
MidrightUpperArm:set_material(orange)
rightUpperArmJoint:add_child(MidrightUpperArm)

BottomrightUpperArm = gr.mesh('cylinder', 'bottom_rightUpperArm')
BottomrightUpperArm:scale(0.06, 0.16, 0.06)
BottomrightUpperArm:rotate('z', 30)
BottomrightUpperArm:translate(S*0.22, S*-0.37, 0)
BottomrightUpperArm:set_material(bluegreen)
rightUpperArmJoint:add_child(BottomrightUpperArm)

-- --------------- right ForeArm ------------------------
rightForeArmNode = gr.node('rightforearmnode')
rightForeArmNode:rotate('x', -90)
rightForeArmNode:rotate('y', -45)
rightForeArmNode:translate(S*0.31, S*-0.53, 0)
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
MidrightForeArm:translate(0, S*-0.25, 0)
MidrightForeArm:set_material(bluegreen)
rightForeArmJoint:add_child(MidrightForeArm)

-------------- Right hand 2 --------------------
HandNode2 = gr.node('hand2')
HandNode2:translate(0, S*-0.5, 0)
rightForeArmJoint:add_child(HandNode2)
HandJoint2 = gr.joint('handjoint2',{-30, 0, 30}, {0, 0, 0}, {-30, 0, 90})
HandNode2:add_child(HandJoint2)

hand2 = gr.mesh('foot', 'hand2')
hand2:scale(0.15, 0.15, 0.15)
hand2:rotate('x', 90)
hand2:rotate('y', 90)
hand2:translate(S*0.05, S*-0.2, 0)
hand2:set_material(bluegreen)
HandJoint2:add_child(hand2)


----------------------- Patrick Star & ---------------

zval = 1
yval = 2.35
xval = 0.04
Sstar = 0.27
S_star = 0.2*rootScale
---------------- Torso ------------------
StarTorsoNode = gr.node('startorsenode')
StarTorsoNode:rotate('y',180)
StarTorsoNode:scale(S_star,S_star,S_star)
StarTorsoNode:translate(0.35*rootScale+xtrans_ship,0.96*rootScale-ytrans_ship-2.5,1.56*rootScale-ztrans_ship2+0.27*rootScale)
shipNode2:add_child(StarTorsoNode)

StarTorse = gr.mesh('starbody', 'startorse')
StarTorse:scale(0.2, 0.2, 0.2)
StarTorse:translate(0, S_star*(yval-0.35),  S_star*zval)
StarTorse:set_material(pink)
StarTorsoNode:add_child(StarTorse)

StarLeftEye = gr.mesh('sphere', 'Starleft_eye')
StarLeftEye:scale(Sstar*0.15, Sstar*0.23, Sstar*0.15)
StarLeftEye:translate( S_star*-xval,  S_star*(yval+0.04),  S_star*(zval-0.17))
StarLeftEye:set_material(gr.material({0.94, 0.9, 0.9}, {0.8, 0.8, 0.8}, 10.0))
StarTorsoNode:add_child(StarLeftEye)

StarRightEye = gr.mesh('sphere', 'Starright_eye')
StarRightEye:scale(Sstar*0.15, Sstar*0.23, Sstar*0.15)
StarRightEye:translate( S_star*xval,  S_star*(yval+0.04),  S_star*(zval-0.17))
StarRightEye:set_material(gr.material({0.94, 0.9, 0.9}, {0.8, 0.8, 0.8}, 10.0))
StarTorsoNode:add_child(StarRightEye)

StarLeftEyeCenter = gr.mesh('sphere', 'Starleft_eyecenter')
StarLeftEyeCenter:scale(Sstar*0.05, Sstar*0.05, Sstar*0.12)
StarLeftEyeCenter:translate( S_star*-xval,  S_star*(yval+0.04),  S_star*(zval-0.15))
StarLeftEyeCenter:set_material(black)
StarTorsoNode:add_child(StarLeftEyeCenter)

StarRightEyeCenter = gr.mesh('sphere', 'Starright_eyecenter')
StarRightEyeCenter:scale(Sstar*0.05, Sstar*0.05, Sstar*0.12)
StarRightEyeCenter:translate( S_star*xval,  S_star*(yval+0.04),  S_star*(zval-0.15))
StarRightEyeCenter:set_material(black)
StarTorsoNode:add_child(StarRightEyeCenter)

Starmouse = gr.mesh('starmouse', 'starmouse')
Starmouse:rotate('x',92)
Starmouse:scale(Sstar*0.3, Sstar*0.2, Sstar*0.3)
Starmouse:translate(0,  S_star*(yval-0.08),  S_star*(zval-0.13))
Starmouse:set_material(red)
StarTorsoNode:add_child(Starmouse)


Starpant = gr.mesh('starpant', 'starpant')
Starpant:scale(Sstar*0.95, Sstar*0.85, Sstar*0.95)
Starpant:translate(0,  S_star*(yval-0.35),  S_star*(zval-0.32))
Starpant:set_material(lightgreen)
StarTorsoNode:add_child(Starpant)

------------------------left arm---------------------

StarLeftArmNode = gr.node('starleftarmNode')
StarLeftArmNode:translate( S_star*(-0.16),  S_star*(yval-0.04-0.12),  S_star*(zval-0.2))
StarTorsoNode:add_child(StarLeftArmNode)

StarleftArmJoint = gr.joint('StarleftArmJoint',{-45, 0, 45}, {0, 0, 0}, {-90, 0, 90})
StarLeftArmNode:add_child(StarleftArmJoint)

StarLeftArm = gr.mesh('sphere', 'starleftarm')
StarLeftArm:scale(0.05, 0.2, 0.05)
StarLeftArm:translate(0,  S_star*0.12, 0)
StarLeftArm:rotate('x', 90)
StarLeftArm:set_material(pink)
StarleftArmJoint:add_child(StarLeftArm)


------------------------right arm---------------------

StarRightArmNode = gr.node('rightarmNode')
StarRightArmNode:translate( S_star*0.16,  S_star*(yval-0.04-0.12),  S_star*(zval-0.2))
StarTorsoNode:add_child(StarRightArmNode)

StarRightArmJoint = gr.joint('StarrightArmJoint',{-45, 0, 45}, {0, 0, 0}, {-90, 0, 90})
StarRightArmNode:add_child(StarRightArmJoint)

StarRightArm = gr.mesh('sphere', 'starrightarm')
StarRightArm:scale(0.05, 0.2, 0.05)
StarRightArm:translate(0,  S_star*0.12, 0)
StarRightArm:rotate('x', 90)
StarRightArm:set_material(pink)
StarRightArmJoint:add_child(StarRightArm)


return rootNode