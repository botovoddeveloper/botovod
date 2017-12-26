#ifndef MAINH
#define MAINH
#include "MyTHREAD_TWO.h"
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "sSkinManager.hpp"
#include "sPageControl.hpp"
#include <Vcl.ComCtrls.hpp>
#include <System.ImageList.hpp>
#include <Vcl.ImgList.hpp>
#include <Vcl.ExtCtrls.hpp>
#include "sRichEdit.hpp"
#include "sGroupBox.hpp"
#include "sListView.hpp"
#include "sEdit.hpp"
#include "sButton.hpp"
#include <Vcl.Menus.hpp>
#include <clipbrd.hpp>
#include <Vcl.Dialogs.hpp>
#include "sBitBtn.hpp"
#include <Vcl.Buttons.hpp>
#include "sCheckBox.hpp"
#include "sComboBox.hpp"
#include "acProgressBar.hpp"
#include <IdAntiFreezeBase.hpp>
#include <IdBaseComponent.hpp>
#include <Vcl.IdAntiFreeze.hpp>
#include <IdThreadComponent.hpp>
#include "sTreeView.hpp"
#include <Vcl.WinXCtrls.hpp>
#include "sListBox.hpp"
#include "sLabel.hpp"
#include "sRadioButton.hpp"
#include <IdComponent.hpp>
#include <IdIOHandler.hpp>
#include <IdIOHandlerSocket.hpp>
#include <IdIOHandlerStack.hpp>
#include <IdSSL.hpp>
#include <IdSSLOpenSSL.hpp>
#include <System.JSON.hpp>
#include <Data.DBXJSON.hpp>
#include "sDialogs.hpp"

typedef String str;

// class c_main
class c_main
{
	public:

		MyTHREAD_TWO    *iMyTHREAD_TWO;                     bool TERMINATED; int  TINT_CURR; str PREFIX, BUFF_CURRENTLOG;

		TClipboard *clp;     TStringList *INBOX, *OUTBOX; bool INBOX_WRITED;

		TStringList *AUTOANSBUFF; // robotName, Token, file, dialog, message, uid, uname, usurname

		int RobotPathID;     str APIV;

		//////////////////////////////

		str f_ini,            f_servers, f_groups, f_workgroups, f_worktasks, f_users,           f_globaluserscache, f_currentlog;

		str p_robots, p_dialogs, p_dialogs_test, p_logs;

		//////////////////////////////

		int CurrentServer;

		//////////////////////////////

		int CNT_HELLO, CNT_ADDFRIEND, CNT_MESSAGES, CNT_CONFIRMFRIEND;

		//////////////////////////////

		c_main();

		void conf(bool save);
		void conf_ini(bool save);
		void conf_intervals(bool save);
		void conf_captcha(bool save);
		void conf_servers(bool save);
		void conf_groups(bool save);
		void conf_workgroups(bool save);
		void conf_worktasks(bool save);
		void conf_robots(int index, bool save);
		void conf_users(int index, bool save);
		void conf_dialogs(bool save);
        void conf_models(bool save);
        void conf_dialogs_test(bool save);

		void show_current_server();
		void increment_server();

		void get_robotdata( int index, str *GroupName, str *RobotName, str *Server_ID, str *Login, str *Password, str *Token, str *Activity );
		int  get_robotnext( str GroupName ); int TGC;

        //////////////////////////////

		void GlobalUsersCache_Add(str id);       TStringList *GLOBAL_USERS_CACHE;
		bool GlobalUsersCache_Exist(str id);
		void GlobalUsersCache_Delete(str id);

		//////////////////////////////

