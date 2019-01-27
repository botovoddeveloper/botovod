#include "Grin.h"
#pragma hdrstop

#include <vcl.h>
#include <System.Classes.hpp>

#include <ComObj.hpp>
#include <clipbrd.hpp>
#include <utilcls.h>
#include <PNGImage.hpp>
#include <jpeg.hpp>
#include <IdHashMessageDigest.hpp>
#include <Vcl.Controls.hpp>
#include <ostream>
#include <sstream>
#include <iomanip>
#include <cstdio>
#include "math.h"
#include <memory>
#include "inifiles.hpp"

#include <IdHTTP.hpp>



c_grin g;
int DevCount = 0;

void SomeFunc()
{
	try
	{

	}
	catch ( Exception *ex )
	{
		ShowMessage( "ERR GRIN.CPP | SomeFunc | " + ex->Message );
	}
}

void c_grin::Terminate()
{
	String x = "Application Terminate.\n\n\n"

			L"В случае неисправности обращайтесь к разработчику:\n\n"
			"-------------------------------------------------------------\n\n"
			"Mail : Jenya-Grin@mail.ru\n\n"
			"Skype : Jenya-Grin\n\n"
			"ICQ  : 647953777\n\n";

	ShowMessage( x );
	Application->Terminate();
}
String  c_grin::GetCaptchaRucaptcha1( String file, String captchakey )
{
	String J = "NOT-SENDED";

	try
	{
		TIdHTTP *HTTP = new TIdHTTP(NULL);
		HTTP->HandleRedirects = true;

		TStringStream *RETURN = new TStringStream;

		std::auto_ptr<TIdMultiPartFormDataStream>Stream(new TIdMultiPartFormDataStream);
		HTTP->Request->ContentType = "multipart/form-data";
		Stream->AddFormField("key",captchakey);
		Stream->AddFile( "file", file, "multipart/form-data" );
		HTTP->Post("http://rucaptcha.com/in.php", Stream.get(), RETURN );

		String RET = RETURN->DataString;

		if ( PosExist("OK|",RET) )
		{
			String ID = RET.SubString(4,RET.Length());

			while ( true )
			{
				Sleep(3000);

				RET = HTTP->Get("http://rucaptcha.com/res.php?key="+captchakey+"&action=get&id="+ID);

				if ( PosExist("OK|",RET) )
				{
					J = RET.SubString(4,RET.Length());
					break;
				}
			}
		}
		else
		{
			J = RET;
		}
	}
	catch (...)
	{
		J = "ERROR";
	}

	return J;
}
String  __fastcall c_grin::GetMD5(const String data)
{
  std::auto_ptr<TIdHashMessageDigest5> idmd5(new TIdHashMessageDigest5());
  return idmd5->HashStringAsHex(data);
}
AnsiString c_grin::UTF8_TO_CP1251( const char *String )
{
	AnsiString res;
	int result_u, result_c;
	result_u = MultiByteToWideChar(CP_UTF8, 0, String, -1, 0, 0);
	if(!result_u)
	return 0;
	wchar_t *ures = new wchar_t[result_u];
	if(!MultiByteToWideChar(CP_UTF8, 0, String, -1, ures, result_u))
	{
	  delete[] ures;
	  return 0;
	}
	result_c = WideCharToMultiByte(1251, 0, ures, -1, 0, 0, 0, 0);
	if(!result_c)
	{
	  delete[] ures;
	  return 0;
	}
	char *cres = new char[result_c];
	if(!WideCharToMultiByte(1251, 0, ures, -1, cres, result_c, 0, 0))
	{
	  delete[] ures;
	  delete[] cres;
	  return 0;
	}
	delete[] ures;
	res = AnsiString(cres);
	delete[] cres;
	return res;
}
String  c_grin::GetHDDSerial_C()
{
	char VolumeName[255], FileSystem[255];
	unsigned long SerialNumber, MCLength, SystemFlags;
	AnsiString VolumeSerial;

	if (GetVolumeInformationA( "C:\\", VolumeName, 254, &SerialNumber, &MCLength, &SystemFlags, FileSystem, 254))
	{
		VolumeSerial = AnsiString(SerialNumber);
	}

	return VolumeSerial;
}
String  c_grin::GetIP_Local()
{
	AnsiString out = "WinSock ERR";
	WSADATA wsaData;
    if (!WSAStartup(WINSOCK_VERSION, &wsaData))
    {
        char chInfo[64];
	    if (!gethostname(chInfo, sizeof(chInfo)))
	    {
	        hostent *sh;
	        sh=gethostbyname((char*)&chInfo);
	        if (sh!=NULL)
	        {
	            int nAdapter = 0;
	            while (sh->h_addr_list[nAdapter])
	            {
	                struct sockaddr_in adr;
	                memcpy(&adr.sin_addr, sh->h_addr_list[nAdapter], sh->h_length);
	                out = inet_ntoa(adr.sin_addr);
	                nAdapter++;
	            }
	        }
	    }
	}
	WSACleanup();
	return out;
}
void c_grin::Sm( String message )
{
	ShowMessage( UnicodeString(message) );
}
String  c_grin::Its( int data )
{
	return IntToStr( data );
}
int  c_grin::Sti( String data )
{
	return StrToInt( data );
}
String  c_grin::Encrypt( int index, int count, String sharp, String dataline )
{
	dataline = dataline + sharp + "ZERO";;

	String J = "";

	try
	{
		for ( int c = 1; c <= count; c++ )
		{
			int p = Pos(sharp,dataline);
			J = dataline.SubString(1,p-1);
			dataline = dataline.SubString(p+1,dataline.Length());

			if ( c == index ) 
            {
                break;
            }
		}
	}
	catch ( Exception *ex )
	{
		J = "ERR GRIN.CPP | Encrypt | " + ex->Message;
	}

	return J;
}
String  c_grin::EncryptToList( String sharp, String dataline )
{
	std::auto_ptr<TStringList> L(new TStringList);

	int shcount = 0;
	for ( int c = 1; c <= dataline.Length(); c++ )
	{
		String ch = dataline[c];
		if ( ch == sharp ) 
        {
            shcount++;
        }
	}

	if ( shcount == 0 ) 
    {
        L->Add( dataline );
    }
	else
	{
		int datacount = shcount + 1;
		for ( int c = 1; c <= datacount; c++ )
		{
			L->Add( Encrypt(c,datacount,sharp,dataline) );
        }
    }

	return L->Text;
}
void c_grin::LoadFromCSV( WideString file, int ColumnCount, int StartRowIndex, TStringGrid *GRID )
{
	try
	{

	}
	catch ( Exception *ex )
	{
		ShowMessage( "ERR GRIN.CPP | LoadFromCSV | " + ex->Message );
	}
}
void c_grin::GetScreen( Graphics::TBitmap *BMP )
{

}
void c_grin::DevLog( bool enabled, String data )
{
	if ( enabled )
	{
		DevCount++;

		String dir  = ExtractFilePath( Application->ExeName ) + "g.DevLog\\";
		String file = dir + IntToStr(DevCount) + ".bmp";

		try
		{
			if ( ! DirectoryExists( dir.c_str() ) ) 
            {
                CreateDirectory( dir.c_str(), NULL );
            }

			std::auto_ptr<Graphics::TBitmap> BIT(new Graphics::TBitmap);
			GetScreen(BIT.get());

			BIT->Canvas->TextOutW(10,10,data);
			BIT->SaveToFile( file );
		}
		catch ( Exception *ex )
		{
			Sm( "ERR GRIN.CPP | DevLog | " + ex->Message );
		}
	}
}
void c_grin::PlaySoundAcync( String file )
{
	PlaySound( file.c_str(), 0, SND_FILENAME | SND_ASYNC );
}
void c_grin::GetFiles( String dir, TStringList *LIST )
{
	LIST->Clear();

	SetCurrentDir( dir );
	TSearchRec sr;
	if ( FindFirst("*",faAnyFile,sr) == 0 )
	{
		do
        {
		    if (sr.Name != ".") 
            {
                LIST->Add(sr.Name);
            }
        }
		while( FindNext(sr) == 0 );
		FindClose(sr);
	}

	LIST->Delete(0);
}
void c_grin::DeletePathWithFiles( String dir )
{
	AnsiString DirName = dir;
	TSearchRec sr;

	if (DirName.Length())
	{
		if (!FindFirst(DirName+"\\*.*",faAnyFile,sr))

		do
		{
			if (!(sr.Name=="." || sr.Name=="..")) // это удалять не надо
            {
                if (((sr.Attr & faDirectory) == faDirectory ) || (sr.Attr == faDirectory)) // найдена папка
			    {
				    FileSetAttr(DirName+"\\"+sr.Name, faDirectory );// сброс всяких read-only
				    DeletePathWithFiles(DirName+"\\"+sr.Name);//рекурсивно удаляем содержимое
				    RemoveDir(DirName + "\\"+sr.Name);// удаляем теперь уже пустую папку
			    }
			    else// иначе найден файл
			    {
				    FileSetAttr(DirName+"\\"+sr.Name, 0);// сброс всяких read-only
				    DeleteFile(DirName+"\\"+sr.Name);// удаляем файл
			    }
            }
		}
		while (!FindNext(sr));// ищем опять, пока не найдем все

		FindClose(sr);
	}

	RemoveDir(DirName);
}
HWND c_grin::GetFormHandle( String formcaption )
{
	wchar_t *wc = formcaption.c_str();
	HWND H = FindWindow( NULL, wc );
	return H;
}
String  c_grin::GetDirectoryApplicationExename()
{
	return ExtractFilePath( Application->ExeName );
}
String  c_grin::GetDirectoryApplicationDatapath()
{
	return ExtractFilePath( Application->ExeName ) + "..\\Data\\";
}
String  c_grin::GetPixSCR( int x, int y )
{
	std::auto_ptr<Graphics::TBitmap> BIT(new Graphics::TBitmap);
	GetScreen( BIT.get() );
	return GetPixBIT( x, y, BIT.get() );
}
String  c_grin::GetPixBIT( int x, int y, Graphics::TBitmap *BIT )
{
	String ret = "err::api::GetPix";
	return ret;
}
void c_grin::ProcessMessages()
{
	Application->ProcessMessages();
}
void c_grin::KeyboardHandleWrite( HWND H, String text, int sleep )
{
	for ( int c = 1; c <= text.Length(); c++ )
	{
		int code = text[c];
		if ( code == 13 )
		{
			SendMessage(H, WM_KEYDOWN, VK_RETURN, 0);
			SendMessage(H, WM_KEYUP  , VK_RETURN, 0);
		}
		else
		{
			SendMessageW( H, WM_CHAR, int(text[c]), 0 );
		}
	}

	Sleep(sleep);
}
void c_grin::KeyboardHandlePageDown( HWND H, int count, int sleep )
{
	for ( int c = 0; c < count; c++ )
	{
		PostMessage( H, WM_KEYDOWN, VK_DOWN, MapVirtualKey(VK_DOWN, 0) );
		Sleep(50);
	}

	Sleep(sleep);
}
void c_grin::MouseHandleClick( HWND H, int x, int y, int sleep )
{
	PostMessage( H, WM_LBUTTONDOWN,MK_LBUTTON, y*65536+x );
	Sleep(100);
	PostMessage( H, WM_LBUTTONUP,MK_LBUTTON, y*65536+x );
	Sleep( sleep );
}
void c_grin::MouseEventClick( int x, int y, int sleep )
{
	SetCursorPos(x,y); Sleep(100);

	mouse_event(MOUSEEVENTF_ABSOLUTE|MOUSEEVENTF_LEFTDOWN, x, y, 0, 0);
	mouse_event(MOUSEEVENTF_ABSOLUTE|MOUSEEVENTF_LEFTUP,   x, y, 0, 0);

	Sleep(sleep);
}
void c_grin::MouseEventWheelUP( int count, int sleep )
{
	for ( int c = 1; c <= count; c++ )
	{
		mouse_event(MOUSEEVENTF_WHEEL,0,0, Cardinal(-240),0);
		Sleep(sleep);
	}
}
String  c_grin::FixFloatToString( float n )
{
	char xstr[32];
	sprintf(xstr, " %3.10f ", n);
	String J = Trim( xstr );
	J = ReplaceChars( J, ".", "," );
	return J;
}
String  c_grin::ReplaceChars( String data, String what, String to )
{
	String J = "";
	for ( int c = 1; c <= data.Length(); c++ )
	{
		String ch = data[c];
		if ( ch == what ) 
        {
            ch = to;
        }
		J = J + ch;
	}
	return J;
}
bool c_grin::PosExist( String sub, String line )
{
	bool j = false;
	if ( Pos(sub,line) != 0 ) 
    {
        j = true;
    }
	return j;
}
void c_grin::SaveToFile( String data, String file )
{
	TStringList *L = new TStringList;
	L->Text = data;
	L->SaveToFile( UnicodeString(file), TEncoding::UTF8 );
	delete L;
}
void c_grin::GoToUrl( String pageurl )
{
	wchar_t *wc = pageurl.c_str();
	char c[MAX_PATH] = {0};
	wcstombs(c, wc, wcslen(wc)+1);
	ShellExecuteA( NULL, "open", c, NULL, NULL, SW_SHOW );
}
String  c_grin::GetHTTP_UTF8TOWIN1251( String url )
{
	TIdHTTP *HTTP = new TIdHTTP(NULL);
	HTTP->HandleRedirects = true;

	TMemoryStream * mstream = new TMemoryStream;
	TStringList * slist = new TStringList;
	TEncoding * enc;
	HTTP->Get(url,mstream);
	mstream->Position = 0;
	slist->LoadFromStream(mstream, enc->UTF8);
	String html = slist->Text;

	delete mstream;
	delete slist;
	delete HTTP;

	return html;
}
bool c_grin::if_image_exist_byScreen( int *xx, int *yy, String file )
{
	bool exist = false;
	return exist;
}
bool c_grin::if_image_exist_byFile( int *xx, int *yy, String file, Graphics::TBitmap *BMPALL )
{
	bool exist = false;
	return exist;
}
String  c_grin::GetRandomItemFromList( String file )
{
	TStringList *L = new TStringList;
	L->LoadFromFile( file );
	String J = L->Strings[ Random( L->Count ) ];

	delete L;
	return J;
}
String  c_grin::MakeTempMail1PostAddress( String LeftAddressSide )
{
	String DATA;
	try
	{
		TIdHTTP *HTTP = new TIdHTTP(NULL);
		HTTP->HandleRedirects = true;
		DATA = HTTP->Get( "http://api.temp-mail.ru/request/domains/" );
		delete HTTP;

		TStringList *X = new TStringList;
		for ( int c = 1; c <= DATA.Length(); c++ )
		{
			String ch = DATA[c];
			if ( ch == "@" ) 
            {
                X->Add( IntToStr(c) );
            }
		}

		int I = Random( X->Count );
		int A = StrToInt( X->Strings[ I ] );

		delete X;
		DATA = DATA.SubString( A, DATA.Length() );
		int P = Pos( "<", DATA );
		DATA = DATA.SubString(1,P-1);
	}
	catch ( Exception *ex )
	{
		ShowMessage( "ERR GRIN.CPP | GetTempMailDomains1 | " + ex->Message );
	}

	return LeftAddressSide + DATA;
}
String  c_grin::GetTempMail1( String PostAddress )
{
	String RETURN; String PD;

	try
	{
		TIdHTTP *HTTP = new TIdHTTP(NULL);
		HTTP->HandleRedirects = true;
		PD = "http://api.temp-mail.ru/request/mail/id/"+ GetMD5(PostAddress).LowerCase() +"/format/php/";
		RETURN = HTTP->Get( PD );
		delete HTTP;
	}
	catch ( Exception *ex )
	{
		RETURN = ex->Message;
	}
	return RETURN;
}
void c_grin::IE_CacheCookiesClear()
{
      HANDLE hEntry;
      DWORD len, entry_size = 0;
      GROUPID id;
      INTERNET_CACHE_ENTRY_INFO * info = NULL;
      HANDLE hGroup = FindFirstUrlCacheGroup(0, CACHEGROUP_SEARCH_ALL, 0,
                                             0, &id, 0);
      if (hGroup) 
      {
        do 
        {
          len = entry_size;
          hEntry = FindFirstUrlCacheEntryEx(NULL, 0, 0xFFFFFFFF, id, info, &len,
                                            NULL, NULL, NULL);
          if (!hEntry && GetLastError() == ERROR_INSUFFICIENT_BUFFER && len) 
          {
            entry_size = len;
            info = (INTERNET_CACHE_ENTRY_INFO *)realloc(info, len);
            if (info) 
            {
              hEntry = FindFirstUrlCacheEntryEx(NULL, 0, 0xFFFFFFFF, id, info,
                                                &len, NULL, NULL, NULL);
            }
          }
          
          if (hEntry && info) 
          {
            bool ok = true;
            do 
            {
              DeleteUrlCacheEntry(info->lpszSourceUrlName);
              len = entry_size;
              if (!FindNextUrlCacheEntryEx(hEntry, info, &len, NULL, NULL, NULL)) 
              {
                if (GetLastError() == ERROR_INSUFFICIENT_BUFFER && len) 
                {
                  entry_size = len;
                  info = (INTERNET_CACHE_ENTRY_INFO *)realloc(info, len);
                  if (info) {
                    if (!FindNextUrlCacheEntryEx(hEntry, info, &len,
                                                 NULL, NULL, NULL)) 
                    {
                      ok = false;
                    }
                  }
                } 
                else 
                {
                  ok = false;
                }
              }
            } 
            while (ok);
          }
          if (hEntry) 
          {
            FindCloseUrlCache(hEntry);
          }
          DeleteUrlCacheGroup(id, CACHEGROUP_FLAG_FLUSHURL_ONDELETE, 0);
        } 
        while(FindNextUrlCacheGroup(hGroup, &id,0));
        FindCloseUrlCache(hGroup);
      }

      len = entry_size;
      hEntry = FindFirstUrlCacheEntryEx(NULL, 0, 0xFFFFFFFF, 0, info, &len,
                                        NULL, NULL, NULL);
      if (!hEntry && GetLastError() == ERROR_INSUFFICIENT_BUFFER && len) 
      {
        entry_size = len;
        info = (INTERNET_CACHE_ENTRY_INFO *)realloc(info, len);
        if (info) 
        {
          hEntry = FindFirstUrlCacheEntryEx(NULL, 0, 0xFFFFFFFF, 0, info, &len,
                                            NULL, NULL, NULL);
        }
      }
      
      if (hEntry && info) 
      {
        bool ok = true;
        do 
        {
          DeleteUrlCacheEntry(info->lpszSourceUrlName);
          len = entry_size;
          if (!FindNextUrlCacheEntryEx(hEntry, info, &len, NULL, NULL, NULL)) 
          {
            if (GetLastError() == ERROR_INSUFFICIENT_BUFFER && len) 
            {
              entry_size = len;
              info = (INTERNET_CACHE_ENTRY_INFO *)realloc(info, len);
              if (info) {
                if (!FindNextUrlCacheEntryEx(hEntry, info, &len,
                                             NULL, NULL, NULL)) 
                {
                  ok = false;
                }
              }
            } 
            else 
            {
              ok = false;
            }
          }
        } 
        while (ok);
      }
      
      if (hEntry) 
      {
        FindCloseUrlCache(hEntry);
      }

      len = entry_size;
      hEntry = FindFirstUrlCacheEntry(NULL, info, &len);
      if (!hEntry && GetLastError() == ERROR_INSUFFICIENT_BUFFER && len) 
      {
        entry_size = len;
        info = (INTERNET_CACHE_ENTRY_INFO *)realloc(info, len);
        if (info) 
        {
          hEntry = FindFirstUrlCacheEntry(NULL, info, &len);
        }
      }
      
      if (hEntry && info) 
      {
        bool ok = true;
        do 
        {
          DeleteUrlCacheEntry(info->lpszSourceUrlName);
          len = entry_size;
          if (!FindNextUrlCacheEntry(hEntry, info, &len)) 
          {
            if (GetLastError() == ERROR_INSUFFICIENT_BUFFER && len) 
            {
              entry_size = len;
              info = (INTERNET_CACHE_ENTRY_INFO *)realloc(info, len);
              if (info) 
              {
                if (!FindNextUrlCacheEntry(hEntry, info, &len)) 
                {
                  ok = false;
                }
              }
            } 
            else 
            {
              ok = false;
            }
          }
        } 
        while (ok);
      }
      
      if (hEntry) 
      {
        FindCloseUrlCache(hEntry);
      }
      
      if (info)
      {
        free(info);
      }
}

