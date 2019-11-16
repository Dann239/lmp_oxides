from ovito.data import *

def modify(frame, data):
    #    frame - The current animation frame number at which the pipeline is being evaluated.
    #    data   - The DataCollection passed in from the pipeline system. 
    #                The function may modify the data stored in this DataCollection as needed.
    
    count = data.particles.count
    occupancy = data.particles['Occupancy']
    position = data.particles.position
    for i in range(count):
        print(occupancy[i][0],occupancy[i][1],position[i][0],position[i][1],position[i][2])
    
    if data.particles != None:
        print("There are %i particles with the following properties:" % data.particles.count)
        for property_name in data.particles.keys():
            print("  '%s'" % property_name)
