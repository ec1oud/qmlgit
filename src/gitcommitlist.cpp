
#include "gitcommitlist.h"

#include <QDateTime>
#include <QDebug>

//// ### Revwalking
////
//// The libgit2 [revision walking api][rw] provides methods to traverse the directed graph created
//// by the parent pointers of the commit objects.  Since all commits point back to the commit that
//// came directly before them, you can walk this parentage as a graph and find all the commits that
//// were ancestors of (reachable from) a given starting point.  This can allow you to create `git log`
//// type functionality.
////
//// [rw]: http://libgit2.github.com/libgit2/#HEAD/group/revwalk

//printf("\n*Revwalking*\n");
//git_revwalk *walk;
//git_commit *wcommit;

//git_oid_fromstr(&oid, "f0877d0b841d75172ec404fc9370173dfffc20d1");

//// To use the revwalker, create a new walker, tell it how you want to sort the output and then push
//// one or more starting points onto the walker.  If you want to emulate the output of `git log` you
//// would push the SHA of the commit that HEAD points to into the walker and then start traversing them.
//// You can also 'hide' commits that you want to stop at or not see any of their ancestors.  So if you
//// want to emulate `git log branch1..branch2`, you would push the oid of `branch2` and hide the oid
//// of `branch1`.
//git_revwalk_new(&walk, repo);
//git_revwalk_sorting(walk, GIT_SORT_TOPOLOGICAL | GIT_SORT_REVERSE);
//git_revwalk_push(walk, &oid);

//const git_signature *cauth;
//const char *cmsg;

//// Now that we have the starting point pushed onto the walker, we can start asking for ancestors. It
//// will return them in the sorting order we asked for as commit oids.
//// We can then lookup and parse the commited pointed at by the returned OID;
//// note that this operation is specially fast since the raw contents of the commit object will
//// be cached in memory
//while ((git_revwalk_next(&oid, walk)) == 0) {
//  error = git_commit_lookup(&wcommit, repo, &oid);
//  cmsg  = git_commit_message(wcommit);
//  cauth = git_commit_author(wcommit);
//  printf("%s (%s)\n", cmsg, cauth->email);
//  git_commit_free(wcommit);
//}

//// Like the other objects, be sure to free the revwalker when you're done to prevent memory leaks.
//// Also, make sure that the repository being walked it not deallocated while the walk is in
//// progress, or it will result in undefined behavior
//git_revwalk_free(walk);



GitCommitList::GitCommitList(git_repository *repo)
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
    beginResetModel();
    if (repo) {
        m_repo = repo;
        loadBranch();
    }
    endResetModel();
}

void GitCommitList::setBranch(const QString &branch)
{
    beginResetModel();
    m_branch = branch;
    loadBranch();
    emit branchChanged();
    endResetModel();
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

void GitCommitList::loadBranch()
{
    if (!m_repo)
        return;

    foreach (git_commit *commit, m_commits)
        git_commit_free(commit);
    m_commits.clear();

    if (m_branch.isEmpty()) {
        qDebug() << "Warning, no branch specified.";
        return;
    }

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
}
