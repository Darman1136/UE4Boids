# UE4Boids

This is a simple boid implementation based on http://www.red3d.com/cwr/boids/. During the update phase the forward vector of all boids are being adjusted. Afterwards they're moved according to the new vector.

UE4 StarterContent is required.

## Classes

### Boid
Implementation of a single Boid.

#### Settings
* **ForwardSpeed** Speed at which the boid is moving

### BoidManager
Manages all boids available in the scene. Ticks and calls the boids updated and move functions.

#### Settings
<img src="/Images/BoidManager.png" width="300">

* **Active** Should boids be updated
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

* **Active** Should spawn boids
* **Class to Spawn** Boid implementation class
* **Spawn Count** Amount of boids to spawn
* **Spawn Radius** Radius around the spawners location to spawn boids at

