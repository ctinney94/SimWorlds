#include "CMOGO.h"
#include "model.h"
#include <windows.h>
#include "Effects.h"
//#include "DXUT.h"
//#include "SDKmisc.h"
#include "MyEffectsFactory.h"
#include "camera.h"
#include "CommonStates.h"
#include "drawdata.h"

ID3D11RasterizerState*  CMOGO::s_pRasterState = nullptr;

using namespace DirectX;

#include "helper.h"


CMOGO::CMOGO(string _fileName, ID3D11Device* _pd3dDevice, MyEffectFactory* _EF) :m_model(nullptr)
{
	//the CMGOs need this raster state
	//if we've not created it yet do so now
	if (!s_pRasterState)
	{
		//Setup Raster State
		D3D11_RASTERIZER_DESC rasterDesc;
		rasterDesc.AntialiasedLineEnable = false;
		rasterDesc.CullMode = D3D11_CULL_BACK;
		rasterDesc.DepthBias = 0;
		rasterDesc.DepthBiasClamp = 0.0f;
		rasterDesc.DepthClipEnable = true;
		rasterDesc.FillMode = D3D11_FILL_SOLID;
		rasterDesc.FrontCounterClockwise = false;
		rasterDesc.MultisampleEnable = false;
		rasterDesc.ScissorEnable = false;
		rasterDesc.SlopeScaledDepthBias = 0.0f;

		// Create the rasterizer state from the description we just filled out.
		HRESULT hr = _pd3dDevice->CreateRasterizerState(&rasterDesc, &s_pRasterState);
	}

	WCHAR str[MAX_PATH];
	wchar_t* file = Helper::charToWChar(_fileName.c_str());

	DXUTFindDXSDKMediaFileCch(str, MAX_PATH, file);
	m_model = Model::CreateFromCMO(_pd3dDevice, str, *_EF, true);
}

CMOGO::~CMOGO()
{
	//model shouldn't need deleting as it's linked to by a unique_ptr
}

void CMOGO::Tick(GameData* _GD)
{
	GameObject::Tick(_GD);
	m_worldMat = m_fudge * m_worldMat;
}

void CMOGO::Draw(DrawData* _DD)
{
	//a dirty hack as the CMO model drawer breaks the depth stencil state
	ID3D11DepthStencilState *DSS = 0;
	UINT ref;
	//pick up a copy of the current state...
	_DD->pd3dImmediateContext->OMGetDepthStencilState(&DSS, &ref);

	m_model->Draw(_DD->pd3dImmediateContext, *_DD->states, //graphics device and CommonStates reuqired by model system
		m_worldMat, //world transform to poisiton this model in the world
		_DD->cam->GetView(), _DD->cam->GetProj(), //veiw and projection matrix of the camera
		false, //NO! I don't want wireframe blocked by the raster state anyhow
		[&](){_DD->pd3dImmediateContext->RSSetState(s_pRasterState);} //this VERY weird construction creates a function on the fly to set up the render states correctly else the model system overrides them BADLY
		);

	//...and put the depth stencil state back again
	_DD->pd3dImmediateContext->OMSetDepthStencilState(DSS, ref);
	if (DSS)DSS->Release();
}