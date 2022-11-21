-- A simple scene with some miscellaneous geometry.

mat1 = gr.material({0.1, 0.7, 0.2}, {0.5, 0.7, 0.5}, 25)
mat2 = gr.material({0.5, 0.5, 0.5}, {0.5, 0.7, 0.5}, 25)
mat3 = gr.material({1.0, 0.6, 0.1}, {0.5, 0.7, 0.5}, 25)
mat4 = gr.material({0.7, 0.6, 1.0}, {0.5, 0.4, 0.8}, 25)
mat5 = gr.material({0.7, 1.0, 0.7}, {0.5, 0.7, 0.5}, 25)
gold = gr.material({0.9, 0.8, 0.4}, {0.8, 0.8, 0.4}, 25)
grass = gr.material({0.1, 0.7, 0.1}, {0.0, 0.0, 0.0}, 0)
blue = gr.material({0.7, 0.6, 1}, {0.5, 0.4, 0.8}, 25)
stone = gr.material({0.8, 0.7, 0.7}, {0.5, 0.7, 0.5}, 25)
hide = gr.material({0.84, 0.6, 0.53}, {0.3, 0.3, 0.3}, 20)

scene_root = gr.node('root')
scene_root:set_mirror()




buckyball = gr.mesh( 'buckyball', 'Assets/buckyball.obj' )
scene_root:add_child(buckyball)
buckyball:set_material(stone)
buckyball:set_bump()
buckyball:scale(1, 1, 1)
buckyball:translate(2, -1, -10)

steldodec = gr.mesh( 'dodec', 'Assets/smstdodeca.obj' )
steldodec:set_material(mat3)
steldodec:set_bump()
steldodec:translate(140, -20, -500)
steldodec:scale(0.03, 0.03, 0.03)
scene_root:add_child(steldodec)



p2 = gr.cube('p2')
scene_root:add_child(p2)
p2:set_material(mat4)
p2:scale(1.5, 1, 1.5)
p2:translate(0, -2, -4)
p2:rotate('y', 45)

s4 = gr.nh_sphere('s4', {2, -2, -2}, 0.5)
scene_root:add_child(s4)
s4:set_material(mat5)

s3 = gr.nh_sphere('s3', {-10, 8, -20}, 7)
scene_root:add_child(s3)
s3:set_material(gold)


s = gr.sphere('s')
scene_root:add_child(s)
s:set_material(mat1)
s:scale(8, 2, 8)
s:translate(0, -4, -10)

cow_poly = gr.mesh('cow', 'Assets/cow.obj')
factor = 1.2/(2.76+3.637)
cow_poly:set_material(hide)
cow_poly:translate(0.0, 3.637, -6.0)
cow_poly:scale(factor, factor, factor)
cow_poly:translate(1.0, -2.8, -1)
cow_poly:rotate('y', 45)
scene_root:add_child(cow_poly)


white_light = gr.light({20.0, 20.0, 30.0}, {0.9, 0.9, 0.9}, {1, 0, 0})
magenta_light = gr.light({400.0, 100.0, 150.0}, {0.7, 0.0, 0.7}, {1, 0, 0})

gr.render(scene_root, 'sample.png', 512, 512,
	  {0, 0, 7}, {0, 0, -1}, {0, 1, 0}, 50,
	  {0.3, 0.3, 0.3}, {white_light,magenta_light})