		void log( str data ); TStringList *LOG;
		void iSleep(int index);
		void logline( str data );
        str  jsonfix( str data );
		void buffer_write( str data );
		void search_request( str RequestUrl, str OffSet, str Count, int iteration );   int countOfprocessed;
		str  get_vkurl_param(str ParamName, str Data);
		str  getCountOfMesages(str Dialog);
		int  getCountOfHello(str GroupName);
		void DrawMessageBox(str Name, str Surname, str RobotGID, TStringList *DIALOGS);
		void DeleteUserFromQueueAndPutToDialogs(str UserID, str UserName, str UserSurname, str RobotName, bool msghello, str Text);
		void WriteInboxList(str Token);
		void WriteOutboxList(str Token, str RobotName);
		void SetAsRead(TStringList *DIALOG);
		str  GetLastStageName(TStringList *DIALOG);
		void GetOnlyOneStage(TStringList *MODEL, str StageName);
        int  GetMaxLevelsOfStages();
		void GetListOfLevel(int level,TStringList *L);
        str  CutLastLevel(str data);
        str  ConvertToVars(str extline);
        str  GetVarFrom(int index);
        str  CreateStageName(str data);
        bool ifStageNameExist(str StageName);
		str  getLastStage(str data);
		void log_transform();
		void generatelogname();
		void createStandartDataHello(TStringList *L);
		void createStandartDataModel(TStringList *L);
		void createStandartDataAutoAnsRules(TStringList *L);
		void createStandartDataAutoAnsDefault(TStringList *L);
		void createStandartDataAutoStopKeys(TStringList *L);
		void createStandartDataAutoStopPosts(TStringList *L);
		void checkLinkedStages();
		void make_new_inbox(TStringList *ALL, TStringList *DEST);
		void deleteDialogsPerRobot(str robotname);
		str  from_list_to_str(TStringList *L);
		int  getrobotindex(str robotname, TStringList *L);
        bool if_imbanned(str Token, str uid);

												TTreeNode *NODE; str FMODEL, MYPARENT;
        void LoadModel(int index);
		void LoadModelHello();               	TStringList *MODEL_HELLO;
        void LoadModelLogical();             	TStringList *MODEL_LOGICAL;
        void LoadModelLogicalTree(int level);
                                                bool MOD, IFCREATE, IFROOT;
        void LoadModelStage(str StageName);
        void LoadModelStagePosts();         	TStringList *POSTS;
        void LoadModelStageVariants();      	TStringList *VARIANTS;
        void LoadModelStageIfVariants();    	TStringList *IFVARIANTS;
        void LoadModelStageExtendeds();     	TStringList *EXTENDED;
		void LoadModelStageDefault();       	TStringList *DEFAULT;

		void ModelLogicalAddStage(TStringList *STAGE);
		void ModelLogicalChangeStage(TStringList *STAGE);
		void ModelLogicalDeleteStage(str StageName);
		void ModelLogicalDeleteStageDo(str StageName, TStringList *MODEL);
		void ModelLogicalDeleteStageDeleteLinked(str StageName, TStringList *MODEL);
		void ModelSaveAndReloadInterface();

		void LoadModelAutoAnsRules();           TStringList *MODEL_AUTOANS; str FAUTOANS;
		void LoadModelAutoAnsDefault(str file);

		void LoadModelAutoStopKeys();           TStringList *MODEL_AUTOSTOP_KEYS, *MODEL_AUTOSTOP_POSTS;
		void LoadModelAutoStopPosts();
};

// class c_process
class c_process
{
	public:

		c_process();

		bool Establish( str RobotName, str *Token );
		str  SendMessage( str UserID, str Message, str Token );
		str  AddToFriends( str UserID, str Message, str Token );
		str  RequestsFriends( str Token );

