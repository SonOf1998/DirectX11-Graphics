#include "pch.h"
#include "AxisAlignedBoundingBox.h"

#include "Geometry.h"

std::vector<AxisAlignedBoundingBox::Plane> AxisAlignedBoundingBox::GetPlanesFromViewProj(const XMMATRIX& viewProjInv) const
{
	std::vector<Plane> planes;

	XMVECTOR rightTopBehind    = XMVectorSet(1, 1, 1, 1) * viewProjInv;
	XMVECTOR leftTopBehind     = XMVectorSet(-1, 1, 1, 1) * viewProjInv;
	XMVECTOR rightBottomBehind = XMVectorSet(1, -1, 1, 1) * viewProjInv;
	XMVECTOR leftBottomBehind  = XMVectorSet(-1, -1, 1, 1) * viewProjInv;
	XMVECTOR rightTopFront     = XMVectorSet(1, 1, 0, 1) * viewProjInv;
	XMVECTOR leftTopFront      = XMVectorSet(-1, 1, 0, 1) * viewProjInv;
	XMVECTOR rightBottomFront  = XMVectorSet(1, -1, 0, 1) * viewProjInv;
	XMVECTOR leftBottomFront   = XMVectorSet(-1, -1, 0, 1) * viewProjInv;
	
	rightTopBehind		/= XMVectorGetW(rightTopBehind);
	leftTopBehind		/= XMVectorGetW(leftTopBehind);
	rightBottomBehind	/= XMVectorGetW(rightBottomBehind);
	leftBottomBehind	/= XMVectorGetW(leftBottomBehind);
	rightTopFront		/= XMVectorGetW(rightTopFront);
	leftTopFront		/= XMVectorGetW(leftTopFront);
	rightBottomFront	/= XMVectorGetW(rightBottomFront);
	leftBottomFront		/= XMVectorGetW(leftBottomFront);

	XMVECTOR backPlaneNormal	= XMVector3Normalize(XMVector3Cross(leftTopBehind - rightTopBehind, rightBottomBehind - rightTopBehind));
	XMVECTOR leftPlaneNormal	= XMVector3Normalize(XMVector3Cross(leftTopFront - leftTopBehind, leftBottomBehind - leftTopBehind));
	XMVECTOR rightPlaneNormal	= XMVector3Normalize(XMVector3Cross(rightTopBehind - rightTopFront, rightBottomFront - rightTopFront));
	XMVECTOR bottomPlaneNormal	= XMVector3Normalize(XMVector3Cross(rightBottomBehind - rightBottomFront, leftBottomFront - rightBottomFront));
	XMVECTOR topPlaneNormal		= XMVector3Normalize(XMVector3Cross(rightTopBehind - leftTopBehind, leftTopFront - leftTopBehind));
	XMVECTOR frontPlaneNormal	= XMVector3Normalize(XMVector3Cross(rightTopFront - leftTopFront, leftBottomFront - leftTopFront));

	Plane plane = {};
	XMStoreFloat3(&(plane.normal), backPlaneNormal);
	XMStoreFloat3(&(plane.point), rightTopBehind);
	planes.push_back(plane);
	XMStoreFloat3(&(plane.normal), leftPlaneNormal);
	XMStoreFloat3(&(plane.point), leftTopBehind);
	planes.push_back(plane);
	XMStoreFloat3(&(plane.normal), rightPlaneNormal);
	XMStoreFloat3(&(plane.point), rightTopBehind);
	planes.push_back(plane);
	XMStoreFloat3(&(plane.normal), bottomPlaneNormal);
	XMStoreFloat3(&(plane.point), rightBottomBehind);
	planes.push_back(plane);
	XMStoreFloat3(&(plane.normal), topPlaneNormal);
	XMStoreFloat3(&(plane.point), rightTopBehind);
	planes.push_back(plane);
	XMStoreFloat3(&(plane.normal), frontPlaneNormal);
	XMStoreFloat3(&(plane.point), rightBottomFront);
	planes.push_back(plane);

	return planes;
}

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

AxisAlignedBoundingBox::AxisAlignedBoundingBox(Geometry* geometry) : AxisAlignedBoundingBox(geometry->xMin, geometry->xMax, geometry->yMin, geometry->yMax, geometry->zMin, geometry->zMax)
{
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
	
	minVertex = XMFLOAT3(newXMin, newYMin, newZMin);
	maxVertex = XMFLOAT3(newXMax, newYMax, newZMax);
}

bool AxisAlignedBoundingBox::IsInsideViewFrustum(const XMMATRIX& viewProj, const XMMATRIX& viewProjInv)
{
	std::vector<Plane> planes = GetPlanesFromViewProj(viewProjInv);
	
	XMFLOAT3 closestAABBVertex;
	
	bool allPlanePositive = true;

	for (int i = 0; i < 6; ++i)
	{
		Plane plane = planes[i];

		if (plane.normal.x >= 0)
		{
			closestAABBVertex.x = maxVertex.x;
		}
		else
		{
			closestAABBVertex.x = minVertex.x;
		}
		if (plane.normal.y >= 0)
		{
			closestAABBVertex.y = maxVertex.y;
		}
		else
		{
			closestAABBVertex.y = minVertex.y;
		}
		if (plane.normal.z >= 0)
		{
			closestAABBVertex.z = maxVertex.z;
		}
		else
		{
			closestAABBVertex.z = minVertex.z;
		}

		if (Dot(closestAABBVertex - plane.point, plane.normal) < 0)
		{
			return false;
		}
	}
	
	return true;
}
