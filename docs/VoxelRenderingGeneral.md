# Voxel Rendering

> Felix Hommel, 3/17/2026

Voxel rendering is a pretty tough challenge. It uses multiple disciplines of computer science (Graphics programming,
algorithms, optimization, ...), which is the main reason it is as tough as it is. The information on how to make a
voxel renderer available on the internet is very sparse, which is frustrating when trying to research it. After being
fed up with the unavailability of information, I came to the hard but real realization: If one want's to create a
voxel renderer, one has to learn it the hard way on their own. So this is exactly what I am going to do. I plan to
document my experiences and realizations, so that future developers do not have to go through all the hard work to
render some voxels.

## Gradual Progression

As often in software development, it is quite hard to go from 0 to 100 in a single step. That is not different here.
I am planning to implement this project in many steps and sub-steps, in the hope to arrive at the end goal eventually.
During my initial research, I found that voxel rendering is much more popular, or at least much better documented, when
it is used in a context where it is chunk based and then used to generate a mesh from the chunks. That is what I call
Minecraft-style voxel rendering. This is what I am NOT trying to make. I am aiming to something that is much more
detailed, muych more complex. I want to be able to see a detailed model in a voxel representation, this inspiration for
me came mostly from games such as [Teardown](https://teardowngame.com/) by [Tuxedo Labs](https://www.tuxedolabs.com/) or
[Town to City](https://store.steampowered.com/app/3115220/Town_to_City/) by [Galaxy Grove](https://www.galaxy-grove.com/)
because I love the visuals that we get to see from both these games. This kind of voxel rendering is what I call dense
voxel rendering. It is much more complex to achieve than Minecraft-style voxel rendering since we can't just put our
voxel data in a chunk style data format (i.e., a 32x32x32 array; I call it a dense grid) this would not scale well
because we would need to go through every voxel to render it. Given that we want to render open worlds, we can assume
that at least a good amount of the voxels will be empty, so most of the voxels we traverse will be skipped. This is why
we need acceleration structures such as Sparse-Voxel-Octrees(SVOs) as they will allow us to skip entire spaces of
equivalent voxels.

## Stage 1: Dense Grid and DDA

In the previous section I wrote, that dense voxels grids are undesirable due to their bad scalability. Despite that
being true, another fact is also true: It is comparatively easy to implement. If we have a simple dense voxel grid,
we can use an algorithm called the Digital Differential Analyzer Algorithm (DDA) to traverse through the grid. DDA is a
very efficient algorithm that can be used to determine which is the first voxel visible from a point of view. An awesome
explanation of how DDA works can be found [here](https://aaaa.sh/creatures/dda-algorithm-interactive/).

