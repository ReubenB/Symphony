//This is the class which will interact with the internet
//and download news items. The news will be stored in a list
//of 'newsItem' classes. 

#ifndef _NEWS_READER_
#define _NEWS_READER_

#include <list>
#include <map>
#include "newsItem.h"
#include "searchTree.h"

class newsReader {
	public:
			newsReader();				//Constructor
			~newsReader();				//Destructor

			void addFeed(string url);	//Adds a rss feed to download from.
										//Must be a stuff.co.nz rss feed.
										//E.g. http://www.stuff.co.nz/rss/world/

			void downloadFromFeeds(); 	//Downloads all the news on the rss feeds which
										//have location information. Will take about a minute
										//or so the first time it is run (it will collect over 
										//100 headlines). Each time after that will be faster 
										//as it caches the stories and will only proccess the new news.

			int getNewsCount();			//Returns how many stories were downloaded.

			newsItem* getNewsItem(int index); //gets a single news item

			list<newsItem*>* getAllNewsItems(); //gets a pointer to a list of all news items 
												//(will be faster to iterate through this list
												//than to use the function above in a for loop).
			

	private:	//Private shet.
			list<string> feeds;
			map<int, bool> foundIds;
			list<newsItem*> newsList;
			searchTree countryLocations;
			searchTree cityLocations;
			
	};

#endif
