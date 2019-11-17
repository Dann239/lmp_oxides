from ovito.io import import_file
from ovito.data import *
from ovito.modifiers.particles import *
from ovito.pipeline.particles import *

pipeline = import_file("output/dumps/dump900diff.atom")

pipeline.modifiers.append(WignerSeitzAnalysisModifier(
    per_type_occupancies=True,
    affine_mapping=ReferenceConfigurationModifier.AffineMapping.ToReference))

