# -*- coding: UTF-8 -*-

import sys, os

filename = "default_track.xml"

try:
  filename = sys.argv[1]
except:
  pass

outfile = open(filename, "w")

outfile.write("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n")
outfile.write("<Track skybox=\"SpaceRoads/SkyBoxes/PurpleNebula2\">\n")

mat = 0

for ii in range(0,100):
  
  x = 0
  y = 0
  z = ii * 20
  sizex = 5
  sizey = 1
  sizez = 20
  
  if mat == 0:
    material = "SpaceRoads/Track/DarkGrey"
    mat = 1
  else:
    material = "SpaceRoads/Track/White"
    mat = 0
  
  outfile.write("<Atom x=\"" + str(x) 
		 + "\" y=\"" + str(y) 
		 + "\" z=\"" + str(z) 
		 + "\" sizex=\"" + str(sizex) 
		 + "\" sizey=\"" + str(sizey) 
		 + "\" sizez=\"" + str(sizez) 
		 + "\" name=\"\"  material=\"" + material + "\"></Atom>\n")

outfile.write("</Track>")
