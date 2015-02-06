#include "VBO.h"
#include "camera.h"
#include <d3dcompiler.h>

ID3D11Buffer* VBO::m_ConstBuffer = nullptr;
ID3D11SamplerState* VBO::m_SamplerState = nullptr;
ID3D11InputLayout* VBO::m_VertexLayout = nullptr;
ID3D11PixelShader* VBO::m_PixelShader = nullptr;
ID3D11VertexShader* VBO::m_VertexShader = nullptr;


//--------------------------------------------------------------------------------------
// Helper for compiling shaders with D3DCompile
//
// With VS 11, we could load up prebuilt .cso files instead...
//--------------------------------------------------------------------------------------
HRESULT CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
{
	HRESULT hr = S_OK;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
	// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	dwShaderFlags |= D3DCOMPILE_DEBUG;

	// Disable optimizations to further improve shader debugging
	dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	ID3DBlob* pErrorBlob = nullptr;
	hr = D3DCompileFromFile(szFileName, nullptr, nullptr, szEntryPoint, szShaderModel,
		dwShaderFlags, 0, ppBlobOut, &pErrorBlob);
	if (FAILED(hr))
	{
		if (pErrorBlob)
		{
			OutputDebugStringA(reinterpret_cast<const char*>(pErrorBlob->GetBufferPointer()));
			pErrorBlob->Release();
		}
		return hr;
	}
	if (pErrorBlob) pErrorBlob->Release();

	return S_OK;
}

VBO::VBO(ID3D11Device* pd3dDevice) :m_VB(nullptr), m_IB(nullptr), m_numVerts(0)
{
	//if default shaders have not been created for all these then create them here
	//note that they are all created static

	// Create the constant buffer
	if (!m_ConstBuffer)
	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(LamData);
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = 0;
		HRESULT hr = pd3dDevice->CreateBuffer(&bd, nullptr, &m_ConstBuffer); 
	}

	// Create the sample state
	if (!m_SamplerState)
	{
		D3D11_SAMPLER_DESC sampDesc;
		ZeroMemory(&sampDesc, sizeof(sampDesc));
		sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		sampDesc.MinLOD = 0;
		sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
		HRESULT hr = pd3dDevice->CreateSamplerState(&sampDesc, &m_SamplerState);
	}

	//create pixel shader
	if (!m_PixelShader)
	{
		ID3DBlob* pPSBlob = nullptr;
		HRESULT hr = CompileShaderFromFile(L"../Assets/Lambert.fx", "ps_main", "ps_4_0", &pPSBlob);

		// Create the pixel shader
		hr = pd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &m_PixelShader);
		pPSBlob->Release();
	}

	ID3DBlob* pVSBlob = nullptr;
	//create vertex shader
	if (!m_VertexShader)
	{
		HRESULT hr = CompileShaderFromFile(L"../Assets/Lambert.fx", "vs_main", "vs_4_0", &pVSBlob);

		// Create the vertex shader
		hr = pd3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &m_VertexShader);
		
	}

	//create veretx layout
	if (!m_VertexLayout)
	{
		// Define the input layout
		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		UINT numElements = ARRAYSIZE(layout);

		// Create the input layout
		HRESULT hr = pd3dDevice->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(),
			pVSBlob->GetBufferSize(), &m_VertexLayout);
		pVSBlob->Release();		
	}

	//load a default white texture
}

VBO::~VBO()
{
	if (m_VB)
	{
		m_VB->Release();
		m_VB = nullptr;
	}
	if (m_IB)
	{
		m_IB->Release();
		m_IB = nullptr;
	}
	if (m_VertexLayout)
	{
		m_VertexLayout->Release();
		m_VertexLayout = nullptr;
	}
	if (m_SamplerState)
	{
		m_SamplerState->Release();
		m_SamplerState = nullptr;
	}
	if (m_ConstBuffer)
	{
		m_ConstBuffer->Release();
		m_ConstBuffer = nullptr;
	}
}

void VBO::tick(GameData* GD)
{
	GameObject::tick(GD);
}

void VBO::draw(ID3D11DeviceContext* pd3dImmediateContext, CommonStates* states, Camera* cam)
{
	//set up constat buffer from camera and my world transform
	if (m_VB && m_IB)
	{
		// Set the input layout
		pd3dImmediateContext->IASetInputLayout(m_VertexLayout);

		LamData currData;
		//multiple the worls view and proj matrices together and find their transpose
		currData.world_view_proj_matrix = XMMatrixTranspose( XMLoadFloat4x4(&this->m_worldMat)* XMLoadFloat4x4(&cam->m_viewMat)*XMLoadFloat4x4(&cam->m_projMat) );
		//calculate the inverse of the world matrix and fidn its transpose
		currData.inv_world_matrix = XMMatrixTranspose(XMMatrixInverse(nullptr, XMLoadFloat4x4(&this->m_worldMat)));

		pd3dImmediateContext->UpdateSubresource(m_ConstBuffer, 0, nullptr, &currData, 0, 0);
		pd3dImmediateContext->VSSetConstantBuffers(0, 1, &m_ConstBuffer);
		pd3dImmediateContext->PSSetConstantBuffers(0, 1, &m_ConstBuffer);

		pd3dImmediateContext->VSSetShader(m_VertexShader, nullptr, 0);
		pd3dImmediateContext->PSSetShader(m_PixelShader, nullptr, 0);
		pd3dImmediateContext->PSSetShaderResources(0, 1, &m_Texture);
		pd3dImmediateContext->PSSetSamplers(0, 1, &m_SamplerState);

		UINT stride = sizeof(LamVertex);
		UINT offset = 0;
		pd3dImmediateContext->IASetVertexBuffers(0, 1, &m_VB, &stride, &offset);

		//if I have an index buffer draw using that
		if (m_IB)
		{
			pd3dImmediateContext->IASetIndexBuffer(m_IB, DXGI_FORMAT_R16_UINT, 0);
			pd3dImmediateContext->DrawIndexed(3 * m_numVerts, 0, 0);
		}
		else //just draw with a vertex buffer
		{
			pd3dImmediateContext->Draw(m_numVerts, 0);
		}

	}
}