#include "GameObject2D.h"
#include "gamedata.h"
#include "DrawData2D.h"
#include "DDSTextureLoader.h"
#include "helper.h"

using namespace DirectX;

GameObject2D::GameObject2D(string _fileName, ID3D11Device* _GD) :m_pTextureRV(nullptr)
{
	m_colour = Color(1.0f, 1.0f,1.0f, 1.0f);
	m_pos = Vector2::Zero;
	m_rotation = 0.0f;
	m_scale = Vector2::One;

	string fullfilename =
#if DEBUG
		"../Debug/"
#else
		"../Release/"
#endif
		+ _fileName + ".dds";
	HRESULT hr = CreateDDSTextureFromFile(_GD, Helper::charToWChar(fullfilename.c_str()), nullptr, &m_pTextureRV);

	//this nasty thing is required to find out the size of this image!
	ID3D11Resource *pResource;
	D3D11_TEXTURE2D_DESC Desc;
	m_pTextureRV->GetResource(&pResource);
	((ID3D11Texture2D *)pResource)->GetDesc(&Desc);
	
	m_origin = 0.5f*Vector2(Desc.Width, Desc.Height);//around which rotation and scaing is done
 
}

GameObject2D::~GameObject2D()
{
	if (m_pTextureRV) m_pTextureRV->Release();
}

void GameObject2D::tick(GameData* GD)
{
	m_rotation += GD->dt;
}

void GameObject2D::draw(DrawData2D* _DD)
{
	//nullptr can be changed to a RECT* to define what area of this image to grab
	//you can also add an extra value at the end to define layer depth
	_DD->m_Sprites->Draw(m_pTextureRV, m_pos, nullptr, m_colour, m_rotation, m_origin, m_scale, SpriteEffects_None);
}