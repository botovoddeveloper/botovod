#pragma hdrstop
#include "MAIN.h"
#pragma package(smart_init)
#pragma link "sSkinManager"
#pragma link "sPageControl"
#pragma link "sRichEdit"
#pragma link "sCheckBox"
#pragma link "sGroupBox"
#pragma link "sListView"
#pragma link "sEdit"
#pragma link "sButton"
#pragma link "sBitBtn"
#pragma link "sCheckBox"
#pragma link "sComboBox"
#pragma link "acProgressBar"
#pragma link "sTreeView"
#pragma link "sListBox"
#pragma link "sLabel"
#pragma link "sRadioButton"
#pragma link "sDialogs"
#pragma link "acPNG"
#pragma link "acPNG"
#pragma link "acPNG"
#pragma link "acPNG"
#pragma link "acPNG"
#pragma resource "*.dfm"

#include <IdThreadComponent.hpp>
#include <IdComponent.hpp>
#include <IdIOHandler.hpp>
#include <IdIOHandlerSocket.hpp>
#include <IdIOHandlerStack.hpp>
#include <IdSSL.hpp>
#include <IdSSLOpenSSL.hpp>
#include <IdAntiFreeze.hpp>

#include <System.JSON.hpp>
#include <Data.DBXJSON.hpp>

#include <Inifiles.hpp>
#include <memory>
#include <FileCtrl.hpp>
#include "DateUtils.hpp"

#include "mmsystem.h"

#include "Grin.h"
#include "VKApi.h"


Tf *f;

bool DEMONSTRATION = false;
bool LOADED = false; 
int PAGEINDEX = 0;


__fastcall Tf::Tf(TComponent* Owner) : TForm(Owner)
{
	main 	= new c_main();
	proc 	= new c_process();
	ii      = new c_ii();
    online  = new c_online();
	vcl     = new c_vcl();
	captcha = new c_captcha();
	InitializeCriticalSection(&CS);
}
void __fastcall Tf::FormShow(TObject *Sender)
{
	if ( ! LOADED )
	{
		bool access = true;

		if ( access )
		{
			f->ClientHeight = 650;
			f->ClientWidth  = 905;

			f->PAGES->Align = alClient;
			f->PAGES->ActivePageIndex = 0;

			f->PAGES_CONFIGURATION->ActivePageIndex = 0;
			f->PAGES_MODELS_PAGEVIEW->ActivePageIndex = 1;
			f->PAGES_AUTOSTOP->ActivePageIndex = 0;

			f->ME_LOG->Font->Color = StringToColor("0x0095640D");
			::PostMessage(f->ME_LOG->Handle,WM_VSCROLL,SB_BOTTOM,0);

			BTEST1->SetFocus();
			main->conf(false);

			vcl->unset();
			vcl->set();
			vcl->EchoStatistic();
            vcl->ModelsSET(false);

			LOADED = true;
			T_OBJ_DRAW->Enabled = true;
			main->iMyTHREAD_ONLINE = new MyTHREAD_ONLINE(false);
		}
		else 
        {
            Application->Terminate();
        }
	}
}
void __fastcall Tf::FormClose(TObject *Sender, TCloseAction &Action)
{
    TRAY->Visible = false;
	g.ProcessMessages();
	ShellExecuteA( NULL, "open", "cmd.exe", "/K taskkill /im \"OAUTH2.exe\" /f /t", NULL, SW_HIDE );
	system("taskkill /im \"The Social Robots.exe\" /f /t");
}
void __fastcall Tf::PAGESChange(TObject *Sender)
{
	if ( LOADED )
	{
		if ( PAGES->ActivePageIndex != 4 )
		{
			PAGEINDEX = PAGES->ActivePageIndex;
		}
		else
		{
			f->Hide();
			PAGES->ActivePageIndex = PAGEINDEX;
			TRAY->Visible = true;
		}

		T_OBJ_DRAW->Enabled = true;
	}
}
void __fastcall Tf::PAGES_CONFIGURATIONChange(TObject *Sender)
{
	if ( LOADED )
	{
		if ( PAGES_CONFIGURATION->ActivePageIndex != 0 && 
             PAGES_CONFIGURATION->Pages[1]->Enabled == false )
		{
			PAGES_CONFIGURATION->ActivePageIndex = 0;
		}
		if ( PAGES_CONFIGURATION->ActivePageIndex == 0 ) 
        {
            LV_CONF_GROUPS->SetFocus();
        }
		if ( PAGES_CONFIGURATION->ActivePageIndex == 1 ) 
        {
            LV_CONF_ROBOTS->SetFocus();
        }
		if ( PAGES_CONFIGURATION->ActivePageIndex == 2 ) 
        {
            LV_CONF_USERS->SetFocus();
        }
	}
}
// C_MAIN
	 c_main::c_main()
{
    HTTPDOMAIN = "http://vk.robots.jenya.in.ua/";
	TERMINATED = true;

	clp = new TClipboard;

	f_ini 				= g.GetDirectoryApplicationDatapath() + "Global.Conf.ini";
	f_servers 			= g.GetDirectoryApplicationDatapath() + "Servers.dat";
	f_groups            = g.GetDirectoryApplicationDatapath() + "Groups.dat";
	f_workgroups        = g.GetDirectoryApplicationDatapath() + "WorkGroups.dat";
	f_worktasks         = g.GetDirectoryApplicationDatapath() + "WorkTasks.dat";
	f_users             = g.GetDirectoryApplicationDatapath() + "Users.dat";
	f_globaluserscache  = g.GetDirectoryApplicationDatapath() + "Global.Users.Cache";
	f_currentlog        = g.GetDirectoryApplicationDatapath() + "Logs\\_Current.log";
    f_soundcaptcha      = g.GetDirectoryApplicationDatapath() + "Sounds\\SoundCaptcha.wav";

	p_robots 			= g.GetDirectoryApplicationDatapath() + "Robots\\";
	p_dialogs           = g.GetDirectoryApplicationDatapath() + "Dialogs\\";
	p_dialogs_test      = g.GetDirectoryApplicationDatapath() + "DialogsTest\\";
	p_logs              = g.GetDirectoryApplicationDatapath() + "Logs\\";

    p_resources_images  = g.GetDirectoryApplicationDatapath() + "Resources\\Images\\";
	p_resources_audio   = g.GetDirectoryApplicationDatapath() + "Resources\\Audio\\";
	p_resources_records = g.GetDirectoryApplicationDatapath() + "Resources\\Records\\";

    checkDirectoryExisting(g.GetDirectoryApplicationDatapath());
    checkDirectoryExisting(p_robots);
    checkDirectoryExisting(p_dialogs);
    checkDirectoryExisting(p_dialogs_test);
    checkDirectoryExisting(p_logs);
    checkDirectoryExisting(g.GetDirectoryApplicationDatapath() + "Resources\\");
    checkDirectoryExisting(p_resources_images);
    checkDirectoryExisting(p_resources_audio);
    checkDirectoryExisting(p_resources_records);
    
	LOG = new TStringList;
    
    if(FileExists(f_currentlog))
    {
        LOG->LoadFromFile( f_currentlog );
        f->ME_LOG->Lines->LoadFromFile( f_currentlog );
    }

	GLOBAL_USERS_CACHE = new TStringList;
    if(FileExists(f_globaluserscache))
    {
	    GLOBAL_USERS_CACHE->LoadFromFile( f_globaluserscache );
    }

	CNT_HELLO 			= 0;
	CNT_ADDFRIEND 		= 0;
	CNT_MESSAGES 		= 0;
	CNT_CONFIRMFRIEND 	= 0;

	TINT_CURR       	= 0;

	INBOX 				= new TStringList;
	AUTOANSOUTBOX       = new TStringList;
	AUTOANSBUFF 		= new TStringList;

    NODE = new TTreeNode(NULL);

	MODEL_HELLO	  			= new TStringList;
    MODEL_GLOBAL            = new TStringList;
	MODEL_LOGICAL 			= new TStringList;
	MODEL_AUTOANS 			= new TStringList;
	MODEL_AUTOSTOP_KEYS 	= new TStringList;
	MODEL_AUTOSTOP_POSTS 	= new TStringList;

    POSTS 		= new TStringList;
    VARIANTS 	= new TStringList;
    IFVARIANTS  = new TStringList;
    EXTENDED 	= new TStringList;
    DEFAULT 	= new TStringList;

    AUTOANS_LIMIT = new TStringList;

	MOD 		= false;
	PREFIX = "";
}
void c_main::conf(bool save)
{
	conf_ini(save);
	conf_intervals(save);
	conf_captcha(save);
	conf_servers(save);
	conf_groups(save);
	conf_dialogs(save);
    conf_models(save);
	conf_worktasks(save);

	f->l_tinterval->Caption = f->e_int_timer_max->Text;
}
void c_main::conf_ini(bool save)
{
	std::auto_ptr<TMemIniFile> INI(new TMemIniFile( UnicodeString(f_ini),TEncoding::UTF8 ));

	if ( save )
	{
		INI->WriteInteger( UnicodeString("SERVER"), UnicodeString("current"),     CurrentServer );
		INI->WriteInteger( UnicodeString("OTHER"),  UnicodeString("robotpathid"), RobotPathID );
		INI->WriteString( UnicodeString("OTHER"),  UnicodeString("offset"),       UnicodeString(f->e_conf_users_offSet->Text) );
		INI->WriteString( UnicodeString("OTHER"),  UnicodeString("count"),        UnicodeString(f->e_conf_users_Count->Text) );
		INI->WriteString( UnicodeString("OTHER"),  UnicodeString("search_url"),   UnicodeString(f->e_conf_users_URL->Text) );
		INI->WriteBool(   UnicodeString("OTHER"),  UnicodeString("log"),         f->CH_LOG->Checked );
		INI->WriteBool(   UnicodeString("OTHER"),  UnicodeString("apiret"),      f->CH_APIRET->Checked );
		INI->WriteBool(   UnicodeString("OTHER"),  UnicodeString("loggoto"),     f->CH_LOGGOTO->Checked );
		INI->WriteBool(   UnicodeString("OTHER"),  UnicodeString("logpauses"),   f->CH_LOG_PAUSES->Checked );
		INI->WriteString( UnicodeString("OTHER"),  UnicodeString("currentlog"),    UnicodeString(BUFF_CURRENTLOG) );
		INI->WriteString( UnicodeString("OTHER"),  UnicodeString("autoanswerlimit"), UnicodeString(f->e_autoanswerlimit->Text) );
        INI->WriteString( UnicodeString("OTHER"),  UnicodeString("speechlimit"), f->e_speechlimit->Text );
		INI->WriteBool(   UnicodeString("OTHER"),  UnicodeString("dynamicsearch"), f->ch_dynamic_userbdata->Checked );
		INI->WriteBool(   UnicodeString("OTHER"),  UnicodeString("randomize_inc"), f->ch_RANDOMIZE->Checked );
		INI->WriteBool(   UnicodeString("OTHER"),  UnicodeString("alphakeys_use"), f->ch_ALPHAKEYS->Checked );
        INI->WriteString( UnicodeString("IODIALOGS"), UnicodeString("import_robot_models"), f_od_import_robot_model );
		INI->WriteString( UnicodeString("IODIALOGS"), UnicodeString("import_robots"), f_od_import_robots );
        INI->WriteString( UnicodeString("OTHER"),   UnicodeString("ccleaner_path"), UnicodeString(Trim(f->e_ccleanerpath->Text)) );
        
        INI->UpdateFile();
	}
	else
	{
		CurrentServer = INI->ReadInteger( UnicodeString("SERVER"), UnicodeString("current"),     0  );
		RobotPathID   = INI->ReadInteger( UnicodeString("OTHER"),  UnicodeString("robotpathid"), 0  );
		APIV          = INI->ReadString(  UnicodeString("OAUTH2"), UnicodeString("api"),        UnicodeString("5.0") );
		f->e_conf_users_offSet->Text  = INI->ReadString( UnicodeString("OTHER"),  UnicodeString("offset"),     UnicodeString("0") );
		f->e_conf_users_Count->Text   = INI->ReadString( UnicodeString("OTHER"),  UnicodeString("count"),      UnicodeString("0") );
		f->e_conf_users_URL->Text	  = INI->ReadString( UnicodeString("OTHER"),  UnicodeString("search_url"), UnicodeString("0") );
		f->CH_LOG->Checked 			  = INI->ReadBool(   UnicodeString("OTHER"),  UnicodeString("log") , false );
		f->CH_APIRET->Checked         = INI->ReadBool(   UnicodeString("OTHER"),  UnicodeString("apiret") , false );
		f->CH_LOGGOTO->Checked        = INI->ReadBool(   UnicodeString("OTHER"),  UnicodeString("loggoto") , false );
		f->CH_LOG_PAUSES->Checked     = INI->ReadBool(   UnicodeString("OTHER"),  UnicodeString("logpauses") , false );
		BUFF_CURRENTLOG               = INI->ReadString( UnicodeString("OTHER"),  UnicodeString("currentlog") , UnicodeString("0") );
		f->e_autoanswerlimit->Text    = INI->ReadString( UnicodeString("OTHER"),  UnicodeString("autoanswerlimit") , UnicodeString("0") );
        f->e_speechlimit->Text        = INI->ReadString( UnicodeString("OTHER"),  UnicodeString("speechlimit"), UnicodeString("0") );
		f->ch_dynamic_userbdata->Checked = INI->ReadBool( UnicodeString("OTHER"), UnicodeString("dynamicsearch"), false );
		f->ch_RANDOMIZE->Checked          = INI->ReadBool( UnicodeString("OTHER"), UnicodeString("randomize_inc"), false );
		f->ch_ALPHAKEYS->Checked          = INI->ReadBool( UnicodeString("OTHER"), UnicodeString("alphakeys_use"), false );
        f_od_import_robot_model       = INI->ReadString( UnicodeString("IODIALOGS"), UnicodeString("import_robot_models"), UnicodeString("") );
		f_od_import_robots 			  = INI->ReadString( UnicodeString("IODIALOGS"), UnicodeString("import_robots"), UnicodeString("") );
        f->e_ccleanerpath->Text       = INI->ReadString( UnicodeString("OTHER"),  UnicodeString("ccleaner_path"), UnicodeString("") );
	}
}
void c_main::conf_intervals(bool save)
{
	std::auto_ptr<TMemIniFile> INI(new TMemIniFile( UnicodeString(f_ini), TEncoding::UTF8 ));

	if ( save )
	{
		INI->WriteString(  UnicodeString("INTERVALS"),  UnicodeString("apirequest"),    UnicodeString(f->e_int_api->Text) );
		INI->WriteString(  UnicodeString("INTERVALS"),  UnicodeString("sendmess"), 	   UnicodeString(f->e_int_msg->Text) );
		INI->WriteString(  UnicodeString("INTERVALS"),  UnicodeString("addfriend"), 	   UnicodeString(f->e_int_addfr->Text) );
		INI->WriteString(  UnicodeString("INTERVALS"),  UnicodeString("confirmfriend"), UnicodeString(f->e_int_confirmfriend->Text) );
		INI->WriteString(  UnicodeString("INTERVALS"),  UnicodeString("timer"), 		   UnicodeString(f->e_int_timer_max->Text) );
		INI->WriteInteger( UnicodeString("INTERVALS"),  UnicodeString("entimer"), 	   f->ch_entimer->Checked );
        INI->UpdateFile();
	}
	else
	{
		f->e_int_api->Text 	     	= INI->ReadString(  UnicodeString("INTERVALS"), UnicodeString("apirequest"),    UnicodeString("0") );
		f->e_int_msg->Text       	= INI->ReadString(  UnicodeString("INTERVALS"), UnicodeString("sendmess"), 		UnicodeString("0") );
		f->e_int_addfr->Text        = INI->ReadString(  UnicodeString("INTERVALS"), UnicodeString("addfriend"),     UnicodeString("0") );
		f->e_int_confirmfriend->Text= INI->ReadString(  UnicodeString("INTERVALS"), UnicodeString("confirmfriend"), UnicodeString("0") );
		f->e_int_timer_max->Text    = INI->ReadString(  UnicodeString("INTERVALS"), UnicodeString("timer"), 		UnicodeString("0") );
		f->ch_entimer->Checked      = INI->ReadInteger( UnicodeString("INTERVALS"), UnicodeString("entimer"), 	     0	 );
	}

	TINT_CURR = 0;
	f->TINTERVAL->Enabled = false;
	f->TINTERVAL->Enabled = f->ch_entimer->Checked;
	if ( StrToInt(f->e_int_timer_max->Text) < 10 ) 
    {
        f->TINTERVAL->Enabled = false;
    }
	g.ProcessMessages();
}
void c_main::conf_captcha(bool save)
{
	std::auto_ptr<TMemIniFile> INI(new TMemIniFile( UnicodeString(f_ini), TEncoding::UTF8 ));

	if ( save )
	{
		INI->WriteInteger( UnicodeString("CAPTCHA"),  UnicodeString("serviceindex"), f->rg_CAPTCHA_SERVICE->ItemIndex );
		INI->WriteString(  UnicodeString("CAPTCHA"),  UnicodeString("key"),          UnicodeString(f->e_CAPTCHA_KEY->Text) );
        INI->WriteBool( UnicodeString("CAPTCHA"),  UnicodeString("sound"),          f->ch_soundcaptcha->Checked );
        INI->UpdateFile();
	}
	else
	{
		f->rg_CAPTCHA_SERVICE->ItemIndex = INI->ReadInteger( UnicodeString("CAPTCHA"),  UnicodeString("serviceindex"),       0  );
		f->e_CAPTCHA_KEY->Text           = INI->ReadString(  UnicodeString("CAPTCHA"),   UnicodeString("key"),               UnicodeString("0") );
        f->ch_soundcaptcha->Checked      = INI->ReadBool( UnicodeString("CAPTCHA"),  UnicodeString("sound"),              false );
	}

    if ( f->rg_CAPTCHA_SERVICE->ItemIndex == 0 ) 
    {   
        f->GroupBox4->Visible = true; 
        f->GroupBox3->Visible = false; 
    }
	if ( f->rg_CAPTCHA_SERVICE->ItemIndex == 1 ) 
    { 
        f->GroupBox4->Visible = false; 
        f->GroupBox3->Visible = true; 
    }
}
void c_main::conf_servers(bool save)
{
	std::auto_ptr<TStringList> L(new TStringList);

	if ( save )
	{
		for ( int c = 0; c < f->LV_SERVERS->Items->Count; c++ )
		{
			String id    = f->LV_SERVERS->Items->Item[c]->SubItems->Strings[0];
			String login = f->LV_SERVERS->Items->Item[c]->SubItems->Strings[1];
			String pass  = f->LV_SERVERS->Items->Item[c]->SubItems->Strings[2];

			L->Add(UnicodeString(id + "#" + login + "#" + pass));
		}
                                                              
		L->SaveToFile(UnicodeString(f_servers),TEncoding::UTF8);
	}
	else
	{
		f->LV_SERVERS->Items->Clear();

        if ( DEMONSTRATION )
		{
			f->b_SERVERS_ADD->Enabled 		= false;
			f->e_servers_client_id->Enabled = false;
			f->e_servers_login->Enabled 	= false;
			f->e_servers_password->Enabled  = false;

			GetServersFromNET( L.get() );

			f->lbl_DATAFROM->Caption = L"Данные были получены с сервера программы.";
		}
		else
		{
            if(FileExists(f_servers))
            {
                L->LoadFromFile( f_servers );
            }
            f->lbl_DATAFROM->Caption = L"Данные были загружены из локального файла.";
		}

		for ( int c = 0; c < L->Count; c++ )
		{
			String line = L->Strings[c];

			TListItem *ListItem;
			ListItem = f->LV_SERVERS->Items->Add();
			ListItem->Caption = " "+IntToStr( c+1 );
			ListItem->SubItems->Add( g.Encrypt(1,3,"#",line) );
			ListItem->SubItems->Add( g.Encrypt(2,3,"#",line) );
			ListItem->SubItems->Add( g.Encrypt(3,3,"#",line) );
        }
	}
}
void c_main::conf_groups(bool save)
{
	std::auto_ptr<TStringList> L(new TStringList);

	if ( save )
	{
		for ( int c = 0; c < f->LV_CONF_GROUPS->Items->Count; c++ )
		{
			L->Add(UnicodeString(Trim(f->LV_CONF_GROUPS->Items->Item[c]->Caption)));
		}

		L->SaveToFile(UnicodeString(f_groups),TEncoding::UTF8);
	}
	else
	{
		f->LV_CONF_GROUPS->Items->Clear();
        if(FileExists(f_groups))
        {
		    L->LoadFromFile( f_groups );
        }

		for ( int c = 0; c < L->Count; c++ )
		{
			TListItem *ListItem;
			ListItem = f->LV_CONF_GROUPS->Items->Add();
			ListItem->Caption = " " + L->Strings[c];
			ListItem->SubItems->Add( "X" );
			ListItem->SubItems->Add( "X" );
		}

		f->vcl->groupechoReadRobots();
		f->vcl->groupechoReadUsers();
	}
	conf_workgroups(false);
}
void c_main::conf_workgroups(bool save)
{
	std::auto_ptr<TStringList> L(new TStringList);

	if ( save )
	{
		for ( int c = 0; c < f->LV_WORKGROUPS->Items->Count; c++ )
		{
			if ( f->LV_WORKGROUPS->Items->Item[c]->Checked ) 
            {
                L->Add(UnicodeString(Trim(f->LV_WORKGROUPS->Items->Item[c]->Caption)));
            }
		}

        L->SaveToFile(UnicodeString(f_workgroups),TEncoding::UTF8);
	}
	else
	{
		f->LV_WORKGROUPS->Items = f->LV_CONF_GROUPS->Items;
        if(FileExists(f_workgroups))
        {
		    L->LoadFromFile( f_workgroups );
        }

		for ( int c = 0; c < f->LV_WORKGROUPS->Items->Count; c++ )
		{
			bool ex = false;
			String groupname = Trim( f->LV_WORKGROUPS->Items->Item[c]->Caption );
			for ( int x = 0; x < L->Count; x++ )
			{
				if ( L->Strings[x] == groupname )
				{
					ex = true;
                    false;
                }
			}

			f->LV_WORKGROUPS->Items->Item[c]->Checked = ex;
		}
	}
}
void c_main::conf_worktasks(bool save)
{
	std::auto_ptr<TStringList> L(new TStringList);

	if ( save )
	{
		for ( int c = 0; c < f->LV_WORKTASKS->Items->Count; c++ )
		{
			if ( f->LV_WORKTASKS->Items->Item[c]->Checked ) 
            {
                L->Add( "1" );
            }
			else                                            
            {
                L->Add( "0" );
            }
		}

		L->SaveToFile( UnicodeString(f_worktasks), TEncoding::UTF8 );
	}
	else if(FileExists(f_worktasks))
	{
		L->LoadFromFile( f_worktasks );       
        int pos = 0;
        f->LV_WORKTASKS->Items->Item[pos]->Checked = (L->Strings[pos] == "1");
		pos = 1;
		f->LV_WORKTASKS->Items->Item[pos]->Checked = (L->Strings[pos] == "1");
		pos = 2;
		f->LV_WORKTASKS->Items->Item[pos]->Checked = (L->Strings[pos] == "1");
		pos = 3;
		f->LV_WORKTASKS->Items->Item[pos]->Checked = (L->Strings[pos] == "1");
		pos = 4;
		f->LV_WORKTASKS->Items->Item[pos]->Checked = (L->Strings[pos] == "1");
        pos = 5;
		f->LV_WORKTASKS->Items->Item[pos]->Checked = (L->Strings[pos] == "1");
        if ( L->Count == 7 )
		{
            pos = 6;
		    f->LV_WORKTASKS->Items->Item[pos]->Checked = (L->Strings[pos] == "1");
        }
	}
}
void c_main::conf_robots(int index, bool save)
{
	std::auto_ptr<TStringList> L(new TStringList);

	if ( save )
	{
		String robotpath = p_robots + IntToStr(RobotPathID) + "_" + g.GetMD5(RobotPathID) + "\\";
        checkDirectoryExisting(robotpath);
        
		RobotPathID++;
		conf_ini( true );

		std::auto_ptr<TMemIniFile> INI(new TMemIniFile( UnicodeString(robotpath + "Conf.ini"), TEncoding::UTF8 ));
		INI->WriteString( UnicodeString("MAIN"),       UnicodeString("owner"),     UnicodeString(Trim(f->LV_CONF_GROUPS->Items->Item[index]->Caption)) );
		INI->WriteString( UnicodeString("MAIN"),       UnicodeString("name"),      UnicodeString(f->e_conf_robots_create_name->Text) );
        INI->WriteBool(   UnicodeString("MAIN"),       UnicodeString("freeze"),     false );
		INI->WriteString( UnicodeString("ACCOUNT"),    UnicodeString("login"),     UnicodeString(f->e_conf_robots_create_login->Text) );
		INI->WriteString( UnicodeString("ACCOUNT"),    UnicodeString("password"),  UnicodeString(f->e_conf_robots_create_password->Text) );
        INI->WriteString( UnicodeString("ACCOUNT"),    UnicodeString("online"),    UnicodeString(IntToStr( f->ch_robot_online->ItemIndex )) );
		INI->WriteString( UnicodeString("ACCOUNT"),    UnicodeString("lastonline"),UnicodeString("1970"));
		INI->WriteString( UnicodeString("CONNECTION"), UnicodeString("activity"),  UnicodeString("NULL") );
		INI->WriteString( UnicodeString("CONNECTION"), UnicodeString("client_id"), UnicodeString("NULL") );
		INI->WriteString( UnicodeString("CONNECTION"), UnicodeString("token"),     UnicodeString("NULL") );
        INI->UpdateFile();

		conf_robots( index, false );

		f->e_conf_robots_create_name->Clear();
		f->e_conf_robots_create_login->Clear();
		f->e_conf_robots_create_password->Clear();
		f->ch_robot_online->ItemIndex = 1;

		createStandartDataHello(L.get());
		L->SaveToFile(robotpath+"Hello.txt");

		createGlobalDataModel(L.get());
		L->SaveToFile(robotpath+"Global.txt");

		createStandartDataModel(L.get());
		L->SaveToFile(robotpath+"Model.txt");

		createStandartDataAutoAnsRules(L.get());
		L->SaveToFile(robotpath+"AutoAnsRules.txt");

		createStandartDataAutoAnsDefault(L.get());
		L->SaveToFile(robotpath+"AutoAnsDefault.txt");

		createStandartDataAutoStopKeys(L.get());
		L->SaveToFile(robotpath+"AutoStopKeys.txt");

		createStandartDataAutoStopPosts(L.get());
		L->SaveToFile(robotpath+"AutoStopPosts.txt");

        conf_models(false);
	}
	else
	{
		f->LV_CONF_ROBOTS->Items->Clear();
		String NeededGroupName = Trim(f->LV_CONF_GROUPS->Items->Item[index]->Caption);
		g.GetFiles( p_robots, L.get() );

        for ( int c = 0; c < L->Count; c++ )
		{
			String GroupName, RobotName, Server_ID, Login, Password, Token, Activity, Online, LastOnline; 
            bool freeze;
			get_robotdata( c, &GroupName, &RobotName, &Server_ID, &Login, &Password, &Token, &Activity, &Online, &LastOnline, &freeze );

			if ( GroupName == NeededGroupName )
			{
				if ( Token == "NULL" ) 
                {
                    Token = "-"; 
                }
                else 
                {
                    Token = g.GetMD5(Token);
                }
				if ( Server_ID == "NULL" ) 
                {
                    Server_ID = "-";
                }
				if ( Activity == "NULL" ) 
                {
                    Activity = "-";
                }
				if ( Online == "0" ) 
                {
                    Online = L"Да"; 
                }
                else 
                {
                    Online = "-";
                }



				TListItem *ListItem;
				ListItem = f->LV_CONF_ROBOTS->Items->Add();
				ListItem->Caption = " " + RobotName;
				ListItem->SubItems->Add( Online );
				ListItem->SubItems->Add( Server_ID );
				ListItem->SubItems->Add( Token );
				ListItem->SubItems->Add( Activity );

				if ( freeze ) 
                {
                    ListItem->Data = Pointer(clGray);
                }
				else         
                {
                    ListItem->Data = Pointer(clBlack);
                }
			}
		}
	}
}
void c_main::conf_robots_import(int groupindex, String name, String login, String pass)
{
	std::auto_ptr<TStringList> L(new TStringList);
    String robotpath = p_robots + IntToStr(RobotPathID) + "_" + g.GetMD5(RobotPathID) + "\\";
    CreateDir( robotpath );
    RobotPathID++;
    conf_ini( true );

    std::auto_ptr<TMemIniFile> INI(new TMemIniFile( UnicodeString(robotpath + "Conf.ini"), TEncoding::UTF8 ));
    INI->WriteString( UnicodeString("MAIN"),    UnicodeString("owner"), UnicodeString(Trim(f->LV_CONF_GROUPS->Items->Item[groupindex]->Caption)) );
    INI->WriteString( UnicodeString("MAIN"),    UnicodeString("name"),  UnicodeString(name) );
    INI->WriteString( UnicodeString("ACCOUNT"), UnicodeString("login"),     UnicodeString(login) );
    INI->WriteString( UnicodeString("ACCOUNT"), UnicodeString("password"),  UnicodeString(pass) );
    INI->WriteString( UnicodeString("ACCOUNT"), UnicodeString("online"),  UnicodeString("1") );
    INI->WriteString( UnicodeString("ACCOUNT"), UnicodeString("lastonline"),  UnicodeString("1970") );
    INI->WriteString( UnicodeString("CONNECTION"), UnicodeString("activity"),  UnicodeString("NULL") );
    INI->WriteString( UnicodeString("CONNECTION"), UnicodeString("client_id"), UnicodeString("NULL") );
    INI->WriteString( UnicodeString("CONNECTION"), UnicodeString("token"),     UnicodeString("NULL") );
    INI->UpdateFile();

    conf_robots( groupindex, false );

    f->e_conf_robots_create_name->Clear();
    f->e_conf_robots_create_login->Clear();
    f->e_conf_robots_create_password->Clear();
    f->ch_robot_online->ItemIndex = 1;

    createStandartDataHello(L.get());
    L->SaveToFile(UnicodeString(robotpath+"Hello.txt"), TEncoding::UTF8);

    createGlobalDataModel(L.get());
    L->SaveToFile(UnicodeString(robotpath+"Global.txt"), TEncoding::UTF8);

    createStandartDataModel(L.get());
    L->SaveToFile(UnicodeString(robotpath+"Model.txt"), TEncoding::UTF8);

    createStandartDataAutoAnsRules(L.get());
    L->SaveToFile(UnicodeString(robotpath+"AutoAnsRules.txt"), TEncoding::UTF8);

    createStandartDataAutoAnsDefault(L.get());
    L->SaveToFile(UnicodeString(robotpath+"AutoAnsDefault.txt"), TEncoding::UTF8);

    createStandartDataAutoStopKeys(L.get());
    L->SaveToFile(UnicodeString(robotpath+"AutoStopKeys.txt"), TEncoding::UTF8);

    createStandartDataAutoStopPosts(L.get());
    L->SaveToFile(UnicodeString(robotpath+"AutoStopPosts.txt"), TEncoding::UTF8);

    conf_models(false);
}
void c_main::conf_users(int index, bool save)
{
	std::auto_ptr<TStringList> L(new TStringList);
	String NeededGroupName = Trim(f->LV_CONF_GROUPS->Items->Item[index]->Caption);

	if ( ! save )
	{
		f->LV_CONF_USERS->Items->Clear();
        if(FileExists(f_users))
        {
		    L->LoadFromFile( f_users );
        }
        int i = 1;
		for ( int c = 0; c < L->Count; c++ )
		{
			String dataline = L->Strings[c];

			String UserGroup 	= g.Encrypt(1,5,"#",dataline);
			String UserID 		= g.Encrypt(2,5,"#",dataline);
			String Name 		= g.Encrypt(3,5,"#",dataline);
			String Surname 	= g.Encrypt(4,5,"#",dataline);
			String Date 		= g.Encrypt(5,5,"#",dataline);

			if ( UserGroup == NeededGroupName )
			{
				TListItem *ListItem;
				ListItem = f->LV_CONF_USERS->Items->Add();
				ListItem->Caption = " " + IntToStr(i);               
                i++;
				ListItem->SubItems->Add( " " + UserID );
				ListItem->SubItems->Add( " " + Name );
				ListItem->SubItems->Add( " " + Surname );
				ListItem->SubItems->Add( Date );
            }
        }
	}
}
void c_main::conf_dialogs(bool save)
{
	std::auto_ptr<TStringList> L(new TStringList);
	std::auto_ptr<TStringList> X(new TStringList);

	if ( ! save )
	{
		f->LV_DIALOGS->Items->Clear();
		g.GetFiles( p_dialogs, L.get() );

		for ( int c = 0; c < L->Count; c++ )
		{
			String id = L->Strings[c];
            if(FileExists(p_dialogs + id))
            {
			    X->LoadFromFile( p_dialogs + id );

			    String name      = X->Strings[1];
			    String surname 	 = X->Strings[2];

			    TListItem *ListItem;
			    ListItem = f->LV_DIALOGS->Items->Add();
			    ListItem->Caption = " " + id;
			    ListItem->SubItems->Add( " " + name );
			    ListItem->SubItems->Add( " " + surname );
			    ListItem->SubItems->Add( getCountOfMesages(X->Text) );
            }
		}

		f->LV_DIALOGS->Visible = true;
		f->LV_DIALOGS->Refresh();
		f->LV_DIALOGS->Repaint();
	}
}
void c_main::conf_models(bool save)
{
	if ( ! save )
	{
		int index = f->CB_MODELS_ROBOTS->ItemIndex;
		f->CB_MODELS_ROBOTS->Items->Clear();

        std::auto_ptr<TStringList> L(new TStringList);
    	g.GetFiles( p_robots, L.get() );
        for ( int c = 0; c < L->Count; c++ )
		{
			String GroupName, RobotName, Server_ID, Login, Password, Token, Activity, Online, LastOnline;  
            bool freeze;
			get_robotdata( c, &GroupName, &RobotName, &Server_ID, &Login, &Password, &Token, &Activity, &Online, &LastOnline, &freeze );
            f->CB_MODELS_ROBOTS->Items->Add( RobotName );
    	}

		if ( index == -1 || index >= f->CB_MODELS_ROBOTS->Items->Count ) 
        {
            index = 0;
        }

        if ( f->CB_MODELS_ROBOTS->Items->Count > 0 )
		{
			f->CB_MODELS_ROBOTS->ItemIndex = index;
			LoadModel(index);
		}
    }
}
void c_main::soundplay_captha()
{
	wchar_t *wc = f_soundcaptcha.c_str();
	char c[255];
	wcstombs(c, wc, wcslen(wc)+1);

	PlaySound( wc, 0, SND_FILENAME | SND_ASYNC );
}
void c_main::show_current_server()
{
    if(f->LV_SERVERS->Items->Count > 0)
    {
        f->l_NextClientID->Caption = "¹" + IntToStr(CurrentServer+1) + " - ID: " + f->LV_SERVERS->Items->Item[CurrentServer]->SubItems->Strings[0];
	    f->LV_SERVERS->ItemIndex = CurrentServer;
    }
}
void c_main::increment_server()
{
	CurrentServer++;
	if ( CurrentServer == f->LV_SERVERS->Items->Count ) 
    {
        CurrentServer = 0;
    }
	conf_ini( true );
	f->LV_SERVERS->ItemIndex = CurrentServer;
}
void c_main::GlobalUsersCache_Add(String id)
{
	GLOBAL_USERS_CACHE->Add( id );
	GLOBAL_USERS_CACHE->SaveToFile( f_globaluserscache );
    log(L"Добавлено в Global.users.cache [ "+id+" ]");
}
bool c_main::GlobalUsersCache_Exist(String id)
{
	bool ex = false;
	for ( int c = 0; c < GLOBAL_USERS_CACHE->Count; c++ )
	{
		String line = GLOBAL_USERS_CACHE->Strings[c];
		if ( line == id )
		{
			ex = true;
			break;
		}
	}
	return ex;
}
void c_main::GlobalUsersCache_Delete(String id)
{
	try
	{
		for ( int c = GLOBAL_USERS_CACHE->Count-1; c >= 0; c-- )
		{
			String line = GLOBAL_USERS_CACHE->Strings[c];
			if ( line == id )
			{
				GLOBAL_USERS_CACHE->Delete( c );
			}
		}

		GLOBAL_USERS_CACHE->SaveToFile( f_globaluserscache );
	}
	catch (...)
	{

    }
}
void c_main::GlobalUsersCache_DublicatesDelete()
{
	int acount = GLOBAL_USERS_CACHE->Count;
	std::auto_ptr<TStringList> L(new TStringList);
	for ( int c = 0; c < GLOBAL_USERS_CACHE->Count; c++ )
	{
		String a = GLOBAL_USERS_CACHE->Strings[c];
		if ( Pos(a,L->Text) == 0 ) 
        {
            L->Add(a);
        }
	}

	GLOBAL_USERS_CACHE->Text = L->Text;
	GLOBAL_USERS_CACHE->SaveToFile( f_globaluserscache );
    
	ShowMessage(L"Выполнено.\n\nДо очистки: "+IntToStr(acount)+L"\nПосле очистки: "+IntToStr(GLOBAL_USERS_CACHE->Count));
}
void c_main::log( String data )
{
	if ( PREFIX.Length() > 0 ) 
    {
        data = PREFIX + data;
    }
        
    log_transform();

    TDateTime T = Time();

    LOG->Add( UnicodeString(TimeToStr(T) + ": " + data) );
    LOG->SaveToFile( UnicodeString(f_currentlog), TEncoding::UTF8 );

    if ( f->CH_LOG->Checked )
    {
        f->ME_LOG->Lines->Add( TimeToStr(T) + ": " + data );
        Application->ProcessMessages();
    }
}

