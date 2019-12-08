#define I_VERSION_STRING    "FuckMeHarder"
#define I_VERSION_MAJOR     "0"
#define I_VERSION_MINOR     "0"
#define I_VERSION_BUILD     "2"

//
// which game version to target - only uncomment one!
//

#define GAME_FNAF1
//#define GAME_FNAF2
//#define GAME_FNAF3

//
// game specific version info
//

//FNAF 1
#ifdef GAME_FNAF1
  #define I_SUPPORT_VERSION   "0.01"
  #define G_NUM_ROOMS         12
  #define G_NUM_ANIMAS        5
#endif

//FNAF 2
#ifdef GAME_FNAF2
  #define I_SUPPORT_VERSION  "0.00"
#endif

//FNAF 3
#ifdef GAME_FNAF3
  #define I_SUPPORT_VERSION  "0.00"
#endif