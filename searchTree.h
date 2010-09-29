//Pay no attention to this class, it is just used by the news reader class.

#ifndef _SEARCH_TREE_
#define _SEARCH_TREE_

#include <utility>

using namespace std;

class searchTree {
	public:
		searchTree(pair <double, double> value);
		searchTree();
		~searchTree();

		bool isFoundNode();
		pair <double, double> getValue();
		searchTree* find(char str);
		void insert(const char* str, pair<double, double>);

	private:
		pair<double, double> myValue;
		searchTree* children[27];
		bool isFound;

	};

#endif
