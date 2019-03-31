#pragma hdrstop
#pragma package(smart_init)
#pragma link "sSkinManager"
#pragma link "sPanel"
#pragma link "acImage"
#pragma link "acPNG"
#pragma link "sButton"
#pragma link "sLabel"
#pragma link "SHDocVw_OCX"
#pragma resource "*.dfm"

#include "OAUTH.h"
#include <vcl.h>
#include <mshtml.h>
#include <mshtmcid.h>
#include <OleCtnrs.hpp>
#include <atl\atlbase.h>
#include <Inifiles.hpp>
#include <Grin.h>
#include "VKApi.h"
#include <System.JSON.hpp>
#include <Data.DBXJSON.hpp>
#include <memory>


Tf *f;

class c_main
{
	public:

		String APPLICATION_ID; 
        String APPLICATION_SECRET; 
        String SCOPE; 
        String API_V; 
        String LOGIN; 
        String PASSWORD; 
        String TOKEN; 
        String TOKEN_EXPIRE;
        String USER_ID;
        String CODE;

		c_main();

		void data_read();
		void data_write();
		void open_urlForCode();
        void open_urlForToken();
		void fill_submit();
        void process_result(String resultString);

		void log( String data );          
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

#if 1
	main->open_urlForToken();          
    g.ProcessMessages();
#else    
    main->open_urlForCode();          
    g.ProcessMessages();
	main->fill_submit();
#endif
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
    // client_id   2274003   client_secret   hHbZxrka2uZ6jB1inYsH
    // client_id   3697615   client_secret   AlVXZFMUqyrnABp8ncuU
    // client_id   3140623   client_secret   VeWdmVclDCtn6ihuP1nt
    // client_id   3502557   client_secret   PEObAuQi6KloPM4T30DV
    // client_id   3682744   client_secret   mY6CDUswIVdJLCD3j15n

	String file = g.GetDirectoryApplicationDatapath() + "Global.Conf.ini";

	TMemIniFile *INI = new TMemIniFile( UnicodeString(file), TEncoding::UTF8 );
	API_V              = INI->ReadString( UnicodeString("OAUTH2"), UnicodeString("api"),       UnicodeString("5.0") );    
	APPLICATION_ID     = INI->ReadString( UnicodeString("OAUTH2"), UnicodeString("application_id"), UnicodeString("0") );
    APPLICATION_SECRET = INI->ReadString( UnicodeString("OAUTH2"), UnicodeString("application_secret"), UnicodeString("0") );    
	SCOPE              = INI->ReadString( UnicodeString("OAUTH2"), UnicodeString("scope"),     UnicodeString("0") );    
	LOGIN 	           = INI->ReadString( UnicodeString("OAUTH2"), UnicodeString("login"),     UnicodeString("0") );    
	PASSWORD           = INI->ReadString( UnicodeString("OAUTH2"), UnicodeString("password"),  UnicodeString("0") );
    TOKEN_EXPIRE       = INI->ReadString( UnicodeString("OAUTH2"), UnicodeString("token_expired"),  UnicodeString("0") );
    USER_ID            = INI->ReadString( UnicodeString("OAUTH2"), UnicodeString("user_id"),  UnicodeString("0") );    
	delete INI;