void c_main::checkDirectoryExisting(String directoryPath)
{
    if(DirectoryExists(directoryPath) == false)
    {
        if (CreateDir(directoryPath) == false ||
            ForceDirectories(directoryPath) == false)
        {
            ShowMessage(L"Не могу создать папку : " + directoryPath + L" Код ошибки: " + IntToStr((int)GetLastError()) );
        }
    }
}
void c_main::iSleep(int index, String token)
{
	String a;
    int x;

    f->online->addtobuffer(token);
    
	// 1 : Api Request, 2 : Add To Friends, 3 : Send Message, 4 : Confirm Friend

	if ( index == 1 ) { a = f->e_int_api->Text; x = StrToInt(f->e_int_api->Text); }
	if ( index == 2 ) { a = f->e_int_addfr->Text; x = StrToInt(f->e_int_addfr->Text); }
	if ( index == 3 ) { a = f->e_int_msg->Text; x = StrToInt(f->e_int_msg->Text); }
    if ( index == 4 ) { a = f->e_int_confirmfriend->Text; x = StrToInt(f->e_int_confirmfriend->Text); }

	if ( x > 0 )
	{
		if ( f->CH_LOG_PAUSES->Checked ) 
        {
            log( a+L" :: Пауза" );
        }

		g.ProcessMessages();

		for ( int c = 0; c < x; c++ )
		{
			Sleep(1);
			g.ProcessMessages();
        }
        
		g.ProcessMessages();
	}
}
void c_main::logline( String data )
{
	String line = ":::::::::::::::::::::::::::::::::::::::::::::::::::::::";

	if ( data == "" || data == "NULL" )
	{
		log(line);
	}
	else
	{
		log(line);
		log( data );
		log(line);
	}
}
void c_main::buffer_write( String data )
{
	bool ok = false;

	while ( ! ok )
	{
		try
		{
			Application->ProcessMessages();
			clp->AsText = data;
			ok = true;                   
            Application->ProcessMessages();
		}
		catch (...)
		{

		}
	}
}
void c_main::get_robotdata( int index, String *GroupName, String *RobotName, String *Server_ID, String *Login, String *Password, String *Token, String *Activity, String *Online, String *LastOnline, bool *freeze )
{
	std::auto_ptr<TStringList> L(new TStringList);
	g.GetFiles( p_robots, L.get() );
	String robotpath = p_robots + L->Strings[index] + "\\";

	std::auto_ptr<TMemIniFile> INI(new TMemIniFile( UnicodeString(robotpath + "Conf.ini"),TEncoding::UTF8 ));

    *freeze     = INI->ReadBool(   UnicodeString("MAIN"), 		UnicodeString("freeze"),    true );
	*GroupName 	= INI->ReadString( UnicodeString("MAIN"),       UnicodeString("owner"),     UnicodeString("0") );
	*RobotName  = INI->ReadString( UnicodeString("MAIN"),       UnicodeString("name"),  	UnicodeString("0") );
	*Login 		= INI->ReadString( UnicodeString("ACCOUNT"),    UnicodeString("login"),     UnicodeString("0") );
	*Password  	= INI->ReadString( UnicodeString("ACCOUNT"),    UnicodeString("password"),  UnicodeString("0") );
    *Online     = INI->ReadString( UnicodeString("ACCOUNT"),     UnicodeString("online"),   UnicodeString("0") );
	*LastOnline = INI->ReadString( UnicodeString("ACCOUNT"),     UnicodeString("lastonline"),UnicodeString("0") );
	*Activity 	= INI->ReadString( UnicodeString("CONNECTION"), UnicodeString("activity"),  UnicodeString("0") );
	*Server_ID 	= INI->ReadString( UnicodeString("CONNECTION"), UnicodeString("client_id"), UnicodeString("0") );
	*Token     	= INI->ReadString( UnicodeString("CONNECTION"), UnicodeString("token"),     UnicodeString("0") );
}
int  c_main::get_robotnext( String GroupName )
{
	int CNT = 0;

	std::auto_ptr<TStringList> L(new TStringList);
	g.GetFiles(p_robots,L.get());
	for ( int c = 0; c < L->Count; c++ )
	{
		String CurGroupName, RobotName, Server_ID, Login, Password, Token, Activity, Online, LastOnline;  
        bool freeze;
		get_robotdata( c, &CurGroupName, &RobotName, &Server_ID, &Login, &Password, &Token, &Activity, &Online, &LastOnline, &freeze );
		if ( CurGroupName == GroupName ) 
        {
            CNT++;
        }
	}

	if ( TGC >= CNT ) 
    {
        TGC = 0;
    }
    
	int RET = TGC;
	TGC++;

	return RET;
}
long int c_main::getUnix()
{
	return Dateutils::DateTimeToUnix(Now());
}
void c_main::search_request( String RequestUrl, String OffSet, String Count, int iteration )
{
	log( "ADD USERS BY VK SEARCH URL" );

	log("ITERATION: "+IntToStr(iteration));
	log("OFFSET:    "+OffSet);
	log("COUNT:     "+Count);

	String GroupName, RobotName, Server_ID, Login, Password, Token, Activity, Online, LastOnline; 
    bool freeze;
	std::auto_ptr<TStringList> L(new TStringList);
	g.GetFiles( p_robots, L.get() );
	get_robotdata( Random(L->Count), &GroupName, &RobotName, &Server_ID, &Login, &Password, &Token, &Activity, &Online, &LastOnline, &freeze );

	if ( f->proc->Establish(RobotName,&Token) )
	{
		String Group               = get_vkurl_param( "group",RequestUrl );
		String q                   = get_vkurl_param( "q",RequestUrl );
		String Country 			= get_vkurl_param( "country",RequestUrl );
		String City                = get_vkurl_param( "city",RequestUrl );
		String AgeFrom 			= get_vkurl_param( "age_from",RequestUrl );
		String AgeTo   			= get_vkurl_param( "age_to",RequestUrl );
		String Sex     			= get_vkurl_param( "sex",RequestUrl );
		String Status  			= get_vkurl_param( "status",RequestUrl );
		String PhotoEX 			= get_vkurl_param( "photo",RequestUrl );
		String OnlineEX 			= get_vkurl_param( "online",RequestUrl );
		String Religion    		= get_vkurl_param( "religion",RequestUrl );
		String PersonalPriority    = get_vkurl_param( "personal_priority",RequestUrl );
		String PeoplePriority  	= get_vkurl_param( "people_priority",RequestUrl );
		String Smoking  			= get_vkurl_param( "smoking",RequestUrl );
		String Alcogol  			= get_vkurl_param( "alcohol",RequestUrl );
		String Company  			= get_vkurl_param( "company",RequestUrl );
		String Position  			= get_vkurl_param( "position",RequestUrl );
		String MilitaryCountry  	= get_vkurl_param( "mil_country",RequestUrl );
		String birth_year  		= get_vkurl_param( "byear",RequestUrl );
		String birth_month  	    = get_vkurl_param( "bmonth",RequestUrl );
		String birth_day  		    = get_vkurl_param( "bday",RequestUrl );

		String request = "q="+q;
        request += "&offset="+OffSet;
        request += "&count="+Count;
        if(Country.IsEmpty() == false)
        {
           request += "&country="+Country;
        }
		if(City.IsEmpty() == false)
        {
            request += "&city="+City;
        }
        if(AgeFrom.IsEmpty() == false)
        {
            request += "&age_from="+AgeFrom;
        }
        if(AgeTo.IsEmpty() == false)
        {
            request += "&age_to="+AgeTo;
        }
        if(Sex.IsEmpty() == false)
        {
            request += "&sex="+Sex;
        }
        if(Status.IsEmpty() == false)
        {
            request += "&status="+Status;
        }
        if(PhotoEX.IsEmpty() == false)
        {
            request += "&has_photo="+PhotoEX;
        }
        if(OnlineEX.IsEmpty() == false)
        {
            request += "&online="+OnlineEX;
        }
        if(Religion.IsEmpty() == false)
        {
            request += "&religion="+Religion;
        }
        if(Company.IsEmpty() == false)
        {
            request += "&company="+Company;
        }
        if(Position.IsEmpty() == false)
        {
            request += "&position="+Position;
        }
        if(Group.IsEmpty() == false)
        {
            request += "&group_id="+Group;
        }
        if(birth_year.IsEmpty() == false)
        {
            request += "&birth_year="+birth_year;
        }
        if(birth_month.IsEmpty() == false)
        {
            request += "&birth_month="+birth_month;
        }
        if(birth_day.IsEmpty() == false)
        {
            request += "&birth_day="+birth_day;
        }

		String response;

        if ( f->ch_dynamic_userbdata->Checked == false )
		{
            bool success = false;
            while ( ! success )
            {
                response = vk.users_search_from_url(&success,request,Token);
                f->main->iSleep(1, Token);
                if ( ! success ) 
                {
                    log(L"Сервер не ответил на запрос. Повтор..");
                }
            }

            Application->ProcessMessages();
            response_read(response,L.get(),Count,OffSet,RequestUrl,iteration);
        }
        else
		{
			for ( int cur_M = 1; cur_M <= 12; cur_M++ )
			{
				for ( int cur_D = 1; cur_D <= 31; cur_D++ )
				{
                    log(L"Поисковый запрос : Месяц рождения [ "+IntToStr(cur_M)+L" ], Число рождения [ "+IntToStr(cur_D)+" ]");
                    request = include_actual_bdata(cur_M,cur_D,request);

                    bool success = false;
                    while ( ! success )
                    {
                        response = vk.users_search_from_url(&success,request,Token);
                        f->main->iSleep(1, Token);
                        if ( ! success ) 
                        {
                            log(L"Сервер не ответил на запрос. Повтор..");
                        }
                    }

                    Application->ProcessMessages();
                    response_read(response,L.get(),Count,OffSet,RequestUrl,iteration);
                }
            }
        }
	}
}
String  c_main::include_actual_bdata(int m, int d, String request)
{
	String fx = "&birth_month=";
	int pos = Pos(fx,request);
	String a = request.SubString(1,pos-1);
    return a + "&birth_month=" + IntToStr(m) + "&birth_day=" + IntToStr(d);
}
void c_main::response_read(String response, TStringList *L, String Count, String OffSet, String RequestUrl, int iteration)
{
    if ( Pos("items\":[]",response) == 0 )
    {
        if(FileExists(f_users))
        {
            L->LoadFromFile( f_users );
        }
        // JSON //////////////////////////////////////////////////////////////////////////////
        std::auto_ptr<TJSONObject> json (static_cast<TJSONObject*>(TJSONObject::ParseJSONValue(response)));
        TJSONObject *obj_response = static_cast<TJSONObject*>(json->Get("response")->JsonValue);
        String allcount = obj_response->GetValue("count")->ToString();

        TJSONArray *obj_items = static_cast<TJSONArray*>(obj_response->Get("items")->JsonValue);
        for ( int c = 0; c < obj_items->Count; c++ )
        {
            TJSONObject* x_obj_items = static_cast<TJSONObject*>(obj_items->Items[c]);

            String uid = "";
            TJSONValue* uidObj = x_obj_items->GetValue("id");
            if(uidObj != NULL)
                uid = uidObj->ToString();
                
            TJSONValue* unameObj = x_obj_items->GetValue("first_name");
            String uname = "";
            if(unameObj != NULL)
                uname = unameObj->ToString();
                
            TJSONValue* usurnameObj = x_obj_items->GetValue("last_name");
            String usurname = "";
            if(usurnameObj != NULL)
                usurname = usurnameObj->ToString();
                
            TJSONValue* canwritepmObj = x_obj_items->GetValue("can_write_private_message");
            String canwritepm = "";
            if(canwritepmObj != NULL)
                canwritepm = canwritepmObj->ToString();

            if ( f->LV_WORKTASKS->Items->Item[5]->Checked && 
                 canwritepm == "0" )
            {
                log(L"Запрет Друзей : Пользователь: [ "+uid+" ] [ "+uname+" "+usurname+L" ] не принимает сообщения." );
                GlobalUsersCache_Delete( uid );
                log(L"Запрет Друзей : Пользователь [ "+uid+L" ] удалён из Global.Users.Cache" );
                continue;
            }

            uname 	 = vk.jsonfix_removeQuotes(uname);
            usurname = vk.jsonfix_removeQuotes(usurname);

            if ( ! GlobalUsersCache_Exist( uid ) )
            {
                TDateTime D = Date();
                TDateTime T = Time();
                String DT = DateToStr(D) + " - " + TimeToStr(T);
                L->Add( Trim(f->LV_CONF_GROUPS->Items->Item[f->LV_CONF_GROUPS->ItemIndex]->Caption)+"#"+uid+"#"+uname+"#"+usurname+"#"+DT );

                GlobalUsersCache_Add( uid );
                log(L"В очередь добавлен пользователь: [ "+uid+" ] [ "+uname+" "+usurname+" ]" );
                countOfprocessed++;

                if ( countOfprocessed >= StrToInt(Count) ) 
                {
                    break;
                }
            }
            else
            {
                log( L"Пользователь [ " +uid+ L" ] уже в очереди или обрабатывался ранее. Global.Users.Cache." );
            }
        }

        // //////////////////////////////////////////////////////////////////////////////////

        L->SaveToFile( UnicodeString(f_users), TEncoding::UTF8 );
        conf_users(f->LV_CONF_GROUPS->ItemIndex,false);
        f->vcl->groupechoReadUsers();

        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        int newOffSet = StrToInt(OffSet)+StrToInt(Count);
        if ( countOfprocessed < StrToInt(Count) )
        {
            search_request(RequestUrl, IntToStr(newOffSet), Count, iteration+1);
        }
        else
        {
            f->e_conf_users_Count->Text = IntToStr( countOfprocessed );
            conf_ini(true);
        }
    }
    else 
    {
        log(L"VK API ВЕРНУЛ [ 0 ] РЕЗУЛЬТАТОВ ПРИ [ OFFSET:"+OffSet+", COUNT:"+Count+" ]");
    }
}
String  c_main::get_vkurl_param(String ParamName, String Data)
{
	String RETURN = "";

	ParamName = "["+ParamName+"]=";
	int pos = Pos(ParamName,Data);
	if ( pos > 0 )
	{
		Data = Data.SubString(pos+ParamName.Length(),Data.Length());
		pos = Pos("&",Data);
		if ( pos > 0 ) 
            Data = Data.SubString(1,pos-1);

		RETURN = Data;
	}

	return RETURN;
}
String  c_main::getCountOfMesages(String Dialog)
{
	std::auto_ptr<TStringList> L(new TStringList);
	L->Text = Dialog;

	int mess_count = 0;
	int new_count  = 0;

	for ( int c = 0; c < L->Count; c++ )
	{
		if ( L->Strings[c] == "#MESSAGE" ) 
            mess_count++;

		if ( L->Strings[c] == "#NEW=1" ) 
            new_count++;
	}

	String J;
	if ( new_count == 0 ) 
        J = IntToStr(mess_count);
	else                  
        J = IntToStr(mess_count) + " (" + IntToStr(new_count) + ")";

	return J;
}
int  c_main::getCountOfHello(String GroupName)
{
	int C = 0;

	std::auto_ptr<TStringList> L(new TStringList);
    if(FileExists(f_users))
    {
	    L->LoadFromFile( f_users );
    }
	for ( int x = 0; x < L->Count; x++ )
	{
		String CurGroup = Trim( g.Encrypt(1,5,"#",L->Strings[x]) );

		if ( CurGroup == GroupName ) 
            C++;
	}
	return C;
}
void c_main::DrawMessageBox(String Name, String Surname, String RobotGID, TStringList *DIALOGS)
{
	for ( int c = 0; c < DIALOGS->Count; c++ )
	{
		String dataline = DIALOGS->Strings[c];
		if ( dataline == "#MESSAGE" )
		{
            std::auto_ptr<TStringList> L(new TStringList);
			bool Out = true;
			if ( DIALOGS->Strings[c+1] == "#INC" ) 
                Out = false;
                
			String  DATE = DIALOGS->Strings[c+2].SubString(7,50);
			int START_C = c + 7;
			while ( true )
			{
				String S = DIALOGS->Strings[START_C];
				if ( S == "#END" ) 
                    break;
				L->Add(S);
				START_C++;
			}

			String Usr = L"РОБОТ [ "+RobotGID+" ] :";
			if ( ! Out ) 
                Usr = Name+" "+Surname+" :";

			f->vcl->Dialog_Add_User(Usr);
			f->vcl->Dialog_Add_Text(L.get());
			f->vcl->Dialog_Add_Date(DATE);
		}
	}
}
void c_main::DeleteUserFromQueueAndPutToDialogs(String UserID, String UserName, String UserSurname, String RobotName, bool msghello, String Text)
{
	std::auto_ptr<TStringList> L(new TStringList);
    if(FileExists(f_users))
    {
	    L->LoadFromFile( f_users );
    }
	for ( int c = L->Count-1; c >= 0; c-- )
	{
		String uid = g.Encrypt(2,5,"#",L->Strings[c]);
		if ( uid == UserID ) 
        { 
            L->Delete(c); 
            break; 
        }
	}
	L->SaveToFile( UnicodeString(f_users), TEncoding::UTF8 );

	f->PAGES_CONFIGURATION->ActivePageIndex = 0;
	f->LV_CONF_GROUPS->ItemIndex            = -1;

	TDateTime D = Date();
	TDateTime T = Time();
	String DT = DateToStr(D) + " - " + TimeToStr(T);
	std::auto_ptr<TStringList> X(new TStringList);
	X->Add(RobotName);
	X->Add(UserName);
	X->Add(UserSurname);
	X->Add(UserID);
	X->Add("");
	X->Add("#MESSAGE");
	X->Add("#OUT");
	X->Add("#DATE:"+DT);
	X->Add("#NEW=0");
	X->Add("#ID=HELLO");
	X->Add("#STAGEDATA=HELLO");
	X->Add("#BEGIN");                       
    if ( ! msghello ) 
        X->Add(L"Исходящая заявка в друзья:");
    else 
        X->Add(L"Начат диалог:");
	X->Add(Text);
	X->Add("#END");
	X->SaveToFile( UnicodeString(p_dialogs + UserID),TEncoding::UTF8 );
}
void c_main::SetAsRead(TStringList *DIALOG)
{
	for ( int c = 0; c < DIALOG->Count; c++ )
	{
		if ( DIALOG->Strings[c] == "#NEW=1" )
		{
			DIALOG->Strings[c] = "#NEW=0";
		}
	}
}
void c_main::WriteInboxList(String RobotName, String Token)
{
	bool ex = false;
	for ( int c = 0; c < INBOX->Count; c++ )
	{
		String _token = g.Encrypt(1,6,"#", INBOX->Strings[c] );
		if ( Token == _token )
		{
			ex = true;
			break;
        }
    }

	if ( !ex )
	{
		log(L"Получение входящих сообщений и заполнение буффера..");
		g.ProcessMessages();
        
        std::auto_ptr<TStringList> UIDS(new TStringList);
		f->main->GetDialogs( UIDS.get(), 0, 0, Token, RobotName );

        std::auto_ptr<TStringList> activeUserIds(new TStringList);
        std::auto_ptr<TStringList> DIALOGS(new TStringList);
        g.GetFiles( f->main->p_dialogs ,DIALOGS.get());
        for ( int c = 0; c < DIALOGS->Count; c++ )
        {
            String DialogFile = f->main->p_dialogs + DIALOGS->Strings[c];
            std::auto_ptr<TStringList> DIALOG(new TStringList);
            DIALOG->LoadFromFile( DialogFile );

            String ROBOTGID = DIALOG->Strings[0];
            String USERID   = DIALOG->Strings[3];

            if ( RobotName == ROBOTGID )
            {
                activeUserIds->Add( USERID );
            }
        }

		for ( int c = 0; c < UIDS->Count; c++ )
		{
			String UID = UIDS->Strings[c];

            if(activeUserIds->IndexOf(UID) >= 0 &&
               activeUserIds->IndexOf(UID) < activeUserIds->Count)
            {
                f->main->GetHistory( INBOX, UID, 0, 10, Token, RobotName );
            }
		}

		log(L"Список сообщений получен. Буффер наполнен. Общее количество: [ "+IntToStr(f->main->INBOX->Count)+" ]");
	}
	else
	{
		log(L"Список сообщений был получен ранее. Буффер наполнен.");
	}
}

void c_main::WriteAutoAnsOutList(String RobotName, String Token)
{
	if ( Pos(Token,f->main->AUTOANSOUTBOX->Text) == 0 )
	{
		log(L"Получение входящих сообщений и заполнение буффера..");
		g.ProcessMessages();
        
        std::auto_ptr<TStringList> UIDS(new TStringList);
		f->main->GetDialogs( UIDS.get(), 0, 0, Token, RobotName );

        std::auto_ptr<TStringList> activeUserIds(new TStringList);
        std::auto_ptr<TStringList> DIALOGS(new TStringList);
        g.GetFiles( f->main->p_dialogs ,DIALOGS.get());
        for ( int c = 0; c < DIALOGS->Count; c++ )
        {
            String DialogFile = f->main->p_dialogs + DIALOGS->Strings[c];
            std::auto_ptr<TStringList> DIALOG(new TStringList);
            DIALOG->LoadFromFile( DialogFile );

            String ROBOTGID = DIALOG->Strings[0];
            String USERID   = DIALOG->Strings[3];

            if ( RobotName == ROBOTGID )
            {
                activeUserIds->Add( USERID );
            }
        }

		for ( int c = 0; c < UIDS->Count; c++ )
		{
			String UID = UIDS->Strings[c];

            if(activeUserIds->IndexOf(UID) < 0 ||
               activeUserIds->IndexOf(UID) >= activeUserIds->Count)
            {
                f->main->GetHistory( AUTOANSOUTBOX, UID, 0, 10, Token, RobotName );
            }
		}

		log(L"Список сообщений получен. Буффер наполнен. Общее количество: [ "+IntToStr(f->main->INBOX->Count)+" ]");
	}
	else
	{
		log(L"Список сообщений был получен ранее. Буффер наполнен.");
	}
}

