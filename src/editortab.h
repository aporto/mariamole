#ifndef EDITOTAB_H
#define EDITOTAB_H

#include <QTabWidget>

class EditorTab : public QTabWidget
{
public:
    EditorTab(QWidget *parent = 0);

public slots:
    void closeTab(int);
};

#endif // EDITOTAB_H