#include <iostream>
#include <list>
#include "newsReader.h"
#include "newsItem.h"

int main(int argc, char* argv[]) {
	newsReader reader;
	
	reader.addFeed("http://www.stuff.co.nz/rss/");
	reader.addFeed("http://www.stuff.co.nz/rss/world/");
	reader.addFeed("http://www.stuff.co.nz/rss/world/australia/");
	reader.addFeed("http://www.stuff.co.nz/rss/world/americas/");
	reader.addFeed("http://www.stuff.co.nz/rss/world/europe/");
	reader.addFeed("http://www.stuff.co.nz/rss/world/middle-east/");
	reader.addFeed("http://www.stuff.co.nz/rss/world/asia/");
	reader.addFeed("http://www.stuff.co.nz/rss/world/africa/");
	reader.addFeed("http://www.stuff.co.nz/rss/world/south-pacific/");
	reader.addFeed("http://www.stuff.co.nz/rss/entertainment/");
	reader.addFeed("http://www.stuff.co.nz/rss/national/");

	cout << "This could take a minute... " << endl;

	reader.downloadFromFeeds();

	list<newsItem*>* news = reader.getAllNewsItems();
	list<newsItem*>::iterator newsIterator;

	cout.precision(5);

	for(newsIterator=news->begin(); newsIterator!= news->end(); newsIterator++) {
		cout << "\n---> " << (*newsIterator)->getTitle() << '\n' << (*newsIterator)->getDescription();
		cout << "\nLocation: " << (*newsIterator)->getLatitude() << " by " << (*newsIterator)->getLongitude() << endl;		
		}

	cout << "\n" << reader.getNewsCount() << " news articles found. " << endl;

	delete news;
	
	return 0;	
	}
