#pragma once
#include "Node.h"

class Sequence : public CompositeNode {
public:
	bool run() override {
		for (Node* child : getChildren()) {
			if (!child->run())
				return false;
		}
		return true;
	}
};