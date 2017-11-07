#include <vcl.h>
#pragma hdrstop
#include "CODE.h"
#include "Grin.cpp"
#include "inifiles.hpp"
#pragma package(smart_init)
#pragma resource "*.dfm"
Tf *f;
typedef String str;

bool LOADED = false;

str datapath;

TStringList *ACTIVEGROUPS = new TStringList;    // NAME
TStringList *ROBOTS 	  = new TStringList;    // NAME, TOKEN
TStringList *SETONLINE 	  = new TStringList;


void params(bool save);
void get_active_groups();
void get_robots_from_active_groups();
void make_setonline();
//
void get_robotdata( int index, str *GroupName, str *RobotName, str *Server_ID, str *Login, str *Password, str *Token, str *Activity );

__fastcall Tf::Tf(TComponent* Owner) : TForm(Owner)
{

}
void __fastcall Tf::FormShow(TObject *Sender)
{
	if ( ! LOADED )
	{
		datapath = ExtractFilePath( Application->ExeName ) + "\\";

		params(false);

		LOADED = true;
    }
}
void __fastcall Tf::b_DOClick(TObject *Sender)
{
	get_active_groups();

	get_robots_from_active_groups();

	make_setonline();
}

void params(bool save)
{
	TIniFile *INI = new TIniFile( datapath+"Online_Maker.ini" );

	if ( save )
	{
		INI->WriteString( "main", "address", Trim(f->Edit2->Text) );
	}
	else
	{
		f->Edit2->Text = INI->ReadString( "main", "address", "" );
	}

	delete INI;
}
void get_active_groups()
{
	ACTIVEGROUPS->LoadFromFile( datapath+"WorkGroups.dat" );
}
void get_robots_from_active_groups()
{
	TStringList *ALLROBOTS = new TStringList;
	g.GetFiles(datapath+"Robots\\",ALLROBOTS);

	for ( int c = 0; c < ALLROBOTS->Count; c++ )
	{
		str GroupName, RobotName, Server_ID, Login, Password, Token, Activity;
		get_robotdata( c, &GroupName, &RobotName, &Server_ID, &Login, &Password, &Token, &Activity );

		bool ifactive = false;

		for ( int x = 0; x < ACTIVEGROUPS->Count; x++ )
		{
			if ( ACTIVEGROUPS->Strings[x] == GroupName )
			{
				ifactive = true;
				break;
            }
		}

		if ( ifactive ) ROBOTS->Add( RobotName + "#" + Token );
	}
}
void make_setonline()
{
    SETONLINE->Clear();

	SETONLINE->Add( "<?php " );
	SETONLINE->Add( "" );

	for ( int c = 0; c < ROBOTS->Count; c++ )
	{
		str dataline = ROBOTS->Strings[c];

		SETONLINE->Add( "$ROBOTNAME = \""+g.Encrypt(1,2,"#",dataline)+"\";" );
		SETONLINE->Add( "$TOKEN = '"+g.Encrypt(2,2,"#",dataline)+"';" );
		SETONLINE->Add( "$AURL = 'https://api.vk.com/method/account.setOnline?voip=0&access_token='.$TOKEN.'&v=5.62';" );
		SETONLINE->Add( "$myCurl = curl_init(); curl_setopt_array($myCurl, array(CURLOPT_URL => $AURL,CURLOPT_RETURNTRANSFER => true,CURLOPT_POST => true, CURLOPT_POSTFIELDS => http_build_query(array()))); $response = curl_exec($myCurl); curl_close($myCurl); sleep(1);" );
		SETONLINE->Add( "" );
	}

	SETONLINE->Add( "?>" );

	SETONLINE->SaveToFile( datapath+"SetOnline.php" );

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	TIdHTTP *HTTP = new TIdHTTP(NULL);
    HTTP->HandleRedirects = true;
    TStringStream *RETURN = new TStringStream;

    std::auto_ptr<TIdMultiPartFormDataStream>Stream(new TIdMultiPartFormDataStream);
    HTTP->Request->ContentType = "multipart/form-data";

	Stream->AddFormField( "XCODE",  "X3464564245" );

	Stream->AddFile( "file", datapath+"SetOnline.php", "multipart/form-data" );

	HTTP->Post( f->Edit2->Text, Stream.get(), RETURN );

    str RET = RETURN->DataString;

	ShowMessage( "Данные отправлены. \n\n"+RET );

	delete HTTP;
	delete RETURN;
}
void get_robotdata( int index, str *GroupName, str *RobotName, str *Server_ID, str *Login, str *Password, str *Token, str *Activity )
{
	TStringList *L = new TStringList;
	g.GetFiles( datapath+"Robots\\", L );
	str robotpath = datapath+"Robots\\" + L->Strings[index] + "\\";

	TIniFile *INI = new TIniFile( robotpath + "Conf.ini" );

	*GroupName 	= INI->ReadString( "MAIN",    	 "owner",     "0" );
	*RobotName  = INI->ReadString( "MAIN",    	 "name",  	  "0" );
	*Login 		= INI->ReadString( "ACCOUNT",    "login",     "0" );
	*Password  	= INI->ReadString( "ACCOUNT",    "password",  "0" );
	*Activity 	= INI->ReadString( "CONNECTION", "activity",  "0" );
	*Server_ID 	= INI->ReadString( "CONNECTION", "client_id", "0" );
	*Token     	= INI->ReadString( "CONNECTION", "token",     "0" );

	delete INI;

	delete L;
}
// fs
void __fastcall Tf::Edit2Change(TObject *Sender)
{
	if ( LOADED ) params(true);
}

