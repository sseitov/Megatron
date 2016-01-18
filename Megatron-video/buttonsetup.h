#ifndef BUTTONSETUP_H
#define BUTTONSETUP_H

#include <QDialog>
#include "controlbutton.h"

namespace Ui {
class ButtonSetup;
}

class ButtonSetup : public QDialog
{
    Q_OBJECT

public:
    explicit ButtonSetup(QWidget *parent = 0);
    ~ButtonSetup();
    void setupButton(ControlButton* button);

private:
    Ui::ButtonSetup *ui;
};

#endif // BUTTONSETUP_H
