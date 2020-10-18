#pragma once
#include "Renderable.h"
#include "RoundManager.h"

class BallObject;
class CueObject;
class PerspectiveCamera;
class WhiteBallObject;

class BallSet : public Renderable
{
	std::vector<std::unique_ptr<BallObject>> balls;
	PerspectiveCamera* camera;
	WhiteBallObject* whiteBallRef;	

	// for game restart //
	// needs refactoring //
	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;
	CueObject* cue;

public:
	BallSet(ID3D11Device* device, ID3D11DeviceContext* deviceContext, PerspectiveCamera* camera, CueObject* cue);
	~BallSet();

	void InitTable();

	virtual void Render(ID3D11DeviceContext* device, Pipeline* pipeline, Camera* camera = nullptr, Light* light = nullptr) override;
	virtual void RenderToShadowMap(ID3D11DeviceContext* deviceContext, Pipeline* pipeline, Light* light);
	virtual void Animate(float t, float dt) override;

	XMVECTOR GetClosestColorToCueBall(const XMVECTOR& cueBallPos, TARGET* targetOutput) const;
	XMVECTOR GetClosestTargetBallToCueBall(const XMVECTOR& cueBallPos, TARGET target)   const;
	XMVECTOR GetWhiteBallPosition()														const;
	void MakeWhiteMovable(); /* for black ball game situatuation */
	bool IsPlaceUsed(const XMVECTOR& place, bool excludeWhite = false)					const;
	bool HasReds()																		const;
	bool HasLower(TARGET target)														const;
};

