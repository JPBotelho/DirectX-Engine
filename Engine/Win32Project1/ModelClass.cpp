#include "stdafx.h"
#include "ModelClass.h"
#include "LogManager.h"
#include "OBJ_Loader.h"

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
	if (FAILED(action))
	{
		logger->Append("Buffers didn't load.");
	}

	action = InitializeIndexBuffer(dev, devcon, logger);
	if (FAILED(action))
	{
		logger->Append("Buffers didn't load.");
	}
	return action;
}

HRESULT ModelClass::InitializeVertexBuffer(ID3D11Device* dev, ID3D11DeviceContext* devcon, LogManager* logger)
{
	struct Vertex
	{
		D3DXVECTOR3 position, normal;
		D3DXVECTOR2 uv;
	};
	objl::Loader Loader;

	bool loadout = Loader.LoadFile("icos.obj");

	if (!loadout)
		return STG_E_FILENOTFOUND;

	objl::Mesh curMesh = Loader.LoadedMeshes[0];

	Vertex vertices[60];
	for (int i = 0; i < curMesh.Vertices.size(); i++)
	{
		vertices[i].position = D3DXVECTOR3(curMesh.Vertices[i].Position.X, curMesh.Vertices[i].Position.Y, curMesh.Vertices[i].Position.Z);
		vertices[i].normal = D3DXVECTOR3(curMesh.Vertices[i].Normal.X, curMesh.Vertices[i].Normal.Y, curMesh.Vertices[i].Normal.Z);
		vertices[i].uv = D3DXVECTOR2(curMesh.Vertices[i].TextureCoordinate.X, curMesh.Vertices[i].TextureCoordinate.Y);
	}

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
	bd.ByteWidth = sizeof(vertices) * curMesh.Vertices.size();             // size is the VERTEX struct * 3
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
	memcpy(ms.pData, vertices, sizeof(vertices));                // copy the data
	devcon->Unmap(vertBuffer, NULL);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	devcon->IASetVertexBuffers(0, 1, &vertBuffer, &stride, &offset);
	return action;
}

HRESULT ModelClass::InitializeIndexBuffer(ID3D11Device* dev, ID3D11DeviceContext* devcon, LogManager* logger)
{
	objl::Loader Loader;

	bool loadout = Loader.LoadFile("icos.obj");

	if (!loadout)
		return STG_E_FILENOTFOUND;

	objl::Mesh curMesh = Loader.LoadedMeshes[0];

	unsigned int indices[60];
	
	for (int i = 0; i < curMesh.Indices.size(); i++)
	{
		indices[i] = curMesh.Indices[i];
	}

	ID3D11Buffer *indexBuffer = NULL;
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));

	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(unsigned int) * 36;
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	// Define the resource data.
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = indices;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;

	// Create the buffer with the device.
	HRESULT action = dev->CreateBuffer(&bufferDesc, &InitData, &indexBuffer);
	if (FAILED(action))
	{
		logger->Append("Index buffer failed");
		return action;
	}

	// Set the buffer.
	devcon->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	//delete[] indices;
}