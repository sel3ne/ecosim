# ecosim
Eco sim game


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
