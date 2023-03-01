#ifndef UTILS_H
#define UTILS_H

#endif  // UTILS_H
#include <QString>
#include <QStringList>

namespace Cesd {
bool matches(const QStringList &regexpatterns, const QString &string,
             QRegExp::PatternSyntax syntax);

int GetCommentStartPos(QString code, int type);
int GetCommentStopPos(QString code, int start_pos = 0);

QList<QString> ParseComment(QString comment, int type);
}  // namespace Cesd
