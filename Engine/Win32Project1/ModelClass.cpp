#include "stdafx.h"
#include "ModelClass.h"
#include "LogManager.h"


ID3D11Buffer *vertBuffer;

ModelClass::ModelClass()
{
}


ModelClass::~ModelClass()
{
}

HRESULT ModelClass::InitializeBuffers(ID3D11Device* dev, ID3D11DeviceContext* devcon, LogManager* logger)
{
	HRESULT action;
	action = InitializeVertexBuffer(dev, devcon, logger);
	return action;
}

HRESULT ModelClass::InitializeVertexBuffer(ID3D11Device* dev, ID3D11DeviceContext* devcon, LogManager* logger)
{
	Vertex triVerts[] =
	{
		{ D3DXVECTOR3(-1, 1, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f) },
		{ D3DXVECTOR3(1, 1, 0.0f), D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f) },
		{ D3DXVECTOR3(-1, -1, 0.0f), D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f) },
		{ D3DXVECTOR3(1, -1, 0.0f), D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f) }
	};

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
	bd.ByteWidth = sizeof(Vertex) * 4;             // size is the VERTEX struct * 3
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer

	HRESULT action = dev->CreateBuffer(&bd, NULL, &vertBuffer);       // create the buffer

	if (FAILED(action))
	{
		logger->Append("\nERROR: Creating vertex buffer failed.");
		return action;
	}

	D3D11_MAPPED_SUBRESOURCE ms;
	devcon->Map(vertBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);   // map the buffer
	memcpy(ms.pData, triVerts, sizeof(triVerts));                // copy the data
	devcon->Unmap(vertBuffer, NULL);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	devcon->IASetVertexBuffers(0, 1, &vertBuffer, &stride, &offset);
	//devcon->IASetVertexBuffers(0, 1, &vertBuffer, sizeof(Vertex), 0);
	// unmap the buffer
	return action;
}