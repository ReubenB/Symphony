#include "searchTree.h"
#include <utility>
#include <cctype>
#include <cstring>
#include <iostream>

using namespace std;

searchTree::searchTree() {
	isFound =false;
	for (int i = 0; i < 27; i++) children[i] = NULL;
	}

searchTree::searchTree(pair<double, double> value) {
	myValue = value;
	isFound = true;
	for (int i = 0; i < 27; i++) children[i] = NULL;
	}

searchTree::~searchTree() {
	for (int i = 0; i < 27; i++) 
		if (children[i] != NULL) 
			delete children[i];
	}

bool searchTree::isFoundNode() {
	return isFound;
	}

pair<double, double> searchTree::getValue() {
	return myValue;
	}

searchTree* searchTree::find(char c) {
	int index;
	if (c == ' ') {
		index = 0;
		}
			else
		{
		c = tolower(c);
		if (c < 'a' || c > 'z') return NULL;
		index = c - 'a' + 1;
		}

	return children[index];
	}

void searchTree::insert(const char* name, pair<double, double> value) {
	if (strlen(name) == 0) return;

	char c = name[0];
	int index;
	if (c == ' ') {
		index = 0;
		}
			else
		{
		c = tolower(c);
		if (c < 'a' || c > 'z') return;
		index = c - 'a' + 1;
		}
	
	if (strlen(name) == 1) {
		if (children[index] == NULL) {
			children[index] = new searchTree(value);
			}
		return;
		}

	if (children[index] == NULL) children[index] = new searchTree();
	children[index]->insert(name+1, value);
	}
