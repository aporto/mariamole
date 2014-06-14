#include "builderthread.h"

//-----------------------------------------------------------------------------

BuilderThread::BuilderThread(QObject *parent)
	: QThread(parent)
{
	//finished = false;
}

//-----------------------------------------------------------------------------

BuilderThread::~BuilderThread()
{	

}

//-----------------------------------------------------------------------------

/*void BuilderThread::Upload(void)
{
	program = true;
	run();
}

//-----------------------------------------------------------------------------

void BuilderThread::Build(void)
{
	program = false;
	run();
}
*/
//-----------------------------------------------------------------------------

void BuilderThread::run()
{
	builder.Build(program);	
	exit();
}

//-----------------------------------------------------------------------------

/*bool BuilderThread::Finished(void)
{
	return finished;
}*/

//-----------------------------------------------------------------------------

