#pragma once
#ifndef OBJECTCLASS_H
#define OBJECTCLASS_H

#include <d3d11.h>
#include <directxmath.h>
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#include <dinput.h>

#include "directInputClass.h"
#include "collisionClass.h"

#include "importerClass.h"

using namespace DirectX;

class objectClass 
{
	public:
		objectClass();
		objectClass(const objectClass& other);
		~objectClass();

		bool initlialize(ID3D11Device* device, const char* filename);
		void shutdown();
		void setVtrxIndexBufferToPipline(ID3D11DeviceContext* deviceCon, int meshID);

		int getVertexCount(int meshID);
		int getMeshCount();

		/*void setBoundingBoxMinStart(XMVECTOR other);
		void setBoundingBoxMaxStart(XMVECTOR other);*/

		XMVECTOR getBoundingBoxMin();
		XMVECTOR getBoundingBoxMax();
		
		XMVECTOR getBoundingBoxMin(int meshID);
		XMVECTOR getBoundingBoxMax(int meshID);

		collisionClass* getCollisionClass();

		void updatePosition(XMMATRIX mat);
		XMVECTOR getPosition();

		void getWorldMatrix(XMMATRIX& other);
		void setWorldMatrix(XMMATRIX& other);

		int getType();
		void setType(int type);

		void setMaterialName(std::string name);
		std::string getMaterialName();

	private:

		Importer mesh;
		collisionClass* collision;
		ID3D11Buffer** vertexBuffer;
		int type;

		bool initializeBuffer(ID3D11Device* device, int count, int meshID);
		void shutdownBuffer();
		void setBuffers(ID3D11DeviceContext* deviceCon, int meshID);
		

		XMMATRIX objWorld;
		std::string matName;
};

#endif
