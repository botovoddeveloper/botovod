//---------------------------------------------------------------------------

#ifndef MyTHREAD_ONLINEH
#define MyTHREAD_ONLINEH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
//---------------------------------------------------------------------------
class MyTHREAD_ONLINE : public TThread
{
private:
protected:
	void __fastcall Execute();
	void __fastcall GOTOF();
public:
	__fastcall MyTHREAD_ONLINE(bool CreateSuspended);
};
//---------------------------------------------------------------------------
#endif
