#version 330 core

out vec4 FragColor;

uniform mat4 invViewProj;
uniform vec3 cameraPos;
uniform vec3 gridMin;
uniform vec3 gridMax;
uniform vec2 resolution;

uniform sampler3D voxelGrid;

/// \brief Simple structure representing a ray by its origin and direction.
///
/// \author Felix Hommel
/// \date 3/18/2026
struct Ray
{
    vec3 origin;
    vec3 direction;
    vec3 invDirection;
};

/// \brief Reverse engineer a viewing ray based on the inverse view projection matrix.
///
/// \return Ray viewing ray for a pixel
Ray reconstructRay()
{
    vec2 uv = (gl_FragCoord.xy / resolution) * 2.0 - 1.0;

    vec4 nearPoint = invViewProj * vec4(uv, -1.0, 1.0);
    vec4 farPoint = invViewProj * vec4(uv, 1.0, 1.0);

    nearPoint /= nearPoint.w;
    farPoint /= farPoint.w;

    Ray r;
    // NOTE: Not sure which of the two origin options is the correct one
    // r.origin = nearPoint.xyz;
    // r.direction = normalize(farPoint.xyz - nearPoint.xyz);
    r.origin = cameraPos;
    r.direction = normalize(farPoint.xyz - cameraPos);
    r.invDirection = 1 / r.direction;

    return r;
}

// NOTE: Reference
// https://en.wikipedia.org/wiki/Slab_method
// https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-box-intersection.html
bool intersectAABB(Ray ray, out float tEnter, out float tExit)
{
    vec3 t0 = (gridMin - ray.origin) * ray.invDirection;
    vec3 t1 = (gridMax - ray.origin) * ray.invDirection;

    vec3 tmin = min(t0, t1);
    vec3 tmax = max(t0, t1);

    tEnter = max(max(tmin.x, tmin.y), tmin.z);
    tExit = min(min(tmax.x, tmax.y), tmax.z);

    return tExit >= max(tEnter, 0.0);
}

void main()
{
    Ray ray = reconstructRay();

    float tEnter;
    float tExit;

    if(!intersectAABB(ray, tEnter, tExit))
    {
        // FragColor = vec4(0.0, 0.0, 0.0, 1.0);
        // return;
        discard;
    }

    float tStart = max(tEnter, 0.0);
    vec3 entryPos = ray.origin + tStart * ray.direction;
    entryPos = clamp(entryPos, gridMin, gridMax); // NOTE: Ensure that entryPos is within grid bounds (avoid floating-point errors)

    // NOTE: Convert the entry position to a voxel grid position 
    ivec3 voxel = ivec3(floor(entryPos));

    // NOTE: Determine the next voxel by getting the sign of the components (+1 means step forward in this direction, -1 means step beackwards in this direction)
    ivec3 voxelStep = ivec3(sign(ray.direction));

    vec3 voxelBoundary = vec3(voxel) + step(vec3(0), vec3(voxelStep)); // NOTE: Determine the next voxel boundary plane
    vec3 tMax = (voxelBoundary - entryPos) * ray.invDirection; // NOTE: How far along the ray do we cross into the next voxel boundary
    vec3 tDelta = abs(ray.invDirection); // NOTE: How far do we move in t to cross one voxel in each axis

    for(int i = 0; i < 256; ++i)
    {
        // NOTE: Sample voxel
        vec3 uv = (vec3(voxel) + 0.5) / gridMax;
        vec4 voxelColor = texture(voxelGrid, uv);
        
        // NOTE: If voxel has a color attached to it (assuming a color value of 0.0 means no color) then use it as fragment color and end ray traversal
        if(voxelColor.r > 0.0)
        {
            FragColor = voxelColor;
            return;
        }

        // NOTE: Determine the closest edge
        if(tMax.x < tMax.y)
        {
            if(tMax.x < tMax.z)
            {
                voxel.x += voxelStep.x;
                tMax.x += tDelta.x;
            }
            else
            {
                voxel.z += voxelStep.z;
                tMax.z += tDelta.z;
            }
        }
        else
        {
            if(tMax.y < tMax.z)
            {
                voxel.y += voxelStep.y;
                tMax.y += tDelta.y;
            }
            else
            {
                voxel.z += voxelStep.z;
                tMax.z += tDelta.z;
            }
        }

        if(any(lessThan(voxel, ivec3(gridMin))) || any(greaterThanEqual(voxel, ivec3(gridMax))))
        {
            // NOTE: Alternatively just break => would cause black color for all "empty" voxels
            discard;
        }
    }

    // NOTE: If ray exhausted all loop iterations without hitting a voxel then discard the fragment.
    discard;
}
