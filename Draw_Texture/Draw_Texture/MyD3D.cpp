//////////////////////////////////////////////////////////////////////////////////////////////////
// 
// File: MyD3D.cpp
// 
// Author: Song Wei (C) All Rights Reserved
//
// System: 3.20 GHz Intel® Core™ Quad CPU, a GeForce GT 770 graphics card, 4 GB RAM, Windows 7.
//
// Desc: Provides utility functions for simplifying common tasks.
//          
//////////////////////////////////////////////////////////////////////////////////////////////////


#include "MyD3D.h"
#include "D3DUT.h"
#include "psystem.h"
#include "camera.h"
#include <cstdlib>
#include <ctime>
#include<list>



psys::PSystem* Gun = 0;
MyD3D::MyD3D() 
{
	int i;
	touch_disappear=0;
	touch=false;
	count=-1;
	type_i=0;

	for(i=0;i<10;i++)
	{
		touch1[i]=false;
		touch_disappear1[i]=0;
		touch_alive1[i]=1;//1为生存

		//type[i]=-1;
	}
	for(i=0;i<3;i++)
	{
		type[i]=i;
	}

}

MyD3D::~MyD3D() 
{

}


bool MyD3D::CreateDevice(HINSTANCE hInstance, int _width, int _height)
{
	d_width = _width;
	d_height = _height;

	if(!InitWindow(hInstance, _width, _height,  true))
	{
		MessageBox(0, "InitD3D() - FAILED", 0, 0);
		return 0;
	}
	if(!InitD3D(hInstance, _width, _height,  true, D3DDEVTYPE_HAL, &p_Device))
	{
		MessageBox(0, "InitD3D() - FAILED", 0, 0);
		return 0;
	}

	return true;
}

