#ifndef BUTTONSETUP_H
#define BUTTONSETUP_H

#include <QDialog>
#include "inputbutton.h"

namespace Ui {
class ButtonSetup;
}

class ButtonSetup : public QDialog
{
    Q_OBJECT

public:
    explicit ButtonSetup(QWidget *parent = 0);
    ~ButtonSetup();
    void setupButton(InputButton* button);

private:
    Ui::ButtonSetup *ui;
};

#endif // BUTTONSETUP_H
