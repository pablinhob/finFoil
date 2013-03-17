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

#ifndef EDITABLEPATH_H
#define EDITABLEPATH_H

#include "patheditorfwd/patheditorfwd.h"

#include <QLinkedList>
#include "pathitem.h"
#include "pathsettings.h"

namespace patheditor
{
    /**
     * @brief A compound path, build by PathItems, that can be edited by dragging control points
     */
    class EditablePath : public QGraphicsObject
    {
        Q_OBJECT
    public:
        explicit EditablePath(QGraphicsItem * parent = 0);

        /**
         * @brief append Append a new path item to the editable path
         * @param pathItem PathItem to append
         */
        virtual void append(QSharedPointer<PathItem> pathItem);

        // Implementing QGraphicsItem
        virtual QRectF boundingRect() const;
        virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                        QWidget *widget);

        /**
         * @brief get the last drawn QPainterPath. The caller takes ownership of the pointer.
         *        Typically used when pathChanged is emitted.
         * @return
         */
        QPainterPath* takePainterPath();

    signals:
        void pathChanged(EditablePath *sender);

    private:
        PathSettings _settings;

        QScopedPointer<QPainterPath> _painterPath;

        QLinkedList<QSharedPointer<PathItem> > _pathItemList;
    };
}

#endif // EDITABLEPATH_H
