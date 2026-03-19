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
    r.origin = nearPoint.xyz;
    // r.origin = cameraPos;
    r.direction = normalize(farPoint.xyz - nearPoint.xyz);

    return r;
}

// https://en.wikipedia.org/wiki/Slab_method
// https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-box-intersection.html
bool intersectAABB(Ray ray, out float tEnter, out float tExit)
{
    vec3 invDir = 1.0 / ray.direction;

    vec3 t0 = (gridMin - ray.origin) * invDir;
    vec3 t1 = (gridMax - ray.origin) * invDir;

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
        // NOTE: Alternatively, just discard the fragment
        FragColor = vec4(0.0, 0.0, 0.0, 1.0);
        return;
    }

    float tStart = max(tEnter, 0.0);
    vec3 entryPos = ray.origin + tStart * ray.direction;

    // FragColor = vec4(1.0, 0.0, 0.0, 1.0);
    FragColor = vec4(entryPos / gridMax, 1.0);
}
