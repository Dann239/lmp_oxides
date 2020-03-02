from ovito.io import *
from ovito.data import *
from ovito.modifiers import *
from ovito.pipeline import *


def re_dump(name, out, ref):
    pipeline = import_file(name)
    ws = WignerSeitzAnalysisModifier(
        per_type_occupancies=True,
        affine_mapping=ReferenceConfigurationModifier.AffineMapping.ToReference)
    ws.reference.load(ref)
    pipeline.modifiers.append(ws)
    pipeline.modifiers.append(SelectExpressionModifier(expression =
        "(ParticleType == 1 && Occupancy.1 == 1 && Occupancy.2 == 0) || \
        (ParticleType == 2 && Occupancy.1 == 0 && Occupancy.2 == 1)"))
    pipeline.modifiers.append(DeleteSelectedModifier())
    export_file(pipeline, out, "xyz", columns=[
        "Particle Type",
        "Position.X",
        "Position.Y",
        "Position.Z", 
        "Occupancy.1",
        "Occupancy.2"], multiple_frames=True)
    print(out)

re_dump("output/dumps/dump1500smol.atom",
        "output/results/raw/raw1500smol.xyz",
        "data_init/Fe3O4_ovito.lmp")
re_dump("output/dumps/dump1500medium.atom",
        "output/results/raw/raw1500medium.xyz",
        "data_init/Fe3O4_ovito_medium.lmp")
re_dump("output/dumps/dump1500big.atom",
        "output/results/raw/raw1500big.xyz",
        "data_init/Fe3O4_ovito_big.lmp")
re_dump("output/dumps/dump1500long.atom",
        "output/results/raw/raw1500long.xyz",
        "data_init/Fe3O4_ovito_big.lmp")
        

