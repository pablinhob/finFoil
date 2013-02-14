/****************************************************************************
  
 Copyright (c) 2013, Hans Robeers
 All rights reserved.
 
 BSD 2-Clause License
 
 Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 
   * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
   
   * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
   
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 
****************************************************************************/

#include "line.h"

#include <QPainter>
#include <QGraphicsItem>
#include "pathpoint.h"
#include "pathsettings.h"

using namespace patheditor;

Line::Line(QSharedPointer<PathPoint> startPoint, QSharedPointer<PathPoint> endPoint,
           QGraphicsItem *parent, QGraphicsScene *scene)
    : PathItem(startPoint, endPoint, parent, scene)
{
}

QList<QSharedPointer<PathPoint> > Line::controlPoints()
{
    return _controlPoints;
}

QRectF Line::boundingRect() const
{
    return _boundingRect;
}

void Line::paint(QPainter *painter, const QStyleOptionGraphicsItem * /*unused*/, QWidget * /*unused*/)
{
    QPainterPath painterPath;

    painterPath.moveTo(*startPoint());
    painterPath.lineTo(*endPoint());
    painter->drawPath(painterPath);

    _boundingRect = painterPath.boundingRect();
}

void Line::paint(PathSettings *settings, QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(settings->linePen());

    paint(painter, option, widget);
}