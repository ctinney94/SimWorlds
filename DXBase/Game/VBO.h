#ifndef _VBO_H_
#define _VBO_H_
#include "gameobject.h"

class GameData;

struct LamData
{
	XMMATRIX world_view_proj_matrix;
	XMVECTOR Light_Ambient;
	XMVECTOR Light1_Position;
	XMMATRIX inv_world_matrix;
};

struct LamVertex
{
	XMFLOAT4 Pos;
	XMFLOAT4 Norm;
	XMFLOAT2 Tex;
};


class VBO : public GameObject
{
public:
	VBO(ID3D11Device* pd3dDevice);
	~VBO();

	virtual void tick(GameData* GD);
	virtual void draw(ID3D11DeviceContext* pd3dImmediateContext, CommonStates* states, Camera* cam);

protected:

	//vertex layout
	static ID3D11InputLayout* m_VertexLayout;

	//vertex buffer and index buffer
	ID3D11Buffer* m_VB;
	ID3D11Buffer* m_IB;

	//Constant Buffer
	static ID3D11Buffer* m_ConstBuffer;

	//Texture Stuff
	static ID3D11SamplerState* m_SamplerState;

	//shaders
	static ID3D11PixelShader* m_PixelShader;
	static ID3D11VertexShader* m_VertexShader;

	int m_numVerts;
};


#endif