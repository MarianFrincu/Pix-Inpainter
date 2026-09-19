FROM ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y --no-install-recommends \
        build-essential \
        cmake \
        ninja-build \
        qt6-base-dev \
        qt6-base-dev-tools \
        libgl1-mesa-dev \
        libxcb-cursor0 \
        libxcb-xinerama0 \
        libgl1 \
        libegl1 \
        fonts-dejavu-core \
        dbus-x11 \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /src
COPY . .

RUN cmake -G Ninja -S . -B /build -DCMAKE_BUILD_TYPE=Release \
    && cmake --build /build

ENV QT_QPA_PLATFORM=xcb
CMD ["/build/PixInpainter"]
