#ifndef VK_APIH
#define VK_APIH

#include <IdHTTP.hpp>


// c_vk :: vk
class c_vk
{
	public:

	String API_VERSION;

    static void connect( String Client_ID, String login, String pass, String *token );
    static bool connected( String token );
    static String  VKAPI_HTTPGET( String url );
	static String  GetParameter( String ParameterName, String *ResponseData, bool Cut, bool ifInteger, bool *Received );
	static String  GetParameterIntString( int index, String Parameter1, String Parameter2, String *Response1, String *Response2, String Data );

	static bool Establish( String Client_ID, String Login, String Password, String *Token, String Api_Version, bool *WasConnected );

	static String  set_online( bool *success, String Token );
	static String  account_ban_user( bool *success, String UserId, String Token );
	static bool robot_in_ban( bool *success, String UserId, String Token );
	static String  users_get( bool *success, String UserIdsCommaLine, String Token, bool MySelf );
	static String  users_search( bool *success, String Request, String Token, int OffSet, int Count, int AgeFrom, int AgeTo, bool OnlyOnline, bool HasPhoto, String Interests );
	static String  users_search_from_url( bool *success, String Request, String Token );
	static String  groups_getMembers( bool *success, String GroupID, String Token, int OffSet, int Count );
    
	static String  friends_request( bool *success, String Token );
	static String  friends_add( bool *success, String UserID, String Token, String Message, String CaptchaSID, String CaptchaANS );
	static String  friends_delete( bool *success, String UserID, String Token );
	static String  friends_get( bool *success, String UserID, String Token, int OffSet, int Count );
    
	static String  messages_send( bool *success, String UserID, String Message, String Token, String CaptchaSID, String CaptchaANS );
	static String  messages_send2( bool *success, String UserID, String attachment, String Token );
	static String  messages_get( bool *success, int OffSet, int Count, String Token );
	static String  messages_getDialogs( bool *success, int OffSet, int Count, String Token );
	static String  messages_getHistory( bool *success, int OffSet, int Count, String UserID, String Token );
	static String  messages_markAsRead( bool *success, String MessageIDS, String Token );
    static String  messages_getUnreadConversations( bool *success, int OffSet, int Count, String Token );
    
	static String  UploadToServer( bool *success, String file, String upload_url );
	static String  photos_getUploadServer( bool *success, String Token );
	static String  photos_saveMessagesPhoto( bool *success, String photo, String server, String hash, String Token );
	static String  audio_getUploadServer( bool *success, String Token );
	static String  audioUploadToServer( bool *success, String file, String upload_url );
	static String  audio_Save( bool *success, String audio, String server, String hash, String Token );
	static String  docs_getMessagesUploadServer( bool *success, String Token );
	static String  docs_save( bool *success, String file, String title, String tags, String Token );
};

extern c_vk vk;

#endif  // VK_APIH
