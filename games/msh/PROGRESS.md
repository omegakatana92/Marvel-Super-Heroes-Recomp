# Marvel Super Heroes: progress

What is known of *Marvel Super Heroes* (Euro 951024)'s memory, and what has been built on it. Every
address here is confirmed by experiment on the running board with the toolkit's own tools
(`docs/FEATURES.md`); nothing is a dump of the game's data, and no game data is written down.

## The ROM set

The set is MAME's **`msh`** (Euro 951024), the parent of `mshu`, `mshj`, `mshjr1`, `msha`, `mshh`,
`mshb`, `mshbr1` and `mshud`. It was told apart from the clones, and its files listed, from the set's
own contents against MAME's ROM definition for `msh` (Capcom, 1995, board `cps2_2p6b`, 2 players, 6
buttons):

- the program is eight 512 KiB ROMs — `mshe.03e`, `mshe.04e`, `msh.05` … `msh.10b` — 4 MiB in all,
  so `code_end = 400000`;
- the graphics are eight 4 MiB ROMs in two rows of four — `msh.13m`, `msh.15m`, `msh.17m`, `msh.19m`
  then `msh.14m`, `msh.16m`, `msh.18m`, `msh.20m` — 32 MiB, side by side, and the graphics region is
  populated from 0 (no leading hole, unlike Street Fighter Alpha);
- the sound program is `msh.01`, `msh.02`; the QSound samples are `msh.11m`, `msh.12m` (4 MiB);
- the key is `msh.key`.

Every file is checked by CRC-32 (`games/msh/game.cfg`), so only this revision is accepted.

## The translation

`make` first built an interpreter-only port, ran it to read the board's program out of the set, and
then translated what it could see. `make explore` drove the game headless in rounds until the count of
instructions with no translation stopped falling. The last round reported:

```
recomp: 1752 entries -> 109930 instructions (430766 bytes) in 26298 blocks,
        6383 routines, 69 files; 69 left to the interpreter (22 of them not
        instructions: data a guess took for code)
recompz80: 380 entries -> 1974 instructions (4074 bytes) in 551 blocks,
        62 routines; 0 left to the interpreter
```

Those 69 are addresses `make explore` guessed were code but which turned out to hold data (a jump
table, a pool of constants); the interpreter runs them as instructions only if control ever reaches
them, which the checks never saw. The sound program is fully translated.

`make check` then showed the translated code matching the interpreter by state hash over 3000 frames,
and two runs of the same input producing the same hash.

## How the game's own memory is found

The tools used, in the order they are reached for (all in `docs/FEATURES.md`):

- `--menu FRAME:KEYS` drives the in-game menu with no window: a coin goes in without a hand.
- `--movie FILE` replays a hand-written input movie, and `--poke ADDR=BYTE` holds a byte at a guess; the
  two together are how the game is driven to a known place and a thought tried.
- `--dump-ram FRAME:FILE` writes the 64 KiB of work RAM after a frame; `--trace LOW-HIGH:FILE` appends a
  stretch of work RAM (or all of it) after every frame; `--shot-every N:PFX` writes pictures of the board.
- `cps2recomp/tools/ramfind.py` lists the addresses that held a known value in each of several dumps;
  `cps2recomp/tools/tracechanges.py` prints a trace where a value changes.
- `--watch LOW[-HIGH]` reports every write the 68000 makes to an address range with the instruction that
  made it, read against `work/msh/program.lst`.

## Getting to a match

The board boots to a warning, then the title and an attract show. A movie presses Coin at frame 900 and
Start at 1000; the character select takes a punch (frame 1200) and its mode select another (frame 1800),
and the round's intro starts about frame 2600. A round is under way when both health bytes are neither 0
outside a round nor 0xFF for a beaten player.

