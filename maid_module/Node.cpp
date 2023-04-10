#include "models.h"

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

    while (current->next != nullptr)
      current = current->next;

    current->next = node_to_insert;

    node_cnt++;
    current = nullptr;
    node_to_insert = nullptr;
}

// * Delete all Nodes in linked list
void Node::DeleteAll(Node* head)
{
    Node* current = head;
    Node* to_delete;

    while (current != nullptr)
    {
      to_delete = current;
      current = current->next;
      delete to_delete;
    }

    node_cnt = 0;
    current = nullptr;
    to_delete = nullptr;
}
