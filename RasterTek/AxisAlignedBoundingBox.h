#pragma once
class AxisAlignedBoundingBox
{
	XMFLOAT3 modelVertices[8];
	XMFLOAT3 worldVertices[8];

public:
	AxisAlignedBoundingBox(float xMin, float xMax, float yMin, float yMax, float zMin, float zMax);
	~AxisAlignedBoundingBox() = default;

	void RecalculateVertices(const XMMATRIX& modelMatrix);
	bool IsInsideViewFrustum(const XMMATRIX& viewProj);
};

