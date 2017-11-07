//---------------------------------------------------------------------------

#ifndef MyTHREAD_TWOH
#define MyTHREAD_TWOH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
//---------------------------------------------------------------------------
class MyTHREAD_TWO : public TThread
{
private:
protected:
	void __fastcall Execute();
	void __fastcall GOTOF();
public:
	__fastcall MyTHREAD_TWO(bool CreateSuspended);
};
//---------------------------------------------------------------------------
#endif