bool MyD3D::Initialize()
{


	// seed random number generator.
	srand((unsigned int)time(0));


	//
	// Create a basic scene.
	//

	DrawBasicScene(p_Device, 1.0f);

	//
	// Set projection matrix.
	//

	// Initialize VirtualCamera.

	/*D3DXVECTOR3 position(0.0f, 0.0f, -10.0f);
	D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
	D3DXMATRIX V;
	D3DXMatrixLookAtLH(&V, &position, &target, &up);
	p_Device->SetTransform(D3DTS_VIEW, &V);*/

	// Set projection matrix.

	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(
		&proj,
		D3DX_PI * 0.5f, // 90 - degree
		(float)d_width / (float)d_height,
		1.0f,
		1000.0f);
	p_Device->SetTransform(D3DTS_PROJECTION, &proj);


	//create objects
	obj_texture_box[0].CreateBuffer(p_Device);
	obj_texture_box[0].SetScale(D3DXVECTOR3(1.5f, 1.5f, 1.5f));
	obj_texture_box[0].SetTranslation(D3DXVECTOR3(2.0f, 0.0f, 0.0f));

	obj_texture_box[1].CreateBuffer(p_Device);
	obj_texture_box[1].SetScale(D3DXVECTOR3(1.5f, 1.5f, 1.5f));
	obj_texture_box[1].SetTranslation(D3DXVECTOR3(2.0f, 2.0f, 0.0f));

	obj_texture_plane.CreateBuffer(p_Device);
	obj_texture_plane.SetScale(D3DXVECTOR3(3.0f, 3.0f, 3.0f));
	obj_texture_plane.SetTranslation(D3DXVECTOR3(-2.0f, 0.0f, 0.0f));


	_xfile1.CreateBuffer(p_Device, "bigship1.x");
	_xfile1.SetScale(D3DXVECTOR3(0.1f, 0.1f, 0.1f));
	_xfile1.SetTranslation(D3DXVECTOR3(0.0f, 3.0f, 0.0f));



	_xfile.CreateBuffer(p_Device, "PLATE.x");
	_xfile.SetScale(D3DXVECTOR3(0.1f, 0.1f, 0.1f));
	_xfile.SetTranslation(D3DXVECTOR3(0.0f, 30.0f, 0.0f));


	//	static D3DXVECTOR3 XfilePos;

	//_xfile.GetPosistion(&XfilePos);
	//_xfile.SetTranslation( D3DXVECTOR3 (9.0f,9.0f,9.0f));





	Gun = new psys::ParticleGun(&_xfile1);
	Gun->init(p_Device, "flare_alpha.dds");



	model_plane = new XFileModel(p_Device);
	model_plane->LoadXFileModelFromFile("111.x");

	int i=0;
	for(i=0;i<10;i++)
	{
		if(i%3==0)
		{
			model_plane1[i]=new XFileModel(p_Device);
			model_plane1[i]->LoadXFileModelFromFile("brr.x");
		}
		if(i%3==1)
		{
			model_plane1[i]=new XFileModel(p_Device);
			model_plane1[i]->LoadXFileModelFromFile("fl.x");
		}
		if(i%3==2)
		{
			model_plane1[i]=new XFileModel(p_Device);
			model_plane1[i]->LoadXFileModelFromFile("ps.x");
		}

	}


	Bounding_Plane1 = new BoundingSphere(p_Device);
	Bounding_Plane1->CreatBoundingSphere(model_plane->getModelMesh());//计算包围盒

	Bounding_Plane2 = new BoundingSphere(p_Device);
	Bounding_Plane2->CreatBoundingSphere(model_plane->getModelMesh());//计算包围盒

	Bounding_Plane3 = new BoundingSphere(p_Device);
	Bounding_Plane3->CreatBoundingSphere(model_plane->getModelMesh());//计算包围盒


	for(i=0;i<10;i++)
	{

		Bounding_Plane[i] = new BoundingSphere(p_Device);
		Bounding_Plane[i]->CreatBoundingSphere(model_plane1[i]->getModelMesh());

	}

	v_pos_plane1 = D3DXVECTOR3(0.0f, 0.0f, -0.0f);//接住的东西
	v_pos_plane2 = D3DXVECTOR3(0.0f, 100.0f, 0.0f);//掉下来的东西
	v_pos_plane3 = D3DXVECTOR3(3.0f, 10.0f, 0.0f);//掉下来的东西

	for(i=0;i<10;i++)
	{
		if(i%3==0)
		{
			v_pos_plane[i] = D3DXVECTOR3(-120.0f+10.0f*i, 60.0f*i, -60.0f);
		}
		else if(i%3==1)
		{
			v_pos_plane[i] = D3DXVECTOR3(+50.0f+40.0f*i, i*50.0f, i+40.0f);
		}
		else if(i%3==2)
		{
			v_pos_plane[i] = D3DXVECTOR3(-50.0f+50.0f*i, i*20.0f, i*i-40.0f);
		}
		/*else if(i%4==3)
		{
			v_pos_plane[i] = D3DXVECTOR3(+10.0f+80.0f*i, i*-70.0f, i*i+30.0f);
		}*/



	}



	//
	// Create a basic scene.
	//

	//TheCamera.setCameraType(Camera.CameraType.LANDOBJECT);

	D3DXVECTOR3 dir(1.0f, -1.0f, 1.0f);
	D3DXCOLOR col(1.0f, 1.0f, 1.0f, 1.0f);
	D3DLIGHT9 light = InitDirectionalLight(&dir, &col);



	p_Device->SetLight(0, &light);
	p_Device->LightEnable(0, true);
	p_Device->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	p_Device->SetRenderState(D3DRS_SPECULARENABLE, true);



	return true;
}
//static bool touch=false;
static bool touch1[10];

