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
	objl::Loader Loader;

	bool loadout = Loader.LoadFile("icos2.obj");

	if (!loadout)
		return STG_E_FILENOTFOUND;

	objl::Mesh curMesh = Loader.LoadedMeshes[0];

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
	bd.ByteWidth = curMesh.Vertices.size() * sizeof(curMesh.Vertices[0]);             // size is the VERTEX struct * 3
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
	memcpy(ms.pData, curMesh.Vertices.data(), curMesh.Vertices.size() * sizeof(curMesh.Vertices[0]));                // copy the data
	devcon->Unmap(vertBuffer, NULL);

	UINT stride = sizeof(curMesh.Vertices[0]);
	UINT offset = 0;
	devcon->IASetVertexBuffers(0, 1, &vertBuffer, &stride, &offset);
	return action;
}

HRESULT ModelClass::InitializeIndexBuffer(ID3D11Device* dev, ID3D11DeviceContext* devcon, LogManager* logger)
{
	objl::Loader Loader;

	bool loadout = Loader.LoadFile("icos2.obj");

	if (!loadout)
		return STG_E_FILENOTFOUND;

	objl::Mesh curMesh = Loader.LoadedMeshes[0];

	ID3D11Buffer *indexBuffer = NULL;
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));

	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(unsigned int) * curMesh.Indices.size();
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	// Define the resource data.
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = curMesh.Indices.data();
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