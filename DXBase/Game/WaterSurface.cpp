#include "WaterSurface.h"
#include <dinput.h>
#include "gamedata.h"

/*WaterSurface::WaterSurface()
{
waveScale = 0;
time = 0;
Amp = 0.5f;
freq = 0.0f;
}*/

void WaterSurface::init(int _size, ID3D11Device* GD)
{
	m_size = _size;

	//calculate number of vertices and primatives
	numVerts = 6 * (m_size - 1) * (m_size - 1);
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

	//in each loop create the two triangles for the matching sub-square on each of the six faces
	int vert = 0;
	for (int i = 0; i<(m_size - 1); i++)
	{
		for (int j = 0; j<(m_size - 1); j++)
		{
			//top
			m_vertices[vert].Color = Color(0.0f, 0.0f, 1.0f, opactity);
			m_vertices[vert++].Pos = Vector3((float)i, startHeight * (float)(m_size - 1), (float)j);
			m_vertices[vert].Color = Color(0.0f, 0.0f, 1.0f, opactity);
			m_vertices[vert++].Pos = Vector3((float)i, startHeight * (float)(m_size - 1), (float)(j + 1));
			m_vertices[vert].Color = Color(0.0f, 0.0f, 1.0f, opactity);
			m_vertices[vert++].Pos = Vector3((float)(i + 1), startHeight * (float)(m_size - 1), (float)j);

			m_vertices[vert].Color = Color(0.0f, 0.0f, 1.0f, opactity);
			m_vertices[vert++].Pos = Vector3((float)(i + 1), startHeight * (float)(m_size - 1), (float)j);
			m_vertices[vert].Color = Color(0.0f, 0.0f, 1.0f, opactity);
			m_vertices[vert++].Pos = Vector3((float)i, startHeight * (float)(m_size - 1), (float)(j + 1));
			m_vertices[vert].Color = Color(0.0f, 0.0f, 1.0f, opactity);
			m_vertices[vert++].Pos = Vector3((float)(i + 1), startHeight  * (float)(m_size - 1), (float)(j + 1));
		}
	}

	//carry out some kind of transform on these vertices's to make this object more interesting
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

	//Do an initial transform
	for (int i = 0; i < m_numPrims * 3; i++)
	{
		if (i >((m_numPrims * 3) / 8));
		{
			float sinX = Amp*(sin((freq*(2 * XM_PI)* time + m_vertices[i].Pos.x + (m_vertices[i].Pos.x*waveScale))));
			float sinZ = Amp*(sin((freq*(2 * XM_PI)* time + m_vertices[i].Pos.z + (m_vertices[i].Pos.z*waveScale))));
			m_vertices[i].Pos.y = sinX * sinZ;
		}
	}

	m_ID3D11Device = GD;

	previousPos = new float[m_size * m_size];
	currentPos = new float[m_size * m_size];

	// sets the values of the array to 0
	memset(previousPos, 0, sizeof(float)*m_size*m_size);
	memset(currentPos, 0, sizeof(float)*m_size*m_size);

	BuildIB(GD, indices);
	//BuildVB(GD, numVerts, m_vertices);
	BuildDVB(GD, numVerts, m_vertices);
}

void WaterSurface::Draw(DrawData* _DD)
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

void WaterSurface::Tick(GameData* _GD)
{
	Transform(_GD);


	HRESULT hr;
	//Setup Raster State
	D3D11_RASTERIZER_DESC rasterDesc;
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;

	if (fillMode)
		rasterDesc.FillMode = D3D11_FILL_SOLID;
	else
		rasterDesc.FillMode = D3D11_FILL_WIREFRAME;

	rasterDesc.FrontCounterClockwise = true;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// Create the rasterizer state from the description we just filled out.
	hr = m_ID3D11Device->CreateRasterizerState(&rasterDesc, &m_pRasterState);

	VBGO::Tick(_GD);
}

