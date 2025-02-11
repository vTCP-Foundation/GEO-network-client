.PHONY: build-debug build-release

build-debug:
	mkdir -p build-debug
	cd build-debug && cmake -B ./ -S .. -DCMAKE_BUILD_TYPE=Debug && cmake --build . -j 8

build-release:
	mkdir -p build-release
	cd build-release && rm -f CMakeCache.txt && cmake -B ./ -S .. -DCMAKE_BUILD_TYPE=Release && cmake --build . -j 8

