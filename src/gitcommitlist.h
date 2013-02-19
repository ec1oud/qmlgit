

#ifndef GITCOMMITLIST_H
#define GITCOMMITLIST_H

#include <git2.h>
#include <QAbstractListModel>

class Worker : public QObject
{
    Q_OBJECT
public:
    Worker(git_repository *repo, const QString &branch) : m_repo(repo), m_branch(branch) {}

    QVector<git_commit*> commits() { return m_commits; }
public slots:
    void loadBranch();
private:
    QVector<git_commit*> m_commits;
    git_repository *m_repo;
    QString m_branch;
signals:
    void done();
};

class GitCommitList : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QString branch READ branch WRITE setBranch NOTIFY branchChanged)

public:
    enum Roles {
        Oid = Qt::UserRole + 1,
        Author,
        AuthorEmail,
        Committer,
        CommitterEmail,
        Message,
        ShortMessage,
        ParentCount,
        Time
    };

    GitCommitList(git_repository *repo);
    ~GitCommitList();

    QHash<int, QByteArray> roleNames() const;

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    void setRepo(git_repository *repo);

    void setBranch(const QString &branch);
    QString branch() const;


Q_SIGNALS:
    void branchChanged();
private Q_SLOTS:
    void branchLoaded();

private:
    void update();
    void loadBranch();
    void clear();

    git_repository *m_repo;
    QString m_branch;

    QVector<git_commit*> m_commits;
    Worker *m_worker;
};

#endif
