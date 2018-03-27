#include <vcl.h>
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
#pragma resource "*.dfm"
#include "Grin.cpp"
#include <Inifiles.hpp>
#include <memory>
#include <FileCtrl.hpp>
#include "DateUtils.hpp"


Tf *f;
bool LOADED = false; int PAGEINDEX = 0;


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
		if ( PAGES->ActivePageIndex != 6 )
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

	p_robots 			= g.GetDirectoryApplicationDatapath() + "Robots\\";
	p_dialogs           = g.GetDirectoryApplicationDatapath() + "Dialogs\\";
	p_dialogs_test      = g.GetDirectoryApplicationDatapath() + "DialogsTest\\";
	p_logs              = g.GetDirectoryApplicationDatapath() + "Logs\\";

    checkDirectoryExisting(g.GetDirectoryApplicationDatapath());
    checkDirectoryExisting(p_robots);
    checkDirectoryExisting(p_dialogs);
    checkDirectoryExisting(p_dialogs_test);
    checkDirectoryExisting(p_logs);
    
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
	MODEL_LOGICAL 			= new TStringList;
	MODEL_AUTOANS 			= new TStringList;
	MODEL_AUTOSTOP_KEYS 	= new TStringList;
	MODEL_AUTOSTOP_POSTS 	= new TStringList;

    POSTS 		= new TStringList;
    VARIANTS 	= new TStringList;
    IFVARIANTS  = new TStringList;
    EXTENDED 	= new TStringList;
    DEFAULT 	= new TStringList;

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
	TMemIniFile *INI = new TMemIniFile( UnicodeString(f_ini),TEncoding::UTF8 );

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
        INI->WriteString( UnicodeString("IODIALOGS"), UnicodeString("import_robot_models"), f_od_import_robot_model );
		INI->WriteString( UnicodeString("IODIALOGS"), UnicodeString("import_robots"), f_od_import_robots );
        
        INI->UpdateFile();
	}
	else
	{
		CurrentServer = INI->ReadInteger( UnicodeString("SERVER"), UnicodeString("current"),     0  );
		RobotPathID   = INI->ReadInteger( UnicodeString("OTHER"),  UnicodeString("robotpathid"), 0  );
		APIV          = INI->ReadString(  UnicodeString("OAUTH2"), UnicodeString("api"),        UnicodeString("0") );
		f->e_conf_users_offSet->Text  = INI->ReadString( UnicodeString("OTHER"),  UnicodeString("offset"),     UnicodeString("0") );
		f->e_conf_users_Count->Text   = INI->ReadString( UnicodeString("OTHER"),  UnicodeString("count"),      UnicodeString("0") );
		f->e_conf_users_URL->Text	  = INI->ReadString( UnicodeString("OTHER"),  UnicodeString("search_url"), UnicodeString("0") );
		f->CH_LOG->Checked 			  = INI->ReadBool(   UnicodeString("OTHER"),  UnicodeString("log") , false );
		f->CH_APIRET->Checked         = INI->ReadBool(   UnicodeString("OTHER"),  UnicodeString("apiret") , false );
		f->CH_LOGGOTO->Checked        = INI->ReadBool(   UnicodeString("OTHER"),  UnicodeString("loggoto") , false );
		f->CH_LOG_PAUSES->Checked     = INI->ReadBool(   UnicodeString("OTHER"),  UnicodeString("logpauses") , false );
		BUFF_CURRENTLOG               = INI->ReadString( UnicodeString("OTHER"),  UnicodeString("currentlog") , UnicodeString("0") );
		f->e_autoanswerlimit->Text    = INI->ReadString( UnicodeString("OTHER"),  UnicodeString("autoanswerlimit") , UnicodeString("0") );
        f_od_import_robot_model       = INI->ReadString( UnicodeString("IODIALOGS"), UnicodeString("import_robot_models"), UnicodeString("") );
		f_od_import_robots 			  = INI->ReadString( UnicodeString("IODIALOGS"), UnicodeString("import_robots"), UnicodeString("") );
	}

	delete INI;
}
void c_main::conf_intervals(bool save)
{
	TMemIniFile *INI = new TMemIniFile( UnicodeString(f_ini), TEncoding::UTF8 );

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

	delete INI;

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
	TMemIniFile *INI = new TMemIniFile( UnicodeString(f_ini), TEncoding::UTF8 );

	if ( save )
	{
		INI->WriteInteger( UnicodeString("CAPTCHA"),  UnicodeString("serviceindex"), f->rg_CAPTCHA_SERVICE->ItemIndex );
		INI->WriteString(  UnicodeString("CAPTCHA"),  UnicodeString("key"),          UnicodeString(f->e_CAPTCHA_KEY->Text) );
        INI->UpdateFile();
	}
	else
	{
		f->rg_CAPTCHA_SERVICE->ItemIndex = INI->ReadInteger( UnicodeString("CAPTCHA"),  UnicodeString("serviceindex"),       0  );
		f->e_CAPTCHA_KEY->Text           = INI->ReadString(  UnicodeString("CAPTCHA"),   UnicodeString("key"),               UnicodeString("0") );
	}

	delete INI;
}
void c_main::conf_servers(bool save)
{
	TStringList *L = new TStringList;

	if ( save )
	{
		for ( int c = 0; c < f->LV_SERVERS->Items->Count; c++ )
		{
			str id    = f->LV_SERVERS->Items->Item[c]->SubItems->Strings[0];
			str login = f->LV_SERVERS->Items->Item[c]->SubItems->Strings[1];
			str pass  = f->LV_SERVERS->Items->Item[c]->SubItems->Strings[2];

			L->Add(UnicodeString(id + "#" + login + "#" + pass));
		}
                                                              
		L->SaveToFile(UnicodeString(f_servers),TEncoding::UTF8);
	}
	else
	{
		f->LV_SERVERS->Items->Clear();
        if(FileExists(f_servers))
        {
		    L->LoadFromFile( f_servers );
        }

		for ( int c = 0; c < L->Count; c++ )
		{
			str line = L->Strings[c];

			TListItem *ListItem;
			ListItem = f->LV_SERVERS->Items->Add();
			ListItem->Caption = " "+IntToStr( c+1 );
			ListItem->SubItems->Add( g.Encrypt(1,3,"#",line) );
			ListItem->SubItems->Add( g.Encrypt(2,3,"#",line) );
			ListItem->SubItems->Add( g.Encrypt(3,3,"#",line) );
        }
	}

	delete L;
}
void c_main::conf_groups(bool save)
{
	TStringList *L = new TStringList;

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

	delete L;

	conf_workgroups(false);
}
void c_main::conf_workgroups(bool save)
{
	TStringList *L = new TStringList;

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
			str groupname = Trim( f->LV_WORKGROUPS->Items->Item[c]->Caption );
			for ( int x = 0; x < L->Count; x++ )
			{
				if ( L->Strings[x] == groupname )
				{
					ex = true;
                    false;
                }
			}

			if ( ex ) 
            {
                f->LV_WORKGROUPS->Items->Item[c]->Checked = true;
            }
			else      
            {
                f->LV_WORKGROUPS->Items->Item[c]->Checked = false;
            }
		}
	}

	delete L;
}
void c_main::conf_worktasks(bool save)
{
	TStringList *L = new TStringList;

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
	}

	delete L;
}
void c_main::conf_robots(int index, bool save)
{
	TStringList *L = new TStringList;

	if ( save )
	{
		str robotpath = p_robots + IntToStr(RobotPathID) + "_" + g.GetMD5(RobotPathID) + "\\";
        checkDirectoryExisting(robotpath);
        
		RobotPathID++;
		conf_ini( true );

		TMemIniFile *INI = new TMemIniFile( UnicodeString(robotpath + "Conf.ini"), TEncoding::UTF8 );
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
		delete INI;

		conf_robots( index, false );

		f->e_conf_robots_create_name->Clear();
		f->e_conf_robots_create_login->Clear();
		f->e_conf_robots_create_password->Clear();
		f->ch_robot_online->ItemIndex = 1;

		createStandartDataHello(L);
		L->SaveToFile(robotpath+"Hello.txt");

		createGlobalDataModel(L);
		L->SaveToFile(robotpath+"Global.txt");

		createStandartDataModel(L);
		L->SaveToFile(robotpath+"Model.txt");

		createStandartDataAutoAnsRules(L);
		L->SaveToFile(robotpath+"AutoAnsRules.txt");

		createStandartDataAutoAnsDefault(L);
		L->SaveToFile(robotpath+"AutoAnsDefault.txt");

		createStandartDataAutoStopKeys(L);
		L->SaveToFile(robotpath+"AutoStopKeys.txt");

		createStandartDataAutoStopPosts(L);
		L->SaveToFile(robotpath+"AutoStopPosts.txt");

        conf_models(false);
	}
	else
	{
		f->LV_CONF_ROBOTS->Items->Clear();
		str NeededGroupName = Trim(f->LV_CONF_GROUPS->Items->Item[index]->Caption);
		g.GetFiles( p_robots, L );

        for ( int c = 0; c < L->Count; c++ )
		{
			str GroupName, RobotName, Server_ID, Login, Password, Token, Activity, Online, LastOnline; 
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

	delete L;
}
void c_main::conf_robots_import(int groupindex, str name, str login, str pass)
{
	TStringList *L = new TStringList;
    str robotpath = p_robots + IntToStr(RobotPathID) + "_" + g.GetMD5(RobotPathID) + "\\";
    CreateDir( robotpath );
    RobotPathID++;
    conf_ini( true );

    TMemIniFile *INI = new TMemIniFile( UnicodeString(robotpath + "Conf.ini"), TEncoding::UTF8 );
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
    delete INI;

    conf_robots( groupindex, false );

    f->e_conf_robots_create_name->Clear();
    f->e_conf_robots_create_login->Clear();
    f->e_conf_robots_create_password->Clear();
    f->ch_robot_online->ItemIndex = 1;

    createStandartDataHello(L);
    L->SaveToFile(UnicodeString(robotpath+"Hello.txt"), TEncoding::UTF8);

    createGlobalDataModel(L);
    L->SaveToFile(UnicodeString(robotpath+"Global.txt"), TEncoding::UTF8);

    createStandartDataModel(L);
    L->SaveToFile(UnicodeString(robotpath+"Model.txt"), TEncoding::UTF8);

    createStandartDataAutoAnsRules(L);
    L->SaveToFile(UnicodeString(robotpath+"AutoAnsRules.txt"), TEncoding::UTF8);

    createStandartDataAutoAnsDefault(L);
    L->SaveToFile(UnicodeString(robotpath+"AutoAnsDefault.txt"), TEncoding::UTF8);

    createStandartDataAutoStopKeys(L);
    L->SaveToFile(UnicodeString(robotpath+"AutoStopKeys.txt"), TEncoding::UTF8);

    createStandartDataAutoStopPosts(L);
    L->SaveToFile(UnicodeString(robotpath+"AutoStopPosts.txt"), TEncoding::UTF8);

    conf_models(false);

	delete L;
}
void c_main::conf_users(int index, bool save)
{
	TStringList *L = new TStringList;
	str NeededGroupName = Trim(f->LV_CONF_GROUPS->Items->Item[index]->Caption);

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
			str dataline = L->Strings[c];

			str UserGroup 	= g.Encrypt(1,5,"#",dataline);
			str UserID 		= g.Encrypt(2,5,"#",dataline);
			str Name 		= g.Encrypt(3,5,"#",dataline);
			str Surname 	= g.Encrypt(4,5,"#",dataline);
			str Date 		= g.Encrypt(5,5,"#",dataline);

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

	delete L;
}
void c_main::conf_dialogs(bool save)
{
	TStringList *L = new TStringList;
	TStringList *X = new TStringList;

	if ( ! save )
	{
		f->LV_DIALOGS->Items->Clear();
		g.GetFiles( p_dialogs, L );

		for ( int c = 0; c < L->Count; c++ )
		{
			str id = L->Strings[c];
            if(FileExists(p_dialogs + id))
            {
			    X->LoadFromFile( p_dialogs + id );

			    str name 		 = X->Strings[1];
			    str surname 	 = X->Strings[2];

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

	delete L;
	delete X;
}
void c_main::conf_models(bool save)
{
	if ( ! save )
	{
		int index = f->CB_MODELS_ROBOTS->ItemIndex;
		f->CB_MODELS_ROBOTS->Items->Clear();

        TStringList *L = new TStringList;
    	g.GetFiles( p_robots, L );
        for ( int c = 0; c < L->Count; c++ )
		{
			str GroupName, RobotName, Server_ID, Login, Password, Token, Activity, Online, LastOnline;  
            bool freeze;
			get_robotdata( c, &GroupName, &RobotName, &Server_ID, &Login, &Password, &Token, &Activity, &Online, &LastOnline, &freeze );
            f->CB_MODELS_ROBOTS->Items->Add( RobotName );
    	}
		delete L;

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
void c_main::GlobalUsersCache_Add(str id)
{
	GLOBAL_USERS_CACHE->Add( id );
	GLOBAL_USERS_CACHE->SaveToFile( f_globaluserscache );
}
bool c_main::GlobalUsersCache_Exist(str id)
{
	bool ex = false;
	for ( int c = 0; c < GLOBAL_USERS_CACHE->Count; c++ )
	{
		str line = GLOBAL_USERS_CACHE->Strings[c];
		if ( line == id )
		{
			ex = true;
			break;
		}
	}
	return ex;
}
void c_main::GlobalUsersCache_Delete(str id)
{
	try
	{
		for ( int c = GLOBAL_USERS_CACHE->Count-1; c >= 0; c-- )
		{
			str line = GLOBAL_USERS_CACHE->Strings[c];
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
	TStringList *L = new TStringList;
	for ( int c = 0; c < GLOBAL_USERS_CACHE->Count; c++ )
	{
		str a = GLOBAL_USERS_CACHE->Strings[c];
		if ( Pos(a,L->Text) == 0 ) 
        {
            L->Add(a);
        }
	}

	GLOBAL_USERS_CACHE->Text = L->Text;
	delete L;
	GLOBAL_USERS_CACHE->SaveToFile( f_globaluserscache );
    
	ShowMessage(L"Выполнено.\n\nДо очистки: "+IntToStr(acount)+L"\nПосле очистки: "+IntToStr(GLOBAL_USERS_CACHE->Count));
}
void c_main::log( str data )
{
	try
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
	catch (...)
	{

	}
}

void c_main::checkDirectoryExisting(str directoryPath)
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
void c_main::iSleep(int index, str token)
{
	str a;
    int x;

    f->online->addtobuffer(token);
    
	// 1 : Api Request, 2 : Add To Friends, 3 : Send Message, 4 : Confirm Friend

	if ( index == 1 ) { a = f->e_int_api->Text; x = StrToInt(f->e_int_api->Text); }
	if ( index == 2 ) { a = f->e_int_addfr->Text; x = StrToInt(f->e_int_addfr->Text); }
	if ( index == 3 ) { a = f->e_int_msg->Text; x = StrToInt(f->e_int_msg->Text); }
    if ( index == 4 ) { a = f->e_int_confirmfriend->Text; x = StrToInt(f->e_int_confirmfriend->Text); }

	if ( x > 0 )
	{
		f->BARSMOOTH->Visible = true;

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

		f->BARSMOOTH->Visible = false;
		g.ProcessMessages();
	}
}
void c_main::logline( str data )
{
	str line = ":::::::::::::::::::::::::::::::::::::::::::::::::::::::";

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
str  c_main::jsonfix( str data )
{
	str buff = "";

	for ( int c = 1; c < data.Length(); c++ )
	{
		str ch = data[c];
		if ( ch != "\"" ) 
        {
            buff = buff + ch;
        }
	}

	return buff;
}
void c_main::buffer_write( str data )
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
void c_main::get_robotdata( int index, str *GroupName, str *RobotName, str *Server_ID, str *Login, str *Password, str *Token, str *Activity, str *Online, str *LastOnline, bool *freeze )
{
	TStringList *L = new TStringList;
	g.GetFiles( p_robots, L );
	str robotpath = p_robots + L->Strings[index] + "\\";

	TMemIniFile *INI = new TMemIniFile( UnicodeString(robotpath + "Conf.ini"),TEncoding::UTF8 );

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
    
	delete INI;
	delete L;
}
int  c_main::get_robotnext( str GroupName )
{
	int CNT = 0;

	TStringList *L = new TStringList;
	g.GetFiles(p_robots,L);
	for ( int c = 0; c < L->Count; c++ )
	{
		str CurGroupName, RobotName, Server_ID, Login, Password, Token, Activity, Online, LastOnline;  
        bool freeze;
		get_robotdata( c, &CurGroupName, &RobotName, &Server_ID, &Login, &Password, &Token, &Activity, &Online, &LastOnline, &freeze );
		if ( CurGroupName == GroupName ) 
        {
            CNT++;
        }
	}

    delete L;

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
void c_main::search_request( str RequestUrl, str OffSet, str Count, int iteration )
{
	logline( "ADD USERS BY VK SEARCH URL" );

	log("ITERATION: "+IntToStr(iteration));
	log("OFFSET:    "+OffSet);
	log("COUNT:     "+Count);

	str GroupName, RobotName, Server_ID, Login, Password, Token, Activity, Online, LastOnline; 
    bool freeze;
	TStringList *L = new TStringList;
	g.GetFiles( p_robots, L );
	get_robotdata( Random(L->Count), &GroupName, &RobotName, &Server_ID, &Login, &Password, &Token, &Activity, &Online, &LastOnline, &freeze );

	if ( f->proc->Establish(RobotName,&Token) )
	{
		str Group               = get_vkurl_param( "group",RequestUrl );
		str q                   = get_vkurl_param( "q",RequestUrl );
		str Country 			= get_vkurl_param( "country",RequestUrl );
		str City                = get_vkurl_param( "city",RequestUrl );
		str AgeFrom 			= get_vkurl_param( "age_from",RequestUrl );
		str AgeTo   			= get_vkurl_param( "age_to",RequestUrl );
		str Sex     			= get_vkurl_param( "sex",RequestUrl );
		str Status  			= get_vkurl_param( "status",RequestUrl );
		str PhotoEX 			= get_vkurl_param( "photo",RequestUrl );
		str OnlineEX 			= get_vkurl_param( "online",RequestUrl );
		str Religion    		= get_vkurl_param( "religion",RequestUrl );
		str PersonalPriority    = get_vkurl_param( "personal_priority",RequestUrl );
		str PeoplePriority  	= get_vkurl_param( "people_priority",RequestUrl );
		str Smoking  			= get_vkurl_param( "smoking",RequestUrl );
		str Alcogol  			= get_vkurl_param( "alcohol",RequestUrl );
		str Company  			= get_vkurl_param( "company",RequestUrl );
		str Position  			= get_vkurl_param( "position",RequestUrl );
		str MilitaryCountry  	= get_vkurl_param( "mil_country",RequestUrl );
		str birth_year  		= get_vkurl_param( "byear",RequestUrl );
		str birth_month  	    = get_vkurl_param( "bmonth",RequestUrl );
		str birth_day  		    = get_vkurl_param( "bday",RequestUrl );

		str request = "q="+q;
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

		str response;

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
            response_read(response,L,Count,OffSet,RequestUrl,iteration);
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
                    response_read(response,L,Count,OffSet,RequestUrl,iteration);
                }
            }
        }
	}

	delete L;

	logline( "" );
}
str  c_main::include_actual_bdata(int m, int d, str request)
{
	str fx = "&birth_month=";
	int pos = Pos(fx,request);
	str a = request.SubString(1,pos-1);
    return a + "&birth_month=" + IntToStr(m) + "&birth_day=" + IntToStr(d);
}
void c_main::response_read(str response, TStringList *L, str Count, str OffSet, str RequestUrl, int iteration)
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
        str allcount = obj_response->GetValue("count")->ToString();

        TJSONArray *obj_items = static_cast<TJSONArray*>(obj_response->Get("items")->JsonValue);
        for ( int c = 0; c < obj_items->Count; c++ )
        {
            TJSONObject* x_obj_items = static_cast<TJSONObject*>(obj_items->Get(c));

            str uid 			= x_obj_items->GetValue("id")->ToString();
            str uname 			= x_obj_items->GetValue("first_name")->ToString();
            str usurname 	   	= x_obj_items->GetValue("last_name")->ToString();

            uname 	 = jsonfix(uname);
            usurname = jsonfix(usurname);

            if ( ! GlobalUsersCache_Exist( uid ) )
            {
                TDateTime D = Date();
                TDateTime T = Time();
                str DT = DateToStr(D) + " - " + TimeToStr(T);
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
str  c_main::get_vkurl_param(str ParamName, str Data)
{
	str RETURN = "";

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
str  c_main::getCountOfMesages(str Dialog)
{
	TStringList *L = new TStringList;
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

	delete L;

	str J;
	if ( new_count == 0 ) 
        J = IntToStr(mess_count);
	else                  
        J = IntToStr(mess_count) + " (" + IntToStr(new_count) + ")";

	return J;
}
int  c_main::getCountOfHello(str GroupName)
{
	int C = 0;

	TStringList *L = new TStringList;
    if(FileExists(f_users))
    {
	    L->LoadFromFile( f_users );
    }
	for ( int x = 0; x < L->Count; x++ )
	{
		str CurGroup = Trim( g.Encrypt(1,5,"#",L->Strings[x]) );

		if ( CurGroup == GroupName ) 
            C++;
	}

	delete L;
	return C;
}
void c_main::DrawMessageBox(str Name, str Surname, str RobotGID, TStringList *DIALOGS)
{
	for ( int c = 0; c < DIALOGS->Count; c++ )
	{
		str dataline = DIALOGS->Strings[c];
		if ( dataline == "#MESSAGE" )
		{
			bool Out = true;
			if ( DIALOGS->Strings[c+1] == "#INC" ) 
                Out = false;
			str  DATE = DIALOGS->Strings[c+2].SubString(7,50);

			TStringList *TEXT = new TStringList;
			int START_C = c + 7;
			while ( true )
			{
				str S = DIALOGS->Strings[START_C];
				if ( S == "#END" ) 
                    break;
				TEXT->Add(S);
				START_C++;
			}

			str Usr = L"РОБОТ [ "+RobotGID+" ] :";
			if ( ! Out ) 
                Usr = Name+" "+Surname+" :";

			f->vcl->Dialog_Add_User(Usr);
			f->vcl->Dialog_Add_Text(TEXT);
			f->vcl->Dialog_Add_Date(DATE);

			delete TEXT;
		}
	}
}
void c_main::DeleteUserFromQueueAndPutToDialogs(str UserID, str UserName, str UserSurname, str RobotName, bool msghello, str Text)
{
	TStringList *L = new TStringList;
    if(FileExists(f_users))
    {
	    L->LoadFromFile( f_users );
    }
	for ( int c = L->Count-1; c >= 0; c-- )
	{
		str uid = g.Encrypt(2,5,"#",L->Strings[c]);
		if ( uid == UserID ) 
        { 
            L->Delete(c); 
            break; 
        }
	}
	L->SaveToFile( UnicodeString(f_users), TEncoding::UTF8 );
	delete L;

	f->PAGES_CONFIGURATION->ActivePageIndex = 0;
	f->LV_CONF_GROUPS->ItemIndex            = -1;

	TDateTime D = Date();
	TDateTime T = Time();
	str DT = DateToStr(D) + " - " + TimeToStr(T);
	TStringList *X = new TStringList;
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
	delete X;
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
void c_main::WriteInboxList(str Token)
{
	bool ex = false;
	for ( int c = 0; c < INBOX->Count; c++ )
	{
		str _token = g.Encrypt(1,6,"#", INBOX->Strings[c] );
		if ( Token == _token )
		{
			ex = true;
			break;
        }
    }

	if ( ! ex )
	{
		log(L"Получение входящих сообщений и заполнение буффера..");
		g.ProcessMessages();

		int max_count_of = 0;
		int current_offset_of = 0;
		while ( true )
		{
			str response;
			bool success = false;
			while ( ! success )
			{
				response = vk.messages_get(&success,current_offset_of,200,Token);
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

			str vMid = "id";
			str vUid = "user_id";
			if ( Pos(vUid,response) == 0 ) 
            { 
                vMid = "mid"; 
                vUid = "uid"; 
            }
			// JSON //////////////////////////////////////////////////////////////////////////////
			str messcount = "0";

			try
			{
				std::auto_ptr<TJSONObject> json (static_cast<TJSONObject*>(TJSONObject::ParseJSONValue(response)));
				TJSONObject *obj_response = static_cast<TJSONObject*>(json->Get("response")->JsonValue);
				messcount = obj_response->GetValue("count")->ToString();
				TJSONArray *obj_items = static_cast<TJSONArray*>(obj_response->Get("items")->JsonValue);
				for ( int c = 0; c < obj_items->Count; c++ )
				{
					TJSONObject* x_obj_items = static_cast<TJSONObject*>(obj_items->Get(c));

					str mid 			= x_obj_items->GetValue(vMid)->ToString();
					str uid 			= x_obj_items->GetValue(vUid)->ToString();
					str read_state 		= x_obj_items->GetValue("read_state")->ToString();
					str title 	   		= x_obj_items->GetValue("title")->ToString();
					str body 			= x_obj_items->GetValue("body")->ToString();

					INBOX->Add( Token+"#"+mid+"#"+uid+"#"+read_state+"#"+title+"#"+body );

					if ( ! GlobalUsersCache_Exist(uid) ) 
                    { 
                        GlobalUsersCache_Add(uid); 
                    }
				}
			}
			catch (...)
			{
				try
				{
					std::auto_ptr<TJSONObject> json (static_cast<TJSONObject*>(TJSONObject::ParseJSONValue(response)));
					TJSONArray *obj_response = static_cast<TJSONArray*>(json->Get("response")->JsonValue);
					messcount = IntToStr( obj_response->Count - 1 );
					for ( int c = 1; c < obj_response->Count; c++ )
					{
						TJSONObject* x_arr_item = static_cast<TJSONObject*>(obj_response->Get(c));

						str mid 			= x_arr_item->GetValue(vMid)->ToString();
						str uid 			= x_arr_item->GetValue(vUid)->ToString();
						str read_state 		= x_arr_item->GetValue("read_state")->ToString();
						str title 	   		= x_arr_item->GetValue("title")->ToString();
						str body 			= x_arr_item->GetValue("body")->ToString();

						INBOX->Add( Token+"#"+mid+"#"+uid+"#"+read_state+"#"+title+"#"+body );

						if ( ! GlobalUsersCache_Exist(uid) ) 
                        { 
                            GlobalUsersCache_Add(uid); 
                        }
					}
				}
				catch (...) 
                { 
                }
			}
			// //////////////////////////////////////////////////////////////////////////////////

			if ( max_count_of == 0 ) 
            {
                max_count_of = StrToInt(messcount);  
            }
            
			log(L"Смещение: [ "+IntToStr(current_offset_of)+L" ] Количество в стеке: [ "+messcount+" ]");

			current_offset_of = current_offset_of + 200;
			if ( max_count_of == 0 || 
                 max_count_of < current_offset_of ) 
            {
                break;
            }
		}

		log(L"Список сообщений получен. Буффер наполнен. Общее количество: [ "+IntToStr(f->main->INBOX->Count)+" ]");
	}
	else
	{
		log(L"Список сообщений был получен ранее. Буффер наполнен.");
	}
}
void c_main::WriteOutboxAutoAnsList(str Token, str RobotName)
{
    str PX = f->main->PREFIX;
	f->main->PREFIX = L"Автоответчик : [ "+RobotName+" ] ";

	if ( Pos(Token,AUTOANSOUTBOX->Text) == 0 )
	{
		TStringList *UIDS = new TStringList;
		GetDialogs( UIDS, 0, 0, Token, RobotName );

		for ( int c = 0; c < UIDS->Count; c++ )
		{
			str UID = UIDS->Strings[c];
			GetHistory( AUTOANSOUTBOX, UID, 1, 10, Token, RobotName );
		}
		delete UIDS;
	}
	else
	{
		log(L"Список диалогов и сообщений был получен ранее. Буффер наполнен.");
	}

	f->main->PREFIX = PX;
}
void c_main::GetDialogs(TStringList *UIDS, int OUT_3, int READSTATE_3, str Token, str RobotName)
{
	log(L"Получение диалогов..");
	g.ProcessMessages();

	int max_count_of = 0;
	int current_offset_of = 0;

	while ( true )
	{
		str response;
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
		str messcount = obj_response->GetValue("count")->ToString();
		TJSONArray *obj_items = static_cast<TJSONArray*>(obj_response->Get("items")->JsonValue);
		for ( int c = 0; c < obj_items->Count; c++ )
		{
			TJSONObject *obj_root = static_cast<TJSONObject*>(obj_items->Get(c));
			str unread          = "0"; 
            try 
            { 
                unread = obj_root->GetValue("unread")->ToString(); 
            } 
            catch (...) 
            { 
            }

			TJSONObject *obj_message = static_cast<TJSONObject*>(obj_root->Get("message")->JsonValue);
			str mid 	     = obj_message->GetValue("id")->ToString();
			str date 	     = obj_message->GetValue("date")->ToString();
			str out 	     = obj_message->GetValue("out")->ToString();
			str user_id 	 = obj_message->GetValue("user_id")->ToString();
			str read_state 	 = obj_message->GetValue("read_state")->ToString();
			str title 	     = obj_message->GetValue("title")->ToString();
			str body 	     = obj_message->GetValue("body")->ToString();

			bool go1 = false; // OUT_3
			if ( OUT_3 == 0 && out == "0" ) 
            {
                go1 = true;
            }
			if ( OUT_3 == 1 && out == "1" )
            {
                go1 = true;
            }
			if ( OUT_3 == 2 ) 
            {
                go1 = true;
            }

			bool go2 = false; // READSTATE_3
			if ( READSTATE_3 == 0 && read_state == "0" ) 
            {
                go2 = true;
            }
			if ( READSTATE_3 == 1 && read_state == "1" ) 
            {
                go2 = true;
            }
			if ( READSTATE_3 == 2                      ) 
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

		log(L"Смещение: [ "+IntToStr(current_offset_of)+L" ] Количество в стеке: [ "+obj_items->Count+" ]");

		current_offset_of = current_offset_of + 200;

		if (  max_count_of == 0 || 
              max_count_of < current_offset_of ) 
        {
            break;
        }
	}

	log(L"Список диалогов получен.");
}
void c_main::GetHistory(TStringList *LIST, str UID, int OUT_3, int Count, str Token, str RobotName)
{
	log(L"Получение истории сообщений пользователя [ "+UID+L" ] .. Макс [ "+IntToStr( Count )+" ] шт.");
	g.ProcessMessages();

	int max_count_of = 0;
	int current_offset_of = 0;

	int gotch_cnt = 0;
	bool breaked = false;

	while ( true )
	{
		str response;
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
		str messcount = obj_response->GetValue("count")->ToString();
		TJSONArray *obj_items = static_cast<TJSONArray*>(obj_response->Get("items")->JsonValue);

		for ( int c = 0; c < obj_items->Count; c++ )
		{
			TJSONObject *obj_root = static_cast<TJSONObject*>(obj_items->Get(c));
			str mid 		= obj_root->GetValue("id")->ToString();
			str body  		= obj_root->GetValue("body")->ToString();
			str user_id 	= obj_root->GetValue("user_id")->ToString();
			str from_id 	= obj_root->GetValue("from_id")->ToString();
			str date 		= obj_root->GetValue("date")->ToString();
			str read_state 	= obj_root->GetValue("read_state")->ToString();
			str out 		= obj_root->GetValue("out")->ToString();

			bool go = false; // OUT_3
			if ( OUT_3 == 0 && out == "0" ) 
            {
                go = true;
            }
			if ( OUT_3 == 1 && out == "1" ) 
            {
                go = true;
            }
			if ( OUT_3 == 2               ) 
            {
                go = true;
            }

			if ( go )
			{
				AUTOANSOUTBOX->Add( Token+"#"+mid+"#"+UID+"#"+read_state+"#NULL#"+body );
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

		log(L"Смещение: [ "+IntToStr(current_offset_of)+L" ] Количество в стеке: [ "+obj_items->Count+" ]");

		current_offset_of = current_offset_of + 200;

		if (  max_count_of == 0 || 
              max_count_of < current_offset_of ) 
        {
            break;
        }
	}

	log(L"Получено от [ "+UID+" ] сообщений [ "+IntToStr( gotch_cnt )+L" ] по заданному фильтру.");
}
str  c_main::GetLastStageName(TStringList *DIALOG)
{
	str J;
	for ( int c = 0; c < DIALOG->Count; c++ )
	{
		str l = DIALOG->Strings[c];
		if ( Pos("#STAGEDATA=",l) != 0 && Pos("#STAGEDATA=INC",l) == 0 )
		{
			J = l.SubString(12,l.Length());
        }
	}
	return J;
}
void c_main::GetOnlyOneStage(TStringList *MODEL, str StageName)
{
	TStringList *A = new TStringList;                                            //log("GetOnlyOneStage - start");

	bool ex = false;
	bool wr = false;

	for ( int c = 0; c < MODEL->Count; c++ )
	{
		str l = Trim( MODEL->Strings[c] );
		if (   wr && Pos("#",l)    != 0 ) 
            break;

		str ch = StageName[1];
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

	delete A;                                                                                
}
int  c_main::GetMaxLevelsOfStages()
{
    int max = 1;
    for ( int c = 0; c < MODEL_LOGICAL->Count; c++ )
    {
        str l = MODEL_LOGICAL->Strings[c];
        if ( Pos("#",l) != 0 )
        {
            int x = 0;
            for ( int i = 1; i <= l.Length(); i++ )
            {
                str ch = l[i];
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
        str l = MODEL_LOGICAL->Strings[c];
        if ( Pos("#",l) != 0 )
        {
            int x = 0;
            for ( int i = 1; i <= l.Length(); i++ )
            {
                str ch = l[i];
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
str  c_main::CutLastLevel(str data)
{
    int p;
    for ( int c = 1; c <= data.Length(); c++ )
    {
        str ch = data[c];
        if ( ch == "." ) 
            p = c;
    }

    str j = data.SubString(1,p-1);
    return j;
}
str  c_main::ConvertToVars(str extline)
{
    str j = "";

    TStringList *L = new TStringList;
    L->Text = g.EncryptToList(",",extline);
    for ( int c = 0; c < L->Count; c++ )
    {
        str fx = VARIANTS->Strings[ g.Sti(L->Strings[c])-1 ];
        fx = fx.SubString(9,fx.Length());
        int p = Pos("'",fx);
        fx = fx.SubString(1,p-1);

        j = j + fx + ",";
    }

    j = j.SubString(1,j.Length()-1);
    return j;
}
str  c_main::GetVarFrom(int index)
{
    str fx = VARIANTS->Strings[index];
    fx = fx.SubString(9,fx.Length());
    int p = Pos("'",fx);
    fx = fx.SubString(1,p-1);
    return fx;
}
str  c_main::CreateStageName(str data)
{
    str j;
    if ( IFCREATE )
    {
        if ( IFROOT )
        {
            str  chx = data[1];
            if ( chx == "#" ) 
                j = data;
            else              
                j = "#" + data;
        }
        else
        {
            str  chx = data[1];
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
bool c_main::ifStageNameExist(str StageName)
{
    bool ex = false;
    if ( IFCREATE )
    {
        for ( int c = 0; c < MODEL_LOGICAL->Count; c++ )
        {
            str l = MODEL_LOGICAL->Strings[c];
            if ( l == StageName )
            {
                ex = true;
                break;
            }
        }
    }
    return ex;
}
str  c_main::getLastStage(str data)
{
    str j = "NULL";

    TStringList *L = new TStringList;
    L->Text = data;

    for ( int c = 0; c < L->Count; c++ )
    {
        str line = L->Strings[c];
        if ( Pos("#STAGEDATA=",line) != 0 )
        {
            j = line.SubString(12,line.Length());
        }
    }
    return j;
}
void c_main::log_transform()
{
	TMemoryStream *ms = new TMemoryStream();
	LOG->SaveToStream(ms);
	ms->Position = 0;

	if ( ms->Size > 2000000 )
	{
		if ( BUFF_CURRENTLOG == "NULL" || BUFF_CURRENTLOG.Length() == 0 ) 
            generatelogname();

		LOG->SaveToFile( p_logs + BUFF_CURRENTLOG + ".log" );

		LOG->Clear();
        f->ME_LOG->Lines->Clear();

		generatelogname();
	}

	delete ms;
}
void c_main::generatelogname()
{
	TDateTime D = Date();
	TDateTime T = Time();

	str SD = DateToStr(D);
	str ST = TimeToStr(T);

	for ( int c = 1; c <= ST.Length(); c++ )
	{
		str ch = ST[c];
		if ( ch == ":" ) 
            ST[c] = '.';
    }

	str d = g.Encrypt(1,3,".",SD);
	str m = g.Encrypt(2,3,".",SD);
	str y = g.Encrypt(3,3,".",SD);

	str j = y+"."+m+"."+d+"_"+ST;

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
	TStringList *J = new TStringList;

	str CurrSTAGE;

	// CHECK AUTOANS VARIANTS

	for ( int c = 0; c < f->LV_MODEL_AUTOANS->Items->Count; c++ )
	{
		str tofind = f->LV_MODEL_AUTOANS->Items->Item[c]->SubItems->Strings[0];

		bool ex = false;
		for ( int x = 0; x < MODEL_LOGICAL->Count; x++ )
		{
			str linex = MODEL_LOGICAL->Strings[x];
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

	str tofind = Trim(f->E_MODEL_AUTOANS_DEFAULT->Text);
	if ( tofind.Length() == 0 ) 
        tofind = "NULL";

	bool ex = false;
	for ( int x = 0; x < MODEL_LOGICAL->Count; x++ )
	{
		str linex = MODEL_LOGICAL->Strings[x];

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
		str line = MODEL_LOGICAL->Strings[c];
		if ( Pos("#",line) != 0 ) 
            CurrSTAGE = line;

		if ( Pos("IF'",line) != 0 || Pos("EXTENDED'",line) != 0 || Pos("DEFAULT'",line) != 0 )
		{
			str tofind;

			if ( Pos("DEFAULT'",line) == 0 ) 
                tofind = g.Encrypt(3,3,"'",line);
			else                             
                tofind = g.Encrypt(2,2,"'",line);

			bool ex = false;
			for ( int x = 0; x < MODEL_LOGICAL->Count; x++ )
			{
				str linex = MODEL_LOGICAL->Strings[x];
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
	delete J;
}
void c_main::make_new_inbox(TStringList *ALL, TStringList *DEST)
{
	for ( int c = 0; c < ALL->Count; c++ )
	{
		str aline = ALL->Strings[c];
		str uid = g.Encrypt(3,6,"#",aline);

		bool ex = false;
		for ( int x = 0; x < DEST->Count; x++ )
		{
			str xline = DEST->Strings[x];
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
void c_main::deleteDialogsPerRobot(str robotname)
{
	TStringList *DIALOGS = new TStringList;
	g.GetFiles( p_dialogs ,DIALOGS);

	for ( int c = 0; c < DIALOGS->Count; c++ )
	{
		str DialogFile = p_dialogs + DIALOGS->Strings[c];
		TStringList *DIALOG = new TStringList;
		DIALOG->LoadFromFile( DialogFile );
		str ROBOTGID = DIALOG->Strings[0];
		delete DIALOG;

		if ( ROBOTGID == robotname )
		{
			DeleteFile( DialogFile.t_str() );
        }
	}

	delete DIALOGS;
	conf_dialogs(false);
}
str  c_main::from_list_to_str(TStringList *L)
{
	str a = "";
	for ( int c = 0; c < L->Count; c++ )
	{
		a = a + L->Strings[c] + "~";
	}

	return a;
}
int  c_main::getrobotindex(str robotname, TStringList *L)
{
	int j = -1;
	for ( int c = L->Count-1; c >= 0; c-- )
	{
		str data = L->Strings[c];
		if ( robotname == g.Encrypt(1,2,"|",data) )
		{
			j = c;
			break;
		}
	}

	return j;
}
bool c_main::if_imbanned(str Token, str uid)
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
void c_main::sended_count_add(str robotname)
{
	int cnt = 0;
	for ( int c = 0; c < AUTOANS_LIMIT->Count; c++ )
	{
		str line = AUTOANS_LIMIT->Strings[c];
		str cr = g.Encrypt(1,2,"#",line);
		str cc = g.Encrypt(2,2,"#",line);

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
bool c_main::sended_count_iflimit(str robotname)
{
	int cnt = 0;
	for ( int c = 0; c < AUTOANS_LIMIT->Count; c++ )
	{
		str line = AUTOANS_LIMIT->Strings[c];
		str cr = g.Encrypt(1,2,"#",line);
		str cc = g.Encrypt(2,2,"#",line);

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

		str me = f->ME_LOG->Lines->Strings[ f->ME_LOG->Lines->Count-1 ];
		if ( Pos("Достигнут лимит отправки сообщений [",me) == 0 ) 
        {
            f->main->log(L"Достигнут лимит отправки сообщений [ "+f->e_autoanswerlimit->Text+" ]");
        }
	}

    return j;
}
void c_main::LoadModel(int index)
{
	TStringList *L = new TStringList;
	g.GetFiles( p_robots, L );

    str file_a = p_robots + L->Strings[index] + "\\Hello.txt";
	str file_b = p_robots + L->Strings[index] + "\\Model.txt";
	str file_c = p_robots + L->Strings[index] + "\\AutoAnsRules.txt";
	str file_d = p_robots + L->Strings[index] + "\\AutoAnsDefault.txt";
	str file_e = p_robots + L->Strings[index] + "\\AutoStopKeys.txt";
	str file_f = p_robots + L->Strings[index] + "\\AutoStopPosts.txt";

	delete L;
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

    LoadModelHello();
	LoadModelLogical();
	LoadModelAutoAnsRules();
    if(FileExists(file_d))
    {
	    LoadModelAutoAnsDefault(file_d);
    }
	LoadModelAutoStopKeys();
	LoadModelAutoStopPosts();
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
	TStringList *L = new TStringList;
    GetListOfLevel(level,L);

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
        	str fx = L->Strings[0];
            for ( int i = 0; i < f->TREE->Items->Count; i++ )
            {
            	str l = f->TREE->Items->Item[i]->Text;
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
void c_main::LoadModelAutoAnsRules()
{
	f->LV_MODEL_AUTOANS->Items->Clear();

	for ( int c = 0; c < MODEL_AUTOANS->Count; c++ )
	{
		str dataline = MODEL_AUTOANS->Strings[c];

		TListItem *ListItem;
		ListItem = f->LV_MODEL_AUTOANS->Items->Add();
		ListItem->Caption = "  "+ g.Encrypt(1,2,"~",dataline);
		ListItem->SubItems->Add( g.Encrypt(2,2,"~",dataline) );
	}
}
void c_main::LoadModelAutoAnsDefault(str file)
{
	TStringList *L = new TStringList;

    if(FileExists(file))
    {
	    L->LoadFromFile( file );
    }

	f->E_MODEL_AUTOANS_DEFAULT->Text = Trim( L->Strings[0] );

	delete L;
}
void c_main::LoadModelAutoStopKeys()
{
	f->LV_MODEL_AUTOSTOP_KEYS->Items->Clear();
	for ( int c = 0; c < MODEL_AUTOSTOP_KEYS->Count; c++ )
	{
		str dataline = MODEL_AUTOSTOP_KEYS->Strings[c];

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
		str dataline = MODEL_AUTOSTOP_POSTS->Strings[c];

		TListItem *ListItem;
		ListItem = f->LV_MODEL_AUTOSTOP_POSTS->Items->Add();
		ListItem->Caption = "  "+ dataline;
	}
}

void c_main::LoadModelStage(str StageName)
{
    f->vcl->ModelStageClear();
    f->E_MODEL_LOGICAL_STAGE->Text = StageName;
	TStringList *A = new TStringList;

    bool REC = false;
    for ( int c = 0; c < MODEL_LOGICAL->Count; c++ )
    {
        str l = MODEL_LOGICAL->Strings[c];
        if ( REC && Pos("#",l) != 0 ) 
            break;
        if ( REC ) 
            A->Add(l);
        if ( StageName == l ) 
            REC = true;
    }

    for ( int c = 0; c < A->Count; c++ )
    {
        str l = A->Strings[c];

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

    delete A;
}
void c_main::LoadModelStagePosts()
{
    for ( int c = 0; c < POSTS->Count; c++ )
    {
        str data = POSTS->Strings[c];
        f->LI_MODEL_LOGICAL_POSTS->Items->Add( data.SubString(6,data.Length()) );
    }
}
void c_main::LoadModelStageVariants()
{
    for ( int c = 0; c < VARIANTS->Count; c++ )
    {
        str data = VARIANTS->Strings[c];
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
        str j = "";
        for ( int x = 0; x < VARIANTS->Count; x++ )
        {
            str data = VARIANTS->Strings[x];
            data = data.SubString(9,data.Length());
            int p = Pos("'",data);
            data = data.SubString(1,p-1);

            if ( Pos(data,EXTENDED->Strings[c]) != 0 ) 
                j = j + g.Its(x+1) + ",";
        }

        j = j.SubString(1,j.Length()-1);
        f->LI_MODEL_LOGICAL_EXTENDED->Items->Add( j );
    }

    f->vcl->GetAllStages( f->CB_MODEL_LOGICAL_EXTENDED );
}
void c_main::LoadModelStageDefault()
{
    f->vcl->GetAllStages( f->CB_MODEL_LOGICAL_DEFAULT );

    str fx = DEFAULT->Strings[0];
    fx = fx.SubString(9,fx.Length());

    for ( int c = 0; c < f->CB_MODEL_LOGICAL_DEFAULT->Items->Count; c++ )
    {
        str data = f->CB_MODEL_LOGICAL_DEFAULT->Items->Strings[c];
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
	TStringList *A = new TStringList;
	TStringList *T = new TStringList;

    int index;

	for ( int c = 0; c < MODEL_LOGICAL->Count; c++ )
    {
		str dataline = MODEL_LOGICAL->Strings[c];

        if ( dataline == MYPARENT ) 
            index = c;

        if ( Pos("#",dataline) != 0 && dataline != MYPARENT )
        {
			T->Text = MODEL_LOGICAL->Text;
            GetOnlyOneStage(T,dataline.SubString(2,dataline.Length()));

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

	delete A;
	delete T;
}
void c_main::ModelLogicalDeleteStage(str StageName)
{
	TStringList *DESTMODEL = new TStringList;
	DESTMODEL->Text = MODEL_LOGICAL->Text;

	for ( int c = 0; c < MODEL_LOGICAL->Count; c++ )
    {
		str dataline = MODEL_LOGICAL->Strings[c];
		if ( Pos(StageName,dataline) != 0  )
		{
			ModelLogicalDeleteStageDo(dataline,DESTMODEL);
        }
    }

	MODEL_LOGICAL->Text = DESTMODEL->Text;

    delete DESTMODEL;

	MODEL_LOGICAL->SaveToFile( UnicodeString(FMODEL), TEncoding::UTF8 );
	f->vcl->ModelsSET(false);
	f->vcl->ModelStageClear();

	conf_models( false );
}
void c_main::ModelLogicalDeleteStageDo(str StageName, TStringList *MODEL)
{
	TStringList *T = new TStringList;

	bool start_fi = false;
	int stophere = 0;

	for ( int c = 0; c < MODEL->Count; c++ )
	{
		str line = MODEL->Strings[c];

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

	ModelLogicalDeleteStageDeleteLinked(StageName,T);
	MODEL->Text = T->Text;

    delete T;
}
void c_main::ModelLogicalDeleteStageDeleteLinked(str StageName, TStringList *MODEL)
{
	for ( int c = 0; c < MODEL->Count; c++ )
	{
		str line = MODEL->Strings[c];
		str myStage = StageName.SubString(2,StageName.Length());
		if ( Pos("'"+myStage,line) > 0 )
		{
			if ( Pos("DEFAULT'",line) == 0 )
			{
				if ( Pos("IF'",line) != 0 || Pos("EXTENDED'",line) != 0 )
				{
					str a = g.Encrypt(1,3,"'",line);
					str b = g.Encrypt(2,3,"'",line);

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
	str iGroup = Trim( f->LV_CONF_GROUPS->Items->Item[f->LV_CONF_GROUPS->ItemIndex]->Caption );
	str iName  = Trim( f->LV_CONF_ROBOTS->Items->Item[f->LV_CONF_ROBOTS->ItemIndex]->Caption );

	TStringList *L = new TStringList;
	g.GetFiles( p_robots, L );

	bool breaked = false;
	for ( int c = 0; c < L->Count; c++ )
	{
		str robotpath = p_robots + L->Strings[c] + "\\" + "Conf.ini";
		TMemIniFile *INI = new TMemIniFile( UnicodeString(robotpath), TEncoding::UTF8 );
		str xGroup = INI->ReadString( UnicodeString("MAIN"), UnicodeString("owner"), UnicodeString("") );
		str xName  = INI->ReadString( UnicodeString("MAIN"), UnicodeString("name"), UnicodeString("") );

		if ( iGroup == xGroup && iName == xName )
		{
			INI->WriteBool( UnicodeString("MAIN"), UnicodeString("freeze"), true );
            INI->UpdateFile();
			breaked = true;
		}
		delete INI;

		if ( breaked ) 
        {
            break;
        }
	}

	delete L;
	conf_robots( f->LV_CONF_GROUPS->ItemIndex, false);
	f->vcl->groupechoReadRobots();
}
void c_main::Robot_UnFreeze(int index)
{
	str iGroup = Trim( f->LV_CONF_GROUPS->Items->Item[f->LV_CONF_GROUPS->ItemIndex]->Caption );
	str iName  = Trim( f->LV_CONF_ROBOTS->Items->Item[f->LV_CONF_ROBOTS->ItemIndex]->Caption );

	TStringList *L = new TStringList;
	g.GetFiles( p_robots, L );

	bool breaked = false;
	for ( int c = 0; c < L->Count; c++ )
	{
		str robotpath = p_robots + L->Strings[c] + "\\" + "Conf.ini";
		TMemIniFile *INI = new TMemIniFile( UnicodeString(robotpath), TEncoding::UTF8 );
		str xGroup = INI->ReadString( UnicodeString("MAIN"), UnicodeString("owner"), UnicodeString("") );
		str xName  = INI->ReadString( UnicodeString("MAIN"), UnicodeString("name"), UnicodeString("") );

		if ( iGroup == xGroup && iName == xName )
		{
			INI->WriteBool( UnicodeString("MAIN"), UnicodeString("freeze"), false );
            INI->UpdateFile();
			breaked = true;
		}

		delete INI;

		if ( breaked ) 
        {
            break;
        }
	}

	delete L;
	conf_robots( f->LV_CONF_GROUPS->ItemIndex, false);
	f->vcl->groupechoReadRobots();
}
void c_main::Robots_Freeze()
{
	str iGroup = Trim( f->LV_CONF_GROUPS->Items->Item[f->LV_CONF_GROUPS->ItemIndex]->Caption );

	TStringList *L = new TStringList;
	g.GetFiles( p_robots, L );

	for ( int c = 0; c < L->Count; c++ )
	{
		str robotpath = p_robots + L->Strings[c] + "\\" + "Conf.ini";
		TMemIniFile *INI = new TMemIniFile( UnicodeString(robotpath), TEncoding::UTF8 );
		str xGroup = INI->ReadString( UnicodeString("MAIN"), UnicodeString("owner"), UnicodeString("") );

		if ( iGroup == xGroup ) 
        {
            INI->WriteBool( UnicodeString("MAIN"), UnicodeString("freeze"), true );
            INI->UpdateFile();
        }
		delete INI;
	}

	delete L;
	conf_robots( f->LV_CONF_GROUPS->ItemIndex, false);
	f->vcl->groupechoReadRobots();
}
void c_main::Robots_UnFreeze()
{
	str iGroup = Trim( f->LV_CONF_GROUPS->Items->Item[f->LV_CONF_GROUPS->ItemIndex]->Caption );

	TStringList *L = new TStringList;
	g.GetFiles( p_robots, L );

	for ( int c = 0; c < L->Count; c++ )
	{
		str robotpath = p_robots + L->Strings[c] + "\\" + "Conf.ini";
        TMemIniFile *INI = new TMemIniFile( UnicodeString(robotpath), TEncoding::UTF8 );
		str xGroup = INI->ReadString( UnicodeString("MAIN"), UnicodeString("owner"), UnicodeString("") );

		if ( iGroup == xGroup ) 
        {
            INI->WriteBool( UnicodeString("MAIN"), UnicodeString("freeze"), false );
            INI->UpdateFile();
        }
		delete INI;
	}

	delete L;
	conf_robots( f->LV_CONF_GROUPS->ItemIndex, false);
	f->vcl->groupechoReadRobots();
}
void c_main::import_robots(str file)
{
	try
	{
		TStringList *L = new TStringList;
        if(FileExists(file))
        {
		    L->LoadFromFile(file);
        }

		for ( int c = 0; c < L->Count; c++ )
		{
			str dataline = Trim( L->Strings[c] );

			str r_name   = get_fromfile(1,dataline);
			str r_login  = get_fromfile(2,dataline);
			str r_pass   = get_fromfile(3,dataline);

			conf_robots_import( f->LV_CONF_GROUPS->ItemIndex, r_name, r_login, r_pass );
		}

		f->vcl->groupechoReadRobots();

		delete L;
	}
	catch (...)
	{
		ShowMessage(L"Ошибка импорта роботов из файла.");
    }
}

str  c_main::get_fromfile(int i, str data)
{
	str j = "NULL";

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

	str A = Trim( data.SubString(1,p-1) );
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

	str B = Trim( data.SubString(1,p-1) );
	str C = Trim( data.SubString(p+1,data.Length()) );

	if ( i == 1 ) j = A;
	if ( i == 2 ) j = B;
	if ( i == 3 ) j = C;

    return j;
}
str  c_main::ConvertErrors(str edata)
{
	str j = edata;
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
str  c_main::GetPHPLINE(str data)
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
		TIdHTTP *HTTP = new TIdHTTP(NULL);
		HTTP->HandleRedirects = true;
		HTTP->ConnectTimeout 	= 10000;
		HTTP->ReadTimeout 		= 10000;

		str URL = HTTPDOMAIN + "API/get_servers.php";
		str J = HTTP->Get( URL );

		delete HTTP;

		J = GetPHPLINE( J );

		TStringList *A = new TStringList;
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
void c_main::asplit(TStringList* lout, str s, str separator)
{
	str buff = "";
	for ( int c = 1; c <= s.Length(); c++ )
	{
		str ch = s[c];
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
bool c_process::Establish( str RobotName, str *Token )
{
	str IniFile, GroupName, CurRobotName, Server_ID, Login, Password, Activity, token, Online, LastOnline;  
    bool freeze;

	TStringList *L = new TStringList;
	g.GetFiles( f->main->p_robots, L );
	for ( int c = 0; c < L->Count; c++ )
	{
		f->main->get_robotdata( c, &GroupName, &CurRobotName, &Server_ID, &Login, &Password, &token, &Activity, &Online, &LastOnline, &freeze );
		if ( CurRobotName == RobotName )
		{
			IniFile = f->main->p_robots + L->Strings[c] + "\\Conf.ini";
			break;
        }
	}
	delete L;

	str client_id = f->LV_SERVERS->Items->Item[ f->main->CurrentServer ]->SubItems->Strings[0];
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
			str DT = DateToStr(D) + " - " + TimeToStr(T);

            TMemIniFile *INI = new TMemIniFile( UnicodeString(IniFile), TEncoding::UTF8 );
			INI->WriteString(UnicodeString("CONNECTION"), UnicodeString("activity"),  UnicodeString(DT));
			INI->WriteString(UnicodeString("CONNECTION"), UnicodeString("client_id"), UnicodeString(client_id));
			INI->WriteString(UnicodeString("CONNECTION"), UnicodeString("token"),     UnicodeString(token));
            INI->UpdateFile();
			delete INI;

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
str  c_process::SendMessage( str UserID, str Message, str Token )
{
	str response;
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
		str CaptchaSID = vk.GetParameter("captcha_sid",&response,false,false,&rec);
		str CaptchaIMG = vk.GetParameter("captcha_img",&response,false,false,&rec);

		f->main->log( L"Запрашивается каптча: [ " + CaptchaSID + " ] [ " + f->captcha->FixURL(CaptchaIMG) + " ]" );
		str CaptchaANS = f->captcha->GetAnswer( CaptchaIMG );
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
str  c_process::AddToFriends( str UserID, str Message, str Token )
{
	str response;
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
		str CaptchaSID = vk.GetParameter("captcha_sid",&response,false,false,&rec);
		str CaptchaIMG = vk.GetParameter("captcha_img",&response,false,false,&rec);
		f->main->log( L"Запрашивается каптча: [ " + CaptchaSID + " ] [ " + f->captcha->FixURL(CaptchaIMG) + " ]" );
		str CaptchaANS = f->captcha->GetAnswer( CaptchaIMG );
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
str  c_process::RequestsFriends( str Token )
{
	str response;

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
	f->main->log(L"");
	f->main->logline(L"[ ПРОЦЕСС ЗАПУЩЕН ]");

	f->main->AUTOANSBUFF->Clear();
	f->main->AUTOANSOUTBOX->Clear();

	f->main->INBOX->Clear();

	for ( int c = 0; c < f->LV_WORKGROUPS->Items->Count; c++ )
	{
        if ( f->LV_WORKGROUPS->Items->Item[c]->Checked )
		{
			str ActiveGroup = Trim(f->LV_WORKGROUPS->Items->Item[c]->Caption);

			f->main->logline(L"");
			f->main->log(L"ГРУППА : [ "+ActiveGroup+" ]");
			f->main->logline(L"");
			f->main->logline(L"");

			try
			{
				if ( f->LV_WORKTASKS->Items->Item[0]->Checked )
				{
					f->main->PREFIX = L"Приветствия : ";
					ProcessHello(ActiveGroup);
				}
				if ( f->main->TERMINATED ) 
                {
                    break;
                }
			}
			catch ( Exception *ex ) 
            { 
                f->main->log(L"FATAL ERROR: [ МОДУЛЬ : ПРИВЕТСТВИЯ ]"); 
            }

			TStringList *L = new TStringList;
			g.GetFiles( f->main->p_robots, L );
			str GroupName, RobotName, Server_ID, Login, Password, Token, Activity, ModelFile, Online, LastOnline;  
            bool freeze;
			for ( int x = 0; x < L->Count; x++ )
			{
				ModelFile             = f->main->p_robots + L->Strings[x] + "\\Model.txt";
				str AutoStopKeysFile  = f->main->p_robots + L->Strings[x] + "\\AutoStopKeys.txt";
				str AutoStopPostsFile = f->main->p_robots + L->Strings[x] + "\\AutoStopPosts.txt";
				f->main->get_robotdata( x, &GroupName, &RobotName, &Server_ID, &Login, &Password, &Token, &Activity, &Online, &LastOnline, &freeze );

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
						f->main->logline(L"");
					}

					f->main->PREFIX = "[ "+RobotName+L" ] Соединение : ";

					if ( Establish(RobotName,&Token) )
					{
						try
						{
							if ( f->LV_WORKTASKS->Items->Item[1]->Checked )
							{
								f->main->PREFIX = "[ "+RobotName+L" ] Приём заявок : ";
								ProcessFriendConfirm(GroupName, RobotName, Server_ID, Login, Password, Token, Activity, ModelFile);
							}
							if ( f->main->TERMINATED ) 
                            {
                                break;
                            }
						}
						catch ( Exception *ex ) 
                        { 
                            f->main->log(L"FATAL ERROR: [ МОДУЛЬ : ПРИЁМ ЗАЯВОК ]"); 
                        }

						try
						{
							if ( f->LV_WORKTASKS->Items->Item[2]->Checked )
							{
								f->main->PREFIX = "[ "+RobotName+L" ] Автоответчик : ";
								ProcessAutoAnswer(GroupName, RobotName, Server_ID, Login, Password, Token, Activity, ModelFile);
							}
							if ( f->main->TERMINATED ) 
                            {
                                break;
                            }
						}
						catch ( Exception *ex ) 
                        { 
                            f->main->log(L"FATAL ERROR: [ МОДУЛЬ : АВТООТВЕТЧИК ]"); 
                        }

						try
						{
							if ( f->LV_WORKTASKS->Items->Item[6]->Checked )
							{
								f->main->PREFIX = "[ "+RobotName+L" ] Сообщ -> GlobalUsersCache : ";
								ProcessMessages_UIDS_To_GlobalUsersCache(RobotName, Token);
							}
							if ( f->main->TERMINATED ) 
                            {
                                break;
                            }
						}
						catch ( Exception *ex ) 
                        { 
                            f->main->log(L"FATAL ERROR: [ МОДУЛЬ : Сообщ -> GlobalUsersCache ]"); 
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

	if ( f->main->AUTOANSBUFF->Count > 0 ) 
    {
        ProcessAutoAnswerRunBuffer();
    }

	f->main->PREFIX = "";

	if ( f->main->TERMINATED ) 
    {
        f->main->logline(L"[ ПРОЦЕСС БЫЛ ПРЕРВАН ]");
    }
	else                         
    {
        f->main->logline(L"[ ПРОЦЕСС УСПЕШНО ЗАВЕРШЕН ]");
    }
}
void c_process::ProcessHello(str GroupName)
{
	f->BAR->Position = 0;
	f->BAR->Max      = f->main->getCountOfHello(GroupName);
	g.ProcessMessages();

	TStringList *L = new TStringList;
    if(FileExists(f->main->f_users))
    {
	    L->LoadFromFile( f->main->f_users );
    }
	f->main->TGC = 0;

	for ( int x = 0; x < L->Count; x++ )
	{
		str GROUP    = Trim( g.Encrypt(1,5,"#",L->Strings[x]) );
		str UID 	 = Trim( g.Encrypt(2,5,"#",L->Strings[x]) );
		str NAME     = Trim( g.Encrypt(3,5,"#",L->Strings[x]) );
		str SURNAME  = Trim( g.Encrypt(4,5,"#",L->Strings[x]) );

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

	delete L;
	g.ProcessMessages();
}
void c_process::ProcessHelloDo(str Group, str UserID, str UserName, str UserSurname)
{
	TStringList *XL = new TStringList;
	TStringList *L = new TStringList;
	g.GetFiles(f->main->p_robots,L);
    
	for ( int c = 0; c < L->Count; c++ )
	{
		str CurGroupName, RobotName, Server_ID, Login, Password, Token, Activity, Online, LastOnline;  
        bool freeze;
		f->main->get_robotdata( c, &CurGroupName, &RobotName, &Server_ID, &Login, &Password, &Token, &Activity, &Online, &LastOnline, &freeze );
		if ( CurGroupName == Group ) 
        {
            XL->Add(L->Strings[c]);
        }
	}

    int robotIndex = f->main->get_robotnext( Group );
	str RobotPath   = f->main->p_robots + XL->Strings[robotIndex] + "\\";
	str RobotConfig = RobotPath + "Conf.ini";

	TMemIniFile *INI = new TMemIniFile( UnicodeString(RobotConfig),TEncoding::UTF8 );
	str iGroupName 	= INI->ReadString( UnicodeString("MAIN"),       UnicodeString("owner"),     UnicodeString("0") );
	str iRobotName  = INI->ReadString( UnicodeString("MAIN"),       UnicodeString("name"),  	  UnicodeString("0") );
	str iLogin 		= INI->ReadString( UnicodeString("ACCOUNT"),    UnicodeString("login"),     UnicodeString("0") );
	str iPassword  	= INI->ReadString( UnicodeString("ACCOUNT"),    UnicodeString("password"),  UnicodeString("0") );
	str iActivity 	= INI->ReadString( UnicodeString("CONNECTION"), UnicodeString("activity"),  UnicodeString("0") );
	str iServer_ID 	= INI->ReadString( UnicodeString("CONNECTION"), UnicodeString("client_id"), UnicodeString("0") );
	str iToken     	= INI->ReadString( UnicodeString("CONNECTION"), UnicodeString("token"),     UnicodeString("0") );
	delete INI;
	delete L;
	delete XL;

	ProcessHelloDoSend(Group,UserID,UserName,UserSurname,iRobotName,iToken,RobotPath);
}
void c_process::ProcessHelloDoSend(str Group, str UserID, str UserName, str UserSurname, str RobotName, str Token, str RobotPath)
{
	TStringList *L = new TStringList;
	L->LoadFromFile(RobotPath+"Hello.txt");
	str LINE = L->Strings[ Random( L->Count ) ];

	f->main->log(L"Робот : [ " + RobotName + " ]");

	if ( Establish(RobotName,&Token) )
	{
		f->main->log(L"Пользователь: [ " + UserID + " ] , [ " + UserName + " " + UserSurname + " ]");
		str response = SendMessage(UserID,LINE,Token);

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
				f->main->log(L"Attach текст: [ " + LINE + " ]" );
				f->main->log(L"Запрос на добавление в друзья успешно подан.");
				f->main->CNT_ADDFRIEND++;
				f->main->DeleteUserFromQueueAndPutToDialogs(UserID,UserName,UserSurname,RobotName,false,LINE);
			}
		}

		f->main->logline(L"");
	}

	delete L;
}
void c_process::ProcessFriendConfirm(str GroupName, str RobotName, str Server_ID, str Login, str Password, str Token, str Activity, str ModelFile)
{
	str response = RequestsFriends(Token);

	if ( Pos("error",response) == 0 )
	{
		int CountOf = StrToInt( vk.GetParameter("count",&response,false,true,false) );
		f->main->log(L"Список заявок получен ( " + IntToStr(CountOf) + " )" );
		if ( CountOf > 0 )
		{
			for ( int cnt = 1; cnt <= CountOf; cnt++ )
			{
				str UserID      = "NULL";
				str UserMESSAGE = "NULL";

				vk.GetParameterIntString(cnt,"user_id","message",&UserID,&UserMESSAGE,response);

				if ( UserMESSAGE == "NULL" ) 
                {
                    f->main->log(L"От пользователя: [ " + UserID + " ]");
                }
				else                         
                {
                    f->main->log(L"От пользователя: [ " + UserID + " ] Attach òåêñò: [ " + UserMESSAGE +" ]" );
                }

				str response;
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

					str code177 = "error_code\":177";
					if ( Pos(code177,response) != 0 )
					{
						f->main->log(L"Ошибка 177. Страница пользователя удалена.");

						str response;

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

					str code1 = "error_code\":1";
					if ( Pos(code1,response) != 0 )
					{
						f->main->log(L"Ошибка 1. Робот находится в чёрном списке.");

						str response;

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

	f->main->logline(L"");
}
void c_process::ProcessAutoAnswer(str GroupName, str RobotName, str Server_ID, str Login, str Password, str Token, str Activity, str ModelFile)
{
	f->main->WriteInboxList(Token);

	f->BAR->Position = 0;
	f->BAR->Max      = f->main->INBOX->Count;
	g.ProcessMessages();

	if ( ! f->main->TERMINATED ) 
    {
        ProcessAutoAnswerCheckNew(RobotName,Token);
    }
	f->main->logline(L"");
}
void c_process::ProcessAutoAnswerCheckNew(str RobotName, str Token)
{
	TStringList *NEW = new TStringList;

    TStringList *DIALOGS = new TStringList;
	g.GetFiles( f->main->p_dialogs ,DIALOGS);

	for ( int c = f->main->INBOX->Count-1; c >= 0 ; c-- )
	{
		str utoken          = g.Encrypt(1,6,"#",f->main->INBOX->Strings[c]);
		str mid 			= g.Encrypt(2,6,"#",f->main->INBOX->Strings[c]);
		str uid 			= g.Encrypt(3,6,"#",f->main->INBOX->Strings[c]);
		str read_state 		= g.Encrypt(4,6,"#",f->main->INBOX->Strings[c]);
		str title           = g.Encrypt(5,6,"#",f->main->INBOX->Strings[c]);
		str body            = g.Encrypt(6,6,"#",f->main->INBOX->Strings[c]);

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
            NEW->Add( f->main->INBOX->Strings[c] );
        }

		f->BAR->Position++;
		f->vcl->EchoStatistic();
		g.ProcessMessages();

		if ( f->main->TERMINATED ) 
        {
            break;
        }
	}

	delete DIALOGS;

	if ( NEW->Count > 0 )
	{
		TStringList *USERS = new TStringList;
		f->main->make_new_inbox(NEW,USERS);
		for ( int c = 0; c < USERS->Count; c++ )
		{
			str uid = USERS->Strings[c];

			if ( f->main->TERMINATED ) 
            {
                break;
            }

			TStringList *USERMESSAGES = new TStringList;
			for ( int i = 0; i < NEW->Count; i++ )
			{
				str nuid = g.Encrypt(3,6,"#",NEW->Strings[i]);
				if ( nuid == uid ) 
                {
                    USERMESSAGES->Add( NEW->Strings[i] );
                }
			}

			f->main->log(L"Сообщения от нового пользователя [ "+uid+" ] [ "+IntToStr(USERMESSAGES->Count)+" ]");
			ProcessAutoAnswerAddToDialogs(USERMESSAGES,RobotName,Token,uid);
			f->main->GlobalUsersCache_Add(uid);

			delete USERMESSAGES;

			if ( f->main->TERMINATED ) 
            {
                break;
            }
        }
		delete USERS;
	}
	else
	{
		f->main->log(L"Сообщений от новых пользователей [ 0 ]");
	}

	delete NEW;
}
void c_process::ProcessAutoAnswerAddToDialogs(TStringList *NEWBOX, str RobotName, str Token, str Uid)
{
	str file = f->main->p_dialogs;

	TStringList *DIALOG = new TStringList;
	TStringList *MESSAGEDATA = new TStringList;

	DIALOG->Add(RobotName);
	DIALOG->Add("A_NULL");
	DIALOG->Add("A_NULL");
	DIALOG->Add(Uid);

	str lastmid = "JNULL";

	for ( int c = 0; c < NEWBOX->Count; c++ )
	{
		str utoken          = g.Encrypt(1,6,"#",NEWBOX->Strings[c]);
		str mid 			= g.Encrypt(2,6,"#",NEWBOX->Strings[c]);        
        lastmid = mid;
		str uid 			= g.Encrypt(3,6,"#",NEWBOX->Strings[c]);
		str read_state 		= g.Encrypt(4,6,"#",NEWBOX->Strings[c]);
		str title           = g.Encrypt(5,6,"#",NEWBOX->Strings[c]);
		str body            = g.Encrypt(6,6,"#",NEWBOX->Strings[c]);

		TDateTime D = Date();
		TDateTime T = Time();
		str DT = DateToStr(D) + " - " + TimeToStr(T);
		DIALOG->Add("");
		DIALOG->Add("#MESSAGE");
		DIALOG->Add("#INC");
		DIALOG->Add("#DATE:"+DT);
		DIALOG->Add("#NEW=1");
		DIALOG->Add("#ID="+mid);
		DIALOG->Add("#STAGEDATA=AUTOANS");
		DIALOG->Add("#BEGIN");
		TStringList *INCDATA = new TStringList; 
        INCDATA->Text = body;
        
		for ( int i = 0; i < INCDATA->Count; i++ ) 
        { 
            DIALOG->Add( INCDATA->Strings[i] ); 
        } 
        
        delete INCDATA;
        
		DIALOG->Add("#END");
		MESSAGEDATA->Add(body);
	}

	f->main->AUTOANSBUFF->Add( RobotName+"|"+Token+"|"+file+Uid+"|"+f->main->from_list_to_str(DIALOG)+"|"+f->main->from_list_to_str(MESSAGEDATA)+"|"+Uid+"|"+lastmid );
	f->main->log( L"Добавлено в буффер выполнения." );

	delete DIALOG;
	delete MESSAGEDATA;
}
void c_process::ProcessAutoAnswerRunBuffer()
{
	f->main->PREFIX = L"Автоответчик : ";
	f->main->log(L"Отправка сообщений из буффера:");

	TStringList *L = new TStringList;
	while ( f->main->AUTOANSBUFF->Count > 0 )
	{
		int indexToAdd = 0; 
        int indexToAddLevel = 999999999;

		for ( int c = 0; c < f->main->AUTOANSBUFF->Count; c++ )
		{
			str robot = g.Encrypt(1,2,"|",f->main->AUTOANSBUFF->Strings[c]);

			int thisIndex = f->main->getrobotindex(robot,L);
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
        TStringList *MESSAGEDATA 	= new TStringList;
		TStringList *DIALOG      	= new TStringList;

		str RobotName 	= g.Encrypt(1,7,"|",L->Strings[c]);
		str Token 		= g.Encrypt(2,7,"|",L->Strings[c]);
		str file 		= g.Encrypt(3,7,"|",L->Strings[c]);
		str dialog 		= g.Encrypt(4,7,"|",L->Strings[c]);
		str message 	= g.Encrypt(5,7,"|",L->Strings[c]);
		str uid 		= g.Encrypt(6,7,"|",L->Strings[c]);
		str lastmid  	= g.Encrypt(7,7,"|",L->Strings[c]);

		if ( ! f->main->sended_count_iflimit(RobotName) )
		{
			str responsedatax;

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
			str uname    = vk.GetParameter("first_name",&responsedatax,false,false,&rec);
			str usurname = vk.GetParameter("last_name",&responsedatax,false,false,&rec);

			DIALOG->Text 		= g.EncryptToList("~",dialog);
			MESSAGEDATA->Text 	= g.EncryptToList("~",message);

			DIALOG->Strings[1] = uname;
			DIALOG->Strings[2] = usurname;

			f->main->PREFIX = L"Автоответчик : [ "+RobotName+ " - " + uname + " " + usurname + " ] ";

			bool success = true;
			bool e900 = false;
			bool e902 = false;
			SendAutoAns(RobotName,uid,MESSAGEDATA->Text,Token,DIALOG,&success,&e900,&e902);
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

			delete MESSAGEDATA;
			delete DIALOG;

			if ( f->main->TERMINATED ) 
            {
                break;
            }
		}
	}

	delete L;
}
void c_process::ProcessSpeech(str GroupName, str RobotName, str Server_ID, str Login, str Password, str Token, str Activity, str ModelGlobalFile, str ModelFile, str AutoStopKeysFile, str AutoStopPostsFile)
{
	f->main->WriteInboxList(Token);

	TStringList *DIALOGS = new TStringList;
	g.GetFiles( f->main->p_dialogs ,DIALOGS);

	f->BAR->Position = 0;
	f->BAR->Max      = DIALOGS->Count;
	g.ProcessMessages();

	for ( int c = 0; c < DIALOGS->Count; c++ )
	{
		str DialogFile = f->main->p_dialogs + DIALOGS->Strings[c];
		TStringList *DIALOG = new TStringList;
		DIALOG->LoadFromFile( DialogFile );

		str ROBOTGID = DIALOG->Strings[0];
		str NAME     = DIALOG->Strings[1];
		str SURNAME  = DIALOG->Strings[2];
		str USERID   = DIALOG->Strings[3];

		if ( RobotName == ROBOTGID )
		{
			if ( Pos("#STAGEDATA=FIN",DIALOG->Text) == 0 )
			{
				bool SaveDialog = true;
				ProcessSpeechUser(ROBOTGID,NAME,SURNAME,USERID,DIALOG,Token,ModelGlobalFile,ModelFile,&SaveDialog,AutoStopKeysFile,AutoStopPostsFile);

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

		delete DIALOG;

		f->BAR->Position++;
		g.ProcessMessages();

		if ( f->main->TERMINATED ) 
        {   
            break;
        }
	}

	delete DIALOGS;

	f->main->logline("");
}
void c_process::ProcessSpeechUser(str RobotGID, str Name, str SurName, str UserID, TStringList *DIALOG, str Token, str RobotModelGlobalFile, str RobotModelFile, bool *SaveDialog, str AutoStopKeysFile, str AutoStopPostsFile)
{
	f->main->log(L"Обработка диалога [ "+Name+" "+SurName+" ] [ "+UserID+" ]");
    
	TStringList *NEWSTACK = new TStringList;
	str inboxIDS = "";

	for ( int c = f->main->INBOX->Count-1; c >= 0 ; c-- )
	{
		str utoken          = g.Encrypt(1,6,"#",f->main->INBOX->Strings[c]);
		str mid 			= g.Encrypt(2,6,"#",f->main->INBOX->Strings[c]);
		str uid 			= g.Encrypt(3,6,"#",f->main->INBOX->Strings[c]);
		str read_state 		= g.Encrypt(4,6,"#",f->main->INBOX->Strings[c]);
		str title           = g.Encrypt(5,6,"#",f->main->INBOX->Strings[c]);
		str body            = g.Encrypt(6,6,"#",f->main->INBOX->Strings[c]);

		if ( utoken == Token && uid == UserID && 
             Pos("#ID="+mid,DIALOG->Text) == 0 )
		{
			f->main->log(L"Новое входящее сообщение: [ ID:" + mid + L" ] Текст: [ "+body+" ]");

			inboxIDS = inboxIDS + mid + ",";

			TDateTime D = Date();
			TDateTime T = Time();
			str DT = DateToStr(D) + " - " + TimeToStr(T);
			DIALOG->Add("");
			DIALOG->Add("#MESSAGE");
			DIALOG->Add("#INC");
			DIALOG->Add("#DATE:"+DT);
			DIALOG->Add("#NEW=1");
			DIALOG->Add("#ID="+mid);
			DIALOG->Add("#STAGEDATA=INC");
			DIALOG->Add("#BEGIN");
			TStringList *INCDATA = new TStringList; 
            INCDATA->Text = body;
			for ( int i = 0; i < INCDATA->Count; i++ ) 
            { 
                DIALOG->Add( INCDATA->Strings[i] ); 
                NEWSTACK->Add( INCDATA->Strings[i] ); 
            } 
            delete INCDATA;
			DIALOG->Add("#END");
		}
	}

	bool makereaded = false;
	if ( inboxIDS.Length() != 0 ) 
    {
        ProcessSpeechUserDo(UserID,DIALOG,NEWSTACK->Text,Token,RobotModelGlobalFile,RobotModelFile,SaveDialog,AutoStopKeysFile,AutoStopPostsFile,&makereaded);
    }

	if ( *SaveDialog )
	{
		if ( inboxIDS.Length() > 0 )
		{
			str tidmsgs = inboxIDS.SubString(1,inboxIDS.Length()-1);

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

	if ( makereaded )
	{
		if ( inboxIDS.Length() > 0 )
		{
			str tidmsgs = inboxIDS.SubString(1,inboxIDS.Length()-1);

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

	delete NEWSTACK;
}
void c_process::ProcessSpeechUserDo(str UserID, TStringList *DIALOG, str STACK, str Token, str RobotGlobalModelFile, str RobotModelFile, bool *SaveDialog, str AutoStopKeysFile, str AutoStopPostsFile, bool *MakeReaded)
{
	TStringList *GLOBALMODEL = new TStringList;
	TStringList *MODEL = new TStringList;
	TStringList *STAGE = new TStringList;
    
	GLOBALMODEL->LoadFromFile(RobotGlobalModelFile);
	MODEL->LoadFromFile(RobotModelFile);
	STAGE->Text = MODEL->Text;

	str CURRENT_STAGE = f->main->GetLastStageName(DIALOG);
	f->main->log(L"Текущий stage: [ #"+CURRENT_STAGE+" ]");

	if ( CURRENT_STAGE == "HELLO" ) 
    {
        SendStage1(UserID,DIALOG,STACK,Token,MODEL);
    }
	else
	{
		f->main->GetOnlyOneStage(STAGE,CURRENT_STAGE);
		if ( STAGE->Strings[0] != "NULL" )
		{
			str TARGET_STAGE = f->ii->GetStage(STACK,STAGE);
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
					str GLOGALPOST = f->ii->GetGlobalPostText(STACK,GLOBALMODEL,DIALOG);
					if ( GLOGALPOST == "NULL" ) 
                    {
                        SendStageX(UserID,DIALOG,STACK,Token,MODEL,TARGET_STAGE);
                    }
					else                        
                    {
                        SendGlobalPost(UserID,DIALOG,GLOGALPOST,Token,MODEL,CURRENT_STAGE);
                    }
				}
				else 
                {
                    SendAutoStop(UserID,DIALOG,STACK,Token,AutoStopPostsFile);
                }
			}
		}
	}

	delete GLOBALMODEL;
	delete MODEL;
	delete STAGE;
}
void c_process::ProcessMessages_UIDS_To_GlobalUsersCache( str RobotName, str Token )
{
	TStringList *UIDS = new TStringList;
	f->main->GetDialogs(UIDS, 2, 2, Token, RobotName);
	for ( int c = 0; c < UIDS->Count; c++ )
	{
		str UID = UIDS->Strings[c];
		if ( ! f->main->GlobalUsersCache_Exist( UID ) )
		{
			f->main->GlobalUsersCache_Add( UID );
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
void c_process::SendStage1(str UserID, TStringList *DIALOG, str STACK, str Token, TStringList *MODEL)
{
	f->main->log(L"Отправка первого stage.");

	str STAGENAME;

	TStringList *STAGE = new TStringList;                  STAGE->Text = MODEL->Text;
	TStringList *POST = new TStringList;

	for ( int c = 0; c < MODEL->Count; c++ )
	{
		str l = MODEL->Strings[c];
		if ( Pos("#",l) != 0 )
		{
			STAGENAME = l.SubString(2,l.Length());
			f->main->GetOnlyOneStage(STAGE,STAGENAME);

			for ( int x = 0; x < STAGE->Count; x++ )
			{
				str ln = STAGE->Strings[x];
				if ( Pos("POST'",ln) != 0 )
				{
					POST->Add( g.Encrypt(2,2,"'",ln) );
				}
            }
			break;
        }
	}

	delete STAGE;

	str DATAPOST = POST->Strings[ Random( POST->Count ) ];
	delete POST;

	str MESSID = SendMessage( UserID, DATAPOST, Token );
	if ( Pos("error",MESSID) == 0 )
	{
		f->main->log(L"Текст: [ " + DATAPOST + " ]" );
		f->main->log(L"Сообщение успешно отправлено. (^_^)");

		TDateTime D = Date();
		TDateTime T = Time();
		str DT = DateToStr(D) + " - " + TimeToStr(T);
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
}
void c_process::SendStageX(str UserID, TStringList *DIALOG, str STACK, str Token, TStringList *MODEL, str TARGET_STAGE)
{
	f->main->log(L"Отправка stage: [ #"+TARGET_STAGE+" ]");

	TStringList *STAGE = new TStringList;
	STAGE->Text = MODEL->Text;
	TStringList *POST = new TStringList;

	f->main->GetOnlyOneStage(STAGE,TARGET_STAGE);
	for ( int x = 0; x < STAGE->Count; x++ )
	{
		str ln = STAGE->Strings[x];
		if ( Pos("POST'",ln) != 0 )
		{
			POST->Add( g.Encrypt(2,2,"'",ln) );
		}
	}

	delete STAGE;
	str DATAPOST = POST->Strings[ Random( POST->Count ) ];
	delete POST;

	str MESSID = SendMessage( UserID, DATAPOST, Token );
    if ( Pos("error",MESSID) == 0 )
	{
		f->main->log(L"Текст: [ " + DATAPOST + " ]" );
		f->main->log(L"Сообщение успешно отправлено. (^_^)");

		TDateTime D = Date();
		TDateTime T = Time();
		str DT = DateToStr(D) + " - " + TimeToStr(T);
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
}
void c_process::SendAutoAns(str RobotName, str UserID, str MessageData, str Token, TStringList *DIALOG, bool *success, bool *e900, bool *e902)
{
	f->main->WriteOutboxAutoAnsList(Token,RobotName);
	f->main->log(L"Входящий текст: [ "+ Trim( MessageData.SubString(1,MessageData.Length()-3) ) +" ]");

	str TARGETSTAGE = "NULL";
	f->ii->GetAutoAns(&MessageData,&TARGETSTAGE,Token,UserID);
	f->main->log(L"Отправка stage: [ "+TARGETSTAGE+" ]");

    TStringList *IMAGELIST = new TStringList;
	TStringList *AUDIOLIST = new TStringList;
	TStringList *RECORDLIST = new TStringList;

	f->ii->WritePostDataIn(IMAGELIST,AUDIOLIST,RECORDLIST,&MessageData);
	MessageData = f->ii->MakePostLine(MessageData);
	str MESSID = SendMessage( UserID, MessageData, Token );

	if ( Pos("error",MESSID) == 0 )
	{
		f->main->log(L"Исходящий текст: [ " + MessageData + " ]" );
		f->main->log(L"Сообщение успешно отправлено. (^_^)");

		TDateTime D = Date();
		TDateTime T = Time();
		str DT = DateToStr(D) + " - " + TimeToStr(T);
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

        SendImage(UserID,IMAGELIST,Token);
		SendAudio(UserID,AUDIOLIST,Token);
		SendAudioRec(UserID,RECORDLIST,Token);
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

    delete IMAGELIST;
	delete AUDIOLIST;
	delete RECORDLIST;
}
void c_process::SendAutoStop(str UserID, TStringList *DIALOG, str STACK, str Token, str AutoStopPostsFile)
{
	f->main->log(L"Отправка: [ Автостоп вариант ]");

	str DATAPOST = f->ii->GetAutoStopMessage(AutoStopPostsFile);
	str MESSID = SendMessage( UserID, DATAPOST, Token );

    if ( Pos("error",MESSID) == 0 )
	{
		f->main->log(L"Текст: [ " + DATAPOST + " ]" );
		f->main->log(L"Сообщение успешно отправлено. (^_^)");

		TDateTime D = Date();
		TDateTime T = Time();
		str DT = DateToStr(D) + " - " + TimeToStr(T);
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
void c_process::SendImage(str UserID, TStringList *IMAGELIST, str Token)
{
	try
	{
		if ( IMAGELIST->Count > 0 )
		{
			str x = IMAGELIST->Strings[ Random( IMAGELIST->Count ) ];
			str afx = "[IMAGE="; 
            x = x.SubString(afx.Length()+1,x.Length()); 
            x = x.SubString(1,x.Length()-1);
			str fullpath = f->main->p_resources_images + x;

			bool success;
			str uploadurl = vk.photos_getUploadServer( &success, Token );

			// JSON //////////////////////////////////////////////////////////////////////////////
			try
			{
				std::auto_ptr<TJSONObject> json (static_cast<TJSONObject*>(TJSONObject::ParseJSONValue(uploadurl)));
				TJSONObject *obj_response = static_cast<TJSONObject*>(json->Get("response")->JsonValue);
				uploadurl    = obj_response->GetValue("upload_url")->ToString();
				str album_id = obj_response->GetValue("album_id")->ToString();
				str user_id  = obj_response->GetValue("user_id")->ToString();
			}
			catch (...) 
            { 
                f->main->log("TJSONObject 1"); 
            }
			///////////////////////////////////////////////////////////////////////////////

			if ( success )
			{
				f->main->log(L"Загрузка изображения на сервер.. [ "+x+" ]");
				str RETURN = vk.UploadToServer( &success, fullpath, sa.cut_kk(uploadurl) );
				f->main->log(L"Загрузка завершена.");
				f->main->log(L"Обработка..");

                // JSON //////////////////////////////////////////////////////////////////////////////
				try
				{
					std::auto_ptr<TJSONObject> json (static_cast<TJSONObject*>(TJSONObject::ParseJSONValue(RETURN)));

					str server    = json->GetValue("server")->ToString();
					str hash      = json->GetValue("hash")->ToString();      hash = sa.cut_kk( hash );

					str photo  = sa.StripSlashes( RETURN );
					photo      = sa.cut_photo( photo );

					str RETURN2 = vk.photos_saveMessagesPhoto( &success, photo, server, hash, Token );
					f->main->log(L"Отправка Media Image..");

                    // JSON //////////////////////////////////////////////////////////////////////////////
					try
					{
						std::auto_ptr<TJSONObject> json (static_cast<TJSONObject*>(TJSONObject::ParseJSONValue(RETURN2)));

						TJSONArray *obj_items = static_cast<TJSONArray*>(json->Get("response")->JsonValue);

						TJSONObject* x_obj_items = static_cast<TJSONObject*>(obj_items->Get(0));

						str xid    = x_obj_items->GetValue("id")->ToString();
						str ownerid = x_obj_items->GetValue("owner_id")->ToString();

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
void c_process::SendAudio(str UserID, TStringList *AUDIOLIST, str Token)
{
	try
	{
		if ( AUDIOLIST->Count > 0 )
		{
			str x = AUDIOLIST->Strings[ Random( AUDIOLIST->Count ) ];
			str afx = "[AUDIO="; 
            x = x.SubString(afx.Length()+1,x.Length()); 
            x = x.SubString(1,x.Length()-1);
			str fullpath = f->main->p_resources_audio + x;

            bool success;
			str uploadurl = vk.audio_getUploadServer( &success, Token );

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
				str RETURN = vk.audioUploadToServer( &success, fullpath, sa.cut_kk(uploadurl) );
				f->main->log(L"Загрузка завершена.");
				f->main->log(L"Обработка..");

				// JSON //////////////////////////////////////////////////////////////////////////////
				try
				{
					std::auto_ptr<TJSONObject> json (static_cast<TJSONObject*>(TJSONObject::ParseJSONValue(RETURN)));
					str redirect    = json->GetValue("redirect")->ToString();
					str server      = json->GetValue("server")->ToString();
					str audio       = json->GetValue("audio")->ToString();
					str hash        = json->GetValue("hash")->ToString();

					redirect = sa.cut_kk( redirect );
					audio = sa.cut_kk( audio );
					hash = sa.cut_kk( hash );

                    str RETURN2 = vk.audio_Save( &success, audio, server, hash, Token );
					f->main->log(L"Отправка Media Audio..");

                    // JSON //////////////////////////////////////////////////////////////////////////////
					try
					{
						std::auto_ptr<TJSONObject> json (static_cast<TJSONObject*>(TJSONObject::ParseJSONValue(RETURN2)));
						TJSONObject *obj_items = static_cast<TJSONObject*>(json->Get("response")->JsonValue);

						str xid      = obj_items->GetValue("id")->ToString();
						str ownerid = obj_items->GetValue("owner_id")->ToString();

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
void c_process::SendAudioRec(str UserID, TStringList *AUDIOLIST, str Token)
{
	try
	{
		if ( AUDIOLIST->Count > 0 )
		{
			str x = AUDIOLIST->Strings[ Random( AUDIOLIST->Count ) ];
			str afx = "[RECORD="; 
            x = x.SubString(afx.Length()+1,x.Length()); 
            x = x.SubString(1,x.Length()-1);
			str fullpath = f->main->p_resources_records + x;

            bool success;
			str uploadurl = vk.docs_getMessagesUploadServer( &success, Token );
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
				str RETURN = vk.audioUploadToServer( &success, fullpath, sa.cut_kk(uploadurl) );
				f->main->log(L"Загрузка завершена.");
				f->main->log(L"Обработка..");

				try
				{
					RETURN = RETURN.SubString(8,RETURN.Length());
					str FILE = sa.cut_kk( RETURN );
					FILE = sa.cut_kk( FILE );

					str RETURN2 = vk.docs_save( &success, FILE, "MyFile1", "Socionic", Token );
					f->main->log(L"Отправка Media Record..");

					// JSON //////////////////////////////////////////////////////////////////////////////
					try
					{
						std::auto_ptr<TJSONObject> json (static_cast<TJSONObject*>(TJSONObject::ParseJSONValue(RETURN2)));
						TJSONArray *obj_items = static_cast<TJSONArray*>(json->Get("response")->JsonValue);
						TJSONObject* x_obj_items = static_cast<TJSONObject*>(obj_items->Get(0));
						str xid    = x_obj_items->GetValue("id")->ToString();
						str ownerid = x_obj_items->GetValue("owner_id")->ToString();
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
str  c_ii::GetGlobalPostText(str STACK, TStringList *GLOBALMODEL, TStringList *DIALOG)
{
	str j = "NULL";

	if ( ! LastMessageIsGlobal(DIALOG) )
	{
		TStringList *IFS   = new TStringList;
		TStringList *POSTS = new TStringList;

		for ( int c = 0; c < GLOBALMODEL->Count; c++ )
		{
			str dataline = GLOBALMODEL->Strings[c];

			if ( Pos("#",dataline) != 0 || c == GLOBALMODEL->Count-1 )
			{
				if ( IFS->Count > 0 && POSTS->Count > 0 )
				{
					for ( int c = 0; c < IFS->Count; c++ )
					{
						str LINE_IF = IFS->Strings[c];
						TStringList *ARR = new TStringList;
						f->main->asplit(ARR, LINE_IF, "'");
						TStringList *ARRC = new TStringList;
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

								str rc = POSTS->Strings[ Random( POSTS->Count ) ];
								j = rc.SubString(6,rc.Length());
							}
						}
						delete ARR;
						delete ARRC;
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

		delete IFS;
		delete POSTS;
	}

	return j;
}
bool c_ii::LastMessageIsGlobal(TStringList *DIALOG)
{
	bool j = false;
	int mc = 0;
	TStringList *X = new TStringList;

	for ( int c = DIALOG->Count-1; c >= 0; c-- )
	{
		str l = DIALOG->Strings[c];
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

	delete X;
	return j;
}
str  c_ii::GetStage(str STACK, TStringList *STAGE)
{
	str J = "NULL";

	try
	{
		TStringList *LinesIF = new TStringList;
		MakeMass(STAGE,LinesIF);

		J = GetTarget(LinesIF,STACK);

		if ( J != "NULL" )
		{
			f->main->log(L"Определён IF. Цель: [ #"+J+" ]");
		}
		else
		{
			GetDefault(STAGE,&J);
			f->main->log(L"IF не найден. Цель по умолчанию: [ #"+J+" ]");
		}

		delete LinesIF;
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
		str dataline = STAGE->Strings[c];
		if ( Pos("IF'",dataline) != 0 )
		{
			LinesIF->Add( dataline );
        }
	}
}
str  c_ii::GetTarget(TStringList *LinesIF, str STACK)
{
	str J = "NULL";

	for ( int c = 0; c < LinesIF->Count; c++ )
	{
		str LINE_IF = LinesIF->Strings[c];
		TStringList *ARR = new TStringList;
		f->main->asplit(ARR, LINE_IF, "'");

		TStringList *ARRC = new TStringList;
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

		delete ARR;
		delete ARRC;
	}

	return J;
}
void c_ii::GetDefault(TStringList *STAGE, str *TARGETSTAGE)
{
	for ( int c = 0; c < STAGE->Count; c++ )
	{
		str dataline = STAGE->Strings[c];
		if ( Pos("DEFAULT'",dataline) != 0 )
		{
			str j = g.Encrypt(2,2,"'",dataline);
			*TARGETSTAGE = j;
			break;
        }
	}
}
str  c_ii::GetOldStage(str uid, str token, TStringList *RobotModel, str Message)
{
	str J = "NULL";                                                                            
	str MYPOST = "NULL";
	str WASSTAGE = "NULL";
																
	for ( int c = 0; c < f->main->AUTOANSOUTBOX->Count; c++ )
	{
		str line = f->main->AUTOANSOUTBOX->Strings[c];           

		str xToken  = g.Encrypt(1,6,"#",line);
		str xMid    = g.Encrypt(2,6,"#",line);
		str xUid 	= g.Encrypt(3,6,"#",line);
		str xRs 	= g.Encrypt(4,6,"#",line);
		str xTitle  = g.Encrypt(5,6,"#",line);
		str xBody 	= g.Encrypt(6,6,"#",line);

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
				str l = RobotModel->Strings[c];
				if ( Pos("POST'",l) != 0 )
				{
					str ModelPost = g.Encrypt(2,2,"'",l);
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
				TStringList *AAA = new TStringList;
				AAA->Text = RobotModel->Text;
				f->main->GetOnlyOneStage(AAA,WASSTAGE);
				J = "#"+f->ii->GetStage(Message,AAA);
				delete AAA;
			}
			else
			{
				f->main->log("Stage [ #"+WASSTAGE+L" ] не найден в модели общения.");
			}
		}
	}

	return J;                                                                                                  
}
void c_ii::GetAutoAns(str *iodata, str *targetstage, str Token, str uid)
{
	str data = *iodata;                        
    str DEF;                                  

	TStringList *ANSMODEL = new TStringList;
	TStringList *DEFAULT = new TStringList;    
	TStringList *ROBOTMODEL = new TStringList;

	TStringList *L = new TStringList;
	g.GetFiles( f->main->p_robots, L );
	str GroupName, RobotName, Server_ID, Login, Password, xToken, Activity, ModelFile, Online, LastOnline;  bool freeze;
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

	delete DEFAULT;

	f->main->log(L"Попытка определить прежний Stage." );
	str target = GetOldStage(uid,xToken,ROBOTMODEL,data);
	if ( target == "NULL" ) 
    {
        f->main->log( L"Прежний Stage не найден. (" );
    }

	if ( target == "NULL" )
	{
		for ( int c = 0; c < ANSMODEL->Count; c++ )
		{
			str key = g.Encrypt(1,2,"~",ANSMODEL->Strings[c]);
			if ( Pos(get_alpha_keys(key),data) > 0 )
			{
				target = g.Encrypt(2,2,"~",ANSMODEL->Strings[c]);
				f->main->log( L"В сообщении найден ключ [ "+get_alpha_keys(key)+" ]" );
				break;
			}
		}
	}

	delete ANSMODEL;

	if ( target == "NULL" ) 
    {
        target = DEF;
	}

	*targetstage = target;
	TStringList *CLEAR = new TStringList;
	bool breaked = false;
	for ( int i = 0; i < ROBOTMODEL->Count; i++ )
	{
		str l = ROBOTMODEL->Strings[i];
		if ( l == target )
		{
			while ( true )
			{
				str l = ROBOTMODEL->Strings[i]; i++;
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

	str J = CLEAR->Strings[ Random( CLEAR->Count ) ];
	J = g.Encrypt(2,2,"'",J);
	*iodata = J;
    
    delete CLEAR;
	delete ROBOTMODEL;                                                                             //f->main->plog("GetAutoAns - stop");
}
bool c_ii::IfAutoStop(str message, str file)
{
	bool stop = false;

	if ( f->LV_WORKTASKS->Items->Item[4]->Checked )
	{
		TStringList *L = new TStringList;
		L->LoadFromFile(file);

		message = message;
		for ( int c = 0; c < L->Count; c++ )
		{
			str key = Trim(L->Strings[c]);
			key = get_alpha_keys(key);

			if ( Pos(key,message) != 0 )
			{
				stop = true;
				f->main->log(L"Зафиксировано : [ "+key+" ]");
				break;
			}
		}
		delete L;
	}
	return stop;
}
str  c_ii::GetAutoStopMessage(str file)
{
	TStringList *L = new TStringList;
	L->LoadFromFile(file);

	str j = "NULL";
	if ( L->Count > 0 ) 
    {
        j = L->Strings[ Random( L->Count ) ];
    }

	delete L;
	return Trim(j);
}
str  c_ii::do_randomize(str text)
{
	str j = "";

	if ( f->ch_RANDOMIZE->Checked )
	{
		for ( int c = 1; c <= text.Length(); c++ )
		{
			str ch = text[c];

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
str  c_ii::get_alpha_keys(str text)
{
	text = text.LowerCase();
	str j = "";

	if ( f->ch_ALPHAKEYS->Checked )
	{
		for ( int c = 1; c <= text.Length(); c++ )
		{
			str ch = text[c];

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
str  c_ii::MakePostLine(str post_variables_line)
{
	f->main->log("POST : [ "+post_variables_line+" ]");

	str j = "";

	int long_memory_index = 0;
	int long_upper_count  = 0;

	bool w = false;
	str  buff = "";
	for ( int c = 1; c <= post_variables_line.Length(); c++ )
	{
		str ch = post_variables_line[c];

		if ( w )
		{
			if ( ch == ")" )
			{
				w = false;
				j = j + MakePostLineGetOne(buff,long_upper_count,&long_memory_index);
				long_upper_count--;
                if ( long_upper_count == 0 ) 
                {
                    long_memory_index = 0;
                }

				try
				{
					str chn = post_variables_line[c+1];
					int chc = StrToInt(post_variables_line[c+2]);
					if ( chn == "^" )
					{
						long_upper_count = chc;
						c++; 
                        c++;
					}
				}
				catch (...) 
                { 
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
				j = j + ch;
			}
		}
    }
	return j;
}
str  c_ii::MakePostLineGetOne(str buffer, int uppercount, int *memory_j)
{
	int sc = 0;
	for ( int c = 1; c < buffer.Length(); c++ )
	{
		str ch = buffer[c];
		if ( ch == "|" ) sc++;
	}

	int parts = sc + 1;
	str j = "NULL";

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
bool c_ii::if_EqualPosts(str ModelPost, str MyPost)
{                                                                        
	bool eq = false;

	TStringList *JPOSTVARIABLES = new TStringList;
	JPOSTVARIABLES->Add("");

	str  buff = "";

	bool w = false;
	for ( int c = 1; c <= ModelPost.Length(); c++ )
	{
		str ch = ModelPost[c];

		if ( w )
		{
			if ( ch == ")" )
			{
				w = false;
				MakePostLineGetAll(buff,JPOSTVARIABLES);

				try
				{
					str chn = ModelPost[c+1];
					if ( chn == "^" ) 
                    { 
                        c++; 
                        c++;
                    }
				}
				catch (...) 
                { 
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
		str l = JPOSTVARIABLES->Strings[i];
		str f1 = CutMediaTags( l );
        f1 = f1.LowerCase();
		str f2 = MyPost.LowerCase();

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

	delete JPOSTVARIABLES;
	return eq;
}
void c_ii::MakePostLineGetAll(str buffer, TStringList *JPOSTVARIABLES_A)
{
	int sc = 0;
	for ( int c = 1; c < buffer.Length(); c++ )
	{
		str ch = buffer[c];
		if ( ch == "|" ) 
        {
            sc++;
        }
	}
	int parts = sc + 1;

	TStringList *JPOSTVARIABLES_D = new TStringList;

	for ( int r = 1;  r <= parts; r++ )
	{
		str j = g.Encrypt(r,parts,"|",buffer);

		for ( int c = 0; c < JPOSTVARIABLES_A->Count; c++ )
		{
			JPOSTVARIABLES_D->Add( JPOSTVARIABLES_A->Strings[c] + j );
        }
	}

	JPOSTVARIABLES_A->Text = JPOSTVARIABLES_D->Text;
	delete JPOSTVARIABLES_D;
}
void c_ii::WritePostDataIn(TStringList *IMAGELIST, TStringList *AUDIOLIST, TStringList *RECORDLIST, str *postline)
{
	str pl = *postline; str fx; int p;

	try
	{
		fx = "[IMAGE=";
		p = Pos(fx,pl);
		if ( p != 0 )
		{
			str L = pl.SubString(1,p-1);
			str R = pl.SubString(p,pl.Length());
			p = Pos("]",R);
			str M = R.SubString(1,p);
			str X = R.SubString(p+1,R.Length());
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
			str L = pl.SubString(1,p-1);
			str R = pl.SubString(p,pl.Length());
			p = Pos("]",R);
			str M = R.SubString(1,p);
			str X = R.SubString(p+1,R.Length());
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
			str L = pl.SubString(1,p-1);
			str R = pl.SubString(p,pl.Length());
			p = Pos("]",R);
			str M = R.SubString(1,p);
			str X = R.SubString(p+1,R.Length());
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
str  c_ii::CutMediaTags(str data)
{
	str fx1 = "[IMAGE=";
	str fx2 = "[AUDIO=";
	str fx3 = "[RECORD=";

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
void c_online::addtobuffer(str token)
{
	long int unix = f->main->getUnix();
	str s = unix;
	APIBUFFER->Add( token + "#" + s );
}
void c_online::processing()
{
	long int UNIX  = f->main->getUnix();
	int      IUNIX = UNIX;

	buffer_clear(IUNIX);

	TStringList *L = new TStringList;
	g.GetFiles( f->main->p_robots, L );

	for ( int c = 0; c < L->Count; c++ )
	{
		str GroupName, RobotName, Server_ID, Login, Password, Token, Activity, Online, LastOnline;  bool freeze;
		f->main->get_robotdata( c, &GroupName, &RobotName, &Server_ID, &Login, &Password, &Token, &Activity, &Online, &LastOnline, &freeze );

		if ( Online == "0" && Token != "NULL" )
		{
			if ( StrToInt(LastOnline)+540+Random(60) < IUNIX )
			{
				bool success = false;

				make_online(RobotName,Token,&success);

				if ( success )
				{
					str tprefix = f->main->PREFIX;
					f->main->PREFIX = L"Вечный онлайн : [ "+RobotName+" ] ";
					f->main->log(L"Статус 'online' установлен.");
					f->main->PREFIX = tprefix;

					str robotpath = f->main->p_robots + L->Strings[c] + "\\" + "Conf.ini";
                    TMemIniFile *INI = new TMemIniFile( UnicodeString(robotpath), TEncoding::UTF8 );
			        INI->WriteInteger(UnicodeString("ACCOUNT"), UnicodeString("lastonline"),  IUNIX);
                    INI->UpdateFile();
			        delete INI;
				}
				else
				{
					str tprefix = f->main->PREFIX;
					f->main->PREFIX = L"Вечный онлайн : [ "+RobotName+" ] ";
					f->main->log(L"Не удалось установить статус 'online'. ((");
					f->main->PREFIX = tprefix;
				}

				Sleep(400);
            }
		}

		if ( Online == "0" && Token == "NULL" )
		{
            str tprefix = f->main->PREFIX;
			f->main->PREFIX = L"Вечный онлайн : [ "+RobotName+" ] ";
			f->main->log(L"Необходимо получить токен!");
			f->main->PREFIX = tprefix;
		}
	}

	delete L;
}
void c_online::make_online(str robotname, str token, bool *success)
{
	str j = vk.set_online(success,token);
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
	TStringList *L = new TStringList;
	for ( int c = 0; c < APIBUFFER->Count; c++ )
	{
		str lastapi = g.Encrypt(2,2,"#",APIBUFFER->Strings[c]);
		if ( StrToInt(lastapi)+5 < unix ) 
        {
            L->Add( APIBUFFER->Strings[c] );
        }
	}

	APIBUFFER->Text = L->Text;
	delete L;
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
		str NeededGroupName = Trim(f->LV_CONF_GROUPS->Items->Item[c]->Caption);

		int Count = 0;
		TStringList *L = new TStringList;
		g.GetFiles( f->main->p_robots, L );
		for ( int x = 0; x < L->Count; x++ )
		{
			str GroupName, RobotName, Server_ID, Login, Password, Token, Activity, Online, LastOnline;  
            bool freeze;
			f->main->get_robotdata( x, &GroupName, &RobotName, &Server_ID, &Login, &Password, &Token, &Activity, &Online, &LastOnline, &freeze );

			if ( GroupName == NeededGroupName ) 
            {
                Count++;
            }
		}
		delete L;

		f->LV_CONF_GROUPS->Items->Item[c]->SubItems->Strings[0] = IntToStr(Count);
    }
}
void c_vcl::groupechoReadUsers()
{
    TStringList *L = new TStringList;
	L->LoadFromFile( f->main->f_users );

	for ( int c = 0; c < f->LV_CONF_GROUPS->Items->Count; c++ )
	{
		str NeededGroupName = Trim(f->LV_CONF_GROUPS->Items->Item[c]->Caption);
		int Count = 0;
		for ( int x = 0; x < L->Count; x++ )
		{
			str UserGroup = g.Encrypt(1,5,"#",L->Strings[x]);

			if ( UserGroup == NeededGroupName ) 
            {
                Count++;
            }
		}

		f->LV_CONF_GROUPS->Items->Item[c]->SubItems->Strings[1] = IntToStr(Count);
	}

	delete L;
}
void c_vcl::Dialog_Add_User(str User)
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
		str textline = TEXT->Strings[c];

        f->ME_DIALOG->SelStart = f->ME_DIALOG->Lines->Text.Length();
		f->ME_DIALOG->SelAttributes->Size = 8;
		f->ME_DIALOG->SelAttributes->Name = "consolas";
		f->ME_DIALOG->SelAttributes->Color = StringToColor("0x00444444");

		f->ME_DIALOG->Lines->Add( textline );
	}

	::SendMessage(f->ME_DIALOG->Handle,WM_VSCROLL,SB_BOTTOM,0);
}
void c_vcl::Dialog_Add_Date(str Date)
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
str  c_vcl::FixInbox(str data)
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
void c_vcl::GoToModels(str RobotName, int PositionView)
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

    TStringList *L = new TStringList;
    g.GetFiles( f->main->p_robots, L );
    for ( int c = 0; c < L->Count; c++ )
    {
		str GroupName, cRobotName, Server_ID, Login, Password, Token, Activity, Online, LastOnline;  
        bool freeze;
		f->main->get_robotdata( c, &GroupName, &cRobotName, &Server_ID, &Login, &Password, &Token, &Activity, &Online, &LastOnline, &freeze );

        if ( RobotName == cRobotName )
		{
			f->CB_MODELS_ROBOTS->ItemIndex = c;
			f->CB_MODELS_ROBOTSChange(new TObject());
			break;
        }
    }
	delete L;
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
        str data = f->main->MODEL_LOGICAL->Strings[c];
        if ( Pos("#",data) != 0 ) 
        {
            CB->Items->Add( data );
        }
    }
}
void c_vcl::GetAllRobots(TsComboBox *CB)
{
    CB->Items->Clear();

    TStringList *L = new TStringList;
	g.GetFiles( f->main->p_robots, L );
    for ( int c = 0; c < L->Count; c++ )
    {
		str GroupName, RobotName, Server_ID, Login, Password, Token, Activity, Online, LastOnline;  
        bool freeze;
		f->main->get_robotdata( c, &GroupName, &RobotName, &Server_ID, &Login, &Password, &Token, &Activity, &Online, &LastOnline, &freeze );

        CB->Items->Add( RobotName );
    }

    delete L;
}
void c_vcl::InputCaptchaOpenForm(str file)
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

}
str  c_captcha::GetAnswer( str CaptchaIMGURL )
{
	str ANS = "NULL";

	try
	{
		CaptchaIMGURL = FixURL( CaptchaIMGURL );
		str CaptchaFILE = SaveToFile(CaptchaIMGURL);

		if ( f->rg_CAPTCHA_SERVICE->ItemIndex == 0 )
		{
			ANS = g.GetCaptchaRucaptcha1(CaptchaFILE,f->e_CAPTCHA_KEY->Text);

			while ( Pos("ERROR",ANS) != 0 )
			{
				f->main->log("ATTEMPT. RUCAPTCHA UNDER CYCLE. RET: "+ANS);
				ANS = g.GetCaptchaRucaptcha1(CaptchaFILE,f->e_CAPTCHA_KEY->Text);
            }
		}

		if ( f->rg_CAPTCHA_SERVICE->ItemIndex == 1 )
		{
			g.Sm("FATAL ERROR! NOT ENOUGH CODE C++ ! USE RUCAPTCHA SERVICE!");
		}

		DeleteFile( CaptchaFILE );
	}
	catch ( Exception *ex )
	{
		f->main->log("E:CAPTCHA::GetAnswer...ex:"+ex->Message);
    }

	return ANS;
}
str  c_captcha::FixURL( str Url )
{
	str j = "";
	for ( int c = 1; c <= Url.Length(); c++ )
	{
		str ch = Url[c];
		if ( ch != "\\" ) 
        {
            j = j + ch;
        }
	}
	return j;
}
str  c_captcha::SaveToFile( str Url )
{
	str file = g.GetDirectoryApplicationDatapath()+"captcha.jpg";

	TMemoryStream *ms = new TMemoryStream();

    TIdHTTP *HTTP = new TIdHTTP(NULL);
	HTTP->HandleRedirects = true;

	HTTP->Get( Url, ms );

	ms->Position = 0;
	ms->SaveToFile( file );

	delete ms;
	delete HTTP;

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

	if ( CH_LOGGOTO->Checked ) PAGES->ActivePageIndex = 4;

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
	str url;
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

		str name  = Trim(LV_CONF_GROUPS->Items->Item[index]->Caption);
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
		str UsesGroupName = Trim(LV_CONF_GROUPS->Items->Item[LV_CONF_GROUPS->ItemIndex]->Caption);
		TStringList *G = new TStringList;
		for ( int c = 0; c < LV_CONF_GROUPS->Items->Count; c++ )
		{
			str tGroup = Trim(LV_CONF_GROUPS->Items->Item[c]->Caption);
			if ( UsesGroupName == tGroup ) 
            {
                cbIndex = c;
            }
			G->Add( tGroup );
		}
		cb_conf_robots_edit_groups->Items->Text = G->Text;
		cb_conf_robots_edit_groups->ItemIndex = cbIndex;
		delete G;

		TStringList *L = new TStringList;
		g.GetFiles( main->p_robots, L );
		for ( int c = 0; c < L->Count; c++ )
		{
			str GroupName, RobotName, Server_ID, Login, Password, Token, Activity, Online, LastOnline;  
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
		delete L;
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
		str OldName = Trim(LV_CONF_ROBOTS->Items->Item[LV_CONF_ROBOTS->ItemIndex]->Caption);

		TStringList *L = new TStringList;
		g.GetFiles( main->p_robots, L );

		for ( int c = 0; c < L->Count; c++ )
		{
			str GroupName, RobotName, Server_ID, Login, Password, Token, Activity, Online, LastOnline;  
            bool freeze;
			main->get_robotdata( c, &GroupName, &RobotName, &Server_ID, &Login, &Password, &Token, &Activity, &Online, &LastOnline, &freeze );

			if ( RobotName == OldName )
			{
                str robotpath = main->p_robots + L->Strings[c] + "\\";
                main->checkDirectoryExisting(robotpath);
				TMemIniFile *INI = new TMemIniFile( UnicodeString(robotpath + "Conf.ini"),TEncoding::UTF8 );
				INI->WriteString( UnicodeString("MAIN"),    UnicodeString("owner"),   UnicodeString(Trim(cb_conf_robots_edit_groups->Items->Strings[cb_conf_robots_edit_groups->ItemIndex]))   );
				INI->WriteString( UnicodeString("MAIN"),    UnicodeString("name"),    UnicodeString(f->e_conf_robots_edit_name->Text) );
				INI->WriteString( UnicodeString("ACCOUNT"), UnicodeString("login"),   UnicodeString(f->e_conf_robots_edit_login->Text) );
				INI->WriteString( UnicodeString("ACCOUNT"), UnicodeString("password"),UnicodeString(f->e_conf_robots_edit_password->Text) );
                INI->WriteString( UnicodeString("ACCOUNT"), UnicodeString("online"),  UnicodeString(IntToStr( f->ch_robot_online_edit->ItemIndex) ) );
                INI->UpdateFile();
				delete INI;

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
	str URL = e_conf_users_URL->Text;
	if ( URL.Length() > 10 && 
         Pos("vk.com/search",URL) > 0 && 
         Pos("=people",URL) > 0 )
	{
		main->countOfprocessed = 0;

		PAGES->ActivePageIndex = 4; g.ProcessMessages();

		try
		{
			TIdHTTP *HTTP = new TIdHTTP(NULL);
			URL = HTTP->URL->URLDecode(URL);
			delete HTTP;

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
		str id = Trim(LV_DIALOGS->Items->Item[index]->Caption);

		TStringList *X = new TStringList;
		X->LoadFromFile( main->p_dialogs + id );

		str robot_gid 	 = X->Strings[0];
		str name 		 = X->Strings[1];
		str surname 	 = X->Strings[2];

		f->l_GID->Caption = robot_gid;

		main->DrawMessageBox(name,surname,robot_gid,X);
		main->SetAsRead( X );

		X->SaveToFile( UnicodeString(main->p_dialogs + id), TEncoding::UTF8 );
		delete X;

		str inc = LV_DIALOGS->Items->Item[index]->SubItems->Strings[2];
		LV_DIALOGS->Items->Item[index]->SubItems->Strings[2] = vcl->FixInbox( inc );
	}
}

void __fastcall Tf::B_SENDMESSAGEClick(TObject *Sender)
{
	B_SENDMESSAGE->Enabled = false; g.ProcessMessages();

	if ( ME_INPUT->Lines->Text.Length() > 2 )
	{
		str id = Trim(LV_DIALOGS->Items->Item[LV_DIALOGS->ItemIndex]->Caption);

		TStringList *L = new TStringList;
		g.GetFiles( main->p_robots, L );

		for ( int c = 0; c < L->Count; c++ )
		{
			str GroupName, RobotName, Server_ID, Login, Password, Token, Activity, Online, LastOnline;  
            bool freeze;
			main->get_robotdata( c, &GroupName, &RobotName, &Server_ID, &Login, &Password, &Token, &Activity, &Online, &LastOnline, &freeze );

			if ( RobotName == Trim(l_GID->Caption) )
			{
				if ( proc->Establish(RobotName,&Token) )
				{
					str MESSID = proc->SendMessage( id, ME_INPUT->Lines->Text, Token );

					TDateTime D = Date();
					TDateTime T = Time();
					str DT = DateToStr(D) + " - " + TimeToStr(T);
					TStringList *X = new TStringList;
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
					delete X;

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
		delete L;
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
            str indexName = TREE->Selected->Text;
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
        TStringList *L = new TStringList;
        g.GetFiles( main->p_robots, L );
		str file = main->p_robots + L->Strings[ CB_MODELS_ROBOTS->ItemIndex ] + "\\Hello.txt";
        delete L;
        
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
		TStringList *L = new TStringList;
        g.GetFiles( main->p_robots, L );
		str file = main->p_robots + L->Strings[ CB_MODELS_ROBOTS->ItemIndex ] + "\\AutoAnsRules.txt";
        delete L;
        
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
		TStringList *L = new TStringList;
        g.GetFiles( main->p_robots, L );
		str file = main->p_robots + L->Strings[ CB_MODELS_ROBOTS->ItemIndex ] + "\\AutoStopKeys.txt";
        delete L;
        
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
		TStringList *L = new TStringList;
        g.GetFiles( main->p_robots, L );
		str file = main->p_robots + L->Strings[ CB_MODELS_ROBOTS->ItemIndex ] + "\\AutoStopPosts.txt";
        delete L;
        
		main->MODEL_AUTOSTOP_POSTS->SaveToFile( UnicodeString(file), TEncoding::UTF8 );
		main->LoadModelAutoStopPosts();
	}
}
void __fastcall Tf::B_MODEL_LOGICAL_ADDEDITClick(TObject *Sender)
{
	if ( LI_MODEL_LOGICAL_POSTS->Items->Count > 0 && E_MODEL_LOGICAL_STAGE->Text.Length() > 0 )
    {
		str StageName = main->CreateStageName(E_MODEL_LOGICAL_STAGE->Text);
        if ( ! main->ifStageNameExist(StageName) )
        {
    		B_MODEL_LOGICAL_ADDEDIT->Caption = "";

    		TStringList *J = new TStringList;
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
                main->ModelLogicalAddStage( J );
            }
            else                  
            {
                main->ModelLogicalChangeStage( J );
            }
            
            main->ModelSaveAndReloadInterface();
            delete J;

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

        str var = g.Its( Random(999999) );
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
        str fx = main->IFVARIANTS->Strings[index];
        fx = fx.SubString(4,fx.Length());
        int p = Pos("'",fx);
        fx = fx.SubString(p+1,fx.Length());
        fx = "#" + fx;

		bool ex = false;
        for ( int c = 0; c < CB_MODEL_LOGICAL_IFVARIANTS->Items->Count; c++ )
        {
            str dataline = CB_MODEL_LOGICAL_IFVARIANTS->Items->Strings[c];
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
        str fx = main->EXTENDED->Strings[index];
        fx = fx.SubString(10,fx.Length());
        int p = Pos("'",fx);
        fx = fx.SubString(p+1,fx.Length());
        fx = "#" + fx;

		bool ex = false;
        for ( int c = 0; c < CB_MODEL_LOGICAL_EXTENDED->Items->Count; c++ )
        {
            str dataline = CB_MODEL_LOGICAL_EXTENDED->Items->Strings[c];
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

        str var = main->GetVarFrom(index);

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
		TStringList *X = new TStringList;
		X->Text = E_MODEL_AUTOANS_DEFAULT->Text;

		TStringList *L = new TStringList;
		g.GetFiles( main->p_robots, L );
		str file = main->p_robots + L->Strings[ CB_MODELS_ROBOTS->ItemIndex ] + "\\AutoAnsDefault.txt";
		X->SaveToFile( UnicodeString(file), TEncoding::UTF8 );
        
		delete L;
		delete X;
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
		str dir = main->f_od_import_robot_model + "\\";  // g.Sm(dir);
		TStringList *SOURCEFILES = new TStringList;
		g.GetFiles( dir, SOURCEFILES );

		for ( int c = 0; c < f->LV_WORKGROUPS->Items->Count; c++ )
		{
			if ( f->LV_WORKGROUPS->Items->Item[c]->Checked )
			{
				str NeededGroup = Trim(f->LV_WORKGROUPS->Items->Item[c]->Caption);

				TStringList *L = new TStringList;
				g.GetFiles( f->main->p_robots, L );
				str GroupName, RobotName, Server_ID, Login, Password, Token, Activity, ModelFile, Online, LastOnline;  
                bool freeze;
				for ( int x = 0; x < L->Count; x++ )
				{
					str CurrentRobotPath = f->main->p_robots + L->Strings[x] + "\\";
					f->main->get_robotdata( x, &GroupName, &RobotName, &Server_ID, &Login, &Password, &Token, &Activity, &Online, &LastOnline, &freeze );

					if ( GroupName == NeededGroup )
					{
						for ( int i = 0; i < SOURCEFILES->Count; i++ )
						{
							str src  = dir + SOURCEFILES->Strings[i];
							str dest = CurrentRobotPath + SOURCEFILES->Strings[i];

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
        str url = "http://www.youtube.com/channel/UCvNUz0gcIU5iEiPIiZ68P5g";
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
        str url = "http://jenya.in.ua/";
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
        str url = "http://vk.robots.jenya.in.ua/";
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
		str X = LV_SERVERS->Items->Item[index]->SubItems->Strings[0];
		main->buffer_write( X );
	}
}
void __fastcall Tf::N3Click(TObject *Sender)
{
	// SERVERS - COPY LOGIN

	int index = LV_SERVERS->ItemIndex;
	if ( index != -1 )
	{
		str X = LV_SERVERS->Items->Item[index]->SubItems->Strings[1];
		main->buffer_write( X );
	}
}
void __fastcall Tf::N4Click(TObject *Sender)
{
	// SERVERS - COPY PASS

	int index = LV_SERVERS->ItemIndex;
	if ( index != -1 )
	{
		str X = LV_SERVERS->Items->Item[index]->SubItems->Strings[2];
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

		str NeededRobotName = Trim( LV_CONF_ROBOTS->Items->Item[index]->Caption );

		TStringList *L = new TStringList;
		g.GetFiles( main->p_robots, L );

		for ( int c = 0; c < L->Count; c++ )
		{
			str GroupName, RobotName, Server_ID, Login, Password, Token, Activity, Online, LastOnline;  
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

		delete L;
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
		str NeededRobotName = Trim(LV_CONF_ROBOTS->Items->Item[index]->Caption);
		str token;
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

	TStringList *L = new TStringList;
	for ( int c = 0; c < LV_CONF_ROBOTS->Items->Count; c++ )
	{
		str NeededRobotName = Trim(LV_CONF_ROBOTS->Items->Item[c]->Caption);
		str token;
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
    delete L;
}
void __fastcall Tf::N10Click(TObject *Sender)
{
	// CONF - USERS - DELETE

	int index = LV_CONF_USERS->ItemIndex;

	if ( index != -1 )
	{
		str id = Trim(LV_CONF_USERS->Items->Item[index]->SubItems->Strings[0]);

		TStringList *L = new TStringList;
        if(FileExists(main->f_users))
        {
		    L->LoadFromFile( main->f_users );
        }
		for ( int c = 0; c < L->Count; c++ )
		{
			str uid = g.Encrypt(2,5,"#",L->Strings[c]);
			if ( uid == id )
			{
				L->Delete(c);
                break;
            }
		}
		L->SaveToFile( UnicodeString(main->f_users), TEncoding::UTF8 );
		delete L;

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
		str id = Trim(LV_CONF_USERS->Items->Item[index]->Caption);
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
		str id = Trim(LV_CONF_USERS->Items->Item[i]->SubItems->Strings[0]);

		TStringList *L = new TStringList;
        if(FileExists(main->f_users))
        {
		    L->LoadFromFile( main->f_users );
        }
		for ( int c = 0; c < L->Count; c++ )
		{
			str uid = g.Encrypt(2,5,"#",L->Strings[c]);
			if ( uid == id )
			{
				L->Delete(c);
                break;
            }
		}
		L->SaveToFile( UnicodeString(main->f_users), TEncoding::UTF8 );
		delete L;

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
		TStringList *L = new TStringList;
		g.GetFiles( main->p_robots, L );
		for ( int c = 0; c < L->Count; c++ )
		{
			str GroupName, RobotName, Server_ID, Login, Password, Token, Activity, Online, LastOnline;  
            bool freeze;
			main->get_robotdata( c, &GroupName, &RobotName, &Server_ID, &Login, &Password, &Token, &Activity, &Online, &LastOnline, &freeze );

			if ( RobotName == Trim(LV_CONF_ROBOTS->Items->Item[index]->Caption) )
			{
				str t = main->p_robots + L->Strings[c] + "\\";

				wchar_t *wc = t.t_str();
				char c[500];
				wcstombs(c, wc, wcslen(wc)+1);
				ShellExecuteA( NULL, "open", c, NULL, NULL, SW_SHOW );

				break;
			}
		}
		delete L;
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
		TStringList *L = new TStringList;
		g.GetFiles( main->p_dialogs, L );

		DeleteFile( main->p_dialogs + L->Strings[index] );

		LV_DIALOGS->Items->Delete(index);

		delete L;
	}
}
void __fastcall Tf::N28Click(TObject *Sender)
{
	// PM LV DIALOGS - DELETE ALL

	TStringList *L = new TStringList;
	g.GetFiles( main->p_dialogs, L );
	for ( int c = 0; c < L->Count; c++ )
	{
		DeleteFile( main->p_dialogs + L->Strings[c] );
	}
	delete L;

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
        TStringList *L = new TStringList;
		g.GetFiles( main->p_robots, L );
        str file = main->p_robots + L->Strings[ CB_MODELS_ROBOTS->ItemIndex ] + "\\Hello.txt";
        delete L;
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

	str DSTAGE = TREE->Selected->Text;
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
        TStringList *L = new TStringList;
		g.GetFiles( main->p_robots, L );
		str file = main->p_robots + L->Strings[ CB_MODELS_ROBOTS->ItemIndex ] + "\\AutoAnsRules.txt";
        delete L;
        
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
		TStringList *L = new TStringList;
		g.GetFiles( main->p_robots, L );
		str file = main->p_robots + L->Strings[ CB_MODELS_ROBOTS->ItemIndex ] + "\\AutoStopKeys.txt";
		delete L;
        
		main->MODEL_AUTOSTOP_KEYS->SaveToFile( UnicodeString(file), TEncoding::UTF8 );
		main->LoadModelAutoStopKeys();
	}
}
void __fastcall Tf::N60Click(TObject *Sender)
{
	// PM MODELS  AUTOSTOP KEYS DEL ALL

	main->MODEL_AUTOSTOP_KEYS->Clear();
	TStringList *L = new TStringList;
	g.GetFiles( main->p_robots, L );
	str file = main->p_robots + L->Strings[ CB_MODELS_ROBOTS->ItemIndex ] + "\\AutoStopKeys.txt";
	delete L;
    
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
		TStringList *L = new TStringList;
		g.GetFiles( main->p_robots, L );
		str file = main->p_robots + L->Strings[ CB_MODELS_ROBOTS->ItemIndex ] + "\\AutoStopPosts.txt";
		delete L;
        
		main->MODEL_AUTOSTOP_POSTS->SaveToFile( UnicodeString(file), TEncoding::UTF8 );
		main->LoadModelAutoStopPosts();
	}
}
void __fastcall Tf::N62Click(TObject *Sender)
{
	// PM MODELS  AUTOSTOP POSTS DEL ALL

	main->MODEL_AUTOSTOP_POSTS->Clear();
	TStringList *L = new TStringList;
	g.GetFiles( main->p_robots, L );
	str file = main->p_robots + L->Strings[ CB_MODELS_ROBOTS->ItemIndex ] + "\\AutoStopPosts.txt";
	delete L;
    
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
	TStringList *L = new TStringList;
	g.GetFiles( main->p_robots, L );
	for ( int c = 0; c < L->Count; c++ )
	{
		str robotpath = main->p_robots + L->Strings[c] + "\\" + "Conf.ini";
        TMemIniFile *INI = new TMemIniFile( UnicodeString(robotpath),TEncoding::UTF8 );
        INI->WriteBool( UnicodeString("MAIN"), UnicodeString("freeze"), true );
        INI->UpdateFile();
		delete INI;
	}
	delete L;
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
void __fastcall Tf::BTEST1Click(TObject *Sender)
{
	ShowMessage("T1");

    TIdHTTP *HTTP = new TIdHTTP(NULL);
	HTTP->HandleRedirects = true;
	HTTP->ConnectTimeout = 500;
	HTTP->ReadTimeout = 500;

	str j = "NULL";

	try
	{
		j = HTTP->Get("http://vkbot.jenya.in.ua/test.php");
	}
	catch (...)
	{
		ShowMessage("Fuck!");
    }

	ShowMessage( j );
}
void __fastcall Tf::BTEST2Click(TObject *Sender)
{
	ShowMessage("T2");
}
void __fastcall Tf::B_TEST_01Click(TObject *Sender)
{
	TStringList *IMAGELIST = new TStringList;
	TStringList *AUDIOLIST = new TStringList;
	TStringList *RECORDLIST = new TStringList;

	str PostLine = L"POST'$25 сутки. Паспорт в залог обязателен![IMAGE=Demotivators\\Image1.jpg][RECORD=Voices-1\\ogg_sound_effect_56.ogg]";

	PostLine = g.Encrypt(2,2,"'",PostLine);

	f->ii->WritePostDataIn(IMAGELIST,AUDIOLIST,RECORDLIST,&PostLine);

	ShowMessage("postline_"+PostLine+"_");

	ShowMessage("IMAGELIST_"+IMAGELIST->Text+"_");
	ShowMessage("AUDIOLIST_"+AUDIOLIST->Text+"_");
	ShowMessage("RECORDLIST_"+RECORDLIST->Text+"_");

	delete IMAGELIST;
	delete AUDIOLIST;
	delete RECORDLIST;
}
























