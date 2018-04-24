const electron = require('electron');
const app = electron.app;
const BrowserWindow = electron.BrowserWindow;

const { ipcRenderer, webContents } = require('electron');
const XinputAddon = require('./build/Release/XinputAddon');

let mainWindow;

function createWindow() {

  mainWindow = new BrowserWindow({ width: 300, height: 200 });

  mainWindow.loadURL(`file://${__dirname}/index.html`);

  mainWindow.on('closed', () => {
    mainWindow = null;
  })

  mainWindow.on('load', () => {
    ipcRenderer.send('change-text-element', "ping")
    console.log('load')
  });

  XinputAddon.watch(function (data) {
    if( mainWindow !== null) {
      mainWindow.webContents.send('info', { msg: 'pen ' + data });
    }
  }, "16");


}

const { ipcMain } = require('electron')
ipcMain.on('asynchronous-message', (event, arg) => {
  console.log(arg)  // prints "ping"
  event.sender.send('asynchronous-reply', 'pong')
})

app.on('ready', createWindow);

app.on('window-all-closed', () => {
  if (process.platform !== 'darwin') {
    app.quit();
  }
});

app.on('activate', () => {
  if (mainWindow === null) {
    createWindow();
  }
});