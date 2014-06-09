#include "editotab.h"

EditoTab::EditoTab(QWidget *parent) : QTabWidget(parent)
{
    connect(this, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));
}



void EditoTab::closeTab(int index)
{
    this->removeTab(index);
}

