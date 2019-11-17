from ovito.io import *
from ovito.data import *
from ovito.modifiers import *
import numpy as np
import sys

print("Arg 1 - dump file, Arg 2 - reference structure file, arg 3 - output file with dump of vacancies")

file1=str(sys.argv[1])
file2=str(sys.argv[2])
file3=str(sys.argv[3])

center=float(sys.argv[4])
radius=float(sys.argv[5])
occup=int(sys.argv[6])

print("Hello, Ovito works")

node = import_file(file1, multiple_frames = True)


ws = WignerSeitzAnalysisModifier(
    eliminate_cell_deformation = True)
ws.reference.load(file2)
node.modifiers.append(ws)
node.compute()

exp_string = "Occupancy == "+str(occup)+" && (Position.X-"+str(center)+")^2+(Position.Y-"+str(center)+")^2+(Position.Z-"+str(center)+")^2 > "+str(radius)+"^2"
str_string = '"'+exp_string+'"'


node.modifiers.append(SelectExpressionModifier(expression=exp_string))

node.compute()

export_file(node, file3, "txt", 
    columns = ["Timestep", "SelectExpression.num_selected"], 
    multiple_frames = True)


