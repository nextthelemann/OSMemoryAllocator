/*
 *COP3503 Programming Assignment 2 [OS Memory Manager]
 ------------------------------------------------------------------------------------
 *	C++ Program that emulates the operating system's responsibility of allocating
 *	memory to certain programs. This is a very simple page-based view of memory management.
 *	On startup, my program will have some 32 pages of contiguous, unused memory. Each page
 *  will be 4 KB long. It then allows the user to "run" programs that require chunks of this
 * memory for some period of time. It should also allow the users to "kill" programs that are
 * in progress. The pages used by programs that are killed can then be re-used for future programs.
 ---------------------------------------------------------------------------------------------------
 * @uthor Scott Thelemann
 * 
  Version 1.0 10/27/2014
 *
*/



#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include "pa2.h"

using namespace std;


bool is_running = true; 				// bool to determine if program is running or not
int num_of_pages = 32; 					//pages of free_memory, can be scaled up to bigger values of memory.
double size_of_pages = 4;				// each page is of 4KB
linkedList freespace(num_of_pages);		//initializes freespace linked list with total number of pages for memory
linkedList usedspace;					//initializes usedspace linked list 

//Program class that creates objects called program to keep track of size, and progam name
class program {
public:
	string program_name;
	int size;
	program(string name, int pages) {
		program_name = name;
		size = pages;
	}
};

vector<program*> programs_running(num_of_pages);	//vector of pointers to object program, used to hold print position of memory 

//Function for adding a program using worst fit algorithm
void worstAdd() {
	string program_name;
	int program_size;
	int pages;
	int largestValue;
	Node* node;
	program* program_added;

	cout << "Program name - ";
	cin >> program_name;
	cout << "Program size (KB) - ";
	cin >> program_size;
	cout << endl;

	double size = program_size;
	double value = ceil(program_size / size_of_pages);		//# of pages of menory program takes up
	pages = value;
	node = freespace.largestNode();		//Largest node of memory for freespace
	largestValue = node->getData();		
	
	if(largestValue < pages) {
		cout << "Error, Not enough memory for Program " << program_name << endl;
		return;
	}

	int node_value = largestValue - pages;
	int total_free_nodes = freespace.nodeCount();
	int free_node_position = freespace.nodePosition(largestValue, total_free_nodes);
	
	//for loop that cheks if program being added is already running or not.
	for(int i = 0; i < programs_running.size(); ++i) {
		if(programs_running[i] == NULL) {
			continue;
		}
		else if(programs_running[i]->program_name == program_name) {
			cout << endl << "Error, Program " << program_name << " already running." << endl;
			return;
		}
	}

	//if/else statment to handle if program takes up all the pages of memory
	if(node_value == 0) {
		freespace.removeNode(largestValue);
	}
	else {
		freespace.addSpecific(node_value, free_node_position);
		freespace.removeNode(largestValue);
	}

	int total_nodes = usedspace.nodeCount() + freespace.nodeCount();
	int used_node_position = 1;
	int used_node_count = 0;
	int total_node_count = 0;

	// Block of if/else if/ else statments below is used to handle the memory situation depending
	// on free_node_position, free- node_value, and total_free_nodes. It then adds the node
	// to a specific spot in the linked list, and adds a pointer to program in the vector
	// to keep print order. 
	if (node_value == 0 && free_node_position == 1 && total_free_nodes > 1) {
		for(int i = 0; i < total_nodes; ++i) {
			if(programs_running[i] == NULL) {
				used_node_position = i;
				break;
			}
			else {
				++total_node_count;
			} 	
		}
		usedspace.addSpecific(pages, total_node_count + 1);
		program_added = new program(program_name, pages);
		programs_running[used_node_position] = program_added;
	}
	else if(node_value == 0 && total_free_nodes == 1) {
		for(int i = 0; i <= total_nodes; ++i) {
			if(programs_running[i] == NULL) {
				used_node_position = i;
				break;
			}
			else {
				++total_node_count;
			} 	
		}
		usedspace.addSpecific(pages, total_node_count + 1);
		program_added = new program(program_name, pages);
		programs_running[used_node_position] = program_added;
	}
	else if(free_node_position < freespace.nodeCount() && free_node_position != 1) {
		for(int i = 1; i <= (total_nodes - free_node_position); ++i) {
			if(programs_running[i-1] == NULL && programs_running[i] != NULL) {
				++total_node_count;
			}
			else if(programs_running[i-1] == NULL && i == (total_nodes - free_node_position)) {
				used_node_position = i - 1;
				break;
			}
			else {
				++total_node_count;
			} 	
		}
		for(int i = total_nodes; i > total_node_count; ++i) {
			programs_running[i] = programs_running[i - 1];
		}
		usedspace.addSpecific(pages, used_node_position);
		program_added = new program(program_name, pages);
		programs_running[total_node_count] = program_added;
	}
	else if(free_node_position == 1 && free_node_position < freespace.nodeCount()) {
		for(int i = 1; i <= (total_nodes - freespace.nodeCount()); ++i) {
			if(programs_running[i-1] == NULL && programs_running[i] != NULL) {
				used_node_position = i;
				break;
			}
			else if(programs_running[i-1] == NULL && programs_running[i] == NULL) {
				break;
			}
			else {
				++total_node_count;
				used_node_position = i + 1;
			} 	
		}
		for(int i = total_nodes; i > total_node_count; --i) {
			programs_running[i] = programs_running[i - 1];
		}
		usedspace.addSpecific(pages, used_node_position);
		program_added = new program(program_name, pages);
		programs_running[total_node_count] = program_added;
	}
	else if(freespace.nodeCount() == 1 && programs_running[total_nodes - 1] == NULL) {
		usedspace.addSpecific(pages, total_nodes);
		program_added = new program(program_name, pages);
		programs_running[total_nodes - 1] = program_added;
	}
	else {
		for(int i = 1; i <= total_nodes; ++i) {
			if(programs_running[i-1] == NULL && programs_running[i] != NULL) {
				++total_node_count;
			}
			else if(programs_running[i-1] == NULL && i == total_nodes) {
				used_node_position = i - 1;
				break;
			}
			else {
				++total_node_count;
			} 
		}
		usedspace.addSpecific(pages, used_node_position);
		program_added = new program(program_name, pages);
		programs_running[total_node_count] = program_added;
	}





 	cout << endl << "Program " << program_name << " added successfully: " << pages << " page(s) used." << endl;
}