void c_main::GetDialogs(TStringList *UIDS, int OUT_3, int READSTATE_3, String Token, String RobotName)
{
	log(L"Получение диалогов..");
	g.ProcessMessages();

	int max_count_of = 0;
	int current_offset_of = 0;

	while ( true )
	{
		String response;
		bool success = false;
		while ( ! success )
		{
			response = vk.messages_getDialogs(&success,current_offset_of,200,Token);
			iSleep(1,Token);
			if ( ! success ) 
            {
                log(L"Сервер не ответил на запрос. Повтор..");
            }
		}

		if ( f->CH_APIRET->Checked ) 
        {
            f->main->log(L"Ответ сервера: ["+response+"]");
        }

		// JSON //////////////////////////////////////////////////////////////////////////////
		std::auto_ptr<TJSONObject> json (static_cast<TJSONObject*>(TJSONObject::ParseJSONValue(response)));
		TJSONObject *obj_response = static_cast<TJSONObject*>(json->Get("response")->JsonValue);
		String messcount = obj_response->GetValue("count")->ToString();
		TJSONArray *obj_items = static_cast<TJSONArray*>(obj_response->Get("items")->JsonValue);
		for ( int c = 0; c < obj_items->Count; c++ )
		{
            TJSONObject *obj_item = static_cast<TJSONObject*>(obj_items->Items[c]);
            if ((obj_item->GetValue("unread"))) {
                String unread 	      = obj_item->GetValue("unread")->ToString();
                if (unread != "1") {
                    continue;
                }
            }

            TJSONObject *obj_message = static_cast<TJSONObject*>(obj_item->Get("message")->JsonValue);
			String mid 	         = obj_message->GetValue("id")->ToString();
			String date 	     = obj_message->GetValue("date")->ToString();
			String out 	         = obj_message->GetValue("out")->ToString();
			String user_id 	     = obj_message->GetValue("user_id")->ToString();
			String read_state 	 = obj_message->GetValue("read_state")->ToString();
			String title 	     = obj_message->GetValue("title")->ToString();
			String body 	     = obj_message->GetValue("body")->ToString();

			bool go1 = false; // OUT_3
			if ( (OUT_3 == 0 && out == "0") ||
                 (OUT_3 == 1 && out == "1") ||  
                  OUT_3 == 2 ) 
            {
                go1 = true;
            }

			bool go2 = false; // READSTATE_3
			if ( (READSTATE_3 == 0 && read_state == "0") ||
                 (READSTATE_3 == 1 && read_state == "1") ||
                  READSTATE_3 == 2 ) 
            {
                go2 = true;
            }

			if ( go1 && go2 )
			{
				UIDS->Add( user_id );
			}
		}

		if ( max_count_of == 0 ) 
        {
            max_count_of = StrToInt(messcount);
        }

        if ( f->CH_APIRET->Checked ) 
        {
		    log(L"Смещение: [ "+IntToStr(current_offset_of)+L" ] Количество в стеке: [ "+obj_items->Count+" ]");
        }

		current_offset_of = current_offset_of + 200;

		if (  max_count_of == 0 || 
              max_count_of < current_offset_of ) 
        {
            break;
        }
	}

	log(L"Список диалогов получен.");
}
void c_main::GetHistory(TStringList *LIST, String UID, int OUT_3, int Count, String Token, String RobotName)
{
	log(L"Получение истории сообщений пользователя [ "+UID+L" ] .. Макс [ "+IntToStr( Count )+L" ] шт.");
	g.ProcessMessages();

	int max_count_of = 0;
	int current_offset_of = 0;

	int gotch_cnt = 0;
	bool breaked = false;

	while ( true )
	{
		String response;
		bool success = false;
		while ( ! success )
		{
			response = vk.messages_getHistory(&success,current_offset_of,200,UID,Token);
			iSleep(1,Token);
			if ( ! success ) 
            {
                log(L"Сервер не ответил на запрос. Повтор..");
            }
		}

		if ( f->CH_APIRET->Checked ) 
        {
            f->main->log(L"Ответ сервера: ["+response+"]");
        }

		// JSON //////////////////////////////////////////////////////////////////////////////
		std::auto_ptr<TJSONObject> json (static_cast<TJSONObject*>(TJSONObject::ParseJSONValue(response)));
		TJSONObject *obj_response = static_cast<TJSONObject*>(json->Get("response")->JsonValue);
		String messcount = obj_response->GetValue("count")->ToString();
		TJSONArray *obj_items = static_cast<TJSONArray*>(obj_response->Get("items")->JsonValue);

		for ( int c = 0; c < obj_items->Count; c++ )
		{
			TJSONObject *obj_root = static_cast<TJSONObject*>(obj_items->Items[c]);
			String mid 		    = obj_root->GetValue("id")->ToString();
			String body  		= obj_root->GetValue("body")->ToString();
			String user_id 	    = obj_root->GetValue("user_id")->ToString();
			String from_id 	    = obj_root->GetValue("from_id")->ToString();
			String date 		= obj_root->GetValue("date")->ToString();
			String read_state 	= obj_root->GetValue("read_state")->ToString();
			String out 		    = obj_root->GetValue("out")->ToString();
            String title 		= "NULL";

			bool go = false; // OUT_3
			if ( (OUT_3 == 0 && out == "0") ||
                 (OUT_3 == 1 && out == "1") ||
                  OUT_3 == 2 ) 
            {
                LIST->Add( Token+"#"+mid+"#"+from_id+"#"+read_state+"#"+title+"#"+body );
                
                gotch_cnt++;
				if ( gotch_cnt >= Count ) 
                { 
                    breaked = true; 
                    break; 
                }
            }
		}
		if ( breaked ) 
        {
            break;
        }
		if ( max_count_of == 0 ) 
        {
            max_count_of = StrToInt(messcount);
        }

        if ( f->CH_APIRET->Checked ) 
        {
		    log(L"Смещение: [ "+IntToStr(current_offset_of)+L" ] Количество в стеке: [ "+obj_items->Count+" ]");
        }

		current_offset_of = current_offset_of + 200;

		if ( max_count_of == 0 || 
             max_count_of < current_offset_of ) 
        {
            break;
        }
	}

	log(L"Получено от [ "+UID+L" ] сообщений [ "+IntToStr( gotch_cnt )+L" ]");
}
String  c_main::GetLastStageName(TStringList *DIALOG)
{
	String J;
	for ( int c = 0; c < DIALOG->Count; c++ )
	{
		String l = DIALOG->Strings[c];
		if ( Pos("#STAGEDATA=",l) != 0 && Pos("#STAGEDATA=INC",l) == 0 )
		{
			J = l.SubString(12,l.Length());
        }
	}
	return J;
}
void c_main::GetOnlyOneStage(TStringList *MODEL, String StageName)
{
	std::auto_ptr<TStringList> A(new TStringList);

	bool ex = false;
	bool wr = false;

	for ( int c = 0; c < MODEL->Count; c++ )
	{
		String l = Trim( MODEL->Strings[c] );
		if (   wr && Pos("#",l)    != 0 ) 
            break;

		String ch = StageName[1];
		if ( ch == "#" )
		{
			if ( ! wr && StageName == l ) 
            { 
                wr = true; 
                ex = true; 
            }
		}
		else
		{
			if ( ! wr && "#"+StageName == l ) 
            { 
                wr = true; 
                ex = true; 
            }
        }

		if (   wr ) 
            A->Add(l);
	}

	if ( ex )
    { 
        MODEL->Text = A->Text;
    }
	else
	{
		f->main->log("FATAL ERROR: STAGE [ "+StageName+" ] NOT FINDED!");
		MODEL->Text = "NULL";
    }
}
int  c_main::GetMaxLevelsOfStages()
{
    int max = 1;
    for ( int c = 0; c < MODEL_LOGICAL->Count; c++ )
    {
        String l = MODEL_LOGICAL->Strings[c];
        if ( Pos("#",l) != 0 )
        {
            int x = 0;
            for ( int i = 1; i <= l.Length(); i++ )
            {
                String ch = l[i];
                if ( ch == "." ) 
                    x++;
            }
            if ( x > 0 && max < x + 1 ) 
                max = x + 1;
        }
    }
    return max;
}
void c_main::GetListOfLevel(int level,TStringList *L)
{
    for ( int c = 0; c < MODEL_LOGICAL->Count; c++ )
    {
    	int max = 1;
        String l = MODEL_LOGICAL->Strings[c];
        if ( Pos("#",l) != 0 )
        {
            int x = 0;
            for ( int i = 1; i <= l.Length(); i++ )
            {
                String ch = l[i];
                if ( ch == "." ) 
                    x++;
            }

            if ( x > 0 ) 
                max = x + 1;

            if ( max == level ) 
                L->Add(l);
        }
        max = 1;
    }
}
String  c_main::CutLastLevel(String data)
{
    int p;
    for ( int c = 1; c <= data.Length(); c++ )
    {
        String ch = data[c];
        if ( ch == "." ) 
            p = c;
    }

    String j = data.SubString(1,p-1);
    return j;
}
String  c_main::ConvertToVars(String extline)
{
    String j = "";

    std::auto_ptr<TStringList> L(new TStringList);
    L->Text = g.EncryptToList(",",extline);
    for ( int c = 0; c < L->Count; c++ )
    {
        String fx = VARIANTS->Strings[ g.Sti(L->Strings[c])-1 ];
        fx = fx.SubString(9,fx.Length());
        int p = Pos("'",fx);
        fx = fx.SubString(1,p-1);

        j = j + fx + ",";
    }

    j = j.SubString(1,j.Length()-1);
    return j;
}
String  c_main::GetVarFrom(int index)
{
    String fx = VARIANTS->Strings[index];
    fx = fx.SubString(9,fx.Length());
    int p = Pos("'",fx);
    fx = fx.SubString(1,p-1);
    return fx;
}
String  c_main::CreateStageName(String data)
{
    String j;
    if ( IFCREATE )
    {
        if ( IFROOT )
        {
            String  chx = data[1];
            if ( chx == "#" ) 
                j = data;
            else              
                j = "#" + data;
        }
        else
        {
            String  chx = data[1];
            if ( chx == "#" ) 
                data = data.SubString(2,data.Length());
            j = MYPARENT + "." + data;
        }
    }
    else
    {
        j = data;
    }
    return j;
}
bool c_main::ifStageNameExist(String StageName)
{
    bool ex = false;
    if ( IFCREATE )
    {
        for ( int c = 0; c < MODEL_LOGICAL->Count; c++ )
        {
            String l = MODEL_LOGICAL->Strings[c];
            if ( l == StageName )
            {
                ex = true;
                break;
            }
        }
    }
    return ex;
}
String  c_main::getLastStage(String data)
{
    String j = "NULL";

    std::auto_ptr<TStringList> L(new TStringList);
    L->Text = data;

    for ( int c = 0; c < L->Count; c++ )
    {
        String line = L->Strings[c];
        if ( Pos("#STAGEDATA=",line) != 0 )
        {
            j = line.SubString(12,line.Length());
        }
    }
    return j;
}
void c_main::log_transform()
{
	std::auto_ptr<TMemoryStream> ms(new TMemoryStream());
	LOG->SaveToStream(ms.get());
	ms->Position = 0;

	if ( ms->Size > 1024000 )
	{
		if ( BUFF_CURRENTLOG == "NULL" || BUFF_CURRENTLOG.Length() == 0 ) 
            generatelogname();

		LOG->SaveToFile( p_logs + BUFF_CURRENTLOG + ".log" );

		LOG->Clear();
        f->ME_LOG->Lines->Clear();

		generatelogname();
	}
}
void c_main::generatelogname()
{
	TDateTime D = Date();
	TDateTime T = Time();

	String SD = DateToStr(D);
	String ST = TimeToStr(T);

	for ( int c = 1; c <= ST.Length(); c++ )
	{
		String ch = ST[c];
		if ( ch == ":" ) 
            ST[c] = '.';
    }

	String d = g.Encrypt(1,3,".",SD);
	String m = g.Encrypt(2,3,".",SD);
	String y = g.Encrypt(3,3,".",SD);

	String j = y+"."+m+"."+d+"_"+ST;

	BUFF_CURRENTLOG = j;
    conf_ini(true);
}
void c_main::createStandartDataHello(TStringList *L)
{
	L->Clear();
	L->Add(UnicodeString(L"Привет!"));
	L->Add(UnicodeString(L"Приветик!"));
	L->Add(UnicodeString(L"Здравствуй!"));
	L->Add(UnicodeString(L"Здравствуйте!"));
	L->Add(UnicodeString(L"Доброго времени суток!"));
}
void c_main::createGlobalDataModel(TStringList *L)
{
	L->Text = UnicodeString(f->me_createGlobalDataModel->Lines->Text);
}
void c_main::createStandartDataModel(TStringList *L)
{
	L->Text = UnicodeString(f->me_createStandartDataModel->Lines->Text);
}
void c_main::createStandartDataAutoAnsRules(TStringList *L)
{
	L->Clear();
	L->Add(UnicodeString(L"Вы сдаёте квартиры?~#1.1"));
	L->Add(UnicodeString(L"Вы сдаёте квартиру?~#1.1"));
	L->Add(UnicodeString(L"Можно снять~#1.2"));
	L->Add(UnicodeString(L"Сколько стоит~#1.2"));
	L->Add(UnicodeString(L"Цена~#1.2"));
}
void c_main::createStandartDataAutoAnsDefault(TStringList *L)
{
	L->Clear();
	L->Add(UnicodeString("#1"));
}
void c_main::createStandartDataAutoStopKeys(TStringList *L)
{
	L->Clear();
	L->Add(UnicodeString(L"Отстань"));
	L->Add(UnicodeString(L"Отвянь"));
	L->Add(UnicodeString(L"Отвали"));
	L->Add(UnicodeString(L"Не пиши больше"));
	L->Add(UnicodeString(L"Не пиши мне больше"));
	L->Add(UnicodeString(L"Заебал"));
	L->Add(UnicodeString(L"Иди нах"));
	L->Add(UnicodeString(L"Иди на х"));
}
void c_main::createStandartDataAutoStopPosts(TStringList *L)
{
	L->Clear();
	L->Add(UnicodeString(L"Извини ("));
	L->Add(UnicodeString(L"Больше не потревожу."));
	L->Add(UnicodeString(L"Ок. Пока."));
	L->Add(UnicodeString(L"Ладно, пока!"));
}
void c_main::checkLinkedStages()
{
	std::auto_ptr<TStringList> J(new TStringList);

	String CurrSTAGE;

	// CHECK AUTOANS VARIANTS

	for ( int c = 0; c < f->LV_MODEL_AUTOANS->Items->Count; c++ )
	{
		String tofind = f->LV_MODEL_AUTOANS->Items->Item[c]->SubItems->Strings[0];

		bool ex = false;
		for ( int x = 0; x < MODEL_LOGICAL->Count; x++ )
		{
			String linex = MODEL_LOGICAL->Strings[x];
			if ( tofind == linex )
			{
				ex = true;
				break;
			}
		}

		if ( ! ex )
		{
			J->Add( L"Автоответчик: [Пункт №"+IntToStr(c+1)+L"] ссылается на [STAGE "+tofind+L"], которого не существует." );
		}

	}

	// CHECK AUTOANS DEFAULT

	String tofind = Trim(f->E_MODEL_AUTOANS_DEFAULT->Text);
	if ( tofind.Length() == 0 ) 
        tofind = "NULL";

	bool ex = false;
	for ( int x = 0; x < MODEL_LOGICAL->Count; x++ )
	{
		String linex = MODEL_LOGICAL->Strings[x];

		if ( tofind == linex )
		{
			ex = true;
			break;
		}
	}

	if ( ! ex )
	{
		J->Add( L"Автоответчик: [DEFAULT] ссылается на [STAGE "+tofind+L"], которого не существует." );
	}

	// CHECK MODEL

	for ( int c = 0; c < MODEL_LOGICAL->Count; c++ )
	{
		String line = MODEL_LOGICAL->Strings[c];
		if ( Pos("#",line) != 0 ) 
            CurrSTAGE = line;

		if ( Pos("IF'",line) != 0 || Pos("EXTENDED'",line) != 0 || Pos("DEFAULT'",line) != 0 )
		{
			String tofind;

			if ( Pos("DEFAULT'",line) == 0 ) 
                tofind = g.Encrypt(3,3,"'",line);
			else                             
                tofind = g.Encrypt(2,2,"'",line);

			bool ex = false;
			for ( int x = 0; x < MODEL_LOGICAL->Count; x++ )
			{
				String linex = MODEL_LOGICAL->Strings[x];
				if ( "#"+tofind == linex )
				{
					ex = true;
					break;
				}
			}

			if ( ! ex )
			{
				J->Add( L"Модель: [STAGE "+CurrSTAGE+L"] ссылается на [STAGE #"+tofind+L"], которого не существует." );
			}
		}
	}

	if ( J->Count > 0 )
	{
		ShowMessage( J->Text );
	}
}
void c_main::make_new_inbox(TStringList *ALL, TStringList *DEST)
{
	for ( int c = 0; c < ALL->Count; c++ )
	{
		String aline = ALL->Strings[c];
		String uid = g.Encrypt(3,6,"#",aline);

		bool ex = false;
		for ( int x = 0; x < DEST->Count; x++ )
		{
			String xline = DEST->Strings[x];
			if ( uid == xline )
			{
				ex = true;
				break;
			}
		}

        if ( ! ex ) 
            DEST->Add(uid);
    }
}
void c_main::deleteDialogsPerRobot(String robotname)
{
    std::auto_ptr<TStringList> DIALOGS(new TStringList);
	g.GetFiles( p_dialogs ,DIALOGS.get());

	for ( int c = 0; c < DIALOGS->Count; c++ )
	{
		String DialogFile = p_dialogs + DIALOGS->Strings[c];
        std::auto_ptr<TStringList> DIALOG(new TStringList);
		DIALOG->LoadFromFile( DialogFile );
		String ROBOTGID = DIALOG->Strings[0];

		if ( ROBOTGID == robotname )
		{
			DeleteFile( DialogFile.c_str() );
        }
	}
	conf_dialogs(false);
}
String  c_main::from_list_to_str(TStringList *L)
{
	String a = "";
	for ( int c = 0; c < L->Count; c++ )
	{
		a = a + L->Strings[c] + "~";
	}

	return a;
}
int  c_main::getrobotindex(String robotname, TStringList *L)
{
	int j = -1;
	for ( int c = L->Count-1; c >= 0; c-- )
	{
		String data = L->Strings[c];
		if ( robotname == g.Encrypt(1,2,"|",data) )
		{
			j = c;
			break;
		}
	}

	return j;
}
bool c_main::if_imbanned(String Token, String uid)
{
	bool imbanned;
	bool success = false;
	while ( ! success )
	{
		imbanned = vk.robot_in_ban(&success,uid,Token);
		f->main->iSleep(1, Token);
		if ( ! success ) 
        {
            log(L"Сервер не ответил на запрос. Повтор..");
        }
	}
	return imbanned;
}
void c_main::sended_count_add(String robotname)
{
	int cnt = 0;
	for ( int c = 0; c < AUTOANS_LIMIT->Count; c++ )
	{
		String line = AUTOANS_LIMIT->Strings[c];
		String cr = g.Encrypt(1,2,"#",line);
		String cc = g.Encrypt(2,2,"#",line);

		if ( robotname == cr )
		{
			AUTOANS_LIMIT->Delete(c);
			cnt = StrToInt(cc);
			break;
		}
	}

	cnt++;
	AUTOANS_LIMIT->Add( robotname+"#"+IntToStr(cnt) );
}
bool c_main::sended_count_iflimit(String robotname)
{
	int cnt = 0;
	for ( int c = 0; c < AUTOANS_LIMIT->Count; c++ )
	{
		String line = AUTOANS_LIMIT->Strings[c];
		String cr = g.Encrypt(1,2,"#",line);
		String cc = g.Encrypt(2,2,"#",line);

		if ( robotname == cr )
		{
			cnt = StrToInt(cc);
			break;
		}
	}

	bool j = false;

	if ( f->e_autoanswerlimit->Text != "0" && 
         cnt >= StrToInt(f->e_autoanswerlimit->Text) )
	{
		j = true;

		String me = f->ME_LOG->Lines->Strings[ f->ME_LOG->Lines->Count-1 ];
		if ( Pos(L"Достигнут лимит отправки сообщений [",me) == 0 ) 
        {
            f->main->log(L"Достигнут лимит отправки сообщений [ "+f->e_autoanswerlimit->Text+" ]");
        }
	}

    return j;
}
void c_main::LoadModel(int index)
{
	std::auto_ptr<TStringList> L(new TStringList);
	g.GetFiles( p_robots, L.get() );

    String file_a = p_robots + L->Strings[index] + "\\Hello.txt";
	String file_b = p_robots + L->Strings[index] + "\\Model.txt";
	String file_c = p_robots + L->Strings[index] + "\\AutoAnsRules.txt";
	String file_d = p_robots + L->Strings[index] + "\\AutoAnsDefault.txt";
	String file_e = p_robots + L->Strings[index] + "\\AutoStopKeys.txt";
	String file_f = p_robots + L->Strings[index] + "\\AutoStopPosts.txt";
    String file_g = p_robots + L->Strings[index] + "\\Global.txt";

    if(FileExists(file_a))
    {
	    MODEL_HELLO->LoadFromFile(file_a);
    }
    if(FileExists(file_b))
    {
	    MODEL_LOGICAL->LoadFromFile(file_b);        
    }
    FMODEL = file_b;
    if(FileExists(file_c))
    {
	    MODEL_AUTOANS->LoadFromFile(file_c);        
    }
    FAUTOANS = file_c;
    if(FileExists(file_e))
    {
	    MODEL_AUTOSTOP_KEYS->LoadFromFile(file_e);
    }
    if(FileExists(file_f))
    {
	    MODEL_AUTOSTOP_POSTS->LoadFromFile(file_f);
    }
    if(FileExists(file_g))
    {
	    MODEL_GLOBAL->LoadFromFile(file_g);
    }

    LoadModelHello();
	LoadModelLogical();
	LoadModelAutoAnsRules();
    if(FileExists(file_d))
    {
	    LoadModelAutoAnsDefault(file_d);
    }
	LoadModelAutoStopKeys();
	LoadModelAutoStopPosts();
    LoadModelGlobal();
}
void c_main::LoadModelHello()
{
	f->LV_MODEL_HELLO->Items->Clear();
	for ( int c = 0; c < MODEL_HELLO->Count; c++ )
	{
    	TListItem *ListItem;
        ListItem = f->LV_MODEL_HELLO->Items->Add();
		ListItem->Caption = "  "+IntToStr( c+1 );
		ListItem->SubItems->Add( MODEL_HELLO->Strings[c] );
	}
}
void c_main::LoadModelLogical()
{
    f->TREE->Items->Clear();

    int level = GetMaxLevelsOfStages();
    for ( int c = 1; c <= level; c++ ) 
    {
        LoadModelLogicalTree(c);
    }
}
void c_main::LoadModelLogicalTree(int level)
{
	std::auto_ptr<TStringList> L(new TStringList);
    GetListOfLevel(level,L.get());

    if ( level == 1 )
    {
        for ( int c = 0; c < L->Count; c++ )
        {
            f->TREE->Items->Add(0,L->Strings[c]);
        }
    }
    else
    {
        while ( L->Count > 0 )
        {
        	String fx = L->Strings[0];
            for ( int i = 0; i < f->TREE->Items->Count; i++ )
            {
            	String l = f->TREE->Items->Item[i]->Text;
            	if ( CutLastLevel(fx) == l )
                {
                	f->TREE->Items->AddChild( f->TREE->Items->Item[i] ,fx);
                	break;
                }
            }

            L->Delete(0);
        }
    }
}
void c_main::LoadModelGlobal()
{
	// UI FOR GLOBAL MODEL    L   MODEL_GLOBAL
}
void c_main::LoadModelAutoAnsRules()
{
	f->LV_MODEL_AUTOANS->Items->Clear();

	for ( int c = 0; c < MODEL_AUTOANS->Count; c++ )
	{
		String dataline = MODEL_AUTOANS->Strings[c];

		TListItem *ListItem;
		ListItem = f->LV_MODEL_AUTOANS->Items->Add();
		ListItem->Caption = "  "+ g.Encrypt(1,2,"~",dataline);
		ListItem->SubItems->Add( g.Encrypt(2,2,"~",dataline) );
	}
}
void c_main::LoadModelAutoAnsDefault(String file)
{
	std::auto_ptr<TStringList> L(new TStringList);

    if(FileExists(file))
    {
	    L->LoadFromFile( file );
    }

	f->E_MODEL_AUTOANS_DEFAULT->Text = Trim( L->Strings[0] );
}
void c_main::LoadModelAutoStopKeys()
{
	f->LV_MODEL_AUTOSTOP_KEYS->Items->Clear();
	for ( int c = 0; c < MODEL_AUTOSTOP_KEYS->Count; c++ )
	{
		String dataline = MODEL_AUTOSTOP_KEYS->Strings[c];

		TListItem *ListItem;
		ListItem = f->LV_MODEL_AUTOSTOP_KEYS->Items->Add();
		ListItem->Caption = "  "+ dataline;
	}
}
void c_main::LoadModelAutoStopPosts()
{
	f->LV_MODEL_AUTOSTOP_POSTS->Items->Clear();
	for ( int c = 0; c < MODEL_AUTOSTOP_POSTS->Count; c++ )
	{
		String dataline = MODEL_AUTOSTOP_POSTS->Strings[c];

		TListItem *ListItem;
		ListItem = f->LV_MODEL_AUTOSTOP_POSTS->Items->Add();
		ListItem->Caption = "  "+ dataline;
	}
}

void c_main::LoadModelStage(String StageName)
{
    f->vcl->ModelStageClear();
    f->E_MODEL_LOGICAL_STAGE->Text = StageName;
	std::auto_ptr<TStringList> A(new TStringList);

    bool REC = false;
    for ( int c = 0; c < MODEL_LOGICAL->Count; c++ )
    {
        String l = MODEL_LOGICAL->Strings[c];
        if ( REC && Pos("#",l) != 0 ) 
            break;
        if ( REC ) 
            A->Add(l);
        if ( StageName == l ) 
            REC = true;
    }

    for ( int c = 0; c < A->Count; c++ )
    {
        String l = A->Strings[c];

        if ( Pos("POST'",l) != 0 ) 
            POSTS->Add( l );
        if ( Pos("VARIANT'",l) != 0 ) 
            VARIANTS->Add( l );
        if ( Pos("IF'",l) != 0 ) 
            IFVARIANTS->Add( l );
        if ( Pos("EXTENDED'",l) != 0 ) 
            EXTENDED->Add( l );
        if ( Pos("DEFAULT'",l) != 0 ) 
            DEFAULT->Add( l );
    }

    LoadModelStagePosts();
    LoadModelStageVariants();
    LoadModelStageIfVariants();
    LoadModelStageExtendeds();
    LoadModelStageDefault();
}
void c_main::LoadModelStagePosts()
{
    for ( int c = 0; c < POSTS->Count; c++ )
    {
        String data = POSTS->Strings[c];
        f->LI_MODEL_LOGICAL_POSTS->Items->Add( data.SubString(6,data.Length()) );
    }
}
void c_main::LoadModelStageVariants()
{
    for ( int c = 0; c < VARIANTS->Count; c++ )
    {
        String data = VARIANTS->Strings[c];
        data = data.SubString(9,data.Length());
        int p = Pos("'",data);
        data = data.SubString(p+1,data.Length());

        f->LI_MODEL_LOGICAL_VARIANTS->Items->Add( data );
    }
}
void c_main::LoadModelStageIfVariants()
{
    f->vcl->GetAllStages( f->CB_MODEL_LOGICAL_IFVARIANTS );
}
void c_main::LoadModelStageExtendeds()
{
    for ( int c = 0; c < EXTENDED->Count; c++ )
    {
        String xext = EXTENDED->Strings[c].SubString(10,EXTENDED->Strings[c].Length());
		int xextpos = Pos("'",xext);
		xext = xext.SubString(1,xextpos-1);

		String j = "";
		for ( int x = 0; x < VARIANTS->Count; x++ )
        {
			String data = VARIANTS->Strings[x];
			data = data.SubString(9,data.Length());
			int p = Pos("'",data);
			data = data.SubString(1,p-1);

			if ( Pos(data,xext) != 0 ) 
            {
                j = j + g.Its(x+1) + ",";
            }
        }

		j = j.SubString(1,j.Length()-1);
		f->LI_MODEL_LOGICAL_EXTENDED->Items->Add( j );
    }

    f->vcl->GetAllStages( f->CB_MODEL_LOGICAL_EXTENDED );
}
void c_main::LoadModelStageDefault()
{
    f->vcl->GetAllStages( f->CB_MODEL_LOGICAL_DEFAULT );

    String fx;
    if(DEFAULT->Count > 0)
    {
        fx = DEFAULT->Strings[0];
        fx = fx.SubString(9,fx.Length());
    }

    for ( int c = 0; c < f->CB_MODEL_LOGICAL_DEFAULT->Items->Count; c++ )
    {
        String data = f->CB_MODEL_LOGICAL_DEFAULT->Items->Strings[c];
        if ( data == "#"+fx )
        {
            f->CB_MODEL_LOGICAL_DEFAULT->ItemIndex = c;
            break;
        }
    }
}

void c_main::ModelLogicalAddStage(TStringList *STAGE)
{
    for ( int c = 0; c < STAGE->Count; c++ )
    {
        MODEL_LOGICAL->Add( STAGE->Strings[c] );
    }
    MODEL_LOGICAL->Add("");
}
void c_main::ModelLogicalChangeStage(TStringList *STAGE)
{
	std::auto_ptr<TStringList> A(new TStringList);
	std::auto_ptr<TStringList> T(new TStringList);

    int index;

	for ( int c = 0; c < MODEL_LOGICAL->Count; c++ )
    {
		String dataline = MODEL_LOGICAL->Strings[c];

        if ( dataline == MYPARENT ) 
            index = c;

        if ( Pos("#",dataline) != 0 && dataline != MYPARENT )
        {
			T->Text = MODEL_LOGICAL->Text;
            GetOnlyOneStage(T.get(),dataline.SubString(2,dataline.Length()));

            for ( int i = 0; i < T->Count; i++ )
            {
            	A->Add( T->Strings[i] );
            }
		}
    }

    MODEL_LOGICAL->Text = A->Text;
    for ( int c = 0; c < STAGE->Count; c++ )
    {
        MODEL_LOGICAL->Insert( index, STAGE->Strings[c] );
        index++;
	}
}
void c_main::ModelLogicalDeleteStage(String StageName)
{
	std::auto_ptr<TStringList> DESTMODEL(new TStringList);
	DESTMODEL->Text = MODEL_LOGICAL->Text;

	for ( int c = 0; c < MODEL_LOGICAL->Count; c++ )
    {
		String dataline = MODEL_LOGICAL->Strings[c];
		if ( Pos(StageName,dataline) != 0  )
		{
			ModelLogicalDeleteStageDo(dataline,DESTMODEL.get());
        }
    }

	MODEL_LOGICAL->Text = DESTMODEL->Text;
	MODEL_LOGICAL->SaveToFile( UnicodeString(FMODEL), TEncoding::UTF8 );
	f->vcl->ModelsSET(false);
	f->vcl->ModelStageClear();

	conf_models( false );
}
void c_main::ModelLogicalDeleteStageDo(String StageName, TStringList *MODEL)
{
	std::auto_ptr<TStringList> T(new TStringList);

	bool start_fi = false;
	int stophere = 0;

	for ( int c = 0; c < MODEL->Count; c++ )
	{
		String line = MODEL->Strings[c];

		if ( StageName == line )
		{
			start_fi = true;
			continue;
		}

		if ( start_fi )
		{
			if ( Pos("#",line) != 0 ) 
                stophere++;
		}

		if ( start_fi == false ) 
            T->Add(line);

		if ( stophere > 0 )      
            T->Add(line);
	}

	ModelLogicalDeleteStageDeleteLinked(StageName,T.get());
	MODEL->Text = T->Text;
}
void c_main::ModelLogicalDeleteStageDeleteLinked(String StageName, TStringList *MODEL)
{
	for ( int c = 0; c < MODEL->Count; c++ )
	{
		String line = MODEL->Strings[c];
		String myStage = StageName.SubString(2,StageName.Length());
		if ( Pos("'"+myStage,line) > 0 )
		{
			if ( Pos("DEFAULT'",line) == 0 )
			{
				if ( Pos("IF'",line) != 0 || Pos("EXTENDED'",line) != 0 )
				{
					String a = g.Encrypt(1,3,"'",line);
					String b = g.Encrypt(2,3,"'",line);

					MODEL->Strings[c] = a+"'"+b+"'NULL";
				}
			}
			else
			{
				MODEL->Strings[c] = "DEFAULT'NULL";
            }
		}
	}
}
void c_main::ModelSaveAndReloadInterface()
{
	MODEL_LOGICAL->SaveToFile( UnicodeString(FMODEL), TEncoding::UTF8 );
	f->vcl->ModelsSET(false);
	f->vcl->ModelStageClear();
	conf_models( false );
}
void c_main::Robot_Freeze(int index)
{
	String iGroup = Trim( f->LV_CONF_GROUPS->Items->Item[f->LV_CONF_GROUPS->ItemIndex]->Caption );
	String iName  = Trim( f->LV_CONF_ROBOTS->Items->Item[f->LV_CONF_ROBOTS->ItemIndex]->Caption );

	std::auto_ptr<TStringList> L(new TStringList);
	g.GetFiles( p_robots, L.get() );

	bool breaked = false;
	for ( int c = 0; c < L->Count; c++ )
	{
		String robotpath = p_robots + L->Strings[c] + "\\" + "Conf.ini";
		std::auto_ptr<TMemIniFile> INI(new TMemIniFile( UnicodeString(robotpath), TEncoding::UTF8 ));
		String xGroup = INI->ReadString( UnicodeString("MAIN"), UnicodeString("owner"), UnicodeString("") );
		String xName  = INI->ReadString( UnicodeString("MAIN"), UnicodeString("name"), UnicodeString("") );

		if ( iGroup == xGroup && iName == xName )
		{
			INI->WriteBool( UnicodeString("MAIN"), UnicodeString("freeze"), true );
            INI->UpdateFile();
			breaked = true;
		}

		if ( breaked ) 
        {
            break;
        }
	}
    
	conf_robots( f->LV_CONF_GROUPS->ItemIndex, false);
	f->vcl->groupechoReadRobots();
}
void c_main::Robot_UnFreeze(int index)
{
	String iGroup = Trim( f->LV_CONF_GROUPS->Items->Item[f->LV_CONF_GROUPS->ItemIndex]->Caption );
	String iName  = Trim( f->LV_CONF_ROBOTS->Items->Item[f->LV_CONF_ROBOTS->ItemIndex]->Caption );

	std::auto_ptr<TStringList> L(new TStringList);
	g.GetFiles( p_robots, L.get() );

	bool breaked = false;
	for ( int c = 0; c < L->Count; c++ )
	{
		String robotpath = p_robots + L->Strings[c] + "\\" + "Conf.ini";
		std::auto_ptr<TMemIniFile> INI(new TMemIniFile( UnicodeString(robotpath), TEncoding::UTF8 ));
		String xGroup = INI->ReadString( UnicodeString("MAIN"), UnicodeString("owner"), UnicodeString("") );
		String xName  = INI->ReadString( UnicodeString("MAIN"), UnicodeString("name"), UnicodeString("") );

		if ( iGroup == xGroup && iName == xName )
		{
			INI->WriteBool( UnicodeString("MAIN"), UnicodeString("freeze"), false );
            INI->UpdateFile();
			breaked = true;
		}

		if ( breaked ) 
        {
            break;
        }
	}
	conf_robots( f->LV_CONF_GROUPS->ItemIndex, false);
	f->vcl->groupechoReadRobots();
}
void c_main::Robots_Freeze()
{
	String iGroup = Trim( f->LV_CONF_GROUPS->Items->Item[f->LV_CONF_GROUPS->ItemIndex]->Caption );

	std::auto_ptr<TStringList> L(new TStringList);
	g.GetFiles( p_robots, L.get() );

	for ( int c = 0; c < L->Count; c++ )
	{
		String robotpath = p_robots + L->Strings[c] + "\\" + "Conf.ini";
		std::auto_ptr<TMemIniFile> INI(new TMemIniFile( UnicodeString(robotpath), TEncoding::UTF8 ));
		String xGroup = INI->ReadString( UnicodeString("MAIN"), UnicodeString("owner"), UnicodeString("") );

		if ( iGroup == xGroup ) 
        {
            INI->WriteBool( UnicodeString("MAIN"), UnicodeString("freeze"), true );
            INI->UpdateFile();
        }
	}
	conf_robots( f->LV_CONF_GROUPS->ItemIndex, false);
	f->vcl->groupechoReadRobots();
}
void c_main::Robots_UnFreeze()
{
	String iGroup = Trim( f->LV_CONF_GROUPS->Items->Item[f->LV_CONF_GROUPS->ItemIndex]->Caption );

	std::auto_ptr<TStringList> L(new TStringList);
	g.GetFiles( p_robots, L.get() );

	for ( int c = 0; c < L->Count; c++ )
	{
		String robotpath = p_robots + L->Strings[c] + "\\" + "Conf.ini";
        std::auto_ptr<TMemIniFile> INI(new TMemIniFile( UnicodeString(robotpath), TEncoding::UTF8 ));
		String xGroup = INI->ReadString( UnicodeString("MAIN"), UnicodeString("owner"), UnicodeString("") );

		if ( iGroup == xGroup ) 
        {
            INI->WriteBool( UnicodeString("MAIN"), UnicodeString("freeze"), false );
            INI->UpdateFile();
        }
	}
	conf_robots( f->LV_CONF_GROUPS->ItemIndex, false);
	f->vcl->groupechoReadRobots();
}
void c_main::import_robots(String file)
{
	try
	{
		std::auto_ptr<TStringList> L(new TStringList);
        if(FileExists(file))
        {
		    L->LoadFromFile(file);
        }

		for ( int c = 0; c < L->Count; c++ )
		{
			String dataline = Trim( L->Strings[c] );

			String r_name   = get_fromfile(1,dataline);
			String r_login  = get_fromfile(2,dataline);
			String r_pass   = get_fromfile(3,dataline);

			conf_robots_import( f->LV_CONF_GROUPS->ItemIndex, r_name, r_login, r_pass );
		}

		f->vcl->groupechoReadRobots();
	}
	catch (...)
	{
		ShowMessage(L"Ошибка импорта роботов из файла.");
    }
}

