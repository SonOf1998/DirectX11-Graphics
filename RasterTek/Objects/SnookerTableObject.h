#pragma once
#include "GameObject.h"
#include "Positions.h"


class Camera;
class Light;
class Pipeline;

class SnookerTableObject : public GameObject
{
	float energyLossFactor = 0.8f;
	float coeffOfFriction  = 0.2f;

	void BuildTable(ID3D11Device*, ID3D11DeviceContext*);

public:
	SnookerTableObject(ID3D11Device*, ID3D11DeviceContext*, XMVECTOR = SNOOKER_TABLE_TRANSLATE, XMVECTOR = SNOOKER_TABLE_SCALE, XMVECTOR = SNOOKER_TABLE_ROT_AXIS, float = SNOOKER_TABLE_ROT_ANGLE);
	~SnookerTableObject() = default;

	virtual void Render(ID3D11DeviceContext*, Pipeline*, Camera* = nullptr, Light* = nullptr) override;
	void RenderToShadowMap(ID3D11DeviceContext*, Pipeline*, Light*) override;
	virtual void Animate(float t, float dt) override;
};

