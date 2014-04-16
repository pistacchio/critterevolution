Critterevolution
================

A simple simulation of evolving creatures
-----------------------------------------

![critterevolution screenshot](https://github.com/pistacchio/critterevolution/raw/master/img/screenshot.png)

Critterevolution simulates an environment where randomly generated creatures (called Critters) swim trying to remain healty and made. Each generation, only the fittest survive and have a chance to pass its genes to its breed.

### Rules
Each Critter is defined by a peculiar movement composed by three trigonometric functions that alternate to generate a path. Each step the Critter loses some energy according to how much it has moved. If the Critters crosses a food (greed dots), it eats it and gains energy. It it crosses another Critter, they can mate and generate a new Critter.

To be able to mate, a Critter:

* Must not have bred already
* Must have a minimum amount of energy
* Must not be "too young" or "too old"

When two Critters mate, the spawned breed randomly inherits each properties from either the parents.

Some movement strategies are more effective than others. The more the Critter moves around, the more it has chances to find food and meet mates, but if it moves too fast and in a small area, it consumes a lot of energy to roam in an area that may not provide food.

Critters live a fixed number of cycles.

### Configuration
The simulation parameters can be set in a <code>conf</code> file located in the same directory of the program.

Sample conf file:

```
health    = 300
age       = 2000
critters  = 20
foods     = 200
foodpower = 400
``

<code>health</code> is the the mimum energy required by a Critter to be able to mate
<code>age</code> is the lifespan of the Critters
<code>critters</code> is the number of initial Critters
<code>foods</code> is the number of Foods available in the simulation
<code>foodpower</code> is the amount of energy provided by each Food

### Building8
Critterevolution consists of a single C++11 file. It depends on the [SFML](http://www.sfml-dev.org/).

