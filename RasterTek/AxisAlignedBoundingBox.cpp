#include "pch.h"
#include "AxisAlignedBoundingBox.h"


AxisAlignedBoundingBox::AxisAlignedBoundingBox(float xMin, float xMax, float yMin, float yMax, float zMin, float zMax)
{
	modelVertices[0] = XMFLOAT3(xMin, yMin, zMin);
	modelVertices[1] = XMFLOAT3(xMax, yMin, zMin);
	modelVertices[2] = XMFLOAT3(xMin, yMax, zMin);
	modelVertices[3] = XMFLOAT3(xMax, yMax, zMin);
	modelVertices[4] = XMFLOAT3(xMin, yMin, zMax);
	modelVertices[5] = XMFLOAT3(xMax, yMin, zMax);
	modelVertices[6] = XMFLOAT3(xMin, yMax, zMax);
	modelVertices[7] = XMFLOAT3(xMax, yMax, zMax);
}

void AxisAlignedBoundingBox::RecalculateVertices(const XMMATRIX& modelMatrix)
{
	float newXMin, newYMin, newZMin;
	float newXMax, newYMax, newZMax;

	newXMin = newYMin = newZMin = std::numeric_limits<float>::max();
	newXMax = newYMax = newZMax = std::numeric_limits<float>::lowest();

	for (int i = 0; i < 8; ++i)
	{
		XMVECTOR modelVertex = XMVectorSet(modelVertices[i].x, modelVertices[i].y, modelVertices[i].z, 1);
		XMVECTOR vertexVec = modelVertex * modelMatrix;
		XMFLOAT3 vertex;

		XMStoreFloat3(&vertex, vertexVec);

		if (vertex.x < newXMin)
		{
			newXMin = vertex.x;
		}
		if (vertex.x > newXMax)
		{
			newXMax = vertex.x;
		}

		if (vertex.y < newYMin)
		{
			newYMin = vertex.y;
		}
		if (vertex.y > newYMax)
		{
			newYMax = vertex.y;
		}

		if (vertex.z < newZMin)
		{
			newZMin = vertex.z;
		}
		if (vertex.z > newZMax)
		{
			newZMax = vertex.z;
		}
	}

	worldVertices[0] = XMFLOAT3(newXMin, newYMin, newZMin);
	worldVertices[1] = XMFLOAT3(newXMax, newYMin, newZMin);
	worldVertices[2] = XMFLOAT3(newXMin, newYMax, newZMin);
	worldVertices[3] = XMFLOAT3(newXMax, newYMax, newZMin);
	worldVertices[4] = XMFLOAT3(newXMin, newYMin, newZMax);
	worldVertices[5] = XMFLOAT3(newXMax, newYMin, newZMax);
	worldVertices[6] = XMFLOAT3(newXMin, newYMax, newZMax);
	worldVertices[7] = XMFLOAT3(newXMax, newYMax, newZMax);
}

bool AxisAlignedBoundingBox::IsInsideViewFrustum(const XMMATRIX& viewProj)
{
	for (int i = 0; i < 8; ++i)
	{
		XMVECTOR worldVertex = XMVectorSet(worldVertices[i].x, worldVertices[i].y, worldVertices[i].z, 1);
		XMVECTOR vertexHom = worldVertex * viewProj;
		XMFLOAT4 vertex;
		XMStoreFloat4(&vertex, vertexHom);

		vertex.x /= vertex.w;
		vertex.y /= vertex.w;
		vertex.z /= vertex.w;


		// If only one of the vertices of the AABB is inside the NDC, then we need to
		// render. The stuff outside the frustum will be clipped.
		if (fabs(vertex.x) < 1 && fabs(vertex.y) < 1 && fabs(vertex.z - 0.5f) < 0.5f)
		{
			return true;
		}
	}

	return false;
}
