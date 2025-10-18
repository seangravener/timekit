# Time Display Project

A collection of C programs for time-related utilities.

## Programs

- **pomodoro**: GTK-based Pomodoro timer application
- **time_display**: Console program that prints current local time
- **time_gtk**: GTK-based live time display that updates every second

## Building

Ensure you have GTK+3.0 development libraries installed:

```bash
# Ubuntu/Debian
sudo apt-get install libgtk-3-dev build-essential pkg-config

# macOS (with Homebrew)
brew install gtk+3 pkg-config
```

Build all programs:

```bash
make
```

Build individual programs:

```bash
make pomodoro
make time_display
make time_gtk
```

Executables will be placed in the `bin/` directory.

## Running

```bash
./bin/pomodoro      # Launch Pomodoro timer GUI
./bin/time_display  # Print current time to console
./bin/time_gtk      # Launch live time display GUI
```

## Cleaning

Remove all built executables:

```bash
make clean
```