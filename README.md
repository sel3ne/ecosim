# ecosim
Eco sim game


## Setup

### Arch Linux

* Install the `sfml` package
* If you use sublime:
  * Install the `clang` package
  * Install the `Clang Format` package in Sublime.
  * Under `Preferences` -> `Package Settings` -> `Clang Format` -> `Settings - User` set the config:

 ```
{
	"style": "Google",
	"format_on_save": true
}
 ```

 ## Building

 ```
 mkdir build
 cd build
 cmake ..
 make
 ```