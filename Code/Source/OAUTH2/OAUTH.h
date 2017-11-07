//---------------------------------------------------------------------------

#ifndef OAUTHH
#define OAUTHH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "sSkinManager.hpp"
#include <SHDocVw.hpp>
#include <Vcl.OleCtrls.hpp>
#include "sPanel.hpp"
#include <Vcl.ExtCtrls.hpp>
#include "acImage.hpp"
#include "acPNG.hpp"
#include <IdAntiFreezeBase.hpp>
#include <IdBaseComponent.hpp>
#include <Vcl.IdAntiFreeze.hpp>
#include "sButton.hpp"
#include "sLabel.hpp"
#include "SHDocVw_OCX.h"
//---------------------------------------------------------------------------
class Tf : public TForm
{
__published:	// IDE-managed Components
	TsSkinManager *MANAGER;
	TButton *B_TEST;
	TButton *B_TEST2;
	TIdAntiFreeze *IdAntiFreeze1;
	TButton *B_TEST3;
	TButton *B_TEST4;
	TCppWebBrowser *BROWSER;
	TTimer *T;
	TTimer *T2;
	TsPanel *sPanel1;
	TImage *Image1;
	TImage *Image2;
	TsLabelFX *sLabelFX1;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall B_TESTClick(TObject *Sender);
	void __fastcall B_TEST2Click(TObject *Sender);
	void __fastcall B_TEST3Click(TObject *Sender);
	void __fastcall B_TEST4Click(TObject *Sender);
	void __fastcall TTimer(TObject *Sender);
	void __fastcall T2Timer(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall Tf(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE Tf *f;
//---------------------------------------------------------------------------
#endif
