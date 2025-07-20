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

Placing static background props:
- Pass in texture ref
- Pass in % pos 
- Done!

Should be able to pass any texture (i.e. place arbitrary props); there is no state that must be held for these past their original pos. for now

Placing floor props:

Since these will spawn dynamically, the scene must create/destroy them at random (as if they were obstacles). The scene therefore needs to know what props it can place

We can internally store an array of placeable textures and then have a function that allows us to add new ones

Pipeline:
- Initialize scene
- Call `addFloorProp(Texture2D& prop)` a bunch of times with all the textures we want
> Maybe we can also pass a scale? We probably will need to resize these
- The scene now knows what it can spawn
- Then, at some interval of time, it will instantiate a rect in an internal vector which will serve as a dest draw rect
- This draw rect will have a texture ref linked to it
> This means the vector will probably contain `std::pair`s
- An `updateProps` func will displace them to the end of the screen and destroy them if they go off bounds

Basically, floor props will work almost exactly like I'm thinking obstacles should, except you can't collide with them :)
