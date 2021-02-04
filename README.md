:construction: **Work in Progress** :construction:

# uartboot

![CI](https://github.com/mihaigalos/uartboot/workflows/CI/badge.svg) [![codecov](https://codecov.io/gh/mihaigalos/uartboot/branch/main/graph/badge.svg?token=K1KVKCKIRG)](https://codecov.io/gh/mihaigalos/uartboot) [![license](https://img.shields.io/badge/license-GPLv3-brightgreen.svg)](LICENSE) [![LoC](https://tokei.rs/b1/github/mihaigalos/uartboot)](https://github.com/Aaronepower/tokei)

![Last Logs](https://img.shields.io/endpoint?style=social&url=https%3A%2F%2Fapi.cl1p.net%2Fuartboot%2Fclipboard)

An UART bootloader for Arduino/AVR, <1kB.
The implementation uses software UART / bit-bang on any microcontroller data pin.

Usecases:
* in-situ updates via i.e. USB-to-UART bridge.
* remote software updates via i.e. RF-to-UART module.

### Build and Test

First, get the sources:
```
git clone https://github.com/mihaigalos/uartboot.git && cd uartboot
```

#### Baremetal

Building and testing is managed by `bazel`, including dependencies to other repos.

```bash
bazel build //...
bazel test //...

bazel build --config=328p --verbose_failures --color=yes 2>&1 //:uartboot_hex
```

#### Using Docker

```bash
rm bazel-* || true && docker run -it --rm --cap-add SYS_PTRACE -v $(pwd):/src -v /tmp:/tmp/bazel docker.pkg.github.com/mihaigalos/docker/avr-gcc /bin/zsh -c "cd /src && bazel --output_base=/tmp/bazel test //..."
```

### Flashing

Example flashing of `uartboot.hex`:
```
sudo bazel-bin/host/host_/host test/host/demo_blink.hex test/host/demo_metadata.yaml
```

You can use CuteCom on Linux for debugging operations with the serial port.