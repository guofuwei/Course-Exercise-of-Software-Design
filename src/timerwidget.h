#ifndef TIMERWIDGET_H
#define TIMERWIDGET_H

#include <QLabel>
#include <QWidget>

class TimerWidget : public QWidget {
  Q_OBJECT
 public:
  explicit TimerWidget(QWidget* parent = nullptr);
  void setInitTime(int hour, int minute, int second);  // 设置初始函数
  void reset();                                        // 重置计时时间
  void start();                                        // 开始计时
  bool IsRun() { return m_is_run; }  // 判断计时器是否在运行

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
  QTimer* timer_;  // 定时器
 public slots:
  void showTime();  // 展示时间

 private:
  void increment();
  QString toShowText();
};

#endif  // TIMERWIDGET_H