**The credits** are a word at `FF4C08`: holding it at 5 with `--poke` changed the title to
"CREDITS 5". One coin adds one, and pressing Start takes it away again. (`FF27F0` and `FFF974` count the
coins put in and are not spent by Start: they are the operator's audit, not the credits.)

**The clock** is a word at `FF4808` whose high byte counts down in BCD (`0x97` is "97"): a dump at frame
2800 read `0x97` and one 60 frames later read `0x96`. Holding the word at `0x99xx` with `--poke` stood
the digits still.

**The player records** are two 0x400-byte records at `FF4000` and `FF4400`. The health is the byte at
`+0x191` (`FF4191`, `FF4591`): dumps taken as a round was fought read `0x90` at its start, and the health
bar was drawn exactly one pixel per point — the bar measured 90 pixels wide when the byte read `0x5A`,
71 at `0x47`, and 52 at `0x34`. It reads 0 outside a round and in the attract show, and `0xFF` once a
player is beaten. The same byte in the second record follows player 2.

The bytes at `FF400C`/`FF440C` (`+0x0C`, a 32-bit value the intro fills) and at `FF8D9C`/`FF8E7C` are
not the health: a `--poke` of them leaves the bars untouched, so the sources do not use them.

**The Infinity Gauge** is two bytes in the same record: the meter at `+0x195` (`FF4195`, `FF4595`) and
the number of full gauges it has earned at `+0x198` (`FF4198`, `FF4598`, 0 to 3). The MAME cheat
collection names both as "Max Power" (`0x90` and 3); holding them there on the board turned the bar under
the health from the dark green stripes to solid cyan and full. Filling the meter alone made the game raise
the count to 3 by itself, and holding the count at 3 alone filled the bar too, so the two agree.

## The memory so far

| Address | Size | What | How it was found |
| --- | --- | --- | --- |
| `FF4C08` | word | **Credits**: the title reads it as "CREDITS n" | Holding it at 5 with `--poke` turned the title to "CREDITS 5"; one coin adds one and Start spends it. |
| `FF4808` | word (high byte BCD) | **The round's clock**: the high byte counts down (`0x99` is "99"), 0 outside a round | `--dump-ram` a second apart read `0x97` then `0x96`; holding it at `0x99xx` stopped the digits. |
| `FF4000` (+`FF4400`) | record | **Player 1 (player 2)'s record**, the two 0x400 apart | Found with the health byte below. |
| `+0x191` (`FF4191`/`FF4591`) | byte | **Player 1 (player 2) health**, `0x90` at a round's start; the bars are drawn one pixel per point; 0 outside a round, `0xFF` once beaten | Dumps over a match: the byte fell exactly as the bar shortened (90, 71, 52 pixels for `0x5A`, `0x47`, `0x34`). |
| `+0x195` (`FF4195`/`FF4595`) | byte | **The Infinity Gauge's meter**, `0x90` full | The cheat collection's "Max Power" pair; holding it at `0x90` turned the bar cyan and full. |
| `+0x198` (`FF4198`/`FF4598`) | byte | **The number of full Infinity Gauges**, 0..3 | The other half of "Max Power"; holding it at 3 filled the bar the same way. |

## Features built on it (`src/`)

| Feature | Category | What it does |
| --- | --- | --- |
| `free_play` | Game (Enhanced) | Holds two credits, so Start works without a coin. |
| `no_damage` | Cheats | Puts player 1's health back to the round's full value every frame. |
| `no_damage_2` | Cheats | The same for player 2 (the opponent's, in a one-player game). |
| `infinite_time` | Cheats | Holds the round's clock where it stood when the round began. |
| `one_hit` | Cheats | Leaves the opponent (player 2) a single point of health, so any hit ends the round. |
| `full_gauge` | Cheats | Holds both players' Infinity Gauges full, so a gem's power is always ready. |

Each was checked with no window against a run of the board, with `--dump-ram` of the bytes and
`--shot-every` pictures:

- `free_play`: a movie with **no** coin press reaches character select and a match (the title reads
  "PRESS START"), where without the feature it stays on the attract show.
- `no_damage`: over the movie above the health byte stays `0x90` all round, where the same movie without
  it drains it to `0x47`.
- `one_hit`: player 2's health byte is held at 1 all round.
- `infinite_time`: the clock word reads `0x9928` at frames 3000 and 3600 where the board's has counted
  down.
- `full_gauge`: both records read `0x90` and 3 at the meter and the count, and the bars under both health
  bars are solid cyan where the board's are dark green stripes.

`make check` still passes: with every feature off the game runs exactly as the board does.

## Next

- The rest of the player records: the place across the stage, the facing and the state that says what a
  player is doing (the training aids' frame data wants it), then the attack boxes. `FF406A`'s `0x80` bit,
  which the cheat collection calls "No Ground Attacks", is a first thread for the state.
- Keep the count of instructions left to the interpreter falling: play the paths the explore runs did
  not reach, then `make` again.
