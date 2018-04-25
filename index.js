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

  XinputAddon.watch(function (data) {
    if( mainWindow !== null) {
      mainWindow.webContents.send('info', { msg: 'pen ' + data });
    }
  }, "13");


}

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