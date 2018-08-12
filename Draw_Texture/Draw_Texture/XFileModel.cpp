#include "XFileModel.h"

XFileModel::XFileModel(IDirect3DDevice9* _p_Device)
{
	p_Device = _p_Device;
}

XFileModel::~XFileModel( )
{
	for(int i = 0; i < d_num_Mtrls; i++)
	{
		model_Textures[i]->Release();
	}
	model_Mesh->Release();
}

ID3DXMesh* XFileModel::getModelMesh()
{
	return (model_Mesh);
}

HRESULT XFileModel::LoadXFileModelFromFile( char* XFilename )
{
	ID3DXBuffer* _adj_Buffer  = 0;
	ID3DXBuffer* _mtrl_Buffer = 0;

	HRESULT _hr = 0;

	//Load mesh from file
	_hr = D3DXLoadMeshFromX( 
		XFilename,
		D3DXMESH_MANAGED,
		p_Device,
		&_adj_Buffer,
		&_mtrl_Buffer,
		0,
		&d_num_Mtrls,
		&model_Mesh);


	if(FAILED(_hr))
	{
		::MessageBox(0, "Load Mesh - Fail", 0, 0);
		return false;
	}

	//Load materials and textures
	if( _mtrl_Buffer != 0 && d_num_Mtrls != 0 )
	{
		model_Mtrls = new D3DMATERIAL9[d_num_Mtrls];
		model_Textures = new LPDIRECT3DTEXTURE9[d_num_Mtrls];
		//get the starting address of _mtrl_Buffer
		D3DXMATERIAL* _mtrls = (D3DXMATERIAL*)_mtrl_Buffer->GetBufferPointer();

		for(int i = 0; i < d_num_Mtrls; i++)
		{


			//set ambient light
			_mtrls[i].MatD3D.Ambient = _mtrls[i].MatD3D.Diffuse;

			// save material
			//model_Mtrls.push_back(_mtrls[i].MatD3D);
			model_Mtrls[i] = _mtrls[i].MatD3D;
			model_Mtrls[i].Ambient = model_Mtrls[i].Diffuse;

			if( _mtrls[i].pTextureFilename != NULL )
			{
				// save the texture
				//IDirect3DTexture9* _tex = NULL;
				D3DXCreateTextureFromFileA(p_Device, _mtrls[i].pTextureFilename, &model_Textures[i]);

				// save the loaded texture
				//model_Textures.push_back(_tex);
			}
			else
			{
				// this subset has no texture
				model_Textures[i] = NULL;
			}
		}
	}
	_mtrl_Buffer->Release(); // done w/ buffer

	//ÓÅ»¯Íø¸ñ
	_hr = model_Mesh->OptimizeInplace(		
		D3DXMESHOPT_ATTRSORT |
		D3DXMESHOPT_COMPACT  |
		D3DXMESHOPT_VERTEXCACHE,
		(DWORD*)_adj_Buffer->GetBufferPointer(),
		0, 0, 0);

	_adj_Buffer->Release() ;// done w/ buffer

	if(FAILED(_hr))
	{
		::MessageBox(0, "OptimizeInplace() - FAILED", 0, 0);
		return false;
	}
	return S_OK;  
}

HRESULT XFileModel::RenderXFlieModel()
{
	for(int i = 0; i < d_num_Mtrls; i++)
	{
		p_Device->SetMaterial( &model_Mtrls[i] );
		p_Device->SetTexture(0, model_Textures[i]);
		model_Mesh->DrawSubset(i);
	}	
	return S_OK;  
}

HRESULT XFileModel::Release()
{
	model_Mesh->Release();

	for(int i = 0; i < d_num_Mtrls; i++)
	{
		model_Textures[i]->Release();
	}
	return S_OK; 
}

