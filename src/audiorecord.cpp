#include "audiorecord.h"

#include <QAudio>
#include <QDebug>
#include <QDir>
AudioRecord::AudioRecord() {
  m_dest_file_path = QDir::currentPath();
  m_is_recording = false;
  m_plaintextedit_log = NULL;
}
AudioRecord::AudioRecord(QPlainTextEdit* plaintextedit) {
  m_dest_file_path = QDir::currentPath();
  qDebug() << "CurrentPath:" << m_dest_file_path;
  m_is_recording = false;
  m_plaintextedit_log = plaintextedit;
}

void AudioRecord::AudioLog(QString content) {
  if (m_plaintextedit_log == NULL) {
    return;
  }
  m_plaintextedit_log->insertPlainText(content);
}

void AudioRecord::StartRecording() {
  m_is_recording = true;
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
  QAudioDeviceInfo info = QAudioDeviceInfo::defaultInputDevice();
  AudioLog(QString("当前的录音设备的名字:%1\n").arg(info.deviceName()));
  // 判断输入的格式是否支持，如果不支持就使用系统支持的默认格式
  if (!info.isFormatSupported(m_audio_input_format)) {
    m_audio_input_format = info.nearestFormat(m_audio_input_format);
    /*
     * 返回与系统支持的提供的设置最接近的QAudioFormat。
       这些设置由所使用的平台/音频插件提供。
       它们还取决于所使用的QAudio :: Mode。
    */
  }
  // 当前设备支持的编码
  AudioLog("当前设备支持的编码格式:\n");
  QStringList list = info.supportedCodecs();
  for (int i = 0; i < list.size(); i++) {
    AudioLog(list.at(i) + "\n");
  }
  AudioLog(
      QString("当前录音的采样率=%1\n").arg(m_audio_input_format.sampleRate()));
  AudioLog(QString("当前录音的通道数=%1\n")
               .arg(m_audio_input_format.channelCount()));
  AudioLog(QString("当前录音的样本大小=%1\n")
               .arg(m_audio_input_format.sampleSize()));
  AudioLog(
      QString("当前录音的编码格式=%1\n").arg(m_audio_input_format.codec()));
  m_audio_in = new QAudioInput(m_audio_input_format, this);
  connect(m_audio_in, SIGNAL(stateChanged(QAudio::State)), this,
          SLOT(handleStateChanged_input(QAudio::State)));
  if (m_audio_in->state() == QAudio::StoppedState) {
    // qDebug()<<"没有处理任何数据.\n";
    // 设置采集的时间
    //    m_dest_file.setFileName(m_dest_file_path + "/temp.pcm");
    //    m_dest_file.open(QIODevice::WriteOnly | QIODevice::Truncate);
    //    m_audio_in->start(&m_dest_file);
  }
}
void AudioRecord::StopRecording() {}
