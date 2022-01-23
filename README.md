embaland
========

Embaland is yet another rendering library that uses Vulkan API.

![Build Status](https://github.com/souryogurt/embaland/actions/workflows/build.yml/badge.svg)
[![Quality Status](https://sonarcloud.io/api/project_badges/measure?project=souryogurt_embaland&metric=alert_status)](https://sonarcloud.io/dashboard?id=souryogurt_embaland)

Features
--------

- Minimal dependencies
- Vulkan rendering

Installation
------------

Install embaland from sources, by running:

```sh
apt install gcc autoconf libtool libvulkan-dev
apt install libglfw3-dev # optionally for samples
git clone https://github.com/souryogurt/embaland.git
cd embaland
autoreconf -if
mkdir build
cd build
../configure
make
make install
```

Development
-----------

To build embaland for development, please use these flags:
```sh
mkdir debug
cd debug
CFLAGS="-Wall -Wextra --std=gnu18 -pedantic -Og -g -fno-inline -fsanitize=address,undefined -fno-common -fsanitize-address-use-after-scope -fno-omit-frame-pointer " ../configure --enable-samples --enable-unit-tests
make
```

Contribute
----------
- Read [How to submit an issue or feature request into tracker](https://github.com/souryogurt/embaland/wiki/How-to-submit-an-issue-or-feature-request)
- Issue Tracker: https://github.com/souryogurt/embaland/issues
- Source Code: https://github.com/souryogurt/embaland

Support
-------

If you are having issues, please let me know.
* Egor Artemov <egor.artemov@gmail.com>
