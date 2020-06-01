#pragma once


class Camera;
class ShaderProgram;
class Mesh;

class GameObject
{

protected:

	std::vector<std::unique_ptr<Mesh>> meshes;

	XMMATRIX modelMatrix;

	XMVECTOR position;
	XMVECTOR scale;
	XMMATRIX rotation;
	
public:

	GameObject(ID3D11Device*, ID3D11DeviceContext*, XMVECTOR = XMVectorSet(0,0,0,0), XMVECTOR = XMVectorSet(1,1,1,1), XMVECTOR = XMVectorSet(0,0,1,0), float = 0.0f);
	GameObject(const GameObject&)	= delete;
	virtual ~GameObject()			= default;

	virtual void Render(ID3D11DeviceContext*, ShaderProgram*, Camera* = nullptr) = 0;
	virtual void Animate(float t, float dt) = 0;

	XMMATRIX GetModelMatrix() const;
	void addMesh(Mesh*);
	void addMesh(std::unique_ptr<Mesh>&&);
};
