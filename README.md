# ecosim
Economy simulation game: There are different resources available (Food, Gold, etc). Humans carry them around from the buildings. The ressources can only be collected at the right places, according to the map.

![Screenshot_map](https://user-images.githubusercontent.com/56499347/99402390-965b7800-28e9-11eb-82e5-a219ac40c117.png)

On the map the player can arrange the buildings (like the lighthouse in the following image):

![Screenshot from 2020-11-17 15-33-01](https://user-images.githubusercontent.com/56499347/99403379-afb0f400-28ea-11eb-8c10-d4f08bd3a6cd.png)

There are buildings like the farmhouse which come automaticly with humans:



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
