# Vault Codes Helper

A Geometry Dash mod built with Geode that adds convenience buttons to display useful vault codes.

<img src="logo.png" width="150" alt="the mod's logo" />

## Features

- **Vault Integration**: Adds a "Codes" button directly in the Secret Vault (SecretLayer2) next to where Wraith appears
- **Main Menu Access**: Also adds a "Codes" button to the main menu for easy access
- **Comprehensive Code List**: Displays a curated list of working Geometry Dash vault codes including:
  - GEOMETRY - Basic unlock
  - DASH - Color unlock  
  - WRAITH - Secret reward
  - HELPER - Bonus items
  - SECRET - Hidden content
  - VAULT - Extra vault access
  - TOWER - Special tower
  - MYSTERY - Unknown reward
  - POWER - Boost ability
  - LEGEND - Legendary unlock
  - SPOOKY - Halloween content
  - LENNY - Special icon
  - BLOCKBITE - Reward
  - NEVERENDING - Icon
  - AHEAD - More content
  - CHALLENGE - Special mode

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