String  c_main::get_fromfile(int i, String data)
{
	String j = "NULL";

	int p1 = Pos(" ",data);
	int p2 = Pos("	",data);
	int p  = p1;
	if ( p2 > p1 ) p = p2;
	if ( p == 0 )
	{
		for ( int cc = 1; cc <= data.Length(); cc++ )
		{
			int code = data[cc];
			if ( code == 9 )
			{
				p = cc;
				break;
            }
        }
	}

	String A = Trim( data.SubString(1,p-1) );
	data = Trim( data.SubString(p+1,data.Length()) );
	p = Pos(" ",data);
    if ( p == 0 )
	{
		for ( int cc = 1; cc <= data.Length(); cc++ )
		{
			int code = data[cc];
			if ( code == 9 )
			{
				p = cc;
				break;
            }
        }
	}

	String B = Trim( data.SubString(1,p-1) );
	String C = Trim( data.SubString(p+1,data.Length()) );

	if ( i == 1 ) j = A;
	if ( i == 2 ) j = B;
	if ( i == 3 ) j = C;

    return j;
}
String  c_main::ConvertErrors(String edata)
{
	String j = edata;
	if ( Pos("Can't send messages to this user due to their privacy settings",edata) != 0 ) 
    {
        j = L"Этот пользователь принимает сообщения только от друзей";
    }
	if ( Pos("Permission to perform this action is denied",edata) != 0 ) 
    {
        j = L"Действие не выполнено.";
    }

	return " "+j+" ";
}
String  c_main::GetPHPLINE(String data)
{
	int p = Pos("{",data);
	data = data.SubString(p+1,data.Length());
	p = Pos("}",data);
	data = data.SubString(1,p-1);
	return data;
}
void c_main::GetServersFromNET(TStringList *L)
{
	try
	{
		std::auto_ptr<TIdHTTP> HTTP(new TIdHTTP(NULL));
		HTTP->HandleRedirects = true;
		HTTP->ConnectTimeout 	= 10000;
		HTTP->ReadTimeout 		= 10000;

		String URL = HTTPDOMAIN + "API/get_servers.php";
		String J = HTTP->Get( URL );
		J = GetPHPLINE( J );

		std::auto_ptr<TStringList> A(new TStringList);
		A->Text = g.EncryptToList("~",J);
		A->Delete(A->Count-1);

        for ( int c = 0; c < A->Count; c++ ) 
        {
            L->Add( UnicodeString(A->Strings[c])+L"#Тест#Тест" );
        }
	}
	catch ( ... )
	{
        ShowMessage(L"Ошибка!\n\nНевозможно получить список серверов.\n\nПроверьте подключение к интернету и перезапустите программу.");
	}
}
void c_main::asplit(TStringList* lout, String s, String separator)
{
	String buff = "";
	for ( int c = 1; c <= s.Length(); c++ )
	{
		String ch = s[c];
		if ( ch == separator )
		{
			lout->Add( buff );
			buff = "";
		}
		else
		{
			buff = buff + ch;
		}
	}

	if ( buff.Length() > 0 ) 
    {
        lout->Add( buff );
    }
}

