#pragma once

#ifndef INTERACTOR
#define INTERACTOR

#include "SPHInteractor3d.h"
#include <cmath>
#include <gl\glew.h>
#include <gl\GL.h>
#include <gl\GLU.h>
#include <glm\gtx\norm.hpp>
#include "GlmVec.h"

struct SPHParticle3d;
class MarchingCubesShaded;

class Interactor : public SPHInteractor3d
{
	glm::vec3 min;
	glm::vec3 max;
	float distanceSquared;
	float distance;
	float dampening;

	void setImage(const char* texturePath);

	MarchingCubesShaded *marchingCubes;
	SPHParticle3d* interactor_;

public:
	Interactor();

	int textureID;
	void addInteractor(glm::vec3 position, glm::vec3 velocity);
	void applyDensity(SPHParticle3d& other, glm::vec3 rvec);
	void applyForce(SPHParticle3d& other, glm::vec3 rvec);
	void enforceInteractor(SPHParticle3d& other, glm::vec3 rvec);
	glm::vec3 directionTo(SPHParticle3d& other);
	void draw();
	void draw(MarchingCubesShaded* ms, float radius);
	void putSphere(float x, float y, float z, float r);
};

#endif
