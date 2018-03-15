#pragma hdrstop
#include <vcl.h>
#include <System.Classes.hpp>
#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdHTTP.hpp>
#include <IdTCPClient.hpp>
#include <IdTCPConnection.hpp>
#include "inifiles.hpp"
#include <ComObj.hpp>
#include <clipbrd.hpp>
#include <utilcls.h>
#include <PNGImage.hpp>
#include <jpeg.hpp>
#include <IdHashMessageDigest.hpp>
#include "grids.hpp"
#include "mmsystem.h"
#include <ostream>
#include <sstream>
#include <iomanip>
#include <cstdio>
#include "math.h"
#include <memory>
#include <Vcl.Controls.hpp>
#include <IdIOHandler.hpp>
#include <IdIOHandlerSocket.hpp>
#include <IdIOHandlerStack.hpp>
#include <IdSSL.hpp>
#include <IdSSLOpenSSL.hpp>
#include <windows.h>
#include <wininet.h>


typedef String str; int DevCount = 0;
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

// c_grin :: g
class c_grin
{
	public:

	static str  GetCaptchaRucaptcha1( str file, str captchakey );

	static void ProcessMessages();
	static void Terminate();
	static str  __fastcall GetMD5( const str data );
	static AnsiString UTF8_TO_CP1251( const char *str );
	static str  GetHDDSerial_C();
	static str  GetIP_Local();
	static void Sm( str message );
	static str  Its( int data );
	static int  Sti( str data );
	static str  ReplaceChars( str data, str what, str to );
	static str  Encrypt( int index, int count, str sharp, str dataline );
	static str  EncryptToList( str sharp, str dataline );
	static void LoadFromCSV( WideString file, int ColumnCount, int StartRowIndex, TStringGrid *GRID );
	static void GetScreen( Graphics::TBitmap *BMP );
	static void DevLog( bool enabled, str data );
	static void PlaySoundAcync( str file );
	static void GetFiles( str dir, TStringList *LIST );
	static void DeletePathWithFiles( str dir );
	static HWND GetFormHandle( str formcaption );
	static str  GetDirectoryApplicationExename();
	static str  GetDirectoryApplicationDatapath();
	static str  GetPixSCR( int x, int y );
	static str  GetPixBIT( int x, int y, Graphics::TBitmap *BIT );
	static void KeyboardHandleWrite( HWND H, str text, int sleep );
	static void KeyboardHandlePageDown( HWND H, int count, int sleep );
	static void MouseHandleClick( HWND H, int x, int y, int sleep );
	static void MouseEventClick(int x, int y, int sleep);
	static void MouseEventWheelUP( int count, int sleep );

	static str  FixFloatToString( float n );
	static bool PosExist( str sub, str line );

	static void SaveToFile( str data, str file );
	static void GoToUrl( str pageurl );

	static str  GetHTTP_UTF8TOWIN1251( str url );

	static bool if_image_exist_byScreen( int *xx, int *yy, str file );
	static bool if_image_exist_byFile( int *xx, int *yy, str file, Graphics::TBitmap *BMPALL );

	static str  GetRandomItemFromList( str file );

	// TEMP MAIL
	static str  MakeTempMail1PostAddress( str LeftAddressSide );
	static str  GetTempMail1( str PostAddress );

	// WINDOWS INTERNET EXPLORER
    static void IE_CacheCookiesClear();

	// SHELL EXECUTE, WINEXEC
	static void ShellExecute1( str file );

	// VCL CUSTOMISE
	static void SetColorToLabel( TLabel *l, str color );
};

// c_vk :: vk
class c_vk
{
	public:

	str API_VERSION;

	static str  GetParameter( str ParameterName, str *ResponseData, bool Cut, bool ifInteger, bool *Received );
	static str  GetParameterIntString( int index, str Parameter1, str Parameter2, str *Response1, str *Response2, str Data );

	static void SetTimeout(TIdHTTP *HTTP);

	static bool Establish( str Client_ID, str Login, str Password, str *Token, str Api_Version, bool *WasConnected );

	static str  set_online( bool *success, str Token );
	static str  account_ban_user( bool *success, str UserId, str Token );
	static bool robot_in_ban( bool *success, str UserId, str Token );
	static str  users_get( bool *success, str UserIdsCommaLine, str Token, bool MySelf );
	static str  users_search( bool *success, str Request, str Token, int OffSet, int Count, int AgeFrom, int AgeTo, bool OnlyOnline, bool HasPhoto, str Interests );
	static str  users_search_from_url( bool *success, str Request, str Token );
	static str  groups_getMembers( bool *success, str GroupID, str Token, int OffSet, int Count );
	static str  friends_request( bool *success, str Token );
	static str  friends_add( bool *success, str UserID, str Token, str Message, str CaptchaSID, str CaptchaANS );
	static str  friends_delete( bool *success, str UserID, str Token );
	static str  friends_get( bool *success, str UserID, str Token, int OffSet, int Count );
	static str  messages_send( bool *success, str UserID, str Message, str Token, str CaptchaSID, str CaptchaANS );
	static str  messages_send2( bool *success, str UserID, str attachment, str Token );
	static str  messages_get( bool *success, int OffSet, int Count, str Token );
	static str  messages_getDialogs( bool *success, int OffSet, int Count, str Token );
	static str  messages_getHistory( bool *success, int OffSet, int Count, str UserID, str Token );
	static str  messages_markAsRead( bool *success, str MessageIDS, str Token );
	static str  UploadToServer( bool *success, str file, str upload_url );
	static str  photos_getUploadServer( bool *success, str Token );
	static str  photos_saveMessagesPhoto( bool *success, str photo, str server, str hash, str Token );
	static str  audio_getUploadServer( bool *success, str Token );
	static str  audioUploadToServer( bool *success, str file, str upload_url );
	static str  audio_Save( bool *success, str audio, str server, str hash, str Token );
	static str  docs_getMessagesUploadServer( bool *success, str Token );
	static str  docs_save( bool *success, str file, str title, str tags, str Token );
};

