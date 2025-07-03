# ---- Этап 1: Build ----
FROM --platform=linux/amd64 ubuntu:22.04 AS build

RUN apt-get update && apt-get install -y \
    build-essential \
    g++-11 \
    curl \
    git \
    cmake \
    ninja-build \
    pkg-config \
    libssl-dev \
    ca-certificates \
    zip \
    unzip \
    tar \
    bison \
    flex \
    linux-libc-dev \
    autoconf \
    automake \
    libtool \
    && apt-get clean && rm -rf /var/lib/apt/lists/*



WORKDIR /app

# Клонирование и билд vcpkg
RUN git clone https://github.com/microsoft/vcpkg.git vcpkg
WORKDIR /app/vcpkg
RUN ./bootstrap-vcpkg.sh

# Копирование только исходников и CMakeLists.txt
WORKDIR /app
COPY . .

# Установка зависимостей с помощью vcpkg
RUN /app/vcpkg/vcpkg install fmt nlohmann-json curl libpq

# Сборка проекта
WORKDIR /app/build
RUN cmake .. -DCMAKE_TOOLCHAIN_FILE=/app/vcpkg/scripts/buildsystems/vcpkg.cmake -DCMAKE_BUILD_TYPE=Release
RUN make

# v1
# # ---- Этап 2: Runtime ----
# FROM --platform=linux/amd64 ubuntu:22.04

# # Установка минимальных зависимостей
# RUN apt-get update && apt-get install -y \
#     libssl3 \
#     ca-certificates && \
#     apt-get clean && rm -rf /var/lib/apt/lists/*

# WORKDIR /app

# # Копирование бинарника из билд-этапа
# COPY --from=build /app/build/TickerPulseWorker .

# CMD ["./TickerPulseWorker"]

# v2
# ---- Этап 2: Runtime ----
FROM --platform=linux/amd64 ubuntu:22.04

# Установка минимальных зависимостей
RUN apt-get update && apt-get install -y \
    libssl3 \
    ca-certificates && \
    apt-get clean && rm -rf /var/lib/apt/lists/*

WORKDIR /app

# Копирование папки resources из билд-этапа
COPY --from=build /app/resources ./resources

# Копирование бинарника из билд-этапа в папку build
RUN mkdir build
COPY --from=build /app/build/TickerPulseWorker ./build/TickerPulseWorker

CMD ["./app/build/TickerPulseWorker"]