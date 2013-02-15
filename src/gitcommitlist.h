

#ifndef GITCOMMITLIST_H
#define GITCOMMITLIST_H

#include <git2.h>

#include <QStandardItemModel>

class GitCommitList : public QStandardItemModel
{
public:
    GitCommitList()
    {}
};

#endif
