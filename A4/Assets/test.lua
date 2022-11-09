-- A simple scene with some miscellaneous geometry.

mat1 = gr.material({0.7, 1.0, 0.7}, {0.5, 0.7, 0.5}, 25)
mat2 = gr.material({0.5, 0.5, 0.5}, {0.5, 0.7, 0.5}, 25)
mat3 = gr.material({1.0, 0.6, 0.1}, {0.5, 0.7, 0.5}, 25)
mat4 = gr.material({0.7, 0.6, 1.0}, {0.5, 0.4, 0.8}, 25)

scene_root = gr.node('root')




-- s1 = gr.nh_sphere('s1', {0, 0, -400}, 100)
-- scene_root:add_child(s1)
-- s1:set_material(mat1)

-- -- s = gr.sphere('s')
-- -- scene_root:add_child(s)
-- -- s:set_material(mat1)
-- -- s:scale(100, 100, 100)
-- -- s:translate(0, 0, -400)

-- s2 = gr.nh_sphere('s2', {200, 50, -100}, 150)
-- scene_root:add_child(s2)
-- s2:set_material(mat1)



-- A small stellated dodecahedron.

steldodec = gr.mesh( 'dodec', 'Assets/smstdodeca.obj' )
steldodec:set_material(mat3)
scene_root:add_child(steldodec)

-- b1 = gr.nh_box('b1', {-220, 132, -178}, 157)
-- scene_root:add_child(b1)
-- b1:set_material(mat4)

-- b1 = gr.nh_box('b1', {-100, -100, -200}, 100)
-- scene_root:add_child(b1)
-- b1:set_material(mat4)


white_light = gr.light({-100.0, 200.0, 400.0}, {0.9, 0.9, 0.9}, {1, 0, 0})
magenta_light = gr.light({400.0, 100.0, 150.0}, {0.7, 0.0, 0.7}, {1, 0, 0})

gr.render(scene_root, 'test.png', 1024, 1024,
	  {-200, 80, 2000}, {0, 0, -1}, {0, 1, 0}, 50,
	  {0.3, 0.3, 0.3}, {white_light, magenta_light})