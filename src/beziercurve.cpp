/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of qmlgit.
**
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
****************************************************************************/


#include "beziercurve.h"

#include <QtQuick/qsgnode.h>
#include <QtQuick/qsgflatcolormaterial.h>

//! [1]
BezierCurve::BezierCurve(QQuickItem *parent)
    : QQuickItem(parent)
    , m_p1(0, 0)
    , m_p2(1, 0)
    , m_p3(0, 1)
    , m_p4(1, 1)
    , m_segmentCount(100)
    , m_item1(0)
    , m_item2(0)
{
    setFlag(ItemHasContents, true);
}
//! [1]

//! [2]
BezierCurve::~BezierCurve()
{
}
//! [2]

//! [3]
void BezierCurve::setP1(const QPointF &p)
{
    if (p == m_p1)
        return;

    m_p1 = p;
    emit p1Changed(p);
    update();
}
//! [3]

void BezierCurve::setP2(const QPointF &p)
{
    if (p == m_p2)
        return;

    m_p2 = p;
    emit p2Changed(p);
    update();
}

void BezierCurve::setP3(const QPointF &p)
{
    if (p == m_p3)
        return;

    m_p3 = p;
    emit p3Changed(p);
    update();
}

void BezierCurve::setP4(const QPointF &p)
{
    if (p == m_p4)
        return;

    m_p4 = p;
    emit p4Changed(p);
    update();
}

void BezierCurve::setSegmentCount(int count)
{
    if (m_segmentCount == count)
        return;

    m_segmentCount = count;
    emit segmentCountChanged(count);
    update();
}

void BezierCurve::setItem1(QQuickItem * arg)
{
    if (m_item1 != arg) {
        m_item1 = arg;

        connect(arg, SIGNAL(xChanged()), this, SLOT(updateShit()));
        connect(arg, SIGNAL(yChanged()), this, SLOT(updateShit()));
        connect(arg, SIGNAL(heightChanged()), this, SLOT(updateShit()));
        connect(arg, SIGNAL(widthChanged()), this, SLOT(updateShit()));

        emit item1Changed(arg);
        updateShit();
    }
}

void BezierCurve::setItem2(QQuickItem * arg)
{
    if (m_item2 != arg) {
        m_item2 = arg;

        connect(arg, SIGNAL(xChanged()), this, SLOT(updateShit()));
        connect(arg, SIGNAL(yChanged()), this, SLOT(updateShit()));
        connect(arg, SIGNAL(heightChanged()), this, SLOT(updateShit()));
        connect(arg, SIGNAL(widthChanged()), this, SLOT(updateShit()));

        printf("hey...\n");

        emit item2Changed(arg);
        updateShit();
    }
}


void BezierCurve::updateShit()
{
    printf("updating shit...");

    if (!m_item1 || !m_item2)
        return;

    QPointF c1 = m_item1->mapToItem(m_item1->parentItem(), m_item1->boundingRect().center());
    QPointF c2 = m_item2->mapToItem(m_item2->parentItem(), m_item2->boundingRect().center());

    setPosition(c1);
    setSize(QSizeF(c2.x() - c1.x(), c2.y() - c1.y()));

//    setP1();
//    setP2(m_item1->mapToItem(m_item1->parentItem(), m_item1->boundingRect().center()));
//    setP3(m_item2->mapToItem(m_item2->parentItem(), m_item2->boundingRect().center()));
//    setP4(m_item2->mapToItem(m_item2->parentItem(), m_item2->boundingRect().center()));


//    setWidth(m_p4.x() - m_p1.x());
//    setHeight(m_p4.y() - m_p1.y());
//    setX(m_p1.x());
//    setY(m_p1.y());
}

//! [4]
QSGNode *BezierCurve::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *)
{
    QSGGeometryNode *node = 0;
    QSGGeometry *geometry = 0;

    if (!oldNode) {
        node = new QSGGeometryNode;
//! [4] //! [5]
        geometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), m_segmentCount);
        geometry->setLineWidth(2);
        geometry->setDrawingMode(QSGGeometry::DrawLineStrip);
        node->setGeometry(geometry);
        node->setFlag(QSGNode::OwnsGeometry);
//! [5] //! [6]
        QSGFlatColorMaterial *material = new QSGFlatColorMaterial;
        material->setColor(QColor(255, 0, 0));
        node->setMaterial(material);
        node->setFlag(QSGNode::OwnsMaterial);
//! [6] //! [7]
    } else {
        node = static_cast<QSGGeometryNode *>(oldNode);
        geometry = node->geometry();
        geometry->allocate(m_segmentCount);
    }
//! [7]

//! [8]
    QRectF bounds = boundingRect();
    QSGGeometry::Point2D *vertices = geometry->vertexDataAsPoint2D();
    for (int i = 0; i < m_segmentCount; ++i) {
        qreal t = i / qreal(m_segmentCount - 1);
        qreal invt = 1 - t;

        QPointF pos = invt * invt * invt * m_p1
                    + 3 * invt * invt * t * m_p2
                    + 3 * invt * t * t * m_p3
                    + t * t * t * m_p4;

        float x = bounds.x() + pos.x() * bounds.width();
        float y = bounds.y() + pos.y() * bounds.height();

        qDebug() << x << y << m_p1 << m_p2 << m_p3 << m_p4;

        vertices[i].set(x, y);
    }
    node->markDirty(QSGNode::DirtyGeometry);
//! [8]


//! [9]
    return node;
}
//! [9]