void MyD3D::FrameMove(float timeDelta)
{
	if( ::GetAsyncKeyState('W') & 0x8000f )
		TheCamera.walk(4.0f * timeDelta);

	if( ::GetAsyncKeyState('S') & 0x8000f )
		TheCamera.walk(-4.0f * timeDelta);

	if( ::GetAsyncKeyState('A') & 0x8000f )
		TheCamera.strafe(-4.0f * timeDelta);

	if( ::GetAsyncKeyState('D') & 0x8000f )
		TheCamera.strafe(4.0f * timeDelta);

	//if( ::GetAsyncKeyState('R') & 0x8000f )
	//	TheCamera.fly(4.0f * timeDelta);

	//if( ::GetAsyncKeyState('F') & 0x8000f )
	//	TheCamera.fly(-4.0f * timeDelta);

	if( ::GetAsyncKeyState(VK_UP) & 0x8000f )
		TheCamera.pitch(1.0f * timeDelta);

	if( ::GetAsyncKeyState(VK_DOWN) & 0x8000f )
		TheCamera.pitch(-1.0f * timeDelta);

	if( ::GetAsyncKeyState(VK_LEFT) & 0x8000f )
		TheCamera.yaw(-1.0f * timeDelta);

	if( ::GetAsyncKeyState(VK_RIGHT) & 0x8000f )
		TheCamera.yaw(1.0f * timeDelta);

	if( ::GetAsyncKeyState('N') & 0x8000f )
		TheCamera.roll(1.0f * timeDelta);

	if( ::GetAsyncKeyState('M') & 0x8000f )
		TheCamera.roll(-1.0f * timeDelta);

	if( ::GetAsyncKeyState('I') & 0x8000f )
	{
		v_pos_plane1.y += 40.0f * timeDelta;
	}

	if( ::GetAsyncKeyState('K') & 0x8000f )
	{
		v_pos_plane1.y -= 40.0f * timeDelta;
	}

	if( ::GetAsyncKeyState('J') & 0x8000f )
	{
		v_pos_plane1.x -= 40.0f * timeDelta;
	}
	if( ::GetAsyncKeyState('L') & 0x8000f )
	{
		v_pos_plane1.x += 40.0f * timeDelta;
	}

	if( ::GetAsyncKeyState('O') & 0x8000f )
	{
		v_pos_plane1.z += 40.0f * timeDelta;
	}

	if( ::GetAsyncKeyState('P') & 0x8000f )
	{
		v_pos_plane1.z -= 30.0f * timeDelta;
	}
	//
	// Create Laser.
	//
	v_pos_plane2.y -= 5.0f * timeDelta;
	v_pos_plane3.y -= 2.0f * timeDelta;

	int i;
	int flag_pos=20;
	for(i=0;i<10;i++)
	{
		if(touch_disappear1[i]==0)
		{
			v_pos_plane[i].y-= 5.0f * timeDelta;//修改飞机的位置
				
		}
			
		else if(touch_disappear1[i]!=0)
			v_pos_plane[i].z+= 5.0f * timeDelta;
	}

	static int time=0;
	if(time%100==0)
	{
		Gun->addParticle();
		//time=time*timeDelta;
	}
	time++;


	D3DXMATRIX V;
	TheCamera.getViewMatrix(&V);
	p_Device->SetTransform(D3DTS_VIEW, &V);

	Gun->update(timeDelta); 


	D3DXMATRIX _m_pos_plane1;
	D3DXMATRIX _m_scale_plane1;
	Bounding_Plane1->setBoundingPos(v_pos_plane1);
	D3DXMatrixTranslation(&_m_pos_plane1, v_pos_plane1.x, v_pos_plane1.y, v_pos_plane1.z);
	D3DXMatrixScaling(&_m_scale_plane1,0.05f,0.05f,0.05f);
	m_plane1 = _m_pos_plane1*_m_scale_plane1;//整体再赋值

	D3DXMATRIX _m_pos_plane2;//往下掉
	D3DXMATRIX _m_scale_plane2;
	Bounding_Plane2->setBoundingPos(v_pos_plane2);
	D3DXMatrixTranslation(&_m_pos_plane2, v_pos_plane2.x, v_pos_plane2.y, v_pos_plane2.z);
	D3DXMatrixScaling(&_m_scale_plane2,0.05f,0.05f,0.05f);
	m_plane2 = _m_pos_plane2*_m_scale_plane2;

	D3DXMATRIX _m_pos_plane3;//往下掉
	D3DXMATRIX _m_scale_plane3;
	Bounding_Plane3->setBoundingPos(v_pos_plane3);
	D3DXMatrixTranslation(&_m_pos_plane3, v_pos_plane3.x, v_pos_plane3.y, v_pos_plane3.z);
	D3DXMatrixScaling(&_m_scale_plane3,0.05f,0.05f,0.05f);
	m_plane3 = _m_pos_plane3*_m_scale_plane3;

	for(i=0; i<10; i++)
	{

		Bounding_Plane[i]->setBoundingPos(v_pos_plane[i]);
		D3DXMatrixTranslation(&_m_pos_plane[i], v_pos_plane[i].x, v_pos_plane[i].y, v_pos_plane[i].z);
		D3DXMatrixScaling(&_m_scale_plane[i],0.05f,0.05f,0.05f);
		m_plane[i] = _m_pos_plane[i]*_m_scale_plane[i];
	}



	touch=Bounding_Plane1->computeBounding(Bounding_Plane2);//计算碰撞盒子
	for(i=0;i<10;i++)
	{
		touch1[i]=Bounding_Plane1->computeBounding(Bounding_Plane[i]);
	}


	TheCamera.getViewMatrix(&V);
	p_Device->SetTransform(D3DTS_VIEW, &V);

	static D3DXVECTOR3 _v_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	if (_v_rot.x > D3DX_PI * 2)
	{
		_v_rot.x = 0.0f;
	}
	if (_v_rot.y > D3DX_PI * 2)
	{
		_v_rot.y = 0.0f;
	}
	obj_texture_box[0].SetRotation(_v_rot);
	obj_texture_box[1].SetRotation(_v_rot);

	_v_rot.x += 0.5f * timeDelta;
	_v_rot.y += 0.5f * timeDelta;

}


