# README

This is the README document of 2025 spring ENGG1340 Computer Programming 2 Group Project. We are group 1.


## Team Members (alphabetically)

- SHING, Zhan Ho Jacob (3036228892) @ShingZhanho


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
