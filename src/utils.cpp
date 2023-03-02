#include <utils.h>

#include <QDebug>
namespace Cesd {
bool matches(const QStringList &regexpatterns, const QString &string,
             QRegExp::PatternSyntax syntax) {
  foreach (const auto &regex, regexpatterns) {
    // 判断字符串是否包含正则内容
    if (string.contains(QRegExp(regex, Qt::CaseSensitive, syntax))) {
      return true;
    }
  }
  return false;
}

int GetCommentStartPos(QString code, int type) {
  if (type == 1) {
    return code.lastIndexOf("[COMMENT-PIC-");
  } else if (type == 2) {
    //    qDebug() << code.indexOf("[COMMENT-AUDIO-");
    //    qDebug() << code.lastIndexOf("[COMMENT-AUDIO-");
    return code.lastIndexOf("[COMMENT-AUDIO-");
  } else {
    return -1;
  }
}
int GetCommentStopPos(QString code, int start_pos) {
  //  qDebug() << code.lastIndexOf("]");
  return code.indexOf("]", start_pos);
}

QList<QString> ParseComment(QString comment, int type) {
  QList<QString> result;
  if (type == 1) {
    comment = comment.mid(QString("[COMMENT-PIC-").size());
    int pos = comment.indexOf("-");
    result.push_back(comment.mid(0, pos));
    comment = comment.mid(pos + 1);
    result.push_back(comment.mid(0, comment.size() - 1));
  } else {
    comment = comment.mid(QString("[COMMENT-AUDIO-").size());
    int pos = comment.indexOf("-");
    result.push_back(comment.mid(0, pos));
    comment = comment.mid(pos + 1);
    result.push_back(comment.mid(0, comment.size() - 1));
  }
  return result;
}
}  // namespace Cesd
