all:
		g++ *.cpp -std=c++11 `imlib2-config --cflags` `imlib2-config --libs` -o imej

