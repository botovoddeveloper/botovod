//---------------------------------------------------------------------------

#include <System.hpp>
#pragma hdrstop

#include "MyTHREAD_TWO.h"
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

__fastcall MyTHREAD_TWO::MyTHREAD_TWO(bool CreateSuspended)
	: TThread(CreateSuspended)
{
}

void __fastcall MyTHREAD_TWO::Execute()
{
    f->main->TERMINATED = false;

	//Synchronize(&GOTOF);

    GOTOF();
}
void __fastcall MyTHREAD_TWO::GOTOF()
{
	try
	{
		f->proc->ProcessTwoOpen();

        f->proc->ProcessTwoClose();
	}
	catch ( Exception *ex )
	{
		f->main->elog("ERROR: MyTHREAD_TWO::Execute(): " + ex->Message );
	}
}