bool MyD3D::Render(float timeDelta)
{
	
	if(count<3)
	{
		if(count==-1)
		{
			MessageBox(0, "欢迎来到拯救adobe游戏，请把里面所有类型的图标拯救", "拯救adobe!", 0);
			count++;
		}
		
	
	if( p_Device ) 
	{

		static int p = 1; 
		static int flag = 1;
		//static float timedelta_new=0.0f;


		D3DXMATRIX POS;
		D3DXVECTOR3 cameraPos;
		static D3DXVECTOR3 XfilePos;

		_xfile1.GetPosistion(XfilePos);

		//	XfilePos.y=3.0f;
		if( ::GetAsyncKeyState('P') & 0x8000f ){
			p = -p;
		}
		if(p==1){  //ziji
			//D3DXMatrixTranslation(&POS, 10.0f, XfilePos.y, 0);

			if(flag == 1){
				XfilePos.y -= timeDelta;
				if( XfilePos.y >= 5) {
					//	flag = -1;
				}
			}
			else if(flag == -1){
				XfilePos.y += timeDelta;

				if( XfilePos.y <= -5) {
					flag = 1;
				}
			}


			//timedelta_new=timedelta_new+0.00001f;
			//timedelta_new++;

			_xfile1.GetPosistion(XfilePos);
			//_xfile1.SetTranslation(XfilePos);
			_xfile1.SetTranslation(D3DXVECTOR3(0.0f, XfilePos.y, 0.0f));

			D3DXMATRIX World = POS;
			p_Device->SetTransform(D3DTS_WORLD, &World);
			//p_Device->SetTransform(D3DTS_WORLD, &World);


		}
		// Set back buffer - 0xffffffff (white)
		//Set each pixel on the depth buffer to a value of 1.0.
		p_Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0);

		p_Device->BeginScene();

		D3DXMATRIX I;
		D3DXMatrixIdentity(&I);
		p_Device->SetTransform(D3DTS_WORLD, &I);

		DrawBasicScene(p_Device, 1.0f);

		p_Device->SetTransform(D3DTS_WORLD, &I);
		Gun->render();

		/*  drawing 

		obj_texture_plane.Render(p_Device);
		p_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
		p_Device->SetSamplerState(0, D3DSAMP_MAXANISOTROPY, 10);
		for (int i = 0; i < 2; i ++)
		{
		obj_texture_box[i].Render(p_Device);
		}*/
		//
		//	_xfile.Render(p_Device);
		//	_xfile1.Render(p_Device);



		p_Device->SetTransform(D3DTS_WORLD, &m_plane1);
		model_plane->RenderXFlieModel();


		Bounding_Plane1->RenderBoundingSphere();


		p_Device->SetTransform(D3DTS_WORLD, &m_plane2);

		if(touch==false  && touch_disappear==0)
		{
			model_plane->RenderXFlieModel();
		}
		else
		{
			touch_disappear=1;
		}

		Bounding_Plane2->RenderBoundingSphere();



		/*p_Device->SetTransform(D3DTS_WORLD, &m_plane3);

		if(touch==false )
		{
		model_plane->RenderXFlieModel();
		}

		Bounding_Plane3->RenderBoundingSphere();*/

		int i;
		int j;
		int j_flag=0;
		for(i=0;i<10;i++)
		{

			p_Device->SetTransform(D3DTS_WORLD, &m_plane[i]);
			if(touch1[i]==false && touch_disappear1[i]==0)
			{
				model_plane1[i]->RenderXFlieModel();
				Bounding_Plane[i]->RenderBoundingSphere();

			}
			if(touch1[i]==true && touch_disappear1[i]==0)
			{
				touch_disappear1[i]=1;
				model_plane1[i]->RenderXFlieModel();
			//	Bounding_Plane[i]->RenderBoundingSphere();

			}

			else if(touch1[i]==true && touch_disappear1[i]==1)
			{
				model_plane1[i]->RenderXFlieModel();
			//	Bounding_Plane[i]->RenderBoundingSphere();

			}
			else if(touch1[i]==false && touch_disappear1[i]==1)
			{

				touch_disappear1[i]=2;
				model_plane1[i]->RenderXFlieModel();
				Bounding_Plane[i]->RenderBoundingSphere();

			}
			else if(touch1[i]==false && touch_disappear1[i]==2)
			{
				//touch_disappear1[i]=3;
				model_plane1[i]->RenderXFlieModel();
				//Bounding_Plane[i]->RenderBoundingSphere();
			}
			
			else if(touch1[i]==true && touch_disappear1[i]==2)
			{
				touch_disappear1[i]=3;

				j_flag=(i%3);
				for(j=0;j<3;j++)
				{

					if(type[j]==j_flag)
					{
						count++;
						
						type[j]=-1;
					}
				}
				/*if(j_flag!=1)
				{
					type[count]=i%3;
					count++;
				}
				
				j_flag=0;*/

			}
			else if(touch_disappear1[i]==3 )
			{

			}	

			
		}




		p_Device->EndScene();

		// Swap the back and front buffers.
		p_Device->Present(0, 0, 0, 0);

		return true;
	}
	}
	else if (count==3)
	{
		MessageBox(0, "你已经拯救了所有类型的文件！", "拯救adobe!", 0);
		count=2;
		
		
	
	}

}

void MyD3D::Release()
{
	Delete<psys::PSystem*>( Gun );
	DrawBasicScene(0, 1.0f);
}

IDirect3DDevice9*  MyD3D::getDevice()
{
	return p_Device;
}