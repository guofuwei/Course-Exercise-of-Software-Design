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

#include "timerwidget.h"

class AudioRecord : public QObject {
  Q_OBJECT
 public:
  AudioRecord();
  AudioRecord(QPlainTextEdit* plaintextedit, TimerWidget* timerwidget);
  ~AudioRecord();
  void AudioLog(QString content);

 private:
  QFile m_dest_file;
  QFile m_source_file;
  bool m_is_recording;
  bool m_is_playing;
  QPlainTextEdit* m_plaintextedit_log;
  TimerWidget* m_timerwidget;
  QString m_dest_file_path;
  QString m_dest_filename;
  QString m_source_file_path;
  QString m_source_filename;
  QAudioFormat m_audio_input_format;
  QAudioInput* m_audio_in;
  QAudioOutput* m_audio_out;
 public slots:
  bool StartRecording();
  bool StopRecording();
  bool StartPlaying();
  bool StopPlaying();
  void HandleInputStateChanged(QAudio::State new_state);
  void HandleOutStateChanged(QAudio::State new_state);
};

#endif  // AUDIORECORD_H
