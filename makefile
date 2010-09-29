example : example.o newsItem.o newsReader.o searchTree.o
	g++ example.o newsItem.o newsReader.o searchTree.o -o example `curl-config --cflags --libs` `pkg-config libxml++-2.6 --cflags --libs`

example.o : example.cpp
	g++ -c example.cpp -o example.o

newsItem.o : newsItem.cpp
	g++ -c newsItem.cpp -o newsItem.o

newsReader.o : newsReader.cpp
	g++ -c newsReader.cpp -o newsReader.o `pkg-config libxml++-2.6 --cflags --libs`

searchTree.o : searchTree.cpp
	g++ -c searchTree.cpp -o searchTree.o


