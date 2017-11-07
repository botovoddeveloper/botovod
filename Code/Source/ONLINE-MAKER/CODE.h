//---------------------------------------------------------------------------

#ifndef CODEH
#define CODEH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdHTTP.hpp>
#include <IdTCPClient.hpp>
#include <IdTCPConnection.hpp>
//---------------------------------------------------------------------------
class Tf : public TForm
{
__published:	// IDE-managed Components
	TEdit *Edit2;
	TLabel *Label2;
	TButton *b_DO;
	TIdHTTP *IdHTTP1;
	void __fastcall b_DOClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall Edit2Change(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall Tf(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE Tf *f;
//---------------------------------------------------------------------------
#endif
