#include "newsItem.h"

newsItem::newsItem(string _title, string _description, double _latitude, double _longitude) {
	title = _title;
	description = _description;	
	lng = _longitude;
	lat = _latitude;
	}

newsItem::~newsItem() {}

string newsItem::getTitle() {
	return title; 
	}

string newsItem::getDescription() {
	return description; 
	}

double newsItem::getLongitude() {
	return lng;
	}

double newsItem::getLatitude() {
	return lat;
	}
