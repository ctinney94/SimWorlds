#ifndef _MY_EFFECTS_FACTORY_H_
#define _MY_EFFECTS_FACTORY_H_
#include "Effects.h"
#include "DXUT.h"
#include "SDKmisc.h"
//helper class required by the model system of DirectXTK
using namespace DirectX;

class MyEffectFactory : public EffectFactory
{
public:
	MyEffectFactory(_In_ ID3D11Device* device) : EffectFactory(device) { *searchPath = 0; }

	virtual void __cdecl CreateTexture(_In_z_ const WCHAR* name, _In_opt_ ID3D11DeviceContext* deviceContext, _Outptr_ ID3D11ShaderResourceView** textureView) override
	{
		WCHAR fname[MAX_PATH] = { 0 };
		if (*searchPath)
			wcscpy_s(fname, searchPath);
		wcscat_s(fname, name);

		WCHAR path[MAX_PATH] = { 0 };
		if (FAILED(DXUTFindDXSDKMediaFileCch(path, MAX_PATH, fname)))
		{
			throw std::exception("Media not found");
		}

		EffectFactory::CreateTexture(path, deviceContext, textureView);
	}

	void SetPath(const WCHAR* path) { if (path) { wcscpy_s(searchPath, path); } else { *searchPath = 0; } }

private:
	WCHAR searchPath[MAX_PATH];
};

#endif