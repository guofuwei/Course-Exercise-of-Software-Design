#ifndef TIMERWIDGET_H
#define TIMERWIDGET_H

#include <QLabel>
#include <QWidget>

class TimerWidget : public QWidget {
  Q_OBJECT
 public:
  explicit TimerWidget(QWidget* parent = nullptr);
  void setInitTime(int hour, int minute, int second);
  void reset();
  void start();
  bool IsRun() { return m_is_run; }

 private:
  int m_hour;
  int m_minute;
  int m_second;
  bool m_is_run;
  QLabel* time_lbl_;

  QString sh_;
  QString sm_;
  QString ss_;
  QString show_text_;
  QString format_;

  QTimer* timer_;

 public slots:
  void showTime();

 private:
  void increment();
  QString toShowText();
};

#endif  // TIMERWIDGET_H
