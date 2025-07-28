# Vault Codes Helper

# Vault Codes Helper

A Geometry Dash mod that adds a helpful "CODES" button to the Vault (SecretLayer5) allowing players to view all vault codes with a convenient checkbox tracking system.

## Features

- 🔘 **Interactive Checkbox System**: Each code has a checkbox that you can click to mark as used
- 💾 **Persistent Storage**: Your checkbox states are automatically saved and restored when you restart the game
- 📋 **Complete Code List**: Includes all major Geometry Dash vault codes with descriptions
- 🎯 **Easy Access**: Conveniently located button right in the vault interface
- ⚡ **Bulk Actions**: "Check All" and "Clear All" buttons for easy management
- 📜 **Scrollable Interface**: Clean, scrollable popup that fits all codes comfortably

## Code List Included

The mod includes the following vault codes with their descriptions:

- **GEOMETRY** - Basic unlock
- **DASH** - Color unlock  
- **WRAITH** - Secret reward
- **HELPER** - Bonus items
- **SECRET** - Hidden content
- **VAULT** - Extra vault access
- **TOWER** - Special tower
- **MYSTERY** - Unknown reward
- **POWER** - Boost ability
- **LEGEND** - Legendary unlock
- **SPOOKY** - Halloween content
- **LENNY** - Special icon
- **BLOCKBITE** - Reward
- **NEVERENDING** - Icon
- **AHEAD** - More content
- **CHALLENGE** - Special mode

## How to Use

1. Open Geometry Dash and navigate to the Vault
2. Look for the new "CODES" button in the vault interface
3. Click the button to open the Vault Codes Tracker popup
4. Browse through the list of codes and their descriptions
5. Click the checkbox next to any code you've already entered to mark it as used
6. Use "Check All" to mark all codes as used, or "Clear All" to reset all checkboxes
7. Your checkbox states will be automatically saved and restored next time you open the game

## Technical Details

- Built using Geode SDK 4.6.3
- Compatible with Geometry Dash 2.2074
- Uses persistent data storage to save checkbox states
- Custom popup interface with scroll support
- Hooks into SecretLayer5 (the main vault)

## Installation

1. Install Geode mod loader
2. Download the `.geode` file from releases
3. Place it in your Geometry Dash mods folder
4. Launch the game and enjoy!

## Building from Source

Requirements:
- Geode CLI
- Visual Studio (Windows)
- Git

```bash
git clone https://github.com/dulakgg/Wraith-codes.git
cd wraith-help
geode build
```

## Contributing

Feel free to submit issues or pull requests if you'd like to:
- Add more codes to the list
- Improve the UI
- Fix bugs or add features

## License

This mod is open source. Feel free to use and modify as needed.

<img src="logo.png" width="150" alt="the mod's logo" />

## Features

- **Vault Integration**: Adds a "Codes" button directly in the Secret Vault (SecretLayer2) next to where Wraith appears
- **Main Menu Access**: Also adds a "Codes" button to the main menu for easy access
- **Comprehensive Code List**: Displays a curated list of working Geometry Dash vault codes including:

## Installation

1. Make sure you have Geode Mod Loader installed
2. Download the `.geode` file from releases
3. Place it in your `geode/mods` folder
4. Launch Geometry Dash

## Usage

1. Go to the Secret Vault in Geometry Dash
2. Look for the "Codes" button next to Wraith
3. Click it to see the list of vault codes
4. Alternatively, use the "Codes" button in the main menu

## Technical Details

- Built using Geode 4.6.3
- Compatible with Geometry Dash 2.2074
- Hooks into SecretLayer2 and MenuLayer
- Uses proper Geode modding practices

## Development

This mod demonstrates:
- Proper use of Geode's modify system
- UI element addition to existing game layers
- Safe memory management with Geode's patterns
- Cross-layer mod implementation

## Building

```bash
cmake --build build --config RelWithDebInfo
```

Requires Geode SDK to be properly installed and `GEODE_SDK` environment variable set.

## Getting started
For more info on Geode modding, check out [the getting started section on our docs](https://docs.geode-sdk.org/getting-started/).

## Build instructions
For more info, see [our docs](https://docs.geode-sdk.org/getting-started/create-mod#build)
```sh
# Assuming you have the Geode CLI set up already
geode build
```

# Resources
* [Geode SDK Documentation](https://docs.geode-sdk.org/)
* [Geode SDK Source Code](https://github.com/geode-sdk/geode/)
* [Geode CLI](https://github.com/geode-sdk/cli)
* [Bindings](https://github.com/geode-sdk/bindings/)
* [Dev Tools](https://github.com/geode-sdk/DevTools)
