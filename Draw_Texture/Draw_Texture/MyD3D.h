//////////////////////////////////////////////////////////////////////////////////////////////////
// 
// File: MyD3D.h
// 
// Author: Song Wei (C) All Rights Reserved
//
// System: 3.20 GHz Intel® Core™ Quad CPU, a GeForce GT 770 graphics card, 4 GB RAM, Windows 7.
//
// Desc: This is a D3D initialization header, which provides basic framework for common applications.
//          
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __MyD3DH__
#define __MyD3DH__

#include "D3DUT.h"
#include "Object_Texture.h"
#include "Xfile.h"
#include "XFileModel.h"
#include "BoundingSphere.h"
#include "camera.h"

class MyD3D
{
public:
	MyD3D();
	~MyD3D();

	bool CreateDevice(HINSTANCE hInstance, int _width, int _height);
	virtual bool Initialize();
	virtual void FrameMove(float timeDelta);
	virtual bool Render(float timeDelta);
	void Release();

	IDirect3DDevice9*  getDevice();
	
protected:


private:
	HWND				hwnd;     //window handle
	IDirect3DDevice9*	p_Device;
	int					d_width;
	int					d_height;

	Box_Texture	        obj_texture_box[2];
	Plane_Texture       obj_texture_plane;
	Xfile				_xfile;
	Xfile				_xfile1;

	D3DXVECTOR3			v_pos_plane1;
	D3DXMATRIX			m_plane1;

	D3DXVECTOR3			v_pos_plane2;
	D3DXMATRIX			m_plane2;

	D3DXVECTOR3			v_pos_plane3;
	D3DXMATRIX			m_plane3;

	D3DXVECTOR3			v_pos_plane[10];
	D3DXMATRIX			m_plane[10];

	D3DXVECTOR3			v_position_xfile;
	D3DXMATRIX			v_xfile_1;

	D3DXMATRIX			_m_pos_plane[10];//往下掉
	D3DXMATRIX			_m_scale_plane[10];




	BoundingSphere*      Bounding_Plane1;
	BoundingSphere*      Bounding_Plane2;
	BoundingSphere*      Bounding_Plane3;

	BoundingSphere*      Bounding_Plane[10];

	Camera				TheCamera;
	XFileModel*			model_plane;
	XFileModel*			model_plane1[10];

	int					touch_disappear;
	bool				touch;

	int					touch_disappear1[10];//修改货物状态
	bool				touch1[10];
	int					touch_alive1[10];

	int					count;
	int					type[4];
	int					type_i;
};

#endif // __MyD3DH__