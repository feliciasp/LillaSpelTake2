#include "playerClass.h"

playerClass::playerClass()
{
	obj = 0;
	moveVal = 0;
	input = 0;
	
	hasRing = false;
	ringType = 0;

	translation = XMMatrixIdentity();
	transStart = XMMatrixIdentity();
	triggerCheck = { 4.0f, 0.0f, 0.0f };

	isInObjHolder = false;

	HP = 3;
	//movement
	moveValX = 0.0f;
	moveValY = 0.0f;
	moveValZ = 0.0f;
	moveMat = XMMatrixIdentity();
	upSpeed = 0.0f;
	isJumping = false;
	justJumped = false;
	flipped = false;

	isPlayerHurt = false;
	frameCount = 2;

	isHurt = false;
	fakeTimer = 0;

	isAttacking = false;

	currentFrame = 1;
	currentAnimation = 1;
	currentTime = 0;

	idle = true;
	running = false;
	jumping = false;
	falling = false;
	attacking = false;
	isHit = false;
	timeBetweenFrames = 0.25f;
	nrOfLoops = 0;

	hasDoubleJumped = false;

	spaceReleased = true;
}

playerClass::playerClass(const playerClass & other)
{
}

playerClass::~playerClass()
{
}

bool playerClass::initialize(ID3D11Device* device, const char* filename, HINSTANCE hInstance, HWND hwnd)
{
	bool result;

	obj = new objectClass;
	if (!obj)
	{
		MessageBox(NULL, L"Error create object obj",
			L"Error", MB_OK | MB_ICONERROR);
		return false;
	}
	result = obj->initlialize(device, filename);
	if (!result)
	{
		MessageBox(NULL, L"Error init enemy obj",
			L"Error", MB_OK | MB_ICONERROR);
		return false;
	}

	obj->setType(2);

	input = new directInput;
	if (!input)
	{
		MessageBox(NULL, L"Error create object obj",
			L"Error", MB_OK | MB_ICONERROR);
		return false;
	}
	result = input->initialize(hInstance, hwnd);
	if (!result)
	{
		MessageBox(NULL, L"Error init enemy obj",
			L"Error", MB_OK | MB_ICONERROR);
		return false;
	}

	weapon = new weaponClass;
	if (!weapon)
	{
		MessageBox(NULL, L"Error create object weapon",
			L"Error", MB_OK | MB_ICONERROR);
		return false;
	}

	setStartMat(0.0f, 5.0f);

	return true;
}

void playerClass::shutdown()
{
	if (obj)
	{
		obj->shutdown();
		delete obj;
		obj = 0;
	}
	if (input)
	{
		input->shutdown();
		delete input;
		input = 0;
	}
	if (weapon)
	{
		weapon->shutdown();
		delete weapon;
		weapon = 0;
	}
}

void playerClass::setTranslation(float x)
{
	translation = XMMatrixTranslation(x, 0.0f, 0.0f);
}

objectClass* playerClass::getObj()
{
	return this->obj;
}

void playerClass::getTranslationMat(XMMATRIX & other)
{
	other = this->translation;
}

void playerClass::getTranslationMatStart(XMMATRIX & other)
{
	other = this->transStart;
}

void playerClass::setStartMat(float x, float y)
{
	this->transStart = XMMatrixTranslation(x, y, 0.0f);
}

float playerClass::getMove()
{
	return moveVal;
}

void playerClass::setMove(float x)
{
	moveVal -= x;
}

void playerClass::resetMove()
{

	moveVal = 0;
}
bool playerClass::hurtState()
{
	if (this->isHurt == false && fakeTimer == 0)
	{
		this->isHurt = true;
		fakeTimer = 300;
		return true;
	}
	else
	{
		return false;
	}
}

void playerClass::updateTimer()
{
	this->fakeTimer -= 1;
	if (this->fakeTimer == 0)
	{
		this->isHurt = false;
	}
}

XMVECTOR playerClass::getTriggerCheck()
{
	return this->triggerCheck;
}