void c_grin::ShellExecute1( String file )
{
	wchar_t *wc = file.c_str();
	char c[MAX_PATH] = {0};
	wcstombs(c, wc, wcslen(wc)+1);

	ShellExecuteA( NULL, "open", c, NULL, NULL, SW_SHOW );
}
void c_grin::SetColorToLabel( TLabel *l, String color )
{
	l->Font->Color = StringToColor( color );
}

String  c_grin::GenerateKey( String source )
{
	String j = ""; String o = "0"; String i = "1";
	for ( int c = 1; c <= source.Length(); c++ )
	{
		String ch = source[c];
	}
	return j;
}
String  c_grin::GetToken()
{
	String to = "";
	for ( int c = 0; c < 20; c++ )
	{
		int r = Random(9);
		to = to + IntToStr(r);
	}

	return to;
}
void c_grin::EncLicPHP(String data, int *count, String *rtoken)
{
	int p;
	String c, t;

    p = Pos("{",data);
    data = data.SubString(p+1,data.Length());
    p = Pos("}",data);
	c = data.SubString(1,p-1);
    data = data.SubString(p+1,data.Length());

    p = Pos("{",data);
    data = data.SubString(p+1,data.Length());
    p = Pos("}",data);
	t = data.SubString(1,p-1);
    data = data.SubString(p+1,data.Length());

	*count = StrToInt(c);
	*rtoken = t;
}
void c_grin::LicSM(int errorcode)
{
	String x = "Application Fatal Error.\n\n"
    "Error code: " + IntToStr( errorcode );
	ShowMessage( x );
}
String  c_grin::ReadSerialFromFile()
{
	String dir = g.GetDirectoryApplicationDatapath();
	String file = dir + "Application";

	if ( ! DirectoryExists( dir.c_str() ) ) 
    {
        CreateDirectory( dir.c_str(), NULL );
    }

	TStringList *L = new TStringList;
	if ( FileExists( file ) )
	{
		L->LoadFromFile( file );
	}
	else
	{
		L->Add("0000-0000-0000-0000");
		L->SaveToFile( file );
	}

	String J = L->Strings[0];

	delete L;
	return Trim( J );
}
void c_grin::cache_findcachefiles(TStringList *l, AnsiString dir)
{
	TSearchRec sr;
	if (dir.Length())
	{
		if (!FindFirst(dir+"\\*.*",faAnyFile,sr))
        {
		    do
		    {
			    if (!(sr.Name=="." || sr.Name==".."))
                {
			        if (((sr.Attr & faDirectory) == faDirectory ) ||
                        (sr.Attr == faDirectory))
			        {
				        cache_findcachefiles(l, dir+"\\"+sr.Name);
			        }
                }
		        else
		        {
			        if ( Pos("static",sr.Name) == 0 && Pos("container.dat",sr.Name) == 0 )
			        {
				        l->Add(dir+"\\"+sr.Name);
			        }
		        }
		    }
		    while (!FindNext(sr));
        }
		FindClose(sr);
	}
}
String  c_grin::cut_kk(String data)
{
	data = data.SubString(2,data.Length());
	data = data.SubString(1,data.Length()-1);

	return data;
}
String  c_grin::cut_photo(String data)
{
	//String fx = "photo\\\":\\\"";

	//int p = Pos( fx, data );

	//data = data.SubString(p+fx.Length(), data.Length() );

	//p = Pos("\\", data);

	//data = data.SubString(1,p-1);

	//return data;

	String fx = "photo\":\"";
	int p  = Pos(fx,data);
	String xData = data.SubString(p+fx.Length(),data.Length());
	p = Pos("\",\"hash",xData);
	xData = xData.SubString(1,p-1);

	return xData;
}
String  c_grin::StripSlashes(String dirty)
{
	String j = "";

	for ( int c = 1; c <= dirty.Length(); c++ )
	{
		String ch = dirty[c];

		if ( ch != "\\" ) j = j + ch;
	}

	return j;
}









