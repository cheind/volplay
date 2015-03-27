# volplay

volplay is a library for creating, manipulating and interacting with volumetric data.Focus of this library is clean, reference code and not performance. All computations carried out on CPU with minimal focus on parallelism. Written in C++11.

## Creating and Manipulating Signed Distance Fields

A signed distance field in volplay is represented by a hierarchy of `volplay::SDFNode`. Leaf nodes represent primitives such as spheres, boxes and planes. Intermediate nodes encapsulate functions on other nodes such as intersection, union, difference, repetition and transformation. A helper class `volplay::SDFMake` is provided to simplify the creation of nodes. Here are some examples

```cpp
#include <volplay/sdf_sphere.h>
#include <volplay/sdf_make.h>

...

namespace vp = volplay;

vp::SDFNodePtr scene = vp::SDFMake::sphere(1) +
                       vp::SDFMake::sphere(1).translate(vp::Vector(0.8f, 0.8f, 0.8f));
```

Here the `sphere()` method creates an `volplay::SDFSphere` node, the `translate()` method creates an `volplay::SDFRidigTransform` intermediate node and `+` operator creates an intermediate node of type `volplay::SDFUnion`. This results in the following scene graph hierarchy.

![Image](etc/images/samplediagram.png?raw=true)

## Ray-tracing Signed Distance Fields

The namespace `volplay::rendering` provides methods to directly visualize the signed distance field by ray-tracing it. Besides the signed distance field itself, rendering can make use of material properties of individual nodes. The ray-tracing pipeline is implemented in `volplay::rendering::Renderer` and involves the following other entities: 
 - A scene `volplay::SDFNode` to be rendered. Each node can be attributed with materials `volplay::rendering::Material`
 - A camera `volplay::rendering::Camera` defining the viewpoint and lens parameters.
 - A set of lights `volplay::rendering::Lights`.
 - A set of image generators `volplay::rendering::ImageGenerator` encapsulating the types of images to be generated.

The example [example_raytracer.cpp](examples/example_raytracer.cpp) demonstrates rendering of two spheres resting on a plane.
When executed the following set of images is generated. From left to right: Blinn-Phong shaded image with shadows and materials applied, depth image of scene and heat image of scene showing hotspots of raytracing.

![BlinnPhong shaded image](etc/images/raytrace.png?raw=true)

## Surface Reconstruction from Signed Distance Fields

The namespace `volplay::surface` provides methods to explicitly generate a polygonal mesh from an iso-surface in a signed distance field. Currently an implementation of Dual Contouring `volplay::surface::DualContouring` is available. Results can be exported in .OFF format. Here is an example

```cpp
#include <volplay/sdf_sphere.h>
#include <volplay/sdf_plane.h>
#include <volplay/sdf_make.h>
#include <volplay/surface/dual_contouring.h>
#include <volplay/surface/off_export.h>

...

namespace vp = volplay;
namespace vps = volplay::surface;

vp::SDFNodePtr scene = vp::make()
    .difference()
        .join()
            .sphere().radius(1)
            .transform().translate(vp::Vector(0.8f, 0.8f, 0.8f))
                .sphere().radius(1)
            .end()
        .end()
        .plane().normal(vp::Vector::UnitZ())
    .end();

vps::DualContouring dc;
vps::IndexedSurface surface = dc.extractSurface(
                       scene, 
                       vp::Vector(-2,-2,-2), vp::Vector(2,2,2), 
                       vp::Vector::Constant(0.05f));

vps::OFFExport off; 
off.exportSurface("surface.off", surface);
```

This produces the surface below. One of the big benefits of Dual Contouring  is that it naturally preserves sharp features.

![Dual Contouring](etc/images/dualcontouring.png?raw=true)


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
