# Marvel Super Heroes Recomp — changelog

## 0.1.0-beta.1

First build. The 68000 and Z80 programs of *Marvel Super Heroes* (Euro 951024) are translated to C++
ahead of time and run on cps2recomp's own machine. The launcher, the in-game menu, save states,
run-ahead, the display options and every control are remappable.

This beta is a translation and toolkit release: the game runs from the translated code, with the
interpreter covering any path `make explore` did not reach. The game's own optional features
(free play, cheats, training aids) are still to be built from addresses found on the running board;
see [ROADMAP.md](ROADMAP.md) and [games/msh/PROGRESS.md](games/msh/PROGRESS.md).
