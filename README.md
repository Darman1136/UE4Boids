# UE4Boids

This is a simple, multithreaded boid implementation based on http://www.red3d.com/cwr/boids/. During the update phase the forward vector of all boids are being adjusted and moved accordingly.

Written in UE4.23.1, StarterContent is required.

## Configuration File

The executable can be configured via the following options. It's located in <code>%appdata%\Local\Boids\Saved\Config\WindowsNoEditor\Boids.ini</code>. The file may not exist, you can use the values below when creating it. (If the folder is missing, start the packaged executable once)

```
[/Script/Boids.BoidSpawner]
SpawnCount=500

[/Script/Boids.BoidManager]
AmountOfThreads=2
```

* **SpawnCount** Amount of boids to spawn
* **AmountOfThreads** The amount of threads to create and use for calculating positional updates

## Classes

### Boid
Implementation of a single Boid.

#### Settings
<img src="/Images/Boid.png" width="300">

* **Amount of Boids to Observe** Max amount of boids to take into account for next calculation
* **Forward Speed** Speed at which the boid is moving
* **Turn Speed** Speed at which the boid is turning
* **Close Boid Distance Cut Off** Max distance for another boid to this boid to be taken into account for next calculation

### BoidManager
Manages all boids available in the scene. Ticks and calls the boids move functions. Creates BoidCalculationWorker and assigns them their boids.

#### Settings
<img src="/Images/BoidManager.png" width="300">

* **Active** Should boids be updated
* **Amount Of Threads** The amount of threads to create and use for calculating positional updates (BoidCalculationWorker)
* **Boids Follow Target** Do boids try to reach the BoidTarget or are free to roam
* **Separation Weight** Weight of Separation, see http://www.red3d.com/cwr/boids/
* **Alignment Weight** Weight of Alignment, see http://www.red3d.com/cwr/boids/
* **Cohesion Weight** Weight of Cohesion, see http://www.red3d.com/cwr/boids/
* **Target Weight** Weight of trying to reach the target

### BoidTarget
The target in the scene the boids try to reach.

### BoidSpawner
Spawns boids according to its settings on BeginPlay.

#### Settings
<img src="/Images/BoidSpawner.png" width="300">

* **Active** Should spawn boids (boids can be added to the scene at any time)
* **Class to Spawn** Boid implementation class
* **Spawn Count** Amount of boids to spawn
* **Spawn Radius** Radius around the spawners location to spawn boids at

### BoidCalculationWorker
Calculates the rotation for its assigned boids. Boids are assigned via the BoidManager. FRunnable implementation.
