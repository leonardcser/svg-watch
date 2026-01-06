# SVG Watch

A qt native application to preview SVG files live

## Installation

First make sure you make qt6 installed on your machine.

Clone the repository and build with the following commands:

```bash
cmake -B build
cmake --build build
```

To install the application:

```bash
cmake --build build --target install
```

Or set a custom prefix:

```bash
cmake -B build -DCMAKE_INSTALL_PREFIX=/usr/local
cmake --build build --target install
```

## Usage

To run the application pass as a command line argument the path to the SVG file:

```bash
./svgwatch <file.svg>
```
