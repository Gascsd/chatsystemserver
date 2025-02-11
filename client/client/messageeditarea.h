#ifndef MESSAGEEDITAREA_H
#define MESSAGEEDITAREA_H

#include <QPlainTextEdit>
#include <QPushButton>
#include <QWidget>
#include "model/data.h"
class MessageEditArea : public QWidget
{
    Q_OBJECT
public:
    explicit MessageEditArea(QWidget *parent = nullptr);

    void initSignalSlot();
    void SendTextMessage();
    void addSelfMessage(model::MessageType messageType, const QByteArray &content, const QString &extraInfo);

private:
    QPushButton* sendImageBtn;
    QPushButton* sendFileBtn;
    QPushButton* sendSpeechBtn;
    QPushButton* showHistoryBtn;
    QPlainTextEdit* textEdit;
    QPushButton* sendTextBtn;

signals:
};

#endif // MESSAGEEDITAREA_H
