#include <utils.h>
namespace Cesd {
  bool matches(const QStringList &regexpatterns, const QString &string, QRegExp::PatternSyntax syntax)
  {
    foreach (const auto &regex, regexpatterns) {
      if (string.contains(QRegExp(regex, Qt::CaseSensitive, syntax))) {
        return true;
      }
    }
    return false;
  }
}
