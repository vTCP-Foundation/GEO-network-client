.PHONY: build-debug build-release


# Builds debug version of vtcpd binary (suitable for internal testing).
build-debug:
	mkdir -p build-debug
	cd build-debug && cmake -B ./ -S .. -DCMAKE_BUILD_TYPE=Debug && cmake --build . -j 8


# Builds release version of vtcpd binary.
build-release:
	mkdir -p build-release
	cd build-release && rm -f CMakeCache.txt && cmake -B ./ -S .. -DCMAKE_BUILD_TYPE=Release && cmake --build . -j 8


# Builds docker image with vtcpd binary for internal testing.
# See https://github.com/vTCP-Foundation/vtcpd-test-suite for the details.
#
# WARN: Ensure that proper build is done before running this command
# (`build-debug` or `build-release` depending on testing purposes)
docker-build-test-manjaro:
	docker build --target final-manjaro \
		--build-arg LISTEN_ADDRESS=0.0.0.0 \
		--build-arg LISTEN_PORT=2000 \
		--build-arg EQUIVALENTS_REGISTRY=eth \
		--build-arg MAX_HOPS=5 \
		-t vtcpd-test:manjaro .


# Builds docker image with vtcpd binary for internal testing.
# See https://github.com/vTCP-Foundation/vtcpd-test-suite for the details.
#
# WARN: Ensure that proper build is done before running this command
# (`build-debug` or `build-release` depending on testing purposes)
docker-build-test-ubuntu:
	docker build --target final-ubuntu \
		--build-arg LISTEN_ADDRESS=0.0.0.0 \
		--build-arg LISTEN_PORT=2000 \
		--build-arg EQUIVALENTS_REGISTRY=eth \
		--build-arg MAX_HOPS=5 \
		-t vtcpd-test:ubuntu .
