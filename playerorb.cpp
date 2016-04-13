#include "playerorb.h"
#include <QKeyEvent> // for key press events
#include <QTimer> //for timers to loop the move() method
#include <iostream> // purely for help finding bugs by printing stuff out
#include <QGraphicsScene> // so this->scene() can be used
#include <QList> // for list of colliding objects
#include "aiorb.h"
#include <typeinfo> // used to make sure we dont decrease the player when decreasing ai orbs
#include <QRectF>
#include <QPixmap>
#include <QGraphicsView>
#include <math.h>

PlayerOrb::PlayerOrb()
{
    // these should probably be paramaterized at some point
    radius = 40;
    imageSource = ":/images/resources/orbPlaceholder.png";
    setPixmap(QPixmap(imageSource).scaled(radius*2,radius*2));
    setPos(1000, 800);
    setAcceleration(.5);
    setMaxVelocity(5);

}

void PlayerOrb::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Left || event->key() == Qt::Key_A)
        keyDirection[0] = true;
    else if (event->key() == Qt::Key_Right || event->key() == Qt::Key_D)
        keyDirection[1] = true;
    else if (event->key() == Qt::Key_Up || event->key() == Qt::Key_W)
        keyDirection[2] = true;
    else if (event->key() == Qt::Key_Down || event->key() == Qt::Key_S)
        keyDirection[3] = true;
}

void PlayerOrb::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Left || event->key() == Qt::Key_A)
        keyDirection[0] = false;
    else if (event->key() == Qt::Key_Right || event->key() == Qt::Key_D)
        keyDirection[1] = false;
    else if (event->key() == Qt::Key_Up || event->key() == Qt::Key_W)
        keyDirection[2] = false;
    else if (event->key() == Qt::Key_Down || event->key() == Qt::Key_S)
        keyDirection[3] = false;

    if (event->key() == Qt::Key_F)
    {
        QGraphicsView * view = scene()->views()[0];
        if (view->isFullScreen())
            view->showNormal();
        else
            view->showFullScreen();
    }
    else if (event->key() == Qt::Key_Escape)
    {
        QGraphicsView * view = scene()->views()[0];
        view->showNormal();
    }


}

void PlayerOrb::grow()
{
    // checks if there is anything in the grow queue. if so, increase the size by the next amount in the queue
    if (growQueue.size() > 0)
    {
        setRadius(radius + growQueue.front());
        growQueue.pop();
    }
}

void PlayerOrb::growBy(qreal amount)
{
    for (int j=0; j<5; j++)
        growQueue.push(amount/5);
}

void PlayerOrb::move()
{
    // Check collisions
    QList<QGraphicsItem *> collisions = collidingItems(); // collidingItems(); provides a QList of QGraphicsItem * that this item is colliding with
    for (int i = 0; i < collisions.size(); i++)
    {
        AIOrb * current = (AIOrb*)collisions[i]; // cast the colliding objects as AIOrb references so their member functions can be accessed
        qreal aiRadius = current->getRadius();

        if (radius > aiRadius) // if the AIOrb is smaller
        {
            scene()->removeItem(collisions[i]);
            delete current; // delete the orb we just ate

            // The radius to be added -- calculated in terms of area
            qreal radiusDiff = sqrt( (double) (radius*radius + aiRadius*aiRadius) ) - radius;
            growBy(radiusDiff);
        }
    }

    // MOVEMENT
    // Slow down or speed up
    for (int i = 0; i < 4; i++)
    {
        if (!keyDirection[i]) //if key is not being pressed
            dirVelocity[i] -= .2; //decrease speed
        else
            dirVelocity[i] += getAcceleration(); //increase speed
    }
    verifyVelocities();

    // Set the x and y velocities based on right/left up/down velocities
    xVel = dirVelocity[1] - dirVelocity[0]; // range of -max to max velocity
    yVel = dirVelocity[3] - dirVelocity[2];

    // Move in a direction
    setPos(x() + xVel, y() + yVel);

    // Grow the player if there is something in the growQueue
    grow();

    // Keep player inside window
    if (x() < 0)
        setPos(0, y());
    else if (x() + 2*radius > scene()->width())
        setPos(scene()->width() - 2*radius, y());
    if (y() < 0)
        setPos(x(), 0);
    else if (y() + 2*radius > scene()->height())
        setPos(x(), scene()->height() - 2*radius);

}
