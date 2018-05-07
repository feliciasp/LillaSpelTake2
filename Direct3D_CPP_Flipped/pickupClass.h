#pragma once
#ifndef PICKUPCLASS_H
#define PICKUPCLASS_H

#include "objectClass.h"

class pickupClass
{
public:
	pickupClass();
	pickupClass(const pickupClass& other);
	~pickupClass();

	void clone(const pickupClass& other);

	bool initlialize(ID3D11Device* device, const char* filename);
	void shutdown();

	objectClass* getObj();

	bool getIsDestry();
	void setIsDestroy(bool check);

	void setStartMat(float x);
	void setTranslationMatStart(XMMATRIX& other);
	void getTranslationMatStart(XMMATRIX& other);
	void resetPickup();

	bool getCheckIfObjHolder();
	void setCheckIfObjHolder(bool check);

	void updateAnimation(double dt);
	int getFrameCount();
	void setFrameCount(int frameCount);
	int getCurrentFrame();
	void setCurrentAnimation(int currentAnimation);
	int getCurrentAnimation();
	void setAnimationCount(int animationCount);
	int getAnimationCount();

	void setPickupType(int other);
	int getPickupType();

	void setRingType(int other);
	int getRingType();

private:
	objectClass * obj;
	bool isDestroy;
	XMMATRIX transStart;
	bool checkIfSetToObjHolder;

	//std::vector<pickupClass> pickupArrayHolder;

	int frameCount;
	int currentFrame;
	int currentAnimation;
	int animationCount;
	float currentTime;
	float animationSpeed;
	bool isAnimated;

	int pickupType; // 0 = 3D PIXEL(health), 1 = PIXEL ESSENCE(exp), 2 = POLYGON(currency), 3 = RING, 4 = LIMBO PLANES, 5 = BLACKSMITH

	bool isRing;
	int ringType; // 0 = DOUBLEJUMP, 1 = FIREBALL, 2 = ???, 3 = ???, 4 = ???, 5 = ???, 6 = ???, 7 = ???, 8 = ???.                                            profit.
};


#endif