#ifndef _GAME_H_
#define _GAME_H_
//Basic Game Manager Class
#include <windows.h>
#include "Effects.h"
//#include "DXUT.h"
//#include "SDKmisc.h"
#include <list>
#include "MyEffectsFactory.h"
#include "CommonStates.h"
#include <dinput.h> 

using namespace std;

using namespace DirectX;

class GameObject;
class GameObject2D;
class Camera;
class Light;
class TPSCamera;
struct GameData;
struct DrawData;
struct DrawData2D;

class Game
{
public:

	Game(ID3D11Device* pd3dDevice, HINSTANCE hInstance);
	~Game();

	bool update(); //tick the game state

	void render(ID3D11DeviceContext* pd3dImmediateContext); //render the current game state

protected:
	bool ReadKeyboard();
	bool ReadMouse();

	DWORD m_playTime; //amount of time since the game started

	Camera* m_cam; //principle camera
	TPSCamera* m_TPSCam; // TPS camera

	Light* m_Light; //base light

	list<GameObject *> m_GameObjects; //data structure storing all GameOBjects of this Game
	list<GameObject2D *> m_GameObject2Ds;//ditto 2D objects

	//required for the CMO model rendering system
	MyEffectFactory* m_myEF; 
	CommonStates* m_States;

	//input stuff
	IDirectInput8*			m_pDirectInput;
	IDirectInputDevice8*	m_pKeyboard;
	unsigned char			m_keyboardState[256];
	unsigned char			m_prevKeyboardState[256];
	IDirectInputDevice8*	m_pMouse;
	DIMOUSESTATE			m_mouse_state;

	//my GameData
	GameData* m_GD;
	DrawData* m_DD;
	DrawData2D* m_DD2D;
	
};


#endif
