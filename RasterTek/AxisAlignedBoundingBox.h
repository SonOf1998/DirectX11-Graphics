#pragma once
class AxisAlignedBoundingBox
{
	struct Plane
	{
		XMFLOAT3 normal;
		XMFLOAT3 point;		// a point on it
	};

	XMFLOAT3 modelVertices[8];
	XMFLOAT3 worldVertices[8];

	std::vector<Plane> GetPlanesFromViewProj(const XMMATRIX& viewProj) const;

public:
	AxisAlignedBoundingBox(float xMin, float xMax, float yMin, float yMax, float zMin, float zMax);
	~AxisAlignedBoundingBox() = default;

	void RecalculateVertices(const XMMATRIX& modelMatrix);
	bool IsInsideViewFrustum(const XMMATRIX& viewProj);
};

