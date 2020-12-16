#ifndef _OSL_KEYS_H_
#define _OSL_KEYS_H_

#ifdef __cplusplus
extern "C" {
#endif


/** @defgroup keys Controller

	Controller functions in OSLib.
	@{
*/

/** List of keys. */
typedef union		{
	struct		{
		int select:1;					//!< Select
		int reserved1:2;				//!< For padding, do not use
		int start:1;					//!< Start
		int up:1;						//!< Up (d-pad)
		int right:1;					//!< Right (d-pad)
		int down:1;						//!< Down (d-pad)
		int left:1;						//!< Left (d-pad)
		int L:1;						//!< L (shoulder)
		int R:1;						//!< R (shoulder)
		int reserved2:2;				//!< For padding, do not use
		int triangle:1;					//!< Triangle
		int circle:1;					//!< Circle
		int cross:1;					//!< Cross
		int square:1;					//!< Square
		int home:1;						//!< Home (seems not to work)
		int hold:1;						//!< Hold (power switch in the opposite direction)
		int reserved3:5;				//!< For padding, do not use
		int note:1;						//!< Note (seems not to work)
	};
	unsigned int value;					//!< 32-bit value containing all keys
} OSL_KEYLIST;




/** Controller type. */
typedef struct		{
	OSL_KEYLIST held;						//!< Keys currently down (held)
	OSL_KEYLIST pressed;					//!< Keys pressed (only reported once when the user pressed it)
	OSL_KEYLIST released;					//!< Keys released (only reported once when the user releases it)
	OSL_KEYLIST lastHeld;					//!< Allows you to trick with the held member without messing up the auto-repeat feature

	short autoRepeatInit;					//!< Time for the initialization of the autorepeat feature
	short autoRepeatInterval;				//!< Interval before the autorepeat feature is switched on (the time the user must let the key down)
	int autoRepeatMask;						//!< Keys affected by the autorepeat feature
	short autoRepeatCounter;				//!< Counter (internal)

	signed char analogToDPadSensivity;		//!< Minimum sensivity for the analog to d-pad function in each direct, 0 means disable analog to d-pad. 127 is the maximum: the stick has to be completely pressed to be detected. A typical value is 80.

	signed char analogX;					//!< Horizontal position of the analog stick (-128: left, +127: right)
	signed char analogY;					//!< Vertical position of the analog stick (-128: top, +127: bottom)

    int holdAffectsAnalog;
} OSL_CONTROLLER;


/** Bit number for each key in the 'value' field. One of these values is returned by oslKbhit or oslWaitKey.

\code
keyPressed = oslWaitKey();
if (keyPressed == OSL_KEY_START)
	...
\endcode */
enum OSL_KEY_BITS		{
	OSL_KEY_SELECT=1,						//!< Select key
	OSL_KEY_START=4,						//!< Start key
	OSL_KEY_UP=5,							//!< Up d-pad key
	OSL_KEY_RIGHT=6,						//!< Right d-pad key
	OSL_KEY_DOWN=7,							//!< Down d-pad key
	OSL_KEY_LEFT=8,							//!< Left d-pad key
	OSL_KEY_L=9,							//!< L (shoulder) key
	OSL_KEY_R=10,							//!< R (shoulder) key
	OSL_KEY_TRIANGLE=13,					//!< Triangle key
	OSL_KEY_CIRCLE=14,						//!< Circle key
	OSL_KEY_CROSS=15,						//!< Cross key
	OSL_KEY_SQUARE=16,						//!< Square key
	OSL_KEY_HOME=17,						//!< Home key (does not work in normal (user) operation)
	OSL_KEY_HOLD=18,						//!< Hold (power switch in the opposite direction) key
	OSL_KEY_NOTE=24							//!< Note key (same remark as Home)
};

/** Mask for each key in the 'value' field. To extract for example the select key, you can do the following:
\code
oslReadKeys();
if (osl_pad.held.value & OSL_KEYMASK_SELECT)	...
//This is equivalent to:
if (osl_pad.held.select)	...
\endcode

The keymasks description is exactly the same as keybits, they are just named OSL_KEYMASK_xxx instead of OSL_KEY_xxx. */
enum OSL_KEY_MASKS			{
	OSL_KEYMASK_SELECT=0x1,
	OSL_KEYMASK_START=0x8,
	OSL_KEYMASK_UP=0x10,
	OSL_KEYMASK_RIGHT=0x20,
	OSL_KEYMASK_DOWN=0x40,
	OSL_KEYMASK_LEFT=0x80,
	OSL_KEYMASK_L=0x100,
	OSL_KEYMASK_R=0x200,
	OSL_KEYMASK_TRIANGLE=0x1000,
	OSL_KEYMASK_CIRCLE=0x2000,
	OSL_KEYMASK_CROSS=0x4000,
	OSL_KEYMASK_SQUARE=0x8000,
	OSL_KEYMASK_HOME=0x10000,
	OSL_KEYMASK_HOLD=0x20000,
	OSL_KEYMASK_NOTE=0x800000
};


/** Sets the auto-repeat parameters (all at once).
	\param keys
		Sets the keys affected by the autorepeat feature. If these keys are held for long enough, they will be reported more times in the pressed member in the osl_pad structure. It's the ideal thing to
		handle a menu, just check for the 'pressed' member and move the cursor when it's set. The auto-repeat feature will automatically work. Typical keys the user would expect to be auto-repeated are:
		OSL_KEYMASK_UP|OSL_KEYMASK_RIGHT|OSL_KEYMASK_DOWN|OSL_KEYMASK_LEFT|OSL_KEYMASK_R|OSL_KEYMASK_L. As you see, just OR the masks of the keys you want to be auto-repeated.
	\param init
		Time (in number of calls, so at 60 fps, 60 = 1 second) before the auto-repeat feature turns on. Usually you must wait a while (1/2 second or more) before the auto-repeat is turned on, but once
		it's on, the key will be repeated more than each 1/2 second. A typical value here is 40 (2/3 second).
	\param interval
		Time interval between each key repeat when the auto-repeat has been turned on. A typical value is 10 (1/6 of a second); with the example above, you would press the key, then 2/3 second later it
		begins to auto-repeat the key each 1/6 second.
*/
#define oslSetKeyAutorepeat(keys,init,interval)		( osl_keys->autoRepeatMask = keys, osl_keys->autoRepeatInit = init, osl_keys->autoRepeatInterval = interval )
/** Separate routine setting the key auto-repeat mask. */
#define oslSetKeyAutorepeatMask(mask)		(osl_keys->autoRepeatMask=mask)
/** Separate routine setting the key auto-repeat initialization value. */
#define oslSetKeyAutorepeatInit(value)		(osl_keys->autoRepeatInit=value)
/** Separate routine setting the key auto-repeat interval value. */
#define oslSetKeyAutorepeatInterval(value)	(osl_keys->autoRepeatInterval=value)

/** Enables or disable automatic redirection from the analog stick to D-Pad buttons (down, left, etc.).
	\param sensivity
		Sensivity for the analog press. Analog values go from -128 to 127, so the maximum sensivity value you can set is 127,
		meaning the stick needs to be at least at -127 or +127 in each direction to be treated as a directional key.

		A typical value is 80, this leaves a sufficient space to avoid unintentioned diagonals, but is also small enough to be sensitive and
		comfortable.

\b Note: Diagonal keys are supported as well with the stick, and this is really a good idea to at least enable this if you don't want to threat
the stick separately. Some people (including me) have a weak D-Pad and it's extremely hard to play any game or app that needs the up/down
keys, or even worse: diagonals.

\code
//Enable default analog handler
oslSetKeyAnalogToDPad(80);
//Read keys
oslReadKeys();
//The stick is upwards OR the D-pad's up direction is held
if (osl_pad.held.up)
	{}

//If you want to differenciate stick and D-pad, you can also do something like this:

//Make sure default analog handler is disabled!
oslSetKeyAnalogToDPad(0);
oslReadKeys();
//D-Pad
if (osl_pad.held.up)
	{}
//Stick
if (osl_pad.analogY < -80)
	{}
\endcode */
#define oslSetKeyAnalogToDPad(sensivity)	(osl_keys->analogToDPadSensivity = sensivity)

/** Current keys. Only here for compatibility, use osl_pad now. */
extern OSL_CONTROLLER *osl_keys;
/** Current keys. In this structure you will find three members: pressed, released and held.
- pressed: keys that were just pressed, reported once when the user pressed it but not after.
- held: always reported while the user holds the key.
- released: reported when the user just releases a pressed key.

Each of these members is an union, containing a structure of type OSL_KEYLIST (contains a member for each key):
\code
oslReadKeys();
if (osl_pad.pressed.start)
	oslDebug("A pause should be thrown!");
if (osl_pad.held.up)
	oslDebug("Moving the main character upwards!");

\endcode

Look at OSL_CONTROLLER for more information. */
extern OSL_CONTROLLER osl_pad;


/** Reads the current controller state and stores the result in the osl_pad structure. Returns a pointer to the actual key structure. It may seem cleaner to store this pointer and access data from it,
but you can do however you want. */
extern OSL_CONTROLLER *oslReadKeys();

/** Sets an external function to read keys.
You can pass to it a function from a kernel prx, so you'll be able to read all buttons (VOLUME_UP, VOLUME_DOWN, NOTE...)
*/
extern int oslSetReadKeysFunction(int (*sceCtrlReadBufferPositive)(SceCtrlData *pad_data, int count));

/** Unsets the function set with oslSetReadKeysFunction
*/
extern int oslUnsetReadKeysFunction();

/** Decide if HOLD will affect also the analog (default=NO).
*/
extern int oslSetHoldForAnalog(int holdForAnalog);

/** Waits for a key and returns its code. Compare the code against one of the OSL_KEYBITS values. */
extern int oslWaitKey();
/** Determines whether a key is currently buffered and returns its code, or 0 else. Compare the code against one of the OSL_KEYBITS values. */
extern int oslKbhit();
/** Flushes the key buffer, removing the pending key (retrievable with oslKbhit). */
extern void oslFlushKey();





//Remote functions:
/** List of remote keys. */
typedef union		{
	struct		{
		int rmplaypause:1;				//!< Play/Pause
		int reserved1:1;				//!< For padding, do not use
		int rmforward:1;				//!< Formward
		int rmback:1;					//!< Back
		int rmvolup:1;					//!< Volume Up
		int rmvoldown:1;				//!< Volume Down
		int rmhold:1;					//!< Hold
		int reserved2:1;				//!< For padding, do not use
	};
	u32 value;							//!< 32-bit value containing all keys
} OSL_REMOTEKEYLIST;


/** Remote Controller type. */
typedef struct		{
	OSL_REMOTEKEYLIST held;						//!< Keys currently down (held)
	OSL_REMOTEKEYLIST pressed;					//!< Keys pressed (only reported once when the user pressed it)
	OSL_REMOTEKEYLIST released;					//!< Keys released (only reported once when the user releases it)
	OSL_REMOTEKEYLIST lastHeld;					//!< Allows you to trick with the held member without messing up the auto-repeat feature

	short autoRepeatInit;					//!< Time for the initialization of the autorepeat feature
	short autoRepeatInterval;				//!< Interval before the autorepeat feature is switched on (the time the user must let the key down)
	int autoRepeatMask;						//!< Keys affected by the autorepeat feature
	short autoRepeatCounter;				//!< Counter (internal)
} OSL_REMOTECONTROLLER;

extern OSL_REMOTECONTROLLER *osl_remotekeys;

/** Current remote keys. In this structure you will find three members: pressed, released and held.
- pressed: keys that were just pressed, reported once when the user pressed it but not after.
- held: always reported while the user holds the key.
- released: reported when the user just releases a pressed key.

Each of these members is an union, containing a structure of type OSL_REMOTEKEYLIST (contains a member for each key):
\code
oslReadRemoteKeys();
if (osl_remote.pressed.rmplaypause)
	oslDebug("A pause should be thrown!");
if (osl_remote.held.rmforward)
	oslDebug("Do something!");

\endcode

Look at OSL_REMOTECONTROLLER for more information. */
extern OSL_REMOTECONTROLLER osl_remote;

 /** Reads the current remote controller state and stores the result in the osl_remote structure. Returns a pointer to the actual key structure. It may seem cleaner to store this pointer and access data from it,
but you can do however you want. */
extern OSL_REMOTECONTROLLER *oslReadRemoteKeys();

/** Flushes the remote key buffer, removing the pending key. */
extern void oslFlushRemoteKey();

/** Sets the auto-repeat parameters (all at once).
	\param keys
		Sets the keys affected by the autorepeat feature. If these keys are held for long enough, they will be reported more times in the pressed member in the osl_pad structure. It's the ideal thing to
		handle a menu, just check for the 'pressed' member and move the cursor when it's set. The auto-repeat feature will automatically work. Typical keys the user would expect to be auto-repeated are:
		OSL_KEYMASK_UP|OSL_KEYMASK_RIGHT|OSL_KEYMASK_DOWN|OSL_KEYMASK_LEFT|OSL_KEYMASK_R|OSL_KEYMASK_L. As you see, just OR the masks of the keys you want to be auto-repeated.
	\param init
		Time (in number of calls, so at 60 fps, 60 = 1 second) before the auto-repeat feature turns on. Usually you must wait a while (1/2 second or more) before the auto-repeat is turned on, but once
		it's on, the key will be repeated more than each 1/2 second. A typical value here is 40 (2/3 second).
	\param interval
		Time interval between each key repeat when the auto-repeat has been turned on. A typical value is 10 (1/6 of a second); with the example above, you would press the key, then 2/3 second later it
		begins to auto-repeat the key each 1/6 second.
*/
#define oslSetRemoteKeyAutorepeat(keys,init,interval)		( osl_remotekeys->autoRepeatMask = keys, osl_remotekeys->autoRepeatInit = init, osl_remotekeys->autoRepeatInterval = interval )
/** Separate routine setting the key auto-repeat mask. */
#define oslSetRemoteKeyAutorepeatMask(mask)		(osl_remotekeys->autoRepeatMask=mask)
/** Separate routine setting the key auto-repeat initialization value. */
#define oslSetRemoteKeyAutorepeatInit(value)		(osl_remotekeys->autoRepeatInit=value)
/** Separate routine setting the key auto-repeat interval value. */
#define oslSetRemoteKeyAutorepeatInterval(value)	(osl_remotekeys->autoRepeatInterval=value)

/**
Determines whether the remote is plugged in.
@return 1 if the remote is plugged in, else 0.
*/
int oslIsRemoteExist();

/** @} */ // end of keys


#ifdef __cplusplus
}
#endif

#endif
