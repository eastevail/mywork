#!/bin/sh
	rm freetype-2.4.0 DirectFB-1.4.0 LiTE-0.8.10 fonts -rf
	cd build
	rm * -rf
	cd -
	tar -xvf depend.tar.gz
	cd freetype-2.4.0
	./build.sh
	cd -
	cd DirectFB-1.4.0
	./build.sh
	cd -
	cd LiTE-0.8.10
	./build.sh
	cd - 
	cp /usr/local/arm_linux_4.2/arm-none-linux-gnueabi/lib/librt* ./build/lib/ -rf
	cp /usr/local/arm_linux_4.2/arm-none-linux-gnueabi/lib/libstdc++* ./build/lib/ -rf
	cp fonts ./build/share/ -rf
	rm freetype-2.4.0 DirectFB-1.4.0 LiTE-0.8.10 fonts -rf
