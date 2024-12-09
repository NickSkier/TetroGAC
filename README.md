# TetroGAC

Simple C++ implementation of Tetris.
Runs in the terminal with a UI in `ncurses`.

## Features (WIP)
- [x] Gravity
- [x] Move left and right
- [x] Soft drop
- [x] Hard drop
- [x] Basic rotation
- [ ] Wall and floor Kicks
- [x] Full lines deletion
- [ ] Game Over
- [ ] Piece preview
- [ ] Score system
- [ ] Levels
- [x] Piece colors
- [ ] Random Generator (7 bag)
- [ ] Hold
- [ ] Ghost piece

## Requirements
- `libncurses5-dev` and `libncursesw5-dev` (for ncurses support)
- `meson`, `ninja-build` (for build system)

## How to run
1. Clone the repository:
```bash
git clone https://github.com/NickSkier/TetroGAC.git
```
2. Enter the cloned directory:
```bash
cd TetroGAC
```
3. Configure the build system with Meson:
```bash
meson setup build
```
4. Compile the project:
```bash
meson compile -C build
```
OR
```bash
cd build && ninja
```
5. Run the game:
```bash
./build/src/tetrogac
```

6. Exit the game using `q` or `Ctrl-C`.

## Controls
- KEY_LEFT/KEY_RIGHT: Move block left/right
- KEY_DOWN: Soft drop
- Space: Hard drop
- KEY_UP: Rotate cloclwise
- E: Rotate counterclockwise
- Q: Quit game
