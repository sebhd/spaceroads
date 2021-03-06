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

  
####################### Here begins the program #######################


filename = "default_track.xml"

try:
  filename = sys.argv[1]
except:
  pass

outfile = open(filename, "w")

outfile.write("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n")
outfile.write("<Track skybox=\"SpaceRoads/SkyBoxes/PurpleNebula2\">\n")

outfile.write('<StartPos x="4" y="10" z="50" />\n')
outfile.write('<AmbientLight r="0.5" g="0.5" b="0.5" />\n')
outfile.write('<DirectionalLight x="-1" y="-1" z="1">\n')

outfile.write('</DirectionalLight>\n')


mat = 0


ii = 0

scalez = 40
scalex = 8
scaley = 1


while(ii < 100):
    
	x = 0
	y = 0
	z = ii * scalez
	
	if (ii - 3) % 20 == 0:
	    outfile.write(makeCurvedRoofTunnel(x,y,z, scalex, 5.0,80))	
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
	
	
	if (ii+2)  % 10 == 0:
		
		#outfile.write(makeXMLTag("Atom", {'x':x - scalex, 'y':y, 'z':z, 'scalex':scalex, 'scaley': 1, 'scalez': scalez, 'material': material2, 'type':"solid"}, True) + "\n")
		outfile.write(makeXMLTag("Atom", {'x':x, 'y':y, 'z':z, 'scalex':scalex, 'scaley': 1, 'scalez':scalez, 'material': material1, 'type':"solid"}, True) + "\n")
	elif (ii + 1) % 10 == 0 or (ii + 2) % 10 == 0:
		pass
	else:
	    outfile.write(makeXMLTag("Atom", {'x':x + scalex, 'y':y, 'z':z, 'scalex':scalex, 'scaley': 1, 'scalez': scalez, 'material': material2, 'type':"solid"}, True) + "\n")
	    outfile.write(makeXMLTag("Atom", {'x':x - scalex, 'y':y, 'z':z, 'scalex':scalex, 'scaley': 1, 'scalez': scalez, 'material': material2, 'type':"solid"}, True) + "\n")
	    outfile.write(makeXMLTag("Atom", {'x':x, 'y':y, 'z':z, 'scalex':scalex, 'scaley': 1, 'scalez':scalez, 'material': material1, 'type':"solid"}, True) + "\n")
		
	    outfile.write(makeXMLTag("Atom", {'x':x + scalex, 'y':60, 'z':z, 'scalex':scalex, 'scaley': 1, 'scalez': scalez, 'material': material2, 'type':"solid"}, True) + "\n")
	    outfile.write(makeXMLTag("Atom", {'x':x - scalex, 'y':60, 'z':z, 'scalex':scalex, 'scaley': 1, 'scalez': scalez, 'material': material2, 'type':"solid"}, True) + "\n")
	    outfile.write(makeXMLTag("Atom", {'x':x,          'y':60, 'z':z, 'scalex':scalex, 'scaley': 1, 'scalez':scalez, 'material': material1, 'type':"solid"}, True) + "\n")
	
	
	outfile.write(makeXMLTag("Atom", {'x':x - scalex * 4, 'y':25, 'z':z, 'scalex':1, 'scaley': 10, 'scalez':scalez, 'material': material1, 'type':"solid"}, True) + "\n")
	outfile.write(makeXMLTag("Atom", {'x':x + scalex * 5, 'y':25, 'z':z, 'scalex':1, 'scaley': 10, 'scalez':scalez, 'material': material1, 'type':"solid"}, True) + "\n")
		
	ii += 1

outfile.write(makeXMLTag("Atom", {'x': 0, 'y':0, 'z': (ii-1) * 20, 'scalex': 8, 'scaley': 10, 'scalez':10, 'material': "none", 'finish':"True", 'type':"finish" }, True) + "\n")

outfile.write("</Track>")