void WaterSurface::Transform(GameData* _GD)
{
	time += _GD->dt *1.0f;
	input(_GD);

	float dropoff = 0.00001f;

	switch (waveType)
	{
	case 0:
		for (int i = 0; i < m_numPrims * 3; i++)
		{
			if (i >((m_numPrims * 3) / 8));
			{
				float sinX = Amp*(sin((freq*(2 * XM_PI)* time + m_vertices[i].Pos.x + (m_vertices[i].Pos.x*waveScale))));
				float sinZ = Amp*(sin((freq*(2 * XM_PI)* time + m_vertices[i].Pos.z + (m_vertices[i].Pos.z*waveScale))));
				m_vertices[i].Pos.y = sinX * sinZ;
			}
		}
		break;
	case 1:

		if (rain)
		{
			if (rand() % rainProb == 1) 
			currentPos[getVertPos(rand() % m_size, rand() % m_size)] += 5.0f;
		}
		for (int i = 0; i < m_size; i++)
		{
			for (int j = 0; j < m_size; j++)
			{
				int p = getVertPos(i, j);//Current vertice to edit in the loop
				currentPos[p] = (2.0f- damp/100)*previousPos[p] - (1.0f - damp/100) * currentPos[p] + ((-2.0f * previousPos[p]) * (0.01*0.01));
				
				if (ripple)
				{
					currentPos[getVertPos(m_size / 2, m_size / 2)] += 250.0f;
					ripple = false;
				}

				diff = diffMultiplier * (previousPos[getVertPos(i, j-1)] + previousPos[getVertPos(i, j+1)] + previousPos[getVertPos(i+1, j)] + previousPos[getVertPos(i-1, j)] - 4.0f * previousPos[p]);
				currentPos[p] += diff*0.01f;
			}
		}

		for (int i = 0; i < m_numVertices; i++)
		{
			m_vertices[i].Pos.y = currentPos[getVertPos(m_vertices[i].Pos.x, m_vertices[i].Pos.z)];
			float difference = currentPos[getVertPos(m_vertices[i].Pos.x, m_vertices[i].Pos.z)] - previousPos[getVertPos(m_vertices[i].Pos.x, m_vertices[i].Pos.z)];
			
			if (difference > ColourTheshold)
			{
				m_vertices[i].Color += Color(0.25f, 0.25f, 0.25f, opactity);
			}
		/*	else
			{
				m_vertices[i].Color -= Color(0.1f, 0.1f, 0.1f, 0.0f);
				if (m_vertices[i].Color > Color(0.0f, 0.0f, 1.0f, opactity))
					m_vertices[i].Color = Color(0.0f, 0.0f, 1.0f, opactity);
			}*/
			else
				m_vertices[i].Color = Color(0.0f, 0.0f, 1.0f, opactity);


				int limit = 100;
				if (m_vertices[i].Pos.y > limit)
				m_vertices[i].Pos.y = limit;
				else if (m_vertices[i].Pos.y < -limit)
				m_vertices[i].Pos.y = -limit;
		}

		//////////////////////////////////////////////////////////////////////////
		//////////////////Make ripples from the boat position/////////////////////
		//////////////////////////////////////////////////////////////////////////
		boatPos = boat->GetPos();
		//This offsets the position we get so (0,0) is the corner of our water plane.
		//Our other corner is at (380, 380).
		boatPos += Vector3(190.0f, 0.0f, 190.0f);
		if (boatPos != prevBoatPos)
		{
			currentPos[getVertPos(boatPos.x / 3.8, boatPos.z / 3.8)] -= 5.0f;
			prevBoatPos = boatPos;
		}
		temp = currentPos;
		currentPos = previousPos;
		previousPos = temp;
		
		break;
	default:
		break;
	}
}

