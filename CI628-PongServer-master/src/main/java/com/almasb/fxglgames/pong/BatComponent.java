/*
 * The MIT License (MIT)
 *
 * FXGL - JavaFX Game Library
 *
 * Copyright (c) 2015-2017 AlmasB (almaslvl@gmail.com)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

package com.almasb.fxglgames.pong;

import com.almasb.fxgl.dsl.FXGL;
import com.almasb.fxgl.entity.component.Component;
import com.almasb.fxgl.physics.PhysicsComponent;

/**
 * @author Almas Baimagambetov (AlmasB) (almaslvl@gmail.com)
 */
public class BatComponent extends Component {

    /**
     * Bat speed edited to make gameplay faster
     */
    private static final double BAT_SPEED = 540;
    private static final double BAT_MIDDLE_BUFFER = 50;
    private static final double BAT_EDGE_BUFFER = 85;


    protected PhysicsComponent physics;

    public void up() {
        if (entity.getY() >= BAT_SPEED / 80)
            physics.setVelocityY(-BAT_SPEED);
        else
            stop();
    }

    public void down() {
        if (entity.getBottomY() <= FXGL.getAppHeight() - (BAT_SPEED / 80))
            physics.setVelocityY(BAT_SPEED);
        else
            stop();
    }

    /**
     * - Added player movement on the x-axis
     * - Players are restricted on the X-axis to a small area (around 50 from each side and form the middle)
     *   by the Entity tags they use as they will always be the same.
     *   Players are restricted on each side like in real life ping pong
     *   A line or Normalizing code needs to be added
     *
     */
    public void right() {

        if (entity.getRightX() <= FXGL.getAppWidth() / 2 - 28 - BAT_MIDDLE_BUFFER && this.entity.isType(EntityType.PLAYER_BAT))
            physics.setVelocityX(BAT_SPEED);
        else if (entity.getRightX() <= FXGL.getAppWidth() - BAT_SPEED / 28 - BAT_EDGE_BUFFER && this.entity.isType(EntityType.ENEMY_BAT))
            physics.setVelocityX(BAT_SPEED);
        else
            stop();
    }

    public void left() {
        if (entity.getX() >= BAT_SPEED / 25 + BAT_EDGE_BUFFER && this.entity.isType(EntityType.PLAYER_BAT))
            physics.setVelocityX(-BAT_SPEED);
        else if (entity.getX() >= FXGL.getAppWidth() / 2 + BAT_SPEED / 28 + BAT_MIDDLE_BUFFER && this.entity.isType(EntityType.ENEMY_BAT))
            physics.setVelocityX(-BAT_SPEED);
        else
            stop();
    }

    public void stop() {
        physics.setLinearVelocity(0, 0);
    }
}
