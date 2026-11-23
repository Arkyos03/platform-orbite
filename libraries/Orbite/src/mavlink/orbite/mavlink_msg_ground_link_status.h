#pragma once
// MESSAGE GROUND_LINK_STATUS PACKING

#include <stdint.h>

#define MAVLINK_MSG_ID_GROUND_LINK_STATUS 51010


typedef struct __mavlink_ground_link_status_t {
 uint32_t time_boot_ms; /*< [ms] Temps depuis le démarrage de la station.*/
 uint32_t rx_packets; /*<  Paquets LoRa reçus avec CRC valide.*/
 uint32_t crc_errors; /*<  Paquets LoRa reçus avec CRC invalide.*/
 uint32_t frames_lost; /*<  Trames MAVLink perdues (trous dans la séquence de la fusée).*/
 uint32_t malformed; /*<  Paquets LoRa ne contenant pas de trame MAVLink valide.*/
 uint32_t last_rx_age_ms; /*< [ms] Temps depuis le dernier paquet valide (UINT32_MAX si jamais reçu).*/
 float rssi; /*< [dBm] RSSI du dernier paquet.*/
 float snr; /*< [dB] SNR du dernier paquet.*/
 float freq_error; /*< [Hz] Erreur de fréquence du dernier paquet.*/
 uint8_t link_quality; /*< [%] Trames reçues / attendues sur la fenêtre glissante.*/
 uint8_t rx_rate; /*<  Paquets valides reçus pendant la dernière seconde.*/
 uint8_t fem_type; /*<  Front-end RF détecté.*/
} mavlink_ground_link_status_t;

#define MAVLINK_MSG_ID_GROUND_LINK_STATUS_LEN 39
#define MAVLINK_MSG_ID_GROUND_LINK_STATUS_MIN_LEN 39
#define MAVLINK_MSG_ID_51010_LEN 39
#define MAVLINK_MSG_ID_51010_MIN_LEN 39

