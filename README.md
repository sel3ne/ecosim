# ecosim
Economy simulation game: There are different resources available (Food, Gold, etc). Humans carry them around from the buildings. The ressources can only be collected at the right places, according to the map.

![alt text](https://github.com/sel3ne/ecosim/data/Screenshot_map.png?raw=true)


## Arch Linux

### Building and Running

Install the `sfml` package.

 ```
 ./compile_and_run.sh
 ```

### Testing

Install the `sfml`, `gtest` and `gmock` packages.

```
./run_tests.sh
```

### Sublime Setup

* Install the `clang` package from the pacman repositories.
* Install the `Clang Format` package in Sublime.
* Under `Preferences` -> `Package Settings` -> `Clang Format` -> `Settings - User` set the config:

 ```
{
	"style": "Google",
	"format_on_save": true
}
 ```
