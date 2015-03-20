#include "VBPlane.h"
#include <dinput.h>
#include "gamedata.h"

VBPlane::VBPlane()
{
	changeMe = 0;
	time = 0;
	Amp = 0.5f;
	freq = 0.0f;
}

void VBPlane::init(int _size, ID3D11Device* GD)
{
	m_size = _size;

	//calculate number of vertices and primatives
	numVerts = 6 * 6 * (m_size - 1) * (m_size - 1);
	m_numPrims = numVerts / 3;
	m_numVertices = numVerts;
	m_vertices = new myVertex[numVerts];
	WORD* indices = new WORD[numVerts];

	//as using the standard VB shader set the tex-coords somewhere safe
	for (int i = 0; i<numVerts; i++)
	{
		indices[i] = i;
		m_vertices[i].texCoord = Vector2::One;
	}
	
	float startHeight = 0.5f;

	//in each loop create the two traingles for the matching sub-square on each of the six faces
	int vert = 0;
	for (int i = -(m_size - 1) / 2; i<(m_size - 1) / 2; i++)
	{
		for (int j = -(m_size - 1) / 2; j<(m_size - 1) / 2; j++)
		{
			//top
			m_vertices[vert].Color = Color(0.0f, 0.0f, 1.0f, 1.0f);
			m_vertices[vert++].Pos = Vector3((float)i, startHeight * (float)(m_size - 1), (float)j);
			m_vertices[vert].Color = Color(0.0f, 0.0f, 1.0f, 1.0f);
			m_vertices[vert++].Pos = Vector3((float)i, startHeight * (float)(m_size - 1), (float)(j + 1));
			m_vertices[vert].Color = Color(0.0f, 0.0f, 1.0f, 1.0f);
			m_vertices[vert++].Pos = Vector3((float)(i + 1), startHeight * (float)(m_size - 1), (float)j);

			m_vertices[vert].Color = Color(0.0f, 0.0f, 1.0f, 1.0f);
			m_vertices[vert++].Pos = Vector3((float)(i + 1), startHeight * (float)(m_size - 1), (float)j);
			m_vertices[vert].Color = Color(0.0f, 0.0f, 1.0f, 1.0f);
			m_vertices[vert++].Pos = Vector3((float)i, startHeight * (float)(m_size - 1), (float)(j + 1));
			m_vertices[vert].Color = Color(0.0f, 0.0f, 1.0f, 1.0f);
			m_vertices[vert++].Pos = Vector3((float)(i + 1), startHeight  * (float)(m_size - 1), (float)(j + 1));
		}
	}

	//carry out some kind of transform on these vertices to make this object more interesting
	//calculate the normals for the basic lighting in the base shader
	for (int i = 0; i < m_numPrims; i++)
	{
		WORD V1 = 3 * i;
		WORD V2 = 3 * i + 1;
		WORD V3 = 3 * i + 2;

		//build normals
		Vector3 norm;
		Vector3 vec1 = m_vertices[V1].Pos - m_vertices[V2].Pos;
		Vector3 vec2 = m_vertices[V3].Pos - m_vertices[V2].Pos;
		norm = vec1.Cross(vec2);
		norm.Normalize();

		m_vertices[V1].Norm = norm;
		m_vertices[V2].Norm = norm;
		m_vertices[V3].Norm = norm;
	}

	BuildIB(GD, indices);
	//BuildVB(GD, numVerts, m_vertices);
	BuildDVB(GD, numVerts, m_vertices);
}

void VBPlane::Draw(DrawData* _DD)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	//Disable GPU access to the vertex buffer data.
	HRESULT hr = _DD->pd3dImmediateContext->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	myVertex* p_vertices = (myVertex*)mappedResource.pData;

	//Update the vertex buffer here.
	memcpy(p_vertices, (void*)m_vertices, sizeof(myVertex) * m_numVertices);
	p_vertices;

	//Reenable GPU access to the vertex buffer data.
	_DD->pd3dImmediateContext->Unmap(m_VertexBuffer, 0);

	VBGO::Draw(_DD);
}

void VBPlane::Tick(GameData* _GD)
{
	Transform(_GD);

	VBGO::Tick(_GD);
}

void VBPlane::Transform(GameData* _GD)
{	
	time += _GD->dt *1.0f;

	if (_GD->keyboard[DIK_I] & 0x80)
	{
		Amp += _GD->dt *5.0f;
	}
	if (_GD->keyboard[DIK_K] & 0x80)
	{
		Amp -= _GD->dt *5.0f;
	}

	if ((_GD->keyboard[DIK_J])/* && !(_GD->prevKeyboard[DIK_J])*/)
	{
		freq += _GD->dt *1.0f;
		//freq++;
	}
	if ((_GD->keyboard[DIK_L])/* && !(_GD->prevKeyboard[DIK_L])*/)
	{
		freq -= _GD->dt *1.0f;
		//freq--;
	}

	if (_GD->keyboard[DIK_O] & 0x80)
	{
		changeMe += _GD->dt *1.0f;
		//changeMe++;
		//changeMe = 5.0f;
	}
	if (_GD->keyboard[DIK_P] & 0x80)
	{
		changeMe += _GD->dt *1.0f;
		//changeMe--;
		//changeMe = 500.0f;
	}


	for (int i = 0; i < m_numPrims * 3; i++)
	{
		//Mate, add + (m_vertices[i].Pos.z*changeMe) to the end and get some wicked shit
		m_vertices[i].Pos.y = Amp*(sin((freq*(2 * XM_PI)* time + m_vertices[i].Pos.x + (m_vertices[i].Pos.z*changeMe))));
	}
	
}