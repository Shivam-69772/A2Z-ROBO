const express = require('express');
const fetch = require('node-fetch');
const app = express();
const PORT = 3000;

const TELEGRAM_BOT_TOKEN = 'YOUR_TELEGRAM_BOT_TOKEN';
const CHAT_ID = 'YOUR_CHAT_ID';

app.use(express.json());

app.post('/notify', async (req, res) => {
  const { ip, userAgent, url } = req.body;

  const message = `
    🔔 New Visitor Alert:
    - IP: ${ip.ip}
    - User Agent: ${userAgent}
    - URL: ${url}
  `;

  const telegramUrl = `https://api.telegram.org/bot${TELEGRAM_BOT_TOKEN}/sendMessage`;

  try {
    await fetch(telegramUrl, {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify({
        chat_id: CHAT_ID,
        text: message
      })
    });

    res.status(200).send('Notification Sent');
  } catch (error) {
    console.error('Error sending notification:', error);
    res.status(500).send('Error Sending Notification');
  }
});

app.listen(PORT, () => console.log(`Server running on port ${PORT}`));
