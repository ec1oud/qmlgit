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

#include "gitdragmodel.h"


bool FilterModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
//    if (m_sourceModel->index(source_row).data(GitModel::Oid) == m_dragCommit)
//        return false;
    return true;
}

void FilterModel::setSourceModel(GitModel *model)
{
    m_sourceModel = model;
    QSortFilterProxyModel::setSourceModel(m_sourceModel);
}

void FilterModel::startDrag(int row, const QString &oid)
{
    dragStartRow = row;
    dragRow = row;
    targetRow = row;
    lastTarget = row;
}

void FilterModel::dropTarget(int row, const QString &oid)
{
    qDebug() << "DRAG " << " current: " << targetRow << " ==> " << row << " last: " << lastTarget;
    if (row == targetRow || row == lastTarget)
        return;

    beginMoveRows(QModelIndex(), targetRow, targetRow, QModelIndex(), row);
    lastTarget = targetRow;
    targetRow = row;
    dragRow = row;
    endMoveRows();
}

QVariant FilterModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    if (row >= targetRow && row < dragRow)
        ++row;
    else if (row <= targetRow && row > dragRow)
        --row;

//    qDebug() << "Data for row: " << index.row() << " returns: " << row;

    return sourceModel()->data(sourceModel()->index(row), role);
}

void FilterModel::finishDragAndDrop()
{
    qDebug() << "Commit data: " << dragStartRow << " to: " << targetRow;

    sourceModel()->moveCommit(dragStartRow, targetRow);
    invalidate();
}

