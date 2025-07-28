# Vault Codes Helper

A Geometry Dash mod that adds a helpful "CODES" button to the Vault (SecretLayer5) allowing players to view all vault codes with a convenient checkbox tracking system.

## Features

- 🔘 **Interactive Checkbox System**: Each code has a checkbox that you can click to mark as used
- 💾 **Persistent Storage**: Your checkbox states are automatically saved and restored when you restart the game
- 📋 **Complete Code List**: Includes all major Geometry Dash vault codes with descriptions
- 🎯 **Easy Access**: Conveniently located button right in the vault interface
- ⚡ **Bulk Actions**: "Check All" and "Clear All" buttons for easy management
- � **Paginated Interface**: Clean pagination showing 5 codes per page for easy navigation

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
4. Navigate through pages using the Previous/Next arrow buttons (5 codes per page)
5. Check the page indicator at the bottom to see which page you're on
6. Click the checkbox next to any code you've already entered to mark it as used
7. Use "Check All" to mark all codes as used, or "Clear All" to reset all checkboxes
8. Your checkbox states will be automatically saved and restored next time you open the game

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

# Resources
* [Geode SDK Documentation](https://docs.geode-sdk.org/)
* [Geode SDK Source Code](https://github.com/geode-sdk/geode/)
* [Geode CLI](https://github.com/geode-sdk/cli)
* [Bindings](https://github.com/geode-sdk/bindings/)
* [Dev Tools](https://github.com/geode-sdk/DevTools)
