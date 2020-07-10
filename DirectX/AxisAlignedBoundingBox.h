#pragma once

struct Geometry;

class AxisAlignedBoundingBox
{
	struct Plane
	{
		XMFLOAT3 normal;
		XMFLOAT3 point;
	};

	XMFLOAT3 modelVertices[8];

	XMFLOAT3 minVertex;
	XMFLOAT3 maxVertex;
	std::vector<Plane> GetPlanesFromViewProj(const XMMATRIX& viewProjInv) const;

public:
	AxisAlignedBoundingBox(float xMin, float xMax, float yMin, float yMax, float zMin, float zMax);
	AxisAlignedBoundingBox(Geometry* geometry);
	~AxisAlignedBoundingBox() = default;

	void RecalculateVertices(const XMMATRIX& modelMatrix);
	bool IsInsideViewFrustum(const XMMATRIX& viewProj, const XMMATRIX& viewProjInv);
};

