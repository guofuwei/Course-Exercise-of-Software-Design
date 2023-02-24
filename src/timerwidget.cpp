#include "timerwidget.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QTimer>

#define MAX_SECOND 60
#define MAX_MINUTE 60
#define MAX_HOUR 24

TimerWidget::TimerWidget(QWidget* parent)
    : QWidget(parent),
      m_hour(0),
      m_minute(0),
      m_second(0),
      m_is_run(false),
      format_("%1: %2: %3"),
      timer_(NULL) {
  time_lbl_ = new QLabel(this);
  // time_lbl_->setText("fdfdfdsfdfdfd");
  time_lbl_->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
  // time_lbl_->setStyleSheet(QString("QLabel{font-size:48px;}"));

  setContentsMargins(0, 0, 0, 0);
  QHBoxLayout* hMainLayout = new QHBoxLayout;
  hMainLayout->addStretch();
  hMainLayout->addWidget(time_lbl_);
  hMainLayout->addStretch();
  setLayout(hMainLayout);
}

QString TimerWidget::toShowText() {
  format_ = "%1: %2: %3";
  sh_ = QString::number(m_hour);
  sm_ = QString::number(m_minute);
  ss_ = QString::number(m_second);
  if (sh_.size() < 2) {
    sh_.push_front('0');
  }
  if (sm_.size() < 2) {
    sm_.push_front('0');
  }
  if (ss_.size() < 2) {
    ss_.push_front('0');
  }
  show_text_ = format_.arg(sh_, sm_, ss_);  //.arg(sm_).arg(ss_);
  return show_text_;
}

void TimerWidget::increment() {
  if (m_second >= MAX_SECOND) {
    m_second = 0;
    m_minute += 1;
  }
  if (m_minute >= MAX_MINUTE) {
    m_minute = 0;
    m_hour += 1;
  }
  if (m_hour >= MAX_HOUR) {
    m_hour = 0;
  }
}

void TimerWidget::start() {
  if (m_is_run) {
    return;
  }
  m_is_run = true;
  timer_ = new QTimer(this);
  connect(timer_, SIGNAL(timeout()), this, SLOT(showTime()));
  timer_->start(1000);

  showTime();
}

void TimerWidget::showTime() {
  time_lbl_->setText(toShowText());
  m_second += 1;
  increment();
  update();
}

void TimerWidget::setInitTime(int hour, int minute, int second) {
  m_hour = hour;
  m_minute = minute;
  m_second = second;
}

void TimerWidget::reset() {
  if (timer_) {
    timer_->stop();
  }
  m_is_run = false;
  m_hour = 0;
  m_minute = 0;
  m_second = 0;
}
