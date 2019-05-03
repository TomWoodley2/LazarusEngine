#include "Collision.h"
#include <iostream>

Collision::Collision()
{

}

bool Collision::checkAABBCollision(glm::vec3 mesh1NegIn, glm::vec3 mesh1PosIn, glm::vec3 mesh2NegIn, glm::vec3 mesh2PosIn)
{
	
	// NEG 1 X > POS 2 Y = no collision
	if (mesh1NegIn.x > mesh2PosIn.x)
	{
		return false;
	}
	if (mesh2NegIn.x > mesh1PosIn.x)
	{
		return false;
	}
	if (mesh1NegIn.y > mesh2PosIn.y)
	{
		return false;
	}
	if (mesh2NegIn.y > mesh1PosIn.y)
	{
		return false;
	}
	if (mesh1NegIn.z > mesh2PosIn.z)
	{
		return false;
	}
	if (mesh2NegIn.z > mesh1PosIn.z)
	{
		return false;
	}

	// Get the closest plane - smallest closestValue
	float closestValue = 999999.0f;
	float currentValue = 0.0f;

	closestPlane = 'N'; // Closest plane
	planeValue = 0; // Value of the plane

	// X Positive
	currentValue = mesh2NegIn.x - mesh1PosIn.x;
	currentValue = currentValue * currentValue; // Square to make positive
	if (currentValue < closestValue)
	{
		closestPlane = 'X';
		planeValue = -1;
		closestValue = currentValue;
	}

	//std::cout << "X positive : " << currentValue << std::endl;

	// X Negative
	currentValue = mesh1NegIn.x - mesh2PosIn.x;
	currentValue = currentValue * currentValue; // Square to make positive
	if (currentValue < closestValue)
	{
		closestPlane = 'X';
		planeValue = -1;
		closestValue = currentValue;
	}

	//std::cout << "X negative : " << currentValue << std::endl;
	
	// Y Positive
	currentValue = mesh2NegIn.y - mesh1PosIn.y;
	currentValue = currentValue * currentValue; // Square to make positive
	if (currentValue < closestValue)
	{
		closestPlane = 'Y';
		planeValue = -1;
		closestValue = currentValue;
	}

	//std::cout << "Y positive : " << currentValue << std::endl;

	// Y Negative
	currentValue = mesh1NegIn.y - mesh2PosIn.y;
	currentValue = currentValue * currentValue; // Square to make positive
	if (currentValue < closestValue)
	{
		closestPlane = 'Y';
		planeValue = -1;
		closestValue = currentValue;
	}

	
	//std::cout << "Y negative : " << currentValue << std::endl;


	// Z Positive
	currentValue = mesh2NegIn.z - mesh1PosIn.z;
	currentValue = currentValue * currentValue; // Square to make positive
	if (currentValue < closestValue)
	{
		closestPlane = 'Z';
		planeValue = -1;
		closestValue = currentValue;
	}

	//std::cout << "Z positive : " << currentValue << std::endl;

	// Z Negative
	currentValue = mesh1NegIn.z - mesh2PosIn.z;
	currentValue = currentValue * currentValue; // Square to make positive
	if (currentValue < closestValue)
	{
		closestPlane = 'Z';
		planeValue = -1;
		closestValue = currentValue;
	}

	//std::cout << "Z negative : " << currentValue << std::endl;

	//std::cout << closestPlane << std::endl;
	//std::cout << planeValue << std::endl;


	return true;
}




char Collision::getClosestPlane()
{
	return closestPlane;
}

int Collision::getPlaneValue()
{
	return planeValue;
}