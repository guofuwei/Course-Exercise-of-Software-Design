#ifndef UTILS_H
#define UTILS_H

#endif  // UTILS_H
#include <QString>
#include <QStringList>

namespace Cesd {
bool matches(const QStringList &regexpatterns, const QString &string,
             QRegExp::PatternSyntax syntax);

int GetCommentStartPos(QString code, int type);
int GetCommentStopPos(QString code);

QList<QString> ParseComment(QString comment, int type);
}  // namespace Cesd
