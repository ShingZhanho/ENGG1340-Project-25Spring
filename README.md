# README

This is the README document of 2025 spring ENGG1340 Computer Programming 2 Group Project. We are group 1.

## Notes for Developers

> [!warning]
> Compiling on different OS requires different rules. To compile, please do the following:

1. Change your working directory to `build`:
   ```bash
   cd build
   ```
   ⚠️ You **MUST** run the make command in the `build` directory, otherwise it will not work and may pollute the repository.
2. Run the `make` command according to your OS:
   - For Ubuntu (default):
      ```bash
      make all
      ```
   - For macOS:
      ```bash
      make all_macos
      ```
   - For Windows (not guaranteed to work):
      ```bash
      make all_windows
      ```
##tips：be careful
      
## Framework

```
User input
   |
   |
   V
+--------------+ Bullet generate message  +-------------------+--------------+
| Player Class |------------------------->| Bullet Generation > Bullet Class |
+--------------+  (Position, direction)   +-------------------+--------------+
   |                                                                 |
   | Position                                                        |
   |     +----------------+---+-----------+                          |
   +-----| Mob generation | > | Mob Class |                          |
   |     +----------------+---+-----------+                          |
   |                             |                                   |
   |                             |                                   |
   V                  Position   |                                   |
+-----------------+<-------------+                                   |
| Collide handler |   Position                                       |
+-----------------+<-------------------------------------------------+
   |
   | All Instances Position
   V
+---------------+
| Output render |
+---------------+


FrameRefresh {
PlayerHandler(Input);
BulletRegister()
MobRegister()
BulletInstanceRefresh();
MobInstanceRefresh(PlayerPosition)
	CollideHandler {
	All Registered Instances
	}
Render
}
```
