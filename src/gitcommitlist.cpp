
#include "gitcommitlist.h"

#include <QDateTime>
#include <QThread>
#include <QDebug>



GitCommitList::GitCommitList(git_repository *repo)
    : m_worker(0)
{
    setRepo(repo);
}

GitCommitList::~GitCommitList()
{
    foreach (git_commit *commit, m_commits)
        git_commit_free(commit);
    m_commits.clear();
}

QHash<int, QByteArray> GitCommitList::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles.insert(Oid, "oid");
    roles.insert(Author, "author");
    roles.insert(AuthorEmail, "authorEmail");
    roles.insert(Committer, "committer");
    roles.insert(CommitterEmail, "committerEmail");
    roles.insert(Message, "message");
    roles.insert(ShortMessage, "shortMessage");
    roles.insert(ParentCount, "parentCount");
    roles.insert(Time, "time");
    return roles;
}

void GitCommitList::setRepo(git_repository *repo)
{
    m_repo = repo;
    update();
}

void GitCommitList::setBranch(const QString &branch)
{
    m_branch = branch;
    update();
}

void GitCommitList::update()
{
    beginResetModel();
    clear();
    endResetModel();

    if (m_repo && !m_branch.isEmpty())
        loadBranch();
}

void GitCommitList::branchLoaded()
{
    beginResetModel();
    m_commits = m_worker->commits();
    delete m_worker;
    m_worker = 0;
    endResetModel();
    emit branchChanged();
}

QString GitCommitList::branch() const
{
    return m_branch;
}

int GitCommitList::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    if (!m_repo)
        return 0;

    return m_commits.count();
}

QVariant GitCommitList::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    QVariant ret;
    git_commit *commit = m_commits.at(index.row());

    switch (role) {
    case Message: {
        ret = QString(git_commit_message(commit));
        break;
    }
    case ShortMessage: {
        QString msg = QString(git_commit_message(commit));
        int index = msg.indexOf('\n');
        ret = msg.left(index);
        break;
    }
    case Author: {
        const git_signature *cauth = git_commit_author(commit);
        ret = QString(cauth->name);
        break;
    }
    case AuthorEmail: {
        const git_signature *cauth = git_commit_author(commit);
        ret = QString(cauth->email);
        break;
    }
    case Committer: {
        const git_signature *com = git_commit_author(commit);
        ret = QString(com->name);
        break;
    }
    case CommitterEmail: {
        const git_signature *com = git_commit_author(commit);
        ret = QString(com->email);
        break;
    }
    case ParentCount:
        ret = git_commit_parentcount(commit);
        break;
    case Time: {
        QDateTime time;
        git_time_t timeT = git_commit_time(commit);
        time.setTime_t(timeT);
        ret = QVariant(time);
        break;
    }
    case Oid: {
        const git_oid *oid = git_commit_id(commit);
        char out[41];
        out[40] = '\0';
        git_oid_fmt(out, oid);
        ret = QString(out);
        break;
    }
    }
    return ret;
}

void GitCommitList::clear()
{
    foreach (git_commit *commit, m_commits)
        git_commit_free(commit);
    m_commits.clear();
}


void GitCommitList::loadBranch()
{
    m_worker = new Worker(m_repo, m_branch);

    QThread *workerThread = new QThread(this);

    connect(workerThread, &QThread::started, m_worker, &Worker::loadBranch);
    connect(m_worker, &Worker::done, workerThread, &QThread::quit);
    connect(workerThread, &QThread::finished, this, &GitCommitList::branchLoaded);

    m_worker->moveToThread(workerThread);

    workerThread->start();
}

void Worker::loadBranch()
{
    Q_ASSERT(m_repo);
    Q_ASSERT(!m_branch.isEmpty());

    git_reference *ref;
    int error = git_reference_lookup(&ref, m_repo, m_branch.toUtf8().constData());
    if (error != 0) {
        qWarning() << "Looking up reference failed: " + m_branch;
        return;
    }

    git_oid oid;
    error = git_reference_name_to_id(&oid, m_repo, m_branch.toUtf8().constData());
    Q_ASSERT(!error);
    git_reference_free(ref);

    git_revwalk *walk;
    git_commit *wcommit;

    git_revwalk_new(&walk, m_repo);
    error = git_revwalk_push(walk, &oid);
    Q_ASSERT(error == 0);

    while ((git_revwalk_next(&oid, walk)) == 0) {
        error = git_commit_lookup(&wcommit, m_repo, &oid);
        if (!error)
            m_commits.append(wcommit);
        else
            qWarning() << "Error looking up commit!";
    }

    git_revwalk_free(walk);
    emit done();
}
