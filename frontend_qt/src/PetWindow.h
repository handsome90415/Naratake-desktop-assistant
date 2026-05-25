#ifndef PETWINDOW_H
#define PETWINDOW_H

#include <QWidget>
#include <QPoint>

class QLabel;
class ChatWindow;
class QMouseEvent;
class QContextMenuEvent;

class PetWindow : public QWidget
{
    Q_OBJECT

public:
    explicit PetWindow(QWidget *parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void contextMenuEvent(QContextMenuEvent *event) override;

private:
    QLabel *characterLabel;
    QPoint dragPosition;
    ChatWindow *chatWindow;

    void showChatWindow();
    void hideChatWindow();
};

#endif
