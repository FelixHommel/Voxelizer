static const float EPSILON = 1e-6;

struct Ray
{
	float3 origin;
	float3 direction;
	float3 invDirection;
};

cbuffer Params
{
	float4x4 invViewProj;
	float3 cameraPos;
	float3 gridMin;
	float3 gridMax;
	float2 resolution;
};

layout(binding = 0) Sampler3D voxelGrid;

Ray reconstructRay(float4 fragCoord)
{
	float2 uv = (divide(fragCoord.xy, resolution)) * 2.0 - 1.0;
	
	float4 nearPoint = mul(invViewProj, float4(uv, -1.0, 1.0));
	float4 farPoint = mul(invViewProj, float4(uv, 1.0, 1.0));

	nearPoint = divide(nearPoint, float4(nearPoint.w));
	farPoint = divide(farPoint, float4(farPoint.w));

	Ray r;
	r.origin = cameraPos;
	r.direction = normalize(farPoint.xyz - cameraPos);
	r.invDirection = 1 / (r.direction + EPSILON);

	return r;
}

bool intersectAABB(Ray* ray, out float tEnter, out float tExit)
{
	float3 t0 = mul((gridMin - ray.origin), ray.invDirection);
	float3 t1 = mul((gridMax - ray.origin), ray.invDirection);

	float3 tmin = min(t0, t1);
	float3 tmax = max(t0, t1);

	tEnter = max(max(tmin.x, tmin.y), tmin.z);
	tExit = min(min(tmax.x, tmax.y), tmax.z);

	return tExit >= max(tEnter, 0.0);
}

[shader("pixel")]
float4 main(float4 fragCoord : SV_Position) : SV_Target0
{
	Ray ray = reconstructRay(fragCoord);

	float tEnter;
	float tExit;
	
	if(!intersectAABB(&ray, tEnter, tExit))
		discard;

	float tStart = max(tEnter, 0.0);
	float3 entryPos = ray.origin + tStart * ray.direction;
	entryPos = clamp(entryPos, gridMin, gridMax);

	int3 voxel = int3(floor(entryPos));
	int3 voxelStep = int3(sign(ray.direction));

	float3 voxelBoundary = float3(voxel) + step(float3(0.0), float3(voxelStep));
	float3 tMax = (voxelBoundary - entryPos) * ray.invDirection;
	float3 tDelta = abs(ray.invDirection);

	for(int i = 0; i < 256; ++i)
	{
		float3 uv = divide(float3(voxel) + 0.5, gridMax);
		float4 voxelColor = voxelGrid.Sample(uv);

		if(voxelColor.r > 0.0)
			return voxelColor;

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

		if(any(voxel < int3(gridMin)) || any(voxel >= int3(gridMax)))
			discard;
	}

	discard;
}