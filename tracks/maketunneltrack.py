# -*- coding: UTF-8 -*-

import sys, os
import random

def makeXMLTag(element, attributes, close):
    result = "<" + element + " "
  
    for key in attributes.keys():
	value = attributes[key]
	result = result + key + "=\"" + str(value) + "\" "
    
    if close:
	result = result + "/"
    
    result = result + ">"
    return result

  
def makeCurvedRoofTunnel(x,y,z, scalex, scaley, scalez):
      
    result = "<!-- ########## BEGIN Tunnel ########## -->\n"
    
    # Decorative mesh:
    result += makeXMLTag("Mesh", {'x':x, 'y':y+1, 'z':z, 'scalex':scalex, 'scaley':scaley, 'scalez':scalez, 'mesh':"CurvedRoofTunnel", 'material':"SpaceRoads/Track/Red"}, True) + "\n"
   
    # Floor:
  #  result += makeXMLTag("Atom", {'x':x, 'y':y, 'z':z, 'scalex':scalex, 'scaley': 1, 'scalez':scalez, 'jaumpForce':0}, True) + "\n"
    
    # Walls:
    result += makeXMLTag("Atom", {'x':x, 'y':y+1, 'z':z, 'scalex':1, 'scaley': scaley - 1, 'scalez':scalez, 'material':'none', 'jaumpForce':0, 'type':"solid"}, True) + "\n"
    result += makeXMLTag("Atom", {'x':x + scalex - 1, 'y':y+1, 'z':z, 'scalex':1, 'scaley': scaley - 1, 'scalez':scalez, 'material':'none', 'jaumpForce':0, 'type':"solid"}, True) + "\n"

    # Roof:
    result += makeXMLTag("Atom", {'x':x, 'y':y+scaley, 'z':z, 'scalex':scalex, 'scaley': 1, 'scalez':scalez, 'material':'none', 'slipOffset': scalex / 2 - 1.7, 'type':"solid"}, True) + "\n"
    result += "<!-- ########## END Tunnel ########## -->\n"
    return result

    
def makeFlatRoofTunnel(x,y,z, scalex, scaley, scalez):
      
    result = "<!-- ########## BEGIN Tunnel ########## -->\n"
    
    # Decorative mesh:
    result += makeXMLTag("Mesh", {'x':x, 'y':y+1, 'z':z, 'scalex':scalex, 'scaley':scaley, 'scalez':scalez, 'mesh':"FlatRoofTunnel", 'material':"SpaceRoads/Track/Red"}, True) + "\n"
   
    # Floor:
  #  result += makeXMLTag("Atom", {'x':x, 'y':y, 'z':z, 'scalex':scalex, 'scaley': 1, 'scalez':scalez, 'jaumpForce':0}, True) + "\n"
    
    # Walls:
    result += makeXMLTag("Atom", {'x':x, 'y':y+1, 'z':z, 'scalex':1, 'scaley': scaley - 1, 'scalez':scalez, 'material':'none', 'jaumpForce':0, 'type':"solid"}, True) + "\n"
    result += makeXMLTag("Atom", {'x':x + scalex - 1, 'y':y+1, 'z':z, 'scalex':1, 'scaley': scaley - 1, 'scalez':scalez, 'material':'none', 'jaumpForce':0, 'type':"solid"}, True) + "\n"

    # Roof:
    result += makeXMLTag("Atom", {'x':x, 'y':y+scaley, 'z':z, 'scalex':scalex, 'scaley': 1, 'scalez':scalez, 'material':'none', 'type':"solid"}, True) + "\n"
    result += "<!-- ########## END Tunnel ########## -->\n"
    return result

  

def makeBlock(x,y,z, sx,sy,sz, mat, side):
	h = random.randint(0,100)	
	if h < 15:
		pass
		
	elif h == 86 or h == 87:
		outfile.write(makeXMLTag("Atom", {'x': x, 'y':y, 'z':z, 'scalex':sx, 'scaley': sy, 'scalez': sz, 'material': 'SpaceRoads/Track/Yellow', 'type':'rotator', 'rotatorAxis':2, 'rotatorSteps':1}, True) + "\n")

	elif h == 88 or h == 89:
		outfile.write(makeXMLTag("Atom", {'x': x, 'y':y, 'z':z, 'scalex':sx, 'scaley': sy, 'scalez': sz, 'material': 'SpaceRoads/Track/Red', 'type':'rotator', 'rotatorAxis':2, 'rotatorSteps':-1}, True) + "\n")
		
	elif h > 98:
		outfile.write(makeXMLTag("Atom", {'x': x, 'y':y, 'z':z, 'scalex': sx, 'scaley': sy, 'scalez': sz, 'material': 'SpaceRoads/Track/Blue', 'type':"refresher"}, True) + "\n")
	else:
		outfile.write(makeXMLTag("Atom", {'x': x, 'y':y, 'z':z, 'scalex':sx, 'scaley': sy, 'scalez': sz, 'material': mat, 'type':"solid"}, True) + "\n")
  
