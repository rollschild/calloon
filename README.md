# Calloon, the Deflate Algorithm implemented in C

## Build

- `cmake . -B build`
- `cmake --build build`

## Run

- To view the huffman codes, provide a string of `<char>:<frequency>` pairs delimited by `;`, eg: `./build/src/calloon "a:5;b:9;c:12;d:13;e:16;f:45"`
- Note that you need to wrap the argument with `""`
