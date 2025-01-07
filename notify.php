<?php
// Telegram Bot Token and Chat ID
$botToken = "7578210734:AAFiWsAxaN0uEhRRoWEfdfc_cZAvVm5CZc4"; // Replace with your Telegram Bot token
$chatID = "7469375202";     // Replace with your chat ID

// Visitor Information
$visitorIP = $_SERVER['REMOTE_ADDR'];
$visitTime = date('Y-m-d H:i:s');
$userAgent = $_SERVER['HTTP_USER_AGENT'];
$referrer = isset($_SERVER['HTTP_REFERER']) ? $_SERVER['HTTP_REFERER'] : 'Direct Access';

// Message to Send
$message = "🚀 *New Visitor on Website*\n";
$message .= "🌐 *IP Address:* `$visitorIP`\n";
$message .= "🕒 *Visit Time:* `$visitTime`\n";
$message .= "📱 *User Agent:* `$userAgent`\n";
$message .= "🔗 *Referrer:* `$referrer`\n";

// Telegram API URL
$apiURL = "https://api.telegram.org/bot$botToken/sendMessage";

// Send Message
$data = [
    'chat_id' => $chatID,
    'text' => $message,
    'parse_mode' => 'Markdown',
];

$options = [
    'http' => [
        'header'  => "Content-Type: application/x-www-form-urlencoded\r\n",
        'method'  => 'POST',
        'content' => http_build_query($data),
    ],
];

$context = stream_context_create($options);
$result = file_get_contents($apiURL, false, $context);

if ($result) {
    echo "Notification sent successfully.";
} else {
    echo "Failed to send notification.";
}
?>