//-----------------------------------------------------------------------------
#ifndef BUILDERTHREAD_H
#define BUILDERTHREAD_H
//-----------------------------------------------------------------------------

#include <QObject>
#include <builder.h>

//-----------------------------------------------------------------------------

class BuilderThread : public QThread
{
	Q_OBJECT

public:
	BuilderThread(QObject *parent);
	~BuilderThread();
	//void Upload(void);
	//void Build(void);	
	void run();
	bool program;	//bool Finished(void);
	bool burnBootloader;
private:	
	
};

//-----------------------------------------------------------------------------

#endif // BUILDERTHREAD_H
