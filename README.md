### Todo!

- Speedup as time increases
    - For this to be fair, it'll require 
    - Varying jump height via press
- Game over/lose
- Score system
- Floor decorations :)
- High score on homescreen

### Can do

- Crouching
- Different obstacle types

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

### Code Style

- Game object interface methods should go in public/private heading AT TOP of child class
    - **Reason:** Clearly separating inherited vs. self methods
- If anything renders/updates but requires params (which game object intf forbids) the methods they use to do so should specify the difference the args make
> e.g. Since spritesheet renderer requires a dest rect, its **render** method should be called something like **renderToDest**
    - **Reason:** It self-documents the why of the divergence from the otherwise expected contract 
- Optional arg initial values should be defined in the HEADERFILE 
    - **Reason:** Default args are added via the .h and not in the definition 
- Always use explicit constructors to avoid weirdness from type conversions

### GUI + Relative Pos

- GUI currently uses px offsets but a static pos is 101% the better way to go here

### Game States

- 3 states title, ingame, game over
    - Title: highscore + gametitle show, no spawns, cant control niko, not keeping score, not speeding up, floor moving Ingame: curr score shows on top right, no gametitle show, enemies spawn, can control niko, keeping score + speeding up
    - Game over: highscore on center, 
- Variables:
    - highscore shows? 
        - top right or screen center?
    - game title shows?
    - game over shows?
    - spawns enabled?
    - niko controllable?
    - score being kept?
        - speeding up? (will speed up based on score so these are coupled)
    - floor moving? 
    - sprite animations on?
    - gravity on?
- Handling:
    - Enum that keeps 3 game states on game manager
    - States are main menu -> ingame -> game over -> main menu -> ... (should ALWAYS follow that cycle)
    - setState function that also calls required "cleanup/reset" logic upon state switching
        - We will need this to clear enemies on restart, reset niko's pos, change gui, etc. 
    - Then, on each individual object, we check the state of the enum to determine what logic to do
    - Pros:
        - Meaningful state tracking (literally ask, "is state game over?")
        - Code should be more readable (e.g. "if game state == game over then do ..." makes a lot of sense)
        - Easy state switching (just change the enum and according to above everything's logic should update)
    - Cons:
        - Decentralized logic makes related state scattered (e.g. if floor moving and animation is kept separate, the floor can be static while animations are on, which doesn't make sense if the state idiomatically means everything should be frozen)
        - We can't check individual variables if needed, only the current state (if we specifically need to check if spawns are enabled, we'd need a specific flag for this, since multiple game states may share the same logic)
        - setState might break if we switch between 2 unintended states (e.g. if we want to go from game over to in game without passing main menu, state might not be set correctly...)
    - Amends: 
        - Enforce cyclic state switching 
            - Add check that prevents setState from moving to illegal state
            - advanceState (increment the enum state by 1 and enforce the order via the enum values)
                - This actually seems like best soln... Sufficiently abstracts stuff while also enforcing cyclic state
    - Verdict:
        - 3-state enum with currentState variable in GAME_MANAGER
        - advanceState ++'s the curr state moving to the required next
            - e.g. if currentState == MAIN_MENU, advanceState() would ++ main menu and make currentState == IN_GAME.
                - First increment state 
                - Then based on newly set state run the required cleanup/advance logic (this could literally be a switch statement)
        - Individual objects then call GAME_MANAGER singleton, check the state, and adjust their update() and render() logic based on it 

### Input management

Right now all input checking is done by niko and anything we do in response to that is also done by him

But what if we want to do something that isn't concerned with Niko ergo changing state? This isn't his responsibility, but because he checks input he's also telling the state manager to switch state, despite him not being supposed to be in control of functionality so high-level

As such we need to lift input management up

We need an input system that can run callbacks based on registered bindings so state changes propagate DOWN from it 

Propose new class `INPUT_MANAGER`

Input manager has a message streamer (it fires so things listen but doesn't expect a response)

That message streamer tells the receiver what key was pressed

The receiver may then run whatever actions it needs based on the message it got

Ok... Why not make this a bunch of calls to Input.IsKeyDown? Why do we need an abstraction?

**Ease of changing keys!**

If we need to change the specific key bound to an action, we'd need to modify the IsKeyDown calls wherever they're made

But if we have a control layer (inputman) we can separate the action from the key that's bound to it such that we can change the keybind at any time for any reason

There's this separation between action and keybind

Action: a distinct signal that's supposed to trigger specific action(s)

Keybind: Keyboard input that triggers the action

Key triggers; action does

We can change the trigger but the action should stay the same; CoD allows you to change the button with which you shoot!

#### Implementation

- Inputman has a list of actions (enum `JUMP`, `ADVANCE_STATE`, etc.)
- We keep a dict where a Raylib keycode is mapped to one or more action(s)
- Whenever we make any input, we: 
    - Check whether its keycode exists in the dict
    - If found, check what actions are bound to that input
    - Send down a message to all input listeners (`GAME_OBJECTS`) containing the actions that were just run 
- e.g.

```json
{
    "ACTIONS":{
        "SPACE": {
            "JUMP",
            "ADVANCE_STATE"
        },
        "ALPHA_W": {
            `WALK_FORWARD`
        }
    }
}
```

```cpp
std::map<KeyboardKey, std::set<ACTION>> bindings;

std::set<ACTION> sendInputMessage(std::set<ACTION> triggeredBindings){
    return triggeredBindings; 
}
```

- Keycode `SPACE` is bound to actions `JUMP` and `ADVANCE_STATE` 
- If I press a key, we get the keycode from raylib, locate it in hashmap, and grab its actions
- We then call something like `sendInputMessage()` to all game objects, this function returns a list of the actions that were just executed

#### Considerations that are definitely overkill for this but would otherwise matter!

- Rebinding may be problematic if every key may have multiple values
- We should also have a way to ensure that every action has exactly ONE key bound to it (no dupes of it exist in any value lists) 
    - Adding a binding may be a bit costly because of this! We need to check for dupes...

# Actually, this design f***ing sucks, here's a better one!

- Simply have the listeners poll an action queue
- If the listener finds an action it cares about the queue is popped

But this sucks even more... If 2 things care about the same action but only 1 consumes the input then the other one won't ever know it was supposed to do something...

### What else can we do?

- Every frame:
    - Check for input as we did
    - If input register, push its action(s) to a set 
    - Repeat for all inputs; we should end up with a list of all the actions that were quieried for this frame
    - Then, each listener can have a `getAction(ACTION)` method which queries the input manager asking it if the action it cares about is in the set
    - It returns true/false and the observer handles whatever it needs to do

The vector changes each frame which might add a little overhead

But wait...

**Raylib doesn't return you a list of keys currently being pressed... Just the latest one :/**

Everything's fucked, I'll come back to this later lol :P
