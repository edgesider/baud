# baud
Set non-standard baud rate for tty on linux.

## Build

Just `make` and then you get `./baud`.

## Usage

```bash
# get baud rate
./baud /dev/ttyUSB0

# set baud rate (both ispeed and ospeed)
./baud /dev/ttyUSB0 117200

# set baud rate
./baud /dev/ttyUSB0 117200 115200
```