// C_PROCESS
c_process::c_process()
{

}
bool c_process::Establish( String RobotName, String *Token )
{
    f->main->PREFIX = "[ "+RobotName+L" ] Соединение : ";

	String IniFile, GroupName, CurRobotName, Server_ID, Login, Password, Activity, token, Online, LastOnline;  
    bool freeze;

	std::auto_ptr<TStringList> L(new TStringList);
	g.GetFiles( f->main->p_robots, L.get() );
	for ( int c = 0; c < L->Count; c++ )
	{
		f->main->get_robotdata( c, &GroupName, &CurRobotName, &Server_ID, &Login, &Password, &token, &Activity, &Online, &LastOnline, &freeze );
		if ( CurRobotName == RobotName )
		{
			IniFile = f->main->p_robots + L->Strings[c] + "\\Conf.ini";
			break;
        }
	}

	String client_id = f->LV_SERVERS->Items->Item[ f->main->CurrentServer ]->SubItems->Strings[0];
	bool WasConnectedEarly;
	bool ESTABLISH = vk.Establish(client_id,Login,Password,&token,f->main->APIV,&WasConnectedEarly);
	f->main->iSleep(1, token);

	if ( ESTABLISH )
	{
		*Token = token;
		if ( ! WasConnectedEarly )
		{
            TDateTime D = Date();
			TDateTime T = Time();
			String DT = DateToStr(D) + " - " + TimeToStr(T);

            std::auto_ptr<TMemIniFile> INI(new TMemIniFile( UnicodeString(IniFile), TEncoding::UTF8 ));
			INI->WriteString(UnicodeString("CONNECTION"), UnicodeString("activity"),  UnicodeString(DT));
			INI->WriteString(UnicodeString("CONNECTION"), UnicodeString("client_id"), UnicodeString(client_id));
			INI->WriteString(UnicodeString("CONNECTION"), UnicodeString("token"),     UnicodeString(token));
            INI->UpdateFile();

			f->main->increment_server();
			f->main->show_current_server();
			f->main->log("[ "+RobotName+L" ] : Токен обновлён.");
		}
	}
	else
	{
		*Token = "NULL";
		f->main->log("[ "+RobotName+L" ] : Соединение отсутствует.....");
	}

	return ESTABLISH;
}
String  c_process::SendMessage( String UserID, String Message, String Token )
{
	String response;
	bool success = false;
	while ( ! success )
	{
		response = vk.messages_send(&success,UserID,Message,Token,"","");
		if ( ! success ) 
        {
            f->main->log(L"Сервер не ответил на запрос. Повтор..");
        }
	}

	if ( Pos("Captcha needed",response) > 0 )
	{
		bool rec = true;
		String CaptchaSID = vk.GetParameter("captcha_sid",&response,false,false,&rec);
		String CaptchaIMG = vk.GetParameter("captcha_img",&response,false,false,&rec);

		f->main->log( L"Запрашивается каптча: [ " + CaptchaSID + " ] [ " + f->captcha->FixURL(CaptchaIMG) + " ]" );
		String CaptchaANS = f->captcha->GetAnswer( CaptchaIMG );
		f->main->log( L"Каптча решена: [ " + CaptchaANS + " ]" );
		f->main->iSleep(1, Token);

		success = false;
        while ( ! success )
		{
			response = vk.messages_send(&success,UserID,Message,Token,CaptchaSID,CaptchaANS);
			if ( ! success )
            {
                f->main->log(L"Сервер не ответил на запрос. Повтор..");
            }
		}
	}

	f->main->iSleep(3, Token);

	if ( Pos("error",response) > 0 ) 
    {
        f->main->log(L"Сервер сообщает об ошибке: ["+response+"]");
    }
	else if ( f->CH_APIRET->Checked ) 
    {
        f->main->log(L"Ответ сервера: ["+response+"]");
    }
	return response;
}
String  c_process::AddToFriends( String UserID, String Message, String Token )
{
	String response;
	bool success = false;
	while ( ! success )
	{
		response = vk.friends_add(&success,UserID,Token,Message,"","");
		if ( ! success ) 
        {
            f->main->log(L"Сервер не ответил на запрос. Повтор..");
        }
	}

	if ( Pos("Captcha needed",response) > 0 )
	{
		bool rec = true;
		String CaptchaSID = vk.GetParameter("captcha_sid",&response,false,false,&rec);
		String CaptchaIMG = vk.GetParameter("captcha_img",&response,false,false,&rec);
		f->main->log( L"Запрашивается каптча: [ " + CaptchaSID + " ] [ " + f->captcha->FixURL(CaptchaIMG) + " ]" );
		String CaptchaANS = f->captcha->GetAnswer( CaptchaIMG );
		f->main->log( L"Каптча решена: [ " + CaptchaANS + " ]" );
		f->main->iSleep(1, Token);

        success = false;
		while ( ! success )
		{
			response = vk.friends_add(&success,UserID,Token,Message,CaptchaSID,CaptchaANS);
			if ( ! success ) 
            {
                f->main->log(L"Сервер не ответил на запрос. Повтор..");
            }
		}
	}

	f->main->iSleep(2, Token);
	if ( Pos("error",response) > 0 ) 
    {   
        f->main->log(L"Сервер сообщает об ошибке: ["+response+"]");
    }
	else if ( f->CH_APIRET->Checked ) 
    {
        f->main->log(L"Ответ сервера: ["+response+"]");
    }
	return response;
}
String  c_process::RequestsFriends( String Token )
{
	String response;

	bool success = false;
	while ( ! success )
	{
		response = vk.friends_request(&success,Token);
		f->main->iSleep(1, Token);
		if ( ! success ) 
        {
            f->main->log(L"Сервер не ответил на запрос. Повтор..");
        }
	}

	if ( Pos("error",response) > 0 ) 
    {
        f->main->log(L"Сервер сообщает об ошибке: ["+response+"]");
    }
	else if ( f->CH_APIRET->Checked ) 
    {
        f->main->log(L"Ответ сервера: ["+response+"]");
    }
	return response;
}
void c_process::ProcessTwoOpen()
{
    f->main->log(L"");
	f->main->log(L"[ ПРОЦЕСС ЗАПУЩЕН ]");

	f->main->AUTOANSBUFF->Clear();
	f->main->AUTOANSOUTBOX->Clear();
	f->main->INBOX->Clear();

	for ( int c = 0; c < f->LV_WORKGROUPS->Items->Count; c++ )
	{
        if ( f->LV_WORKGROUPS->Items->Item[c]->Checked )
		{
			String ActiveGroup = Trim(f->LV_WORKGROUPS->Items->Item[c]->Caption);

			f->main->log(L"");
			f->main->log(L"ГРУППА : [ "+ActiveGroup+" ]");
			f->main->log(L"");

			if ( f->LV_WORKTASKS->Items->Item[0]->Checked )
            {
                ProcessHello(ActiveGroup);
            }
            if ( f->main->TERMINATED ) 
            {
                break;
            }

            std::auto_ptr<TStringList> L_ROBOTS(new TStringList);
			g.GetFiles( f->main->p_robots, L_ROBOTS.get() );
			
			for ( int x = 0; x < L_ROBOTS->Count; x++ )
			{
                String GroupName, RobotName, Server_ID, Login, Password, Token, Activity, Online, LastOnline;  
                bool freeze;
				f->main->get_robotdata( x, 
                                        &GroupName, 
                                        &RobotName, 
                                        &Server_ID, 
                                        &Login, 
                                        &Password, 
                                        &Token, 
                                        &Activity, 
                                        &Online, 
                                        &LastOnline, 
                                        &freeze );

				if ( freeze ) 
                { 
                    continue; 
                }

				if ( GroupName == ActiveGroup )
				{
					if ( f->LV_WORKTASKS->Items->Item[1]->Checked ||
						 f->LV_WORKTASKS->Items->Item[2]->Checked ||
						 f->LV_WORKTASKS->Items->Item[3]->Checked  )
					{
						f->main->log(L"РОБОТ : [ "+RobotName+" ] SERVER : [ "+Server_ID+" ]");
					}

					
					if ( Establish(RobotName,&Token) )
					{
						if ( f->LV_WORKTASKS->Items->Item[1]->Checked )
                        {
                            ProcessFriendConfirm(x,
                                                 GroupName, 
                                                 RobotName, 
                                                 Server_ID, 
                                                 Login, 
                                                 Password, 
                                                 Token, 
                                                 Activity);
                        }
                        if ( f->main->TERMINATED ) 
                        {
                            break;
                        }

						
                        if ( f->LV_WORKTASKS->Items->Item[2]->Checked )
                        {
                            ProcessAutoAnswerCheckNew(RobotName, Token);
                        }
                        if ( f->main->TERMINATED ) 
                        {
                            break;
                        }
						

						if ( f->LV_WORKTASKS->Items->Item[6]->Checked )
                        {
                            f->main->PREFIX = "[ "+RobotName+L" ] Сообщ -> GlobalUsersCache : ";
                            
                            std::auto_ptr<TStringList> UIDS(new TStringList);
                            f->main->GetDialogs(UIDS.get(), 2, 2, Token, RobotName);
                            
                            for ( int c = 0; c < UIDS->Count; c++ )
                            {
                                String UID = UIDS->Strings[c];
                                if ( ! f->main->GlobalUsersCache_Exist( UID ) )
                                {
                                    f->main->GlobalUsersCache_Add( UID );
                                }
                            }
                        }
                        if ( f->main->TERMINATED ) 
                        {
                            break;
                        }

                        
                        if ( f->LV_WORKTASKS->Items->Item[3]->Checked )
                        {
                            ProcessSpeech(x,
                                          GroupName,
                                          RobotName,
                                          Server_ID,
                                          Login,
                                          Password,
                                          Token,
                                          Activity);
                        }
                        if ( f->main->TERMINATED ) 
                        {
                            break;
                        }
					}
                    else 
                    {
                        f->main->log("ProcessTwoOpen::Establish return FALSE");
                    }
				}
				if ( f->main->TERMINATED ) 
                {
                    break;
                }
			}
		}
		if ( f->main->TERMINATED ) 
        {
            break;
        }
	}

	ProcessAutoAnswerRunBuffer();

	f->main->PREFIX = "";
	f->main->log(f->main->TERMINATED ? L"[ ПРОЦЕСС БЫЛ ПРЕРВАН ]" : L"[ ПРОЦЕСС УСПЕШНО ЗАВЕРШЕН ]");
}
void c_process::ProcessHello(String GroupName)
{
    f->main->PREFIX = L"Приветствия : ";
        
	f->BAR->Position = 0;
	f->BAR->Max      = f->main->getCountOfHello(GroupName);
	g.ProcessMessages();

    std::auto_ptr<TStringList> L(new TStringList);
    if(FileExists(f->main->f_users))
    {
	    L->LoadFromFile( f->main->f_users );
    }
	f->main->TGC = 0;

	for ( int x = 0; x < L->Count; x++ )
	{
		String GROUP    = Trim( g.Encrypt(1,5,"#",L->Strings[x]) );
		String UID 	    = Trim( g.Encrypt(2,5,"#",L->Strings[x]) );
		String NAME     = Trim( g.Encrypt(3,5,"#",L->Strings[x]) );
		String SURNAME  = Trim( g.Encrypt(4,5,"#",L->Strings[x]) );

		if ( GROUP == GroupName )
		{
			ProcessHelloDo(GROUP,UID,NAME,SURNAME);
            
			f->BAR->Position++;
			f->vcl->EchoStatistic();
			g.ProcessMessages();

			if ( f->main->TERMINATED ) 
            {
                break;
            }
		}

		if ( f->main->TERMINATED ) 
        {
            break;
        }
	}

	f->vcl->groupechoReadUsers();
	f->vcl->set_enabled_dialogs(false);

	g.ProcessMessages();
}
void c_process::ProcessHelloDo(String Group, String UserID, String UserName, String UserSurname)
{
    std::auto_ptr<TStringList> L(new TStringList);
	g.GetFiles(f->main->p_robots,L.get());
    
    std::auto_ptr<TStringList> XL(new TStringList);
	for ( int c = 0; c < L->Count; c++ )
	{
		String CurGroupName, RobotName, Server_ID, Login, Password, Token, Activity, Online, LastOnline;  
        bool freeze;
		f->main->get_robotdata( c, 
                                &CurGroupName, 
                                &RobotName, 
                                &Server_ID, 
                                &Login, 
                                &Password, 
                                &Token, 
                                &Activity, 
                                &Online, 
                                &LastOnline, 
                                &freeze );
		if ( CurGroupName == Group ) 
        {
            XL->Add(L->Strings[c]);
        }
	}

    int robotIndex = f->main->get_robotnext( Group );
	String RobotPath   = f->main->p_robots + XL->Strings[robotIndex] + "\\";
	String RobotConfig = RobotPath + "Conf.ini";

	std::auto_ptr<TMemIniFile> INI(new TMemIniFile( UnicodeString(RobotConfig),TEncoding::UTF8 ));
	String iGroupName 	= INI->ReadString( UnicodeString("MAIN"),       UnicodeString("owner"),     UnicodeString("0") );
	String iRobotName  = INI->ReadString( UnicodeString("MAIN"),       UnicodeString("name"),  	  UnicodeString("0") );
	String iLogin 		= INI->ReadString( UnicodeString("ACCOUNT"),    UnicodeString("login"),     UnicodeString("0") );
	String iPassword  	= INI->ReadString( UnicodeString("ACCOUNT"),    UnicodeString("password"),  UnicodeString("0") );
	String iActivity 	= INI->ReadString( UnicodeString("CONNECTION"), UnicodeString("activity"),  UnicodeString("0") );
	String iServer_ID 	= INI->ReadString( UnicodeString("CONNECTION"), UnicodeString("client_id"), UnicodeString("0") );
	String iToken     	= INI->ReadString( UnicodeString("CONNECTION"), UnicodeString("token"),     UnicodeString("0") );

	ProcessHelloDoSend(Group,UserID,UserName,UserSurname,iRobotName,iToken,RobotPath);
}
void c_process::ProcessHelloDoSend(String Group, String UserID, String UserName, String UserSurname, String RobotName, String Token, String RobotPath)
{
	std::auto_ptr<TStringList> L(new TStringList);
	L->LoadFromFile(RobotPath+"Hello.txt");
	String LINE = L->Strings[ Random( L->Count ) ];

	if ( Establish(RobotName,&Token) )
	{
		f->main->log(L"Пользователь: [ " + UserID + " ] , [ " + UserName + " " + UserSurname + " ]");

        LINE = f->ii->MakePostLine(LINE);
		String response = SendMessage(UserID,LINE,Token);

		if ( Pos("error",response) == 0 )
		{
			f->main->log(L"Текст: [ " + LINE + " ]" );
			f->main->log(L"Сообщение успешно отправлено. (^_^)");
			f->main->CNT_HELLO++;

			f->main->DeleteUserFromQueueAndPutToDialogs(UserID,UserName,UserSurname,RobotName,true,LINE);
		}
		else
		{
			f->main->log(L"Невозможно отправить сообщение.. (");

			response = AddToFriends(UserID,LINE,Token);

			if ( Pos("response",response) != 0 )
			{
				f->main->log(L"Текстовая вставка: [ " + LINE + " ]" );
				f->main->log(L"Запрос на добавление в друзья успешно подан.");
				f->main->CNT_ADDFRIEND++;
				f->main->DeleteUserFromQueueAndPutToDialogs(UserID,UserName,UserSurname,RobotName,false,LINE);
			}
		}
	}
}
void c_process::ProcessFriendConfirm(int robotIndex, String GroupName, String RobotName, String Server_ID, String Login, String Password, String Token, String Activity)
{
    f->main->PREFIX = "[ "+RobotName+L" ] Приём заявок : ";
	String response = RequestsFriends(Token);

	if ( Pos("error",response) == 0 )
	{
		int CountOf = StrToInt( vk.GetParameter("count",&response,false,true,false) );
		f->main->log(L"Список заявок получен ( " + IntToStr(CountOf) + " )" );
		if ( CountOf > 0 )
		{
			for ( int cnt = 1; cnt <= CountOf; cnt++ )
			{
				String UserID      = "NULL";
				String UserMESSAGE = "NULL";

				vk.GetParameterIntString(cnt,"user_id","message",&UserID,&UserMESSAGE,response);

				if ( UserMESSAGE == "NULL" ) 
                {
                    f->main->log(L"От пользователя: [ " + UserID + " ]");
                }
				else                         
                {
                    f->main->log(L"От пользователя: [ " + UserID + L" ] Текстовая вставка: [ " + UserMESSAGE +" ]" );
                }

				String response;
				bool success = false;
				while ( ! success )
				{
					response = vk.friends_add(&success,UserID,Token,"","","");
					if ( ! success ) 
                    {
                        f->main->log(L"Сервер не ответил на запрос. Повтор..");
                    }
				}

				if ( Pos("error",response) != 0 )
				{
					bool processed = false;

					String code177 = "error_code\":177";
					if ( Pos(code177,response) != 0 )
					{
						f->main->log(L"Ошибка 177. Страница пользователя удалена.");

						String response;

						bool success = false;
						while ( ! success )
						{
							response = vk.account_ban_user(&success,UserID,Token);
							f->main->iSleep(1, Token);
							if ( ! success ) 
                            {
                                f->main->log(L"Сервер не ответил на запрос. Повтор..");
                            }
						}

						if ( f->CH_APIRET->Checked ) 
                        {
                            f->main->log(L"Ответ сервера: ["+response+"]");
                        }
						f->main->log(L"Пользователь добавлен в чёрный список.");
						processed = true;
					}

					String code1 = "error_code\":1";
					if ( Pos(code1,response) != 0 )
					{
						f->main->log(L"Ошибка 1. Робот находится в чёрном списке.");

						String response;

						bool success = false;
						while ( ! success )
						{
							response = vk.account_ban_user(&success,UserID,Token);
							f->main->iSleep(1, Token);
							if ( ! success ) 
                            {
                                f->main->log(L"Сервер не ответил на запрос. Повтор..");
                            }
						}

						if ( f->CH_APIRET->Checked ) 
                        {
                            f->main->log(L"Ответ сервера: ["+response+"]");
                        }
						f->main->log(L"Пользователь добавлен в чёрный список.");
						processed = true;
					}

					if ( ! processed )
					{
						f->main->log(L"Необработанная ошибка: "+response);
                    }
				}
				else
				{
					if ( f->CH_APIRET->Checked ) 
                    {
                        f->main->log(L"Ответ сервера: ["+response+"]");
                    }
					f->main->log(L"Заявка подтверждена.");
					f->main->iSleep(4, Token);
				}

				f->main->CNT_CONFIRMFRIEND++;
				f->vcl->EchoStatistic();
				g.ProcessMessages();

				if ( f->main->TERMINATED ) 
                {
                    break;
                }
			}
		}
	}
	else
	{
		f->main->log(L"Невозможно просмотреть заявки..");
	}
}
void c_process::ProcessAutoAnswerCheckNew(String RobotName, String Token)
{
    f->main->PREFIX = "[ "+RobotName+L" ] Автоответчик : ";

    f->main->WriteAutoAnsOutList(RobotName, Token);

	f->BAR->Position = 0;
	f->BAR->Max      = f->main->AUTOANSOUTBOX->Count;
	g.ProcessMessages();

	std::auto_ptr<TStringList> NEW(new TStringList);
    std::auto_ptr<TStringList> DIALOGS(new TStringList);
    g.GetFiles( f->main->p_dialogs ,DIALOGS.get());

    for ( int c = f->main->AUTOANSOUTBOX->Count-1; c >= 0 ; c-- )
    {
        String utoken           = g.Encrypt(1,6,"#",f->main->AUTOANSOUTBOX->Strings[c]);
        String mid 			    = g.Encrypt(2,6,"#",f->main->AUTOANSOUTBOX->Strings[c]);
        String uid 			    = g.Encrypt(3,6,"#",f->main->AUTOANSOUTBOX->Strings[c]);
        String read_state 		= g.Encrypt(4,6,"#",f->main->AUTOANSOUTBOX->Strings[c]);
        String title            = g.Encrypt(5,6,"#",f->main->AUTOANSOUTBOX->Strings[c]);
        String body             = g.Encrypt(6,6,"#",f->main->AUTOANSOUTBOX->Strings[c]);

        bool ex = false;

        for ( int j = 0; j < DIALOGS->Count; j++ )
        {
            if ( uid == DIALOGS->Strings[j] )
            {
                ex = true;
                break;
            }
        }

        if ( ! ex && read_state == "1" ) 
        {
            ex = true;
        }

        if ( ! ex ) 
        {
            NEW->Add( f->main->AUTOANSOUTBOX->Strings[c] );
        }

        f->BAR->Position++;
        f->vcl->EchoStatistic();
        g.ProcessMessages();

        if ( f->main->TERMINATED ) 
        {
            break;
        }
    }

    f->main->log(L"Новых сообщений от пользователей [ "+IntToStr( NEW->Count )+" ]");
    if ( NEW->Count > 0 )
    {
        std::auto_ptr<TStringList> USERS(new TStringList);
        f->main->make_new_inbox(NEW.get(),USERS.get());
        for ( int c = 0; c < USERS->Count; c++ )
        {
            String uid = USERS->Strings[c];

            if ( f->main->TERMINATED ) 
            {
                break;
            }
            
            std::auto_ptr<TStringList> USERMESSAGES(new TStringList);
            for ( int i = 0; i < NEW->Count; i++ )
            {
                String nuid = g.Encrypt(3,6,"#",NEW->Strings[i]);
                if ( nuid == uid ) 
                {
                    USERMESSAGES->Add( NEW->Strings[i] );
                }
            }

            f->main->log(L"Сообщения от нового пользователя [ "+uid+" ] [ "+IntToStr(USERMESSAGES->Count)+" ]");
            ProcessAutoAnswerAddToDialogs(USERMESSAGES.get(),RobotName,Token,uid);
            f->main->GlobalUsersCache_Add(uid);

            if ( f->main->TERMINATED ) 
            {
                break;
            }
        }
    }
    else
    {
        f->main->log(L"Сообщений от новых пользователей [ 0 ]");
    }
}
void c_process::ProcessAutoAnswerAddToDialogs(TStringList *NEWBOX, String RobotName, String Token, String Uid)
{
	String file = f->main->p_dialogs;
    std::auto_ptr<TStringList> DIALOG(new TStringList);
    std::auto_ptr<TStringList> MESSAGEDATA(new TStringList);

	DIALOG->Add(RobotName);
	DIALOG->Add("A_NULL");
	DIALOG->Add("A_NULL");
	DIALOG->Add(Uid);

	String lastmid = "JNULL";

	for ( int c = 0; c < NEWBOX->Count; c++ )
	{
		String utoken           = g.Encrypt(1,6,"#",NEWBOX->Strings[c]);
		String mid 			    = g.Encrypt(2,6,"#",NEWBOX->Strings[c]);        
        lastmid = mid;
		String uid 			    = g.Encrypt(3,6,"#",NEWBOX->Strings[c]);
		String read_state 		= g.Encrypt(4,6,"#",NEWBOX->Strings[c]);
		String title            = g.Encrypt(5,6,"#",NEWBOX->Strings[c]);
		String body             = g.Encrypt(6,6,"#",NEWBOX->Strings[c]);

		TDateTime D = Date();
		TDateTime T = Time();
		String DT = DateToStr(D) + " - " + TimeToStr(T);
		DIALOG->Add("");
		DIALOG->Add("#MESSAGE");
		DIALOG->Add("#INC");
		DIALOG->Add("#DATE:"+DT);
		DIALOG->Add("#NEW=1");
		DIALOG->Add("#ID="+mid);
		DIALOG->Add("#STAGEDATA=AUTOANS");
		DIALOG->Add("#BEGIN");
		
        std::auto_ptr<TStringList> INCDATA(new TStringList);
        INCDATA->Text = body;
        
		for ( int i = 0; i < INCDATA->Count; i++ ) 
        { 
            DIALOG->Add( INCDATA->Strings[i] ); 
        } 
        
		DIALOG->Add("#END");
		MESSAGEDATA->Add(body);
	}

	f->main->AUTOANSBUFF->Add( RobotName+"|"+Token+"|"+file+Uid+"|"+f->main->from_list_to_str(DIALOG.get())+"|"+f->main->from_list_to_str(MESSAGEDATA.get())+"|"+Uid+"|"+lastmid );
	f->main->log( L"Добавлено в буффер выполнения." );
}
void c_process::ProcessAutoAnswerRunBuffer()
{
    if ( f->main->AUTOANSBUFF->Count > 0 ) 
    {
        f->main->PREFIX = L"Автоответчик : ";
        f->main->log(L"Отправка сообщений из буффера:");

        std::auto_ptr<TStringList> L(new TStringList);
        while ( f->main->AUTOANSBUFF->Count > 0 )
        {
            int indexToAdd = 0; 
            int indexToAddLevel = 999999999;

            for ( int c = 0; c < f->main->AUTOANSBUFF->Count; c++ )
            {
                String robot = g.Encrypt(1,2,"|",f->main->AUTOANSBUFF->Strings[c]);

                int thisIndex = f->main->getrobotindex(robot,L.get());
                if ( thisIndex < indexToAddLevel )
                {
                    indexToAddLevel = thisIndex;
                    indexToAdd = c;
                }
            }

            L->Add( f->main->AUTOANSBUFF->Strings[indexToAdd] );
            f->main->AUTOANSBUFF->Delete( indexToAdd );
        }

        f->main->AUTOANS_LIMIT->Clear();
        for ( int c = 0; c < L->Count; c++ )
        {
            std::auto_ptr<TStringList> MESSAGEDATA(new TStringList);
            std::auto_ptr<TStringList> DIALOG(new TStringList);

            String RobotName 	= g.Encrypt(1,7,"|",L->Strings[c]);
            String Token 		= g.Encrypt(2,7,"|",L->Strings[c]);
            String file 		= g.Encrypt(3,7,"|",L->Strings[c]);
            String dialog 		= g.Encrypt(4,7,"|",L->Strings[c]);
            String message 	= g.Encrypt(5,7,"|",L->Strings[c]);
            String uid 		= g.Encrypt(6,7,"|",L->Strings[c]);
            String lastmid  	= g.Encrypt(7,7,"|",L->Strings[c]);

            if ( ! f->main->sended_count_iflimit(RobotName) )
            {
                String responsedatax;

                bool asuccess = false;
                while ( ! asuccess )
                {
                    responsedatax = vk.users_get(&asuccess,uid,Token,false);
                    f->main->iSleep(1,Token);
                    if ( ! asuccess ) 
                    {
                        f->main->log(L"Сервер не ответил на запрос. Повтор..");
                    }
                }

                bool rec;
                String uname    = vk.GetParameter("first_name",&responsedatax,false,false,&rec);
                String usurname = vk.GetParameter("last_name",&responsedatax,false,false,&rec);

                DIALOG->Text 		= g.EncryptToList("~",dialog);
                MESSAGEDATA->Text 	= g.EncryptToList("~",message);

                DIALOG->Strings[1] = uname;
                DIALOG->Strings[2] = usurname;

                f->main->PREFIX = L"Автоответчик : [ "+RobotName+ " - " + uname + " " + usurname + " ] ";

                bool success = true;
                bool e900 = false;
                bool e902 = false;
                SendAutoAns(RobotName,uid,MESSAGEDATA->Text,Token,DIALOG.get(),&success,&e900,&e902);
                if ( success )
                {
                    DIALOG->SaveToFile(file);
                    f->main->sended_count_add(RobotName);
                }
                else
                {
                    f->main->log(L"Файл диалога создан не был.(");
                    if ( f->main->if_imbanned(Token,uid) )
                    {
                        f->main->log(L"Пользователь занёс робота в ЧС.(");

                        bool bsuccess = false;
                        while ( ! bsuccess )
                        {
                            vk.messages_markAsRead(&bsuccess,lastmid,Token);
                            f->main->iSleep(1,Token);
                            if ( ! bsuccess ) 
                            {
                                f->main->log(L"Сервер не ответил на запрос. Повтор..");
                            }
                        }
                    }

                    if ( e900 )
                    {
                        f->main->log(L"Ошибка 900.(");

                        bool jsuccess = false;
                        while ( ! jsuccess )
                        {
                            vk.messages_markAsRead(&jsuccess,lastmid,Token);
                            f->main->iSleep(1,Token);
                            if ( ! jsuccess ) 
                            {
                                f->main->log(L"Сервер не ответил на запрос. Повтор..");
                            }
                        }
                    }

                    if ( e902 )
                    {
                        f->main->log(L"Ошибка 902.(");

                        bool jsuccess = false;
                        while ( ! jsuccess )
                        {
                            vk.messages_markAsRead(&jsuccess,lastmid,Token);
                            f->main->iSleep(1,Token);
                            if ( ! jsuccess ) 
                            {
                                f->main->log(L"Сервер не ответил на запрос. Повтор..");
                            }
                        }
                    }
                }

                if ( f->main->TERMINATED ) 
                {
                    break;
                }
            }
        }
    }
}
void c_process::ProcessSpeech(int robotIndex, String GroupName, String RobotName, String Server_ID, String Login, String Password, String Token, String Activity)
{
    f->main->PREFIX = "[ "+RobotName+L" ] Общение : ";
    
	f->main->WriteInboxList(RobotName, Token);

    std::auto_ptr<TStringList> DIALOGS(new TStringList);
	g.GetFiles( f->main->p_dialogs ,DIALOGS.get());

	f->BAR->Position = 0;
	f->BAR->Max      = DIALOGS->Count;
	g.ProcessMessages();

	for ( int c = 0; c < DIALOGS->Count; c++ )
	{
		String DialogFile = f->main->p_dialogs + DIALOGS->Strings[c];
        std::auto_ptr<TStringList> DIALOG(new TStringList);
		DIALOG->LoadFromFile( DialogFile );

		String ROBOTGID = DIALOG->Strings[0];
		String NAME     = DIALOG->Strings[1];
		String SURNAME  = DIALOG->Strings[2];
		String USERID   = DIALOG->Strings[3];

		if ( RobotName == ROBOTGID )
		{
			if ( Pos("#STAGEDATA=FIN",DIALOG->Text) == 0 )
			{
				bool SaveDialog = true;
				ProcessSpeechUser(robotIndex,
                                  ROBOTGID,
                                  NAME,
                                  SURNAME,
                                  USERID,
                                  DIALOG.get(),
                                  Token,
                                  &SaveDialog);

				if ( SaveDialog ) 
                {
                    DIALOG->SaveToFile( DialogFile );
                }
				else 			  
                {   
                    f->main->log(L"Сообщение не было сохранено.(");
                }
			}
			else
			{
				f->main->log(f->main->PREFIX + L"Пропуск диалога [ "+USERID+", "+NAME+" "+SURNAME+L" ] : [ Диалог достиг конечной точки ]");
			}
		}

		f->BAR->Position++;
		g.ProcessMessages();

		if ( f->main->TERMINATED ) 
        {   
            break;
        }
	}
}
void c_process::ProcessSpeechUser(int robotIndex, String RobotGID, String Name, String SurName, String UserID, TStringList *DIALOG, String Token, bool *SaveDialog)
{
	f->main->log(L"Обработка диалога [ "+Name+" "+SurName+" ] [ "+UserID+" ]");
    
    std::auto_ptr<TStringList> NEWSTACK(new TStringList);
	String inboxIDS = "";

	for ( int c = f->main->INBOX->Count-1; c >= 0 ; c-- )
	{
		String utoken          = g.Encrypt(1,6,"#",f->main->INBOX->Strings[c]);
		String mid 			   = g.Encrypt(2,6,"#",f->main->INBOX->Strings[c]);
		String uid 			   = g.Encrypt(3,6,"#",f->main->INBOX->Strings[c]);
		String read_state 	   = g.Encrypt(4,6,"#",f->main->INBOX->Strings[c]);
		String title           = g.Encrypt(5,6,"#",f->main->INBOX->Strings[c]);
		String body            = g.Encrypt(6,6,"#",f->main->INBOX->Strings[c]);

		if ( utoken == Token && 
             uid == UserID && 
             Pos("#ID="+mid,DIALOG->Text) == 0 )
		{
			f->main->log(L"Новое входящее сообщение: [ ID:" + mid + L" ] Текст: [ "+body+" ]");

			inboxIDS = inboxIDS + mid + ",";

			TDateTime D = Date();
			TDateTime T = Time();
			String DT = DateToStr(D) + " - " + TimeToStr(T);
			DIALOG->Add("");
			DIALOG->Add("#MESSAGE");
			DIALOG->Add("#INC");
			DIALOG->Add("#DATE:"+DT);
			DIALOG->Add("#NEW=1");
			DIALOG->Add("#ID="+mid);
			DIALOG->Add("#STAGEDATA=INC");
			DIALOG->Add("#BEGIN");
			
            std::auto_ptr<TStringList> INCDATA(new TStringList); 
            INCDATA->Text = body;
			for ( int i = 0; i < INCDATA->Count; i++ ) 
            { 
                DIALOG->Add( INCDATA->Strings[i] ); 
                NEWSTACK->Add( INCDATA->Strings[i] ); 
            } 
			DIALOG->Add("#END");
		}
	}

	bool makereaded = false;
	if ( inboxIDS.Length() != 0 ) 
    {
        ProcessSpeechUserDo(robotIndex,
                            UserID,
                            DIALOG,
                            NEWSTACK->Text,
                            Token,
                            SaveDialog,
                            &makereaded);
    }

	if ( *SaveDialog )
	{
		if ( inboxIDS.Length() > 0 )
		{
			String tidmsgs = inboxIDS.SubString(1,inboxIDS.Length()-1);

			bool csuccess = false;
			while ( !csuccess )
			{
				vk.messages_markAsRead(&csuccess,tidmsgs,Token);
				f->main->iSleep(1,Token);
				if ( ! csuccess ) 
                {
                    f->main->log(L"Сервер не ответил на запрос. Повтор..");
                }
			}

			f->main->log(L"Сообщения помечены прочитанными: [ ID:"+tidmsgs+" ]");
		}
	}

	if ( makereaded )
	{
		if ( inboxIDS.Length() > 0 )
		{
			String tidmsgs = inboxIDS.SubString(1,inboxIDS.Length()-1);

			bool csuccess = false;
			while ( ! csuccess )
			{
				vk.messages_markAsRead(&csuccess,tidmsgs,Token);
				f->main->iSleep(1,Token);
				if ( ! csuccess ) 
                {
                    f->main->log(L"Сервер не ответил на запрос. Повтор..");
                }
			}

			f->main->log(L"Сообщения помечены прочитанными: [ ID:"+tidmsgs+" ]");
        }
	}
}
void c_process::ProcessSpeechUserDo(int robotIndex, String UserID, TStringList *DIALOG, String STACK, String Token, bool *SaveDialog, bool *MakeReaded)
{
    std::auto_ptr<TStringList> L_ROBOTS(new TStringList);
    g.GetFiles( f->main->p_robots, L_ROBOTS.get() );
    String ModelGlobalFile = f->main->p_robots + L_ROBOTS->Strings[robotIndex] + "\\Global.txt";
    String ModelFile = f->main->p_robots + L_ROBOTS->Strings[robotIndex] + "\\Model.txt";
    String AutoStopKeysFile  = f->main->p_robots + L_ROBOTS->Strings[robotIndex] + "\\AutoStopKeys.txt";
    String AutoStopPostsFile = f->main->p_robots + L_ROBOTS->Strings[robotIndex] + "\\AutoStopPosts.txt";

    std::auto_ptr<TStringList> GLOBALMODEL(new TStringList);
    std::auto_ptr<TStringList> MODEL(new TStringList);
    std::auto_ptr<TStringList> STAGE(new TStringList);
    
	GLOBALMODEL->LoadFromFile(ModelGlobalFile);
	MODEL->LoadFromFile(ModelFile);
	STAGE->Text = MODEL->Text;

	String CURRENT_STAGE = f->main->GetLastStageName(DIALOG);
	f->main->log(L"Текущий stage: [ #"+CURRENT_STAGE+" ]");

	if ( CURRENT_STAGE == "HELLO" ) 
    {
        SendStage1(UserID,DIALOG,STACK,Token,MODEL.get());
    }
	else
	{
		f->main->GetOnlyOneStage(STAGE.get(),CURRENT_STAGE);
		if ( STAGE->Strings[0] != "NULL" )
		{
			String TARGET_STAGE = f->ii->GetStage(STACK,STAGE.get());
			if ( TARGET_STAGE == "NULL" || 
                 Pos("FIN",TARGET_STAGE) != 0 )
			{
				*SaveDialog = false;

				if ( TARGET_STAGE == "NULL" ) 		
                {
                    f->main->log(L"Критическая ошибка: Цель [ "+TARGET_STAGE+L" ] не найдена.");
                }
				if ( Pos("FIN",TARGET_STAGE) != 0 )
				{
					*MakeReaded = true;
					f->main->log(f->main->PREFIX + L"Пропуск диалога [ "+UserID+L" ] : [ Диалог достиг конечной точки ]");
				}
			}
			else
			{
				if ( ! f->ii->IfAutoStop(STACK,AutoStopKeysFile) )
				{
					String GLOGALPOST = f->ii->GetGlobalPostText(STACK,GLOBALMODEL.get(),DIALOG);
					if ( GLOGALPOST == "NULL" ) 
                    {
                        SendStageX(UserID,DIALOG,STACK,Token,MODEL.get(),TARGET_STAGE);
                    }
					else                        
                    {
                        SendGlobalPost(UserID,DIALOG,GLOGALPOST,Token,MODEL.get(),CURRENT_STAGE);
                    }
				}
				else 
                {
                    SendAutoStop(UserID,DIALOG,STACK,Token,AutoStopPostsFile);
                }
			}
		}
	}
}
void c_process::ProcessTwoClose()
{
	f->b_PROCESS_TWO_START->Enabled = true;
	f->b_PROCESS_TWO_STOP->Enabled = false;
	f->TINTERVAL->Enabled = f->ch_entimer->Checked;
	g.ProcessMessages();
}
void c_process::SendStage1(String UserID, TStringList *DIALOG, String STACK, String Token, TStringList *MODEL)
{
	f->main->log(L"Отправка первого stage.");

    std::auto_ptr<TStringList> IMAGELIST(new TStringList);
    std::auto_ptr<TStringList> AUDIOLIST(new TStringList);
    std::auto_ptr<TStringList> RECORDLIST(new TStringList);
    std::auto_ptr<TStringList> POST(new TStringList);
    std::auto_ptr<TStringList> STAGE(new TStringList);
    STAGE->Text = MODEL->Text;

    String STAGENAME;
	for ( int c = 0; c < MODEL->Count; c++ )
	{
		String l = MODEL->Strings[c];
		if ( Pos("#",l) != 0 )
		{
			STAGENAME = l.SubString(2,l.Length());
			f->main->GetOnlyOneStage(STAGE.get(),STAGENAME);

			for ( int x = 0; x < STAGE->Count; x++ )
			{
				String ln = STAGE->Strings[x];
				if ( Pos("POST'",ln) != 0 )
				{
					String PostLine = g.Encrypt(2,2,"'",ln);
					f->ii->WritePostDataIn(IMAGELIST.get(),AUDIOLIST.get(),RECORDLIST.get(),&PostLine);
					POST->Add( PostLine );
				}
				else
				{
					if ( Pos("[IMAGE=",ln) != 0 )
					{
						IMAGELIST->Add( ln );
					}
					if ( Pos("[AUDIO=",ln) != 0 )
					{
						AUDIOLIST->Add( ln );
					}
					if ( Pos("[RECORD=",ln) != 0 )
					{
						RECORDLIST->Add( ln );
					}
				}
            }
			break;
        }
	}

	String DATAPOST = POST->Strings[ Random( POST->Count ) ];
	DATAPOST = f->ii->MakePostLine(DATAPOST);

	String MESSID = SendMessage( UserID, DATAPOST, Token );
	if ( Pos("error",MESSID) == 0 )
	{
		f->main->log(L"Текст: [ " + DATAPOST + " ]" );
		f->main->log(L"Сообщение успешно отправлено. (^_^)");

		TDateTime D = Date();
		TDateTime T = Time();
		String DT = DateToStr(D) + " - " + TimeToStr(T);
		DIALOG->Add("");
		DIALOG->Add("#MESSAGE");
		DIALOG->Add("#OUT");
		DIALOG->Add("#DATE:"+DT);
		DIALOG->Add("#NEW=0");
		DIALOG->Add("#ID="+MESSID);
		DIALOG->Add("#STAGEDATA="+STAGENAME);
		DIALOG->Add("#BEGIN");
		DIALOG->Add( DATAPOST );
		DIALOG->Add("#END");

        f->main->CNT_MESSAGES++;
		f->vcl->EchoStatistic();
	}
	else
	{
		f->main->log(L"Невозможно отправить сообщение.. (");
	}

    SendImage(UserID,IMAGELIST.get(),Token);
	SendAudio(UserID,AUDIOLIST.get(),Token);
	SendAudioRec(UserID,RECORDLIST.get(),Token);
}
void c_process::SendGlobalPost(String UserID, TStringList *DIALOG, String POSTTEXT, String Token, TStringList *MODEL, String CURRENT_STAGE)
{
	POSTTEXT = f->ii->MakePostLine(POSTTEXT);
	String MESSID = SendMessage( UserID, POSTTEXT, Token );

	if ( Pos("error",MESSID) == 0 )
	{
		f->main->log(L"Текст: [ " + POSTTEXT + " ]" );
		f->main->log(L"Сообщение успешно отправлено. (^_^)");

		TDateTime D = Date();
		TDateTime T = Time();
		String DT = DateToStr(D) + " - " + TimeToStr(T);
		DIALOG->Add("");
		DIALOG->Add("#MESSAGE");
		DIALOG->Add("#OUT");
		DIALOG->Add("#DATE:"+DT);
		DIALOG->Add("#NEW=0");
		DIALOG->Add("#ID="+MESSID);
		DIALOG->Add("#STAGEDATA="+CURRENT_STAGE);
		DIALOG->Add("#BEGIN");
		DIALOG->Add( POSTTEXT );
		DIALOG->Add("#END");
		DIALOG->Add("#ITISGLOBAL");

		f->main->CNT_MESSAGES++;
		f->vcl->EchoStatistic();
	}
	else
	{
		f->main->log(L"Невозможно отправить сообщение.. (");
	}
}
void c_process::SendStageX(String UserID, TStringList *DIALOG, String STACK, String Token, TStringList *MODEL, String TARGET_STAGE)
{
	f->main->log(L"Отправка stage: [ #"+TARGET_STAGE+" ]");

    std::auto_ptr<TStringList> POST(new TStringList);
    std::auto_ptr<TStringList> IMAGELIST(new TStringList);
    std::auto_ptr<TStringList> AUDIOLIST(new TStringList);
    std::auto_ptr<TStringList> RECORDLIST(new TStringList);
    std::auto_ptr<TStringList> STAGE(new TStringList);
    STAGE->Text = MODEL->Text;

	f->main->GetOnlyOneStage(STAGE.get(),TARGET_STAGE);
	for ( int x = 0; x < STAGE->Count; x++ )
	{
		String ln = STAGE->Strings[x];
		if ( Pos("POST'",ln) != 0 )
		{
			String PostLine = g.Encrypt(2,2,"'",ln);
			f->ii->WritePostDataIn(IMAGELIST.get(),AUDIOLIST.get(),RECORDLIST.get(),&PostLine);
			POST->Add( PostLine );
		}
		else
		{
			if ( Pos("[IMAGE=",ln) != 0 )
			{
				IMAGELIST->Add( ln );
			}
			if ( Pos("[AUDIO=",ln) != 0 )
			{
				AUDIOLIST->Add( ln );
			}
			if ( Pos("[RECORD=",ln) != 0 )
			{
				RECORDLIST->Add( ln );
			}
		}
	}

	String DATAPOST = POST->Strings[ Random( POST->Count ) ];
	DATAPOST = f->ii->MakePostLine(DATAPOST);

	String MESSID = SendMessage( UserID, DATAPOST, Token );
    if ( Pos("error",MESSID) == 0 )
	{
		f->main->log(L"Текст: [ " + DATAPOST + " ]" );
		f->main->log(L"Сообщение успешно отправлено. (^_^)");

		TDateTime D = Date();
		TDateTime T = Time();
		String DT = DateToStr(D) + " - " + TimeToStr(T);
		DIALOG->Add("");
		DIALOG->Add("#MESSAGE");
		DIALOG->Add("#OUT");
		DIALOG->Add("#DATE:"+DT);
		DIALOG->Add("#NEW=0");
		DIALOG->Add("#ID="+MESSID);
		DIALOG->Add("#STAGEDATA="+TARGET_STAGE);
		DIALOG->Add("#BEGIN");
		DIALOG->Add( DATAPOST );
		DIALOG->Add("#END");

		f->main->CNT_MESSAGES++;
		f->vcl->EchoStatistic();
	}
	else
	{
		f->main->log(L"Невозможно отправить сообщение.. (");
	}

    SendImage(UserID,IMAGELIST.get(),Token);
	SendAudio(UserID,AUDIOLIST.get(),Token);
	SendAudioRec(UserID,RECORDLIST.get(),Token);
}
void c_process::SendAutoAns(String RobotName, String UserID, String MessageData, String Token, TStringList *DIALOG, bool *success, bool *e900, bool *e902)
{
    String PX = f->main->PREFIX;
	f->main->PREFIX = L"Автоответчик : [ "+RobotName+" ] ";
    
	f->main->WriteAutoAnsOutList(RobotName, Token);

	f->main->PREFIX = PX;
    
	f->main->log(L"Входящий текст: [ "+ Trim( MessageData.SubString(1,MessageData.Length()-3) ) +" ]");

	String TARGETSTAGE = "NULL";
	f->ii->GetAutoAns(&MessageData,&TARGETSTAGE,Token,UserID);
	f->main->log(L"Отправка stage: [ "+TARGETSTAGE+" ]");

    std::auto_ptr<TStringList> IMAGELIST(new TStringList);
    std::auto_ptr<TStringList> AUDIOLIST(new TStringList);
    std::auto_ptr<TStringList> RECORDLIST(new TStringList);

	f->ii->WritePostDataIn(IMAGELIST.get(),AUDIOLIST.get(),RECORDLIST.get(),&MessageData);
	MessageData = f->ii->MakePostLine(MessageData);
	String MESSID = SendMessage( UserID, MessageData, Token );

	if ( Pos("error",MESSID) == 0 )
	{
		f->main->log(L"Исходящий текст: [ " + MessageData + " ]" );
		f->main->log(L"Сообщение успешно отправлено. (^_^)");

		TDateTime D = Date();
		TDateTime T = Time();
		String DT = DateToStr(D) + " - " + TimeToStr(T);
		DIALOG->Add("");
		DIALOG->Add("#MESSAGE");
		DIALOG->Add("#OUT");
		DIALOG->Add("#DATE:"+DT);
		DIALOG->Add("#NEW=0");
		DIALOG->Add("#ID="+MESSID);
		DIALOG->Add("#STAGEDATA="+TARGETSTAGE);
		DIALOG->Add("#BEGIN");
		DIALOG->Add( MessageData );
		DIALOG->Add("#END");

        f->main->CNT_MESSAGES++;
		f->vcl->EchoStatistic();

        SendImage(UserID,IMAGELIST.get(),Token);
		SendAudio(UserID,AUDIOLIST.get(),Token);
		SendAudioRec(UserID,RECORDLIST.get(),Token);
	}
	else
	{
		*success = false;

		f->main->log(L"Невозможно отправить сообщение.. (");

		if ( Pos("900",MESSID) != 0 ) 
        {
            *e900 = true;
        }
		if ( Pos("902",MESSID) != 0 ) 
        {
            *e902 = true;
        }
	}
}
void c_process::SendAutoStop(String UserID, TStringList *DIALOG, String STACK, String Token, String AutoStopPostsFile)
{
	f->main->log(L"Отправка: [ Автостоп вариант ]");

	String DATAPOST = f->ii->GetAutoStopMessage(AutoStopPostsFile);
    DATAPOST = f->ii->MakePostLine(DATAPOST);
	String MESSID = SendMessage( UserID, DATAPOST, Token );

    if ( Pos("error",MESSID) == 0 )
	{
		f->main->log(L"Текст: [ " + DATAPOST + " ]" );
		f->main->log(L"Сообщение успешно отправлено. (^_^)");

		TDateTime D = Date();
		TDateTime T = Time();
		String DT = DateToStr(D) + " - " + TimeToStr(T);
		DIALOG->Add("");
		DIALOG->Add("#MESSAGE");
		DIALOG->Add("#OUT");
		DIALOG->Add("#DATE:"+DT);
		DIALOG->Add("#NEW=0");
		DIALOG->Add("#ID="+MESSID);
		DIALOG->Add("#STAGEDATA=FIN");
		DIALOG->Add("#BEGIN");
		DIALOG->Add( DATAPOST );
		DIALOG->Add("#END");

		f->main->CNT_MESSAGES++;
		f->vcl->EchoStatistic();
	}
	else
	{
		f->main->log(L"Невозможно отправить сообщение.. (");
	}
}
void c_process::SendImage(String UserID, TStringList *IMAGELIST, String Token)
{
	try
	{
		if ( IMAGELIST->Count > 0 )
		{
			String x = IMAGELIST->Strings[ Random( IMAGELIST->Count ) ];
			String afx = "[IMAGE="; 
            x = x.SubString(afx.Length()+1,x.Length()); 
            x = x.SubString(1,x.Length()-1);
			String fullpath = f->main->p_resources_images + x;

			bool success;
			String uploadurl = vk.photos_getUploadServer( &success, Token );

			// JSON //////////////////////////////////////////////////////////////////////////////
			try
			{
				std::auto_ptr<TJSONObject> json (static_cast<TJSONObject*>(TJSONObject::ParseJSONValue(uploadurl)));
				TJSONObject *obj_response = static_cast<TJSONObject*>(json->Get("response")->JsonValue);
				uploadurl    = obj_response->GetValue("upload_url")->ToString();
				String album_id = obj_response->GetValue("album_id")->ToString();
				String user_id  = obj_response->GetValue("user_id")->ToString();
			}
			catch (...) 
            { 
                f->main->log("TJSONObject 1"); 
            }
			///////////////////////////////////////////////////////////////////////////////

			if ( success )
			{
				f->main->log(L"Загрузка изображения на сервер.. [ "+x+" ]");
				String RETURN = vk.UploadToServer( &success, fullpath, g.cut_kk(uploadurl) );
				f->main->log(L"Загрузка завершена.");
				f->main->log(L"Обработка..");

                // JSON //////////////////////////////////////////////////////////////////////////////
				try
				{
					std::auto_ptr<TJSONObject> json (static_cast<TJSONObject*>(TJSONObject::ParseJSONValue(RETURN)));

					String server    = json->GetValue("server")->ToString();
					String hash      = json->GetValue("hash")->ToString();      
                    hash = g.cut_kk( hash );
					String photo  = g.StripSlashes( RETURN );
					photo      = g.cut_photo( photo );

					String RETURN2 = vk.photos_saveMessagesPhoto( &success, photo, server, hash, Token );
					f->main->log(L"Отправка Media Image..");

                    // JSON //////////////////////////////////////////////////////////////////////////////
					try
					{
						std::auto_ptr<TJSONObject> json (static_cast<TJSONObject*>(TJSONObject::ParseJSONValue(RETURN2)));

						TJSONArray *obj_items = static_cast<TJSONArray*>(json->Get("response")->JsonValue);

						TJSONObject* x_obj_items = static_cast<TJSONObject*>(obj_items->Items[0]);

						String xid    = x_obj_items->GetValue("id")->ToString();
						String ownerid = x_obj_items->GetValue("owner_id")->ToString();

						vk.messages_send2( &success, UserID, "photo"+ownerid+"_"+xid, Token );
					}
					catch (...) 
                    { 
                        f->main->log("TJSONObject 3"); 
                    }
				}
				catch (...) 
                { 
                    f->main->log("TJSONObject 2"); 
                }
			}
		}
	}
	catch (...)
	{
		f->main->log("error in: c_process::SendImage(catch);");
	}
}
void c_process::SendAudio(String UserID, TStringList *AUDIOLIST, String Token)
{
	try
	{
		if ( AUDIOLIST->Count > 0 )
		{
			String x = AUDIOLIST->Strings[ Random( AUDIOLIST->Count ) ];
			String afx = "[AUDIO="; 
            x = x.SubString(afx.Length()+1,x.Length()); 
            x = x.SubString(1,x.Length()-1);
			String fullpath = f->main->p_resources_audio + x;

            bool success;
			String uploadurl = vk.audio_getUploadServer( &success, Token );

			// JSON //////////////////////////////////////////////////////////////////////////////
			try
			{
				std::auto_ptr<TJSONObject> json (static_cast<TJSONObject*>(TJSONObject::ParseJSONValue(uploadurl)));
				TJSONObject *obj_response = static_cast<TJSONObject*>(json->Get("response")->JsonValue);
				uploadurl    = obj_response->GetValue("upload_url")->ToString();
			}
			catch (...) 
            {
                f->main->log("TJSONObject 1");
            }

			if ( success )
			{
				f->main->log(L"Загрузка аудиофайла на сервер.. [ "+x+" ]");
				String RETURN = vk.audioUploadToServer( &success, fullpath, g.cut_kk(uploadurl) );
				f->main->log(L"Загрузка завершена.");
				f->main->log(L"Обработка..");

				// JSON //////////////////////////////////////////////////////////////////////////////
				try
				{
					std::auto_ptr<TJSONObject> json (static_cast<TJSONObject*>(TJSONObject::ParseJSONValue(RETURN)));
					String redirect    = json->GetValue("redirect")->ToString();
					String server      = json->GetValue("server")->ToString();
					String audio       = json->GetValue("audio")->ToString();
					String hash        = json->GetValue("hash")->ToString();

					redirect = g.cut_kk( redirect );
					audio = g.cut_kk( audio );
					hash = g.cut_kk( hash );

                    String RETURN2 = vk.audio_Save( &success, audio, server, hash, Token );
					f->main->log(L"Отправка Media Audio..");

                    // JSON //////////////////////////////////////////////////////////////////////////////
					try
					{
						std::auto_ptr<TJSONObject> json (static_cast<TJSONObject*>(TJSONObject::ParseJSONValue(RETURN2)));
						TJSONObject *obj_items = static_cast<TJSONObject*>(json->Get("response")->JsonValue);

						String xid      = obj_items->GetValue("id")->ToString();
						String ownerid = obj_items->GetValue("owner_id")->ToString();

						vk.messages_send2( &success, UserID, "audio"+ownerid+"_"+xid, Token );
					}
					catch (...) 
                    {
                        f->main->log("TJSONObject 3"); 
                    }
				}
				catch (...) 
                { 
                    f->main->log("TJSONObject 1"); 
                }
			}
		}
	}
	catch (...)
	{
		f->main->log("error in: c_process::SendAudio(catch);");
	}
}
void c_process::SendAudioRec(String UserID, TStringList *AUDIOLIST, String Token)
{
	try
	{
		if ( AUDIOLIST->Count > 0 )
		{
			String x = AUDIOLIST->Strings[ Random( AUDIOLIST->Count ) ];
			String afx = "[RECORD="; 
            x = x.SubString(afx.Length()+1,x.Length()); 
            x = x.SubString(1,x.Length()-1);
			String fullpath = f->main->p_resources_records + x;

            bool success;
			String uploadurl = vk.docs_getMessagesUploadServer( &success, Token );
			// JSON //////////////////////////////////////////////////////////////////////////////
			try
			{
				std::auto_ptr<TJSONObject> json (static_cast<TJSONObject*>(TJSONObject::ParseJSONValue(uploadurl)));
				TJSONObject *obj_response = static_cast<TJSONObject*>(json->Get("response")->JsonValue);
				uploadurl    = obj_response->GetValue("upload_url")->ToString();
			}
			catch (...) 
            {
                f->main->log("TJSONObject 1"); 
            }
			if ( success )
			{
				f->main->log(L"Загрузка аудиозаписи на сервер.. [ "+x+" ]");
				String RETURN = vk.audioUploadToServer( &success, fullpath, g.cut_kk(uploadurl) );
				f->main->log(L"Загрузка завершена.");
				f->main->log(L"Обработка..");

				try
				{
					RETURN = RETURN.SubString(8,RETURN.Length());
					String FILE = g.cut_kk( RETURN );
					FILE = g.cut_kk( FILE );

					String RETURN2 = vk.docs_save( &success, FILE, "MyFile1", "Socionic", Token );
					f->main->log(L"Отправка Media Record..");

					// JSON //////////////////////////////////////////////////////////////////////////////
					try
					{
						std::auto_ptr<TJSONObject> json (static_cast<TJSONObject*>(TJSONObject::ParseJSONValue(RETURN2)));
						TJSONArray *obj_items = static_cast<TJSONArray*>(json->Get("response")->JsonValue);
						TJSONObject* x_obj_items = static_cast<TJSONObject*>(obj_items->Items[0]);
						String xid    = x_obj_items->GetValue("id")->ToString();
						String ownerid = x_obj_items->GetValue("owner_id")->ToString();
						vk.messages_send2( &success, UserID, "doc"+ownerid+"_"+xid, Token );
					}
					catch (...) 
                    { 
                        f->main->log("TJSONObject 3"); 
                    }
				}
				catch (...) 
                { 
                    f->main->log("TJSONObject 1"); 
                }
			}
		}
	}
	catch (...)
	{
		f->main->log("error in: c_process::SendRecoed(catch);");
	}
}
// C_II
c_ii::c_ii()
{

}
String  c_ii::GetGlobalPostText(String STACK, TStringList *GLOBALMODEL, TStringList *DIALOG)
{
	String j = "NULL";

	if ( ! LastMessageIsGlobal(DIALOG) )
	{
        std::auto_ptr<TStringList> IFS(new TStringList);
        std::auto_ptr<TStringList> POSTS(new TStringList);

		for ( int c = 0; c < GLOBALMODEL->Count; c++ )
		{
			String dataline = GLOBALMODEL->Strings[c];

			if ( Pos("#",dataline) != 0 || c == GLOBALMODEL->Count-1 )
			{
				if ( IFS->Count > 0 && POSTS->Count > 0 )
				{
					for ( int c = 0; c < IFS->Count; c++ )
					{
						String LINE_IF = IFS->Strings[c];
                        std::auto_ptr<TStringList> ARR(new TStringList);
                        
						f->main->asplit(ARR.get(), LINE_IF, "'");
                        std::auto_ptr<TStringList> ARRC(new TStringList);
						for ( int x = 0; x < ARR->Count; x++ )
						{
							if (  ARR->Strings[x] != "IF" && ARR->Strings[x] != "OR" ) 
                            {
                                ARRC->Add( ARR->Strings[x] );
                            }
						}

						for ( int x = 0; x < ARRC->Count; x++ )
						{
							if ( Pos( ARRC->Strings[x].LowerCase(), STACK.LowerCase() ) != 0 )
							{
								f->main->log(L"GLOBAL IF : найден ключ: [ "+ARRC->Strings[x]+L" ] в [ "+Trim(STACK)+" ]");

								String rc = POSTS->Strings[ Random( POSTS->Count ) ];
								j = rc.SubString(6,rc.Length());
							}
						}
					}
				}

				IFS->Clear();
				POSTS->Clear();
			}
			else
			{
				if ( Pos("IF'",dataline)   != 0 ) 
                {
                    IFS->Add(dataline);
                }
				if ( Pos("POST'",dataline) != 0 ) 
                {
                    POSTS->Add(dataline);
                }
			}
		}
	}
	return j;
}
bool c_ii::LastMessageIsGlobal(TStringList *DIALOG)
{
	bool j = false;
	int mc = 0;
    std::auto_ptr<TStringList> X(new TStringList);

	for ( int c = DIALOG->Count-1; c >= 0; c-- )
	{
		String l = DIALOG->Strings[c];
		if ( l == "#MESSAGE" ) 
        {
            mc++;
        }
        if ( mc > 1 ) 
        {
            break;
        }
		if ( l == "#ITISGLOBAL" )
		{
			j = true;
			break;
        }
	}
	return j;
}
String  c_ii::GetStage(String STACK, TStringList *STAGE)
{
	String J = "NULL";

	try
	{
        std::auto_ptr<TStringList> LinesIF(new TStringList);
		MakeMass(STAGE,LinesIF.get());

		J = GetTarget(LinesIF.get(),STACK);

		if ( J != "NULL" )
		{
			f->main->log(L"Определён IF. Цель: [ #"+J+" ]");
		}
		else
		{
			GetDefault(STAGE,&J);
			f->main->log(L"IF не найден. Цель по умолчанию: [ #"+J+" ]");
		}
	}
	catch ( Exception *ex )
	{
		J = "NULL";
		f->main->log(L"Критическая ошибка: GetStage: "+ex->Message);
	}

	return J;
}
void c_ii::MakeMass(TStringList *STAGE, TStringList *LinesIF)
{
	for ( int c = 0; c < STAGE->Count; c++ )
	{
		String dataline = STAGE->Strings[c];
		if ( Pos("IF'",dataline) != 0 )
		{
			LinesIF->Add( dataline );
        }
	}
}
String  c_ii::GetTarget(TStringList *LinesIF, String STACK)
{
	String J = "NULL";

	for ( int c = 0; c < LinesIF->Count; c++ )
	{
		String LINE_IF = LinesIF->Strings[c];
        std::auto_ptr<TStringList> ARR(new TStringList);
		f->main->asplit(ARR.get(), LINE_IF, "'");
        std::auto_ptr<TStringList> ARRC(new TStringList);
        
		for ( int x = 0; x < ARR->Count; x++ )
		{
			if (  ARR->Strings[x] != "IF" && ARR->Strings[x] != "OR" ) 
            {
                ARRC->Add( ARR->Strings[x] );
            }
        }

		for ( int x = 0; x < ARRC->Count-1; x++ )
		{
			if ( Pos( ARRC->Strings[x].LowerCase(), STACK.LowerCase() ) != 0 )
			{
				J = ARRC->Strings[ARRC->Count-1];
            }
		}
	}

	return J;
}
void c_ii::GetDefault(TStringList *STAGE, String *TARGETSTAGE)
{
	for ( int c = 0; c < STAGE->Count; c++ )
	{
		String dataline = STAGE->Strings[c];
		if ( Pos("DEFAULT'",dataline) != 0 )
		{
			String j = g.Encrypt(2,2,"'",dataline);
			*TARGETSTAGE = j;
			break;
        }
	}
}
String  c_ii::GetOldStage(String uid, String token, TStringList *RobotModel, String Message)
{
	String J = "NULL";                                                                            
	String MYPOST = "NULL";
	String WASSTAGE = "NULL";
																
	for ( int c = 0; c < f->main->AUTOANSOUTBOX->Count; c++ )
	{
		String line = f->main->AUTOANSOUTBOX->Strings[c];           

		String xToken  = g.Encrypt(1,6,"#",line);
		String xMid    = g.Encrypt(2,6,"#",line);
		String xUid 	= g.Encrypt(3,6,"#",line);
		String xRs 	= g.Encrypt(4,6,"#",line);
		String xTitle  = g.Encrypt(5,6,"#",line);
		String xBody 	= g.Encrypt(6,6,"#",line);

		xBody = xBody.SubString(2,xBody.Length());
        xBody = xBody.SubString(1,xBody.Length()-1);

		if ( xToken == token && uid == xUid )
		{
			if ( WASSTAGE != "NULL" ) 
            { 
                f->main->log(L"Итерации поиска предыдущего STAGE завершены."); 
                break; 
            }

			MYPOST = xBody;

			f->main->log(L"Обнаружена предыдущая переписка с пользователем. Сообщение №"+IntToStr(c+1));
			f->main->log(L"Сообщение робота: [ "+MYPOST+" ]");

			bool xbreaked = false;
			for ( int c = 0; c < RobotModel->Count; c++ )
			{
				String l = RobotModel->Strings[c];
				if ( Pos("POST'",l) != 0 )
				{
					String ModelPost = g.Encrypt(2,2,"'",l);
					if ( if_EqualPosts(ModelPost,MYPOST) )
					{
						int C = c;
						while ( true )
						{
							C--;
							if ( Pos("#",RobotModel->Strings[C]) != 0 )
							{
								WASSTAGE = RobotModel->Strings[C].SubString(2,RobotModel->Strings[C].Length());
								f->main->log("Stage [ #"+WASSTAGE+L" ] инициализирован.");
								xbreaked = true;
								break;
							}
						}
					}
				}

				if ( xbreaked ) 
                {
                    break;
                }
			}

			if ( WASSTAGE != "NULL" )
			{
				std::auto_ptr<TStringList> L(new TStringList);
				L->Text = RobotModel->Text;
				f->main->GetOnlyOneStage(L.get(),WASSTAGE);
				J = "#"+f->ii->GetStage(Message,L.get());
			}
			else
			{
				f->main->log("Stage [ #"+WASSTAGE+L" ] не найден в модели общения.");
			}
		}
	}

	return J;                                                                                                  
}
void c_ii::GetAutoAns(String *iodata, String *targetstage, String Token, String uid)
{
	String data = *iodata;                        
    String DEF;                                  
    
    std::auto_ptr<TStringList> CLEAR(new TStringList);
    std::auto_ptr<TStringList> ANSMODEL(new TStringList);
    std::auto_ptr<TStringList> DEFAULT(new TStringList);
    std::auto_ptr<TStringList> ROBOTMODEL(new TStringList);
    std::auto_ptr<TStringList> L(new TStringList);
	g.GetFiles( f->main->p_robots, L.get() );
    
	String GroupName, RobotName, Server_ID, Login, Password, xToken, Activity, ModelFile, Online, LastOnline;  bool freeze;
	for ( int x = 0; x < L->Count; x++ )
	{
		f->main->get_robotdata( x, &GroupName, &RobotName, &Server_ID, &Login, &Password, &xToken, &Activity, &Online, &LastOnline, &freeze );

		if ( Token == xToken )
		{
			ANSMODEL->LoadFromFile( f->main->p_robots + L->Strings[x] + "\\AutoAnsRules.txt" );
			DEFAULT->LoadFromFile( f->main->p_robots + L->Strings[x] + "\\AutoAnsDefault.txt" );
			DEF = Trim(DEFAULT->Strings[0]);
			ROBOTMODEL->LoadFromFile( f->main->p_robots + L->Strings[x] + "\\Model.txt" );
			break;
		}
	}

	f->main->log(L"Попытка определить прежний Stage." );
	String target = GetOldStage(uid,xToken,ROBOTMODEL.get(),data);
	if ( target == "NULL" ) 
    {
        f->main->log( L"Прежний Stage не найден. (" );
    }

	if ( target == "NULL" )
	{
		for ( int c = 0; c < ANSMODEL->Count; c++ )
		{
			String key = g.Encrypt(1,2,"~",ANSMODEL->Strings[c]);
			if ( Pos(get_alpha_keys(key),data) > 0 )
			{
				target = g.Encrypt(2,2,"~",ANSMODEL->Strings[c]);
				f->main->log( L"В сообщении найден ключ [ "+get_alpha_keys(key)+" ]" );
				break;
			}
		}
	}

	if ( target == "NULL" ) 
    {
        target = DEF;
	}

	*targetstage = target;
    
	bool breaked = false;
	for ( int i = 0; i < ROBOTMODEL->Count; i++ )
	{
		String l = ROBOTMODEL->Strings[i];
		if ( l == target )
		{
			while ( true )
			{
				String l = ROBOTMODEL->Strings[i]; i++;
				if ( Pos("DEFAULT",l) > 0 ) 
                {
                    breaked = true;
                }
				if ( Pos("POST'",l)   > 0 ) 
                {
                    CLEAR->Add(l);
                }
				if ( breaked ) 
                {
                    break;
                }
			}
		}

		if ( breaked ) 
        {
            break;
        }
	}

	String J = CLEAR->Strings[ Random( CLEAR->Count ) ];
	J = g.Encrypt(2,2,"'",J);
	*iodata = J;
}
bool c_ii::IfAutoStop(String message, String file)
{
	bool stop = false;

	if ( f->LV_WORKTASKS->Items->Item[4]->Checked )
	{
        std::auto_ptr<TStringList> L(new TStringList);
		L->LoadFromFile(file);

		message = message;
		for ( int c = 0; c < L->Count; c++ )
		{
			String key = Trim(L->Strings[c]);
			key = get_alpha_keys(key);

			if ( Pos(key,message) != 0 )
			{
				stop = true;
				f->main->log(L"Зафиксировано : [ "+key+" ]");
				break;
			}
		}
	}
	return stop;
}
String  c_ii::GetAutoStopMessage(String file)
{
	std::auto_ptr<TStringList> L(new TStringList);
	L->LoadFromFile(file);

	String j = "NULL";
	if ( L->Count > 0 ) 
    {
        j = L->Strings[ Random( L->Count ) ];
    }
	return Trim(j);
}
String  c_ii::do_randomize(String text)
{
	String j = "";

	if ( f->ch_RANDOMIZE->Checked )
	{
		for ( int c = 1; c <= text.Length(); c++ )
		{
			String ch = text[c];

			int x = Random(2);
			if ( x == 1 )
			{
				if ( ch == L"А" ) ch = L"A";
				if ( ch == L"В" ) ch = L"B";
				if ( ch == L"Е" ) ch = L"E";
				if ( ch == L"К" ) ch = L"K";
				if ( ch == L"М" ) ch = L"M";
				if ( ch == L"Н" ) ch = L"H";
				if ( ch == L"О" ) ch = L"O";
				if ( ch == L"Р" ) ch = L"P";
				if ( ch == L"С" ) ch = L"C";
				if ( ch == L"Т" ) ch = L"T";
				if ( ch == L"Х" ) ch = L"X";

				if ( ch == L"а" ) ch = L"a";
				if ( ch == L"е" ) ch = L"e";
				if ( ch == L"о" ) ch = L"o";
				if ( ch == L"р" ) ch = L"p";
				if ( ch == L"с" ) ch = L"c";
				if ( ch == L"у" ) ch = L"y";
				if ( ch == L"х" ) ch = L"x";
			}

			j = j + ch;
		}
	}
    else 
    {
        j = text;
    }
    return j;
}
String  c_ii::get_alpha_keys(String text)
{
	text = text.LowerCase();
	String j = "";

	if ( f->ch_ALPHAKEYS->Checked )
	{
		for ( int c = 1; c <= text.Length(); c++ )
		{
			String ch = text[c];

			if ( ch == L"а" ) ch = L"a";
			if ( ch == L"е" ) ch = L"e";
			if ( ch == L"о" ) ch = L"o";
			if ( ch == L"р" ) ch = L"p";
			if ( ch == L"с" ) ch = L"c";
			if ( ch == L"у" ) ch = L"y";
			if ( ch == L"х" ) ch = L"x";

			j = j + ch;
		}
	}
	else 
    {
        j = text;
    }

	return j;
}
String  c_ii::MakePostLine(String post_variables_line)
{
	f->main->log("POST : [ "+post_variables_line+" ]");

	String result = "";

	int long_memory_index = 0;
	int long_upper_count  = 0;

	bool tempBlockInProcess = false;
	String  tempBufer = "";
	for ( int c = 1; c <= post_variables_line.Length(); c++ )
	{
		String ch = post_variables_line[c];
		if ( tempBlockInProcess == true )
		{
			if ( ch == ")" )
			{
				tempBlockInProcess = false;
				result = result + MakePostLineGetOne(tempBufer,long_upper_count,&long_memory_index);
#if 0                
				long_upper_count--;
                if ( long_upper_count == 0 ) 
                {
                    long_memory_index = 0;
                }

				try
				{
					String chn = post_variables_line[c+1];
                    int chc = 0;
                    if(TryStrToInt(post_variables_line[c+2], chc) == true &&
                       chn == "^")
                    {
                        long_upper_count = chc;
                        c += 2; 
                    }
				}
				catch (...) 
                { 
                }
#endif
			}
			else
			{
				tempBufer = tempBufer + ch;
			}
		}
		else
		{
			if ( ch == "(" )
			{
				tempBlockInProcess = true;
                tempBufer = "";
			}
			else
			{
				result = result + ch;
			}
		}
    }
	return result;
}
String  c_ii::MakePostLineGetOne(String buffer, int uppercount, int *memory_j)
{
	int sc = 0;
	for ( int c = 1; c < buffer.Length(); c++ )
	{
		String ch = buffer[c];
		if ( ch == "|" ) 
        {
            sc++;
        }
	}

	int parts = sc + 1;
	String j = "NULL";

	if ( uppercount > 0 && *memory_j > 0 )
	{
		j = g.Encrypt(*memory_j,parts,"|",buffer);
	}
	else
	{
		int r = Random( parts ) + 1;
		*memory_j = r;
		j = g.Encrypt(r,parts,"|",buffer);
    }

	return j;
}
bool c_ii::if_EqualPosts(String ModelPost, String MyPost)
{                                                                        
	bool eq = false;

    std::auto_ptr<TStringList> JPOSTVARIABLES(new TStringList);
	JPOSTVARIABLES->Add("");

	String  buff = "";

	bool w = false;
	for ( int c = 1; c <= ModelPost.Length(); c++ )
	{
		String ch = ModelPost[c];

		if ( w )
		{
			if ( ch == ")" )
			{
				w = false;
				MakePostLineGetAll(buff,JPOSTVARIABLES.get());

                if(ModelPost.Length() > (c+1))
                {
                    String chn = ModelPost[c+1];
                    if ( chn == "^" )
                    { 
                        c++; 
                        c++;
                    }
                }
			}
			else
			{
				buff = buff + ch;
			}
		}
		else
		{
			if ( ch == "(" )
			{
				w = true;
                buff = "";
			}
			else
			{
				for ( int x = 0; x < JPOSTVARIABLES->Count; x++ )
				{
                    JPOSTVARIABLES->Strings[x] = JPOSTVARIABLES->Strings[x] + ch;
                }
			}
		}
	}

	for ( int i = 0; i < JPOSTVARIABLES->Count; i++ )
	{
		String l = JPOSTVARIABLES->Strings[i];
		String f1 = CutMediaTags( l );
        f1 = f1.LowerCase();
		String f2 = MyPost.LowerCase();

		if ( f1.Length() > 0 && f2.Length() > 0 )
		{
			if ( f1 == f2 )
			{
				eq = true;
				f->main->log(L"Найдено соответствие [ "+MyPost+" ]");
				break;
			}
		}
    }
	return eq;
}
void c_ii::MakePostLineGetAll(String buffer, TStringList *JPOSTVARIABLES_A)
{
	int sc = 0;
	for ( int c = 1; c < buffer.Length(); c++ )
	{
		String ch = buffer[c];
		if ( ch == "|" ) 
        {
            sc++;
        }
	}
	int parts = sc + 1;

    std::auto_ptr<TStringList> JPOSTVARIABLES_D(new TStringList);
	for ( int r = 1;  r <= parts; r++ )
	{
		String j = g.Encrypt(r,parts,"|",buffer);

		for ( int c = 0; c < JPOSTVARIABLES_A->Count; c++ )
		{
			JPOSTVARIABLES_D->Add( JPOSTVARIABLES_A->Strings[c] + j );
        }
	}

	JPOSTVARIABLES_A->Text = JPOSTVARIABLES_D->Text;
}
void c_ii::WritePostDataIn(TStringList *IMAGELIST, TStringList *AUDIOLIST, TStringList *RECORDLIST, String *postline)
{
	String pl = *postline; 
    String fx; 
    int p;

	try
	{
		fx = "[IMAGE=";
		p = Pos(fx,pl);
		if ( p != 0 )
		{
			String L = pl.SubString(1,p-1);
			String R = pl.SubString(p,pl.Length());
			p = Pos("]",R);
			String M = R.SubString(1,p);
			String X = R.SubString(p+1,R.Length());
			pl = L + X;
			IMAGELIST->Add(M);
		}
	}
	catch (...) 
    { 
        f->main->log("c_ii::WritePostDataIn(catch:IMAGE)"); 
    }

	try
	{
		fx = "[AUDIO=";
		p = Pos(fx,pl);
		if ( p != 0 )
		{
			String L = pl.SubString(1,p-1);
			String R = pl.SubString(p,pl.Length());
			p = Pos("]",R);
			String M = R.SubString(1,p);
			String X = R.SubString(p+1,R.Length());
			pl = L + X;
			AUDIOLIST->Add(M);
		}
    }
	catch (...) 
    { 
        f->main->log("c_ii::WritePostDataIn(catch:AUDIO)"); 
    }

	try
	{
		fx = "[RECORD=";
		p = Pos(fx,pl);
		if ( p != 0 )
		{
			String L = pl.SubString(1,p-1);
			String R = pl.SubString(p,pl.Length());
			p = Pos("]",R);
			String M = R.SubString(1,p);
			String X = R.SubString(p+1,R.Length());
			pl = L + X;
			RECORDLIST->Add(M);
		}
    }
	catch (...) 
    { 
        f->main->log("c_ii::WritePostDataIn(catch:RECORD)"); 
    }

	*postline = pl;
}
void c_ii::ShowDataLNS(TStringList *IMAGELIST, TStringList *AUDIOLIST, TStringList *RECORDLIST,TStringList *POSTS)
{
	for ( int c = 0; c < POSTS->Count; c++ )
	{
		f->main->log( "POSTS_"+POSTS->Strings[c]+"_" );
	}

	for ( int c = 0; c < IMAGELIST->Count; c++ )
	{
		f->main->log( "IMAGELIST_"+IMAGELIST->Strings[c]+"_" );
	}

	for ( int c = 0; c < AUDIOLIST->Count; c++ )
	{
		f->main->log( "AUDIOLIST_"+AUDIOLIST->Strings[c]+"_" );
	}

	for ( int c = 0; c < RECORDLIST->Count; c++ )
	{
		f->main->log( "RECORDLIST_"+RECORDLIST->Strings[c]+"_" );
	}
}
void c_ii::ShowDataLNS(TStringList *IMAGELIST, TStringList *AUDIOLIST, TStringList *RECORDLIST)
{

	for ( int c = 0; c < IMAGELIST->Count; c++ )
	{
		f->main->log( "IMAGELIST_"+IMAGELIST->Strings[c]+"_" );
	}

	for ( int c = 0; c < AUDIOLIST->Count; c++ )
	{
		f->main->log( "AUDIOLIST_"+AUDIOLIST->Strings[c]+"_" );
	}

	for ( int c = 0; c < RECORDLIST->Count; c++ )
	{
		f->main->log( "RECORDLIST_"+RECORDLIST->Strings[c]+"_" );
	}
}
String  c_ii::CutMediaTags(String data)
{
	String fx1 = "[IMAGE=";
	String fx2 = "[AUDIO=";
	String fx3 = "[RECORD=";

	int p1 = Pos( fx1, data );
	int p2 = Pos( fx2, data );
	int p3 = Pos( fx3, data );

	if ( p1 != 0 || p2 != 0 || p3 != 0 )
	{
		int x = 99999999;

		if ( p1 > 0 && p1 < x ) 
        {
            x = p1;
        }
		if ( p2 > 0 && p2 < x ) 
        {
            x = p2;
        }
		if ( p3 > 0 && p3 < x ) 
        {
            x = p3;
        }

		data = data.SubString(1,x-1);
	}

	return data;
}
// C_ONLINE
c_online::c_online()
{
	APIBUFFER = new TStringList;
}
void c_online::addtobuffer(String token)
{
	long int unix = f->main->getUnix();
	String s = unix;
	APIBUFFER->Add( token + "#" + s );
}
void c_online::processing()
{
	long int UNIX  = f->main->getUnix();
	int      IUNIX = UNIX;

	buffer_clear(IUNIX);

    std::auto_ptr<TStringList> L(new TStringList);
	g.GetFiles( f->main->p_robots, L.get() );

	for ( int c = 0; c < L->Count; c++ )
	{
		String GroupName, RobotName, Server_ID, Login, Password, Token, Activity, Online, LastOnline;  bool freeze;
		f->main->get_robotdata( c, &GroupName, &RobotName, &Server_ID, &Login, &Password, &Token, &Activity, &Online, &LastOnline, &freeze );

		if ( Online == "0" && Token != "NULL" )
		{
			if ( StrToInt(LastOnline)+540+Random(60) < IUNIX )
			{
				bool success = false;
				make_online(RobotName,Token,&success);
				if ( success )
				{
					String tprefix = f->main->PREFIX;
					f->main->PREFIX = L"Вечный онлайн : [ "+RobotName+" ] ";
					f->main->log(L"Статус 'online' установлен.");
					f->main->PREFIX = tprefix;

					String robotpath = f->main->p_robots + L->Strings[c] + "\\" + "Conf.ini";
                    std::auto_ptr<TMemIniFile> INI(new TMemIniFile( UnicodeString(robotpath), TEncoding::UTF8 ));
			        INI->WriteInteger(UnicodeString("ACCOUNT"), UnicodeString("lastonline"),  IUNIX);
                    INI->UpdateFile();
				}
				else
				{
					String tprefix = f->main->PREFIX;
					f->main->PREFIX = L"Вечный онлайн : [ "+RobotName+" ] ";
					f->main->log(L"Не удалось установить статус 'online'. ((");
					f->main->PREFIX = tprefix;
				}

				Sleep(400);
            }
		}

		if ( Online == "0" && Token == "NULL" )
		{
            String tprefix = f->main->PREFIX;
			f->main->PREFIX = L"Вечный онлайн : [ "+RobotName+" ] ";
			f->main->log(L"Необходимо получить токен!");
			f->main->PREFIX = tprefix;
		}
	}
}
void c_online::make_online(String robotname, String token, bool *success)
{
	String j = vk.set_online(success,token);
	if ( *success )
	{
		if ( Pos("error",j) != 0 ) 
        {
            *success = false;
        }
	}
}
void c_online::buffer_clear(int unix)
{
    std::auto_ptr<TStringList> L(new TStringList);
	for ( int c = 0; c < APIBUFFER->Count; c++ )
	{
		String lastapi = g.Encrypt(2,2,"#",APIBUFFER->Strings[c]);
		if ( StrToInt(lastapi)+5 < unix ) 
        {
            L->Add( APIBUFFER->Strings[c] );
        }
	}

	APIBUFFER->Text = L->Text;
}
// C_VCL
c_vcl::c_vcl()
{

}
void c_vcl::obj_draw()
{
	g.SetColorToLabel( f->l_tinterval, "0x0095640D" );
	f->l_tinterval->Alignment = taCenter;
	f->l_tinterval->Width = 63;

	f->CH_LOG->Font->Color = StringToColor( "0x0095640D" );

	g.SetColorToLabel( f->Label5, "0x0095640D" );
	g.SetColorToLabel( f->Label6, "0x0095640D" );
	g.SetColorToLabel( f->Label7, "0x0095640D" );
	g.SetColorToLabel( f->Label2, "0x0095640D" );
	g.SetColorToLabel( f->Label4, "0x0095640D" );
	g.SetColorToLabel( f->Label10, "0x0095640D" );
	g.SetColorToLabel( f->Label17, "0x0095640D" );

	g.SetColorToLabel( f->l_NextClientID, "0x0095640D" );
    g.SetColorToLabel( f->lbl_DATAFROM, "0x0095640D" );
	g.SetColorToLabel( f->l_GID, "0x0095640D" );
}
void c_vcl::set()
{
	f->main->show_current_server();
}
void c_vcl::unset()
{
	set_enabled_conf_groups( false );
	set_enabled_conf_robots( false );
	set_enabled_dialogs( false );
}
void c_vcl::set_enabled_conf_groups( bool enable )
{
	// Pages
	for ( int c = 1; c < f->PAGES_CONFIGURATION->PageCount; c++ )
	{
		f->PAGES_CONFIGURATION->Pages[c]->Enabled = enable;
	}

	// Edit Group
	f->sGroupBox5->Enabled = enable;
	f->e_conf_groups_edit_name->Enabled = enable;
	f->b_CONF_GROUPS_EDIT_APPLY->Enabled = enable;
	f->Label10->Enabled = enable;

	f->e_conf_groups_edit_name->Clear();
}
void c_vcl::set_enabled_conf_robots( bool enable )
{
	// Edit Group
	f->sGroupBox17->Enabled = enable;
	f->e_conf_robots_edit_name->Enabled = enable;
	f->e_conf_robots_edit_login->Enabled = enable;
	f->e_conf_robots_edit_password->Enabled = enable;
	f->cb_conf_robots_edit_groups->Enabled = enable;
	f->ch_robot_online_edit->Enabled = enable;

	f->b_CONF_ROBOTS_EDIT_APPLY->Enabled = enable;

	f->Label40->Enabled = enable;
	f->Label41->Enabled = enable;
	f->Label42->Enabled = enable;
	f->Label11->Enabled = enable;
	f->Label23->Enabled = enable;

	f->e_conf_robots_edit_name->Clear();
	f->e_conf_robots_edit_login->Clear();
	f->e_conf_robots_edit_password->Clear();

	f->cb_conf_robots_edit_groups->ItemIndex = -1;

	f->ch_robot_online_edit->ItemIndex = -1;
}
void c_vcl::set_enabled_dialogs( bool enable )
{
	f->GROUP_DIALOG->Enabled = enable;
	f->sGroupBox10->Enabled = enable;

	f->l_GID->Caption   = L"Нет данных.";

	f->B_SENDMESSAGE->Enabled = enable;

	f->ME_DIALOG->Lines->Clear();       
    f->ME_DIALOG->Lines->Add("");
	f->ME_INPUT->Lines->Clear();
}
void c_vcl::groupechoReadRobots()
{
	for ( int c = 0; c < f->LV_CONF_GROUPS->Items->Count; c++ )
	{
		String NeededGroupName = Trim(f->LV_CONF_GROUPS->Items->Item[c]->Caption);

		int Count = 0;
        std::auto_ptr<TStringList> L(new TStringList);
		g.GetFiles( f->main->p_robots, L.get() );
		for ( int x = 0; x < L->Count; x++ )
		{
			String GroupName, RobotName, Server_ID, Login, Password, Token, Activity, Online, LastOnline;  
            bool freeze;
			f->main->get_robotdata( x, &GroupName, &RobotName, &Server_ID, &Login, &Password, &Token, &Activity, &Online, &LastOnline, &freeze );

			if ( GroupName == NeededGroupName ) 
            {
                Count++;
            }
		}
		f->LV_CONF_GROUPS->Items->Item[c]->SubItems->Strings[0] = IntToStr(Count);
    }
}
void c_vcl::groupechoReadUsers()
{
    std::auto_ptr<TStringList> L(new TStringList);
    if(FileExists(f->main->f_users))
    {
	    L->LoadFromFile( f->main->f_users );
    }

	for ( int c = 0; c < f->LV_CONF_GROUPS->Items->Count; c++ )
	{
		String NeededGroupName = Trim(f->LV_CONF_GROUPS->Items->Item[c]->Caption);
		int Count = 0;
		for ( int x = 0; x < L->Count; x++ )
		{
			String UserGroup = g.Encrypt(1,5,"#",L->Strings[x]);

			if ( UserGroup == NeededGroupName ) 
            {
                Count++;
            }
		}

		f->LV_CONF_GROUPS->Items->Item[c]->SubItems->Strings[1] = IntToStr(Count);
	}
}
void c_vcl::Dialog_Add_User(String User)
{
	f->ME_DIALOG->SelStart = f->ME_DIALOG->Lines->Text.Length();
	f->ME_DIALOG->SelAttributes->Size = 8;
	f->ME_DIALOG->SelAttributes->Style = TFontStyles() << fsBold;
	f->ME_DIALOG->SelAttributes->Name = "consolas";
	f->ME_DIALOG->SelAttributes->Color = StringToColor("0x0095640D");

	f->ME_DIALOG->Lines->Add( User );

	::SendMessage(f->ME_DIALOG->Handle,WM_VSCROLL,SB_BOTTOM,0);
}
void c_vcl::Dialog_Add_Text(TStringList *TEXT)
{
	for ( int c = 0; c < TEXT->Count; c++ )
	{
		String textline = TEXT->Strings[c];

        f->ME_DIALOG->SelStart = f->ME_DIALOG->Lines->Text.Length();
		f->ME_DIALOG->SelAttributes->Size = 8;
		f->ME_DIALOG->SelAttributes->Name = "consolas";
		f->ME_DIALOG->SelAttributes->Color = StringToColor("0x00444444");

		f->ME_DIALOG->Lines->Add( textline );
	}

	::SendMessage(f->ME_DIALOG->Handle,WM_VSCROLL,SB_BOTTOM,0);
}
void c_vcl::Dialog_Add_Date(String Date)
{
	f->ME_DIALOG->SelStart = f->ME_DIALOG->Lines->Text.Length();
	f->ME_DIALOG->SelAttributes->Size = 8;
	f->ME_DIALOG->SelAttributes->Name = "consolas";
	f->ME_DIALOG->SelAttributes->Color = StringToColor("0x00777777");

	f->ME_DIALOG->Lines->Add( "- - - - - - -  "+Date+"  - - - - - -" );

	f->ME_DIALOG->Lines->Add( "" );

	::SendMessage(f->ME_DIALOG->Handle,WM_VSCROLL,SB_BOTTOM,0);
}
void c_vcl::EchoStatistic()
{
	f->l_CNT_HELLO->Caption = IntToStr( f->main->CNT_HELLO );
	f->l_CNT_ADDFRIEND->Caption = IntToStr( f->main->CNT_ADDFRIEND );
	f->l_CNT_MESSAGES->Caption = IntToStr( f->main->CNT_MESSAGES );
	f->l_CNT_CONFIRMFRIENDS->Caption = IntToStr( f->main->CNT_CONFIRMFRIEND );

	g.ProcessMessages();
}
String  c_vcl::FixInbox(String data)
{
	int x = Pos("(",data);
	if ( x != 0 ) 
    {
        data = data.SubString(1,x-1);
    }

	return Trim(data);
}
void c_vcl::ModelsSET(bool set)
{
	f->E_MODEL_LOGICAL_POSTS_ADD->Enabled = set;
    f->B_MODEL_LOGICAL_POSTS_ADD->Enabled = set;
    f->CB_MODEL_LOGICAL_IFVARIANTS->Enabled = set;
    f->E_MODEL_LOGICAL_VARIANTS_ADD->Enabled = set;
    f->B_MODEL_LOGICAL_VARIANTS_ADD->Enabled = set;
    f->CB_MODEL_LOGICAL_EXTENDED->Enabled = set;
    f->E_MODEL_LOGICAL_EXTENDED_ADD->Enabled = set;
    f->B_MODEL_LOGICAL_EXTENDED_ADD->Enabled = set;
    f->CB_MODEL_LOGICAL_DEFAULT->Enabled = set;
    f->E_MODEL_LOGICAL_STAGE->Enabled = set;
    f->B_MODEL_LOGICAL_ADDEDIT->Enabled = set;
    f->B_MODEL_LOGICAL_VARIANTS_APPLY->Enabled = set;
	f->B_MODEL_LOGICAL_EXTENDED_APPLY->Enabled = set;

	if ( set )
	{
		f->LI_MODEL_LOGICAL_POSTS->PopupMenu    = f->PM_POSTS;
		f->LI_MODEL_LOGICAL_VARIANTS->PopupMenu = f->PM_VARIANTS;
		f->LI_MODEL_LOGICAL_EXTENDED->PopupMenu = f->PM_EXTENDED;
	}
	else
	{
		f->LI_MODEL_LOGICAL_POSTS->PopupMenu    = NULL;
		f->LI_MODEL_LOGICAL_VARIANTS->PopupMenu = NULL;
		f->LI_MODEL_LOGICAL_EXTENDED->PopupMenu = NULL;
    }

    if ( set ) 
    {
        f->TREE->Enabled = false;
    }
    else
    {
    	f->TREE->Enabled = true;
        f->GB_MODEL->Caption = L" Выделенное звено ";
    }
}
void c_vcl::GoToModels(String RobotName, int PositionView)
{
	f->PAGES->ActivePageIndex = 6;
	if ( PositionView == 0 ) 
    { 
        f->RB_MODELS_A->Checked = true; 
        f->RB_MODELS_B->Checked = false; 
        f->RB_MODELS_C->Checked = false; 
        f->RB_MODELS_D->Checked = false; 
    }
	if ( PositionView == 1 ) 
    { 
        f->RB_MODELS_A->Checked = false; 
        f->RB_MODELS_B->Checked = true; 
        f->RB_MODELS_C->Checked = false; 
        f->RB_MODELS_D->Checked = false; 
    }
	if ( PositionView == 2 ) 
    { 
        f->RB_MODELS_A->Checked = false; 
        f->RB_MODELS_B->Checked = false; 
        f->RB_MODELS_C->Checked = true;
        f->RB_MODELS_D->Checked = false; 
    }
	if ( PositionView == 3 ) 
    { 
        f->RB_MODELS_A->Checked = false; 
        f->RB_MODELS_B->Checked = false; 
        f->RB_MODELS_C->Checked = false; 
        f->RB_MODELS_D->Checked = true; 
    }

    f->PAGES_MODELS_PAGEVIEW->ActivePageIndex = PositionView;
    
    std::auto_ptr<TStringList> L(new TStringList);
    g.GetFiles( f->main->p_robots, L.get() );
    for ( int c = 0; c < L->Count; c++ )
    {
		String GroupName, cRobotName, Server_ID, Login, Password, Token, Activity, Online, LastOnline;  
        bool freeze;
		f->main->get_robotdata( c, &GroupName, &cRobotName, &Server_ID, &Login, &Password, &Token, &Activity, &Online, &LastOnline, &freeze );

        if ( RobotName == cRobotName )
		{
			f->CB_MODELS_ROBOTS->ItemIndex = c;
			f->CB_MODELS_ROBOTSChange(new TObject());
			break;
        }
    }
}
void c_vcl::ModelStageClear()
{
    f->main->POSTS->Clear();
    f->main->VARIANTS->Clear();
    f->main->IFVARIANTS->Clear();
    f->main->EXTENDED->Clear();
    f->main->DEFAULT->Clear();

    f->LI_MODEL_LOGICAL_POSTS->Items->Clear();
    f->LI_MODEL_LOGICAL_VARIANTS->Items->Clear();
    f->LI_MODEL_LOGICAL_EXTENDED->Items->Clear();
    f->E_MODEL_LOGICAL_STAGE->Clear();

    f->CB_MODEL_LOGICAL_IFVARIANTS->Items->Clear();
    f->CB_MODEL_LOGICAL_EXTENDED->Items->Clear();
    f->CB_MODEL_LOGICAL_DEFAULT->Items->Clear();
}
void c_vcl::GetAllStages(TsComboBox *CB)
{
    CB->Clear();

    for ( int c = 0; c < f->main->MODEL_LOGICAL->Count; c++ )
    {
        String data = f->main->MODEL_LOGICAL->Strings[c];
        if ( Pos("#",data) != 0 ) 
        {
            CB->Items->Add( data );
        }
    }
}
void c_vcl::GetAllRobots(TsComboBox *CB)
{
    CB->Items->Clear();

    std::auto_ptr<TStringList> L(new TStringList);
	g.GetFiles( f->main->p_robots, L.get() );
    for ( int c = 0; c < L->Count; c++ )
    {
		String GroupName, RobotName, Server_ID, Login, Password, Token, Activity, Online, LastOnline;  
        bool freeze;
		f->main->get_robotdata( c, &GroupName, &RobotName, &Server_ID, &Login, &Password, &Token, &Activity, &Online, &LastOnline, &freeze );

        CB->Items->Add( RobotName );
    }
}
void c_vcl::InputCaptchaOpenForm(String file)
{
    f->PAGES->Enabled = false;
	f->i_INPUTCAPCHA->Picture->LoadFromFile( file );
    f->e_INPUTCAPCHA->Clear();
	f->p_INPUTCAPTCHA->Left = 292;
	f->p_INPUTCAPTCHA->Top  = 140;
	f->p_INPUTCAPTCHA->Visible = true;
	f->p_INPUTCAPTCHA->BringToFront();

    Application->ProcessMessages();
}
// C_CAPTCHA
c_captcha::c_captcha()
{
    WAITMANUAL = false;
}
String  c_captcha::GetAnswer( String CaptchaIMGURL )
{
	String ANS = "NULL";

	try
	{
		CaptchaIMGURL = FixURL( CaptchaIMGURL );
		String CaptchaFILE = SaveToFile(CaptchaIMGURL);

		if ( f->rg_CAPTCHA_SERVICE->ItemIndex == 1 ) // AUTO
		{
			ANS = g.GetCaptchaRucaptcha1(CaptchaFILE,f->e_CAPTCHA_KEY->Text);

			while ( Pos("ERROR",ANS) != 0 )
			{
				f->main->log("ATTEMPT. RUCAPTCHA UNDER CYCLE. RET: "+ANS);
				ANS = g.GetCaptchaRucaptcha1(CaptchaFILE,f->e_CAPTCHA_KEY->Text);
			}
		}

		if ( f->rg_CAPTCHA_SERVICE->ItemIndex == 0 ) // MANUAL
		{
			WAITMANUAL = true;

			if ( f->ch_soundcaptcha->Checked ) 
            {
                f->main->soundplay_captha();
            }

			f->vcl->InputCaptchaOpenForm( CaptchaFILE );

			while ( WAITMANUAL )
			{
				Sleep(100);
				Application->ProcessMessages();
			}

			ANS = Trim( f->e_INPUTCAPCHA->Text );
		}

		DeleteFile( CaptchaFILE );
	}
	catch ( Exception *ex )
	{
		f->main->log("E:CAPTCHA::GetAnswer...ex:"+ex->Message);
    }

	return ANS;
}
String  c_captcha::FixURL( String Url )
{
	String j = "";
	for ( int c = 1; c <= Url.Length(); c++ )
	{
		String ch = Url[c];
		if ( ch != "\\" ) 
        {
            j = j + ch;
        }
	}
	return j;
}
String  c_captcha::SaveToFile( String Url )
{
	String file = g.GetDirectoryApplicationDatapath()+"captcha.jpg";

    std::auto_ptr<TMemoryStream> ms(new TMemoryStream);
    std::auto_ptr<TIdHTTP> HTTP(new TIdHTTP(NULL));
	HTTP->HandleRedirects = true;

	HTTP->Get( Url, ms.get() );

	ms->Position = 0;
	ms->SaveToFile( file );
	return file;
}
// TRAY
void __fastcall Tf::TRAYClick(TObject *Sender)
{
	 f->Show();
	 T_OBJ_DRAW->Enabled = true;
}
// TIMER
void __fastcall Tf::T_OBJ_DRAWTimer(TObject *Sender)
{
	T_OBJ_DRAW->Enabled = false;
	vcl->obj_draw();
}
void __fastcall Tf::TINTERVALTimer(TObject *Sender)
{
	main->TINT_CURR++;

	if ( main->TINT_CURR >= StrToInt(e_int_timer_max->Text) )
	{
		main->TINT_CURR = 0;
		TINTERVAL->Enabled = false;
		b_PROCESS_TWO_START->Click();
	}

	l_tinterval->Caption = StrToInt(e_int_timer_max->Text) - main->TINT_CURR;

	g.SetColorToLabel( l_tinterval, "0x0095640D" );
	l_tinterval->Alignment = taCenter;
    l_tinterval->Width = 63;

	g.ProcessMessages();
}
// FASTCALLS
void __fastcall Tf::b_PROCESS_TWO_STARTClick(TObject *Sender)
{
	b_PROCESS_TWO_START->Enabled = false;
	b_PROCESS_TWO_STOP->Enabled = true;

	g.ProcessMessages();

	TINTERVAL->Enabled = false;
	main->TINT_CURR = 0;

	if ( CH_LOGGOTO->Checked ) 
    {
        PAGES->ActivePageIndex = 1;
    }

	main->iMyTHREAD_TWO = new MyTHREAD_TWO(false);
}
void __fastcall Tf::b_PROCESS_TWO_STOPClick(TObject *Sender)
{
	main->TERMINATED = true;

	b_PROCESS_TWO_STOP->Enabled = false;
	g.ProcessMessages();
}
void __fastcall Tf::rg_CAPTCHA_SERVICEChange(TObject *Sender)
{
	if ( LOADED )
	{
		main->conf_captcha( true );
    }
}
void __fastcall Tf::ch_soundcaptchaClick(TObject *Sender)
{
	if ( LOADED )
	{
		main->conf_captcha( true );
	}
}
void __fastcall Tf::b_SERVERS_ADDClick(TObject *Sender)
{
	if ( e_servers_client_id->Text.Length() > 0 && 
         e_servers_login->Text.Length() > 0 && 
         e_servers_password->Text > 0 )
	{
		TListItem *ListItem;
		ListItem = f->LV_SERVERS->Items->Add();
		ListItem->Caption = " "+IntToStr( f->LV_SERVERS->Items->Count+1 );
		ListItem->SubItems->Add( e_servers_client_id->Text );
		ListItem->SubItems->Add( e_servers_login->Text );
		ListItem->SubItems->Add( e_servers_password->Text );

		main->conf_servers( true );
		main->conf_servers( false );

		e_servers_client_id->Clear();
		e_servers_login->Clear();
		e_servers_password->Clear();
	}
	else
	{
		g.Sm(L"Пожалуйста, заполните все обязательные поля.");
    }
}
void __fastcall Tf::b_CAPTCHA_GOTOPROFILEClick(TObject *Sender)
{
	String url;
	if ( rg_CAPTCHA_SERVICE->ItemIndex == 0 ) 
    {
        url = "https://rucaptcha.com/setting";
    }
	if ( rg_CAPTCHA_SERVICE->ItemIndex == 1 ) 
    {
        url = "https://anti-captcha.com/panel/reports/dashboard";
    }

	g.ShellExecute1( url );
}
void __fastcall Tf::b_CONF_GROUPS_ADDClick(TObject *Sender)
{
	if ( Trim(e_conf_groups_create_name->Text).Length() > 0 )
	{
		TListItem *ListItem;
		ListItem = f->LV_CONF_GROUPS->Items->Add();
		ListItem->Caption = " "+e_conf_groups_create_name->Text;
		ListItem->SubItems->Add( "0" );
		ListItem->SubItems->Add( "0" );

		e_conf_groups_create_name->Clear();
		main->conf_groups( true );
	}
}
void __fastcall Tf::LV_CONF_GROUPSChange(TObject *Sender, TListItem *Item, TItemChange Change)
{
	if ( LV_CONF_GROUPS->ItemIndex == -1 ) 
    {
        vcl->set_enabled_conf_groups(false);
    }
	else               					   
    {
        vcl->set_enabled_conf_groups(true);
    }
}
void __fastcall Tf::b_CONF_GROUPS_EDIT_APPLYClick(TObject *Sender)
{
	if ( e_conf_groups_edit_name->Text.Length() > 0 )
	{
		LV_CONF_GROUPS->Items->Item[LV_CONF_GROUPS->ItemIndex]->Caption = " "+e_conf_groups_edit_name->Text;
		main->conf_groups( true );
        vcl->set_enabled_conf_groups( false );
	}
}
void __fastcall Tf::LV_CONF_GROUPSClick(TObject *Sender)
{
	int index = LV_CONF_GROUPS->ItemIndex;
	if ( index != -1 )
	{
		vcl->set_enabled_conf_groups(true);

		String name  = Trim(LV_CONF_GROUPS->Items->Item[index]->Caption);
		e_conf_groups_edit_name->Text = name;

		main->conf_robots( index, false );
		main->conf_users( index, false );
    }
}
void __fastcall Tf::LV_CONF_ROBOTSChange(TObject *Sender, TListItem *Item, TItemChange Change)
{
	if ( LV_CONF_ROBOTS->ItemIndex == -1 ) 
    {
        vcl->set_enabled_conf_robots(false);
    }
	else               					   
    {
        vcl->set_enabled_conf_robots(true);
    }
}
void __fastcall Tf::LV_CONF_ROBOTSClick(TObject *Sender)
{
	int index = LV_CONF_ROBOTS->ItemIndex;
	if ( index != -1 )
	{
		int cbIndex = -1;
		String UsesGroupName = Trim(LV_CONF_GROUPS->Items->Item[LV_CONF_GROUPS->ItemIndex]->Caption);
		std::auto_ptr<TStringList> G(new TStringList);
		for ( int c = 0; c < LV_CONF_GROUPS->Items->Count; c++ )
		{
			String tGroup = Trim(LV_CONF_GROUPS->Items->Item[c]->Caption);
			if ( UsesGroupName == tGroup ) 
            {
                cbIndex = c;
            }
			G->Add( tGroup );
		}
		cb_conf_robots_edit_groups->Items->Text = G->Text;
		cb_conf_robots_edit_groups->ItemIndex = cbIndex;

		std::auto_ptr<TStringList> L(new TStringList);
		g.GetFiles( main->p_robots, L.get() );
		for ( int c = 0; c < L->Count; c++ )
		{
			String GroupName, RobotName, Server_ID, Login, Password, Token, Activity, Online, LastOnline;  
            bool freeze;
			main->get_robotdata( c, &GroupName, &RobotName, &Server_ID, &Login, &Password, &Token, &Activity, &Online, &LastOnline, &freeze );

			if ( RobotName == Trim(LV_CONF_ROBOTS->Items->Item[index]->Caption) )
			{
				e_conf_robots_edit_name->Text 		= RobotName;
				e_conf_robots_edit_login->Text 		= Login;
				e_conf_robots_edit_password->Text 	= Password;
				ch_robot_online_edit->ItemIndex = StrToInt(Online);

				if ( freeze )
				{
					N70->Enabled = false;   //g.Sm("1");
					N71->Enabled = true;
				}
				else
				{
					N70->Enabled = true;   //g.Sm("2");
					N71->Enabled = false;
				}
				break;
			}
		}
	}
	else
	{
		N70->Enabled = false;
		N71->Enabled = false;
    }
}
void __fastcall Tf::b_CONF_ROBOTS_CREATEClick(TObject *Sender)
{
	if ( e_conf_robots_create_name->Text.Length() > 0 && 
         e_conf_robots_create_login->Text.Length() > 0 && 
         e_conf_robots_create_password->Text.Length() > 0 )
	{
		main->conf_robots( LV_CONF_GROUPS->ItemIndex, true );
        vcl->groupechoReadRobots();
	}
	else 
    {
        g.Sm(L"Пожалуйста, заполните все обязательные поля.");
    }
}
void __fastcall Tf::b_CONF_ROBOTS_EDIT_APPLYClick(TObject *Sender)
{
	if ( e_conf_robots_edit_name->Text.Length() > 0 && 
         e_conf_robots_edit_login->Text.Length() > 0 && 
         e_conf_robots_edit_password->Text.Length() > 0 && 
         cb_conf_robots_edit_groups->ItemIndex != -1 )
	{
		String OldName = Trim(LV_CONF_ROBOTS->Items->Item[LV_CONF_ROBOTS->ItemIndex]->Caption);

		std::auto_ptr<TStringList> L(new TStringList);
		g.GetFiles( main->p_robots, L.get() );

		for ( int c = 0; c < L->Count; c++ )
		{
			String GroupName, RobotName, Server_ID, Login, Password, Token, Activity, Online, LastOnline;  
            bool freeze;
			main->get_robotdata( c, &GroupName, &RobotName, &Server_ID, &Login, &Password, &Token, &Activity, &Online, &LastOnline, &freeze );

			if ( RobotName == OldName )
			{
                String robotpath = main->p_robots + L->Strings[c] + "\\";
                main->checkDirectoryExisting(robotpath);
				std::auto_ptr<TMemIniFile> INI(new TMemIniFile( UnicodeString(robotpath + "Conf.ini"),TEncoding::UTF8 ));
				INI->WriteString( UnicodeString("MAIN"),    UnicodeString("owner"),   UnicodeString(Trim(cb_conf_robots_edit_groups->Items->Strings[cb_conf_robots_edit_groups->ItemIndex]))   );
				INI->WriteString( UnicodeString("MAIN"),    UnicodeString("name"),    UnicodeString(f->e_conf_robots_edit_name->Text) );
				INI->WriteString( UnicodeString("ACCOUNT"), UnicodeString("login"),   UnicodeString(f->e_conf_robots_edit_login->Text) );
				INI->WriteString( UnicodeString("ACCOUNT"), UnicodeString("password"),UnicodeString(f->e_conf_robots_edit_password->Text) );
                INI->WriteString( UnicodeString("ACCOUNT"), UnicodeString("online"),  UnicodeString(IntToStr( f->ch_robot_online_edit->ItemIndex) ) );
                INI->UpdateFile();

				main->conf_robots( LV_CONF_GROUPS->ItemIndex, false );
				vcl->set_enabled_conf_robots(false);
				main->conf_groups( false );
                PAGES_CONFIGURATION->ActivePageIndex = 0;
            }
		}
	}
	else 
    {
        g.Sm(L"Пожалуйста, заполните все обязательные поля.");
    }
}
void __fastcall Tf::b_CONF_USERS_SEARCHClick(TObject *Sender)
{
	String URL = e_conf_users_URL->Text;
	if ( URL.Length() > 10 && 
         Pos("vk.com/search",URL) > 0 && 
         Pos("=people",URL) > 0 )
	{
		main->countOfprocessed = 0;

		PAGES->ActivePageIndex = 4; g.ProcessMessages();

		try
		{
			std::auto_ptr<TIdHTTP> HTTP(new TIdHTTP(NULL));
			URL = HTTP->URL->URLDecode(URL);

			main->search_request( URL, e_conf_users_offSet->Text, e_conf_users_Count->Text, 1 );
		}
		catch ( Exception *ex )
		{
			main->log( "CONF_USERS_SEARCH CLICK :: ERROR!!!   :   " + ex->Message );
		}

        PAGES->ActivePageIndex = 2; g.ProcessMessages();
	}
	else 
    {
        g.Sm(L"Пожалуйста, введите корректный поисковый адрес.");
    }
}
void __fastcall Tf::LV_DIALOGSChange(TObject *Sender, TListItem *Item, TItemChange Change)
{
	if ( LV_DIALOGS->ItemIndex == -1 ) 
    {
        vcl->set_enabled_dialogs(false);
    }
	else               				   
    {
        vcl->set_enabled_dialogs(true);
    }
}
void __fastcall Tf::LV_DIALOGSClick(TObject *Sender)
{
	if ( LV_DIALOGS->ItemIndex == -1 ) 
    {
        vcl->set_enabled_dialogs(false);
    }
	else               				   
    {
        vcl->set_enabled_dialogs(true);
    }

	int index = LV_DIALOGS->ItemIndex;
	if ( index != -1 )
	{
		String id = Trim(LV_DIALOGS->Items->Item[index]->Caption);

        std::auto_ptr<TStringList> X(new TStringList);
		X->LoadFromFile( main->p_dialogs + id );

		String robot_gid 	 = X->Strings[0];
		String name 		 = X->Strings[1];
		String surname 	 = X->Strings[2];

		f->l_GID->Caption = robot_gid;

		main->DrawMessageBox(name,surname,robot_gid,X.get());
		main->SetAsRead( X.get() );

		X->SaveToFile( UnicodeString(main->p_dialogs + id), TEncoding::UTF8 );

		String inc = LV_DIALOGS->Items->Item[index]->SubItems->Strings[2];
		LV_DIALOGS->Items->Item[index]->SubItems->Strings[2] = vcl->FixInbox( inc );
	}
}

