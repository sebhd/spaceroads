# -*- coding: UTF-8 -*-

import sys, os

def makeXMLTag(element, attributes, close):
    result = "<" + element + " "
  
    for key in attributes.keys():
	value = attributes[key]
	result = result + key + "=\"" + str(value) + "\" "
    
    if close:
	result = result + "/"
    
    result = result + ">"
    return result

  
def makeTunnel(x,y,z, scalex, scaley, scalez):
      
    result = "<!-- ########## BEGIN Tunnel ########## -->\n"
    
    # Decorative mesh:
    result += makeXMLTag("Mesh", {'x':x, 'y':y+1, 'z':z, 'scalex':scalex, 'scaley':scaley, 'scalez':scalez, 'mesh':"Tunnel"}, True) + "\n"
   
    # Floor:
    result += makeXMLTag("Atom", {'x':x, 'y':y, 'z':z, 'scalex':scalex, 'scaley': 1, 'scalez':scalez, 'jumpForce':0}, True) + "\n"
    
    # Walls:
    result += makeXMLTag("Atom", {'x':x, 'y':y+1, 'z':z, 'scalex':1, 'scaley': scaley - 1, 'scalez':scalez, 'material':'none', 'jumpForce':0}, True) + "\n"
    result += makeXMLTag("Atom", {'x':x + scalex - 1, 'y':y+1, 'z':z, 'scalex':1, 'scaley': scaley - 1, 'scalez':scalez, 'material':'none', 'jumpForce':0}, True) + "\n"

    # Roof:
    result += makeXMLTag("Atom", {'x':x, 'y':y+scaley, 'z':z, 'scalex':scalex, 'scaley': 1, 'scalez':scalez, 'material':'none', 'slipOffset': scalex / 2 - 1.7}, True) + "\n"
    result += "<!-- ########## END Tunnel ########## -->\n"
    return result
    
  
####################### Here begins the program #######################


filename = "default_track.xml"

try:
  filename = sys.argv[1]
except:
  pass

outfile = open(filename, "w")

outfile.write("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n")
outfile.write("<Track skybox=\"SpaceRoads/SkyBoxes/PurpleNebula2\">\n")

outfile.write('<StartPos x="4" y="10" z="5" />\n')

mat = 0





ii = 0
count = 0

while(ii < 100):
    
    x = 0
    y = 0
    z = ii * 20
    scalex = 8
    scaley = 1
    scalez = 20

    
    if count == 10:
	outfile.write(makeTunnel(x,y,z, scalex, 5,100))	
	count = 0
	ii += 5
    
    else:
	if mat == 0:
	    material = "SpaceRoads/Track/DarkGrey"
	    mat = 1
	else:
	    material = "SpaceRoads/Track/White"
	    mat = 0
	
	outfile.write(makeXMLTag("Atom", {'x':x, 'y':y, 'z':z, 'scalex':scalex, 'scaley': 1, 'scalez':scalez, 'material': material}, True) + "\n")
	    
	ii += 1
    
    count += 1
		
outfile.write("</Track>")
