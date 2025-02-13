###################################################################################
# Common base stage with shared environment variables
FROM scratch AS base-common

# Create startup script that will be copied to final images
COPY - /docker-entrypoint.sh


###################################################################################
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


###################################################################################
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


###################################################################################
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
    chmod -R 777 /vtcpd && \
    chmod +x /vtcpd/vtcpd

# Define build arguments and convert them to runtime environment variables
ARG LISTEN_ADDRESS=127.0.0.1
ARG LISTEN_PORT=2000
ARG EQUIVALENTS_REGISTRY=eth
ARG MAX_HOPS=5

ENV LISTEN_ADDRESS=${LISTEN_ADDRESS}
ENV LISTEN_PORT=${LISTEN_PORT}
ENV EQUIVALENTS_REGISTRY=${EQUIVALENTS_REGISTRY}
ENV MAX_HOPS=${MAX_HOPS}

# Create startup script that uses runtime environment variables
RUN echo '#!/bin/bash\n\
if [ -n "$LISTEN_ADDRESS" ] && [ -n "$LISTEN_PORT" ]; then\n\
    sed -i "s/\"address\":\"[^\"]*\"/\"address\":\"$LISTEN_ADDRESS:$LISTEN_PORT\"/g" /vtcpd/conf.json\n\
fi\n\
if [ -n "$EQUIVALENTS_REGISTRY" ]; then\n\
    sed -i "s/\"equivalents_registry_address\":\"[^\"]*\"/\"equivalents_registry_address\":\"$EQUIVALENTS_REGISTRY\"/g" /vtcpd/conf.json\n\
fi\n\
if [ -n "$MAX_HOPS" ]; then\n\
    sed -i "s/\"max_hops_count\":[[:space:]]*[0-9][0-9]*/\"max_hops_count\": $MAX_HOPS/g" /vtcpd/conf.json\n\
fi\n\
exec "$@"' > /docker-entrypoint.sh && \
chmod +x /docker-entrypoint.sh

ENTRYPOINT ["/docker-entrypoint.sh"]
CMD ["/vtcpd/vtcpd"]


###################################################################################
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
    chmod -R 777 /vtcpd && \
    chmod +x /vtcpd/vtcpd

# Define build arguments and convert them to runtime environment variables
ARG LISTEN_ADDRESS=127.0.0.1
ARG LISTEN_PORT=2000
ARG EQUIVALENTS_REGISTRY=eth
ARG MAX_HOPS=5

ENV LISTEN_ADDRESS=${LISTEN_ADDRESS}
ENV LISTEN_PORT=${LISTEN_PORT}
ENV EQUIVALENTS_REGISTRY=${EQUIVALENTS_REGISTRY}
ENV MAX_HOPS=${MAX_HOPS}

# Create startup script that uses runtime environment variables
RUN echo '#!/bin/bash\n\
if [ -n "$LISTEN_ADDRESS" ] && [ -n "$LISTEN_PORT" ]; then\n\
    sed -i "s/\"address\":\"[^\"]*\"/\"address\":\"$LISTEN_ADDRESS:$LISTEN_PORT\"/g" /vtcpd/conf.json\n\
fi\n\
if [ -n "$EQUIVALENTS_REGISTRY" ]; then\n\
    sed -i "s/\"equivalents_registry_address\":\"[^\"]*\"/\"equivalents_registry_address\":\"$EQUIVALENTS_REGISTRY\"/g" /vtcpd/conf.json\n\
fi\n\
if [ -n "$MAX_HOPS" ]; then\n\
    sed -i "s/\"max_hops_count\":[[:space:]]*[0-9][0-9]*/\"max_hops_count\": $MAX_HOPS/g" /vtcpd/conf.json\n\
fi\n\
exec "$@"' > /docker-entrypoint.sh && \
chmod +x /docker-entrypoint.sh

ENTRYPOINT ["/docker-entrypoint.sh"]
CMD ["/vtcpd/vtcpd"]
