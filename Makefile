INSTALL_PATH?=/usr/local

all:
	g++ *.cpp -std=c++11 `imlib2-config --cflags` `imlib2-config --libs` -o imej

install:
	mv imej "${INSTALL_PATH}/bin"

