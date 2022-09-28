#include "models.h"

Node::Node(float speed, float voltage)
{
	this->speed_kmh = speed;
	this->voltage_v = voltage;
    this->time = millis();
	this->next = NULL;
}

Node* Node::CreateHead(float speed, float voltage)
{
	Node* head = new Node(speed, voltage);
    node_cnt++;
	return head;
}

void Node::Insert(Node* head, float speed, float voltage)
{    
	Node* current = head;
	Node* node_to_insert = new Node(speed, voltage);

	while (current->next != NULL)
		current = current->next;

	current->next = node_to_insert;

    node_cnt++;
	current = NULL;
	node_to_insert = NULL;
}

void Node::DeleteAll(Node* head)
{
	Node* current = head;
	Node* to_delete;

	while (current != NULL)
	{
		to_delete = current;
		current = current->next;
		delete to_delete;
	}

    node_cnt = 0;
	current = NULL;
	to_delete = NULL;
}
