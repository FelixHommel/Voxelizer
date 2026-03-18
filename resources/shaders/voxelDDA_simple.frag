#version 330 core

out vec4 FragColor;

uniform mat4 invViewProj;
uniform vec3 cameraPos;
uniform vec2 resolution;

uniform sampler3D voxelGrid;

void main()
{
    vec2 uv = (gl_FragCoord.xy / resolution) * 2.0 - 1.0;

    vec4 nearPoint = invViewProj * vec4(uv, -1.0, 1.0);
    vec4 farPoint = invViewProj * vec4(uv, 1.0, 1.0);

    nearPoint /= nearPoint.w;
    farPoint /= farPoint.w;

    vec3 rayOrigin = nearPoint.xyz;
    vec3 rayDir = normalize(farPoint.xyz - nearPoint.xyz);

    FragColor = vec4(rayDir * 0.5 + 0.5, 1.0);
}
