# README

This is the README document of 2025 spring ENGG1340 Computer Programming 2 Group Project. We are group 1.


## Team Members (alphabetically)

- CHAN, Ho Nam (???) [@Scevenate](https://github.com/Scevenate)
- LEUNG, Ming Ngai (3036393221) [@ThomasL956](https://github.com/ThomasL956)
- LIU, Jia Lin (3036391493) [@Kylineason-Liu](https://github.com/Kylineason-Liu)
- SHING, Zhan Ho Jacob (3036228892) [@ShingZhanho](https://github.com/ShingZhanho)
- SONG, Sizhe （3036457247）[@TIP-SoNg](https://github.com/TIP-SoNg)

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
