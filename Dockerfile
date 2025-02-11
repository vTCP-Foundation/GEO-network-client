# Common base stage with shared environment variables
FROM scratch AS base-common

# Manjaro Linux runtime environment
FROM manjarolinux/base AS runtime-manjaro
RUN pacman -Syu --noconfirm && \
    pacman -S --noconfirm \
    boost \
    boost-libs \
    libsodium \
    sqlite && \
    # Debug library locations
    echo "Library locations:" && \
    ls -la /usr/lib/libboost_* && \
    ls -la /usr/lib/libsodium* && \
    echo "Library dependencies:" && \
    ldd /vtcpd/vtcpd || true

# Create a non-root user for running the daemon
RUN useradd -r -s /usr/bin/nologin vtcpd

# Ubuntu runtime environment
FROM ubuntu:22.04 AS runtime-ubuntu
RUN apt-get update && \
    apt-get install -y --no-install-recommends \
    libboost-system1.74.0 \
    libboost-filesystem1.74.0 \
    libboost-program-options1.74.0 \
    libsodium23 \
    sqlite3 && \
    rm -rf /var/lib/apt/lists/* && \
    useradd -r -s /usr/sbin/nologin vtcpd

# Final stage for Manjaro Linux
FROM runtime-manjaro AS final-manjaro
COPY --from=base-common / /

# Create vtcpd directory and set permissions
RUN mkdir -p /vtcpd
WORKDIR /vtcpd

# Copy pre-built binaries and config from host
COPY ./build-debug/bin/vtcpd ./build-debug/bin/conf.json /vtcpd/

# Make fifo and io directories with proper permissions
RUN mkdir -p /vtcpd/fifo /vtcpd/io && \
    chmod -R 777 /vtcpd

# Set proper permissions for executables
RUN chmod +x /vtcpd/vtcpd

# Define build arguments
ARG LISTEN_ADDRESS=127.0.0.1
ARG LISTEN_PORT=2000
ARG EQUIVALENTS_REGISTRY=eth
ARG MAX_HOPS=5

# Configure the application
RUN sed -i 's|\("address":[[:space:]]*"\)[^"]*\(".*\)|\1'"${LISTEN_ADDRESS}:${LISTEN_PORT}"'\2|' /vtcpd/conf.json && \
    sed -i 's|\("equivalents_registry_address":[[:space:]]*"\)[^"]*\(".*\)|\1'"${EQUIVALENTS_REGISTRY}"'\2|' /vtcpd/conf.json && \
    sed -i 's|\("max_hops_count":[[:space:]]*\)[0-9][0-9]*\(.*\)|\1'"${MAX_HOPS}"'\2|' /vtcpd/conf.json

CMD ["/vtcpd/vtcpd"]

# Final stage for Ubuntu
FROM runtime-ubuntu AS final-ubuntu
COPY --from=base-common / /

# Create vtcpd directory and set permissions
RUN mkdir -p /vtcpd
WORKDIR /vtcpd

# Copy pre-built binaries and config from host
COPY ./build-debug/bin/vtcpd ./build-debug/bin/conf.json /vtcpd/

# Make fifo and io directories with proper permissions
RUN mkdir -p /vtcpd/fifo /vtcpd/io && \
    chmod -R 777 /vtcpd

# Set proper permissions for executables
RUN chmod +x /vtcpd/vtcpd

# Configure the application
RUN sed -i "s/\"address\":\"127.0.0.1:2000\"/\"address\":\"$LISTEN_ADDRESS:$LISTEN_PORT\"/g" /vtcpd/conf.json && \
    sed -i "s/\"equivalents_registry_address\":\"eth\"/\"equivalents_registry_address\":\"$EQUIVALENTS_REGISTRY\"/g" /vtcpd/conf.json && \
    sed -i "s/\"max_hops_count\": 2/\"max_hops_count\": $MAX_HOPS/g" /vtcpd/conf.json

CMD ["/vtcpd/vtcpd"]