void __fastcall Tf::B_SENDMESSAGEClick(TObject *Sender)
{
	B_SENDMESSAGE->Enabled = false; g.ProcessMessages();

	if ( ME_INPUT->Lines->Text.Length() > 2 )
	{
		String id = Trim(LV_DIALOGS->Items->Item[LV_DIALOGS->ItemIndex]->Caption);

		std::auto_ptr<TStringList> L(new TStringList);
		g.GetFiles( main->p_robots, L.get() );

		for ( int c = 0; c < L->Count; c++ )
		{
			String GroupName, RobotName, Server_ID, Login, Password, Token, Activity, Online, LastOnline;  
            bool freeze;
			main->get_robotdata( c, &GroupName, &RobotName, &Server_ID, &Login, &Password, &Token, &Activity, &Online, &LastOnline, &freeze );

			if ( RobotName == Trim(l_GID->Caption) )
			{
				if ( proc->Establish(RobotName,&Token) )
				{
					String MESSID = proc->SendMessage( id, ME_INPUT->Lines->Text, Token );

					TDateTime D = Date();
					TDateTime T = Time();
					String DT = DateToStr(D) + " - " + TimeToStr(T);
					std::auto_ptr<TStringList> X(new TStringList);
					X->LoadFromFile( main->p_dialogs + id );
					X->Add("");
					X->Add("#MESSAGE");
					X->Add("#OUT");
					X->Add("#DATE:"+DT);
					X->Add("#NEW=0");
					X->Add("#ID="+MESSID);
					X->Add("#STAGEDATA=MANUALSEND");
					X->Add("#BEGIN");
					for ( int i = 0; i < ME_INPUT->Lines->Count; i++ ) X->Add( ME_INPUT->Lines->Strings[i] );
					X->Add("#END");
					X->SaveToFile(  main->p_dialogs + id );

					int DiaIndex = LV_DIALOGS->ItemIndex;
					main->conf_dialogs(false);
					LV_DIALOGS->ItemIndex = DiaIndex;
					vcl->set_enabled_dialogs(true);
					TObject *Sender;
					LV_DIALOGSClick(Sender);
				}

				break;
			}
		}
	}

	B_SENDMESSAGE->Enabled = true; 
    g.ProcessMessages();
}
void __fastcall Tf::e_int_apiChange(TObject *Sender)
{
	if ( LOADED )
	{
		main->conf_intervals( true );
	}
}
void __fastcall Tf::e_autoanswerlimitChange(TObject *Sender)
{
	if ( LOADED )
	{
		main->conf_ini( true );
	}
}
void __fastcall Tf::LV_WORKGROUPSClick(TObject *Sender)
{
	if ( LOADED ) 
    {
        main->conf_workgroups(true);
    }
}
void __fastcall Tf::LV_WORKTASKSClick(TObject *Sender)
{
	if ( LOADED ) 
    {
        main->conf_worktasks(true);
    }
}
void __fastcall Tf::LV_CONF_GROUPSDblClick(TObject *Sender)
{
	f->PAGES_CONFIGURATION->ActivePageIndex = 1;
}
void __fastcall Tf::TREEClick(TObject *Sender)
{
    LOADED = false;

	try
	{
		GB_MODEL->Caption = L" Выделенное звено ";
        if(TREE->Items->Count > 0)
        {
            String indexName = TREE->Selected->Text;
		    main->LoadModelStage( indexName );
        }
	}
	catch (...)
	{

    }

    LOADED = true;
}
void __fastcall Tf::TREEChange(TObject *Sender, TTreeNode *Node)
{
    main->NODE = Node;
}
void __fastcall Tf::CB_MODELS_ROBOTSChange(TObject *Sender)
{
	int index = CB_MODELS_ROBOTS->ItemIndex;

	if ( LOADED && index != -1 && 
         index < CB_MODELS_ROBOTS->Items->Count )
    {
		vcl->ModelStageClear();
		vcl->ModelsSET(false);

		main->LoadModel(index);
		main->MOD = false;
	}
}
void __fastcall Tf::RB_MODELS_AClick(TObject *Sender)
{
    if ( LOADED )
    {
    	PAGES_MODELS_PAGEVIEW->ActivePageIndex = 0;

		RB_MODELS_A->Checked = true;
		RB_MODELS_B->Checked = false;
		RB_MODELS_C->Checked = false;
		RB_MODELS_D->Checked = false;
	}
}
void __fastcall Tf::RB_MODELS_BClick(TObject *Sender)
{
    if ( LOADED )
    {
    	PAGES_MODELS_PAGEVIEW->ActivePageIndex = 1;

        RB_MODELS_B->Checked = true;
		RB_MODELS_A->Checked = false;
		RB_MODELS_C->Checked = false;
		RB_MODELS_D->Checked = false;
    }
}
void __fastcall Tf::RB_MODELS_CClick(TObject *Sender)
{
	if ( LOADED )
	{
		PAGES_MODELS_PAGEVIEW->ActivePageIndex = 2;

		RB_MODELS_A->Checked = false;
		RB_MODELS_B->Checked = false;
		RB_MODELS_C->Checked = true;
		RB_MODELS_D->Checked = false;
	}
}
void __fastcall Tf::RB_MODELS_DClick(TObject *Sender)
{
	if ( LOADED )
	{
		PAGES_MODELS_PAGEVIEW->ActivePageIndex = 3;

		RB_MODELS_A->Checked = false;
		RB_MODELS_B->Checked = false;
		RB_MODELS_C->Checked = false;
		RB_MODELS_D->Checked = true;
	}
}
void __fastcall Tf::B_MODEL_HELLO_ADDClick(TObject *Sender)
{
	if ( E_MODEL_HELLO_ADD->Text.Length() > 0 )
	{
        main->MODEL_HELLO->Add( E_MODEL_HELLO_ADD->Text );
		E_MODEL_HELLO_ADD->Clear();
        std::auto_ptr<TStringList> L(new TStringList);
        g.GetFiles( main->p_robots, L.get() );
		String file = main->p_robots + L->Strings[ CB_MODELS_ROBOTS->ItemIndex ] + "\\Hello.txt";
        
        main->MODEL_HELLO->SaveToFile( UnicodeString(file), TEncoding::UTF8 );
		main->LoadModelHello();
	}
}
void __fastcall Tf::B_MODEL_AUTOANS_ADDClick(TObject *Sender)
{
	if ( E_MODEL_AUTOANS_RULETEXT->Text.Length() > 0 && E_MODEL_AUTOANS_RULESTAGE->Text.Length() > 0 )
	{
		main->MODEL_AUTOANS->Add( E_MODEL_AUTOANS_RULETEXT->Text+"~"+E_MODEL_AUTOANS_RULESTAGE->Text );
		E_MODEL_AUTOANS_RULETEXT->Clear();
		E_MODEL_AUTOANS_RULESTAGE->Clear();
		std::auto_ptr<TStringList> L(new TStringList);
        g.GetFiles( main->p_robots, L.get() );
		String file = main->p_robots + L->Strings[ CB_MODELS_ROBOTS->ItemIndex ] + "\\AutoAnsRules.txt";
        
		main->MODEL_AUTOANS->SaveToFile( UnicodeString(file), TEncoding::UTF8 );
		main->LoadModelAutoAnsRules();
	}
}
void __fastcall Tf::B_MODEL_AUTOSTOP_KEYSClick(TObject *Sender)
{
	if ( E_MODEL_AUTOSTOP_KEYS->Text.Length() > 0 )
	{
		main->MODEL_AUTOSTOP_KEYS->Add( E_MODEL_AUTOSTOP_KEYS->Text );
		E_MODEL_AUTOSTOP_KEYS->Clear();
		std::auto_ptr<TStringList> L(new TStringList);
        g.GetFiles( main->p_robots, L.get() );
		String file = main->p_robots + L->Strings[ CB_MODELS_ROBOTS->ItemIndex ] + "\\AutoStopKeys.txt";
        
		main->MODEL_AUTOSTOP_KEYS->SaveToFile( UnicodeString(file), TEncoding::UTF8 );
		main->LoadModelAutoStopKeys();
	}
}
void __fastcall Tf::B_MODEL_AUTOSTOP_POSTSClick(TObject *Sender)
{
	if ( E_MODEL_AUTOSTOP_POSTS->Text.Length() > 0 )
	{
		main->MODEL_AUTOSTOP_POSTS->Add( E_MODEL_AUTOSTOP_POSTS->Text );
		E_MODEL_AUTOSTOP_POSTS->Clear();
		std::auto_ptr<TStringList> L(new TStringList);
        g.GetFiles( main->p_robots, L.get() );
		String file = main->p_robots + L->Strings[ CB_MODELS_ROBOTS->ItemIndex ] + "\\AutoStopPosts.txt";
        
		main->MODEL_AUTOSTOP_POSTS->SaveToFile( UnicodeString(file), TEncoding::UTF8 );
		main->LoadModelAutoStopPosts();
	}
}
void __fastcall Tf::B_MODEL_LOGICAL_ADDEDITClick(TObject *Sender)
{
	if ( LI_MODEL_LOGICAL_POSTS->Items->Count > 0 && E_MODEL_LOGICAL_STAGE->Text.Length() > 0 )
    {
		String StageName = main->CreateStageName(E_MODEL_LOGICAL_STAGE->Text);
        if ( ! main->ifStageNameExist(StageName) )
        {
    		B_MODEL_LOGICAL_ADDEDIT->Caption = "";

    		std::auto_ptr<TStringList> J(new TStringList);
            J->Add( StageName );
    		for ( int c = 0; c < main->POSTS->Count; c++ ) 		
            {
                J->Add( main->POSTS->Strings[c] );
            }
    		for ( int c = 0; c < main->VARIANTS->Count; c++ ) 	
            {
                J->Add( main->VARIANTS->Strings[c] );
            }
    		for ( int c = 0; c < main->IFVARIANTS->Count; c++ ) 
            {
                J->Add( main->IFVARIANTS->Strings[c] );
            }
    		for ( int c = 0; c < main->EXTENDED->Count; c++ ) 	
            {
                J->Add( main->EXTENDED->Strings[c] );
            }
    		if ( CB_MODEL_LOGICAL_DEFAULT->ItemIndex == -1 ) 	
            {
                J->Add( "DEFAULT'NULL" );
            }
    		else 												
            {
                J->Add( "DEFAULT'"+CB_MODEL_LOGICAL_DEFAULT->Items->Strings[CB_MODEL_LOGICAL_DEFAULT->ItemIndex].SubString(2,CB_MODEL_LOGICAL_DEFAULT->Items->Strings[CB_MODEL_LOGICAL_DEFAULT->ItemIndex].Length()) );
            }

            if ( main->IFCREATE ) 
            {
                main->ModelLogicalAddStage( J.get() );
            }
            else                  
            {
                main->ModelLogicalChangeStage( J.get() );
            }
            
            main->ModelSaveAndReloadInterface();
            main->MOD = false;
        }
        else
        {
        	g.Sm(L"Такое имя STAGE уже существует..");
        }
    }
    else
    {
		g.Sm(L"Введите варианты POSTS и имя текущего STAGE..");
    }
}
void __fastcall Tf::B_MODEL_LOGICAL_POSTS_ADDClick(TObject *Sender)
{
    if ( E_MODEL_LOGICAL_POSTS_ADD->Text.Length() > 0 )
    {
        LI_MODEL_LOGICAL_POSTS->Items->Add( E_MODEL_LOGICAL_POSTS_ADD->Text );
        main->POSTS->Add( "POST'"+E_MODEL_LOGICAL_POSTS_ADD->Text );
        E_MODEL_LOGICAL_POSTS_ADD->Clear();
    }
}
void __fastcall Tf::B_MODEL_LOGICAL_VARIANTS_ADDClick(TObject *Sender)
{
    if ( E_MODEL_LOGICAL_VARIANTS_ADD->Text.Length() > 0 )
    {
        LI_MODEL_LOGICAL_VARIANTS->Items->Add( E_MODEL_LOGICAL_VARIANTS_ADD->Text );

        String var = g.Its( Random(999999) );
        main->VARIANTS->Add( "VARIANT'"+var+"'"+E_MODEL_LOGICAL_VARIANTS_ADD->Text );

        if ( CB_MODEL_LOGICAL_IFVARIANTS->ItemIndex == -1 ) 
        {
            main->IFVARIANTS->Add( "IF'"+var+"'NULL");
        }
        else 												
        {
            main->IFVARIANTS->Add( "IF'"+var+"'"+CB_MODEL_LOGICAL_IFVARIANTS->Items->Strings[CB_MODEL_LOGICAL_IFVARIANTS->ItemIndex].SubString(2,CB_MODEL_LOGICAL_IFVARIANTS->Items->Strings[CB_MODEL_LOGICAL_IFVARIANTS->ItemIndex].Length()) );
        }

        E_MODEL_LOGICAL_VARIANTS_ADD->Clear();
        CB_MODEL_LOGICAL_IFVARIANTS->ItemIndex = -1;
    }
}
void __fastcall Tf::B_MODEL_LOGICAL_EXTENDED_ADDClick(TObject *Sender)
{
    if ( E_MODEL_LOGICAL_EXTENDED_ADD->Text.Length() > 0 )
    {
        LI_MODEL_LOGICAL_EXTENDED->Items->Add(E_MODEL_LOGICAL_EXTENDED_ADD->Text);

        if ( CB_MODEL_LOGICAL_EXTENDED->ItemIndex == -1 ) 
        {
            main->EXTENDED->Add("EXTENDED'"+ main->ConvertToVars(E_MODEL_LOGICAL_EXTENDED_ADD->Text) +"'NULL");
        }
        else                                              
        {
            main->EXTENDED->Add("EXTENDED'"+ main->ConvertToVars(E_MODEL_LOGICAL_EXTENDED_ADD->Text) +"'"+CB_MODEL_LOGICAL_EXTENDED->Items->Strings[CB_MODEL_LOGICAL_EXTENDED->ItemIndex].SubString(2,CB_MODEL_LOGICAL_EXTENDED->Items->Strings[CB_MODEL_LOGICAL_EXTENDED->ItemIndex].Length()) );
        }

        E_MODEL_LOGICAL_EXTENDED_ADD->Clear();
        CB_MODEL_LOGICAL_EXTENDED->ItemIndex = -1;
    }
}
void __fastcall Tf::LI_MODEL_LOGICAL_VARIANTSClick(TObject *Sender)
{
    int index = LI_MODEL_LOGICAL_VARIANTS->ItemIndex;

    if ( index != -1 )
    {
        String fx = main->IFVARIANTS->Strings[index];
        fx = fx.SubString(4,fx.Length());
        int p = Pos("'",fx);
        fx = fx.SubString(p+1,fx.Length());
        fx = "#" + fx;

		bool ex = false;
        for ( int c = 0; c < CB_MODEL_LOGICAL_IFVARIANTS->Items->Count; c++ )
        {
            String dataline = CB_MODEL_LOGICAL_IFVARIANTS->Items->Strings[c];
            if ( dataline == fx )
            {
				ex = true;
				CB_MODEL_LOGICAL_IFVARIANTS->ItemIndex = c;
                break;
            }
        }

		E_MODEL_LOGICAL_VARIANTS_ADD->Text = LI_MODEL_LOGICAL_VARIANTS->Items->Strings[index];
		if ( ! ex ) 
        {
            CB_MODEL_LOGICAL_IFVARIANTS->ItemIndex = -1;
        }
    }
}
void __fastcall Tf::LI_MODEL_LOGICAL_EXTENDEDClick(TObject *Sender)
{
    int index = LI_MODEL_LOGICAL_EXTENDED->ItemIndex;

    if ( index != -1 )
    {
        String fx = main->EXTENDED->Strings[index];
        fx = fx.SubString(10,fx.Length());
        int p = Pos("'",fx);
        fx = fx.SubString(p+1,fx.Length());
        fx = "#" + fx;

		bool ex = false;
        for ( int c = 0; c < CB_MODEL_LOGICAL_EXTENDED->Items->Count; c++ )
        {
            String dataline = CB_MODEL_LOGICAL_EXTENDED->Items->Strings[c];
            if ( dataline == fx )
			{
				ex = true;
				CB_MODEL_LOGICAL_EXTENDED->ItemIndex = c;
                break;
            }
        }

		E_MODEL_LOGICAL_EXTENDED_ADD->Text = LI_MODEL_LOGICAL_EXTENDED->Items->Strings[index];

		if ( ! ex ) 
        {
            CB_MODEL_LOGICAL_EXTENDED->ItemIndex = -1;
        }
	}
}
void __fastcall Tf::B_MODEL_LOGICAL_VARIANTS_APPLYClick(TObject *Sender)
{
	int index = LI_MODEL_LOGICAL_VARIANTS->ItemIndex;

    if ( index != -1 )
    {
        LI_MODEL_LOGICAL_VARIANTS->Items->Strings[index] = E_MODEL_LOGICAL_VARIANTS_ADD->Text;

        String var = main->GetVarFrom(index);

        main->VARIANTS->Delete(index);
        main->IFVARIANTS->Delete(index);

        main->VARIANTS->Insert(index,"VARIANT'"+var+"'"+E_MODEL_LOGICAL_VARIANTS_ADD->Text );

        if ( CB_MODEL_LOGICAL_IFVARIANTS->ItemIndex == -1 ) 
        {
            main->IFVARIANTS->Insert(index,"IF'"+var+"'NULL");
        }
        else 												
        {
            main->IFVARIANTS->Insert(index,"IF'"+var+"'"+CB_MODEL_LOGICAL_IFVARIANTS->Items->Strings[CB_MODEL_LOGICAL_IFVARIANTS->ItemIndex].SubString(2,CB_MODEL_LOGICAL_IFVARIANTS->Items->Strings[CB_MODEL_LOGICAL_IFVARIANTS->ItemIndex].Length()) );
        }
    }

    E_MODEL_LOGICAL_VARIANTS_ADD->Clear();
    CB_MODEL_LOGICAL_IFVARIANTS->ItemIndex = -1;
    LI_MODEL_LOGICAL_VARIANTS->ItemIndex = -1;
}
void __fastcall Tf::B_MODEL_LOGICAL_EXTENDED_APPLYClick(TObject *Sender)
{
    int index = LI_MODEL_LOGICAL_EXTENDED->ItemIndex;

    if ( index != -1 )
    {
		LI_MODEL_LOGICAL_EXTENDED->Items->Strings[index] = E_MODEL_LOGICAL_EXTENDED_ADD->Text;

        main->EXTENDED->Delete(index);

        if ( CB_MODEL_LOGICAL_EXTENDED->ItemIndex == -1 )
        {
            main->EXTENDED->Insert(index,"EXTENDED'"+ main->ConvertToVars(E_MODEL_LOGICAL_EXTENDED_ADD->Text) +"'NULL");
        }
        else                                              
        {
            main->EXTENDED->Insert(index,"EXTENDED'"+ main->ConvertToVars(E_MODEL_LOGICAL_EXTENDED_ADD->Text) +"'"+CB_MODEL_LOGICAL_EXTENDED->Items->Strings[CB_MODEL_LOGICAL_EXTENDED->ItemIndex].SubString(2,CB_MODEL_LOGICAL_EXTENDED->Items->Strings[CB_MODEL_LOGICAL_EXTENDED->ItemIndex].Length()) );
        }
    }

    E_MODEL_LOGICAL_EXTENDED_ADD->Clear();
    CB_MODEL_LOGICAL_EXTENDED->ItemIndex = -1;
    LI_MODEL_LOGICAL_EXTENDED->ItemIndex = -1;
}
void __fastcall Tf::CH_LOGClick(TObject *Sender)
{
	if ( LOADED )
	{
		main->conf_ini(true);
    }
}
void __fastcall Tf::E_MODEL_AUTOANS_DEFAULTChange(TObject *Sender)
{
	if ( E_MODEL_AUTOANS_DEFAULT->Text.Length() > 0 && LOADED )
	{
		std::auto_ptr<TStringList> X(new TStringList);
		X->Text = E_MODEL_AUTOANS_DEFAULT->Text;

		std::auto_ptr<TStringList> L(new TStringList);
		g.GetFiles( main->p_robots, L.get() );
		String file = main->p_robots + L->Strings[ CB_MODELS_ROBOTS->ItemIndex ] + "\\AutoAnsDefault.txt";
		X->SaveToFile( UnicodeString(file), TEncoding::UTF8 );
	}
}
void __fastcall Tf::sTabSheet9Show(TObject *Sender)
{
	if ( LOADED ) 
    {
        main->conf_dialogs(false);
    }
}
void __fastcall Tf::b_MODEL_IMPORTClick(TObject *Sender)
{
	SelectDirectory(L"Укажите папки с файлами модели.", "", main->f_od_import_robot_model);
	main->conf_ini(true);
	if ( main->f_od_import_robot_model != "NULL" && 
         Pos("\\",main->f_od_import_robot_model) != 0 )
	{
		String dir = main->f_od_import_robot_model + "\\";  // g.Sm(dir);
        std::auto_ptr<TStringList> SOURCEFILES(new TStringList);
		g.GetFiles( dir, SOURCEFILES.get() );

		for ( int c = 0; c < f->LV_WORKGROUPS->Items->Count; c++ )
		{
			if ( f->LV_WORKGROUPS->Items->Item[c]->Checked )
			{
				String NeededGroup = Trim(f->LV_WORKGROUPS->Items->Item[c]->Caption);

				std::auto_ptr<TStringList> L(new TStringList);
				g.GetFiles( f->main->p_robots, L.get() );
				String GroupName, RobotName, Server_ID, Login, Password, Token, Activity, ModelFile, Online, LastOnline;  
                bool freeze;
				for ( int x = 0; x < L->Count; x++ )
				{
					String CurrentRobotPath = f->main->p_robots + L->Strings[x] + "\\";
					f->main->get_robotdata( x, &GroupName, &RobotName, &Server_ID, &Login, &Password, &Token, &Activity, &Online, &LastOnline, &freeze );

					if ( GroupName == NeededGroup )
					{
						for ( int i = 0; i < SOURCEFILES->Count; i++ )
						{
							String src  = dir + SOURCEFILES->Strings[i];
							String dest = CurrentRobotPath + SOURCEFILES->Strings[i];

							CopyFile( src.c_str(), dest.c_str(), false );
						}

                        main->log( L"Импорт модели в [ " + RobotName + L" ] Готово." );
					}
				}
			}
		}

		PAGES->ActivePageIndex = 1;
    }
}
void __fastcall Tf::b_INPUTCAPCHAClick(TObject *Sender)
{
	captcha->WAITMANUAL = false;
	p_INPUTCAPTCHA->Visible = false;
	f->PAGES->Enabled = true;
	Application->ProcessMessages();
}
void __fastcall Tf::ch_dynamic_userbdataClick(TObject *Sender)
{
	if ( LOADED )
	{
		main->conf_ini(true);
	}
}
void __fastcall Tf::b_GlobalUsersCacheDublicatesDELClick(TObject *Sender)
{
	main->GlobalUsersCache_DublicatesDelete();
}
void __fastcall Tf::i_about_b_1Click(TObject *Sender)
{
	try
	{
        String url = "http://www.youtube.com/channel/UCvNUz0gcIU5iEiPIiZ68P5g";
		g.ShellExecute1( url );
	}
	catch (...)
	{

	}
}
void __fastcall Tf::i_about_b_2Click(TObject *Sender)
{
	try
	{
        String url = "http://jenya.in.ua/";
		g.ShellExecute1( url );
	}
	catch (...)
	{

	}
}
void __fastcall Tf::i_about_b_3Click(TObject *Sender)
{
	try
	{
        String url = "http://vk.robots.jenya.in.ua/";
		g.ShellExecute1( url );
	}
	catch (...)
	{

	}
}
void __fastcall Tf::ch_RANDOMIZEClick(TObject *Sender)
{
	if ( LOADED )
	{
		main->conf_ini(true);
	}
}
void __fastcall Tf::N70Click(TObject *Sender)
{
	int index = LV_CONF_ROBOTS->ItemIndex;
	if ( index != -1 )
	{
		main->Robot_Freeze(index);
	}
	else
	{
		g.Sm(L"Пожалуйста, выделите элемент.");
	}
}
void __fastcall Tf::N71Click(TObject *Sender)
{
	int index = LV_CONF_ROBOTS->ItemIndex;
	if ( index != -1 )
	{
		main->Robot_UnFreeze(index);
	}
	else
	{
		g.Sm(L"Пожалуйста, выделите элемент.");
	}
}
void __fastcall Tf::N72Click(TObject *Sender)
{
	main->Robots_Freeze();
}
void __fastcall Tf::N73Click(TObject *Sender)
{
	main->Robots_UnFreeze();
}
void __fastcall Tf::LV_CONF_ROBOTSCustomDrawItem(TCustomListView *Sender, TListItem *Item, TCustomDrawState State, bool &DefaultDraw)
{
	Sender->Canvas->Font->Color = TColor(Item->Data);
}
void __fastcall Tf::LV_CONF_ROBOTSCustomDrawSubItem(TCustomListView *Sender, TListItem *Item, int SubItem, TCustomDrawState State, bool &DefaultDraw)
{
	Sender->Canvas->Font->Color = TColor(Item->Data);
}
void __fastcall Tf::b_ccleanerBrowseClick(TObject *Sender)
{
	if ( OpenDialog2->Execute() )
	{
		e_ccleanerpath->Text = OpenDialog2->FileName;
		main->conf_ini( true );
    }
}
// PM
void __fastcall Tf::N1Click(TObject *Sender)
{
	// SERVERS - DELETE

	int index = LV_SERVERS->ItemIndex;
	if ( index != -1 )
	{
		if ( LV_SERVERS->Items->Count > 1 )
		{
			LV_SERVERS->Items->Delete( index);

			main->conf_servers( true );
			main->conf_servers( false );

			if ( main->CurrentServer >= LV_SERVERS->Items->Count )
			{
				main->CurrentServer = 0;
				main->conf_ini( true );
			}

            main->show_current_server();
		}
		else
		{
			g.Sm(L"Невозможно удалить самый последний пункт..");
        }
    }
}
void __fastcall Tf::N2Click(TObject *Sender)
{
	// SERVERS - COPY ID

	int index = LV_SERVERS->ItemIndex;
	if ( index != -1 )
	{
		String X = LV_SERVERS->Items->Item[index]->SubItems->Strings[0];
		main->buffer_write( X );
	}
}
void __fastcall Tf::N3Click(TObject *Sender)
{
	// SERVERS - COPY LOGIN

	int index = LV_SERVERS->ItemIndex;
	if ( index != -1 )
	{
		String X = LV_SERVERS->Items->Item[index]->SubItems->Strings[1];
		main->buffer_write( X );
	}
}
void __fastcall Tf::N4Click(TObject *Sender)
{
	// SERVERS - COPY PASS

	int index = LV_SERVERS->ItemIndex;
	if ( index != -1 )
	{
		String X = LV_SERVERS->Items->Item[index]->SubItems->Strings[2];
		main->buffer_write( X );
	}
}
void __fastcall Tf::N6Click(TObject *Sender)
{
	// LOG - OPEN FILE

	g.ShellExecute1( main->f_currentlog );
}
void __fastcall Tf::N66Click(TObject *Sender)
{
	// LOG - LOAD ANOTHER FILE

	OD_LOG->InitialDir = main->p_logs;

	if ( OD_LOG->Execute() )
	{
		ME_LOG->Lines->LoadFromFile( OD_LOG->FileName );
        ::PostMessage(f->ME_LOG->Handle,WM_VSCROLL,SB_BOTTOM,0);
	}
}
void __fastcall Tf::N7Click(TObject *Sender)
{
	// LOG - COPY DATA

	main->buffer_write( ME_LOG->Lines->Text );
}
void __fastcall Tf::N12Click(TObject *Sender)
{
	// LOG - CLEAR ME

   	ME_LOG->Lines->Clear();
}
void __fastcall Tf::N17Click(TObject *Sender)
{
	// CONF - GROUPS - DELETE

	int index = LV_CONF_GROUPS->ItemIndex;
	if ( index != -1 )
	{
		LV_CONF_GROUPS->Items->Delete( index );
		main->conf_groups(true);
    }
}
void __fastcall Tf::N18Click(TObject *Sender)
{
	// CONF - ROBOTS - DELETE

	int index = LV_CONF_ROBOTS->ItemIndex;
	if ( index != -1 )
	{
		bool DialogsDelete = MessageDlg(L"Удалить все диалоги связанные с этим роботом?",mtWarning,TMsgDlgButtons() << mbCancel << mbOK , 0) == 1;

		String NeededRobotName = Trim( LV_CONF_ROBOTS->Items->Item[index]->Caption );

		std::auto_ptr<TStringList> L(new TStringList);
		g.GetFiles( main->p_robots, L.get() );

		for ( int c = 0; c < L->Count; c++ )
		{
			String GroupName, RobotName, Server_ID, Login, Password, Token, Activity, Online, LastOnline;  
            bool freeze;
			main->get_robotdata( c, &GroupName, &RobotName, &Server_ID, &Login, &Password, &Token, &Activity, &Online, &LastOnline, &freeze );

			if ( RobotName == NeededRobotName )
			{
				g.DeletePathWithFiles( main->p_robots + L->Strings[c] );
				main->conf_robots( f->LV_CONF_GROUPS->ItemIndex, false);

				vcl->groupechoReadRobots();
				break;
            }
		}

		main->conf_models(false);

		if ( DialogsDelete ) 
        {
            main->deleteDialogsPerRobot(NeededRobotName);
        }
    }
}
void __fastcall Tf::N20Click(TObject *Sender)
{
	// CONF - ROBOTS - CONNECT

	int index = LV_CONF_ROBOTS->ItemIndex;

	if ( index != -1 )
	{
		String NeededRobotName = Trim(LV_CONF_ROBOTS->Items->Item[index]->Caption);
		String token;
        if(f->LV_SERVERS->Items->Count > 0)
        {
            if ( proc->Establish( NeededRobotName, &token ) )
            {
                main->conf_robots(LV_CONF_GROUPS->ItemIndex,false);

                g.Sm( L"[ OK ]\n\nДоступ к API разрешён." );
            }
            else
            {
                main->conf_robots(LV_CONF_GROUPS->ItemIndex,false);
                g.Sm( L"[ ERROR ]\n\nДоступ к API запрещён." );
            }
        }
		else
        {
            main->conf_robots(LV_CONF_GROUPS->ItemIndex,false);
            g.Sm( L"[ ERROR ]\n\nСервера не добавлены." );
        }    
	}
}
void __fastcall Tf::N58Click(TObject *Sender)
{
	// CONF - ROBOTS - CONNECT ALL

	std::auto_ptr<TStringList> L(new TStringList);
	for ( int c = 0; c < LV_CONF_ROBOTS->Items->Count; c++ )
	{
		String NeededRobotName = Trim(LV_CONF_ROBOTS->Items->Item[c]->Caption);
		String token;
		if ( proc->Establish( NeededRobotName, &token ) )
		{
			main->conf_robots(LV_CONF_GROUPS->ItemIndex,false);
			L->Add( "[ OK ]  "+NeededRobotName );
		}
		else
		{
			main->conf_robots(LV_CONF_GROUPS->ItemIndex,false);
			L->Add( "[ ERROR ]  "+NeededRobotName );
		}
	}

	ShowMessage(L->Text);
}
void __fastcall Tf::N10Click(TObject *Sender)
{
	// CONF - USERS - DELETE

	int index = LV_CONF_USERS->ItemIndex;

	if ( index != -1 )
	{
		String id = Trim(LV_CONF_USERS->Items->Item[index]->SubItems->Strings[0]);

		std::auto_ptr<TStringList> L(new TStringList);
        if(FileExists(main->f_users))
        {
		    L->LoadFromFile( main->f_users );
        }
		for ( int c = 0; c < L->Count; c++ )
		{
			String uid = g.Encrypt(2,5,"#",L->Strings[c]);
			if ( uid == id )
			{
				L->Delete(c);
                break;
            }
		}
		L->SaveToFile( UnicodeString(main->f_users), TEncoding::UTF8 );

		main->GlobalUsersCache_Delete( id );

		main->conf_users( LV_CONF_GROUPS->ItemIndex,false );
		vcl->groupechoReadUsers();
	}
}
void __fastcall Tf::N9Click(TObject *Sender)
{
	// CONF - USERS - GOTO PROFILE

	int index = LV_CONF_USERS->ItemIndex;
	if ( index != -1 )
	{
		String id = Trim(LV_CONF_USERS->Items->Item[index]->Caption);
		g.ShellExecute1( "https://vk.com/id"+id );
	}

}
void __fastcall Tf::N11Click(TObject *Sender)
{
	// DIALOGS - ME - ALCLIENT

	GROUP_DIALOG->Align = alClient;
}
void __fastcall Tf::N22Click(TObject *Sender)
{
	// DIALOGS - ME - ALCUSTOM

	GROUP_DIALOG->Align = alCustom;
}
void __fastcall Tf::N23Click(TObject *Sender)
{
	// CONF - USERS - DELETE ALL

	for ( int i = 0; i < LV_CONF_USERS->Items->Count; i++ )
	{
		String id = Trim(LV_CONF_USERS->Items->Item[i]->SubItems->Strings[0]);

		std::auto_ptr<TStringList> L(new TStringList);
        if(FileExists(main->f_users))
        {
		    L->LoadFromFile( main->f_users );
        }
		for ( int c = 0; c < L->Count; c++ )
		{
			String uid = g.Encrypt(2,5,"#",L->Strings[c]);
			if ( uid == id )
			{
				L->Delete(c);
                break;
            }
		}
		L->SaveToFile( UnicodeString(main->f_users), TEncoding::UTF8 );
		main->GlobalUsersCache_Delete( id );
	}

	LV_CONF_USERS->Items->Clear();
	vcl->groupechoReadUsers();
}
void __fastcall Tf::N26Click(TObject *Sender)
{
	// CONF - ROBOTS - OPEN DIRECTORY

	int index = LV_CONF_ROBOTS->ItemIndex;
	if ( index != -1 )
	{
		std::auto_ptr<TStringList> L(new TStringList);
		g.GetFiles( main->p_robots, L.get() );
		for ( int c = 0; c < L->Count; c++ )
		{
			String GroupName, RobotName, Server_ID, Login, Password, Token, Activity, Online, LastOnline;  
            bool freeze;
			main->get_robotdata( c, &GroupName, &RobotName, &Server_ID, &Login, &Password, &Token, &Activity, &Online, &LastOnline, &freeze );

			if ( RobotName == Trim(LV_CONF_ROBOTS->Items->Item[index]->Caption) )
			{
				String t = main->p_robots + L->Strings[c] + "\\";

				wchar_t *wc = t.c_str();
				char c[500];
				wcstombs(c, wc, wcslen(wc)+1);
				ShellExecuteA( NULL, "open", c, NULL, NULL, SW_SHOW );

				break;
			}
		}
	}
}
void __fastcall Tf::N24Click(TObject *Sender)
{
	// CONF - ROBOTS - OPEN MODEL HELLO

    int index = LV_CONF_ROBOTS->ItemIndex;
    if ( index != -1 )
	{
        vcl->GoToModels( Trim( LV_CONF_ROBOTS->Items->Item[index]->Caption ), 0 );
    }
}
void __fastcall Tf::Hellotxt1Click(TObject *Sender)
{
	// CONF - ROBOTS - OPEN MODEL LOGICAL

	int index = LV_CONF_ROBOTS->ItemIndex;
	if ( index != -1 )
	{
		vcl->GoToModels( Trim( LV_CONF_ROBOTS->Items->Item[index]->Caption ), 1 );
	}
}
void __fastcall Tf::N63Click(TObject *Sender)
{
	// CONF - ROBOTS - OPEN MODEL AUTOANS

    int index = LV_CONF_ROBOTS->ItemIndex;
	if ( index != -1 )
	{
		vcl->GoToModels( Trim( LV_CONF_ROBOTS->Items->Item[index]->Caption ), 2 );
	}
}
void __fastcall Tf::N64Click(TObject *Sender)
{
	// CONF - ROBOTS - OPEN MODEL AUTOSTOP

    int index = LV_CONF_ROBOTS->ItemIndex;
	if ( index != -1 )
	{
		vcl->GoToModels( Trim( LV_CONF_ROBOTS->Items->Item[index]->Caption ), 3 );
	}
}
void __fastcall Tf::N27Click(TObject *Sender)
{
	// PM LV DIALOGS - DELETE

	int index = LV_DIALOGS->ItemIndex;
	if ( index != -1 )
	{
		std::auto_ptr<TStringList> L(new TStringList);
		g.GetFiles( main->p_dialogs, L.get() );

		DeleteFile( main->p_dialogs + L->Strings[index] );

		LV_DIALOGS->Items->Delete(index);
	}
}
void __fastcall Tf::N28Click(TObject *Sender)
{
	// PM LV DIALOGS - DELETE ALL

	std::auto_ptr<TStringList> L(new TStringList);
	g.GetFiles( main->p_dialogs, L.get() );
	for ( int c = 0; c < L->Count; c++ )
	{
		DeleteFile( main->p_dialogs + L->Strings[c] );
	}
	LV_DIALOGS->Items->Clear();
}
void __fastcall Tf::N29Click(TObject *Sender)
{
	// PM URL - CLEAR

	e_conf_users_URL->Clear();
}
void __fastcall Tf::N31Click(TObject *Sender)
{
	// PM MODEL HELLO COPY

	int index = LV_MODEL_HELLO->ItemIndex;
	if ( index != -1 )
	{
		main->buffer_write( LV_MODEL_HELLO->Items->Item[index]->SubItems->Strings[0] );
	}
}
void __fastcall Tf::N33Click(TObject *Sender)
{
	// PM MODEL HELLO DEL

	int index = LV_MODEL_HELLO->ItemIndex;
	if ( index != -1 )
	{
		main->MODEL_HELLO->Delete( index );
        std::auto_ptr<TStringList> L(new TStringList);
		g.GetFiles( main->p_robots, L.get() );
        String file = main->p_robots + L->Strings[ CB_MODELS_ROBOTS->ItemIndex ] + "\\Hello.txt";
		main->MODEL_HELLO->SaveToFile( UnicodeString(file), TEncoding::UTF8 );
		main->LoadModelHello();
	}
}
void __fastcall Tf::N34Click(TObject *Sender)
{
    // PM TREE - CREATE ROOT

    GB_MODEL->Caption = L" Создание корневого звена ";

    main->MYPARENT = TREE->Selected->Text;

    main->IFCREATE = true;
    main->IFROOT   = true;

    main->POSTS->Clear();
    main->VARIANTS->Clear();
    main->IFVARIANTS->Clear();
    main->EXTENDED->Clear();
    main->DEFAULT->Clear();
    vcl->ModelStageClear();
    vcl->ModelsSET(true);
    vcl->GetAllStages(CB_MODEL_LOGICAL_IFVARIANTS);
    vcl->GetAllStages(CB_MODEL_LOGICAL_EXTENDED);
    vcl->GetAllStages(CB_MODEL_LOGICAL_DEFAULT);

    B_MODEL_LOGICAL_ADDEDIT->Caption = "+";

    main->MOD = true;
}
void __fastcall Tf::N36Click(TObject *Sender)
{
    // PM TREE - CREATE DAU

    GB_MODEL->Caption = L" Создание дочернего звена. Родитель - " + TREE->Selected->Text + " ";

    main->MYPARENT = TREE->Selected->Text;

    main->IFCREATE = true;
    main->IFROOT   = false;

    main->POSTS->Clear();
    main->VARIANTS->Clear();
    main->IFVARIANTS->Clear();
    main->EXTENDED->Clear();
    main->DEFAULT->Clear();
    vcl->ModelStageClear();
    vcl->ModelsSET(true);
    vcl->GetAllStages(CB_MODEL_LOGICAL_IFVARIANTS);
    vcl->GetAllStages(CB_MODEL_LOGICAL_EXTENDED);
    vcl->GetAllStages(CB_MODEL_LOGICAL_DEFAULT);

    B_MODEL_LOGICAL_ADDEDIT->Caption = "+";

    main->MOD = true;
}
void __fastcall Tf::N35Click(TObject *Sender)
{
	// PM TREE - EDIT

    GB_MODEL->Caption = L" Редактирование звена. Звено - " + TREE->Selected->Text + " ";
    main->MYPARENT = TREE->Selected->Text;
    main->IFCREATE = false;
	vcl->ModelsSET(true);
    E_MODEL_LOGICAL_STAGE->Text = main->MYPARENT;
    B_MODEL_LOGICAL_ADDEDIT->Caption = "ok";
    main->MOD = true;
}
void __fastcall Tf::N53Click(TObject *Sender)
{
	// PM TREE - DELETE

	String DSTAGE = TREE->Selected->Text;
	main->ModelLogicalDeleteStage( DSTAGE );
}
void __fastcall Tf::N55Click(TObject *Sender)
{
	// PM TREE - ПРОВЕРИТЬ ЦЕПЬ ДИАЛОГОВ

	main->checkLinkedStages();
}
void __fastcall Tf::N57Click(TObject *Sender)
{
	// PM MODEL AUTOANS - ПРОВЕРИТЬ ЦЕПЬ ДИАЛОГОВ

	main->checkLinkedStages();
}
void __fastcall Tf::N38Click(TObject *Sender)
{
    // PM MODELS POSTS COPY

    int index = LI_MODEL_LOGICAL_POSTS->ItemIndex;
    if ( index != -1 )
    {
        main->buffer_write( LI_MODEL_LOGICAL_POSTS->Items->Strings[index] );
    }
}
void __fastcall Tf::N40Click(TObject *Sender)
{
    // PM MODELS POSTS DEL

    if ( main->MOD )
    {
    	int index = LI_MODEL_LOGICAL_POSTS->ItemIndex;
    	if ( index != -1 )
    	{
        	LI_MODEL_LOGICAL_POSTS->Items->Delete( index );
        	main->POSTS->Delete( index );
   	 	}
    }
}
void __fastcall Tf::N41Click(TObject *Sender)
{
    // PM MODELS VARIANT COPY

    int index = LI_MODEL_LOGICAL_VARIANTS->ItemIndex;
    if ( index != -1 )
    {
        main->buffer_write( LI_MODEL_LOGICAL_VARIANTS->Items->Strings[index] );
    }
}
void __fastcall Tf::N43Click(TObject *Sender)
{
    // PM MODELS VARIANT DEL

    if ( main->MOD )
    {
    	int index = LI_MODEL_LOGICAL_VARIANTS->ItemIndex;
    	if ( index != -1 )
    	{
        	LI_MODEL_LOGICAL_VARIANTS->Items->Delete( index );

        	main->VARIANTS->Delete( index );
        	main->IFVARIANTS->Delete( index );

        	if ( LI_MODEL_LOGICAL_VARIANTS->Items->Count <= 1 ) 
            {
                main->EXTENDED->Clear();
            }
        	LI_MODEL_LOGICAL_EXTENDED->Items->Clear();
        	main->LoadModelStageExtendeds();
    	}
    }
}
void __fastcall Tf::N44Click(TObject *Sender)
{
    // PM MODELS EXTENDED COPY

    int index = LI_MODEL_LOGICAL_EXTENDED->ItemIndex;
    if ( index != -1 )
    {
        main->buffer_write( LI_MODEL_LOGICAL_EXTENDED->Items->Strings[index] );
    }
}
void __fastcall Tf::N46Click(TObject *Sender)
{
    // PM MODELS EXTENDED DEL

    if ( main->MOD )
    {
    	int index = LI_MODEL_LOGICAL_EXTENDED->ItemIndex;
    	if ( index != -1 )
    	{
        	LI_MODEL_LOGICAL_EXTENDED->Items->Delete( index );
        	main->EXTENDED->Delete( index );
    	}
    }
}
void __fastcall Tf::N47Click(TObject *Sender)
{
	// PM MODEL AUTOANS COPY

	int index = LV_MODEL_AUTOANS->ItemIndex;
	if ( index != -1 )
	{
		main->buffer_write( Trim( LV_MODEL_AUTOANS->Items->Item[index]->Caption ) );
	}
}
void __fastcall Tf::N49Click(TObject *Sender)
{
	// PM MODEL AUTOANS DELETE

	int index = LV_MODEL_AUTOANS->ItemIndex;
	if ( index != -1 )
	{
		main->MODEL_AUTOANS->Delete( index );
        std::auto_ptr<TStringList> L(new TStringList);
		g.GetFiles( main->p_robots, L.get() );
		String file = main->p_robots + L->Strings[ CB_MODELS_ROBOTS->ItemIndex ] + "\\AutoAnsRules.txt";
        
		main->MODEL_AUTOANS->SaveToFile( UnicodeString(file), TEncoding::UTF8 );
		main->LoadModelAutoAnsRules();
	}
}
void __fastcall Tf::N51Click(TObject *Sender)
{
	// PM DIALOGS UPDATE

	main->conf_dialogs(false);
}
void __fastcall Tf::N59Click(TObject *Sender)
{
	// PM MODELS  AUTOSTOP KEYS DEL

	int index = LV_MODEL_AUTOSTOP_KEYS->ItemIndex;
	if ( index != -1 )
	{
		main->MODEL_AUTOSTOP_KEYS->Delete( index );
		std::auto_ptr<TStringList> L(new TStringList);
		g.GetFiles( main->p_robots, L.get() );
		String file = main->p_robots + L->Strings[ CB_MODELS_ROBOTS->ItemIndex ] + "\\AutoStopKeys.txt";
		
		main->MODEL_AUTOSTOP_KEYS->SaveToFile( UnicodeString(file), TEncoding::UTF8 );
		main->LoadModelAutoStopKeys();
	}
}
void __fastcall Tf::N60Click(TObject *Sender)
{
	// PM MODELS  AUTOSTOP KEYS DEL ALL

	main->MODEL_AUTOSTOP_KEYS->Clear();
	std::auto_ptr<TStringList> L(new TStringList);
	g.GetFiles( main->p_robots, L.get() );
	String file = main->p_robots + L->Strings[ CB_MODELS_ROBOTS->ItemIndex ] + "\\AutoStopKeys.txt";
    
	main->MODEL_AUTOSTOP_KEYS->SaveToFile( UnicodeString(file), TEncoding::UTF8 );
	main->LoadModelAutoStopKeys();
}
void __fastcall Tf::N61Click(TObject *Sender)
{
	// PM MODELS  AUTOSTOP POSTS DEL

	int index = LV_MODEL_AUTOSTOP_POSTS->ItemIndex;
	if ( index != -1 )
	{
		main->MODEL_AUTOSTOP_POSTS->Delete( index );
		std::auto_ptr<TStringList> L(new TStringList);
		g.GetFiles( main->p_robots, L.get() );
		String file = main->p_robots + L->Strings[ CB_MODELS_ROBOTS->ItemIndex ] + "\\AutoStopPosts.txt";
        
		main->MODEL_AUTOSTOP_POSTS->SaveToFile( UnicodeString(file), TEncoding::UTF8 );
		main->LoadModelAutoStopPosts();
	}
}
void __fastcall Tf::N62Click(TObject *Sender)
{
	// PM MODELS  AUTOSTOP POSTS DEL ALL

	main->MODEL_AUTOSTOP_POSTS->Clear();
	std::auto_ptr<TStringList> L(new TStringList);
	g.GetFiles( main->p_robots, L.get() );
	String file = main->p_robots + L->Strings[ CB_MODELS_ROBOTS->ItemIndex ] + "\\AutoStopPosts.txt";
    
	main->MODEL_AUTOSTOP_POSTS->SaveToFile( UnicodeString(file), TEncoding::UTF8 );
	main->LoadModelAutoStopPosts();
}
void __fastcall Tf::N65Click(TObject *Sender)
{
	for ( int c = 0; c < f->LV_WORKTASKS->Items->Count; c++ )
	{
		if ( f->LV_WORKTASKS->Items->Item[c]->Checked ) 
        {
            f->LV_WORKTASKS->Items->Item[c]->Checked = false;
        }
		else                                            
        {
            f->LV_WORKTASKS->Items->Item[c]->Checked = true;       
        }
    }
	main->conf_worktasks(true);
}
void __fastcall Tf::N69Click(TObject *Sender)
{
	// PM GROUPS IMPORT ROBOTS FROM TXT

	OpenDialog1->InitialDir = main->f_od_import_robots;
	if ( OpenDialog1->Execute() )
	{
		main->f_od_import_robots = OpenDialog1->InitialDir;
        main->conf_ini(true);
		main->import_robots( OpenDialog1->FileName );
    }
}
void __fastcall Tf::N75Click(TObject *Sender)
{
	std::auto_ptr<TStringList> L(new TStringList);
	g.GetFiles( main->p_robots, L.get() );
	for ( int c = 0; c < L->Count; c++ )
	{
		String robotpath = main->p_robots + L->Strings[c] + "\\" + "Conf.ini";
        std::auto_ptr<TMemIniFile> INI(new TMemIniFile( UnicodeString(robotpath),TEncoding::UTF8 ));
        INI->WriteBool( UnicodeString("MAIN"), UnicodeString("freeze"), true );
        INI->UpdateFile();
	}
	g.Sm(L"Готово");
}
void __fastcall Tf::e_conf_users_CountChange(TObject *Sender)
{
	if ( LOADED )
	{
		main->conf_ini(true);
	}
}
// TEST
void __fastcall Tf::BTEST2Click(TObject *Sender)
{
	ShowMessage("T2");
}
void __fastcall Tf::B_TEST_01Click(TObject *Sender)
{
    std::auto_ptr<TStringList> IMAGELIST(new TStringList);
    std::auto_ptr<TStringList> AUDIOLIST(new TStringList);
    std::auto_ptr<TStringList> RECORDLIST(new TStringList);

	String PostLine = L"POST'$25 сутки. Паспорт в залог обязателен![IMAGE=Demotivators\\Image1.jpg][RECORD=Voices-1\\ogg_sound_effect_56.ogg]";

	PostLine = g.Encrypt(2,2,"'",PostLine);

	f->ii->WritePostDataIn(IMAGELIST.get(),AUDIOLIST.get(),RECORDLIST.get(),&PostLine);

	ShowMessage("postline_"+PostLine+"_");
	ShowMessage("IMAGELIST_"+IMAGELIST->Text+"_");
	ShowMessage("AUDIOLIST_"+AUDIOLIST->Text+"_");
	ShowMessage("RECORDLIST_"+RECORDLIST->Text+"_");
}
