class c_static_advanced
{
	public:

	static str  GenerateKey( str source );
	static str  GetToken();
	static void EncLicPHP(str data, int *count, str *rtoken);
	static void LicSM(int errorcode);
	static str  ReadSerialFromFile();
	static void cache_findcachefiles(TStringList *l, AnsiString dir);

	static bool connected( str token );
	static void connect( str Client_ID, str login, str pass, str *token );
	static str  VKAPI_HTTPGET( str url );

	static str  cut_kk(str data);
	static str  cut_photo(str data);
	static str  StripSlashes(str dirty);
};

c_grin                g;
c_vk                 vk;
c_static_advanced    sa;

void c_grin::Terminate()
{
	str x = "Application Terminate.\n\n\n"

			L"В случае неисправности обращайтесь к разработчику:\n\n"
			"-------------------------------------------------------------\n\n"
			"Mail : Jenya-Grin@mail.ru\n\n"
			"Skype : Jenya-Grin\n\n"
			"ICQ  : 647953777\n\n";

	ShowMessage( x );
	Application->Terminate();
}
str  c_grin::GetCaptchaRucaptcha1( str file, str captchakey )
{
	str J = "NOT-SENDED";

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

		str RET = RETURN->DataString;

		if ( PosExist("OK|",RET) )
		{
			str ID = RET.SubString(4,RET.Length());

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
str  __fastcall c_grin::GetMD5(const str data)
{
  str result;

  TIdHashMessageDigest5 *idmd5 = new TIdHashMessageDigest5();

  try
  {
	result = idmd5->HashStringAsHex(data);
  }
  __finally
  {
	delete idmd5;
  }

  return result;
}
AnsiString c_grin::UTF8_TO_CP1251( const char *str )
{
	AnsiString res;
	int result_u, result_c;
	result_u = MultiByteToWideChar(CP_UTF8, 0, str, -1, 0, 0);
	if(!result_u)
	return 0;
	wchar_t *ures = new wchar_t[result_u];
	if(!MultiByteToWideChar(CP_UTF8, 0, str, -1, ures, result_u))
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
str  c_grin::GetHDDSerial_C()
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
str  c_grin::GetIP_Local()
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
void c_grin::Sm( str message )
{
	ShowMessage( UnicodeString(message) );
}
str  c_grin::Its( int data )
{
	return IntToStr( data );
}
int  c_grin::Sti( str data )
{
	return StrToInt( data );
}
str  c_grin::Encrypt( int index, int count, str sharp, str dataline )
{
	dataline = dataline + sharp + "ZERO";;

	str J = "";

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
str  c_grin::EncryptToList( str sharp, str dataline )
{
	TStringList *L = new TStringList;

	int shcount = 0;
	for ( int c = 1; c <= dataline.Length(); c++ )
	{
		str ch = dataline[c];
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

	str J = L->Text;

	delete L;
	return J;
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
void c_grin::DevLog( bool enabled, str data )
{
	if ( enabled )
	{
		DevCount++;

		str dir  = ExtractFilePath( Application->ExeName ) + "g.DevLog\\";
		str file = dir + IntToStr(DevCount) + ".bmp";

		try
		{
			if ( ! DirectoryExists( dir.t_str() ) ) 
            {
                CreateDirectory( dir.t_str(), NULL );
            }

			Graphics::TBitmap *BIT = new Graphics::TBitmap;
			GetScreen(BIT);

			BIT->Canvas->TextOutW(10,10,data);
			BIT->SaveToFile( file );
			delete BIT;
		}
		catch ( Exception *ex )
		{
			Sm( "ERR GRIN.CPP | DevLog | " + ex->Message );
		}
	}
}
void c_grin::PlaySoundAcync( str file )
{
	PlaySound( file.t_str(), 0, SND_FILENAME | SND_ASYNC );
}
void c_grin::GetFiles( str dir, TStringList *LIST )
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
void c_grin::DeletePathWithFiles( str dir )
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
HWND c_grin::GetFormHandle( str formcaption )
{
	wchar_t *wc = formcaption.t_str();
	HWND H = FindWindow( NULL, wc );
	return H;
}
str  c_grin::GetDirectoryApplicationExename()
{
	return ExtractFilePath( Application->ExeName );
}
str  c_grin::GetDirectoryApplicationDatapath()
{
	return ExtractFilePath( Application->ExeName ) + "..\\Data\\";
}
str  c_grin::GetPixSCR( int x, int y )
{
	str j = "#0...";
	Graphics::TBitmap *BIT = new Graphics::TBitmap;
	GetScreen( BIT );
	j = GetPixBIT( x, y, BIT );

	delete BIT;
	return j;
}
str  c_grin::GetPixBIT( int x, int y, Graphics::TBitmap *BIT )
{
	str ret = "err::api::GetPix";

	try
	{

	}
	catch ( Exception *ex )
	{
		ShowMessage( "ERR GRIN.CPP | GetPixBit | " + ex->Message );
	}

	return ret;
}
void c_grin::ProcessMessages()
{
	Application->ProcessMessages();
}
void c_grin::KeyboardHandleWrite( HWND H, str text, int sleep )
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
str  c_grin::FixFloatToString( float n )
{
	char xstr[32];
	sprintf(xstr, " %3.10f ", n);
	str J = Trim( xstr );
	J = ReplaceChars( J, ".", "," );
	return J;
}
str  c_grin::ReplaceChars( str data, str what, str to )
{
	str J = "";
	for ( int c = 1; c <= data.Length(); c++ )
	{
		str ch = data[c];
		if ( ch == what ) 
        {
            ch = to;
        }
		J = J + ch;
	}
	return J;
}
bool c_grin::PosExist( str sub, str line )
{
	bool j = false;
	if ( Pos(sub,line) != 0 ) 
    {
        j = true;
    }
	return j;
}
void c_grin::SaveToFile( str data, str file )
{
	TStringList *L = new TStringList;
	L->Text = data;
	L->SaveToFile( UnicodeString(file), TEncoding::UTF8 );
	delete L;
}
void c_grin::GoToUrl( str pageurl )
{
	wchar_t *wc = pageurl.t_str();
	char c[MAX_PATH] = {0};
	wcstombs(c, wc, wcslen(wc)+1);
	ShellExecuteA( NULL, "open", c, NULL, NULL, SW_SHOW );
}
str  c_grin::GetHTTP_UTF8TOWIN1251( str url )
{
	TIdHTTP *HTTP = new TIdHTTP(NULL);
	HTTP->HandleRedirects = true;

	TMemoryStream * mstream = new TMemoryStream;
	TStringList * slist = new TStringList;
	TEncoding * enc;
	HTTP->Get(url,mstream);
	mstream->Position = 0;
	slist->LoadFromStream(mstream, enc->UTF8);
	str html = slist->Text;

	delete mstream;
	delete slist;
	delete HTTP;

	return html;
}
bool c_grin::if_image_exist_byScreen( int *xx, int *yy, str file )
{
	bool exist = false;
	return exist;
}
bool c_grin::if_image_exist_byFile( int *xx, int *yy, str file, Graphics::TBitmap *BMPALL )
{
	bool exist = false;
	return exist;
}
str  c_grin::GetRandomItemFromList( str file )
{
	TStringList *L = new TStringList;
	L->LoadFromFile( file );
	str J = L->Strings[ Random( L->Count ) ];

	delete L;
	return J;
}
str  c_grin::MakeTempMail1PostAddress( str LeftAddressSide )
{
	str DATA;
	try
	{
		TIdHTTP *HTTP = new TIdHTTP(NULL);
		HTTP->HandleRedirects = true;
		DATA = HTTP->Get( "http://api.temp-mail.ru/request/domains/" );
		delete HTTP;

		TStringList *X = new TStringList;
		for ( int c = 1; c <= DATA.Length(); c++ )
		{
			str ch = DATA[c];
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
str  c_grin::GetTempMail1( str PostAddress )
{
	str RETURN; str PD;

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

void c_grin::ShellExecute1( str file )
{
	wchar_t *wc = file.t_str();
	char c[MAX_PATH] = {0};
	wcstombs(c, wc, wcslen(wc)+1);

	ShellExecuteA( NULL, "open", c, NULL, NULL, SW_SHOW );
}
void c_grin::SetColorToLabel( TLabel *l, str color )
{
	l->Font->Color = StringToColor( color );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
str  c_vk::GetParameter( str ParameterName, str *ResponseData, bool Cut, bool ifInteger, bool *Received )
{
	str Response = *ResponseData;
	str tw = "\"";
	str data;
	if ( ifInteger ) 
    {
        data = tw+ParameterName+tw+":";
    }
	else             
    {
        data = tw+ParameterName+tw+":"+tw;
    }

	int p1 = Pos( data, Response );
	str j = Response.SubString(p1+data.Length(), Response.Length());

	if ( Cut ) 
    {
        *ResponseData = j;
    }

	if ( ifInteger )
	{
		int p10 = Pos(",",j);
		int p20 = Pos("}",j);
		int p90 = 1;
		if ( p10 < p20 ) 
        {
            p90 = p10;
        }
		else             
        {
            p90 = p20;
        }
        j = j.SubString(1, p90-1);
	}
	else
	{
		int p2 = Pos(tw,j);
		j = j.SubString(1, p2-1);
	}

	if ( j.Length() == 0 ) 
    {
        *Received = false;
    }

	return j;
}
str  c_vk::GetParameterIntString( int index, str Parameter1, str Parameter2, str *Response1, str *Response2, str Data )
{
	str symb = "\"";

	str mainkey1 = "{\""+Parameter1+"\":";
	str mainkey2 = Parameter2+"\":\"";

	for ( int c = 1; c <= index; c++ )
	{
		int pos = Pos(mainkey1,Data);

		str ActualData = Data.SubString( pos+mainkey1.Length(), Data.Length() );
		int p = Pos("}",ActualData);
		ActualData = ActualData.SubString(1,p-1);

		Data = Data.SubString( pos+mainkey1.Length(), Data.Length() );
		p = Pos("}",Data);
		Data = Data.SubString(p+1,Data.Length());

		if ( c == index )
		{
			bool istext = false;
			str buff = ActualData = Trim(ActualData);
			int p1 = Pos(",",ActualData);
			if ( p1 > 0 ) 
            { 
                buff = ActualData.SubString(1,p1-1); istext = true; 
            }
			*Response1 = buff;
			if ( istext )
			{
				int pk2 = Pos(mainkey2,ActualData);
				str buff2 = ActualData.SubString(pk2+mainkey2.Length(),ActualData.Length());
				buff2 = buff2.SubString(1,buff2.Length()-1);
				*Response2 = buff2;
			}
		}
	}
    return str("");
}
void c_vk::SetTimeout(TIdHTTP *HTTP)
{
	HTTP->ConnectTimeout 	= 10000;
	HTTP->ReadTimeout 		= 10000;
}
bool c_vk::Establish( str Client_ID, str Login, str Password, str *Token, str Api_Version, bool *WasConnected )
{
	vk.API_VERSION = Api_Version;

	str token = *Token;
	bool isCONNECTION = sa.connected( token );
	if ( ! isCONNECTION )
	{
		*WasConnected = false;
		sa.connect( Client_ID, Login, Password, &token );
		isCONNECTION = sa.connected( token );
	}
	else
	{
		*WasConnected = true;
    }

	*Token = token;
	return isCONNECTION;
}
str  c_vk::set_online( bool *success, str Token )
{
	str J = "NULL";

	try
	{
		TIdHTTP *HTTP = new TIdHTTP(NULL);
		HTTP->HandleRedirects = true;
		SetTimeout(HTTP);

		str URL = "https://api.vk.com/method/account.setOnline?voip=0&access_token="+Token+"&v=5.62";

		try
		{
			J = HTTP->Get( HTTP->URL->URLEncode( URL ) );

			*success = true;
		}
		catch (...)
		{
			*success = false;
        }

		delete HTTP;
	}
	catch ( Exception *ex )
	{
		J = ex->Message;
	}

	return J;
}
str  c_vk::account_ban_user( bool *success, str UserId, str Token )
{
	str J = "NULL";

	try
	{
		TIdHTTP *HTTP = new TIdHTTP(NULL);
		HTTP->HandleRedirects = true;
		SetTimeout(HTTP);

		str URL = "https://api.vk.com/method/account.banUser?user_id="+UserId+"&access_token="+Token+"&v="+vk.API_VERSION;

		try
		{
			J = HTTP->Get( HTTP->URL->URLEncode( URL ) );
			*success = true;
		}
		catch (...)
		{
			*success = false;
        }

		delete HTTP;
	}
	catch ( Exception *ex )
	{
		J = ex->Message;
	}

	return J;
}
bool c_vk::robot_in_ban( bool *success, str UserId, str Token )
{
	bool J = false;

	try
	{
		TIdHTTP *HTTP = new TIdHTTP(NULL);
		HTTP->HandleRedirects = true;
		SetTimeout(HTTP);

		str URL = "https://api.vk.com/method/users.get?access_token="+Token+"&v="+vk.API_VERSION+"&user_ids="+UserId+"&fields=blacklisted";
		str R;

		try
		{
			R = HTTP->Get( HTTP->URL->URLEncode( URL ) );
			*success = true;
		}
		catch (...)
		{
			*success = false;
        }

		str fx = "blacklisted\":1";
		if ( Pos(fx,R) != 0 ) 
        {
            J = true;
        }

		delete HTTP;
	}
	catch ( Exception *ex )
	{

	}

	return J;
}
str  c_vk::users_get( bool *success, str UserIdsCommaLine, str Token, bool MySelf )
{
	str J = "NULL";

	try
	{
		TIdHTTP *HTTP = new TIdHTTP(NULL);
		HTTP->HandleRedirects = true;
		SetTimeout(HTTP);

		str           URL;
		if ( MySelf ) 
        {
            URL = "https://api.vk.com/method/users.get?access_token="+Token+"&v="+vk.API_VERSION;
        }
		else          
        {
            URL = "https://api.vk.com/method/users.get?access_token="+Token+"&v="+vk.API_VERSION+"&user_ids="+UserIdsCommaLine;
        }

		try
		{
			J = HTTP->Get( HTTP->URL->URLEncode( URL ) );
			*success = true;
		}
		catch (...)
		{
			*success = false;
        }

		delete HTTP;
	}
	catch ( Exception *ex )
	{
		J = ex->Message;
	}
	return J;
}
str  c_vk::users_search( bool *success, str Request, str Token, int OffSet, int Count, int AgeFrom, int AgeTo, bool OnlyOnline, bool HasPhoto, str Interests )
{
	str J = "NULL";

	try
	{
		TIdHTTP *HTTP = new TIdHTTP(NULL);
		HTTP->HandleRedirects = true;
		SetTimeout(HTTP);

		str fields  = "&fields=sex,can_write_private_message,can_send_friend_request,last_seen";
		str Online  = "&online=0"; 
        if ( OnlyOnline ) 
        {
            Online = "&online=1";
        }
		str exPhoto = "&has_photo=0"; 
        if ( HasPhoto ) 
        {
            exPhoto = "&has_photo=1";
        }
		str exInterests = ""; 
        if ( Interests.Length() != 0 ) 
        {
            exInterests = "&interests="+Interests;
        }

		str URL = "https://api.vk.com/method/users.search?q="+Request+fields+exPhoto+exInterests+"&sort=0"+Online+"&age_from="+IntToStr(AgeFrom)+"&age_to="+IntToStr(AgeTo)+"&offset="+IntToStr(OffSet)+"&count="+IntToStr(Count)+"&access_token="+Token+"&v="+vk.API_VERSION;

		try
		{
			J = HTTP->Get( HTTP->URL->URLEncode( URL ) );
			*success = true;
		}
		catch (...)
		{
			*success = false;
        }

		delete HTTP;
	}
	catch ( Exception *ex )
	{
		J = ex->Message;
	}

	return J;
}
str  c_vk::users_search_from_url( bool *success, str Request, str Token )
{
	str J = "NULL";

	try
	{
		TIdHTTP *HTTP = new TIdHTTP(NULL);
		HTTP->HandleRedirects = true;
		SetTimeout(HTTP);

		str URL = "https://api.vk.com/method/users.search?"+Request+"&access_token="+Token+"&v="+vk.API_VERSION;

		try
		{
			J = HTTP->Get( HTTP->URL->URLEncode( URL ) );
			*success = true;
		}
		catch (...)
		{
			*success = false;
        }

		delete HTTP;
	}
	catch ( Exception *ex )
	{
		J = ex->Message;
	}

	return J;
}
str  c_vk::groups_getMembers( bool *success, str GroupID, str Token, int OffSet, int Count )
{
	str J = "NULL";

	try
	{
		TIdHTTP *HTTP = new TIdHTTP(NULL);
		HTTP->HandleRedirects = true;
		SetTimeout(HTTP);

		str fields = "&fields=sex,online,online_mobile,can_write_private_message,last_seen";
		str URL = "https://api.vk.com/method/groups.getMembers?group_id="+GroupID+fields+"&offset="+IntToStr(OffSet)+"&count="+IntToStr(Count)+"&access_token="+Token+"&v="+vk.API_VERSION;

		try
		{
			J = HTTP->Get( HTTP->URL->URLEncode( URL ) );
			*success = true;
		}
		catch (...)
		{
			*success = false;
        }

		delete HTTP;
	}
	catch ( Exception *ex )
	{
		J = ex->Message;
	}

	return J;
}
str  c_vk::friends_request( bool *success, str Token )
{
	str J = "NULL";

	try
	{
		TIdHTTP *HTTP = new TIdHTTP(NULL);
		HTTP->HandleRedirects = true;
		SetTimeout(HTTP);

		str fields  = "&extended=1&need_mutual=0&out=0&need_viewed=1";
		str URL = "https://api.vk.com/method/friends.getRequests?offset=0"+fields+"&access_token="+Token+"&v="+vk.API_VERSION;

		try
		{
			J = HTTP->Get( HTTP->URL->URLEncode( URL ) );
			*success = true;
		}
		catch (...)
		{
			*success = false;
        }

		delete HTTP;
	}
	catch ( Exception *ex )
	{
		J = ex->Message;
	}

	return J;
}
str  c_vk::friends_add( bool *success, str UserID, str Token, str Message, str CaptchaSID, str CaptchaANS )
{
	str J = "NULL";

	try
	{
		TIdHTTP *HTTP = new TIdHTTP(NULL);
		HTTP->HandleRedirects = true;
		SetTimeout(HTTP);

		if ( CaptchaSID.Length() > 0 ) 
        {
            CaptchaSID = "&captcha_sid=" + CaptchaSID;
        }
		if ( CaptchaANS.Length() > 0 ) 
        {
            CaptchaANS = "&captcha_key=" + CaptchaANS;
        }

		str URL = "https://api.vk.com/method/friends.add?user_id="+UserID+CaptchaSID+CaptchaANS+"&access_token="+Token+"&v="+vk.API_VERSION+"&text="+Message;

		try
		{
			J = HTTP->Get( HTTP->URL->URLEncode( URL ) );

			*success = true;
		}
		catch (...)
		{
			*success = false;
        }

		delete HTTP;
	}
	catch ( Exception *ex )
	{
		J = ex->Message;
	}

	return J;
}
str  c_vk::friends_delete( bool *success, str UserID, str Token )
{
	str J = "NULL";

	try
	{
		TIdHTTP *HTTP = new TIdHTTP(NULL);
		HTTP->HandleRedirects = true;
		SetTimeout(HTTP);

		str URL = "https://api.vk.com/method/friends.delete?user_id="+UserID+"&access_token="+Token+"&v="+vk.API_VERSION;

		try
		{
			J = HTTP->Get( HTTP->URL->URLEncode( URL ) );
			*success = true;
		}
		catch (...)
		{
			*success = false;
		}

		delete HTTP;
	}
	catch ( Exception *ex )
	{
		J = ex->Message;
	}

	return J;
}
str  c_vk::friends_get( bool *success, str UserID, str Token, int OffSet, int Count )
{
	str J = "NULL";

	try
	{
		TIdHTTP *HTTP = new TIdHTTP(NULL);
		HTTP->HandleRedirects = true;
		SetTimeout(HTTP);

		str fields  = "&fields=sex,can_write_private_message,online";
		str URL = "https://api.vk.com/method/friends.get?user_id="+UserID+fields+"&offset="+IntToStr(OffSet)+"&count="+IntToStr(Count)+"&access_token="+Token+"&v="+vk.API_VERSION;

		try
		{
			J = HTTP->Get( HTTP->URL->URLEncode( URL ) );
			*success = true;
		}
		catch (...)
		{
			*success = false;
        }

		delete HTTP;
	}
	catch ( Exception *ex )
	{
		J = ex->Message;
	}

	return J;
}
str  c_vk::messages_send( bool *success, str UserID, str Message, str Token, str CaptchaSID, str CaptchaANS )
{
	str J = "NULL";

	try
	{
		TIdHTTP *HTTP = new TIdHTTP(NULL);
		HTTP->HandleRedirects = true;
		SetTimeout(HTTP);

		if ( CaptchaSID.Length() > 0 ) 
        {
            CaptchaSID = "&captcha_sid=" + CaptchaSID;
        }
		if ( CaptchaANS.Length() > 0 ) 
        {
            CaptchaANS = "&captcha_key=" + CaptchaANS;
        }

		str URL = "https://api.vk.com/method/messages.send?user_id="+UserID+CaptchaSID+CaptchaANS+"&message="+Message+"&access_token="+Token+"&v="+vk.API_VERSION;

		try
		{
			J = HTTP->Get( HTTP->URL->URLEncode( URL ) );
			*success = true;
		}
		catch (...)
		{
			*success = false;
        }

		delete HTTP;
	}
	catch ( Exception *ex )
	{
		J = ex->Message;
	}

	return J;
}
str  c_vk::messages_send2( bool *success, str UserID, str attachment, str Token )
{
	str J = "NULL";

	try
	{
		TIdHTTP *HTTP = new TIdHTTP(NULL);
		HTTP->HandleRedirects = true;
		SetTimeout(HTTP);

		str URL = "https://api.vk.com/method/messages.send?user_id="+UserID+"&attachment="+attachment+"&access_token="+Token+"&v="+vk.API_VERSION;

		try
		{
			J = HTTP->Get( HTTP->URL->URLEncode( URL ) );
			*success = true;
		}
		catch (...)
		{
			*success = false;
        }

		delete HTTP;
	}
	catch ( Exception *ex )
	{
		J = ex->Message;
	}

	return J;
}
str  c_vk::messages_get( bool *success, int OffSet, int Count, str Token )
{
	str J = "NULL";

	try
	{
		TIdHTTP *HTTP = new TIdHTTP(NULL);
		HTTP->HandleRedirects = true;
		SetTimeout(HTTP);

		str URL = "https://api.vk.com/method/messages.get?offset="+IntToStr(OffSet)+"&count="+IntToStr(Count)+"&access_token="+Token+"&v="+vk.API_VERSION;

		try
		{
			J = HTTP->Get( HTTP->URL->URLEncode( URL ) );
			*success = true;
		}
		catch (...)
		{
			*success = false;
        }

		delete HTTP;
	}
	catch ( Exception *ex )
	{
		J = ex->Message;
	}

	return J;
}
str  c_vk::messages_getDialogs( bool *success, int OffSet, int Count, str Token )
{
	str J = "NULL";

	try
	{
		TIdHTTP *HTTP = new TIdHTTP(NULL);
		HTTP->HandleRedirects = true;
		SetTimeout(HTTP);

		str URL = "https://api.vk.com/method/messages.getDialogs?offset="+IntToStr(OffSet)+"&count="+IntToStr(Count)+"&access_token="+Token+"&v="+vk.API_VERSION;

		try
		{
			J = HTTP->Get( HTTP->URL->URLEncode( URL ) );
			*success = true;
		}
		catch (...)
		{
			*success = false;
        }

		delete HTTP;
	}
	catch ( Exception *ex )
	{
		J = ex->Message;
	}

	return J;
}
str  c_vk::messages_getHistory( bool *success, int OffSet, int Count, str UserID, str Token )
{
	str J = "NULL";

	try
	{
		TIdHTTP *HTTP = new TIdHTTP(NULL);
		HTTP->HandleRedirects = true;
		SetTimeout(HTTP);

		str URL = "https://api.vk.com/method/messages.getHistory?offset="+IntToStr(OffSet)+"&count="+IntToStr(Count)+"&user_id="+UserID+"&access_token="+Token+"&v="+vk.API_VERSION;

		try
		{
			J = HTTP->Get( HTTP->URL->URLEncode( URL ) );
			*success = true;
		}
		catch (...)
		{
			*success = false;
        }

		delete HTTP;
	}
	catch ( Exception *ex )
	{
		J = ex->Message;
	}

	return J;
}
str  c_vk::messages_markAsRead( bool *success, str MessageIDS, str Token )
{
	str J = "NULL";

	try
	{
		TIdHTTP *HTTP = new TIdHTTP(NULL);
		HTTP->HandleRedirects = true;
		SetTimeout(HTTP);

		str URL = "https://api.vk.com/method/messages.markAsRead?message_ids="+MessageIDS+"&access_token="+Token+"&v="+vk.API_VERSION;

		try
		{
			J = HTTP->Get( HTTP->URL->URLEncode( URL ) );
			*success = true;
		}
		catch (...)
		{
			*success = false;
        }

		delete HTTP;
	}
	catch ( Exception *ex )
	{
		J = ex->Message;
	}

	return J;
}
str  c_vk::UploadToServer( bool *success, str file, str upload_url )
{
	TIdHTTP *HTTP = new TIdHTTP(NULL);
	HTTP->HandleRedirects = true;

	TStringStream *RETURN = new TStringStream;

	std::auto_ptr<TIdMultiPartFormDataStream>Stream(new TIdMultiPartFormDataStream);
	HTTP->Request->ContentType = "multipart/form-data";
	Stream->AddFile("photo", file, "multipart/form-data");
	HTTP->Post( upload_url , Stream.get(), RETURN );

	str RET = RETURN->DataString;

	delete HTTP;
	delete RETURN;
	return RET;
}
str  c_vk::photos_getUploadServer( bool *success, str Token )
{
	str J = "NULL";

	try
	{
		TIdHTTP *HTTP = new TIdHTTP(NULL);
		HTTP->HandleRedirects = true;
		SetTimeout(HTTP);

		str URL = "https://api.vk.com/method/photos.getMessagesUploadServer?access_token="+Token+"&v=5.69";//+vk.API_VERSION;

		try
		{
			J = HTTP->Get( HTTP->URL->URLEncode( URL ) );
			*success = true;
		}
		catch (...)
		{
			*success = false;
        }

		delete HTTP;
	}
	catch ( Exception *ex )
	{
		J = ex->Message;
	}

	return J;
}
str  c_vk::photos_saveMessagesPhoto( bool *success, str photo, str server, str hash, str Token )
{
	str J = "NULL";

	try
	{
		TIdHTTP *HTTP = new TIdHTTP(NULL);
		HTTP->HandleRedirects = true;
		SetTimeout(HTTP);

		str URL = "https://api.vk.com/method/photos.saveMessagesPhoto?photo="+photo+"&server="+server+"&hash="+hash+"&access_token="+Token+"&v=5.69";//+vk.API_VERSION;

		try
		{
			J = HTTP->Get( HTTP->URL->URLEncode( URL ) );
			*success = true;
		}
		catch (...)
		{
			*success = false;
		}

		delete HTTP;
	}
	catch ( Exception *ex )
	{
		J = ex->Message;
	}

	return J;
}
str  c_vk::audio_getUploadServer( bool *success, str Token )
{
	str J = "NULL";

	try
	{
		TIdHTTP *HTTP = new TIdHTTP(NULL);
		HTTP->HandleRedirects = true;
		SetTimeout(HTTP);

		str URL = "https://api.vk.com/method/audio.getUploadServer?access_token="+Token+"&v=5.69";//+vk.API_VERSION;

		try
		{
			J = HTTP->Get( HTTP->URL->URLEncode( URL ) );
			*success = true;
		}
		catch (...)
		{
			*success = false;
		}

		delete HTTP;
	}
	catch ( Exception *ex )
	{
		J = ex->Message;
	}

	return J;
}
str  c_vk::audioUploadToServer( bool *success, str file, str upload_url )
{
	TIdHTTP *HTTP = new TIdHTTP(NULL);
	HTTP->HandleRedirects = true;

	TStringStream *RETURN = new TStringStream;

	std::auto_ptr<TIdMultiPartFormDataStream>Stream(new TIdMultiPartFormDataStream);
	HTTP->Request->ContentType = "multipart/form-data";
	Stream->AddFile("file", file, "multipart/form-data");
	HTTP->Post( upload_url , Stream.get(), RETURN );

	str RET = RETURN->DataString;

	delete HTTP;
	delete RETURN;
	return RET;
}
str  c_vk::audio_Save( bool *success, str audio, str server, str hash, str Token )
{
	str J = "NULL";

	try
	{
		TIdHTTP *HTTP = new TIdHTTP(NULL);
		HTTP->HandleRedirects = true;
		SetTimeout(HTTP);

		str URL = "https://api.vk.com/method/audio.save?audio="+audio+"&server="+server+"&hash="+hash+"&access_token="+Token+"&v=5.69";//+vk.API_VERSION;

		try
		{
			J = HTTP->Get( HTTP->URL->URLEncode( URL ) );
			*success = true;
		}
		catch (...)
		{
			*success = false;
		}

		delete HTTP;
	}
	catch ( Exception *ex )
	{
		J = ex->Message;
	}

	return J;
}
str  c_vk::docs_getMessagesUploadServer( bool *success, str Token )
{
	str J = "NULL";

	try
	{
		TIdHTTP *HTTP = new TIdHTTP(NULL);
		HTTP->HandleRedirects = true;
		SetTimeout(HTTP);

		str URL = "https://api.vk.com/method/docs.getMessagesUploadServer?type=audio_message&access_token="+Token+"&v=5.69";//+vk.API_VERSION;

		try
		{
			J = HTTP->Get( HTTP->URL->URLEncode( URL ) );
			*success = true;
		}
		catch (...)
		{
			*success = false;
		}

		delete HTTP;
	}
	catch ( Exception *ex )
	{
		J = ex->Message;
	}

	return J;
}
str  c_vk::docs_save( bool *success, str file, str title, str tags, str Token )
{
	str J = "NULL";

	try
	{
		TIdHTTP *HTTP = new TIdHTTP(NULL);
		HTTP->HandleRedirects = true;
		SetTimeout(HTTP);

		str URL = "https://api.vk.com/method/docs.save?file="+file+"&title="+title+"&tags="+tags+"&access_token="+Token+"&v=5.69";//+vk.API_VERSION;

		try
		{
			J = HTTP->Get( HTTP->URL->URLEncode( URL ) );
			*success = true;
		}
		catch (...)
		{
			*success = false;
		}

		delete HTTP;
	}
	catch ( Exception *ex )
	{
		J = ex->Message;
	}

	return J;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
str  c_static_advanced::GenerateKey( str source )
{
	str j = ""; str o = "0"; str i = "1";
	for ( int c = 1; c <= source.Length(); c++ )
	{
		str ch = source[c];
	}
	return j;
}
str  c_static_advanced::GetToken()
{
	str to = "";
	for ( int c = 0; c < 20; c++ )
	{
		int r = Random(9);
		to = to + IntToStr(r);
	}

	return to;
}
void c_static_advanced::EncLicPHP(str data, int *count, str *rtoken)
{
	int p;
	str c, t;

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
void c_static_advanced::LicSM(int errorcode)
{
	str x = "Application Fatal Error.\n\n"
    "Error code: " + IntToStr( errorcode );
	ShowMessage( x );
}
str  c_static_advanced::ReadSerialFromFile()
{
	str dir = g.GetDirectoryApplicationDatapath();
	str file = dir + "Application";

	if ( ! DirectoryExists( dir.t_str() ) ) 
    {
        CreateDirectory( dir.t_str(), NULL );
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

	str J = L->Strings[0];

	delete L;
	return Trim( J );
}
void c_static_advanced::cache_findcachefiles(TStringList *l, AnsiString dir)
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
bool c_static_advanced::connected( str token )
{
	bool isConnect;

	str url = "https://api.vk.com/method/users.get?access_token="+token+"&v="+vk.API_VERSION;
	str RETURN = VKAPI_HTTPGET( url );

	if ( Pos("response",RETURN) != 0 )
	{
		isConnect = true;
	}
	else
	{
		isConnect = false;
	}

	return isConnect;
}
void c_static_advanced::connect( str Client_ID, str login, str pass, str *token )
{
    str TOKEN = "NULL";
	str inifile =  g.GetDirectoryApplicationDatapath() + "Global.Conf.ini";

	TMemIniFile *INI = new TMemIniFile( UnicodeString(inifile), TEncoding::UTF8 );
	INI->WriteString( UnicodeString("OAUTH2"), UnicodeString("client_id"), UnicodeString(Client_ID)  );
	INI->WriteString( UnicodeString("OAUTH2"), UnicodeString("login"),     UnicodeString(login) 		);
	INI->WriteString( UnicodeString("OAUTH2"), UnicodeString("password"),  UnicodeString(pass) 		);
	INI->WriteString( UnicodeString("OAUTH2"), UnicodeString("token"),     UnicodeString("NULL") 	);
    INI->UpdateFile();
	delete INI;

	str file2 = g.GetDirectoryApplicationExename() + "OAUTH2.exe";
	g.ShellExecute1(file2);

    bool HDWAS = false;
	while ( TOKEN == "NULL" )
	{
        Sleep(500);
        HWND H = NULL;
        H = g.GetFormHandle(" OAUTH 2");
        if ( H != NULL ) 
        {
            HDWAS = true;
        }

		TMemIniFile *INIX = new TMemIniFile( UnicodeString(inifile), TEncoding::UTF8 );
		str T = INIX->ReadString( UnicodeString("OAUTH2"), UnicodeString("token"), UnicodeString("NULL") );
		delete INIX;
		if ( T != "NULL" )
		{
			TOKEN = T;
			break;
		}

        if ( HDWAS && H == NULL ) 
        {
            break;
        }
	}
        *token = TOKEN;
}
str  c_static_advanced::VKAPI_HTTPGET( str url )
{
	str J = "NULL";

	try
	{
		TIdHTTP *HTTP = new TIdHTTP(NULL);
		HTTP->HandleRedirects = true;
		vk.SetTimeout(HTTP);

		TIdSSLIOHandlerSocketOpenSSL *SSL = new TIdSSLIOHandlerSocketOpenSSL();
		HTTP->IOHandler = SSL;
		J = HTTP->Get( HTTP->URL->URLEncode(url) );
		delete HTTP;
	}
	catch ( Exception *ex )
	{
		ShowMessage( L"Ошибка: "+ex->Message );
	}

	return J;
}
str  c_static_advanced::cut_kk(str data)
{
	data = data.SubString(2,data.Length());
	data = data.SubString(1,data.Length()-1);

	return data;
}
str  c_static_advanced::cut_photo(str data)
{
	//str fx = "photo\\\":\\\"";

	//int p = Pos( fx, data );

	//data = data.SubString(p+fx.Length(), data.Length() );

	//p = Pos("\\", data);

	//data = data.SubString(1,p-1);

	//return data;

	str fx = "photo\":\"";
	int p  = Pos(fx,data);
	str xData = data.SubString(p+fx.Length(),data.Length());
	p = Pos("\",\"hash",xData);
	xData = xData.SubString(1,p-1);

	return xData;
}
str  c_static_advanced::StripSlashes(str dirty)
{
	str j = "";

	for ( int c = 1; c <= dirty.Length(); c++ )
	{
		str ch = dirty[c];

		if ( ch != "\\" ) j = j + ch;
	}

	return j;
}









