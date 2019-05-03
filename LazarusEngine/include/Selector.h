#pragma once
#include "Node.h"

class Selector : public CompositeNode
{
public:
	bool run() override {
		for (Node* child : getChildren())
		{
			if (child->run())
				return true;
		}
		return false;
	}
};