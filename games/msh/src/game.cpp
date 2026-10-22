// SPDX-License-Identifier: GPL-3.0-only
//
// Marvel Super Heroes: what the game's own sources add to the port (see PROGRESS.md). The features are
// built here from the game's own memory (msh.h) and hooks into the game's code (runtime/lift.h).
#include "game.h"

namespace cps2recomp {

// (the game's own parts; each registers its features and state and is called from around a frame)
void RegisterCheats(); // cheats.cpp
void CheatsFrame(Machine &machine);
void CheatsState(Archive &ar);

namespace {
    // (the cheats see what a frame did, after it)
    void Frame(Machine &machine) {
        CheatsFrame(machine);
    }

    void State(Archive &ar) {
        CheatsState(ar);
    }
} // namespace

void RegisterGame() {
    RegisterCheats();
    TheGame().frame = Frame;
    TheGame().state = State;
}

} // namespace cps2recomp
