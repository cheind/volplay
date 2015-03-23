# volplay

volplay is a library for creating, manipulating and interacting with volumetric data.Focus of this library is clean, reference code and not performance. All computations carried out on CPU with minimal focus on parallelism. Written in C++11.

## Creating and Manipulating Signed Distance Fields
A signed distance field in volplay is represented by a hierarchy of `volplay::SDFNode`. Leaf nodes represent primitives such as spheres, boxes and planes. Intermediate nodes encapsulate functions on other nodes such as intersection, union, difference, repetition and transformation. A helper class `volplay::SDFMake` is provided to simplify the creation of nodes. Here are some examples

```cpp
#include <volplay/sdf_sphere.h>
#include <volplay/sdf_plane.h>
#include <volplay/sdf_make.h>

...

namespace vp = volplay;

vp::SDFNodePtr scene = vp::SDFMake::sphere(1) +
                       vp::SDFMake::sphere(1).translate(vp::Vector(0.8f, 0.8f, 0.8f));
```

Here the `sphere()` method creates an `volplay::SDFSphere` node, the `translate()` method creates an `volplay::SDFRidigTransform` intermediate node and `+` operator creates an intermediate node of type `volplay::SDFUnion`. This results in the following scene graph hierarchy.

![Image](etc/images/samplediagram.png?raw=true)


# References

http://aka-san.halcy.de/distance_fields_prefinal.pdf
http://www.iquilezles.org/www/articles/distfunctions/distfunctions.htm
http://www.ronaldperry.org/SIG2006Course_FriskenPerryDistFields.pdf
http://cg.ibds.kit.edu/downloads/IntModelingSDF.pdf
http://iquilezles.org/www/material/nvscene2008/rwwtt.pdf
http://elib.uni-stuttgart.de/opus/volltexte/2010/5229/pdf/DIP_2970.pdf
http://csokavar.hu/raytrace/imm6392.pdf
http://9bitscience.blogspot.co.at/2013/07/raymarching-distance-fields_14.html
http://www.arcsynthesis.org/gltut/Illumination/Tut12%20Monitors%20and%20Gamma.html
http://www.cambridgeincolour.com/tutorials/gamma-correction.htm
