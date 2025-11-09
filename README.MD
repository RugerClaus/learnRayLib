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