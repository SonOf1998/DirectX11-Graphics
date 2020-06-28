#include "pch.h"
#include "AxisAlignedBoundingBox.h"


std::vector<AxisAlignedBoundingBox::Plane> AxisAlignedBoundingBox::GetPlanesFromViewProj(const XMMATRIX& viewProj) const
{
	std::vector<Plane> planes;

	XMMATRIX inverseViewProj = XMMatrixInverse(nullptr, viewProj);

	XMVECTOR rightTopBehind    = XMVectorSet(1, 1, 1, 1) * inverseViewProj;
	XMVECTOR leftTopBehind     = XMVectorSet(-1, 1, 1, 1) * inverseViewProj;
	XMVECTOR rightBottomBehind = XMVectorSet(1, -1, 1, 1) * inverseViewProj;
	XMVECTOR leftBottomBehind  = XMVectorSet(-1, -1, 1, 1) * inverseViewProj;
	XMVECTOR rightTopFront     = XMVectorSet(1, 1, 0, 1) * inverseViewProj;
	XMVECTOR leftTopFront      = XMVectorSet(-1, 1, 0, 1) * inverseViewProj;
	XMVECTOR rightBottomFront  = XMVectorSet(1, -1, 0, 1) * inverseViewProj;
	XMVECTOR leftBottomFront   = XMVectorSet(-1, -1, 0, 1) * inverseViewProj;
	
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
	std::vector<Plane> planes = GetPlanesFromViewProj(viewProj);

	XMFLOAT3 minVertex = worldVertices[0];
	XMFLOAT3 maxVertex = worldVertices[7];

	XMFLOAT3 P;
	XMFLOAT3 Q;

	bool allPlanePositive = true;

	for (int i = 0; i < 6; ++i)
	{
		Plane plane = planes[i];

		if (plane.normal.x >= 0)
		{
			P.x = minVertex.x;
			Q.x = maxVertex.x;
		}
		else
		{
			P.x = maxVertex.x;
			Q.x = minVertex.x;
		}
		if (plane.normal.y >= 0)
		{
			P.y = minVertex.y;
			Q.y = maxVertex.y;
		}
		else
		{
			P.y = maxVertex.y;
			Q.y = minVertex.y;
		}
		if (plane.normal.z >= 0)
		{
			P.z = minVertex.z;
			Q.z = maxVertex.z;
		}
		else
		{
			P.z = maxVertex.z;
			Q.z = minVertex.z;
		}

		if (Dot((P - plane.point), plane.normal) < 0)
		{
			allPlanePositive = false;
		}

		if (Dot((P - plane.point), plane.normal) < 0 && Dot((Q - plane.point), plane.normal) > 0)
		{
			return true;
		}
	}

	if (allPlanePositive)
	{
		return true;
	}

	return allPlanePositive;
}
