# tiny-embed

## Installation

```bash
$ git clone https://github.com/handicraftsman/tiny-embed
$ cd tiny-embed
$ mkdir build
$ cd build
$ cmake -DCMAKE_BUILD_TYPE=Release ..
$ make
$ sudo make install
```

## Usage

tiny-embed uses newline-separated symbol-file pairs as shown in tiny-embed.txt.

```
$ tiny-embed --help
Usage: ./tiny-embed [OPTIONS]

Available options: 
  -h        --help              Show this message
  -c FILE   --config-file FILE  Config file       [./tiny-embed.txt]
  -d LEVEL  --working-dir DIR   Working directory [./]
  -o FILE   --output      FILE  Output .c file    [./output.c]
  -H FILE   --header      FILE  Output .h file    [./output.h]
```

Packed data can be accessed by symbol names defined in the config file.
