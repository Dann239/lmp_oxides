from ovito.io import *
from ovito.data import *
from ovito.modifiers import *
from ovito.pipeline import *


def re_dump(name, out):
    pipeline = import_file(name)
    ws = WignerSeitzAnalysisModifier(
        per_type_occupancies=True,
        affine_mapping=ReferenceConfigurationModifier.AffineMapping.ToReference)
    ws.reference.load("data_init/Fe3O4_ovito_medium.lmp")
    pipeline.modifiers.append(ws)
    pipeline.modifiers.append(SelectExpressionModifier(
    		expression = "(ParticleType == 1 && Occupancy.1 == 1 && Occupancy.2 == 0) || (ParticleType == 2 && Occupancy.1 == 0 && Occupancy.2 == 1)"))
    pipeline.modifiers.append(DeleteSelectedModifier())
    export_file(pipeline, out, "xyz",
                columns=["Particle Type", "Position.X", "Position.Y", "Position.Z", "Occupancy.1", "Occupancy.2"], multiple_frames=True)

postfix = "medium"
for i in ["1500"]:
    re_dump("output/dumps/dump" + i + postfix + ".atom",
            "output/results/raw/raw" + i + postfix + ".xyz")
    print(i)

