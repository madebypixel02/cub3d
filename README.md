# cub3d | 42 Madrid

*My first RayCaster with miniLibX 💡*

<div align="center">
    <img src="https://user-images.githubusercontent.com/40824677/156563198-ac320c5a-be9e-43cc-9bcf-cd49670661e4.gif">
</div>

### Table of Contents

* [Introduction](#introduction)
* [Map Parsing](#map-parsing)
* [Raycasting](#raycasting)
     * [Walls](#walls)
     * [Textures](#textures)
* [Controls](#controls)
* [Bonus](#bonus)
* [Extras](#extras)
* [Gameplay](#gameplay)
* [Installation](#installation)
* [References](#references)
* [Summary](#summary)

## Introduction

The aim of the ``cub3d`` proyect is to create a 3D game using the raycasting technique which is a rendering method implemented in the world-famous ``Wolfenstein 3D`` game. This was a group proyect and I had the honor to do it with [@mbueno-g](https://github.com/mbueno-g) again :)

## Map Parsing

The ``cub3D`` executable receives as a single argument the map file we read, which must be of ``.cub`` filetype.

The file must follow these rules:
- There must be header lines before the actual map containing the following:
    - At least a line containing ``NO``, ``SO``, ``EA`` and ``WE`` followed by a valid path to an xpm image
    - A line starting with ``F`` (**f**loor) or ``C`` (**c**eiling) followed by a color in RGB separated by commas
- Only `` `` *(empty)*, ``1`` *(wall)*, ``0`` *(floor)*, and either ``N``, ``S``, ``E`` or ``W`` *(Player starting position looking at **N**orth, **S**outh, **E**ast or **W**est)*, will be accepted characters in our map (except if you add other characters as bonus)
- The map may not be rectangular, but it must be surrounded by walls
- Spaces are allowed but the player cannot walk on them, thus every space must also be closed/surrounded by walls
- There must be a single player on the map

Here's an example of a valid map (not rectangular but still valid):

```
NO textures/wall_1.xpm
SO textures/wall_2.xpm
WE textures/wall_3.xpm
EA textures/wall.xpm

F 184,113,39
C 51,198,227

         111111111  111111111111111111
         100000001  100000000000000001
         10110000111100000001111111111
11111111110000000000000000011111
10000000111100010000N00001001
100000001  100110101110000001
10000000111110000001 10100001
11111111111111111111 11111111
     
1111 111111111111111111111111
1111 1000000cococococ00000001
1111 111111111111111111111111
```

## Raycasting

Raycasting is a rendering technique to create a 3D perspective in a 2D map. 
The logic behind RayCasting is to throw rays in the direction of the player view. Basically, we need to check the distance between the player and the nearest wall (i.e. the point where the ray hits a wall) to caculate the height of the vertical lines we draw. Here is a simple depiction of it:

<div align="center">
     <img width="200" alt="Raycast Example 1" src="https://user-images.githubusercontent.com/71781441/154158563-5b4f7641-4f3d-4cca-97f1-4cc79aac16dd.png">
    <img width="233" alt="Raycast Example 2" src="https://user-images.githubusercontent.com/71781441/154159164-667da898-a8d5-4991-a8d0-a6008f111054.png">
</div>

### Walls
    
To calculate the **distance between the player and the nearest wall**, we can use the following algorithm:

**1.** Define and initialize some basic attributes needed for the projection: 

<table align="center">
    <tr aling="center">
        <th> Attribute </th>
        <th> Description </th>
        <th> Value </th>
    </tr>
    <tr align="center">
        <td> FOV </td>
        <td> The field of view of the player
            <div align="center">
    <img width="150" align="center" alt="FOV Image" src="https://user-images.githubusercontent.com/71781441/154864710-baee6726-6f2a-4f37-8125-97a5cf52c4f7.png">
</div>
        <td> 60º </td>
    </tr>
    <tr align="center">
        <td> HFOV </td>
        <td> Half of the player's FOV </td>
        <td> 30º </td>
    </tr>
    <tr align="center">
        <td> Ray angle </td>
        <td> Angle of the player view's direction </td>
        <td> N (270º), S (90º), W (180º), E (0º) </td>
    </tr>
    <tr align="center">
        <td> Ray increment angle </td>
        <td> Angle difference between one ray and the next one </td>
        <td> <code>2 * HFOV / window_width</code> </td>
    </tr>
    <tr align="center">
        <td> Precision </td>
        <td> Size of 'steps' taken every iteration </td>
        <td> 50 </td>
    </tr>
    <tr align="center">
        <td> Limit </td>
        <td> Limit of the distance the player can view </td>
        <td> 11 </td>
    </tr>
    <tr align="center">
        <td> Player's position </td>
        <td> Center of the square where the player is </td>
        <td> <code>(int)(player_x + 0.5), (int)(player_y + 0.5)</code> </td>
    </tr>
</table>


**2.** From the the player's position, we move the ray forward incrementing the x's and y's coordinates of the ray.

<img align="right" width="333" alt="Screenshot 2022-02-20 at 22 35 23" src="https://user-images.githubusercontent.com/71781441/154865310-1b8dc0c5-0def-416f-adb6-7acf2a01c53a.png">

```c
ray.x += ray_cos;
ray.y += ray_sin;
```

where `ray_cos` and `ray_sin` are the following:
```c
ray_cos = cos(degree_to_radians(ray_angle)) / g->ray.precision;
ray_sin = sin(degree_to_radians(ray_angle)) / g->ray.precision;
```

**3.** Repeat step **2** until we reach the limit or we hit a wall.

**4.** Calculate the distance between the player's and the ray's position using the euclidean distance:
```c
distance = sqrt(powf(x - pl.x - 0.5, 2.) + powf(y - pl.y - 0.5, 2.));
```

**5.** Fix fisheye
```c
distance = distance * cos(degree_to_radians(ray_angle - g->ray.angle))
```

This algorith is repeated ``window_width`` times, i.e. in every iteration we increment the angle until we have been through all the field of view. 
This distance is really helpful to calculate the height of the wall height:
```c
wall_height = (window_height / (1.5 * distance));
```

### Textures

Once we have hit a wall and know its position and distance to the player, we must check which side was hit and choose the correct texture for that side of the wall. With the correct texture file and the proper height of the wall at hand it we can read pixels from the texture file at a given width and copy them to the screen, following this formula:

```c
/* Get the color from image i at the given x and y pixel */
color = my_mlx_pixel_get(i, (int)(i->width * (g->x + g->y)) % i->width, z);
```

Note: in some cases the sprite's height is smaller than the height of the sprite we have to draw. We have an algorithm that effectively 'stretches' the sprite to fit the proper height

## Controls

Here is a summary of the various controls in the game:

- The ``WASD`` keys move the player up, down, left and right relative to the player's viewing angle
- The ``left`` and ``right`` arrow keys rotate the viewing angle of the player
- Press the ``ESC`` key or the ``X`` button on the window to exit the game

Note: these are the basic mandatory controls, but we added a few more keys to handle other things. See below for such controls

## Bonus

For this project there were several bonuses, and we did all of them:

* Wall Collisions

When walking to a wall, instead of stopping in front of it we split the movement into the ``x`` and ``y`` vectors and try to move in either of them, making wall collisions possible

* Minimap

Being entirely honest, we did this bonus out of necessity, because we had some issues with our raycasting algorithm in the beginning and the best way to solve those issues was to visualize what we were doing in 2D. We decided to center the player on the minimap and only draw a part of it to prevent UI inconsistencies. Sometimes very large maps would cover a large part of the screen

* Doors

This bonus was qucick to implement. We added two new characters to the map: ``c`` for **c**losed doors and ``o`` for **o**pen doors. We launch a ray that looks for doors and walls in the direction of the player and if a door is hit we open/close that particular door. Press the ``E`` key to trigger nearby doors

* Animations

A simple way to animate sprites was to animate the walls themselves. When we read multiple lines with ``NO`` (for example) we add it to a new node in a linked list. Then we just iterate over the linked list changing the sprite to the next one on the list

* Rotation with mouse

This one was very straightforward. There is an event on the ``minilibX`` library that tells the user the position of the mouse. When the position changes, we increment/decrement the player's view direction accordingly

## Extras

We implemented a few things that we were not asked to implement, but we thought would give the project a cooler vibe:

- Added a centered green scope on the window to help the user open/close doors. Also added green centered ray on the minimap
- Ability to end the game with the ``Q`` key
- Added darkening effect to the game. The farther a wall is hit, the darker it is drawn. This gives the game a cave-like feel
- Flash screen green/red when opening/closing a wall respectively
- Add some sample animations to test in maps (flaming torches in walls and various Pac-Man sprites)
- Ability to invert colors of the game by pressing the ``R`` key
- Game works both in Linux and MacOS

## Gameplay

Here are a few samples of how our maps look

- ``2.cub``

<div align="center">
    <img src="https://user-images.githubusercontent.com/40824677/156563400-46e259e7-b197-446b-8578-c80ff97fc709.gif">
</div>

- ``space.cub``

<div align="center">
    <img src="https://user-images.githubusercontent.com/40824677/156563514-1697f731-3a15-4e56-a047-db3a9aa1c121.gif">
</div>

- ``frame.cub``

<div align="center">
    <img src="https://user-images.githubusercontent.com/40824677/156563421-491b363c-21f2-4129-ad90-83e57ec2c4f6.gif">
</div>

- ``pac.cub``

<div align="center">
    <img src="https://user-images.githubusercontent.com/40824677/156563445-617cb27a-8df7-451e-9ce4-e22409bffdab.gif">
</div>


- ``pac2.cub``

<div align="center">
    <img src="https://user-images.githubusercontent.com/40824677/156563462-300565a2-9d6a-4a82-9c18-d16d3fb35ffb.gif">
</div>

To check some of our favorite layouts, see [MAPS.md](./maps/MAPS.md)


## Installation

### Cloning the repositories
```shell
git clone https://gitlab.com/madebypixel02/cub3d.git
cd cub3d
make
```

### Installing the MLX library

* ``Linux``

If you're not using a MacOS computer from 42, you'll need to install the libraries manually. Please refer to the [official github](https://github.com/42Paris/minilibx-linux) for more details. To install it, do the following (requires root access):

```shell
git clone https://github.com/42Paris/minilibx-linux.git
cd minilibx-linux/
make
sudo cp mlx.h /usr/include
sudo cp libmlx.a /usr/lib
```

* ``MacOS``

To install the library, you will need to first install a package manager like homebrew (check [here](https://brew.sh/)) to then install the X11 package with ``brew install Xquartz``. After that you must extract the minilibx file called ``minilibx_opengl.tgz``. Then install it to your system with the following commands (requires sudo as well):

```shell
cd minilibx_opengl
make
sudo cp mlx.h /usr/local/include
sudo cp libmlx.a /usr/local/lib
sudo reboot
```
Note: A reboot is necessary to ensure that the ``Xquartz`` is working properly. You can test if it is by running a test example with the command ``xeyes``.

### Installing the manuals

If you want quick access to the mlx manuals, it is recommended that you copy the files from the ``man`` folder in [minilibx-linux](https://github.com/42Paris/minilibx-linux) to your system manuals:

* ``Linux``
```shell
sudo cp man/man3/* /usr/share/man/man3/
```
Note: Depending on your Linux configuration, to get the manuals working (e.g. ``man mlx``) you will need to individually gzip all the manual files you just copied, e.g. ``sudo gzip /usr/share/man/man3/mlx.3``.

* ``MacOS``
```shell
sudo cp man/man3/* /usr/X11/share/man/man3
```

### Usage

```
make                        compiles cub3D executable
make bonus                  compiles cub3D executable (again)
make test MAP={path_to_map} compiles and executes cub3D with the specified map
make git                    adds and commits everything, then pushes to upstream branch
make norminette             runs norminette for all files in the project that need to pass it
```

## References

* [vinibiavatti1 - RayCastingTutorial](https://github.com/vinibiavatti1/RayCastingTutorial/wiki)
* [Lode's Tutorial Part 1](https://lodev.org/cgtutor/raycasting.html)
* [Lode's Tutorial Part 2](https://lodev.org/cgtutor/raycasting2.html)
* [Lode's Tutorial Part 3](https://lodev.org/cgtutor/raycasting3.html)
* [Lode's Tutorial Part 4](https://lodev.org/cgtutor/raycasting4.html)
* [42docs - miniLibX](https://harm-smits.github.io/42docs/libs/minilibx)

## Summary

After learning the basics of the ``miniLibX`` graphics library in the [so_long](https://gitlab.com/madebypixel02/so_long) project, this project was quicker to do than expected. However, raycasting was a brand new concept with tons of mathematical concepts behind it, so it was a bit tricky to grasp at first. Having said that, it was still a lot of fun :)

February 24th, 2022
