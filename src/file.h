#ifndef ALL
#define ALL
// #define DEBUG
inline const char* PUBLISH_REQUEST = "1111111111";
inline const char* FETCH_REQUEST = "1111100000";
inline const char* DOWN_REQUEST = "1010101010";
inline const char* PING_REQUEST = "1000000001";
inline const char* DELETE_FILE_REQUEST = "1110001110";
inline const char* REGISTER_REQUEST = "1100101101";
inline const char* SIGNIN_REQUEST = "1110001110";
inline const char* SIGNOUT_REQUEST = "0011010101";
inline const int REQUEST_ID_LENGTH = 10;
inline const int HASHINFO_LENGTH = 10;


inline const char *SERVER_LISTEN_IP = "127.0.0.1";
inline const int SERVER_LISTEN_PORT = 7000;
inline const char *SERVER_LISTEN_IP_BACKUP = "127.0.0.1";
inline const int SERVER_LISTEN_PORT_BACKUP = 7001;

#endif