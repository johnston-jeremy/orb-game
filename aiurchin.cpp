#include <QTimer>
#include <QGraphicsScene>
#include <iostream>
#include <QList>
#include <QPixmap>
#include <typeinfo>
#include "playerorb.h"


AIUrchin::AIUrchin()
{
  */create urchin here*/
  radius = 40;
  setPixmap(QPixmap(":/images/resources/urchin.png").scaled(radius*2,radius*2)); // need to upload an image for urchin orb
  setPos(qrand() % 750, qrand() % 550);
}


void AIUrchin::shrink()
{
  */method for shrinking player or aiorbs if hit */
}
