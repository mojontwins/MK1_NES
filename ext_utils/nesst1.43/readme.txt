NES Screen Tool by Shiru



This Windows tool allows to edit NES nametables, attributes, palettes, and pattern graphics. The program and it's source code are released into the Public Domain, you can do whatever you want with it. The source code is for Borland Turbo C++ Explorer (C++ Builder).

BMP import supports 256x240 images with 16 or 256 colors only, with 256 or less unique tiles. It removes duplicated tiles and creates nametable. Only two lower bits of the graphics are used.

You can save and load RLE-packed nametables (with or without attributes), decompressor is provided as well.

You can place nes.pal file (192 bytes) in the program directory, if you want to tweak the colors of the NES palette.

You can load all the supported files(tiles, nametable, palette) of the same name at once using Open menu item.

You can associate the program with *.chr files, and open them by double click.

You can load and save 1K, 2K, 4K, 8K, or other size of multiple of 16, CHR files. 4K files always replace current tileset, smaller files loads and saves from the current tile position. Only half of 8K file is visible at a time, you can switch between the halfs using A B buttons under the tile set.

You have one step of undo for destructive operations, use Ctrl+Z.

You can select an area in the tileset before using Optimize in order to exclude the area from the optimization process. This way you can optimize the tileset without changing the selected area and keeping tiles in the area in place.

You can interleave and deinterleave tiles in the tileset to make work on 8x16 sprites simpler. Draw them as vertical-oriented, then use Interleave function of Patterns menu to rearrange the tiles in order (top become left, bottom become right). Use Deinterleave function if you need to rearrange the tiles back. Be careful, this function changes actual arrangement of the data, not just how it displayed.


Nametable controls:

Left click - set current tile into nametable, right click - set the tile from the nametable as current. You can copy/paste tiles graphics in the tileset using Ctrl+C, Ctrl+V, Ctrl+X. You can copy/paste tiles in the nametable. Hold Shift or press Caps Lock to allow area selection, left mouse button to define the selection while holding Shift, right mouse button to remove the selection. When there is selection in the nametable, you can use Ctrl+C, Ctrl+V, Ctrl+X, Ctrl+F (to fill the selection with current tile), and Shift+C (copy selection into the clipboard as text). Note that Apply tile/Apply palette buttons also applied to the nametable copy/paste functions. You also can rotate the nametable without the attributes using the cursor keys, or with the attributes (per 2 tile) using the cursor keys with Ctrl.

You can check attributes with a checker tile, using 'Attr checker' button or 'A' key. Warning, you still can draw tiles in this mode.


Tileset controls:

Left click - set the tile as current. Double click to open the tile in the CHR editor. You can select and copy/paste tile groups similar to nametable. You also can select non-rectangular groups of tiles using Ctrl+click (left click selects a tile, right click deselects). If you copy a non-rectangular group, it will be then pasted as linear sequence of tiles. You can save selected tiles into a separate file.


Palette controls:

Left click - select current color and palette, or assign a color to current color in current palette.


CHR Editor controls:

Left click - set pixel, right click - set current color to the color of the pixel, Ctrl+left click - fill an area with current color. Cursor keys scrolls the tile, H and V keys mirrors the tile.



History:

v1.43 04.12.11 - Number of selected tiles now displayed in the info box
v1.42 10.10.11 - Attributes table could be saved in a separate file
v1.41 02.08.11 - Filenames of files opened through Open or double click are put into save dialogs
v1.40 31.07.11 - Correct work with files that contain dots in path
v1.39 30.07.11 - Fixed wrong file extension and CHR size that was suggested sometimes
v1.38 27.07.11 - Put block as a metasprite into clipboard function
v1.37 01.07.11 - Attribute checker
v1.36 26.06.11 - Save and copy nametable to clipboard as C code
v1.35 04.06.11 - Tileset and nametable export as bitmap files
v1.34 16.05.11 - CHR import from NES files, supports 24K and 40K ROMs
v1.32 10.05.11 - Autofix for unsafe color $0d in palettes, color emphasis display
v1.31 18.01.11 - Minor fixes and some small features
v1.3  03.01.11 - Bugfixes, some new features
v1.23 11.12.10 - CHR redundancy optimization
v1.22 10.12.10 - Copy/paste tile groups in tileset
v1.21 09.12.10 - One-step undo
v1.2  08.12.10 - 1K, 2K, and 8K CHR files support, additional features for CHR editor
v1.12 07.12.10 - Copying parts of the nametable in the clipboard as text
v1.11 06.12.10 - Detailed information bar
v1.1  05.12.10 - Minor fixes and improvements, open all, nametable copy/paste functions
v1.0  04.12.10 - Initial release



mailto: shiru@mail.ru
http:// shiru.untergrund.net