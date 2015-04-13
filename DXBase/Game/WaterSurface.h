#ifndef _WATERSURFACE_H_
#define _WATERSURFACE_H_
#include "VBGO.h"
#include "vertex.h"
#include "drawdata.h"
#include "GameData.h"
#include <vector>
#include "Turret_base.h"

class Turret_Base;

class WaterSurface : public VBGO
{
public:
	WaterSurface(){};
	virtual ~WaterSurface(){ delete[] m_vertices; };

	ID3D11Device* m_ID3D11Device;

	//initialize the Vertex and Index buffers for the cube
	void init(int _size, ID3D11Device* _GD);
	void Draw(DrawData* _DD);
	void Tick(GameData* _GD);
	Turret_Base* boat;

	Vector3 boatPos, prevBoatPos;

	float waveScale = -0.85f;//THIS IS THE SCALE OF THE WAVE
	float time = 0.0f;
	float Amp = 2.0f;
	float freq = -0.5f;
	float opactity = 0.5f;
	int waveType = 0;
	float damp = 1.0f;
	float diffMultiplier = 1.0f;
	float ColourTheshold = 0.1f;
	bool rain = false;
	int rainProb = 10;

protected:
	//this is to allow custom versions of this which create the basic cube and then distort it
	//see Spiral, SpikedVB and Pillow
	void Transform(GameData* _GD);
	void input(GameData* _GD);

	int getVertPos(int x, int y);

	int numVerts;
	int m_size;

	bool fillMode;
	bool ripple = false;

	float diff;

	int m_numVertices;
	myVertex* m_vertices;

	float* previousPos;
	float* currentPos;
	float* temp;
};

#endif