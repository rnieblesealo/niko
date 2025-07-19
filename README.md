messing around with raylb 

check it

![fukin niko](github/niko.png)

this is Niko the Nicotine-Addicted Punk Salamander

he will probaly star in this game

---

### What Systems Can I Isolate?

- Animated sprite renderer 
- Obstacle spawner (spawner)
    - Obstacles themselves don't have to be a specialized class 
- Player (unique final class)
- Game state handler (singleton pattern)
- Constants into header file

### A Scene...

- Needs a floor where player stands on
- Some decorative background stuff (clouds, a mountain, the sun, etc.) 
- Floor decorations/foliage (Cactus, grass, etc)

**In the event of a screen resize, how should we position all of these things?**

Position can be in absolute screen coordinates (raw pixels) or relative coords (% coords)

| thing | % coord | abs pixel coord | why |
-|-|-|-
clouds/bg deco (mountains, sun.. (mountains, sun...).) | x | | a window resize is analogous to our camera's fov increasing irl: when that happens, foreground stuff (player, floor, grass, etc.) noticeably has more room around it, but far away stuff like clouds stays in roughly the same position. relative coordinates take care of this for us. **tldr: relative coord if static, screen cord if not.**
player | | x | player's size shouldn't be proportional to the world's size
floor | x | | though floor is technically foreground, if we for e.g. enlarge the screen and floor is in screen coords, it will take a significantly smaller amount of screenspace. we want the amount of screenspace the floor takes up to be consistent regardless of screensize.
floor deco  | | x | fg item
obstacles | | x | fg item

> It seems right that anything on the floor = **foreground**, everything else = **background**
