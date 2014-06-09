#include <iostream>
using namespace std;
#include "editortab.h"


EditorTab::EditorTab(QWidget *parent) : QTabWidget(parent)
{
    this->setTabsClosable(true);
    this->setStyleSheet("border-style:solid");
    this->setStyleSheet("background-color: rgb(32, 40, 42)");
}



void EditorTab::closeTab(int index)
{

    cout << "Index to remove == "  << index << endl;
    QWidget* tabItem = this->widget(index);
    // Removes the tab at position index from this stack of widgets.
    // The page widget itself is not deleted.
    this->removeTab(index);
    //delete this->widget(index);
    //delete tabItem; //It does not work, still do not know why...
}




