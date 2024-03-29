﻿# function_printer

西电编译原理大作业——函数绘图语言解释器，使用libSDL2绘图

## Usage

`Usage: function_printer [-h] [-W Width] [-H High] <FILE>`

## build

推荐使用`Visual studio 2022`进行构建，当然不用也无所谓

### Windows

#### Use Cmake

Do the following in the project root directory：

1. `mkdir build`

2. `cd build`

3. `cmake ..`

4. Open `function_printer.sln` in visual studio then build it.

#### Use Visual studio

Open the project folder in visual studio and build.

#### Notice

1. The output files is located in the `/bin` directory

2. Before running the program, please **ensure** that it is located in the same directory as `bin/SDL2.dll`

### Linux

#### Use Cmake

1. install `libSDL2`：

```shell
sudo apt install libsdl2-dev
sudo apt install libsdl2-image-dev
sudo apt install libsdl2-mixer-dev
sudo apt install libsdl2-ttf-dev
sudo apt install libsdl2-gfx-dev
```
Do the following in the project root directory：

2. `mkdir build`

3. `cd build`

4. `cmake ..`

5. `make`

#### Notice

1. The output files is located in the `/bin` directory

2. If your system cannot use package manager (like `apt`) to install `libsld2`, try building and installing from the source code: [Downdlow Source Code](https://github.com/libsdl-org/SDL/releases/tag/release-2.24.1).

## Test Case

test.funcs

```
rot is 0;
origin is (50, 400);
scale is (2, 1);
for T from 0 to 300 step 0.01 draw (t, 0);
for T from 0 to 300 step 0.01 draw (0, -t);
for T from 0 to 300 step 0.01 draw (t, -t);
scale is (2, 0.1);
for T from 0 to 55 step 0.01 draw (t, -(t*t));
scale is (10, 5);
for T from 0 to 60 step 0.01 draw (t, -sqrt(t));
scale is (20, 0.1);
for T from 0 to 8 step 0.01 draw (t, -exp(t));
scale is (2, 20);
for T from 0 to 300 step 0.01 draw (t, -ln(t));
```

`function_printer ./test.funcs`

1. Windows

![test_result_win](./test_result_win.jpg)

2. Linux

![test_result_linux](./test_result_linux.png)





