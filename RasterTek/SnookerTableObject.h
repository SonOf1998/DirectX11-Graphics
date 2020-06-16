#pragma once
#include "GameObject.h"

class Camera;
class Light;
class ShaderProgram;

class SnookerTableObject : public GameObject
{

	void BuildTable(ID3D11Device*, ID3D11DeviceContext*);

public:
	SnookerTableObject(ID3D11Device*, ID3D11DeviceContext*, XMVECTOR = SNOOKER_TABLE_TRANSLATE, XMVECTOR = SNOOKER_TABLE_SCALE, XMVECTOR = SNOOKER_TABLE_ROT_AXIS, float = SNOOKER_TABLE_ROT_ANGLE);
	~SnookerTableObject() = default;

	virtual void Render(ID3D11DeviceContext*, ShaderProgram*, Camera* = nullptr, Light* = nullptr) override;
	void RenderToShadowMap(ID3D11DeviceContext*, ShaderProgram*, Light*) override;
	virtual void Animate(float t, float dt) override;
};

