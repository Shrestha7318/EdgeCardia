#ifndef NET_CONFIG_H
#define NET_CONFIG_H

#define WIFI_SSID     "YOUR_WIFI_NAME"
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"

/*
 * This should be the machine running your Node dashboard server.
 * For local testing, use your PC's local IP on the same Wi-Fi.
 * Example: "192.168.1.23"
 */
#define DASHBOARD_HOST "192.168.1.23"
#define DASHBOARD_PORT 3000
#define DASHBOARD_PATH "/api/metrics"

#endif