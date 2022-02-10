import { Time } from "./main.js";

export class Airplane 
{
    constructor(config = {}, local = false)
    {
        this.transform = {
            pos: (config?.transform?.pos) 
                ? createVector(config.transform.pos.x, config.transform.pos.y)
                : createVector(width / 2, height / 2)
            ,

            vel: (config?.transform?.vel) 
                ? createVector(config.transform.vel.x, config.transform.vel.y)
                : createVector(0, 0)
            ,
            
            rot: config?.transform?.rot || 0,

            angularVel: config?.transform?.angularVel
        };
        
        this.pos = this.transform.pos;
        this.vel = this.transform.vel;

        this.rot = this.transform.rot;
        this.angularVel = this.transform.angularVel;

        this.diameter = config.diameter || 20;
        this.color = config.color || color(0, 255, 0);
        this.arrowLength = config.arrowLength || 25;

        this.local = local;

        if(!this.local)
        {
            this.net = {
                lastKnown: {
                    pos: this.pos.copy(),
                    vel: this.vel.copy()
                },
                lastReceivedTime: Time.now
            }
        }

        console.log('new ariplane', this);
    }

    update()
    {
        if(this.local)
        {
            this.clientInputs();
            this.pos.add(this.vel.copy().mult(Time.deltaSecond));
        } 
        else
        {
            this.pos = this.predictPostion();
        }
    }

    predictPostion()
    {
        // Pt = P0 + V0*T + (0.5 * A0*T²)

        const lastKnown = 
        {
            pos: this.net.lastKnown.pos.copy(),
            vel: this.net.lastKnown.vel.copy()
        };

        const elapsedTime = (Time.now - this.net.lastReceivedTime);

        const newPos = lastKnown.pos.copy()
        .add(lastKnown.vel.copy().mult(elapsedTime))
        .add(lastKnown.pos.copy().mult(0.5).mult(elapsedTime * elapsedTime));

        return newPos;
    }
    
    draw()
    {
        fill(this.color);
        circle(this.pos.x, this.pos.y, this.diameter);
        
        this.drawArrow(
            this.pos, 
            this.vel.copy().normalize().mult(this.arrowLength)
        );
    }
    
    drawArrow(pivot, vec, color = "red", lineWeight = 2, arrowSize = 3)
    {
        push();
        
        stroke(color);
        strokeWeight(lineWeight);
        fill(color);
        translate(pivot.x, pivot.y);
        line(0, 0, vec.x, vec.y);
        
        rotate(vec.heading());
        translate(vec.mag() - arrowSize, 0);
        triangle(0, arrowSize / 2, 0, -arrowSize / 2, arrowSize, 0);

        pop();
    }

    clientInputs()
    {
        
    }
}