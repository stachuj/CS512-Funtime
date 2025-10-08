#ifndef SHARED_HPP
#define SHARED_HPP

#include <raylib-cpp.hpp>
#include <raylib-cpp-utils.hpp>

#if defined(_WIN32) || defined(_WIN64)

/*
 * The following defines are meant to avoid conflicts between raylib and windows.h
 * https://github.com/raysan5/raylib/issues/857
 */

//Stupid hacky fix that prevents conflicts between raylib and windows.h
#define NOGDICAPMASKS     // CC_*, LC_*, PC_*, CP_*, TC_*, RC_
#define NOVIRTUALKEYCODES // VK_*
#define NOWINMESSAGES     // WM_*, EM_*, LB_*, CB_*
#define NOWINSTYLES       // WS_*, CS_*, ES_*, LBS_*, SBS_*, CBS_*
#define NOSYSMETRICS      // SM_*
#define NOMENUS           // MF_*
#define NOICONS           // IDI_*
#define NOKEYSTATES       // MK_*
#define NOSYSCOMMANDS     // SC_*
#define NORASTEROPS       // Binary and Tertiary raster ops
#define NOSHOWWINDOW      // SW_*
#define OEMRESOURCE       // OEM Resource values
#define NOATOM            // Atom Manager routines
#define NOCLIPBOARD       // Clipboard routines
#define NOCOLOR           // Screen colors
#define NOCTLMGR          // Control and Dialog routines
#define NODRAWTEXT        // DrawText() and DT_*
#define NOGDI             // All GDI defines and routines
#define NOKERNEL          // All KERNEL defines and routines
#define NOUSER            // All USER defines and routines
/*#define NONLS             // All NLS defines and routines*/
#define NOMB              // MB_* and MessageBox()
#define NOMEMMGR          // GMEM_*, LMEM_*, GHND, LHND, associated routines
#define NOMETAFILE        // typedef METAFILEPICT
#define NOMINMAX          // Macros min(a,b) and max(a,b)
#define NOMSG             // typedef MSG and associated routines
#define NOOPENFILE        // OpenFile(), OemToAnsi, AnsiToOem, and OF_*
#define NOSCROLL          // SB_* and scrolling routines
#define NOSERVICE         // All Service Controller routines, SERVICE_ equates, etc.
#define NOSOUND           // Sound driver routines
#define NOTEXTMETRIC      // typedef TEXTMETRIC and associated routines
#define NOWH              // SetWindowsHook and WH_*
#define NOWINOFFSETS      // GWL_*, GCL_*, associated routines
#define NOCOMM            // COMM driver routines
#define NOKANJI           // Kanji support stuff.
#define NOHELP            // Help engine interface.
#define NOPROFILER        // Profiler interface.
#define NODEFERWINDOWPOS  // DeferWindowPos routines
#define NOMCX             // Modem Configuration Extensions

// Type required before windows.h inclusion
typedef struct tagMSG *LPMSG;

#include <winsock2.h> // Has to be included before windows.h
#include <windows.h>

//ChatGPT saved my life today
//#pragma comment(lib, "Ws2_32.lib")

#endif // WINDOWS

#define PROTOCOL_NAME "funtime"
#define PORT 42042

#define NBN_LogInfo(...) TraceLog(LOG_INFO, __VA_ARGS__)
#define NBN_LogError(...) TraceLog(LOG_ERROR, __VA_ARGS__)
#define NBN_LogDebug(...) TraceLog(LOG_DEBUG, __VA_ARGS__)
#define NBN_LogTrace(...) TraceLog(LOG_TRACE, __VA_ARGS__)
#define NBN_LogWarning(...) TraceLog(LOG_WARNING, __VA_ARGS__)

#define NBNET_IMPL

#include "nbnet.h"
#include "udp.h"


#define TICK_RATE 60

#define GAME_WIDTH 800
#define GAME_HEIGHT 600

#define MIN_FLOAT_VAL -5 // Minimum value of networked client float value
#define MAX_FLOAT_VAL 5 // Maximum value of networked client float value

// Maximum number of connected clients at a time
#define MAX_CLIENTS 4

// Max number of colors for client to switch between
#define MAX_COLORS 7

// A code passed by the server when closing a client connection due to being full (max client count reached)
#define SERVER_FULL_CODE 42

// Message ids
enum
{
    CHANGE_COLOR_MESSAGE,
    UPDATE_STATE_MESSAGE,
    GAME_STATE_MESSAGE
};

// Messages

typedef struct
{
    int x;
    int y;
    float val;
} UpdateStateMessage;

// Client colors used for ChangeColorMessage and GameStateMessage messages
typedef enum
{
    CLI_RED,
    CLI_GREEN,
    CLI_BLUE,
    CLI_YELLOW,
    CLI_ORANGE,
    CLI_PURPLE,
    CLI_PINK
} ClientColor;

typedef struct
{
    ClientColor color;
} ChangeColorMessage;

// Client state, represents a client over the network
typedef struct
{
    uint32_t client_id;
    int x;
    int y;
    float val;
    ClientColor color;
} ClientState;

typedef struct
{
    unsigned int client_count;
    ClientState client_states[MAX_CLIENTS];
} GameStateMessage;

// Store all options from the command line
typedef struct
{
    float packet_loss;
    float packet_duplication;
    float ping;
    float jitter;
} Options;

ChangeColorMessage *ChangeColorMessage_Create(void);
void ChangeColorMessage_Destroy(ChangeColorMessage *);
int ChangeColorMessage_Serialize(ChangeColorMessage *msg, NBN_Stream *);

UpdateStateMessage *UpdateStateMessage_Create(void);
void UpdateStateMessage_Destroy(UpdateStateMessage *);
int UpdateStateMessage_Serialize(UpdateStateMessage *, NBN_Stream *);

GameStateMessage *GameStateMessage_Create(void);
void GameStateMessage_Destroy(GameStateMessage *);
int GameStateMessage_Serialize(GameStateMessage *, NBN_Stream *);

int ReadCommandLine(int, char *[]);
Options GetOptions(void);


#endif SHARED_HPP