// Function for adding programs using best fit alogrithm. Runs exact same as worst fit except
// instead of searching for largstNode, method searches for smallest chunk of memory that program
// can fit into. It then takes memory from that chunk to be used for program. 
void bestAdd() {
	string program_name;
	int program_size;
	int pages;
	int bestValue;
	Node* node;
	program* program_added;

	cout << "Program name - ";
	cin >> program_name;
	cout << "Program size (KB) - ";
	cin >> program_size;

	double size = program_size;
	double value = ceil(program_size / size_of_pages);
	pages = value;
	node = freespace.bestFitNode(value);
	bestValue = node->getData();
	
	if(bestValue < pages) {
		cout << "Error, Not enough memory for Program " << program_name << endl;
		return;
	}

	int node_value = bestValue - pages;
	int total_free_nodes = freespace.nodeCount();
	int free_node_position = freespace.nodePosition(bestValue, free_node_position);
	
	for(int i = 0; i < programs_running.size(); ++i) {
		if(programs_running[i] == NULL) {
			continue;
		}
		else if(programs_running[i]->program_name == program_name) {
			cout << endl << "Error, Program " << program_name << " already running." << endl;
			return;
		}
	}

	if (node_value == 0) {
		freespace.removeNode(bestValue);
	}
	else {
		freespace.addSpecific(node_value, free_node_position);
		freespace.removeNode(bestValue);
	}

	int total_nodes = usedspace.nodeCount() + freespace.nodeCount();
	int used_node_position = 1;
	int used_node_count = 0;
	int total_node_count = 0;

	if (node_value == 0 && free_node_position == 1 && total_free_nodes > 1) {
		for(int i = 0; i < total_nodes; ++i) {
			if(programs_running[i] == NULL) {
				used_node_position = i;
				break;
			}
			else {
				++total_node_count;
			} 	
		}
		usedspace.addSpecific(pages, total_node_count + 1);
		program_added = new program(program_name, pages);
		programs_running[used_node_position] = program_added;
	}
	else if(node_value == 0 && total_free_nodes == 1) {
		for(int i = 0; i <= total_nodes; ++i) {
			if(programs_running[i] == NULL) {
				used_node_position = i;
				break;
			}
			else {
				++total_node_count;
			} 	
		}
		usedspace.addSpecific(pages, total_node_count + 1);
		program_added = new program(program_name, pages);
		programs_running[used_node_position] = program_added;
	}
	else if(node_value == 0 && total_free_nodes > 1 && free_node_position != 1) {
		int total_free_count = 0;
		int i = 0;
		while(total_free_count != free_node_position) {
			if(programs_running[i] == NULL) {
				++total_free_count;
			}
			else {
				++total_node_count;
			}
			++i; 	
		}
		usedspace.addSpecific(pages, total_node_count + 1);
		program_added = new program(program_name, pages);
		programs_running[i - 1] = program_added;
	}
	else if (free_node_position < freespace.nodeCount() && free_node_position != 1) {
		for(int i = 1; i <= (total_nodes - free_node_position); ++i) {
			if(programs_running[i-1] == NULL && programs_running[i] != NULL) {
				++total_node_count;
			}
			else if(programs_running[i-1] == NULL && i == (total_nodes - free_node_position)) {
				used_node_position = i - 1;
				break;
			}
			else {
				++total_node_count;
			} 	
		}
		for(int i = total_nodes; i > total_node_count; ++i) {
			programs_running[i] = programs_running[i - 1];
		}
		usedspace.addSpecific(pages, used_node_position);
		program_added = new program(program_name, pages);
		programs_running[total_node_count] = program_added;
	}
	else if(free_node_position == 1 && free_node_position < freespace.nodeCount()) {
		for(int i = 1; i <= (total_nodes - freespace.nodeCount()); ++i) {
			if(programs_running[i-1] == NULL && programs_running[i] != NULL) {
				used_node_position = i;
				break;
			}
			else if(programs_running[i-1] == NULL && programs_running[i] == NULL) {
				break;
			}
			else {
				++total_node_count;
				used_node_position = i + 1;
			} 	
		}
		for(int i = total_nodes; i > total_node_count; --i) {
			programs_running[i] = programs_running[i - 1];
		}
		usedspace.addSpecific(pages, used_node_position);
		program_added = new program(program_name, pages);
		programs_running[total_node_count] = program_added;
	}
	else if(freespace.nodeCount() == 1 && programs_running[total_nodes - 1] == NULL) {
		usedspace.addSpecific(pages, total_nodes);
		program_added = new program(program_name, pages);
		programs_running[total_nodes - 1] = program_added;
	}
	else {
		for(int i = 1; i <= total_nodes; ++i) {
			if(programs_running[i-1] == NULL && programs_running[i] != NULL) {
				++total_node_count;
			}
			else if(programs_running[i-1] == NULL && i == total_nodes) {
				used_node_position = i - 1;
				break;
			}
			else {
				++total_node_count;
			} 
		}
		usedspace.addSpecific(pages, used_node_position);
		program_added = new program(program_name, pages);
		programs_running[total_node_count] = program_added;
	}



	cout << endl << "Program " << program_name << " added successfully: " << pages << " page(s) used." << endl;
}

