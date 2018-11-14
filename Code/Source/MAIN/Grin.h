#ifndef GRIN_H
#define GRIN_H

#include "grids.hpp"

// c_grin :: g
class c_grin
{
	public:

	static String  GetCaptchaRucaptcha1( String file, String captchakey );

	static void ProcessMessages();
	static void Terminate();
	static String  __fastcall GetMD5( const String data );
	static AnsiString UTF8_TO_CP1251( const char *String );
	static String  GetHDDSerial_C();
	static String  GetIP_Local();
	static void Sm( String message );
	static String  Its( int data );
	static int  Sti( String data );
	static String  ReplaceChars( String data, String what, String to );
	static String  Encrypt( int index, int count, String sharp, String dataline );
	static String  EncryptToList( String sharp, String dataline );
	static void LoadFromCSV( WideString file, int ColumnCount, int StartRowIndex, TStringGrid *GRID );
	static void GetScreen( Graphics::TBitmap *BMP );
	static void DevLog( bool enabled, String data );
	static void PlaySoundAcync( String file );
	static void GetFiles( String dir, TStringList *LIST );
	static void DeletePathWithFiles( String dir );
	static HWND GetFormHandle( String formcaption );
	static String  GetDirectoryApplicationExename();
	static String  GetDirectoryApplicationDatapath();
	static String  GetPixSCR( int x, int y );
	static String  GetPixBIT( int x, int y, Graphics::TBitmap *BIT );
	static void KeyboardHandleWrite( HWND H, String text, int sleep );
	static void KeyboardHandlePageDown( HWND H, int count, int sleep );
	static void MouseHandleClick( HWND H, int x, int y, int sleep );
	static void MouseEventClick(int x, int y, int sleep);
	static void MouseEventWheelUP( int count, int sleep );

	static String  FixFloatToString( float n );
	static bool PosExist( String sub, String line );

	static void SaveToFile( String data, String file );
	static void GoToUrl( String pageurl );

	static String  GetHTTP_UTF8TOWIN1251( String url );

	static bool if_image_exist_byScreen( int *xx, int *yy, String file );
	static bool if_image_exist_byFile( int *xx, int *yy, String file, Graphics::TBitmap *BMPALL );

	static String  GetRandomItemFromList( String file );

	// TEMP MAIL
	static String  MakeTempMail1PostAddress( String LeftAddressSide );
	static String  GetTempMail1( String PostAddress );

	// WINDOWS INTERNET EXPLORER
    static void IE_CacheCookiesClear();

	// SHELL EXECUTE, WINEXEC
	static void ShellExecute1( String file );

	// VCL CUSTOMISE
	static void SetColorToLabel( TLabel *l, String color );

    static String  GenerateKey( String source );
	static String  GetToken();
	static void EncLicPHP(String data, int *count, String *rtoken);
	static void LicSM(int errorcode);
	static String  ReadSerialFromFile();
	static void cache_findcachefiles(TStringList *l, AnsiString dir);
	static String  cut_kk(String data);
	static String  cut_photo(String data);
	static String  StripSlashes(String dirty);
};

extern c_grin g;

#endif GRIN_H