		void ProcessTwoOpen();
		void ProcessHello(str GroupName);
		void ProcessHelloDo(str Group, str UserID, str UserName, str UserSurname);
		void ProcessHelloDoSend(str Group, str UserID, str UserName, str UserSurname, str RobotName, str Token, str RobotPath);
		void ProcessFriendConfirm(str GroupName, str RobotName, str Server_ID, str Login, str Password, str Token, str Activity, str ModelFile);
		void ProcessAutoAnswer(str GroupName, str RobotName, str Server_ID, str Login, str Password, str Token, str Activity, str ModelFile);
		void ProcessAutoAnswerCheckNew(str RobotName, str Token);
		void ProcessAutoAnswerAddToDialogs(TStringList *NEWBOX, str RobotName, str Token, str Uid);
		void ProcessAutoAnswerRunBuffer();
		void ProcessSpeech(str GroupName, str RobotName, str Server_ID, str Login, str Password, str Token, str Activity, str ModelFile, str AutoStopKeysFile, str AutoStopPostsFile);
		void ProcessSpeechUser(str RobotGID, str Name, str SurName, str UserID, TStringList *DIALOG, str Token, str RobotModelFile, bool *SaveDialog, str AutoStopKeysFile, str AutoStopPostsFile);
		void ProcessSpeechUserDo(str UserID, TStringList *DIALOG, str STACK, str Token, str RobotModelFile, bool *SaveDialog, str AutoStopKeysFile, str AutoStopPostsFile, bool *MakeReaded);
        void ProcessTwoClose();

		void SendStage1(str UserID, TStringList *DIALOG, str STACK, str Token, TStringList *MODEL);
		void SendStageX(str UserID, TStringList *DIALOG, str STACK, str Token, TStringList *MODEL, str TARGET_STAGE);
		void SendAutoAns(str RobotName, str UserID, str MessageData, str Token, TStringList *DIALOG, bool *success);
		void SendAutoStop(str UserID, TStringList *DIALOG, str STACK, str Token, str AutoStopPostsFile);


};

// class c_ii
class c_ii
{
	public:

		c_ii();

		str  GetStage(str STACK, TStringList *STAGE);

		void MakeVariantMass(TStringList *STAGE, TStringList *VARIANTS);
		void WriteVariantTarget(bool *changed, TStringList *STAGE, TStringList *VARIANTS, str STACK);
		void CheckExtended(bool *extended, TStringList *STAGE, TStringList *VARIANTS, str *TARGETSTAGE);

		void GetChanged(TStringList *STAGE, TStringList *VARIANTS, str *TARGETSTAGE);
		void GetDefault(TStringList *STAGE, str *TARGETSTAGE);

		str  GetOldStage(str uid, str token, TStringList *RobotModel, str Message);
		void GetAutoAns(str *iodata, str *targetstage, str Token, str uid);

		bool IfAutoStop(str message, str file);
		str  GetAutoStopMessage(str file);
};

// class c_vcl
class c_vcl
{
	public:

		c_vcl();

		void obj_draw();

		void set();

		void unset();
		void set_enabled_conf_groups( bool enable );
		void set_enabled_conf_robots( bool enable );

		void set_enabled_dialogs( bool enable );
        void set_enabled_test_dialogs( bool enable );

		void groupechoReadRobots();
		void groupechoReadUsers();

		void Dialog_Add_User(str User);
		void Dialog_Add_Text(TStringList *TEXT);
		void Dialog_Add_Date(str Date);

		void EchoStatistic();

		str  FixInbox(str data);

        void ModelsSET(bool set);
        void GoToModels(str RobotName, int PositionView);
        void ModelStageClear();
        void GetAllStages(TsComboBox *CB);
        void GetAllRobots(TsComboBox *CB);
};

// class c_captcha
class c_captcha
{
	public:

	c_captcha();

	str GetAnswer( str CaptchaIMGURL );

	str FixURL( str Url );
	str SaveToFile( str Url );
};

