# TTMS (Theater Ticket Management System)

A terminal-based theater ticket management system written in C, organized in layered modules:

- `View/` — menu/UI flows
- `Service/` — business logic
- `Persistence/` — file-based storage
- `Common/` — shared helpers

## Features

- Studio (projection room) management
- Play management
- Schedule listing/query
- Ticket generation for a schedule
- Ticket sale entry flow (play → schedule → ticket UI)
- Account management

## Build

Prerequisites:

- C compiler (GCC/Clang)
- CMake
- Ninja or Make

Build commands:

```bash
cmake -S . -B build
cmake --build build
```

Executable output:

- `build/TTMS`

Run:

```bash
./build/TTMS
```

## Main Menu

At runtime, the main menu includes:

- `S` Studio Management
- `P` Play Management
- `T` Ticket Sale
- `Q` Queries
- `A` Account Management
- `E` Exit

## Data Storage

This project uses local data files (binary `.dat` files) for persistence, for example:

- `Schedule.dat`
- `Ticket.dat`
- `sale.dat`

These files are created/updated during runtime.

## Notes

- The project is educational and currently uses simple file persistence.
- Some legacy code paths still use older C I/O patterns; warnings may appear during linking/compilation.
