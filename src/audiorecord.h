#ifndef AUDIORECORD_H
#define AUDIORECORD_H
#include <QAudio>  //这五个是QT处理音频的库
#include <QAudioFormat>
#include <QAudioInput>
#include <QDir>
#include <QFile>
#include <QIODevice>
#include <QPlainTextEdit>

class AudioRecord : public QObject {
  Q_OBJECT
 public:
  AudioRecord();
  AudioRecord(QPlainTextEdit* plaintextedit);

 private:
  QFile m_dest_file;
  bool m_is_recording;
  QPlainTextEdit* m_plaintextedit_log;
  QString m_dest_file_path;
  QAudioFormat m_audio_input_format;
  QAudioInput* m_audio_in;
  void AudioLog(QString content);
 public slots:
  void StartRecording();
  void StopRecording();
};

#endif  // AUDIORECORD_H
