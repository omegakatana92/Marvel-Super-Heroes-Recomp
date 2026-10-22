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

## The memory so far

_(None yet: this port has just been translated, with `make explore` driving the game's code out of the
interpreter and into translated C++. Addresses found from here on are written in this table, and the
`src/` files name only those.)_

| Address | Size | What | How it was found |
| --- | --- | --- | --- |
| _(none yet)_ | | | |

## Features built on it (`src/`)

_(None yet. Any game-side fix — a hook for a piece of the program the port has to do differently from
the board — is added in `games/msh/src/` and written up here.)_

The toolkit's own features work without any of this: the launcher, the in-game menu, save states,
run-ahead, the Display settings and full control remapping.

## Next

- Find the player records (health, the Infinity Gauge, the round's clock) and the state that says what a
  player is doing, as the sibling ports do, and build the features the toolkit's `fight/` parts want on
  top of them.
- Keep the count of instructions left to the interpreter falling: play the paths the explore runs did
  not reach, then `make` again.
