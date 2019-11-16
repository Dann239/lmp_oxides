from ovito.data import *
from random import *
def modify(frame, data):
    
    #    frame - The current animation frame number at which the pipeline is being evaluated.
    #    data   - The DataCollection passed in from the pipeline system. 
    #                The function may modify the data stored in this DataCollection as needed.
    
    selection = data.particles_.create_property('Selection')
    position = data.particles.position
    type = data.particles.particle_type
    count = data.particles.count

    for cnt in range(9):
        i = randint(0, count - 1)
        while selection[i] == 1 or type[i] != 1:
            i = randint(0, count - 1)
            yield
        selection[i] = 1

    for cnt in range(12):
        i = randint(0, count - 1)
        while selection[i] == 1 or type[i] != 2:
            i = randint(0, count - 1)
            yield
        selection[i] = 1
