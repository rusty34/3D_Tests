
#include "node.h"

#include <vector>

Node::Node(const char* name) 
	: m_Name(name)
{

}


Node::~Node()
{
	m_Children.clear();
}


void Node::AddChild(Node* child)
{
	if(child != NULL)
	{
		m.Children.push_back(child);
	}
}

