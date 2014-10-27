//Header that contains classes for Linked List, wordIndex, and Node.

#include <iostream>
#include <string>
#include <iomanip>

using namespace std;

// wordIndex class is used to keep track of the number of words printed to the outstream
// This class is used to insure that 8 words are printed per line. Class contains a function
// to reset the word index increment back to 0, and also to get the word index.
class wordIndex {
public:
	int word_index;

	int getIndex(void) {
		return word_index;
	}

	void resetIndex(void) {
		word_index = 0;
	}
	void incrementIndex() {
		++word_index;
	}

	wordIndex() {
		word_index = 0;
	}
};

// Node class is used to hold the data of our programs, and the pointer to the next node within the
// linked list. Class contains methods for returning the value of data, and returning the pointer to
// next node in linked list.
class Node {
public:	
	int data;
	Node* next;

	int getData(void) {
		return data;
	}
	Node* getNext(void) {
		return next;
	}
	Node(int x) {
		data = x;
	}
	Node(int x, Node* y) {
		data = x;
		next = y;
	}
};

// linkedList class is used for all of the conditions needed to handle a linked list. Including the
// methods addNode(for adding a node to linkedlist), nodePosition(for returning the value of the nodes
// position within the linked list), combine(for combining two nodes together), removeNode(for removing
// a node from the linked list),  search(for finding a specific node using its value as an indicator), 
// nodeCount(for returning the total number of nodes contained in linked list), addSpecific(for adding
// a node to a specific spot in the linked list), print(for printing out the print_data given to method as
// as many times as data value for node is), largestNode(for finding the largest node within the linkedlist),
// and bestFitNode(for finding the node with the smallest data that the value given can fit into).
class linkedList {
	Node* head;
public:
	linkedList() {
		head = NULL;
	}
	linkedList(int val) {
		head = new Node(val, NULL);
	}
	void addNode(int val) {
		Node *p;
		p = head;
		if(head == NULL) {
			head = new Node(val, NULL);
		}
		else {
			p = head;
			while(p->next != NULL) {
				p = p->next;
			}
			p->next = new Node(val, NULL);
		}
	}
	int nodePosition(int val, int total_nodes) {
		Node* p;
		p = head;

		for(int i = 1; i <= total_nodes; ++i) {
			if(p->data == val) {
				return i;
			}
			else {
				p = p->next;
			}
		}
	}
	int combine(int val, int node) {
		Node* p;
		p = head;
		int node_number = node;
		int new_data;

		for(int i = 1; i < node_number; ++i) {
			p = p->next;
		}
		p->data = p->data + val;
		return p->data;
	} 
	void removeNode(int val) {
		Node* pPre; Node* pDel;
		if(head->data == val) {
			pDel = head;
			head = pDel->next;
			delete pDel;
			return;
		}
		pPre = head;
		pDel = head->next;
		while(pDel != NULL) {
			if(pDel->data == val) {
				pPre->next = pDel->next;
				delete pDel;
				break;
			}
			pPre = pDel;
			pDel = pDel->next;
		}
	}
	Node* search(int val) {
		Node* pNode = head;
		while(pNode != NULL) {
			if(pNode->data == val) {
				return pNode;
			}
			pNode = pNode->next;
		}
		return NULL;
	}

	int nodeCount() {
		Node* p;
		p = head;
		int count = 0;
		while(p != NULL) {
			count++;
			p = p->next;
		}
		return count;
	}
	void addSpecific(int val, int node_no) {
		Node* temp;
		Node* temp1;
		int i;
		Node* prev;

		if(node_no == 0) {
			temp = new Node(val, head);
		}
		else if(node_no == nodeCount() + 1) {
			addNode(val);
		}
		else if(node_no == 1) {
			temp = head;
			temp1 = new Node(val, temp);
			temp1->next = head;
			head = temp1;
		}
		else {
			temp = head;
			i = 1;
			while(i < node_no) {
				i = i + 1;
				prev = temp;
				temp = temp->next;
			}
			temp1 = new Node(val, temp);
			prev->next = temp1;
			temp = temp->next;
		}
	}
	void print(string print_data, int node_no, wordIndex* index) {
		Node* p = head;
		Node* temp;
		int i;
		int number = node_no;
		wordIndex* word_index = index;

		if(node_no == 0 || node_no == 1) {
			temp = p;
		}
		else {
			i = 1;
			while(i < node_no) {
				i = i +1;
				temp = p->next;
				p = p->next;
			}
		}
		cout << left;
		for(int i = 0; i < temp->data; ++i) {
			cout << setw(4) << print_data << " ";
			word_index->incrementIndex();
			if(word_index->getIndex() == 8) {
				cout << endl;
				word_index->resetIndex();
			}
		}
	}

	Node* largestNode() {
		Node* largest = head;
		Node* current = head;
		while(current) {
			if(largest->data < current->data) {
				largest = current;
			}
			else {
				current = current->next;
			}
		}
		return largest;
	}
	Node* bestFitNode(int val) {
		Node* best = NULL;
		Node* node = head;
		if(nodeCount() == 1) {
			best = node;
			return best;
		}

		while(node != NULL) {
			if((node->data >= val) && (best == NULL || node->data < best->data)) {
				best = node;
				if(best->data == val) {
					break;
				}
			}
			node = node->next;
		}
		return best;
	}
};