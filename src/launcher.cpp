//-----------------------------------------------------------------------------
#include "launcher.h"
//-----------------------------------------------------------------------------

Launcher * launcher;

Launcher::Launcher(QObject *parent)
    : QObject(parent)
{
}

//-----------------------------------------------------------------------------

Launcher::~Launcher()
{
}

//-----------------------------------------------------------------------------

bool Launcher::RunCommand(QString cmd, QStringList args, unsigned int timeOut, BuildWindow * const progress)
{
    /*if (cancel) {
        return false;
    }*/

    proc = new QProcess(NULL);
    proc->setProcessChannelMode(QProcess::MergedChannels);    
    bool c = connect(proc, SIGNAL(readyReadStandardOutput()), this, SLOT(OnReadStandardOutput()));

    QString txt = ">> " + cmd;
    for (unsigned int i=0; i < args.count(); i++) {
        txt = txt + " " + args[i];
    }
    msg.AddOutput(txt, false);
    
    enable = true;
    proc->start(cmd, args);
    //cancel = false;
    bool ok = false;
    if (timeOut > 0) {
        unsigned int timeOutCounter = 0;
        bool running = true;
        // timeout * 10 because out time resolution here is 100 ms
        while (running && (timeOutCounter < timeOut * 10)) {
            percentage = 10 * timeOutCounter / timeOut;
            progress->setValue(percentage);            
            running = !(proc->waitForFinished(-1));
            qApp->processEvents();            
            QThread::msleep(30);
            timeOutCounter++;
            if (progress->wasCanceled()) {
                break;
            }
        }

        ok = true;
        if (running) {
            proc->kill();
            ok = false;
        }
    } else {
        proc->waitForFinished();
        ok = (proc->exitStatus() == QProcess::NormalExit);
    }

    enable = false;
    
    if (ok) {
        int ec = proc->exitCode();
        ok = (ec == 0);
    }

    c = disconnect(proc, SIGNAL(readyReadStandardOutput()), this, SLOT(OnReadStandardOutput()));
    delete proc;
    
    return ok;
}

//-----------------------------------------------------------------------------

void Launcher::OnReadStandardOutput(void)
{
    if (enable == false) {
        return;
    }
    QTextStream computerOutput(proc->readAll());
    while (computerOutput.atEnd() == false) {
        msg.AddOutput(computerOutput.readLine());
    }
}

//-----------------------------------------------------------------------------
/*
bool Launcher::GetCancel(void)
{
    return cancel;
}

//-----------------------------------------------------------------------------
*/
/*void Launcher::SetCancel(bool value)
{
    cancel = value;
}

//-----------------------------------------------------------------------------

void Launcher::SetPercentage(int value)
{
    percentage = value;
}

//-----------------------------------------------------------------------------

int Launcher::GetPercentage(void)
{
	    return percentage;
}
*/
//-----------------------------------------------------------------------------
