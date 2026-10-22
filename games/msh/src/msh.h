// SPDX-License-Identifier: GPL-3.0-only
//
// Marvel Super Heroes (Euro 951024): names for what is known of the game's memory. Every address here was
// found by experiment on the running board with the toolkit's own tools (--menu, --movie, --dump-ram,
// --poke, --trace), as PROGRESS.md tells in full. No game data is written down here, only where the game
// keeps it.
#pragma once

#include "game.h"

#include <cstdint>

namespace msh {

// The credits the game is holding, a word (big-endian): the title reads it as "CREDITS n". Found by
// holding it at 5 with --poke and watching the title change to "CREDITS 5" (PROGRESS.md); one coin adds
// one to it, and pressing Start takes it away again.
constexpr uint32_t kCredits = 0xFF4C08;
constexpr uint16_t kCreditsFree = 2; // (enough for both players to start)

// A player's record: the two are 0x400 apart, player 1's first (FF4000, FF4400).
//   +0x191  the health, a byte: 0x90 at a round's start, and the health bar is drawn one pixel per point
//           (a bar 90 pixels wide at 0x5A, 71 at 0x47, 52 at 0x34). 0 outside a round and in the attract
//           show; a beaten player's reads 0xFF.
// Found with --dump-ram over a match: the byte fell exactly as the bar shortened, and the same byte in the
// record 0x400 on did the same for player 2 (PROGRESS.md).
constexpr uint32_t kPlayer0 = 0xFF4000, kPlayerStride = 0x400;
constexpr uint32_t kHealth = 0x191;
constexpr uint8_t kHealthFull = 0x90;
inline constexpr uint32_t Player(int n) {
    return kPlayer0 + static_cast<uint32_t>(n) * kPlayerStride;
}

// The round's clock, a word: its high byte counts down in BCD (0x99 is "99"), the low byte carries the rest
// of the countdown. Found by tracing the whole of work RAM over a match for a byte counting down one step a
// second: FF4808 went 0x97 to 0x96 after 60 frames, and holding it at 0x99 with --poke stood the digits
// still (PROGRESS.md). 0 outside a round.
constexpr uint32_t kClock = 0xFF4808;
constexpr uint16_t kClockFull = 0x9900; // (BCD: ninety-nine)

// The Infinity Gauge, two bytes in a player's record: the meter that fills as the fight goes on (+0x195)
// and the number of full gauges it has earned (+0x198, 0..3). Holding the meter at 0x90 turns the bar
// under the health cyan and full, and the game itself raises the count to 3; holding the count at 3 does
// the same, so the pair is what the cheat collection calls "Max Power" (PROGRESS.md).
constexpr uint32_t kGaugeMeter = 0x195, kGaugeLevel = 0x198;
constexpr uint8_t kGaugeFull = 0x90, kGaugeLevelsFull = 3;

} // namespace msh
