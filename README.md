# NodePen
> Node.js xinput api implement for linux

Besides allowing raw access to the xinput device, it also provides a small wrapper class which emits events based on controller input.

## Requirements

[xinput](https://packages.debian.org/search?keywords=xinput) for linux. Please ensure you have the [required dependencies](https://github.com/nodejs/node-gyp#installation) as well installed.

## Building

node-gyp is required to build addon.

```
npm install

```

Then configure and build:

```
npm run make

```

For identify your device id folow this command :

```
xinput --list

```

Now, you can adapt and run demo with : 

```
npm start

```

## Usages

### Simple
```JavaScript

const XinputAddon = require('./build/Release/XinputAddon')

// get device id with xinput package
let deviceId = "16"

XinputAddon.watch(function (data) {
    console.log('pen ' + data )
}, deviceId);

```