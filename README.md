# UE5 Online Project

This is a personal project made for learning purpose.

Barebone Unreal engine projet that only contain online features implementations required for a basic online game.

It is developed in C++ for core systems and on Blueprint for implementations that should be accessible by designers and artists.

**This project is a work in progress and is not suitable for production**

### Dependency
This project depend on my own [backend server](https://github.com/wpatruno/ASP.NET-Game-Backend) made on ASP.NET HTTPS and SignalR.

Connection are made with HTTPS request (GET, POST etc...) and SignalR (Websocket) when needed.


Plugins used:
- [SignalR](https://github.com/wpatruno/Unreal-SignalR) : Fork of FrozenStormInteractive/Unreal-SignalR with minor changes for my own use
- [UE5-HTTPBackend](https://github.com/wpatruno/UE5-HTTPBackend) : Plugin I made to easly use HTTP request in C++ and BP

To use the project demo, in ContentBrowser do Add>FeatureOrContentPack
Add StarterContent and ThirdPersonCharacter

### Features I am working on

## Main
- Connect to an account
- Access user data (Level, money, inventory)
- Show public servers
- Join Offline game
- Join Online server by IP
- Join Online server with matchmaking
- Microtransaction Shop

## Later
- Add and show friends
- Message friends
- Group friends for matchmaking
