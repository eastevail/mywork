#!/bin/sh
	rm avilib libevent-master log4cpp tinyxml2 -rf
	cd build
	rm * -rf
	cd -
	tar -xvf thirdlib.tar.gz
	cd libevent-master
	./build.sh
	cd -
	cd tinyxml2
	./build.sh
	cd -
	cd avilib
	./build.sh
	cd -
	rm avilib libevent-master log4cpp tinyxml2 -rf
