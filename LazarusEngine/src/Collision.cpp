#include "Collision.h"
#include <iostream>

Collision::Collision()
{

}

bool Collision::checkAABBCollision(glm::vec3 mesh1NegIn, glm::vec3 mesh1PosIn, glm::vec3 mesh2NegIn, glm::vec3 mesh2PosIn)
{
	// Output mesh values to check them
	/*
	std::cout << "Mesh 1 Negative, X : " << mesh1NegIn.x << " Y : " << mesh1NegIn.y << " Z : " << mesh1NegIn.z << std::endl;
	std::cout << "Mesh 1 Positive, X : " << mesh1PosIn.x << " Y : " << mesh1PosIn.y << " Z : " << mesh1PosIn.z << std::endl;
	std::cout << "Mesh 2 Negative, X : " << mesh2NegIn.x << " Y : " << mesh2NegIn.y << " Z : " << mesh2NegIn.z << std::endl;
	std::cout << "Mesh 2 Positive, X : " << mesh2NegIn.x << " Y : " << mesh2PosIn.y << " Z : " << mesh2PosIn.z << std::endl;
	*/

	/*
	if ((mesh1NegIn.x < mesh2PosIn.x && mesh1NegIn.x > mesh2NegIn.x) || (mesh2NegIn.x < mesh1PosIn.x && mesh2NegIn.x > mesh1NegIn.x))
	{
		if ((mesh1NegIn.y < mesh2PosIn.y && mesh1NegIn.y > mesh2NegIn.y) || (mesh2NegIn.y < mesh1PosIn.y && mesh2NegIn.y > mesh1NegIn.y))
		{
			if ((mesh1NegIn.z < mesh2PosIn.z && mesh1NegIn.z > mesh2NegIn.z) || (mesh2NegIn.z < mesh1PosIn.z && mesh2NegIn.z > mesh1NegIn.z))
			{
				return true;
			}
		}
	}
	*/

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

	return true;
}