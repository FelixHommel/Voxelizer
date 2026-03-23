static const float2 verts[3] = float2[](
	float2(-1.0, -1.0),
	float2( 3.0, -1.0),
	float2(-1.0,  3.0)
);

[shader("vertex")]
float4 main(uint vertexID : SV_VertexID) : SV_Position
{
	// NOTE: Simply generate a fullscreen triangle to render to
	return float4(verts[vertexID], 0.0, 1.0);;
}