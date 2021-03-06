/*
 * Handles creating vertex and index buffers from a given obj file. 
 * .obj file is currently hardcoded in each function
 */

#pragma once
#include <d3d11.h>
#include <d3dx10math.h>
#include "LogManager.h"

class ModelClass
{
	private:
		struct Vertex
		{
			D3DXVECTOR3 position;
			D3DXVECTOR4 color;
		};

	public:
		ModelClass();
		~ModelClass();
		HRESULT InitializeBuffers(ID3D11Device*, ID3D11DeviceContext*, LogManager*);
		HRESULT InitializeVertexBuffer(ID3D11Device*, ID3D11DeviceContext*, LogManager*);
		HRESULT InitializeIndexBuffer(ID3D11Device*, ID3D11DeviceContext*, LogManager*);		
};

