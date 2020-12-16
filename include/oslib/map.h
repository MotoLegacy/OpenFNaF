#ifndef _OSL_MAP_H_
#define _OSL_MAP_H_

#ifdef __cplusplus
extern "C" {
#endif



/** @defgroup maps Maps

	Map functions in OSLib.
	@{
*/

/** Map structure. */
typedef struct		{
	OSL_IMAGE *img;
	void *map;
	int scrollX, scrollY;
	int tileX, tileY;
	int drawSizeX, drawSizeY;
	int mapSizeX, mapSizeY;
	u8 format, flags;
	u8 addit1;
} OSL_MAP;

/** Available map formats. */
enum OSL_MAP_FORMATS {
	OSL_MF_U16=1,								//!< 16 bit format
	OSL_MF_U16_GBA=2,							//!< 16 bit, 10 bit for the tile number, 2 for the mirror, palette is unsupported - addit1 holds the number of tile bits (10 by default), the two next bits are for mirroring (11 = horizontal and 12 = vertical)
};

/** Internal map flags. */
typedef enum OSL_MAP_FLAGS {
	OSL_MF_SIMPLE=1,							//!< Defines if the map is "simple" (oslDrawMapSimple) - UNSUPPORTED YET, DO NOT USE IT!
	OSL_MF_TILE1_TRANSPARENT=2,					//!< Defines if the first tile is always transparent, no matter its pattern.
} OSL_MAP_FLAGS;

/** Creates a new map.
	\param img
		Image used as tileset. Remember the maximum size of images is 512x512, neither the width or the height can exceed 512. In a tileset image, tiles are placed from in lines from left to right
		and from top to bottom. Each tile has a specific size.
	\param map_data
		Binary data representing the map data in its raw format. This is a table of entries, where each entry is 16-bit (or something else depending on the map format) and represents a tile number to
		be displayed. Please note that tile entries, just like tilesets, are placed from left to right, and then from top to bottom. To retrieve a specific map entry (x = horizontal position,
		y = vertical position), the algorithm is the following: ((map_type*)map_data)[y * width + x].
	\param tileX, tileY
		Size of a tile in the tileset.
	\param mapSizeX, mapSizeY
		Width and height of the map. Maps are wrapped around (that is, when you're all to the right, the left is displayed again, same vertically).
	\param map_format
		One of the #OSL_MAP_FORMATS values.

	\return
		NULL in case of error, a pointer to a map object in case of success.
*/
extern OSL_MAP *oslCreateMap(OSL_IMAGE *img, void *map_data, int tileX, int tileY, int mapSizeX, int mapSizeY, int map_format);

/** Draws a map on the screen, using the map properties:
	- scrollX, scrollY: respectively horizontal and vertical scrolling values (in pixels).
	The map is always drawn on the whole screen.
*/
extern void oslDrawMap(OSL_MAP *m);

/** Same as oslDrawMap, only here for backward compatibility. Do not use! */
extern void oslDrawMapSimple(OSL_MAP *m);					//tileX, tileY, img->sizeX doivent être des puissances de deux!

/** Deletes a map. Call this after you've finished with a map. Please note that the map data you supplied to #oslCreateMap
was your own data, passed by reference, and thus it will not be freed! Example:
\code
unsigned short *mapData;
OSL_MAP *map;
//Data for a 128x64 map
mapData = malloc(128 * 64 * sizeof(*mapData));
//Create a map with this
map = oslCreateMap(anImage, mapData, 16, 16, 128, 64, OSL_MF_U16);
//Now delete this map
oslDeleteMap(map);
//Don't forget to free the map data too
free(mapData);
\endcode */
extern void oslDeleteMap(OSL_MAP *m);

/** @} */ // end of keys


#ifdef __cplusplus
}
#endif

#endif
