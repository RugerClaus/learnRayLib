## Game Instructions

- **Press Space** to start the game (if that isn't obvious).
- **F10** will close the program at any time.
- Use **WASD keys** to move up, left, down, and right.
- Use Left Shift to sprint
- use Left Control to sneak

## Sea of bullshit

This is a test repo for learning Raylib using the C programming language.

I'm not the most familiar with C, but I've been able to wrangle it well enough to make this dumb little demo.


I have begun implementing procedural terrain generation with chunking. can't go past 0,0 left or up, but chunks generate continuously in the positive coordinates. I will implement a world border past y 0 or x 0, but other than that it will be infinite

began implementing terrain features from sprites. still figuring that one out, will work on it tonight

scratch that, I made a small tile sheet and implemented the textures for the terrain gen
it totally works ;)

implemented multi-layered perlin noise system

generates biomes based on elevation and temperature. 

now has a 5x5 mapped tilesheet that is ready for expansion.

renders tiles based on biome. each biome has its own row in the tile sheet
from left to right is elevation. the lower elevation tiles are on the left and the higher on the right

thanks for coming to my TED talk


experimenting with biome blending and temperature calculations.

implemented fractalnoise to calculate biomes and temperature

integrated player into entity system. began setting up an ECS with basic render handling. this is to prepare for my addition of entities. 

added at app state finite state machine pattern to keep track of basic application states IE GAME_ACTIVE, MAIN_MENU, QUIT. This helps control flow and minimizes input bugs at different parts of the program.

when in game, a small overlay is displayed for the player indicating health, level, and exp count. These will be future systems. This will be a free roam game, but i'm not sure beyond that. 

Next step is to define more boundaries for the player and begin working on some static entities.

unlike the framework I'm building in pygame, the debug overlay is not accessible from anywhere within this application, soley from the game state.

If you want to run this, make sure you have raylib installed on your computer, then run:

```
bash compile.sh

```

DISCLAIMER: I am not good at fancy algorithms, and my butchered noise algorithm has been pulled from a bunch of nonsense sources. However it does work how I need it to, so you can bite me. Everyone needs to start somewhere and I will say most programming I've done does not include this dense forest of math. But it has been lots of fun to mess with


Added scrolling world background for title screen

Added song for music that fits the theme of the place.

Updated world generation to be a bit smoother with a new fractal noise function

I have done my utmost to prevent the player from spawning in deep water tiles, but i've only been moderately successful so far
Regardless i've done a decent job of preventing it. The player can only spawn in the first 64 tiles though. 

implemented a getChunk function to aid in finding the biome and tile at a given position. This is also working for temperature and the pattern is good to expand it to other world properties that I procedurally generate. 

Next on the agenda is preventing the player from entering water tiles (for now, I will be making a boat system or something) and generating the first entities, the static entities (trees, rocks). That tile map is included in this update.

The song for the sound track is call Isle of Atmospheres

added sprinting and sneaking despite not having any characters. may as well work on basic mechanics

oops didn't update this last commit. 

Updated world generation to be agnostic of all entities. But i have hit a major wall with calculating the biomes and tiles
may have to rewrite the world gen logic. It will be fine for the title screen background, which I've separated into separate functions so that 
I don't have to deal with rewriting that as it works fine as is.

testing offset of player and tiles, but I think the tiles are all kinda fucked. I'm going to re-implement them at some point if necessary but I hope I just have some calculations off, but changing the position I calculate for the tiles isn't changing anything and the biome is correct in the debug overlay, so the tile system is fucky