# Instructions
Make a game in the style of your choice (2D or 3D). No 2D Platformers or extensions of the previous project. If you are making a 2D game it needs to be something other than a platformer. It can be a top down game for instance, or other style of 2D game. A 3D platformer is OK! You must use new assets (art, music, sound) for this project. No Good/Evil Professor Carmines. No using George or anything from the lectures.

### Requirements:
- Menu Screen: Show the name of the game. Press enter to start.
- About 2 Minutes of Gameplay. (can be 1 giant level, multiple levels, etc.)
- Some sort of moving AI
- Some way for the player to lose (touching an AI, falling off a ledge, running out of time).
- Some way for the player to win (killing all the AI, collecting all the coins, whatever you want).
- At least 1 looping background music, and at least 1 sound effect.

# Controls
| Key | Function |
| :-: | :-: |
| `Enter` | Start Game |
| `WASD` | Orbit Scene |
| `Arrow Keys` | Move snail |
| `Right Shift` | Climb up a wall |
| `Left Shift` | Climb down a wall |

> Once the snail is up a wall, it will only move along the y-axis. To move along other axis, return to the aquarium floor by holding down `LSHIFT`.

# Demo
## Completed Tasks:
- Menu Screen: 
  - Name of game is displayed
  - `Enter` to start
- Moving AI Betta
  - Betta follows you unless you're near a plant
  - If you're near a plant, the betta swims away in the opposite direction
  - Betta slowly swims lower and lower until it hits the snail 
  
- Some way for player to win
  - Snail is moved "out" of tank when `y.pos > 2` (hold down `RSHIFT` for a few seconds to test)
  - `Win` screen
 - Some way for player to lose
  - Betta collision: life lost and level is restarted
  - after 3 lives are lost, `Lose` screen
- 1 looping background music
- 1 sound effect
  - Played when pressing `Enter` on `Menu` screen
  - Also played when betta and snail collide
- Implementation of original assets not used in previous assignments

## In Progress:
-

## Future Development:
- Make gameplay last longer
- Set wall boundaries (maybe with more cubes?)
- More natural fish movements
  - making betta movements more realistic by rotating along y-axis
- Shorter load time in between scenes?

*Stay tuned!*

| ![](demos/title1.png) | ![](demos/title2.png) |
| - | - |
| ![](demos/game.png) | ![](demos/win.png) |

# Asset Credits
## Music
Original music by [Charlie Reynolds](https://soundcloud.com/saxyjew).

## Font
[Small Blocky Font](https://opengameart.org/content/small-blocky-font) by [Jerom](https://opengameart.org/users/jerom) via [`OpenGameArt.org`](https://opengameart.org/).

## Graphics
| ![](https://github.com/mkarroqe/CS3113/blob/master/06-Students-Choice/Assets/Betta/betta-demo.jpg) | ![](https://github.com/mkarroqe/CS3113/blob/master/06-Students-Choice/Assets/Snail/SNAIL.png) | ![](https://github.com/mkarroqe/CS3113/blob/master/06-Students-Choice/Assets/Grass/grass-demo.png) |
| :-: | :-: | :-: |
| [Betta](https://free3d.com/3d-model/crowntailbetta-v1--31129.html) | [Snail](https://free3d.com/3d-model/snail-23158.html) | [Grass](https://free3d.com/3d-model/high-quality-grass-78178.html) |
| ![](https://github.com/mkarroqe/CS3113/blob/master/06-Students-Choice/Assets/Palm/Palm_01.jpeg) | ![](https://github.com/mkarroqe/CS3113/blob/master/06-Students-Choice/Assets/Fern/fern-demo.jpg) | ![](https://github.com/mkarroqe/CS3113/blob/master/06-Students-Choice/Assets/Ground/gravel2.jpg) |
| [Palm](https://free3d.com/3d-model/palm-54462.html) | [Fern](https://free3d.com/3d-model/-fern-v2--65217.html) | [Gravel](https://www.pinterest.com/pin/353814114473920228/) |

# Inspiration
| My late aquatic children, Roomba the snail and Emo the betta. | <img src="demos/muses.png" width=500px /> |
| - | - |

# Bloopers
| ![](demos/bloopers/fern-overload.gif) | ![](demos/bloopers/angry-betta.gif) |
| - | - |
