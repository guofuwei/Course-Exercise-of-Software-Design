#ifndef AUDIORECORD_H
#define AUDIORECORD_H
#include <QAudio>  //这五个是QT处理音频的库
#include <QAudioFormat>
#include <QAudioInput>
#include <QAudioOutput>
#include <QDir>
#include <QFile>
#include <QIODevice>
#include <QPlainTextEdit>

#include "texteditor.h"
#include "timerwidget.h"

class AudioRecord : public QObject {
  Q_OBJECT
 public:
  AudioRecord();
  AudioRecord(QPlainTextEdit* plaintextedit, TimerWidget* timerwidget);
  ~AudioRecord();
  void AudioLog(QString content);  // 输出到录音日志
  void SetFilename(QString filename) {
    m_open_filename = filename;
  }  // 设置打开的文件名
  void SetTextEditor(TextEditor* texteditor) {
    m_texteditor = texteditor;
  }  // 传入代码编辑器

 private:
  TextEditor* m_texteditor;
  QString m_open_filename;
  // 目标文件和原文件
  QFile m_dest_file;
  QFile m_source_file;
  // 标志位，标识是否播放或录音
  bool m_is_recording;
  bool m_is_playing;
  // 指向外部的日志和计时器模块
  QPlainTextEdit* m_plaintextedit_log;
  TimerWidget* m_timerwidget;
  // 目标和原文件的路径及文件名
  QString m_dest_file_path;
  QString m_dest_filename;
  QString m_source_file_path;
  QString m_source_filename;
  // 输入和输出的音频
  QAudioFormat m_audio_input_format;
  QAudioInput* m_audio_in;
  QAudioOutput* m_audio_out;
 public slots:
  bool StartRecording();
  bool StopRecording();
  bool StartPlaying();
  bool StopPlaying();
  // 处理状态变化
  void HandleInputStateChanged(QAudio::State new_state);
  void HandleOutStateChanged(QAudio::State new_state);
};

#endif  // AUDIORECORD_H
