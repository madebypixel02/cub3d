# cub3d

*My first RayCaster with miniLibX 💡*

🚧 WIP 🚧

<p align="center">
    <img src="https://user-images.githubusercontent.com/40824677/155116913-80e549a9-1080-4eaf-bf44-0c0940998119.gif">
</p>

### Table of Contents

* [Introduction](#introduction)
* [Raycasting](#raycasting)
     * [Walls](#walls)
     * [Textures](#textures)
* [Map Parsing](#map-parsing)
* [Bonus](#bonus)
* [Extras](#extras)
* [Gameplay](#gameplay)
* [Installation](#installation)
* [References](#references)
* [Summary](#summary)

## Introduction


## Introduction

The aim of the ``cub3d`` proyect is to create a 3D game using the raycasting technique which is a rendering method implemented in the world-famous ``Wolfenstein 3D`` game. This was a group proyect and I had the honor to repeat with @mbueno-g :)


## Raycasting

Raycasting is a rendering technique to create a 3D perspective in a 2D map. 
The logic behind RayCasting is to throw rays in the direction of the player view. Basically, we need to check the distance between the player and the nearest wall (i.e. the point where the ray hits a wall) to caculate the height of the vertical lines we draw. Here is a simple depiction of it:

<p align="center">
     <img width="200" alt="Raycast Example 1" src="https://user-images.githubusercontent.com/71781441/154158563-5b4f7641-4f3d-4cca-97f1-4cc79aac16dd.png">
    <img width="233" alt="Raycast Example 2" src="https://user-images.githubusercontent.com/71781441/154159164-667da898-a8d5-4991-a8d0-a6008f111054.png">
</p>

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
        <td> The field of view of the player <img width="150" align="center" alt="FOV Image" src="https://user-images.githubusercontent.com/71781441/154864710-baee6726-6f2a-4f37-8125-97a5cf52c4f7.png"></td>
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
        <td> 2 * HFOV / window_width </td>
    </tr>
    <tr align="center">
        <td> Precision </td>
        <td> Size of 'steps' taken every iteration </td>
        <td> 70 </td>
    </tr>
    <tr align="center">
        <td> Limit </td>
        <td> Limit of the distance the player can view </td>
        <td> 11 </td>
    </tr>
    <tr align="center">
        <td> Player's position </td>
        <td> Center of the square where the player is </td>
        <td> pl.x += 0.5 ; pl.y += 0.5 </td>
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

**3.** Repeat step 2 until we reach the limit or we hit a wall.

**4.** Calculate the distance between the player's and the ray's position using the euclidean distance:
```c
distance = sqrt(powf(x - pl.x - 0.5, 2.) + powf(y - pl.y - 0.5, 2.));
```

**5.** Fix fisheye
```c
distance = distance * cos(degree_to_radians(ray_angle - g->ray.angle))
```

This algorith is repeated window_width times, i.e. in every iteration we increment the angle until we have been through all the field of view. 
This distance is really helpful to calculate the height of the wall height:
```c
wall_height = (window_height / (1.5 * distance));
```

### Textures

## Map Parsing

## Bonus

* Wall Collisions
* Minimap
* Doors
* Animations
* Rotation with mouse

## Extras

## Gameplay

Here are a few samples of how our maps look

- ``1.cub``

<p align="center">
    <img src="https://user-images.githubusercontent.com/40824677/155116616-824e5a4e-7dc2-4e6b-8d00-5d22e57d7c2c.gif">
</p>

- ``2.cub``

<p align="center">
    <img src="https://user-images.githubusercontent.com/40824677/155116751-58a1f0d4-8500-480d-bf47-c93073933fbb.gif">
</p>

- ``pac.cub``

<p align="center">
    <img src="https://user-images.githubusercontent.com/40824677/155117235-bd8e392a-d856-4201-9f56-13a72f7ef3ca.gif">
</p>


- ``pac2.cub``

<p align="center">
    <img src="https://user-images.githubusercontent.com/40824677/155117526-38191be8-94d7-43e5-bca0-aca7a0efeb00.gif">
</p>

To check some of our favorite layouts, see [MAPS.md](https://github.com/mbueno-g/cub3d/blob/main/maps/MAPS.md)


## Installation

## References

## Summary
