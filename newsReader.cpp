#include <cstdlib>
#include <cstring>
#include <string>
#include <list>
#include <map>
#include <sstream>
#include <iostream>
#include <fstream>

#include <curl/curl.h>
#include <libxml++/libxml++.h>

#include "newsReader.h"
#include "newsItem.h"
#include "searchTree.h"

#define FEED_COUNT 11
#define COUNTRY_LOCATIONS_INPUT_FILE "CountryLookUp"
#define CITY_LOCATIONS_INPUT_FILE "CityLookUp"

using namespace std;

static char curlErrorBuffer[CURL_ERROR_SIZE];
static string curlBuffer;

static int curlWriter(char *data, size_t size, size_t nmemb, string *buffer) {
	int result = 0;

	if (buffer != NULL) {
    	buffer->append(data, size * nmemb);
		result = size * nmemb;
		}

  	return result;
	}

void initalizeSearchTreeFromFile(searchTree &tree, string filepath) {
	 ifstream filestr(filepath.c_str());

    if (filestr.is_open()) {
        string line;
        char* cstr = new char[256];

        while (!filestr.eof()) {
            getline(filestr, line);
            if (line == "") break;

            string name;
            double longitude;
            double latitude;

            strcpy(cstr, line.c_str());
            char* p = strtok(cstr, ",");

            name = p;
            p = strtok(NULL, ",");
            string s(p);
            istringstream buffer(s);
            buffer >> latitude;
            p = strtok(NULL, ",");
            string s2(p);
            istringstream buffer2(s2);
            buffer2 >> longitude;

            pair <double, double> location (latitude, longitude);

            tree.insert(name.c_str(), location);
            }
        delete [] cstr;
        }
            else
        {
        cout << "Error: could not open country look up file." << endl;
        exit(1);
        }

    filestr.close();
	}

newsReader::newsReader() {
	initalizeSearchTreeFromFile(countryLocations, COUNTRY_LOCATIONS_INPUT_FILE);
	initalizeSearchTreeFromFile(cityLocations, CITY_LOCATIONS_INPUT_FILE);
	}

newsReader::~newsReader() {
	list<newsItem*>::iterator it;
	for (it = newsList.begin(); it!=newsList.end(); it++) {
		if ((*it) != NULL) delete (*it);		
		}	
	}

void newsReader::addFeed(string url) {
	feeds.push_front(url);	
	}

void newsReader::downloadFromFeeds() {
	CURL *curl;
    CURLcode result;

    curl = curl_easy_init();

    if (!curl) {
        cout << "Curl Error: could not initalize" << endl;
        exit (1);
        }

	
	list<string>::iterator feedIterator;
	for (feedIterator = feeds.begin(); feedIterator!=feeds.end(); feedIterator++) {
		curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, curlErrorBuffer);
		curl_easy_setopt(curl, CURLOPT_URL, (*feedIterator).c_str());
		curl_easy_setopt(curl, CURLOPT_HEADER, 0);
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curlWriter);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &curlBuffer);

		result = curl_easy_perform(curl);

      	if (result != CURLE_OK) {
        	cout << "Curl Error: [" << result << "] - " << curlErrorBuffer << endl;
			exit(1);
      		}

 
		xmlpp::DomParser parser;
		parser.parse_memory(curlBuffer);
		xmlpp::Node* rootNode = parser.get_document()->get_root_node();
		xmlpp::Node::NodeList nodes;
		nodes = rootNode->get_children();
		xmlpp::Node::NodeList::iterator it;
		xmlpp::Node* channelNode = NULL;
		for (it = nodes.begin(); it != nodes.end(); it++) {
			if ((*it)->get_name() == "channel") {
				channelNode = *it;
				break;
				}		
			}

		if (!channelNode) continue;

		nodes = channelNode->get_children();
		for (it = nodes.begin(); it != nodes.end(); it++) {
			if ((*it)->get_name() == "item") {
				const xmlpp::Element* nodeElement = dynamic_cast<const xmlpp::Element*>(*it);
				
				string storyTitle = "";
				string storyContent = "";
				int storyId =-1;			
				istringstream converter(nodeElement->get_attribute_value("id"));
				converter >> storyId;

				xmlpp::Node::NodeList children = (*it)->get_children();
				xmlpp::Node::NodeList::iterator it2;
				for (it2 = children.begin(); it2 != children.end(); it2++) {
					if ((*it2)->get_name() == "title") {
						xmlpp::Node* innerNode = (*it2)->get_children().front();
						const xmlpp::TextNode* nodeText = dynamic_cast<const xmlpp::TextNode*>(innerNode);
						if (nodeText) storyTitle = nodeText->get_content();
						}	
					if ((*it2)->get_name() == "description") {
						xmlpp::Node* innerNode = (*it2)->get_children().front();
						const xmlpp::TextNode* nodeText = dynamic_cast<const xmlpp::TextNode*>(innerNode);
						if (nodeText) storyContent = nodeText->get_content();
						}				
					}
				if (storyId == -1) continue;
				if (foundIds[storyId] == true) continue;
				foundIds[storyId] = true;

				bool found = false;
				string str = storyTitle + " " + storyContent;
				int pos = -1;
				do {
					pos ++;
					searchTree* searchNode = cityLocations.find(str[pos]);
					int i = 0;
					
					while (searchNode != NULL && pos+i < str.length()) {
						i++;
						if (searchNode->isFoundNode()) {
							pair<double, double> location = searchNode->getValue();
							newsList.push_front(new newsItem(storyTitle, storyContent, location.first, location.second));
							found = true;
							break;
							}
						
						searchNode = searchNode->find(str[pos+i]);
						}				

					if (found) break;
					}
				while ((pos = str.find(' ', pos)) != string::npos);

				if (found) { continue;}				
				
				pos = -1;
				do {
					pos ++;
					searchTree* searchNode = countryLocations.find(str[pos]);
					int i = 0;
					while (searchNode != NULL && pos+i < str.length()) {
						i++;
						if (searchNode->isFoundNode()) {
							pair<double, double> location = searchNode->getValue();
							newsList.push_front(new newsItem(storyTitle, storyContent, location.first, location.second));
							found = true;
							break;
							}
						
						searchNode = searchNode->find(str[pos+i]);
						}				
					if (found) break;
					}
				while ((pos = str.find(' ', pos)) != string::npos);
				}
			}

		curlBuffer = ""; 
  		}

    curl_easy_cleanup(curl);
	}

int newsReader::getNewsCount() {
	return newsList.size();	
	}

newsItem* newsReader::getNewsItem(int index) {
	list<newsItem*>::iterator it;
	int i = 0;	
	for (it = newsList.begin(); it!=newsList.end(); it++) {
		if (i == index) return *it;
		i++;		
		}
	}

list<newsItem*>* newsReader::getAllNewsItems() {
	return new list<newsItem*>(newsList);
	}