// Method that kills programs from worst_fit alogrithm.
void worstKill() {
	string name;
	int pages;

	cout << "Program name - ";
	cin >> name;

	// bool & for loop that checks if program is running or not, and continues with kill process
	// if program is indeed running.
	bool doesNotExist = true;
	for(int i = 0; i < programs_running.size(); ++i) {
		if(programs_running[i] == NULL) {
			continue;
		}
		else if(programs_running[i]->program_name == name) {
			doesNotExist = false;
			break;
		}
	}
	if(doesNotExist == true) {
		cout << endl << "Program " << name << " is not running." << endl;
		return;
	}

	// if/else if statements nested inside of a for loop that runs through programs_running vector
	// until it finds the program running that user wants to kill. It then uses free node counter 
	// to keep track of position within vector and linked list.
	int free_nodes = 0;
	int total_nodes = freespace.nodeCount() + usedspace.nodeCount();
	for(int i = 0; i < total_nodes; ++i) {
		if(programs_running[i] == NULL) {
			++free_nodes;
		}
		else if(programs_running[i]->program_name == name) {
			if(i + 1 == total_nodes && programs_running[i - 1] != NULL) {
				freespace.addSpecific(programs_running[i]->size, free_nodes + 1);
				usedspace.removeNode(programs_running[i]->size);
				pages = programs_running[i]->size;
				programs_running[i] = NULL;
			}
			else if(programs_running[i - 1] == NULL && programs_running[i + 1] != NULL) {
				freespace.combine(programs_running[i]->size, free_nodes);
				usedspace.removeNode(programs_running[i]->size);
				pages = programs_running[i]->size;
				programs_running[i] = NULL;
				for(int j = i; j < total_nodes - 1; ++j) {
					programs_running[j] = programs_running[j + 1];
				}
			}
			else if(programs_running[i + 1] == NULL && programs_running[i - 1] != NULL) {
				freespace.combine(programs_running[i]->size, free_nodes + 1);
				usedspace.removeNode(programs_running[i]->size);
				pages = programs_running[i]->size;
				programs_running[i] = NULL;
				for(int j = i; j < total_nodes - 1; ++j) {
					programs_running[j] = programs_running[j + 1];
				}
			}
			else if(programs_running[i + 1] == NULL && programs_running[i - 1] == NULL) {
				int value = freespace.combine(programs_running[i]->size, free_nodes);
				freespace.combine(value, free_nodes + 1);
				freespace.removeNode(value);
				usedspace.removeNode(programs_running[i]->size);
				pages = programs_running[i]->size;
				programs_running[i] = NULL;
				for(int j = i; j < total_nodes - 2; ++j) {
					programs_running[j] = programs_running[j + 2];
				} 
			}
			else {
				freespace.addSpecific(programs_running[i]->size, free_nodes + 1);
				usedspace.removeNode(programs_running[i]->size);
				pages = programs_running[i]->size;
				programs_running[i] = NULL;
			}
			break;
		}
	}
	cout << endl << "Program  " << name << " successfully killed, " << pages << " page(s) reclaimed." <<endl;
}
// Method that kills programs while running best fit algorithm. It runs pretty much exact same as
// worstKill().
void bestKill() {
	string name;
	int pages;

	cout << "Program name - ";
	cin >> name;

	bool doesNotExist = true;
	for(int i = 0; i < programs_running.size(); ++i) {
		if(programs_running[i] == NULL) {
			continue;
		}
		else if(programs_running[i]->program_name == name) {
			doesNotExist = false;
			break;
		}
	}
	if(doesNotExist == true) {
		cout << endl << "Program " << name << " is not running." << endl;
		return;
	}

	int free_nodes = 0;
	int total_nodes = freespace.nodeCount() + usedspace.nodeCount();

	for(int i = 0; i < total_nodes; ++i) {
		if(programs_running[i] == NULL) {
			++free_nodes;
		}
		else if(programs_running[i]->program_name == name) {
			if(i + 1 == total_nodes && programs_running[i - 1] != NULL) {
				freespace.addSpecific(programs_running[i]->size, free_nodes + 1);
				usedspace.removeNode(programs_running[i]->size);
				pages = programs_running[i]->size;
				programs_running[i] = NULL;
			}
			else if(programs_running[i - 1] == NULL && programs_running[i + 1] != NULL) {
				freespace.combine(programs_running[i]->size, free_nodes);
				usedspace.removeNode(programs_running[i]->size);
				pages = programs_running[i]->size;
				programs_running[i] = NULL;
				for(int j = i; j < total_nodes - 1; ++j) {
					programs_running[j] = programs_running[j + 1];
				}
			}
			else if(programs_running[i + 1] == NULL && programs_running[i - 1] != NULL) {
				freespace.combine(programs_running[i]->size, free_nodes + 1);
				usedspace.removeNode(programs_running[i]->size);
				pages = programs_running[i]->size;
				programs_running[i] = NULL;
				for(int j = i; j < total_nodes - 1; ++j) {
					programs_running[j] = programs_running[j + 1];
				}
			}
			else if(programs_running[i + 1] == NULL && programs_running[i - 1] == NULL) {
				int value = freespace.combine(programs_running[i]->size, free_nodes);
				freespace.combine(value, free_nodes + 1);
				freespace.removeNode(value);
				usedspace.removeNode(programs_running[i]->size);
				pages = programs_running[i]->size;
				programs_running[i] = NULL;
				for(int j = i; j < total_nodes - 2; ++j) {
					programs_running[j] = programs_running[j + 2];
				} 
			}
			else {
				freespace.addSpecific(programs_running[i]->size, free_nodes + 1);
				usedspace.removeNode(programs_running[i]->size);
				pages = programs_running[i]->size;
				programs_running[i] = NULL;
			}
			break;
		}
	}

	cout << endl << "Program  " << name << " successfully killed, " << pages << " page(s) reclaimed." <<endl;
}
// Method that returns the total # of fragments of free memory by counting total number of free nodes in
// free space linked list.
void frag() {
	int fragments = freespace.nodeCount();

	cout << endl << "There are " << fragments << " fragment(s)." << endl;
}

