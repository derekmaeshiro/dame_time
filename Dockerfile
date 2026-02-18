# 1. Start with a basic Linux system (Debian is a common choice)
FROM debian:bullseye-slim

# 2. Install the tools we need to download things
# We run this ONCE when building the image.
RUN apt-get update && apt-get install -y \
    wget \
    git \
    make \
    bzip2 \
    xz-utils

# 3. Install the ARM Compiler
# We download and unzip it to the /opt folder
WORKDIR /opt
RUN wget -qO- https://developer.arm.com/-/media/Files/downloads/gnu/12.2.rel1/binrel/arm-gnu-toolchain-12.2.rel1-x86_64-arm-none-eabi.tar.xz | tar -xJ

# 4. Add the compiler to the "PATH"
# This makes sure we can type 'arm-none-eabi-gcc' from anywhere
ENV PATH="/opt/arm-gnu-toolchain-12.2.rel1-x86_64-arm-none-eabi/bin:${PATH}"

# 5. Clone the massive STM32 Library
# This is the "slow" part you want to avoid repeating.
# We do it here, so it gets baked into the image forever.
RUN git clone --depth 1 --recursive https://github.com/STMicroelectronics/STM32CubeF4.git /opt/stm32-libs

# 6. Set the Environment Variable
# So your Makefile knows where the libs are
ENV STM_PATH=/opt/stm32-libs

# 7. Set the starting folder
WORKDIR /project