//Class used to store an individual news story thing.
//Should all be pretty selve explanitory.

#ifndef _NEWS_ITEM_
#define _NEWS_ITEM_

#include <string>
using namespace std;

class newsItem {
	public:
		newsItem(string _title, string _description, double _latitude, double _longitude);
		~newsItem();

		string getTitle();
		string getDescription();		
		double getLongitude();
		double getLatitude();
		//bool hasPicture();
		//image(?) getPicture();

	private:
		string title;
		string description;
		double lng;
		double lat;

	};

#endif