    log("API_V="+API_V);
    log("APPLICATION_ID="+APPLICATION_ID);
    log("APPLICATION_SECRET="+APPLICATION_SECRET);
    log("SCOPE="+SCOPE);
    log("LOGIN="+LOGIN);
    log("PASSWORD="+PASSWORD);
}
void c_main::data_write()
{
	log("DATAWRITE:");
	String file = g.GetDirectoryApplicationDatapath() + "Global.Conf.ini";
    TMemIniFile *INI = new TMemIniFile( UnicodeString(file), TEncoding::UTF8 );
	INI->WriteString( UnicodeString("OAUTH2"), UnicodeString("token"), UnicodeString(TOKEN) );  
    INI->WriteString( UnicodeString("OAUTH2"), UnicodeString("token_expired"), UnicodeString(TOKEN_EXPIRE) );
    INI->WriteString( UnicodeString("OAUTH2"), UnicodeString("user_id"), UnicodeString(USER_ID) );   
    INI->UpdateFile();
	delete INI;

    log("TOKEN="+TOKEN);
    log("TOKEN_EXPIRE="+TOKEN_EXPIRE);
    log("USER_ID="+USER_ID);
}
void c_main::open_urlForCode()
{
	try
	{
		log("OPENURL_CODE:");
		String request = "https://oauth.vk.com/authorize"
        +String("?client_id="+APPLICATION_ID)
        +"&scope="+SCOPE
        +"&redirect_uri=https://oauth.vk.com/blank.html"
        +"&display=popup"
        +"&response_type=code"
        +"&v="+API_V;

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
void c_main::open_urlForToken()
{
	try
	{
		log("OPENURL_TOKEN:");
        String response;
        bool success = false;
        while ( ! success )
        {
            response = vk.token_get(&success, LOGIN, PASSWORD, APPLICATION_ID, APPLICATION_SECRET, SCOPE);
            if ( ! success ) 
            {
                log(L"Сервер не ответил на запрос. Повтор..");
            }
        }

        if ( Pos("error",response) > 0 ) 
        {   
            log(L"Сервер сообщает об ошибке: ["+response+"]");
        }
        else
        {
            log(L"Ответ сервера: ["+response+"]");

            std::auto_ptr<TJSONObject> json (static_cast<TJSONObject*>(TJSONObject::ParseJSONValue(response)));
            TOKEN = vk.jsonfix_removeQuotes(json->GetValue("access_token")->ToString());
            TOKEN_EXPIRE = json->GetValue("expires_in")->ToString();
            USER_ID  = json->GetValue("user_id")->ToString();
        
            data_write();
        }
        
		log("OAUTH2 STOP");
		Application->Terminate();
		
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
    main->log("T2Timer");
    T2->Enabled = false;
	main->process_result(BROWSER->LocationURL);
}
void c_main::process_result(String resultString)
{
	if ( Pos("code=",resultString) != 0 )
	{
		log("CODE LOCATION:");
		log( resultString );
		
        log("GETCODE:");
        int p = Pos("code=",resultString);
        CODE = resultString.SubString( p+5, resultString.Length() );
        log( CODE );

        String response;
        bool success = false;
        while ( ! success )
        {
            response = vk.token_get(&success, CODE, APPLICATION_ID, APPLICATION_SECRET, String("https://oauth.vk.com/blank.html"));
            if ( ! success ) 
            {
                log(L"Сервер не ответил на запрос. Повтор..");
            }
        }

        if ( Pos("error",response) > 0 ) 
        {   
            log(L"Сервер сообщает об ошибке: ["+response+"]");
        }
        else
        {
            log(L"Ответ сервера: ["+response+"]");

            std::auto_ptr<TJSONObject> json (static_cast<TJSONObject*>(TJSONObject::ParseJSONValue(response)));
            TJSONObject *obj_response = static_cast<TJSONObject*>(json->Get("response")->JsonValue);
            TOKEN = vk.jsonfix_removeQuotes(obj_response->GetValue("access_token")->ToString());
            String expires_in = obj_response->GetValue("expires_in")->ToString();
            String user_id  = obj_response->GetValue("user_id")->ToString();
        
            data_write();
        }
        
		log("OAUTH2 STOP");
		Application->Terminate();
	}
    else if ( Pos("access_token=",resultString) != 0 )
	{
		log("TOKEN LOCATION:");
		log( resultString );

        log("GETTOKEN:");
        int p = Pos("access_token=",resultString);
        TOKEN = resultString.SubString( p+13, resultString.Length() );
        p = Pos("&",main->TOKEN);
        TOKEN = TOKEN.SubString( 1, p-1 );
        log( main->TOKEN );
        
        data_write();
		log("OAUTH2 STOP");
		Application->Terminate();
	}
	else
	{
        f->T2->Enabled = true;
        ClickButton(L"Разрешить");
	}
}
void c_main::log( String data )
{
	String file = g.GetDirectoryApplicationDatapath() + "Logs\\" + "OAUTH2.log";
    TDateTime T = Time();
    LOG->Add( TimeToStr(T) + ": " + data );
    LOG->SaveToFile( UnicodeString(file), TEncoding::UTF8 );
    f->sLabelFX1->Caption = data;
    Application->ProcessMessages();
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




