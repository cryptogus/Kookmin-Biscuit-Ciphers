#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <iostream>

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    unsigned char *text_;
    unsigned char *key_;
    unsigned char *iv_;
private slots:
    void on_openKBC_clicked();

    void on_dockWidget_allowedAreasChanged(const Qt::DockWidgetAreas &allowedAreas);

    void on_textEdit_copyAvailable(bool b);

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
