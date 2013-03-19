
#include <QQuickTextDocument>
#include <QSyntaxHighlighter>

#include <qdebug.h>

class Highlighter : public QSyntaxHighlighter
{
public:
    Highlighter(QTextDocument *parent)
        : QSyntaxHighlighter(parent)
    {
        normal.setFontWeight(QFont::Light);
        normal.setForeground(QColor("#585858"));
        diffRemoved.setFontWeight(QFont::Normal);
        diffRemoved.setForeground(QColor("#D5271E"));
        diffAdded.setFontWeight(QFont::Normal);
        diffAdded.setForeground(Qt::darkGreen);
    }

    void highlightBlock(const QString & text)
    {
        if (text.isEmpty())
            return;

        if (text.at(0) == QLatin1Char('-')) {
            setFormat(0, text.length(), diffRemoved);
        } else if (text.at(0) == QLatin1Char('+')) {
            setFormat(0, text.length(), diffAdded);
        } else {
            setFormat(0, text.length(), normal);
        }
    }
private:
    QTextCharFormat diffAdded;
    QTextCharFormat diffRemoved;
    QTextCharFormat normal;
};

class DiffHighlighter : public QQuickItem
{
    Q_OBJECT

    Q_PROPERTY(QQuickItem *target READ target WRITE setTarget NOTIFY targetChanged)

public:
    DiffHighlighter() : m_highlighter(0)
    {}

    QQuickItem *target() { return m_target; }

    void setTarget(QQuickItem *target)
    {
        m_target = target;
        emit targetChanged();

        QVariant doc = m_target->property("textDocument");
        if (doc.canConvert<QQuickTextDocument*>()) {
            QQuickTextDocument *qqDoc = doc.value<QQuickTextDocument*>();

            if (qqDoc) {
                if (m_highlighter)
                    delete m_highlighter;
                m_highlighter = new Highlighter(qqDoc->textDocument());
            }
        }
    }

Q_SIGNALS:
    void targetChanged();

private:
    QQuickItem *m_target;
    QSyntaxHighlighter *m_highlighter;
};
