# SA-MP Noclip Include v0.2.4

> Based on original code by Southclaws. Refactored and modularized by xWendorion.

## Overview

This include provides a plug-and-play noclip system for SA-MP servers.  
Originally created by Southclaws, this version has been updated, cleaned, and converted into a modern include for easier use and maintenance.

## Features

- Smooth free-fly (noclip) movement
- Acceleration-based controls
- Toggle via command
- Easy integration
- Works with Open.MP and SA-MP 0.3.7

---

## Setup Instructions

### 1. Include the file

At the top of your `.pwn` file:

```pawn
#include <noclip>
```

### 2. Add the callbacks

Add the following to your `OnGameModeInit`, `OnPlayerConnect`, and `OnPlayerUpdate`:

```pawn
public OnGameModeInit()
{
    NoclipInit();
    return 1;
}

public OnPlayerConnect(playerid)
{
    NoclipConnect(playerid);
    return 1;
}

public OnPlayerUpdate(playerid)
{
    NoclipUpdate(playerid);
    return 1;
}
```

### 3. Example Command (Pawn.CMD)

```pawn
COMMAND:noclip(playerid)
{
    if (GetPVarInt(playerid, "FlyMode") == 1)
    {
        CancelFlyMode(playerid);
        SendClientMessage(playerid, -1, "[Noclip] Disabled.");
    }
    else
    {
        FlyMode(playerid);
        SendClientMessage(playerid, -1, "[Noclip] Enabled.");
    }
    return 1;
}
```

---

## Requirements

- SA-MP 0.3.7 or Open.MP
- Pawn.CMD (or another command processor)
- Player admin/auth system (for access control)

## Credits

- **Southclaws** – Original noclip codebase  
- **xWendorion** – Refactor, optimization, include version

---

## License
MIT
This include is open-source and free to use, modify, and redistribute.
