## BallPit
**BallPit** is a small app based on [Falcon framework](https://github.com/walker2/Falcon).
Spawn balls, move them and watch as they clip into each other.

## Controls
* Left Click - spawn balls
* Right Click - move selected ball
* 1 number key - sets another renderer
* Arrows - sets gravity in arrow direction

## Build excecutable for Linux
### Clone repo from GitHub
```
$ git clone https://github.com/walker2/BallPit
```
### Download and install dependencies
```
$ sudo apt-get install libsdl2-dev
$ sudo apt-get install libsdl2-ttf-dev
$ sudo apt-get install libsdl2-mixer-dev
$ sudo apt-get install libglew-dev
$ sudo apt-get install libglm-dev
```
### Navigate to the cloned folder and run
```
cmake CMakeLists.txt
make
```