void playerClass::handleMovement(double dt)
{

	oldMoveValX = moveValX;
	oldMoveValY = moveValY;

	if (dt > 20)
		dt = 0;

	idle = true;


	justJumped = false;
	moveValY += upSpeed * dt;


	currentAnimation = 1;
	frameCount = 2;
	timeBetweenFrames = 0.25f;

	input->readKeyboard(dt);
	if (this->input->isAPressed())
	{
		moveValX += -10.0f * dt;
		if (running == false && attacking == false)
		{
			currentTime = 0;
			currentFrame = 1;
		}
		currentAnimation = 2;
		frameCount = 8;
		timeBetweenFrames = 0.1f;
		idle = false;
		running = true;
		//OutputDebugString(L"func move left called");
		if (this->flipped == false)
		{
			flipped = true;

		}
	}
	
	if (this->input->isDPressed())
	{
		moveValX += 10.0f * dt;
		if (running == false && attacking == false)
		{
			currentTime = 0;
			currentFrame = 1;
		}
		currentAnimation = 2;
		frameCount = 8;
		timeBetweenFrames = 0.1f;
		idle = false;
		running = true;
		//OutputDebugString(L"func move right called");
		if (this->flipped == true)
		{
			flipped = false;
		}
	}
	
	if (this->input->isSpacePressed())
	{
		if (!isJumping)
		{
			upSpeed = 23.5f;
			//OutputDebugString(L"upSpeed set");
			justJumped = true;
			spaceReleased = false;
			if (attacking == false)
			{
				currentFrame = 1;
				currentTime = 0;
			}
			
		}


		if (hasRing == true && ringType == 0)
		{
			if (isJumping == true && !hasDoubleJumped && spaceReleased)
			{
				upSpeed = 23.5f;
				if (attacking == false)
				{
					currentFrame = 1;
					currentTime = 0;
				}
				hasDoubleJumped = true;
			}
		}
		isJumping = true;
	}

	if (!this->input->isSpacePressed() && upSpeed > upSpeed * 0.5)
	{
		upSpeed -= upSpeed - (upSpeed * 0.99);
	}
	if (!this->input->isSpacePressed())
	{
		spaceReleased = true;
	}

	if (upSpeed > 1)
	{
		running = false;
		idle = false;
		jumping = true;
		currentAnimation = 3;
		frameCount = 2;
		timeBetweenFrames = 0.1f;
	}
	
	if (upSpeed > -1.0f)
	{
		upSpeed += (-50 * dt) - moveValY * dt;
		isJumping = true;
	}
	else if (upSpeed < -1.0f) //upSpeed less than -1.0f;
	{
		if (falling == false && attacking == false)
		{
			currentTime = 0;
			currentFrame = 1;
		}
		upSpeed += (-50 * dt) - -upSpeed * dt;
		currentAnimation = 4;
		timeBetweenFrames = 0.1f;
		frameCount = 2;
		idle = false;
		running = false;
		falling = true;
	}

	if (this->input->isOPressed())
	{
		if (attacking == false)
		{
			attacking = true;
			currentTime = 0;
			currentFrame = 1;
		}
	}

	if (attacking == true)
	{
		currentAnimation = 6;
		frameCount = 4;

		timeBetweenFrames = 0.1f;

		idle = false;
	}
	
	if (running == true && idle == true && attacking == false)
	{
		running = false;
		currentTime = 0;
		currentFrame = 1;
		timeBetweenFrames = 0.25f;
	}

	if (falling == true && idle == true && attacking == false)
	{
		falling = false;
		currentTime = 0;
		currentFrame = 1;
		timeBetweenFrames = 0.25f;
	}

	if (jumping == true && idle == true && attacking == false)
	{
		jumping = false;
		currentTime = 0;
		currentFrame = 1;
		timeBetweenFrames = 0.25f;

	}

	if (isHit == true && idle == true)
	{
		isHit = false;
		currentTime = 0;
		currentFrame = 1;
		timeBetweenFrames = 0.1f;
	}
	moveMat = XMMatrixTranslation(moveValX, moveValY+8, 0.0f);

}

void playerClass::checkCollisions(bool top, bool left, bool right, bool bot)
{
	if (top)
	{
		moveValY = oldMoveValY;
		if (upSpeed > 0)
			upSpeed = 0;
	}

	if (bot)
	{
		moveValY = oldMoveValY;
		isJumping = false;
		upSpeed = 0;
		idle = true;
		hasDoubleJumped = false;
	}

	if (left)
	{
		moveValX = oldMoveValX;
	}
	if (right)
	{
		moveValX = oldMoveValX;
	}

	moveMat = XMMatrixTranslation(moveValX, moveValY + 8, 0.0f);
}

void playerClass::checkIfAttacking()
{
	if (isHit == true)
	{
		this->isAttacking = true;
	}
	else
	{
		this->isAttacking = false;
	}
}

bool playerClass::getIfAttack()
{
	return isHit;
}

bool playerClass::getIfInObjHolder()
{
	return this->isInObjHolder;
}

void playerClass::setIfInObjHolder(bool other)
{
	this->isInObjHolder = other;
}


float playerClass::getMoveValY()
{
	return this->moveValY;
}

void playerClass::setHasRing(bool check)
{
	this->hasRing = check;
}

bool playerClass::getHasRing()
{
	return this->hasRing;
}

void playerClass::setRingType(int other)
{
	this->ringType = other;
}

int playerClass::getRingType()
{
	return this->ringType;
}

void playerClass::getMoveMat(XMMATRIX& mat)
{
	mat = moveMat;
}

void playerClass::resetPlayer()
{
	moveVal = 0;
	translation = XMMatrixIdentity();
	transStart = XMMatrixIdentity();

	//movement
	moveValX = 0.0f;
	moveValY = 0.0f;
	moveValZ = 0.0f;
	moveMat = XMMatrixIdentity();
	upSpeed = 0.0f;
	isJumping = false;
	HP = 3;
	isAttacking = false;

	isInObjHolder = false;

	hasRing = false;

}

void playerClass::setPlayerHP(int x)
{
	this->HP = x;
}

int playerClass::getPlayerHP()
{
	return this->HP;
}

void playerClass::setPlayerHurt(bool x)
{
	this->isPlayerHurt = x;
}

bool playerClass::getPlayerHurt()
{
	return this->isPlayerHurt;
}

void playerClass::updateAnimation(double dt)
{
	if (currentTime > timeBetweenFrames)
	{
		currentFrame++;
		if (currentFrame > frameCount)
		{
			currentFrame = 1;
			if (attacking == false)
			{
				if (idle == true || running == true)
					nrOfLoops++;
			}
			
			if (attacking == true)
			{
				attacking = false;
				isHit = true;
			}
			
		}
		
		if (nrOfLoops == 3 && currentFrame == frameCount)
		{
			currentFrame++;
			nrOfLoops = 0;
		}
		currentTime = 0;
	}

	currentTime+= 1 * dt;
}

bool playerClass::getFlipped()
{
	return this->flipped;
}

int playerClass::getFrameCount()
{
	return this->frameCount;
}

int playerClass::getCurrentFrame()
{
	return this->currentFrame;
}

int playerClass::getCurrentAnimation()
{
	return this->currentAnimation;
}

float playerClass::getAnimationTime()
{
	return this->animationTime;
}

float playerClass::getcurrentTime()
{
	return this->currentTime;
}

weaponClass * playerClass::getWeapon()
{
	return this->weapon;
}
