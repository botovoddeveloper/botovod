//---------------------------------------------------------------------------

#include <System.hpp>
#pragma hdrstop

#include "MyTHREAD_ONLINE.h"
#include "MAIN.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------

//   Important: Methods and properties of objects in VCL can only be
//   used in a method called using Synchronize, for example:
//
//      Synchronize(&UpdateCaption);
//
//   where UpdateCaption could look like:
//
//      void __fastcall MyTHREAD_SPEECH::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------

__fastcall MyTHREAD_ONLINE::MyTHREAD_ONLINE(bool CreateSuspended)
	: TThread(CreateSuspended)
{
}

void __fastcall MyTHREAD_ONLINE::Execute()
{
    f->main->TERMINATED = false;

	//Synchronize(&GOTOF);

    GOTOF();
}
void __fastcall MyTHREAD_ONLINE::GOTOF()
{
	while ( true )
	{
		Sleep(10000);

		try
		{
			f->online->processing();
		}
		catch ( Exception *ex )
		{
			f->main->log("CATCH: MyTHREAD_ONLINE::Execute(): " + ex->Message );
		}
	}
}