####################### Here begins the program #######################

random.seed()


filename = "default_track.xml"

try:
  filename = sys.argv[1]
except:
  pass

outfile = open(filename, "w")

outfile.write("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n")
outfile.write("<Track skybox=\"SpaceRoads/SkyBoxes/PurpleNebula2\">\n")

outfile.write('<StartPos x="4" y="10" z="50" />\n')
outfile.write('<AmbientLight r="0.4" g="0.4" b="0.4" />\n')
outfile.write('<DirectionalLight x="-1" y="-1" z="1">\n')

outfile.write('</DirectionalLight>\n')


mat = 0


ii = 0

scalez = 50
scalex = 30
scaley = 1


while(ii < 50):
    
	x = 0
	y = 0
	z = ii * scalez
	#
	#if (ii - 3) % 20 == 0:
#	    outfile.write(makeCurvedRoofTunnel(x,y,z, scalex, 10,80))	
	  #  outfile.write(makeXMLTag("Atom", {'x':x - scalex, 'y':y+1, 'z':z, 'scalex':scalex, 'scaley': 5, 'scalez': 80, 'material': "SpaceRoads/Track/Yellow"}, True) + "\n")
	    #outfile.write(makeXMLTag("Atom", {'x':x - scalex, 'y':y, 'z':z, 'scalex':scalex, 'scaley': 1, 'scalez': 60, 'material': material}, True) + "\n")
		
	if mat == 0:
	    material1 = "SpaceRoads/Track/DarkGrey"
	    material2 = "SpaceRoads/Track/White"
	    mat = 1
	else:	    
	    material1 = "SpaceRoads/Track/White"
	    material2 = "SpaceRoads/Track/DarkGrey"
	    mat = 0
	
	
	# Boden:
	makeBlock(x + scalex, y, z, scalex, 1, scalez, material2, 0)
	makeBlock(x,          y, z, scalex, 1, scalez, material1, 0)
	makeBlock(x - scalex, y, z, scalex, 1, scalez, material2, 0)
	

	th = scalex * 3
		
	# Decke:
	makeBlock(x + scalex, th, z, scalex, 1, scalez, material2, 2)
	makeBlock(x         , th, z, scalex, 1, scalez, material1, 2)
	makeBlock(x - scalex, th, z, scalex, 1, scalez, material2, 2)
	
	
	
	# Wand links:
	makeBlock(x + scalex*2, y, z, 1, scalex, scalez, material2, 1)
	makeBlock(x + scalex*2, y + scalex, z, 1, scalex, scalez, material1, 1)
	makeBlock(x + scalex*2, y + scalex * 2, z, 1, scalex, scalez, material2, 1)


	
	
	
	# Wand rechts:
	makeBlock(x - scalex, y, z, 1, scalex, scalez, material2, 3)
	makeBlock(x - scalex, y + scalex, z, 1, scalex, scalez, material1, 3)
	makeBlock(x - scalex, y + scalex * 2, z, 1, scalex, scalez, material2, 3)

	
	
	
	#outfile.write(makeXMLTag("Atom", {'x':x - scalex * 4, 'y':25, 'z':z, 'scalex':1, 'scaley': 10, 'scalez':scalez, 'material': material1, 'type':"solid"}, True) + "\n")
	#outfile.write(makeXMLTag("Atom", {'x':x + scalex * 5, 'y':25, 'z':z, 'scalex':1, 'scaley': 10, 'scalez':scalez, 'material': material1, 'type':"solid"}, True) + "\n")
		
	ii += 1

# Finish:
outfile.write(makeXMLTag("Atom", {'x': x - scalex, 'y':0, 'z': z, 'scalex': scalex * 3, 'scaley': scalex * 3, 'scalez':1, 'material': "none",  'type':"finish" }, True) + "\n")

outfile.write("</Track>")
