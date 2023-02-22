#include "models.h"

// * Create Node example with readings inside
Node::Node(int impulse_cnt, int analog_voltage, ulong millis)
{
    this->impulse_cnt = impulse_cnt;
    this->analog_voltage = analog_voltage;
    this->time = millis;
    this->next = NULL;
}

// * Create head of linked list
Node* Node::CreateHead(int impulse_cnt, int analog_voltage, ulong millis)
{
    Node* head = new Node(impulse_cnt, analog_voltage, millis);
    node_cnt++;
    
    return head;
}

// * Insert Node in linked list
void Node::Insert(Node* head, int impulse_cnt, int analog_voltage, ulong millis)
{    
    Node* current = head;
    Node* node_to_insert = new Node(impulse_cnt, analog_voltage, millis);

    while (current->next != NULL)
      current = current->next;

    current->next = node_to_insert;

    node_cnt++;
    current = NULL;
    node_to_insert = NULL;
}

// * Delete all Nodes in linked list
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
