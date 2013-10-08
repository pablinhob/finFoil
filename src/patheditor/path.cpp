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

#include "path.h"

#include <QVarLengthArray>
#include "pathfunctors.h"

using namespace patheditor;

Path::Path(QObject *parent) :
    QObject(parent)
{
}

void Path::append(QSharedPointer<PathItem> pathItem)
{
    // Set pathItem next and prev
    if (!_pathItemList.isEmpty())
    {
        QSharedPointer<PathItem> last = _pathItemList.last();
        last->setNextPathItem(pathItem);
        pathItem->setPrevPathItem(last);
    }

    if (!_pathItemList.isEmpty())
    {
        pathItem->setStartPoint(_pathItemList.last()->endPoint());
    }

    // Link endpoints to controlpoints
    if (pathItem->controlPoints().count() >= 2)
    {
        pathItem->startPoint()->addFollowingPoint(pathItem->controlPoints().first());
        pathItem->endPoint()->addFollowingPoint(pathItem->controlPoints().last());
    }

//    connectPoints(pathItem.data());
    emit onAppend(pathItem.data());
    _pathItemList.append(pathItem);
}

QList<QSharedPointer<PathItem> > Path::pathItems()
{
    return _pathItemList;
}

QRectF Path::controlPointRect() const
{
    if (_pathItemList.count() <= 0)
        return QRectF(0,0,0,0);

    QRectF retVal = _pathItemList.first()->controlPointRect();
    foreach(QSharedPointer<PathItem> item, _pathItemList)
    {
        retVal |= item->controlPointRect();
    }

    return retVal;
}

QPointF Path::pointAtPercent(qreal t) const
{
    int pathItemCount = _pathItemList.count();
    qreal itemRange = 1/qreal(pathItemCount);

    int item = 0;
    while (t > itemRange)
    {
        t -= itemRange;
        item++;
    }

    t = t/itemRange;

    return _pathItemList[item]->pointAtPercent(t);
}

qreal Path::minX(qreal *t_top, qreal percTol) const
{
    return extreme(Min, X, t_top, percTol);
}

qreal Path::maxX(qreal *t_top, qreal percTol) const
{
    return extreme(Max, X, t_top, percTol);
}

qreal Path::minY(qreal *t_top, qreal percTol) const
{
    return extreme(Min, Y, t_top, percTol);
}

qreal Path::maxY(qreal *t_top, qreal percTol) const
{
    return extreme(Max, Y, t_top, percTol);
}

qreal Path::area(int resolution) const
{
    qreal percStep = 1 / qreal(resolution);
    QVarLengthArray<QPointF, PATH_AREARES> points(resolution);
    qreal perc = 0;
    for (int i = 0; i < resolution; i++)
    {
        points[i] = pointAtPercent(perc);
        perc += percStep;
    }

    qreal area = 0;
    int j = 0;
    for (int i = 0; i < resolution; i++)
    {
        j = (i + 1) % resolution;
        area += points[i].x() * points[j].y();
        area -= points[j].x() * points[i].y();
    }

    return qAbs(area) / 2;
}

void Path::onPathChanged()
{
    emit pathChanged(this);
}

void Path::onPathReleased()
{
    emit pathReleased(this);
}

qreal Path::extreme(Ext ext, Dimension dimension, qreal *t_ext, qreal percTol) const
{
    qreal multiplier = 1;

    QScopedPointer<hrlib::func_mult_offset_base> target;

    switch (dimension)
    {
    case X:
        target.reset(new f_xValueAtPercentPath(this));
        break;
    case Y:
        target.reset(new f_yValueAtPercentPath(this));
        break;
    }

    if (ext == Max)
    {
        target->setMultiplier(-1);
        multiplier = -1;
    }


    // find the min of the path
    if (t_ext == 0)
    {
        qreal t = 0.5;
        return hrlib::Brent::local_min(0, 1, percTol, *(target.data()), t) * multiplier;
    }
    else
        return hrlib::Brent::local_min(0, 1, percTol, *(target.data()), *t_ext) * multiplier;
}
