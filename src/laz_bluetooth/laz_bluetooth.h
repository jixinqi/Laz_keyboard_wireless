#ifndef _LAZ_BLUETOOTH_H_
#define _LAZ_BLUETOOTH_H_

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/hci.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/gatt.h>

int laz_bluetooth_init();
void on_laz_bluetooth_connected(struct bt_conn *conn, uint8_t ec);
void on_laz_bluetooth_disconnected(struct bt_conn *conn, uint8_t ec);
 
#endif // _LAZ_BLUETOOTH_H_
