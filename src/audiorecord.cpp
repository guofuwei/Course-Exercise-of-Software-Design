#include "audiorecord.h"

#include <QAudio>
#include <QDebug>
#include <QDir>
AudioRecord::AudioRecord() {
  m_dest_file_path = QDir::currentPath() + "/comment/audio";
  m_dest_filename = "temp.pcm";
  m_source_file_path = QDir::currentPath() + "/comment/audio";
  m_source_filename = "temp.pcm";
  m_is_recording = false;
  m_is_playing = false;
  m_plaintextedit_log = NULL;
  m_timerwidget = NULL;
  // 设置录音的格式
  m_audio_input_format.setSampleRate(16000);  // 设置采样率以对赫兹采样。
  m_audio_input_format.setChannelCount(1);  // 将通道数设置为通道。
  m_audio_input_format.setSampleSize(
      16); /*将样本大小设置为指定的sampleSize（以位为单位）通常为8或16，但是某些系统可能支持更大的样本量。*/
  m_audio_input_format.setCodec("audio/pcm");  // 设置编码格式
  m_audio_input_format.setByteOrder(
      QAudioFormat::LittleEndian);  // 样本是小端字节顺序
  m_audio_input_format.setSampleType(
      QAudioFormat::UnSignedInt);  // 样本是无符号整数
}
AudioRecord::AudioRecord(QPlainTextEdit* plaintextedit,
                         TimerWidget* timerwidget) {
  m_dest_file_path = QDir::currentPath() + "/comment/audio";
  m_dest_filename = "temp.pcm";
  m_source_file_path = QDir::currentPath() + "/comment/audio";
  m_source_filename = "temp.pcm";
  m_is_recording = false;
  m_is_playing = false;
  m_plaintextedit_log = plaintextedit;
  m_timerwidget = timerwidget;
  // 设置录音的格式
  m_audio_input_format.setSampleRate(16000);  // 设置采样率以对赫兹采样。
  m_audio_input_format.setChannelCount(1);  // 将通道数设置为通道。
  m_audio_input_format.setSampleSize(
      16); /*将样本大小设置为指定的sampleSize（以位为单位）通常为8或16，但是某些系统可能支持更大的样本量。*/
  m_audio_input_format.setCodec("audio/pcm");  // 设置编码格式
  m_audio_input_format.setByteOrder(
      QAudioFormat::LittleEndian);  // 样本是小端字节顺序
  m_audio_input_format.setSampleType(
      QAudioFormat::UnSignedInt);  // 样本是无符号整数
}

void AudioRecord::AudioLog(QString content) {
  if (m_plaintextedit_log == NULL) {
    return;
  }
  m_plaintextedit_log->insertPlainText(content);
}

AudioRecord::~AudioRecord() { delete m_audio_in; }

bool AudioRecord::StartRecording() {
  if (m_is_recording) {
    return false;
  }
  QAudioDeviceInfo info = QAudioDeviceInfo::defaultInputDevice();
  AudioLog(QString("当前的录音设备的名字:%1\n").arg(info.deviceName()));
  // 判断输入的格式是否支持，如果不支持就使用系统支持的默认格式
  if (!info.isFormatSupported(m_audio_input_format)) {
    m_audio_input_format = info.nearestFormat(m_audio_input_format);
  }
  m_audio_in = new QAudioInput(m_audio_input_format, this);
  connect(m_audio_in, SIGNAL(stateChanged(QAudio::State)), this,
          SLOT(HandleInputStateChanged(QAudio::State)));
  if (m_audio_in->state() == QAudio::StoppedState) {
    // 设置采集的时间
    m_dest_file.setFileName(m_dest_file_path + "/" + m_dest_filename);
    m_dest_file.open(QIODevice::WriteOnly | QIODevice::Truncate);
    m_audio_in->start(&m_dest_file);
  }
  m_is_recording = true;
  return true;
}
bool AudioRecord::StopRecording() {
  if (!m_is_recording) {
    return false;
  }
  m_is_recording = false;
  AudioLog("停止录音.\n");
  m_audio_in->stop();
  m_dest_file.close();
  return true;
}

void AudioRecord::HandleInputStateChanged(QAudio::State new_state) {
  switch (new_state) {
    case QAudio::StoppedState:
      if (m_audio_in->error() != QAudio::NoError) {
        AudioLog("录音出现错误.\n");
      } else {
        AudioLog("完成录音，文件已保存到:" + m_dest_file_path + "/" +
                 m_dest_filename + "\n");
      }
      break;
    case QAudio::ActiveState:
      AudioLog("开始从IO设备读取PCM声音数据.\n");
      break;
    default:
      break;
  }
}

bool AudioRecord::StartPlaying() {
  if (m_is_playing) {
    return false;
  }
  QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
  if (!info.isFormatSupported(m_audio_input_format)) {
    AudioLog("后端不支持原始音频格式，无法播放音频.\n");
    return false;
  }
  m_audio_out = new QAudioOutput(m_audio_input_format, this);
  connect(m_audio_out, SIGNAL(stateChanged(QAudio::State)), this,
          SLOT(HandleOutStateChanged(QAudio::State)));
  m_source_file.setFileName(m_source_file_path + "/" + m_source_filename);
  if (!m_source_file.open(QIODevice::ReadOnly)) {
    AudioLog("打开音频源文件失败\n");
    return false;
  }
  m_audio_out->start(&m_source_file);
  AudioLog("开始播放\n");
  m_is_playing = true;
  return true;
}

bool AudioRecord::StopPlaying() {
  if (!m_is_playing) {
    return false;
  }
  m_is_playing = false;
  AudioLog("停止播放.\n");
  m_audio_out->stop();
  m_source_file.close();
  return true;
}

// 播放音频的反馈信息
void AudioRecord::HandleOutStateChanged(QAudio::State newState) {
  switch (newState) {
    case QAudio::IdleState:
      m_audio_out->stop();
      m_source_file.close();
      m_is_playing = false;
      AudioLog("音频播放完成.\n");
      m_timerwidget->reset();
      break;
    case QAudio::StoppedState:
      if (m_audio_out->error() != QAudio::NoError) {
        AudioLog("播放音频出现错误.\n");
      }
      break;
    default:
      break;
  }
}