#define MAVLINK_MSG_ID_GROUND_LINK_STATUS_CRC 7
#define MAVLINK_MSG_ID_51010_CRC 7



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_GROUND_LINK_STATUS { \
    51010, \
    "GROUND_LINK_STATUS", \
    12, \
    {  { "time_boot_ms", NULL, MAVLINK_TYPE_UINT32_T, 0, 0, offsetof(mavlink_ground_link_status_t, time_boot_ms) }, \
         { "rx_packets", NULL, MAVLINK_TYPE_UINT32_T, 0, 4, offsetof(mavlink_ground_link_status_t, rx_packets) }, \
         { "crc_errors", NULL, MAVLINK_TYPE_UINT32_T, 0, 8, offsetof(mavlink_ground_link_status_t, crc_errors) }, \
         { "frames_lost", NULL, MAVLINK_TYPE_UINT32_T, 0, 12, offsetof(mavlink_ground_link_status_t, frames_lost) }, \
         { "malformed", NULL, MAVLINK_TYPE_UINT32_T, 0, 16, offsetof(mavlink_ground_link_status_t, malformed) }, \
         { "last_rx_age_ms", NULL, MAVLINK_TYPE_UINT32_T, 0, 20, offsetof(mavlink_ground_link_status_t, last_rx_age_ms) }, \
         { "rssi", NULL, MAVLINK_TYPE_FLOAT, 0, 24, offsetof(mavlink_ground_link_status_t, rssi) }, \
         { "snr", NULL, MAVLINK_TYPE_FLOAT, 0, 28, offsetof(mavlink_ground_link_status_t, snr) }, \
         { "freq_error", NULL, MAVLINK_TYPE_FLOAT, 0, 32, offsetof(mavlink_ground_link_status_t, freq_error) }, \
         { "link_quality", NULL, MAVLINK_TYPE_UINT8_T, 0, 36, offsetof(mavlink_ground_link_status_t, link_quality) }, \
         { "rx_rate", NULL, MAVLINK_TYPE_UINT8_T, 0, 37, offsetof(mavlink_ground_link_status_t, rx_rate) }, \
         { "fem_type", NULL, MAVLINK_TYPE_UINT8_T, 0, 38, offsetof(mavlink_ground_link_status_t, fem_type) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_GROUND_LINK_STATUS { \
    "GROUND_LINK_STATUS", \
    12, \
    {  { "time_boot_ms", NULL, MAVLINK_TYPE_UINT32_T, 0, 0, offsetof(mavlink_ground_link_status_t, time_boot_ms) }, \
         { "rx_packets", NULL, MAVLINK_TYPE_UINT32_T, 0, 4, offsetof(mavlink_ground_link_status_t, rx_packets) }, \
         { "crc_errors", NULL, MAVLINK_TYPE_UINT32_T, 0, 8, offsetof(mavlink_ground_link_status_t, crc_errors) }, \
         { "frames_lost", NULL, MAVLINK_TYPE_UINT32_T, 0, 12, offsetof(mavlink_ground_link_status_t, frames_lost) }, \
         { "malformed", NULL, MAVLINK_TYPE_UINT32_T, 0, 16, offsetof(mavlink_ground_link_status_t, malformed) }, \
         { "last_rx_age_ms", NULL, MAVLINK_TYPE_UINT32_T, 0, 20, offsetof(mavlink_ground_link_status_t, last_rx_age_ms) }, \
         { "rssi", NULL, MAVLINK_TYPE_FLOAT, 0, 24, offsetof(mavlink_ground_link_status_t, rssi) }, \
         { "snr", NULL, MAVLINK_TYPE_FLOAT, 0, 28, offsetof(mavlink_ground_link_status_t, snr) }, \
         { "freq_error", NULL, MAVLINK_TYPE_FLOAT, 0, 32, offsetof(mavlink_ground_link_status_t, freq_error) }, \
         { "link_quality", NULL, MAVLINK_TYPE_UINT8_T, 0, 36, offsetof(mavlink_ground_link_status_t, link_quality) }, \
         { "rx_rate", NULL, MAVLINK_TYPE_UINT8_T, 0, 37, offsetof(mavlink_ground_link_status_t, rx_rate) }, \
         { "fem_type", NULL, MAVLINK_TYPE_UINT8_T, 0, 38, offsetof(mavlink_ground_link_status_t, fem_type) }, \
         } \
}
#endif

/**
 * @brief Pack a ground_link_status message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param time_boot_ms [ms] Temps depuis le démarrage de la station.
 * @param rx_packets  Paquets LoRa reçus avec CRC valide.
 * @param crc_errors  Paquets LoRa reçus avec CRC invalide.
 * @param frames_lost  Trames MAVLink perdues (trous dans la séquence de la fusée).
 * @param malformed  Paquets LoRa ne contenant pas de trame MAVLink valide.
 * @param last_rx_age_ms [ms] Temps depuis le dernier paquet valide (UINT32_MAX si jamais reçu).
 * @param rssi [dBm] RSSI du dernier paquet.
 * @param snr [dB] SNR du dernier paquet.
 * @param freq_error [Hz] Erreur de fréquence du dernier paquet.
 * @param link_quality [%] Trames reçues / attendues sur la fenêtre glissante.
 * @param rx_rate  Paquets valides reçus pendant la dernière seconde.
 * @param fem_type  Front-end RF détecté.
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_ground_link_status_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint32_t time_boot_ms, uint32_t rx_packets, uint32_t crc_errors, uint32_t frames_lost, uint32_t malformed, uint32_t last_rx_age_ms, float rssi, float snr, float freq_error, uint8_t link_quality, uint8_t rx_rate, uint8_t fem_type)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_GROUND_LINK_STATUS_LEN];
    _mav_put_uint32_t(buf, 0, time_boot_ms);
    _mav_put_uint32_t(buf, 4, rx_packets);
    _mav_put_uint32_t(buf, 8, crc_errors);
    _mav_put_uint32_t(buf, 12, frames_lost);
    _mav_put_uint32_t(buf, 16, malformed);
    _mav_put_uint32_t(buf, 20, last_rx_age_ms);
    _mav_put_float(buf, 24, rssi);
    _mav_put_float(buf, 28, snr);
    _mav_put_float(buf, 32, freq_error);
    _mav_put_uint8_t(buf, 36, link_quality);
    _mav_put_uint8_t(buf, 37, rx_rate);
    _mav_put_uint8_t(buf, 38, fem_type);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_GROUND_LINK_STATUS_LEN);
#else
    mavlink_ground_link_status_t packet;
    packet.time_boot_ms = time_boot_ms;
    packet.rx_packets = rx_packets;
    packet.crc_errors = crc_errors;
    packet.frames_lost = frames_lost;
    packet.malformed = malformed;
    packet.last_rx_age_ms = last_rx_age_ms;
    packet.rssi = rssi;
    packet.snr = snr;
    packet.freq_error = freq_error;
    packet.link_quality = link_quality;
    packet.rx_rate = rx_rate;
    packet.fem_type = fem_type;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_GROUND_LINK_STATUS_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_GROUND_LINK_STATUS;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_GROUND_LINK_STATUS_MIN_LEN, MAVLINK_MSG_ID_GROUND_LINK_STATUS_LEN, MAVLINK_MSG_ID_GROUND_LINK_STATUS_CRC);
}

/**
 * @brief Pack a ground_link_status message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 *
 * @param time_boot_ms [ms] Temps depuis le démarrage de la station.
 * @param rx_packets  Paquets LoRa reçus avec CRC valide.
 * @param crc_errors  Paquets LoRa reçus avec CRC invalide.
 * @param frames_lost  Trames MAVLink perdues (trous dans la séquence de la fusée).
 * @param malformed  Paquets LoRa ne contenant pas de trame MAVLink valide.
 * @param last_rx_age_ms [ms] Temps depuis le dernier paquet valide (UINT32_MAX si jamais reçu).
 * @param rssi [dBm] RSSI du dernier paquet.
 * @param snr [dB] SNR du dernier paquet.
 * @param freq_error [Hz] Erreur de fréquence du dernier paquet.
 * @param link_quality [%] Trames reçues / attendues sur la fenêtre glissante.
 * @param rx_rate  Paquets valides reçus pendant la dernière seconde.
 * @param fem_type  Front-end RF détecté.
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_ground_link_status_pack_status(uint8_t system_id, uint8_t component_id, mavlink_status_t *_status, mavlink_message_t* msg,
                               uint32_t time_boot_ms, uint32_t rx_packets, uint32_t crc_errors, uint32_t frames_lost, uint32_t malformed, uint32_t last_rx_age_ms, float rssi, float snr, float freq_error, uint8_t link_quality, uint8_t rx_rate, uint8_t fem_type)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_GROUND_LINK_STATUS_LEN];
    _mav_put_uint32_t(buf, 0, time_boot_ms);
    _mav_put_uint32_t(buf, 4, rx_packets);
    _mav_put_uint32_t(buf, 8, crc_errors);
    _mav_put_uint32_t(buf, 12, frames_lost);
    _mav_put_uint32_t(buf, 16, malformed);
    _mav_put_uint32_t(buf, 20, last_rx_age_ms);
    _mav_put_float(buf, 24, rssi);
    _mav_put_float(buf, 28, snr);
    _mav_put_float(buf, 32, freq_error);
    _mav_put_uint8_t(buf, 36, link_quality);
    _mav_put_uint8_t(buf, 37, rx_rate);
    _mav_put_uint8_t(buf, 38, fem_type);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_GROUND_LINK_STATUS_LEN);
#else
    mavlink_ground_link_status_t packet;
    packet.time_boot_ms = time_boot_ms;
    packet.rx_packets = rx_packets;
    packet.crc_errors = crc_errors;
    packet.frames_lost = frames_lost;
    packet.malformed = malformed;
    packet.last_rx_age_ms = last_rx_age_ms;
    packet.rssi = rssi;
    packet.snr = snr;
    packet.freq_error = freq_error;
    packet.link_quality = link_quality;
    packet.rx_rate = rx_rate;
    packet.fem_type = fem_type;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_GROUND_LINK_STATUS_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_GROUND_LINK_STATUS;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_GROUND_LINK_STATUS_MIN_LEN, MAVLINK_MSG_ID_GROUND_LINK_STATUS_LEN, MAVLINK_MSG_ID_GROUND_LINK_STATUS_CRC);
#else
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_GROUND_LINK_STATUS_MIN_LEN, MAVLINK_MSG_ID_GROUND_LINK_STATUS_LEN);
#endif
}

/**
 * @brief Pack a ground_link_status message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param time_boot_ms [ms] Temps depuis le démarrage de la station.
 * @param rx_packets  Paquets LoRa reçus avec CRC valide.
 * @param crc_errors  Paquets LoRa reçus avec CRC invalide.
 * @param frames_lost  Trames MAVLink perdues (trous dans la séquence de la fusée).
 * @param malformed  Paquets LoRa ne contenant pas de trame MAVLink valide.
 * @param last_rx_age_ms [ms] Temps depuis le dernier paquet valide (UINT32_MAX si jamais reçu).
 * @param rssi [dBm] RSSI du dernier paquet.
 * @param snr [dB] SNR du dernier paquet.
 * @param freq_error [Hz] Erreur de fréquence du dernier paquet.
 * @param link_quality [%] Trames reçues / attendues sur la fenêtre glissante.
 * @param rx_rate  Paquets valides reçus pendant la dernière seconde.
 * @param fem_type  Front-end RF détecté.
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_ground_link_status_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint32_t time_boot_ms,uint32_t rx_packets,uint32_t crc_errors,uint32_t frames_lost,uint32_t malformed,uint32_t last_rx_age_ms,float rssi,float snr,float freq_error,uint8_t link_quality,uint8_t rx_rate,uint8_t fem_type)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_GROUND_LINK_STATUS_LEN];
    _mav_put_uint32_t(buf, 0, time_boot_ms);
    _mav_put_uint32_t(buf, 4, rx_packets);
    _mav_put_uint32_t(buf, 8, crc_errors);
    _mav_put_uint32_t(buf, 12, frames_lost);
    _mav_put_uint32_t(buf, 16, malformed);
    _mav_put_uint32_t(buf, 20, last_rx_age_ms);
    _mav_put_float(buf, 24, rssi);
    _mav_put_float(buf, 28, snr);
    _mav_put_float(buf, 32, freq_error);
    _mav_put_uint8_t(buf, 36, link_quality);
    _mav_put_uint8_t(buf, 37, rx_rate);
    _mav_put_uint8_t(buf, 38, fem_type);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_GROUND_LINK_STATUS_LEN);
#else
    mavlink_ground_link_status_t packet;
    packet.time_boot_ms = time_boot_ms;
    packet.rx_packets = rx_packets;
    packet.crc_errors = crc_errors;
    packet.frames_lost = frames_lost;
    packet.malformed = malformed;
    packet.last_rx_age_ms = last_rx_age_ms;
    packet.rssi = rssi;
    packet.snr = snr;
    packet.freq_error = freq_error;
    packet.link_quality = link_quality;
    packet.rx_rate = rx_rate;
    packet.fem_type = fem_type;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_GROUND_LINK_STATUS_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_GROUND_LINK_STATUS;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_GROUND_LINK_STATUS_MIN_LEN, MAVLINK_MSG_ID_GROUND_LINK_STATUS_LEN, MAVLINK_MSG_ID_GROUND_LINK_STATUS_CRC);
}

/**
 * @brief Encode a ground_link_status struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param ground_link_status C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_ground_link_status_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_ground_link_status_t* ground_link_status)
{
    return mavlink_msg_ground_link_status_pack(system_id, component_id, msg, ground_link_status->time_boot_ms, ground_link_status->rx_packets, ground_link_status->crc_errors, ground_link_status->frames_lost, ground_link_status->malformed, ground_link_status->last_rx_age_ms, ground_link_status->rssi, ground_link_status->snr, ground_link_status->freq_error, ground_link_status->link_quality, ground_link_status->rx_rate, ground_link_status->fem_type);
}

/**
 * @brief Encode a ground_link_status struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param ground_link_status C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_ground_link_status_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_ground_link_status_t* ground_link_status)
{
    return mavlink_msg_ground_link_status_pack_chan(system_id, component_id, chan, msg, ground_link_status->time_boot_ms, ground_link_status->rx_packets, ground_link_status->crc_errors, ground_link_status->frames_lost, ground_link_status->malformed, ground_link_status->last_rx_age_ms, ground_link_status->rssi, ground_link_status->snr, ground_link_status->freq_error, ground_link_status->link_quality, ground_link_status->rx_rate, ground_link_status->fem_type);
}

/**
 * @brief Encode a ground_link_status struct with provided status structure
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 * @param ground_link_status C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_ground_link_status_encode_status(uint8_t system_id, uint8_t component_id, mavlink_status_t* _status, mavlink_message_t* msg, const mavlink_ground_link_status_t* ground_link_status)
{
    return mavlink_msg_ground_link_status_pack_status(system_id, component_id, _status, msg,  ground_link_status->time_boot_ms, ground_link_status->rx_packets, ground_link_status->crc_errors, ground_link_status->frames_lost, ground_link_status->malformed, ground_link_status->last_rx_age_ms, ground_link_status->rssi, ground_link_status->snr, ground_link_status->freq_error, ground_link_status->link_quality, ground_link_status->rx_rate, ground_link_status->fem_type);
}

/**
 * @brief Send a ground_link_status message
 * @param chan MAVLink channel to send the message
 *
 * @param time_boot_ms [ms] Temps depuis le démarrage de la station.
 * @param rx_packets  Paquets LoRa reçus avec CRC valide.
 * @param crc_errors  Paquets LoRa reçus avec CRC invalide.
 * @param frames_lost  Trames MAVLink perdues (trous dans la séquence de la fusée).
 * @param malformed  Paquets LoRa ne contenant pas de trame MAVLink valide.
 * @param last_rx_age_ms [ms] Temps depuis le dernier paquet valide (UINT32_MAX si jamais reçu).
 * @param rssi [dBm] RSSI du dernier paquet.
 * @param snr [dB] SNR du dernier paquet.
 * @param freq_error [Hz] Erreur de fréquence du dernier paquet.
 * @param link_quality [%] Trames reçues / attendues sur la fenêtre glissante.
 * @param rx_rate  Paquets valides reçus pendant la dernière seconde.
 * @param fem_type  Front-end RF détecté.
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_ground_link_status_send(mavlink_channel_t chan, uint32_t time_boot_ms, uint32_t rx_packets, uint32_t crc_errors, uint32_t frames_lost, uint32_t malformed, uint32_t last_rx_age_ms, float rssi, float snr, float freq_error, uint8_t link_quality, uint8_t rx_rate, uint8_t fem_type)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_GROUND_LINK_STATUS_LEN];
    _mav_put_uint32_t(buf, 0, time_boot_ms);
    _mav_put_uint32_t(buf, 4, rx_packets);
    _mav_put_uint32_t(buf, 8, crc_errors);
    _mav_put_uint32_t(buf, 12, frames_lost);
    _mav_put_uint32_t(buf, 16, malformed);
    _mav_put_uint32_t(buf, 20, last_rx_age_ms);
    _mav_put_float(buf, 24, rssi);
    _mav_put_float(buf, 28, snr);
    _mav_put_float(buf, 32, freq_error);
    _mav_put_uint8_t(buf, 36, link_quality);
    _mav_put_uint8_t(buf, 37, rx_rate);
    _mav_put_uint8_t(buf, 38, fem_type);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_GROUND_LINK_STATUS, buf, MAVLINK_MSG_ID_GROUND_LINK_STATUS_MIN_LEN, MAVLINK_MSG_ID_GROUND_LINK_STATUS_LEN, MAVLINK_MSG_ID_GROUND_LINK_STATUS_CRC);
#else
    mavlink_ground_link_status_t packet;
    packet.time_boot_ms = time_boot_ms;
    packet.rx_packets = rx_packets;
    packet.crc_errors = crc_errors;
    packet.frames_lost = frames_lost;
    packet.malformed = malformed;
    packet.last_rx_age_ms = last_rx_age_ms;
    packet.rssi = rssi;
    packet.snr = snr;
    packet.freq_error = freq_error;
    packet.link_quality = link_quality;
    packet.rx_rate = rx_rate;
    packet.fem_type = fem_type;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_GROUND_LINK_STATUS, (const char *)&packet, MAVLINK_MSG_ID_GROUND_LINK_STATUS_MIN_LEN, MAVLINK_MSG_ID_GROUND_LINK_STATUS_LEN, MAVLINK_MSG_ID_GROUND_LINK_STATUS_CRC);
#endif
}

/**
 * @brief Send a ground_link_status message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_ground_link_status_send_struct(mavlink_channel_t chan, const mavlink_ground_link_status_t* ground_link_status)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_ground_link_status_send(chan, ground_link_status->time_boot_ms, ground_link_status->rx_packets, ground_link_status->crc_errors, ground_link_status->frames_lost, ground_link_status->malformed, ground_link_status->last_rx_age_ms, ground_link_status->rssi, ground_link_status->snr, ground_link_status->freq_error, ground_link_status->link_quality, ground_link_status->rx_rate, ground_link_status->fem_type);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_GROUND_LINK_STATUS, (const char *)ground_link_status, MAVLINK_MSG_ID_GROUND_LINK_STATUS_MIN_LEN, MAVLINK_MSG_ID_GROUND_LINK_STATUS_LEN, MAVLINK_MSG_ID_GROUND_LINK_STATUS_CRC);
#endif
}

#if MAVLINK_MSG_ID_GROUND_LINK_STATUS_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This variant of _send() can be used to save stack space by reusing
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_ground_link_status_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint32_t time_boot_ms, uint32_t rx_packets, uint32_t crc_errors, uint32_t frames_lost, uint32_t malformed, uint32_t last_rx_age_ms, float rssi, float snr, float freq_error, uint8_t link_quality, uint8_t rx_rate, uint8_t fem_type)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint32_t(buf, 0, time_boot_ms);
    _mav_put_uint32_t(buf, 4, rx_packets);
    _mav_put_uint32_t(buf, 8, crc_errors);
    _mav_put_uint32_t(buf, 12, frames_lost);
    _mav_put_uint32_t(buf, 16, malformed);
    _mav_put_uint32_t(buf, 20, last_rx_age_ms);
    _mav_put_float(buf, 24, rssi);
    _mav_put_float(buf, 28, snr);
    _mav_put_float(buf, 32, freq_error);
    _mav_put_uint8_t(buf, 36, link_quality);
    _mav_put_uint8_t(buf, 37, rx_rate);
    _mav_put_uint8_t(buf, 38, fem_type);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_GROUND_LINK_STATUS, buf, MAVLINK_MSG_ID_GROUND_LINK_STATUS_MIN_LEN, MAVLINK_MSG_ID_GROUND_LINK_STATUS_LEN, MAVLINK_MSG_ID_GROUND_LINK_STATUS_CRC);
#else
    mavlink_ground_link_status_t *packet = (mavlink_ground_link_status_t *)msgbuf;
    packet->time_boot_ms = time_boot_ms;
    packet->rx_packets = rx_packets;
    packet->crc_errors = crc_errors;
    packet->frames_lost = frames_lost;
    packet->malformed = malformed;
    packet->last_rx_age_ms = last_rx_age_ms;
    packet->rssi = rssi;
    packet->snr = snr;
    packet->freq_error = freq_error;
    packet->link_quality = link_quality;
    packet->rx_rate = rx_rate;
    packet->fem_type = fem_type;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_GROUND_LINK_STATUS, (const char *)packet, MAVLINK_MSG_ID_GROUND_LINK_STATUS_MIN_LEN, MAVLINK_MSG_ID_GROUND_LINK_STATUS_LEN, MAVLINK_MSG_ID_GROUND_LINK_STATUS_CRC);
#endif
}
#endif

#endif

// MESSAGE GROUND_LINK_STATUS UNPACKING


/**
 * @brief Get field time_boot_ms from ground_link_status message
 *
 * @return [ms] Temps depuis le démarrage de la station.
 */
static inline uint32_t mavlink_msg_ground_link_status_get_time_boot_ms(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint32_t(msg,  0);
}

/**
 * @brief Get field rx_packets from ground_link_status message
 *
 * @return  Paquets LoRa reçus avec CRC valide.
 */
static inline uint32_t mavlink_msg_ground_link_status_get_rx_packets(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint32_t(msg,  4);
}

/**
 * @brief Get field crc_errors from ground_link_status message
 *
 * @return  Paquets LoRa reçus avec CRC invalide.
 */
static inline uint32_t mavlink_msg_ground_link_status_get_crc_errors(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint32_t(msg,  8);
}

/**
 * @brief Get field frames_lost from ground_link_status message
 *
 * @return  Trames MAVLink perdues (trous dans la séquence de la fusée).
 */
static inline uint32_t mavlink_msg_ground_link_status_get_frames_lost(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint32_t(msg,  12);
}

/**
 * @brief Get field malformed from ground_link_status message
 *
 * @return  Paquets LoRa ne contenant pas de trame MAVLink valide.
 */
static inline uint32_t mavlink_msg_ground_link_status_get_malformed(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint32_t(msg,  16);
}

/**
 * @brief Get field last_rx_age_ms from ground_link_status message
 *
 * @return [ms] Temps depuis le dernier paquet valide (UINT32_MAX si jamais reçu).
 */
static inline uint32_t mavlink_msg_ground_link_status_get_last_rx_age_ms(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint32_t(msg,  20);
}

/**
 * @brief Get field rssi from ground_link_status message
 *
 * @return [dBm] RSSI du dernier paquet.
 */
static inline float mavlink_msg_ground_link_status_get_rssi(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  24);
}

/**
 * @brief Get field snr from ground_link_status message
 *
 * @return [dB] SNR du dernier paquet.
 */
static inline float mavlink_msg_ground_link_status_get_snr(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  28);
}

/**
 * @brief Get field freq_error from ground_link_status message
 *
 * @return [Hz] Erreur de fréquence du dernier paquet.
 */
static inline float mavlink_msg_ground_link_status_get_freq_error(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  32);
}

