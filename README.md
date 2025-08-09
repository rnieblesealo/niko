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

- References are not assignable, so the floor prop textures array must use `std::reference_wrapper`

---

When adding floors, the initializer currently adds 2 with the correct consistent dimensions
The floor moving logic assumes they are all the same width
If we ever change the initializer's logic to allow multiple floor textures, this will be an issue!

### Obstacles + Scene

- The scene shouldn't be in control of spawning obstacles; it's simply where they'll be placed
- There should be a dedicated singleton "game manager" that handles things like collision and such
- Obstacle textures will be a vector now but it will be private so we cant add to it after its committed!
- The reason it's a vector is that we must specialize size of `std::array` in member
> i.e. If we wanted to add more obstacle texts. in the `main.cpp`, we'd need to update the array length in 2 places. Better to have an editable vector!

### Why Circle Collision?

- Less difficult to adjust than collision rect
> Sprite dest rect often has whitespace around it which is also accounted for in collision. We want a manually adjustable collision shape to amend this. A circle simply requires a point and a center, and we only need to store the radius!
- Consider changing this for a struct!

### Default Args

As a coding standard thing, default args should always be in the .h

### Game Manager Snafoo

It makes sense to have the game manager be a singleton because the entire game should have a big puppet master controlling it all. We give him what he needs and he just does it.

Since singletons shouldn't have constructors with params (not sure why yet) I had to move members it used to have (like the ref. to the texture pool it could grab from) to outside and pass them to functions instead.
 
I also made a dedicated value object for obstacles that stores proper full texture references (not just an index) and the unique rect for that obstacle as members. This is more idiomatic and uncouples this from the texture pool array.

This required, however, using shared pointers to manage the texture refs; using normal pointers is a bit shitty (we have to manage that mem. ourselves) and refs are unassignable since they're just an alias. The initial obstacle texture pool then stores all possible textures as shared pointers and we go from there!

### What is a game object?

Anything that "moves with the game"

i.e. We care about updating + re-rendering it every frame 


