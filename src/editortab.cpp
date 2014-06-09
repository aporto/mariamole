#include "editortab.h"


EditorTab::EditorTab(QWidget *parent) : QTabWidget(parent)
{

}



void EditorTab::closeTab(int index)
{
    this->removeTab(index);
}