/**
 * @brief Get field link_quality from ground_link_status message
 *
 * @return [%] Trames reçues / attendues sur la fenêtre glissante.
 */
static inline uint8_t mavlink_msg_ground_link_status_get_link_quality(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  36);
}

/**
 * @brief Get field rx_rate from ground_link_status message
 *
 * @return  Paquets valides reçus pendant la dernière seconde.
 */
static inline uint8_t mavlink_msg_ground_link_status_get_rx_rate(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  37);
}

/**
 * @brief Get field fem_type from ground_link_status message
 *
 * @return  Front-end RF détecté.
 */
static inline uint8_t mavlink_msg_ground_link_status_get_fem_type(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  38);
}

/**
 * @brief Decode a ground_link_status message into a struct
 *
 * @param msg The message to decode
 * @param ground_link_status C-struct to decode the message contents into
 */
static inline void mavlink_msg_ground_link_status_decode(const mavlink_message_t* msg, mavlink_ground_link_status_t* ground_link_status)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    ground_link_status->time_boot_ms = mavlink_msg_ground_link_status_get_time_boot_ms(msg);
    ground_link_status->rx_packets = mavlink_msg_ground_link_status_get_rx_packets(msg);
    ground_link_status->crc_errors = mavlink_msg_ground_link_status_get_crc_errors(msg);
    ground_link_status->frames_lost = mavlink_msg_ground_link_status_get_frames_lost(msg);
    ground_link_status->malformed = mavlink_msg_ground_link_status_get_malformed(msg);
    ground_link_status->last_rx_age_ms = mavlink_msg_ground_link_status_get_last_rx_age_ms(msg);
    ground_link_status->rssi = mavlink_msg_ground_link_status_get_rssi(msg);
    ground_link_status->snr = mavlink_msg_ground_link_status_get_snr(msg);
    ground_link_status->freq_error = mavlink_msg_ground_link_status_get_freq_error(msg);
    ground_link_status->link_quality = mavlink_msg_ground_link_status_get_link_quality(msg);
    ground_link_status->rx_rate = mavlink_msg_ground_link_status_get_rx_rate(msg);
    ground_link_status->fem_type = mavlink_msg_ground_link_status_get_fem_type(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_GROUND_LINK_STATUS_LEN? msg->len : MAVLINK_MSG_ID_GROUND_LINK_STATUS_LEN;
        memset(ground_link_status, 0, MAVLINK_MSG_ID_GROUND_LINK_STATUS_LEN);
    memcpy(ground_link_status, _MAV_PAYLOAD(msg), len);
#endif
}
