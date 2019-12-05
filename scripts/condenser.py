from ovito.io import *
from ovito.data import *
from ovito.modifiers import *
from ovito.pipeline import *


def re_dump(name, ref, out):
    pipeline = import_file(name)
    ws = WignerSeitzAnalysisModifier(
        per_type_occupancies=True,
        affine_mapping=ReferenceConfigurationModifier.AffineMapping.ToReference)
    ws.reference.load(ref)
    pipeline.modifiers.append(ws)
    pipeline.modifiers.append(SelectExpressionModifier(
    		expression = "(ParticleType == 1 && Occupancy.1 == 1 && Occupancy.2 == 0) || (ParticleType == 2 && Occupancy.1 == 0 && Occupancy.2 == 1)"))
    pipeline.modifiers.append(DeleteSelectedModifier())
    export_file(pipeline, out, "xyz",
                columns=["Particle Type", "Position.X", "Position.Y", "Position.Z", "Occupancy.1", "Occupancy.2"], multiple_frames=True)


for i in ["800", "900", "1000", "1100", "1200", "1300", "1400", "1425", "1450", "1475", "1500", "1525", "1550", "1575", "1600", "1700", "1800", "1900"]:
    re_dump("output/dumps/dump" + i + ".atom",
            "output/reference/dump" + i + "reference.atom",
            "output/results/raw/raw" + i + ".xyz")
    print(i)

