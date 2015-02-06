#ifndef _CMOGO_H_
#define _CMOGO_H_
#include "gameobject.h"
#include "Model.h"
#include <string>
#include "DXUT.h"
#include "DXUTgui.h"
#include "DXUTmisc.h"
//base class for using CMO models
using namespace std;
using namespace DirectX;

class MyEffectFactory; 
struct DrawData;

class CMOGO : public GameObject
{
public:
	CMOGO(string _fileName, ID3D11Device* _pd3dDevice, MyEffectFactory* _EF);
	virtual ~CMOGO();

	virtual void Tick(GameData* _GD);
	virtual void Draw(DrawData* _DS);

protected:
	unique_ptr<Model>  m_model;

	//needs a slightly different raster state that the VBGOs so create one and let them all use it
	static ID3D11RasterizerState*  s_pRasterState;
};

#endif
