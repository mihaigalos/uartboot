:construction: **Work in Progress** :construction:

# uartboot

![CI](https://github.com/mihaigalos/uartboot/workflows/CI/badge.svg) [![codecov](https://codecov.io/gh/mihaigalos/uartboot/branch/main/graph/badge.svg?token=K1KVKCKIRG)](https://codecov.io/gh/mihaigalos/uartboot) [![license](https://img.shields.io/badge/license-GPLv3-brightgreen.svg)](LICENSE) [![LoC](https://tokei.rs/b1/github/mihaigalos/uartboot)](https://github.com/Aaronepower/tokei)

An UART bootloader for Arduino/AVR.



### Build and Test

First, get the sources:
```
git clone https://github.com/mihaigalos/uartboot.git && cd uartboot
```

#### Baremetal

Building and testing is managed by `bazel`, including dependencies to other repos such as [containers](https://github.com/mihaigalos/containers) (transparent to the end-user).

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
sudo bazel-bin/host/host_/host  test/host/demo_blink.hex test/host/demo_metadata.yaml
```

You can use CuteCom on Linux for debugging operations with the serial port.