int WaterSurface::getVertPos(int x, int y)
{
	//i/x is the largest number, it will be our Y axis
	//j/y is our x axis
	//int temp = (x*(m_size - 1) * 2) + y;
	/*if (x < 0)
		x = 1;
	else if (x * m_size)
		x = m_size-1;
	if (y < 0)
		y = 1;
	else if (y > m_size)
		y = m_size-1;*/

	if (x < 0)
		x = 0;
	else if (x >= m_size)
		x = m_size - 1;
	if (y < 0)
		y = 1;
	else if (y >= m_size)
		y = m_size - 1;

	int returnMe = (x*m_size) + y;
	return returnMe;
}

void WaterSurface::input(GameData* _GD)
{
	if (_GD->keyboard[DIK_LCONTROL] && !_GD->prevKeyboard[DIK_LCONTROL]) //Toggle wireframe mode.
		fillMode = !fillMode;

	if (_GD->keyboard[DIK_F] && !_GD->prevKeyboard[DIK_F])
	{
		if (!ripple)
			ripple = true;
	}
	
	if (_GD->keyboard[DIK_T] && !_GD->prevKeyboard[DIK_T])
	{
		if (!rain)
			rain = true;
		else
			rain = false;
	}


	if (_GD->keyboard[DIK_TAB] && !_GD->prevKeyboard[DIK_TAB])
	{
		waveType++;
		if (waveType < 1)
			waveType = 0;
	}
	
	if (_GD->keyboard[DIK_H] & 0x80)
	{
		rainProb++;
		if (rainProb > 250)
			rainProb = 250;
	}
	if (_GD->keyboard[DIK_G] & 0x80)
	{
		rainProb--;
		if (rainProb < 2)
			rainProb = 2;
	}

	if (_GD->keyboard[DIK_I] & 0x80)
	{
		Amp += _GD->dt *5.0f;
	}
	if (_GD->keyboard[DIK_K] & 0x80)
		Amp -= _GD->dt *5.0f;

	if ((_GD->keyboard[DIK_J])/* && !(_GD->prevKeyboard[DIK_J])*/)
		freq += _GD->dt *1.0f;

	if ((_GD->keyboard[DIK_L])/* && !(_GD->prevKeyboard[DIK_L])*/)
		freq -= _GD->dt *1.0f;

	if (_GD->keyboard[DIK_O] & 0x80)
		waveScale += _GD->dt *1.0f;

	if (_GD->keyboard[DIK_P] & 0x80)
		waveScale -= _GD->dt *1.0f;

	if (_GD->keyboard[DIK_N] & 0x80)
		diffMultiplier += _GD->dt *1.0f;

	if (_GD->keyboard[DIK_M] & 0x80)
	{
		diffMultiplier -= _GD->dt *1.0f;
		if (diffMultiplier < 0)
			diffMultiplier = 0;
	}

	if (_GD->keyboard[DIK_V] & 0x80)
		damp += _GD->dt *1.0f;

	if (_GD->keyboard[DIK_B] & 0x80)
		damp -= _GD->dt *1.0f;

	if (_GD->keyboard[DIK_N] & 0x80)
		diffMultiplier += _GD->dt *1.0f;

	if (_GD->keyboard[DIK_M] & 0x80)
		diffMultiplier -= _GD->dt *1.0f;

	if (_GD->keyboard[DIK_Z] & 0x80)
		 ColourTheshold -= _GD->dt *1.0f;

	if (_GD->keyboard[DIK_X] & 0x80)
		ColourTheshold += _GD->dt *1.0f;

	if (_GD->keyboard[DIK_R] & 0x80)
	{
		for (int i = 0; i < m_numVertices; i++)
		{
			//m_vertices[i].Pos.y = 0.0f;
			memset(previousPos, 0, sizeof(float)*m_size*m_size);
			memset(currentPos, 0, sizeof(float)*m_size*m_size);
			m_vertices[i].Color = Color(0.0f, 0.0f, 1.0f, opactity);
		}
	}
}
