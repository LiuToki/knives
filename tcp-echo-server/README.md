<h1 align="center">tcp-echo-server</h1>

<div align="center">
    <strong>The server only echo.</strong>
</div>

<br/>

<div align="center">
    <sub>
        This template using C++ with vcpkg.
    </sub>
</div>

<br/>

## Table of Contents
- [Installation](#installation)
- [Features](#features)
- [Usage](#usage)
- [Author](#author)
- [LICENSE](#license)

## Installation
    $ git clone -b tcp-echo-server https://github.com/LiuToki/knives.git

or

    $ wget https://github.com/LiuToki/knives/archive/refs/heads/tcp-echo-server.zip
    $ unzip tcp-echo-server.zip

### Initialize
```
$ git submodule init
```

### Build
```
$ mkdir build
$ cd build
$ cmake -G Ninja ..
$ ninja
```

## Features
- Echo the message to stream from tcp/ip connection.

## Usage
```
$ tcp-echo-server
```

## Author
[LiuToki](https://github.com/LiuToki)

## License
[MIT](./LICENCE)