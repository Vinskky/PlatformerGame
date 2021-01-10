# Undefault Platformer

Platformer game developed during the course 2020-21 for the subject of development with the aim to achive 2D programming skills. 

## Key Features

 - Xml config file loading for different modules.
 - Load/Save for player and enemies state using XML file.
 - Tiled TMX map loading and drawing (orthographic)
 - Map collisions detection (no walkable tiles, starting tile for each level, die tiles, end level tiles)
 - Map navigation: player movement and jumping
 - Intro logo, Title screen with fade-in fade-out
 - Title screen (press Enter to start)
 - Death screen (press Enter to try again)
 - 2 levels
 - Background with parallax on both levels
 - Collectible coin system
 - Life system for player
 - Life refillers
 - Checkpoints and teleport to them
 - Ground Enemy with pathfinding A*
 - Destroyable enemies with attack
 - Frame Capped 60 or 30 fps
 - Invincibility frames when player hurt
 - Implemented GUI (NEW!)
     - Buttons
     - Sliders
     - Check Boxs
 - Main menu scene (NEW!)
     - Play button
     - Continue button
     - Settings button
     - Credits button
     - Exit button
 - Settings scene (NEW!)
     - Volume & Fx slider
     - vSync & Full Screen Mode
 - Ingame Pause menu scene (NEW!)
     - Resume button
     - Settings button
     - Main menu button
     - Exit button
 - Ingame death timer (NEW!)
 - Updated Fx (NEW!)
 - Implemented fonts using TTF (NEW!)
 - Saved game detection (NEW!)
     - WARNING: There's no !save_game.xml! file created. It is generated automatically when saved 1rst time. If you want to delete your progress, delete
       "save_game.xml" file in the /Output folder. The "DISABLED" state of "Continue" button (in Main Menu) depends on the existance of this file.
 
## Controls

 - A: Moves left
 - D: Moves Right
 - Spacebar: Jump
 - M: Sword attack
 - ESC: Pause menu ingame (NEW!)
 
 - Aditional controls in GOD MODE:
 - W: Moves Up (with height limitations)
 - S: Moves Down (with height limitations)
 
 ## Debug keys
 
 - 1: Teleport player to checkpoint on lvl 1
 - 2: Teleport player to checkpoint on lvl 2
 - P: Hurts player (-1 life)
 - F1: Start from the first level
 - F2: Start from the second level
 - F3: Start from the beggining of the current level
 - F5: Save current state of the game
 - F6: Load the previous state (even across levels)
 - F8: View GUI colliders / logic (NEW!)
 - F9: View colliders / logic
 - F10: God Mode
 - F11: Capped 60/30 fps

## Developers (Undefault Boys)

 - Sebastián Delgado - [Github](https://github.com/Vinskky)
 - Martí Buxeda (Raxer Sound) - [Github](https://github.com/BooStarGamer)

## License

This project is licensed under an unmodified MIT license, which is an OSI-certified license that allows static linking with closed source software. Check [LICENSE](LICENSE) for further details.

- [Steampunk PNG's details](https://github.com/Vinskky/PlatformerGame/blob/master/Output/Assets/maps/licence%20Steampunk%20png's)
- [Parallax PNG's details](https://github.com/Vinskky/PlatformerGame/blob/master/Output/Assets/maps/licence%20skill-desc%20png's)
- [Character PNG details](https://github.com/Vinskky/PlatformerGame/blob/master/Output/Assets/textures/Licence%20Character.png)
- [Jumping wav details](https://github.com/Vinskky/PlatformerGame/blob/master/Output/Assets/audio/fx/jump.wav%20licence)
- [Music and FX except Jump by Raxer Sound](https://lnkfi.re/EhBCPZ4v)
- [Bat enemy](https://elthen.itch.io/bat-sprite-pack)
- [Main Menu Background](https://www.deviantart.com/mockingjay1701/art/Pixel-art-landscape-525082296)
- [GUI & TTF font](https://www.kenney.nl/assets/ui-pack)
