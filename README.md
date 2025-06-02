# Chess Simulator
A C++ and OpenGL-based application that visually simulates pre-recorded chess games from a file. The program renders a 3D chessboard and animates each move step-by-step, providing a simple and interactive way to replay full games.

## Table of Contents
* [Preview](#preview)
* [Requirements](#requirements)
* [Installation](#installation)
* [Controls](#controls)
* [Game's file format](#games-file-format)

## Preview

## Requirements
* [glew](https://github.com/nigels-com/glew) (v2.2.0 or higher)
* [glfw](https://github.com/glfw/glfw) (v3.4.0 or higher)
* [glm](https://github.com/g-truc/glm) (v1.0.1 or higher)
* [make](https://www.gnu.org/software/make) (v4.4.1 or higher)

## Installation

### Linux
1. **Clone repository:**
```bash
git clone https://github.com/mikolajdlugolecki/chess-simulator.git
```
2. **Go into _chess-simulator_:**
```bash
cd chess-simulator
```
3. **Use _make_ to build the project:**
```bash
make
```
4. **Run program:**
```bash
./chess
```

## Controls
| **Key**   | **Action**                          |
| --------- | ----------------------------------- |
| **Space** | _Make move_                         |
| **R**     | _Restart game_                      |
| **1**     | _Whites' perspective_               |
| **2**     | _Blacks' perspective_               |
| **3**     | _Blacks' on left, whites' on right_ |
| **4**     | _Whites' on left, blacks' on right_ |

## Game's file format

To work correctly, the program requires a file named `game` containing a sequence of moves in a specific format. Each move consists of **two lines**:

1. The first line describes the move of the **white piece**.
2. The second line describes the move of the **black piece**.

Each line should follow this structure:

```text
[starting square] [destination square]
```

For example:

```text
e2 e4
e7 e5
g1 f3
b8 c6
```

This represents a sequence where the white and black pieces are moved in alternating order. The file must not contain any extra formatting, comments or empty lines.