// Method that prints from either freespace linked list or usedspace linked list, depending on the 
// programs_running vector that keeps track of the print order.
void print() {
	int fragments = freespace.nodeCount();
	int total_nodes = freespace.nodeCount() + usedspace.nodeCount();
	int free_nodes_used = 1;
	int used_nodes_used = 0;
	wordIndex* index = new wordIndex();

	cout << endl;

	for(int i = 0; i < total_nodes; ++i) {
		if(programs_running[i] == NULL) {
			freespace.print("Free", free_nodes_used, index);
			++free_nodes_used;
			if(programs_running[i + 1] == NULL) {
				break;
			}
		}
		else {
			++used_nodes_used;
			usedspace.print(programs_running[i]->program_name, used_nodes_used, index);
		}
	}
	index->resetIndex();
}
// Method that when called by user exits from program. 
int exit() {
	is_running = false;
}

// Method that is called if user inputs an invalid choice for a process to run.
void notValid() {
	char wrong;
	cout << "Invalid choice, Please choose another proccess to run.";
	cin.clear();
	cin.ignore();
}

// Method that is called when user starts program using worst fit algorithm. Method contains the
// menu that prints to the screen and takes in the users input for choice of process to run. 
void worst() {
	int choice;
	
	cout << "Using worst fit algorithm" << endl << endl;
	cout << "1. Add program" << endl;
	cout << "2. Kill program" << endl;
	cout << "3. Fragmentation" << endl;
	cout << "4. Print memory" << endl;
	cout << "5. Exit" << endl;

	while(is_running == true) {
		cout << endl << "choice - ";
		cin >> choice;

		if(choice == 1) {
			worstAdd();
		}
		else if(choice == 2) {
			worstKill();
		}
		else if(choice == 3) {
			frag();
		}
		else if(choice == 4) {
			print();
		}
		else if(choice == 5) {
			exit();
		}
		else {
			notValid();
		}
	}
}

// Method that is called when user starts program using best fit algorithm. Method contains the
// menu that prints to the screen and takes in the users input for choice of process to run.
void best() {
	int choice;
	
	cout << "Using best fit algorithm" << endl << endl;
	cout << "1. Add program" << endl;
	cout << "2. Kill program" << endl;
	cout << "3. Fragmentation" << endl;
	cout << "4. Print memory" << endl;
	cout << "5. Exit" << endl;

	while(is_running == true) {
		cout << endl << "choice - ";
		cin >> choice;

		if(choice == 1) {
			bestAdd();
		}
		else if(choice == 2) {
			bestKill();
		}
		else if(choice == 3) {
			frag();
		}
		else if(choice == 4) {
			print();
		}
		else if(choice == 5) {
			exit();
		}
		else {
			notValid();
		}
	}
}

int main(int argc, char* argv[]) {
	string algorithm = string(argv[1]);
	if(algorithm == "worst") {
		worst();
	}
	else if(algorithm == "best") {
		best();
	}
	else {
		cout << "Please define which alogrithm to use before running program: worst or best" << endl;
	}
	return 0;
}
