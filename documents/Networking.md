
# Networking paradigm

## Paradigm

Network data sharing come with his problem's lot, first one is data share latency to counter this we must always anticipate and start interpolating without knowing other players actions.

Our first tool is velocity, when we send a transform data we must send the transform action velocity

## Dead Reckoning using linear physics

### Why



### Exemple case of position extrapolation

For player position we must send his position and his velocity 
expressed like : `position: Vector2 { x, y }` & `velocity: Vector2 { x, y }`

In an exemple case where the player position is `{1, 5}` and he is moving to x at a speed of 5

The packet data must look like this : 
```js
{
    position: { x: 1, y: 5 },
    velocity: { x: 5, y: 0 }
}
```

After packet reception while we are waiting for the next packet we must anticipate his next actions by applying his last velocity.
To keep a coherent speed   

So client side we come with a pseudocode looking like this :
```c++
Vector2 newPosition = { 1, 5 };
Vector2 velocity = { 5, 0 };

// time between data reception and last data reception
float dataReceptionDelta;
Player playerToUpdate;

void onDataReceived()
{
    playerToUpdate.transform.pos = newPosition;
}

void update(float renderDeltaTime) 
{
    // Without acceleration
    playerToUpdate.transform.pos += (velocity * dataReceptionDelta) * renderDeltaTime;
    
    // With acceleration
    // Pt = P0 + V0*T + (0.5 * A0*T²)
    lastKnown.pos + lastKnown.vel * elapsedTime 
                + (0.5 * lastKnown.pos * (elapsedTime * elapsedTime))

}
```
This is a very simple simplified way of doing dead reckoning who not consider a lot of cases, specially the correction part (we are going to talk about it a bit later).

In this exemple we're using a basic way of doing extrapolation: linear physics who can be expressed like :

<!--$$ {\displaystyle P_{t}=P_{0}+V_{0}T+{\frac {1}{2}}A_{0}T^{2}} $$-->
<img src="https://render.githubusercontent.com/render/math?math={\displaystyle P_{t}=P_{0}+V_{0}T+{\frac {1}{2}}A_{0}T^{2}}">

