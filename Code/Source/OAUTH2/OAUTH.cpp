#include <vcl.h>
#pragma hdrstop
#include "OAUTH.h"
#pragma package(smart_init)
#pragma link "sSkinManager"
#pragma link "sPanel"
#pragma link "acImage"
#pragma link "acPNG"
#pragma link "sButton"
#pragma link "sLabel"
#pragma link "SHDocVw_OCX"
#pragma resource "*.dfm"
#include "SHDocVw_OCX.h"
#include <mshtml.h>
#include <mshtmcid.h>
#include <OleCtnrs.hpp>
#include <atl\atlbase.h>
#include <Inifiles.hpp>
#include <Grin.cpp>


Tf *f;

class c_main
{
	public:

		str CLIENT_ID, SCOPE, API_V, LOGIN, PASSWORD, TOKEN;

		c_main();

		void data_read();
		void data_write();
		void open_url();
		void fill_submit();
		void process_result(str data);

		str  get_token( str data );

		void log( str data );          
        TStringList *LOG;

        void FillForm(String FieldName, String FieldText);
		void ClickButton(String ButtonName);
};

c_main *main;

__fastcall Tf::Tf(TComponent* Owner) : TForm(Owner)
{
	g.IE_CacheCookiesClear();
}
void __fastcall Tf::FormShow(TObject *Sender)
{
	f->ClientWidth  = 600;
	f->ClientHeight = 400;

	f->BROWSER->Width  = f->ClientWidth  + 4;
	f->BROWSER->Height = f->ClientHeight + 4;

	f->BROWSER->Left = -2;
	f->BROWSER->Top  = -2;

	main = new c_main();

	main->data_read();         
    g.ProcessMessages();
	main->open_url();          
    g.ProcessMessages();
	main->fill_submit();
}


	 c_main::c_main()
{
	LOG = new TStringList;
	TOKEN = "NULL";
	log("OAUTH2 START");
}
void c_main::data_read()
{
	log("DATAREAD:");

	str file = g.GetDirectoryApplicationDatapath() + "Global.Conf.ini";

	TMemIniFile *INI = new TMemIniFile( UnicodeString(file), TEncoding::UTF8 );
	API_V     = INI->ReadString( UnicodeString("OAUTH2"), UnicodeString("api"),       UnicodeString("5.0") );    
	CLIENT_ID = INI->ReadString( UnicodeString("OAUTH2"), UnicodeString("client_id"), UnicodeString("0") );    
	SCOPE     = INI->ReadString( UnicodeString("OAUTH2"), UnicodeString("scope"),     UnicodeString("0") );    
	LOGIN 	  = INI->ReadString( UnicodeString("OAUTH2"), UnicodeString("login"),     UnicodeString("0") );    
	PASSWORD  = INI->ReadString( UnicodeString("OAUTH2"), UnicodeString("password"),  UnicodeString("0") );    
	delete INI;

    log("API_V="+API_V);
    log("CLIENT_ID="+CLIENT_ID);
    log("SCOPE="+SCOPE);
    log("LOGIN="+LOGIN);
    log("PASSWORD="+PASSWORD);
}
void c_main::data_write()
{
	log("DATAWRITE:");
	str file = g.GetDirectoryApplicationDatapath() + "Global.Conf.ini";
    TMemIniFile *INI = new TMemIniFile( UnicodeString(file), TEncoding::UTF8 );
	INI->WriteString( UnicodeString("OAUTH2"), UnicodeString("token"), UnicodeString(TOKEN) );     
    INI->UpdateFile();
	delete INI;

    log("TOKEN="+TOKEN);
}
void c_main::open_url()
{
	try
	{
		log("OPENURL:");
		str request = "https://oauth.vk.com/authorize?client_id="+CLIENT_ID+"&scope="+SCOPE+"&redirect_uri=https://oauth.vk.com/blank.html&display=popup&v="+API_V+"&response_type=token";

		log( request );
		f->BROWSER->Navigate( request.w_str() );

		while ( f->BROWSER->ReadyState != Shdocvw::READYSTATE_COMPLETE ) 
        {
            Application->ProcessMessages();
        }
		log( f->BROWSER->LocationURL );
	}
	catch ( Exception *ex )
	{
		log( ex->Message );
	}
}
void c_main::fill_submit()
{
	log("FILLSUBMIT");

	FillForm( "email", LOGIN );
	FillForm( "pass", PASSWORD );
	ClickButton("submit_input");

	while ( f->BROWSER->ReadyState != Shdocvw::READYSTATE_COMPLETE ) 
    {
        Application->ProcessMessages();
    }
	f->T->Enabled = true;
}
void __fastcall Tf::TTimer(TObject *Sender)
{
	main->log("T1");
	T->Enabled = false;
	T2->Enabled = true;
}
void __fastcall Tf::T2Timer(TObject *Sender)
{
	main->log("T2");
	T2->Enabled = false;

	str RETURN = BROWSER->LocationURL;

	if ( Pos("access_token=",RETURN) != 0 )
	{
		main->log("LOCATION:");
		main->log( RETURN );
		main->process_result( RETURN );
	}
	else
	{
        T2->Enabled = true;
        main->ClickButton(L"Разрешить");
	}
}
void c_main::process_result(str data)
{
	if ( Pos("access_token",data) > 0 )
	{
		TOKEN = get_token( data );
		data_write();
		log("OAUTH2 STOP");
		Application->Terminate();
	}
	else 
    {
        log("ACCESS TOKEN NOT FOUND ((((((((((");
    }
}
void c_main::log( str data )
{
	try
	{
		str file = g.GetDirectoryApplicationDatapath() + "Logs\\" + "OAUTH2.log";
		TDateTime T = Time();
		LOG->Add( TimeToStr(T) + ": " + data );
		LOG->SaveToFile( UnicodeString(file), TEncoding::UTF8 );
		f->sLabelFX1->Caption = data;
		Application->ProcessMessages();
	}
	catch (...)
	{

	}
}
str  c_main::get_token( str data )
{
	log("GETTOKEN:");
	int p = Pos("access_token=",data);
	data = data.SubString( p+13, data.Length() );
	p = Pos("&",data);
	data = data.SubString( 1, p-1 );
	log( data );
	return data;
}
void c_main::FillForm(String FieldName, String FieldText)
{
	try
	{
		TCppWebBrowser *CppWebBrowser1 = f->BROWSER;

		CComQIPtr<IHTMLDocument2, &IID_IHTMLDocument2>                  pDoc;
		CComQIPtr<IHTMLElementCollection, &IID_IHTMLElementCollection>  pColl;
		CComQIPtr<IDispatch, &IID_IDispatch>                            pTmpDisp;
		CComQIPtr<IHTMLInputElement, &IID_IHTMLInputElement>            pElement;
		CComQIPtr<IDispatch, &IID_IDispatch>                            pDisp;

		if (SUCCEEDED(CppWebBrowser1->Document->QueryInterface(IID_IHTMLDocument2, (LPVOID*)&pDoc)) )
		{
			 if(SUCCEEDED (pDoc->get_all(&pColl) ) )
			 {
                if(SUCCEEDED (pColl->item(TVariant(WideString(FieldName)), TVariant(0), &pDisp) ) &&
                    pDisp != NULL)
                {
                    pElement = pDisp;
                    pElement->put_value(FieldText.w_str() );
                }
			 }
		}
	}
	catch ( Exception *ex )
	{
		log( ex->Message );
    }
}
void c_main::ClickButton(String ButtonName)
{
	try
	{
		TCppWebBrowser* Browser = f->BROWSER;

	    if( Browser->Document )
		{
		    TComInterface<IHTMLDocument2> pDocument;
		    Browser->Document->QueryInterface(IID_IHTMLDocument2,
											(LPVOID*)&pDocument);
		    if( pDocument )
            {
			    TComInterface<IHTMLElementCollection> pCollection;
			    pDocument->get_all(&pCollection);
			    if( pCollection )
				{
				    TComInterface<IDispatch> pDisp;
				    pCollection->item(TVariant(WideString(ButtonName)),
									    TVariant(0),
									    &pDisp);
				    if( pDisp )
					{
					    TComInterface<IHTMLElement> pElement;
					    pDisp->QueryInterface(IID_IHTMLElement,
											    (LPVOID*)&pElement);
					    if( pElement )
					    {
						    pElement->click();
					    }
					}
                    else
                    {
                        long nItemCount = 0;				
                        HRESULT hr = pCollection->get_length(&nItemCount);
                        if (SUCCEEDED(hr))
                        {
                            for (long i = 0; i < nItemCount; i++)
                            {
                                TComInterface<IDispatch> spDisp;
                                hr = pCollection->item(CComVariant(i), CComVariant(), &spDisp);
                                if (SUCCEEDED(hr))
                                {
                                    TComInterface<IHTMLElement> spElement = spDisp;
                                    if (spElement)
                                    {
                                        CComBSTR className;
                                        hr = spElement->get_className(&className);
                                        if (SUCCEEDED(hr) && className)
                                        {
                                            log((LPCTSTR)className);
                                        }
                                    
                                        CComBSTR innerText;
                                        hr = spElement->get_innerText(&innerText);
                                        if (SUCCEEDED(hr) && innerText)
                                        {
                                            String innerTextStr = (LPCTSTR)innerText;
                                            if(innerTextStr.CompareIC(ButtonName) == 0)
                                            {
                                                spElement->click();
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
				}
			}
		 }
	}
	catch ( Exception *ex )
	{
		log( ex->Message );
    }
}

void __fastcall Tf::B_TESTClick(TObject *Sender)
{
    ShowMessage( BROWSER->LocationURL );
}
void __fastcall Tf::B_TEST2Click(TObject *Sender)
{
	main->log("LOCATION11: "+BROWSER->LocationURL);
}
void __fastcall Tf::B_TEST3Click(TObject *Sender)
{
//
}
void __fastcall Tf::B_TEST4Click(TObject *Sender)
{
//
}




