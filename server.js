const express = require('express');
const open = require('open');
const path = require('path');
const addon = require('./build/Release/addon');

const app = express();
const port = 3000;

app.use(express.urlencoded({ extended: true }));

app.get('/', (req, res) => {
  res.sendFile(path.join(__dirname, 'index.html'));
});

app.post('/check', (req, res) => {
  const username = req.body.username;
  const privilege = addon.checkPrivilege(username);
  let message;

  if (privilege) {
    message = `Пользователь ${username} имеет привилегию ${privilege}`;
  } else {
    message = `Пользователя ${username} нет`;
  }

  const backButton = '<form action="/" method="get"><button type="submit">Назад</button></form>';
  res.send(`<div>${message}</div>${backButton}`);
});

app.listen(port, () => {
  console.log(`Сервер запущен на http://localhost:${port}`);
  open(`http://localhost:${port}`);
});