// class t_form
class Tf : public TForm
{
	__published:
	TsSkinManager *MANAGER;
	TButton *BTEST1;
	TButton *BTEST2;
	TsPageControl *PAGES;
	TsTabSheet *sTabSheet1;
	TsTabSheet *sTabSheet2;
	TsTabSheet *sTabSheet3;
	TsTabSheet *sTabSheet4;
	TImageList *IL_PAGES;
	TsTabSheet *sTabSheet5;
	TsTabSheet *sTabSheet6;
	TsTabSheet *sTabSheet7;
	TTrayIcon *TRAY;
	TTimer *T_OBJ_DRAW;
	TsListView *LV_SERVERS;
	TLabel *Label2;
	TLabel *Label5;
	TLabel *Label6;
	TLabel *Label7;
	TsGroupBox *sGroupBox1;
	TsGroupBox *sGroupBox2;
	TsEdit *e_servers_client_id;
	TsEdit *e_servers_login;
	TsEdit *e_servers_password;
	TsButton *b_SERVERS_ADD;
	TPopupMenu *PM_SERVERS;
	TMenuItem *N1;
	TsGroupBox *sGroupBox3;
	TsGroupBox *sGroupBox4;
	TLabel *l_NextClientID;
	TLabel *Label4;
	TMenuItem *N2;
	TMenuItem *N3;
	TMenuItem *N4;
	TMenuItem *N5;
	TPopupMenu *PM_LOG;
	TMenuItem *N6;
	TMenuItem *N7;
	TPopupMenu *PM_USERS;
	TMenuItem *N10;
	TMenuItem *N12;
	TPopupMenu *PM_QUEUE;
	TMenuItem *N13;
	TMenuItem *N14;
	TMenuItem *N15;
	TMenuItem *N16;
	TsRadioGroup *rg_CAPTCHA_SERVICE;
	TEdit *e_CAPTCHA_KEY;
	TLabel *Label34;
	TsButton *b_CAPTCHA_GOTOPROFILE;
	TsPageControl *sPageControl1;
	TsTabSheet *sTabSheet12;
	TsListView *sListView6;
	TsGroupBox *sGroupBox20;
	TLabel *Label53;
	TLabel *Label54;
	TsGroupBox *sGroupBox21;
	TsRichEdit *sRichEdit5;
	TsGroupBox *sGroupBox14;
	TLabel *Label35;
	TLabel *Label36;
	TsEdit *sEdit1;
	TsEdit *sEdit2;
	TsButton *sButton1;
	TsPageControl *PAGES_CONFIGURATION;
	TsTabSheet *sTabSheet13;
	TsTabSheet *sTabSheet14;
	TsGroupBox *sGroupBox6;
	TLabel *Label9;
	TsEdit *e_conf_groups_create_name;
	TsButton *b_CONF_GROUPS_ADD;
	TsListView *LV_CONF_GROUPS;
	TsGroupBox *sGroupBox5;
	TPopupMenu *PM_GROUPS;
	TMenuItem *N17;
	TsButton *b_CONF_GROUPS_EDIT_APPLY;
	TsEdit *e_conf_groups_edit_name;
	TLabel *Label10;
	TsGroupBox *sGroupBox7;
	TsTabSheet *sTabSheet15;
	TsGroupBox *sGroupBox15;
	TsListView *LV_CONF_ROBOTS;
	TsGroupBox *sGroupBox16;
	TLabel *Label37;
	TLabel *Label38;
	TsEdit *e_conf_robots_create_login;
	TsEdit *e_conf_robots_create_password;
	TsButton *b_CONF_ROBOTS_CREATE;
	TsEdit *e_conf_robots_create_name;
	TLabel *Label39;
	TsGroupBox *sGroupBox17;
	TLabel *Label40;
	TLabel *Label41;
	TLabel *Label42;
	TsEdit *e_conf_robots_edit_login;
	TsEdit *e_conf_robots_edit_password;
	TsButton *b_CONF_ROBOTS_EDIT_APPLY;
	TsEdit *e_conf_robots_edit_name;
	TsComboBox *cb_conf_robots_edit_groups;
	TLabel *Label11;
	TsGroupBox *sGroupBox18;
	TsListView *LV_CONF_USERS;
	TPopupMenu *PM_ROBOTS;
	TMenuItem *N18;
	TMenuItem *N19;
	TMenuItem *N20;
	TsGroupBox *sGroupBox19;
	TLabel *Label12;
	TLabel *Label14;
	TLabel *Label16;
	TsEdit *e_conf_users_URL;
	TsBitBtn *b_CONF_USERS_SEARCH;
	TsEdit *e_conf_users_offSet;
	TsEdit *e_conf_users_Count;
	TMenuItem *N8;
	TMenuItem *N9;
	TsListView *LV_DIALOGS;
	TsGroupBox *sGroupBox10;
	TLabel *l_GID;
	TLabel *Label17;
	TsGroupBox *GROUP_DIALOG;
	TsRichEdit *ME_DIALOG;
	TsRichEdit *ME_INPUT;
	TsBitBtn *B_SENDMESSAGE;
	TPopupMenu *PM_ME_DIALOG;
	TMenuItem *N11;
	TMenuItem *N21;
	TMenuItem *N22;
	TsGroupBox *sGroupBox8;
	TLabel *Label13;
	TsEdit *e_int_addfr;
	TLabel *Label18;
	TLabel *Label19;
	TsEdit *e_int_api;
	TsEdit *e_int_msg;
	TsGroupBox *sGroupBox9;
	TListView *LV_WORKGROUPS;
	TMenuItem *N23;
	TMenuItem *N24;
	TMenuItem *Hellotxt1;
	TMenuItem *N25;
	TMenuItem *N26;
	TsGroupBox *sGroupBox11;
	TsGroupBox *sGroupBox12;
	TLabel *Label3;
	TLabel *Label8;
	TLabel *Label15;
	TsProgressBar *BAR;
	TLabel *l_CNT_HELLO;
	TLabel *l_CNT_ADDFRIEND;
	TLabel *l_CNT_MESSAGES;
	TsBitBtn *b_PROCESS_TWO_START;
	TsProgressBar *BARSMOOTH;
	TPopupMenu *PM_DIALOGS;
	TMenuItem *N27;
	TMenuItem *N28;
	TIdAntiFreeze *IdAntiFreeze1;
	TsCheckBox *ch_entimer;
	TLabel *Label20;
	TsEdit *e_int_timer_max;
	TLabel *Label21;
	TLabel *l_tinterval;
	TTimer *TINTERVAL;
	TsBitBtn *b_PROCESS_TWO_STOP;
	TsTreeViewEx *TREE;
	TPopupMenu *PM_URL;
	TMenuItem *N29;
	TsPageControl *PAGES_SPEECH;
	TsTabSheet *sTabSheet8;
	TsListView *LV_DIALOGS_TEST;
	TsGroupBox *sGroupBox13;
	TLabel *Label22;
	TLabel *Label23;
	TsGroupBox *sGroupBox22;
	TsTabSheet *sTabSheet9;
	TsTabSheet *sTabSheet10;
	TPopupMenu *PM_TRAY;
	TMenuItem *T1;
	TMenuItem *EST11;
	TLabel *Label26;
	TsComboBox *CB_MODELS_ROBOTS;
	TsGroupBox *sGroupBox24;
	TsGroupBox *GROUP;
	TsPageControl *PAGES_MODELS_PAGEVIEW;
	TsTabSheet *sTabSheet11;
	TsTabSheet *sTabSheet16;
	TMenuItem *N30;
	TsListView *LV_MODEL_HELLO;
	TsEdit *E_MODEL_HELLO_ADD;
	TsBitBtn *B_MODEL_HELLO_ADD;
	TPopupMenu *PM_MODEL_HELLO;
	TMenuItem *N31;
	TMenuItem *N32;
	TMenuItem *N33;
	TsGroupBox *sGroupBox25;
	TsGroupBox *GB_MODEL;
	TsGroupBox *sGroupBox27;
	TsGroupBox *sGroupBox28;
	TsGroupBox *sGroupBox29;
	TsComboBox *CB_MODEL_LOGICAL_IFVARIANTS;
	TsListBox *LI_MODEL_LOGICAL_POSTS;
	TsEdit *E_MODEL_LOGICAL_POSTS_ADD;
	TsBitBtn *B_MODEL_LOGICAL_POSTS_ADD;
	TsListBox *LI_MODEL_LOGICAL_VARIANTS;
	TsEdit *E_MODEL_LOGICAL_VARIANTS_ADD;
	TsButton *B_MODEL_LOGICAL_VARIANTS_ADD;
	TsLabel *sLabel1;
	TsGroupBox *sGroupBox30;
	TsLabel *sLabel2;
	TsListBox *LI_MODEL_LOGICAL_EXTENDED;
	TsEdit *E_MODEL_LOGICAL_EXTENDED_ADD;
	TsButton *B_MODEL_LOGICAL_EXTENDED_ADD;
	TsComboBox *CB_MODEL_LOGICAL_EXTENDED;
	TsComboBox *CB_MODEL_LOGICAL_DEFAULT;
	TsGroupBox *sGroupBox31;
	TsEdit *E_MODEL_LOGICAL_STAGE;
	TsButton *B_MODEL_LOGICAL_ADDEDIT;
	TPopupMenu *PM_TREE;
	TMenuItem *N34;
	TMenuItem *N35;
	TMenuItem *N36;
	TMenuItem *N37;
	TsRadioButton *RB_MODELS_B;
	TsRadioButton *RB_MODELS_A;
	TsButton *B_MODEL_LOGICAL_VARIANTS_APPLY;
	TsButton *B_MODEL_LOGICAL_EXTENDED_APPLY;
	TPopupMenu *PM_POSTS;
	TMenuItem *N38;
	TMenuItem *N39;
	TMenuItem *N40;
	TPopupMenu *PM_VARIANTS;
	TMenuItem *N41;
	TMenuItem *N42;
	TMenuItem *N43;
	TPopupMenu *PM_EXTENDED;
	TMenuItem *N44;
	TMenuItem *N45;
	TMenuItem *N46;
	TsComboBox *CB_DIALOGS_TEST_ROBOTS;
	TLabel *Label27;
	TsEdit *sEdit5;
	TsButton *sButton3;
	TsGroupBox *sGroupBox26;
	TLabel *Label28;
	TsButton *sButton4;
	TsRichEdit *ME_DIALOG_TEST;
	TsCheckBox *CH_LOG;
	TsTabSheet *sTabSheet17;
	TsRadioButton *RB_MODELS_C;
	TsListView *LV_MODEL_AUTOANS;
	TsGroupBox *sGroupBox23;
	TsEdit *E_MODEL_AUTOANS_RULETEXT;
	TsBitBtn *B_MODEL_AUTOANS_ADD;
	TsEdit *E_MODEL_AUTOANS_RULESTAGE;
	TPopupMenu *PM_MODEL_AUTOANS;
	TMenuItem *N47;
	TMenuItem *N48;
	TMenuItem *N49;
	TsGroupBox *sGroupBox32;
	TsEdit *E_MODEL_AUTOANS_DEFAULT;
	TMemo *me_createStandartDataModel;
	TMenuItem *N50;
	TMenuItem *N51;
	TLabel *Label1;
	TLabel *l_CNT_CONFIRMFRIENDS;
	TMenuItem *N52;
	TMenuItem *N53;
	TMenuItem *N54;
	TMenuItem *N55;
	TMenuItem *N56;
	TMenuItem *N57;
	TMenuItem *N58;
	TsRadioButton *RB_MODELS_D;
	TsTabSheet *sTabSheet18;
	TsPageControl *PAGES_AUTOSTOP;
	TsTabSheet *sTabSheet19;
	TsTabSheet *sTabSheet20;
	TsListView *LV_MODEL_AUTOSTOP_KEYS;
	TsGroupBox *sGroupBox33;
	TsEdit *E_MODEL_AUTOSTOP_KEYS;
	TsBitBtn *B_MODEL_AUTOSTOP_KEYS;
	TsListView *LV_MODEL_AUTOSTOP_POSTS;
	TsGroupBox *sGroupBox34;
	TsEdit *E_MODEL_AUTOSTOP_POSTS;
	TsBitBtn *B_MODEL_AUTOSTOP_POSTS;
	TPopupMenu *PM_MODELS_AUTOSTOP_KEYS;
	TPopupMenu *PM_MODELS_AUTOSTOP_POSTS;
	TMenuItem *N59;
	TMenuItem *N60;
	TMenuItem *N61;
	TMenuItem *N62;
	TMenuItem *N63;
	TMenuItem *N64;
	TListView *LV_WORKTASKS;
	TPopupMenu *PM_TASKS;
	TMenuItem *N65;
	TMenuItem *N66;
	TsCheckBox *CH_APIRET;
	TLabel *Label24;
	TsEdit *e_int_confirmfriend;
	TsCheckBox *CH_LOGGOTO;
	TsCheckBox *CH_LOG_PAUSES;
	TMemo *ME_LOG;
	TMenuItem *N67;
	TMenuItem *N68;
	TsOpenDialog *OD_LOG;
	TsEdit *e_autoanswerlimit;
	TGroupBox *GroupBox1;
	TGroupBox *GroupBox2;
	TsButton *b_MODEL_IMPORT;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall PAGESChange(TObject *Sender);
	void __fastcall TRAYClick(TObject *Sender);
	void __fastcall T_OBJ_DRAWTimer(TObject *Sender);
	void __fastcall N1Click(TObject *Sender);
	void __fastcall b_SERVERS_ADDClick(TObject *Sender);
	void __fastcall N2Click(TObject *Sender);
	void __fastcall N3Click(TObject *Sender);
	void __fastcall N4Click(TObject *Sender);
	void __fastcall N6Click(TObject *Sender);
	void __fastcall N7Click(TObject *Sender);
	void __fastcall BTEST2Click(TObject *Sender);
	void __fastcall N12Click(TObject *Sender);
	void __fastcall rg_CAPTCHA_SERVICEChange(TObject *Sender);
	void __fastcall b_CAPTCHA_GOTOPROFILEClick(TObject *Sender);
	void __fastcall PAGES_CONFIGURATIONChange(TObject *Sender);
	void __fastcall b_CONF_GROUPS_ADDClick(TObject *Sender);
	void __fastcall LV_CONF_GROUPSChange(TObject *Sender, TListItem *Item, TItemChange Change);
	void __fastcall b_CONF_GROUPS_EDIT_APPLYClick(TObject *Sender);
	void __fastcall LV_CONF_GROUPSClick(TObject *Sender);
	void __fastcall N17Click(TObject *Sender);
	void __fastcall LV_CONF_ROBOTSChange(TObject *Sender, TListItem *Item, TItemChange Change);
	void __fastcall LV_CONF_ROBOTSClick(TObject *Sender);
	void __fastcall b_CONF_ROBOTS_CREATEClick(TObject *Sender);
	void __fastcall N18Click(TObject *Sender);
	void __fastcall b_CONF_ROBOTS_EDIT_APPLYClick(TObject *Sender);
	void __fastcall N20Click(TObject *Sender);
	void __fastcall b_CONF_USERS_SEARCHClick(TObject *Sender);
	void __fastcall N10Click(TObject *Sender);
	void __fastcall N9Click(TObject *Sender);
	void __fastcall LV_DIALOGSChange(TObject *Sender, TListItem *Item, TItemChange Change);
	void __fastcall LV_DIALOGSClick(TObject *Sender);
	void __fastcall B_SENDMESSAGEClick(TObject *Sender);
	void __fastcall N11Click(TObject *Sender);
	void __fastcall N22Click(TObject *Sender);
	void __fastcall e_int_apiChange(TObject *Sender);
	void __fastcall e_conf_users_offSetChange(TObject *Sender);
	void __fastcall LV_WORKGROUPSClick(TObject *Sender);
	void __fastcall N23Click(TObject *Sender);
	void __fastcall N24Click(TObject *Sender);
	void __fastcall Hellotxt1Click(TObject *Sender);
	void __fastcall N26Click(TObject *Sender);
	void __fastcall LV_CONF_GROUPSDblClick(TObject *Sender);
	void __fastcall N27Click(TObject *Sender);
	void __fastcall N28Click(TObject *Sender);
	void __fastcall b_PROCESS_TWO_STARTClick(TObject *Sender);
	void __fastcall BTEST1Click(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall TINTERVALTimer(TObject *Sender);
	void __fastcall b_PROCESS_TWO_STOPClick(TObject *Sender);
	void __fastcall N29Click(TObject *Sender);
	void __fastcall TREEClick(TObject *Sender);
	void __fastcall CB_MODELS_ROBOTSChange(TObject *Sender);
	void __fastcall B_MODEL_HELLO_ADDClick(TObject *Sender);
	void __fastcall N31Click(TObject *Sender);
	void __fastcall N33Click(TObject *Sender);
	void __fastcall N34Click(TObject *Sender);
	void __fastcall N35Click(TObject *Sender);
	void __fastcall B_MODEL_LOGICAL_ADDEDITClick(TObject *Sender);
	void __fastcall TREEChange(TObject *Sender, TTreeNode *Node);
	void __fastcall N36Click(TObject *Sender);
	void __fastcall RB_MODELS_AClick(TObject *Sender);
	void __fastcall RB_MODELS_BClick(TObject *Sender);
	void __fastcall B_MODEL_LOGICAL_POSTS_ADDClick(TObject *Sender);
	void __fastcall B_MODEL_LOGICAL_VARIANTS_ADDClick(TObject *Sender);
	void __fastcall B_MODEL_LOGICAL_EXTENDED_ADDClick(TObject *Sender);
	void __fastcall N38Click(TObject *Sender);
	void __fastcall N40Click(TObject *Sender);
	void __fastcall N41Click(TObject *Sender);
	void __fastcall N43Click(TObject *Sender);
	void __fastcall N44Click(TObject *Sender);
	void __fastcall N46Click(TObject *Sender);
	void __fastcall LI_MODEL_LOGICAL_VARIANTSClick(TObject *Sender);
	void __fastcall LI_MODEL_LOGICAL_EXTENDEDClick(TObject *Sender);
	void __fastcall B_MODEL_LOGICAL_VARIANTS_APPLYClick(TObject *Sender);
	void __fastcall B_MODEL_LOGICAL_EXTENDED_APPLYClick(TObject *Sender);
	void __fastcall LV_DIALOGS_TESTClick(TObject *Sender);
	void __fastcall CH_LOGClick(TObject *Sender);
	void __fastcall RB_MODELS_CClick(TObject *Sender);
	void __fastcall B_MODEL_AUTOANS_ADDClick(TObject *Sender);
	void __fastcall N47Click(TObject *Sender);
	void __fastcall N49Click(TObject *Sender);
	void __fastcall E_MODEL_AUTOANS_DEFAULTChange(TObject *Sender);
	void __fastcall N51Click(TObject *Sender);
	void __fastcall sTabSheet9Show(TObject *Sender);
	void __fastcall N53Click(TObject *Sender);
	void __fastcall N55Click(TObject *Sender);
	void __fastcall N57Click(TObject *Sender);
	void __fastcall N58Click(TObject *Sender);
	void __fastcall RB_MODELS_DClick(TObject *Sender);
	void __fastcall B_MODEL_AUTOSTOP_KEYSClick(TObject *Sender);
	void __fastcall B_MODEL_AUTOSTOP_POSTSClick(TObject *Sender);
	void __fastcall N59Click(TObject *Sender);
	void __fastcall N60Click(TObject *Sender);
	void __fastcall N61Click(TObject *Sender);
	void __fastcall N62Click(TObject *Sender);
	void __fastcall N63Click(TObject *Sender);
	void __fastcall N64Click(TObject *Sender);
	void __fastcall LV_WORKTASKSClick(TObject *Sender);
	void __fastcall N65Click(TObject *Sender);
	void __fastcall N66Click(TObject *Sender);
	void __fastcall e_autoanswerlimitChange(TObject *Sender);
	void __fastcall b_MODEL_IMPORTClick(TObject *Sender);





	private:
	public:
	__fastcall Tf(TComponent* Owner);
	///////////////////////////////////////////////////////////
	c_main 	    *main;
	c_process   *proc;
	c_ii        *ii;
	c_vcl       *vcl;
	c_captcha   *captcha;

	CRITICAL_SECTION CS;
	///////////////////////////////////////////////////////////
};

extern PACKAGE Tf *f;

#endif
