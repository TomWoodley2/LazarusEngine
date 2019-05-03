#pragma once
#include <list>
#include <string>

class Node
{
public:
	virtual bool run() = 0;
};

class CompositeNode : public Node {
private:
	std::list<Node*> m_children;

public:
	const std::list<Node*>& getChildren() const { return m_children; }
	void addChild(Node* child) { m_children.emplace_back(child); }
};