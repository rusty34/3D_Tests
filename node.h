
#ifndef NODE_H
#define NODE_H

#include <vector>

class Node 
{

public:
	Node(const char* name);
	~Node();

	void AddChild(Node* child);

	const char* GetName() { return m_Name; }

private:
	std::vector<Node*> m_Children;
	const char* m_Name;

};

#endif