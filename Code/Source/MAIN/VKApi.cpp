#include "VKApi.h"

#include <vcl.h>
#include <System.Classes.hpp>

#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdTCPClient.hpp>
#include <IdTCPConnection.hpp>
#include <IdIOHandler.hpp>
#include <IdIOHandlerSocket.hpp>
#include <IdIOHandlerStack.hpp>
#include <IdSSL.hpp>
#include <IdSSLOpenSSL.hpp>

#include "mmsystem.h"
#include <ostream>
#include <sstream>
#include <iomanip>
#include <cstdio>
#include "math.h"
#include <memory>
#include "inifiles.hpp"
#include <windows.h>
#include <wininet.h>

#include "Grin.h"


c_vk vk;

bool c_vk::connected( String token )
{
	bool isConnect = false;

	String url = "https://api.vk.com/method/users.get"
    +String("?access_token="+token)
    +"&v="+vk.API_VERSION;
    
	String RETURN = VKAPI_HTTPGET( url );

	if ( Pos("response",RETURN) != 0 )
	{
		isConnect = true;
	}
	return isConnect;
}
void c_vk::connect( String Client_ID, String login, String pass, String *token )
{
    String TOKEN = "NULL";
	String inifile =  g.GetDirectoryApplicationDatapath() + "Global.Conf.ini";

	std::auto_ptr<TMemIniFile> INI(new TMemIniFile( UnicodeString(inifile), TEncoding::UTF8 ));
	INI->WriteString( UnicodeString("OAUTH2"), UnicodeString("client_id"), UnicodeString(Client_ID)  );
	INI->WriteString( UnicodeString("OAUTH2"), UnicodeString("login"),     UnicodeString(login) 		);
	INI->WriteString( UnicodeString("OAUTH2"), UnicodeString("password"),  UnicodeString(pass) 		);
	INI->WriteString( UnicodeString("OAUTH2"), UnicodeString("token"),     UnicodeString("NULL") 	);
    INI->UpdateFile();

	String file2 = g.GetDirectoryApplicationExename() + "OAUTH2.exe";
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

		std::auto_ptr<TMemIniFile> INIX(new TMemIniFile( UnicodeString(inifile), TEncoding::UTF8 ));
		String T = INIX->ReadString( UnicodeString("OAUTH2"), UnicodeString("token"), UnicodeString("NULL") );
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
String  c_vk::VKAPI_HTTPGET( String url )
{
	String J = "NULL";

	try
	{
		std::auto_ptr<TIdHTTP> idHttp(new TIdHTTP(NULL));
		idHttp->HandleRedirects = true;
		idHttp->ConnectTimeout 	= 10000;
	    idHttp->ReadTimeout 	= 10000;

		std::auto_ptr<TIdSSLIOHandlerSocketOpenSSL> idSSL(new TIdSSLIOHandlerSocketOpenSSL());
		idHttp->IOHandler = idSSL.get();
		J = idHttp->Get( idHttp->URL->URLEncode(url) );
	}
	catch ( Exception *ex )
	{
		ShowMessage( L"Ошибка: "+ex->Message );
	}

	return J;
}
String  c_vk::GetParameter( String ParameterName, String *ResponseData, bool Cut, bool ifInteger, bool *Received )
{
	String Response = *ResponseData;
	String tw = "\"";
	String data;
	if ( ifInteger ) 
    {
        data = tw+ParameterName+tw+":";
    }
	else             
    {
        data = tw+ParameterName+tw+":"+tw;
    }

	int p1 = Pos( data, Response );
	String j = Response.SubString(p1+data.Length(), Response.Length());

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
String  c_vk::GetParameterIntString( int index, String Parameter1, String Parameter2, String *Response1, String *Response2, String Data )
{
	String symb = "\"";

	String mainkey1 = "{\""+Parameter1+"\":";
	String mainkey2 = Parameter2+"\":\"";

	for ( int c = 1; c <= index; c++ )
	{
		int pos = Pos(mainkey1,Data);

		String ActualData = Data.SubString( pos+mainkey1.Length(), Data.Length() );
		int p = Pos("}",ActualData);
		ActualData = ActualData.SubString(1,p-1);

		Data = Data.SubString( pos+mainkey1.Length(), Data.Length() );
		p = Pos("}",Data);
		Data = Data.SubString(p+1,Data.Length());

		if ( c == index )
		{
			bool istext = false;
			String buff = ActualData = Trim(ActualData);
			int p1 = Pos(",",ActualData);
			if ( p1 > 0 ) 
            { 
                buff = ActualData.SubString(1,p1-1); istext = true; 
            }
			*Response1 = buff;
			if ( istext )
			{
				int pk2 = Pos(mainkey2,ActualData);
				String buff2 = ActualData.SubString(pk2+mainkey2.Length(),ActualData.Length());
				buff2 = buff2.SubString(1,buff2.Length()-1);
				*Response2 = buff2;
			}
		}
	}
    return String("");
}

bool c_vk::Establish( String Client_ID, String Login, String Password, String *Token, String Api_Version, bool *WasConnected )
{
	vk.API_VERSION = Api_Version;

	String token = *Token;
	bool isCONNECTION = vk.connected( token );
	if ( ! isCONNECTION )
	{
		*WasConnected = false;
		vk.connect( Client_ID, Login, Password, &token );
		isCONNECTION = vk.connected( token );
	}
	else
	{
		*WasConnected = true;
    }

	*Token = token;
	return isCONNECTION;
}
String  c_vk::set_online( bool *success, String Token )
{
	String J = "NULL";

	try
	{
		std::auto_ptr<TIdHTTP> idHttp(new TIdHTTP(NULL));
		idHttp->HandleRedirects = true;
		idHttp->ConnectTimeout 	= 10000;
	    idHttp->ReadTimeout 	= 10000;

		String URL = "https://api.vk.com/method/account.setOnline"
        +String("?voip=0")
        +"&access_token="+Token
        +"&v="+vk.API_VERSION;

		try
		{
			J = idHttp->Get( idHttp->URL->URLEncode( URL ) );

			*success = true;
		}
		catch (...)
		{
			*success = false;
        }
	}
	catch ( Exception *ex )
	{
		J = ex->Message;
	}

	return J;
}
String  c_vk::account_ban_user( bool *success, String UserId, String Token )
{
	String J = "NULL";

	try
	{
		std::auto_ptr<TIdHTTP> idHttp(new TIdHTTP(NULL));
		idHttp->HandleRedirects = true;
		idHttp->ConnectTimeout 	= 10000;
	    idHttp->ReadTimeout 	= 10000;

		String URL = "https://api.vk.com/method/account.banUser"
        +String("?user_id="+UserId)
        +"&access_token="+Token
        +"&v="+vk.API_VERSION;

		try
		{
			J = idHttp->Get( idHttp->URL->URLEncode( URL ) );
			*success = true;
		}
		catch (...)
		{
			*success = false;
        }
	}
	catch ( Exception *ex )
	{
		J = ex->Message;
	}

	return J;
}
bool c_vk::robot_in_ban( bool *success, String UserId, String Token )
{
	bool J = false;

	try
	{
		std::auto_ptr<TIdHTTP> idHttp(new TIdHTTP(NULL));
		idHttp->HandleRedirects = true;
		idHttp->ConnectTimeout 	= 10000;
	    idHttp->ReadTimeout 	= 10000;

		String URL = "https://api.vk.com/method/users.get"
        +String("?user_ids="+UserId)
        +"&fields=blacklisted"
        +"access_token="+Token
        +"&v="+vk.API_VERSION;
        
		String R;
		try
		{
			R = idHttp->Get( idHttp->URL->URLEncode( URL ) );
			*success = true;
		}
		catch (...)
		{
			*success = false;
        }

		String fx = "blacklisted\":1";
		if ( Pos(fx,R) != 0 ) 
        {
            J = true;
        }
	}
	catch ( Exception *ex )
	{
	}

	return J;
}
String  c_vk::users_get( bool *success, String UserIdsCommaLine, String Token, bool MySelf )
{
	String J = "NULL";

	try
	{
		std::auto_ptr<TIdHTTP> idHttp(new TIdHTTP(NULL));
		idHttp->HandleRedirects = true;
		idHttp->ConnectTimeout 	= 10000;
	    idHttp->ReadTimeout 	= 10000;

		String URL = "https://api.vk.com/method/users.get?access_token="+Token+"&v="+vk.API_VERSION;
		if ( !MySelf ) 
        {
            URL = URL+"&user_ids="+UserIdsCommaLine;
        }

		try
		{
			J = idHttp->Get( idHttp->URL->URLEncode( URL ) );
			*success = true;
		}
		catch (...)
		{
			*success = false;
        }
	}
	catch ( Exception *ex )
	{
		J = ex->Message;
	}
	return J;
}
String  c_vk::users_search( bool *success, String Request, String Token, int OffSet, int Count, int AgeFrom, int AgeTo, bool OnlyOnline, bool HasPhoto, String Interests )
{
	String J = "NULL";

	try
	{
		std::auto_ptr<TIdHTTP> idHttp(new TIdHTTP(NULL));
		idHttp->HandleRedirects = true;
		idHttp->ConnectTimeout 	= 10000;
	    idHttp->ReadTimeout 	= 10000;

		String Online  = "&online=0"; 
        if ( OnlyOnline ) 
        {
            Online = "&online=1";
        }
		String exPhoto = "&has_photo=0"; 
        if ( HasPhoto ) 
        {
            exPhoto = "&has_photo=1";
        }
		String exInterests = ""; 
        if ( Interests.Length() != 0 ) 
        {
            exInterests = "&interests="+Interests;
        }

		String URL = "https://api.vk.com/method/users.search?q="
        +Request                                                                  
        +"&fields=sex,can_write_private_message,can_send_friend_request,last_seen"    
        +exPhoto
        +exInterests
        +"&sort=0"
        +Online
        +"&age_from="+IntToStr(AgeFrom)
        +"&age_to="+IntToStr(AgeTo)
        +"&offset="+IntToStr(OffSet)
        +"&count="+IntToStr(Count)
        +"&access_token="+Token
        +"&v="+vk.API_VERSION;

		try
		{
			J = idHttp->Get( idHttp->URL->URLEncode( URL ) );
			*success = true;
		}
		catch (...)
		{
			*success = false;
        }
	}
	catch ( Exception *ex )
	{
		J = ex->Message;
	}

	return J;
}
String  c_vk::users_search_from_url( bool *success, String Request, String Token )
{
	String J = "NULL";

	try
	{
		std::auto_ptr<TIdHTTP> idHttp(new TIdHTTP(NULL));
		idHttp->HandleRedirects = true;
		idHttp->ConnectTimeout 	= 10000;
	    idHttp->ReadTimeout 	= 10000;

		String URL = "https://api.vk.com/method/users.search?"
        +Request
        +"&access_token="+Token
        +"&v="+vk.API_VERSION;

		try
		{
			J = idHttp->Get( idHttp->URL->URLEncode( URL ) );
			*success = true;
		}
		catch (...)
		{
			*success = false;
        }
	}
	catch ( Exception *ex )
	{
		J = ex->Message;
	}

	return J;
}
String  c_vk::groups_getMembers( bool *success, String GroupID, String Token, int OffSet, int Count )
{
	String J = "NULL";

	try
	{
		std::auto_ptr<TIdHTTP> idHttp(new TIdHTTP(NULL));
		idHttp->HandleRedirects = true;
		idHttp->ConnectTimeout 	= 10000;
	    idHttp->ReadTimeout 	= 10000;

		String URL = "https://api.vk.com/method/groups.getMembers"
        +String("?group_id="+GroupID)
        +"&fields=sex,online,online_mobile,can_write_private_message,last_seen"
        +"&offset="+IntToStr(OffSet)
        +"&count="+IntToStr(Count)
        +"&access_token="+Token
        +"&v="+vk.API_VERSION;

		try
		{
			J = idHttp->Get( idHttp->URL->URLEncode( URL ) );
			*success = true;
		}
		catch (...)
		{
			*success = false;
        }
	}
	catch ( Exception *ex )
	{
		J = ex->Message;
	}

	return J;
}
String  c_vk::friends_request( bool *success, String Token )
{
	String J = "NULL";

	try
	{
		std::auto_ptr<TIdHTTP> idHttp(new TIdHTTP(NULL));
		idHttp->HandleRedirects = true;
		idHttp->ConnectTimeout 	= 10000;
	    idHttp->ReadTimeout 	= 10000;

		String URL = "https://api.vk.com/method/friends.getRequests"
        +String("?offset=0")
        +"&extended=1&need_mutual=0&out=0&need_viewed=1"
        +"&access_token="+Token
        +"&v="+vk.API_VERSION;

		try
		{
			J = idHttp->Get( idHttp->URL->URLEncode( URL ) );
			*success = true;
		}
		catch (...)
		{
			*success = false;
        }
	}
	catch ( Exception *ex )
	{
		J = ex->Message;
	}

	return J;
}
String  c_vk::friends_add( bool *success, String UserID, String Token, String Message, String CaptchaSID, String CaptchaANS )
{
	String J = "NULL";

	try
	{
		std::auto_ptr<TIdHTTP> idHttp(new TIdHTTP(NULL));
		idHttp->HandleRedirects = true;
		idHttp->ConnectTimeout 	= 10000;
	    idHttp->ReadTimeout 	= 10000;

		if ( CaptchaSID.Length() > 0 ) 
        {
            CaptchaSID = "&captcha_sid=" + CaptchaSID;
        }
		if ( CaptchaANS.Length() > 0 ) 
        {
            CaptchaANS = "&captcha_key=" + CaptchaANS;
        }

		String URL = "https://api.vk.com/method/friends.add"
        +String("?user_id="+UserID)
        +CaptchaSID
        +CaptchaANS
        +"&text="+Message
        +"&access_token="+Token
        +"&v="+vk.API_VERSION;

		try
		{
			J = idHttp->Get( idHttp->URL->URLEncode( URL ) );

			*success = true;
		}
		catch (...)
		{
			*success = false;
        }
	}
	catch ( Exception *ex )
	{
		J = ex->Message;
	}

	return J;
}
String  c_vk::friends_delete( bool *success, String UserID, String Token )
{
	String J = "NULL";

	try
	{
		std::auto_ptr<TIdHTTP> idHttp(new TIdHTTP(NULL));
		idHttp->HandleRedirects = true;
		idHttp->ConnectTimeout 	= 10000;
	    idHttp->ReadTimeout 	= 10000;

		String URL = "https://api.vk.com/method/friends.delete"
        +String("?user_id="+UserID)
        +"&access_token="+Token
        +"&v="+vk.API_VERSION;

		try
		{
			J = idHttp->Get( idHttp->URL->URLEncode( URL ) );
			*success = true;
		}
		catch (...)
		{
			*success = false;
		}
	}
	catch ( Exception *ex )
	{
		J = ex->Message;
	}

	return J;
}
String  c_vk::friends_get( bool *success, String UserID, String Token, int OffSet, int Count )
{
	String J = "NULL";

	try
	{
		std::auto_ptr<TIdHTTP> idHttp(new TIdHTTP(NULL));
		idHttp->HandleRedirects = true;
		idHttp->ConnectTimeout 	= 10000;
	    idHttp->ReadTimeout 	= 10000;

		String URL = "https://api.vk.com/method/friends.get"
        +String("?user_id="+UserID)
        +"&fields=sex,can_write_private_message,online"
        +"&offset="+IntToStr(OffSet)
        +"&count="+IntToStr(Count)
        +"&access_token="+Token
        +"&v="+vk.API_VERSION;

		try
		{
			J = idHttp->Get( idHttp->URL->URLEncode( URL ) );
			*success = true;
		}
		catch (...)
		{
			*success = false;
        }
	}
	catch ( Exception *ex )
	{
		J = ex->Message;
	}

	return J;
}
String  c_vk::messages_send( bool *success, String UserID, String Message, String Token, String CaptchaSID, String CaptchaANS )
{
	String J = "NULL";

	try
	{
		std::auto_ptr<TIdHTTP> idHttp(new TIdHTTP(NULL));
		idHttp->HandleRedirects = true;
		idHttp->ConnectTimeout 	= 10000;
	    idHttp->ReadTimeout 	= 10000;

		if ( CaptchaSID.Length() > 0 ) 
        {
            CaptchaSID = "&captcha_sid=" + CaptchaSID;
        }
		if ( CaptchaANS.Length() > 0 ) 
        {
            CaptchaANS = "&captcha_key=" + CaptchaANS;
        }

		String URL = "https://api.vk.com/method/messages.send"
        +String("?user_id="+UserID)
        +CaptchaSID
        +CaptchaANS
        +"&message="+Message
        +"&access_token="
        +Token+"&v="
        +vk.API_VERSION;

		try
		{
			J = idHttp->Get( idHttp->URL->URLEncode( URL ) );
			*success = true;
		}
		catch (...)
		{
			*success = false;
        }
	}
	catch ( Exception *ex )
	{
		J = ex->Message;
	}

	return J;
}
String  c_vk::messages_send2( bool *success, String UserID, String attachment, String Token )
{
	String J = "NULL";

	try
	{
		std::auto_ptr<TIdHTTP> idHttp(new TIdHTTP(NULL));
		idHttp->HandleRedirects = true;
		idHttp->ConnectTimeout 	= 10000;
	    idHttp->ReadTimeout 	= 10000;

		String URL = "https://api.vk.com/method/messages.send"
        +String("?user_id="+UserID)
        +"&attachment="+attachment
        +"&access_token="+Token
        +"&v="+vk.API_VERSION;

		try
		{
			J = idHttp->Get( idHttp->URL->URLEncode( URL ) );
			*success = true;
		}
		catch (...)
		{
			*success = false;
        }
	}
	catch ( Exception *ex )
	{
		J = ex->Message;
	}

	return J;
}
String  c_vk::messages_getUnreadConversations( bool *success, int OffSet, int Count, String Token )
{
	String J = "NULL";

	try
	{
		std::auto_ptr<TIdHTTP> idHttp(new TIdHTTP(NULL));
		idHttp->HandleRedirects = true;
		idHttp->ConnectTimeout 	= 10000;
	    idHttp->ReadTimeout 	= 10000;

		String URL = "https://api.vk.com/method/messages.getConversations"
        +String("?offset="+IntToStr(OffSet))                                                           
        +"&count="+IntToStr(Count) 
        +"&filter=unread" 
        +"&access_token="+Token
        +"&v="+vk.API_VERSION;

		try
		{
			J = idHttp->Get( idHttp->URL->URLEncode( URL ) );
			*success = true;
		}
		catch (...)
		{
			*success = false;
        }
	}
	catch ( Exception *ex )
	{
		J = ex->Message;
	}

	return J;
}
String  c_vk::messages_get( bool *success, int OffSet, int Count, String Token )
{
	String J = "NULL";

	try
	{
        std::auto_ptr<TIdHTTP> idHttp(new TIdHTTP(NULL));
		idHttp->HandleRedirects = true;
		idHttp->ConnectTimeout 	= 10000;
	    idHttp->ReadTimeout 	= 10000;

		String URL = "https://api.vk.com/method/messages.get"
        +String("?offset="+IntToStr(OffSet))
        +"&count="+IntToStr(Count)
        +"&access_token="+Token
        +"&v="+vk.API_VERSION;

		try
		{
			J = idHttp->Get( idHttp->URL->URLEncode( URL ) );
			*success = true;
		}
		catch (...)
		{
			*success = false;
        }
	}
	catch ( Exception *ex )
	{
		J = ex->Message;
	}

	return J;
}
String  c_vk::messages_getDialogs( bool *success, int OffSet, int Count, String Token )
{
	String J = "NULL";

	try
	{
		std::auto_ptr<TIdHTTP> idHttp(new TIdHTTP(NULL));
		idHttp->HandleRedirects = true;
		idHttp->ConnectTimeout 	= 10000;
	    idHttp->ReadTimeout 	= 10000;

		String URL = "https://api.vk.com/method/messages.getDialogs"
        +String("?offset="+IntToStr(OffSet))
        +"&count="+IntToStr(Count)
        +"&access_token="+Token
        +"&v="+vk.API_VERSION;

		try
		{
			J = idHttp->Get( idHttp->URL->URLEncode( URL ) );
			*success = true;
		}
		catch (...)
		{
			*success = false;
        }
	}
	catch ( Exception *ex )
	{
		J = ex->Message;
	}

	return J;
}
String  c_vk::messages_getHistory( bool *success, int OffSet, int Count, String UserID, String Token )
{
	String J = "NULL";

	try
	{
		std::auto_ptr<TIdHTTP> idHttp(new TIdHTTP(NULL));
		idHttp->HandleRedirects = true;
		idHttp->ConnectTimeout 	= 10000;
	    idHttp->ReadTimeout 	= 10000;

		String URL = "https://api.vk.com/method/messages.getHistory"
        +String("?offset="+IntToStr(OffSet))
        +"&count="+IntToStr(Count)
        +"&user_id="+UserID
        +"&access_token="+Token
        +"&v="+vk.API_VERSION;

		try
		{
			J = idHttp->Get( idHttp->URL->URLEncode( URL ) );
			*success = true;
		}
		catch (...)
		{
			*success = false;
        }
	}
	catch ( Exception *ex )
	{
		J = ex->Message;
	}

	return J;
}
String  c_vk::messages_markAsRead( bool *success, String MessageIDS, String Token )
{
	String J = "NULL";

	try
	{
		std::auto_ptr<TIdHTTP> idHttp(new TIdHTTP(NULL));
		idHttp->HandleRedirects = true;
		idHttp->ConnectTimeout 	= 10000;
	    idHttp->ReadTimeout 	= 10000;

		String URL = "https://api.vk.com/method/messages.markAsRead"
        +String("?message_ids="+MessageIDS)
        +"&access_token="+Token
        +"&v="+vk.API_VERSION;

		try
		{
			J = idHttp->Get( idHttp->URL->URLEncode( URL ) );
			*success = true;
		}
		catch (...)
		{
			*success = false;
        }
	}
	catch ( Exception *ex )
	{
		J = ex->Message;
	}

	return J;
}
String  c_vk::UploadToServer( bool *success, String file, String upload_url )
{
	std::auto_ptr<TIdHTTP> idHttp(new TIdHTTP(NULL));
	idHttp->HandleRedirects = true;

	std::auto_ptr<TStringStream> RETURN(new TStringStream);
	std::auto_ptr<TIdMultiPartFormDataStream> Stream(new TIdMultiPartFormDataStream);
    
	idHttp->Request->ContentType = "multipart/form-data";
	Stream->AddFile("photo", file, "multipart/form-data");
	idHttp->Post( upload_url , Stream.get(), RETURN.get());

    return RETURN->DataString;
}
String  c_vk::photos_getUploadServer( bool *success, String Token )
{
	String J = "NULL";

	try
	{
		std::auto_ptr<TIdHTTP> idHttp(new TIdHTTP(NULL));
		idHttp->HandleRedirects = true;
		idHttp->ConnectTimeout 	= 10000;
	    idHttp->ReadTimeout 	= 10000;

		String URL = "https://api.vk.com/method/photos.getMessagesUploadServer"
        +String("?access_token="+Token)
        +"&v="+vk.API_VERSION;

		try
		{
			J = idHttp->Get( idHttp->URL->URLEncode( URL ) );
			*success = true;
		}
		catch (...)
		{
			*success = false;
        }
	}
	catch ( Exception *ex )
	{
		J = ex->Message;
	}

	return J;
}
String  c_vk::photos_saveMessagesPhoto( bool *success, String photo, String server, String hash, String Token )
{
	String J = "NULL";

	try
	{
		std::auto_ptr<TIdHTTP> idHttp(new TIdHTTP(NULL));
		idHttp->HandleRedirects = true;
		idHttp->ConnectTimeout 	= 10000;
	    idHttp->ReadTimeout 	= 10000;

		String URL = "https://api.vk.com/method/photos.saveMessagesPhoto"
        +String("?photo="+photo)
        +"&server="+server
        +"&hash="+hash
        +"&access_token="+Token
        +"&v="+vk.API_VERSION;

		try
		{
			J = idHttp->Get( idHttp->URL->URLEncode( URL ) );
			*success = true;
		}
		catch (...)
		{
			*success = false;
		}
	}
	catch ( Exception *ex )
	{
		J = ex->Message;
	}

	return J;
}
String  c_vk::audio_getUploadServer( bool *success, String Token )
{
	String J = "NULL";

	try
	{
		std::auto_ptr<TIdHTTP> idHttp(new TIdHTTP(NULL));
		idHttp->HandleRedirects = true;
		idHttp->ConnectTimeout 	= 10000;
	    idHttp->ReadTimeout 	= 10000;

		String URL = "https://api.vk.com/method/audio.getUploadServer"
        +String("?access_token="+Token)
        +"&v="+vk.API_VERSION;

		try
		{
			J = idHttp->Get( idHttp->URL->URLEncode( URL ) );
			*success = true;
		}
		catch (...)
		{
			*success = false;
		}
	}
	catch ( Exception *ex )
	{
		J = ex->Message;
	}

	return J;
}
String  c_vk::audioUploadToServer( bool *success, String file, String upload_url )
{
	std::auto_ptr<TIdHTTP> HTTP(new TIdHTTP(NULL));
	HTTP->HandleRedirects = true;

	std::auto_ptr<TStringStream> RETURN(new TStringStream);
	std::auto_ptr<TIdMultiPartFormDataStream>Stream(new TIdMultiPartFormDataStream);
    
	HTTP->Request->ContentType = "multipart/form-data";
	Stream->AddFile("file", file, "multipart/form-data");
	HTTP->Post( upload_url , Stream.get(), RETURN.get());

    return RETURN->DataString;
}
String  c_vk::audio_Save( bool *success, String audio, String server, String hash, String Token )
{
	String J = "NULL";

	try
	{
		std::auto_ptr<TIdHTTP> idHttp(new TIdHTTP(NULL));
		idHttp->HandleRedirects = true;
		idHttp->ConnectTimeout 	= 10000;
	    idHttp->ReadTimeout 	= 10000;

		String URL = "https://api.vk.com/method/audio.save"
        +String("?audio="+audio)
        +"&server="+server
        +"&hash="+hash
        +"&access_token="+Token
        +"&v="+vk.API_VERSION;

		try
		{
			J = idHttp->Get( idHttp->URL->URLEncode( URL ) );
			*success = true;
		}
		catch (...)
		{
			*success = false;
		}
	}
	catch ( Exception *ex )
	{
		J = ex->Message;
	}

	return J;
}
String  c_vk::docs_getMessagesUploadServer( bool *success, String Token )
{
	String J = "NULL";

	try
	{
		std::auto_ptr<TIdHTTP> idHttp(new TIdHTTP(NULL));
		idHttp->HandleRedirects = true;
		idHttp->ConnectTimeout 	= 10000;
	    idHttp->ReadTimeout 	= 10000;

		String URL = "https://api.vk.com/method/docs.getMessagesUploadServer"
        +String("?type=audio_message")+
        +"&access_token="+Token
        +"&v="+vk.API_VERSION;

		try
		{
			J = idHttp->Get( idHttp->URL->URLEncode( URL ) );
			*success = true;
		}
		catch (...)
		{
			*success = false;
		}
	}
	catch ( Exception *ex )
	{
		J = ex->Message;
	}

	return J;
}
String  c_vk::docs_save( bool *success, String file, String title, String tags, String Token )
{
	String J = "NULL";

	try
	{
		std::auto_ptr<TIdHTTP> idHttp(new TIdHTTP(NULL));
		idHttp->HandleRedirects = true;
		idHttp->ConnectTimeout 	= 10000;
	    idHttp->ReadTimeout 	= 10000;

		String URL = "https://api.vk.com/method/docs.save"
        +String("?file="+file)
        +"&title="+title
        +"&tags="+tags
        +"&access_token="+Token
        +"&v="+vk.API_VERSION;

		try
		{
			J = idHttp->Get( idHttp->URL->URLEncode( URL ) );
			*success = true;
		}
		catch (...)
		{
			*success = false;
		}
	}
	catch ( Exception *ex )
	{
		J = ex->Message;
	}

	return J;
}
