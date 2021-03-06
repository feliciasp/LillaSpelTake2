#include "pickupClass.h"

pickupClass::pickupClass()
{
	obj = 0;
	transStart = XMMatrixIdentity();
	isDestroy = false;
	checkIfSetToObjHolder = false;
}

pickupClass::pickupClass(const pickupClass & other)
{
}

pickupClass::~pickupClass()
{
}

bool pickupClass::initlialize(ID3D11Device * device, const char* filename)
{
	bool result;
	obj = new objectClass;

	if (!obj)
	{
		MessageBox(NULL, L"Error create bakcground obj",
			L"Error", MB_OK | MB_ICONERROR);
		return false;
	}
	result = obj->initlialize(device, filename);
	if (!result)
	{
		MessageBox(NULL, L"Error init background obj",
			L"Error", MB_OK | MB_ICONERROR);
		return false;
	}

	setStartMat(-3.0f);
	return true;
}

void pickupClass::shutdown()
{
	if (obj)
	{
		obj->shutdown();
		delete obj;
		obj = 0;
	}
}

objectClass * pickupClass::getObj()
{
	return this->obj;
}

bool pickupClass::getIsDestry()
{
	return this->isDestroy;
}

void pickupClass::setIsDestroy(bool check)
{
	this->isDestroy = check;
}

void pickupClass::setStartMat(float x)
{
	this->transStart = XMMatrixTranslation(x - 10, 1.5f, 0.0f);
}

void pickupClass::getTranslationMatStart(XMMATRIX & other)
{
	other = this->transStart;
}

void pickupClass::resetPickup()
{
	isDestroy = false;
}


bool pickupClass::getCheckIfObjHolder()
{
	return this->checkIfSetToObjHolder;
}

void pickupClass::setCheckIfObjHolder(bool check)
{
	this->checkIfSetToObjHolder = check;
}