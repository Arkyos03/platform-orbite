#pragma once
// MESSAGE ROCKET_STATUS PACKING

#include <stdint.h>

#define MAVLINK_MSG_ID_ROCKET_STATUS 51001


typedef struct __mavlink_rocket_status_t {
 uint32_t time_boot_ms; /*< [ms] Temps depuis le démarrage de l'ordinateur de bord.*/
 int32_t max_alt_agl; /*< [cm] Altitude max atteinte (apogée).*/
 uint32_t apogee_time_ms; /*< [ms] Instant de détection de l'apogée (time_boot_ms), 0 si non atteinte.*/
 uint16_t vbat; /*< [mV] Tension batterie.*/
 int16_t temperature; /*< [cdegC] Température (baromètre).*/
 int16_t max_vz; /*< [cm/s] Vitesse verticale max.*/
 int16_t max_acc; /*< [mG] Accélération axiale max.*/
 uint16_t sensor_health; /*<  Santé des sous-ensembles (1 = OK).*/
 uint16_t gps_hdop; /*< [1E-2] HDOP GPS x100 (UINT16_MAX si inconnu).*/
 uint8_t pyro_continuity; /*<  Continuité des inflammateurs (1 = OK).*/
 uint8_t pyro_fired; /*<  Voies déjà déclenchées.*/
 uint8_t gps_fix_type; /*<  Type de fix GPS.*/
 uint8_t gps_sats; /*<  Nombre de satellites utilisés.*/
} mavlink_rocket_status_t;

#define MAVLINK_MSG_ID_ROCKET_STATUS_LEN 28
#define MAVLINK_MSG_ID_ROCKET_STATUS_MIN_LEN 28
#define MAVLINK_MSG_ID_51001_LEN 28
#define MAVLINK_MSG_ID_51001_MIN_LEN 28

#define MAVLINK_MSG_ID_ROCKET_STATUS_CRC 111
#define MAVLINK_MSG_ID_51001_CRC 111



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_ROCKET_STATUS { \
    51001, \
    "ROCKET_STATUS", \
    13, \
    {  { "time_boot_ms", NULL, MAVLINK_TYPE_UINT32_T, 0, 0, offsetof(mavlink_rocket_status_t, time_boot_ms) }, \
         { "max_alt_agl", NULL, MAVLINK_TYPE_INT32_T, 0, 4, offsetof(mavlink_rocket_status_t, max_alt_agl) }, \
         { "apogee_time_ms", NULL, MAVLINK_TYPE_UINT32_T, 0, 8, offsetof(mavlink_rocket_status_t, apogee_time_ms) }, \
         { "vbat", NULL, MAVLINK_TYPE_UINT16_T, 0, 12, offsetof(mavlink_rocket_status_t, vbat) }, \
         { "temperature", NULL, MAVLINK_TYPE_INT16_T, 0, 14, offsetof(mavlink_rocket_status_t, temperature) }, \
         { "max_vz", NULL, MAVLINK_TYPE_INT16_T, 0, 16, offsetof(mavlink_rocket_status_t, max_vz) }, \
         { "max_acc", NULL, MAVLINK_TYPE_INT16_T, 0, 18, offsetof(mavlink_rocket_status_t, max_acc) }, \
         { "sensor_health", NULL, MAVLINK_TYPE_UINT16_T, 0, 20, offsetof(mavlink_rocket_status_t, sensor_health) }, \
         { "gps_hdop", NULL, MAVLINK_TYPE_UINT16_T, 0, 22, offsetof(mavlink_rocket_status_t, gps_hdop) }, \
         { "pyro_continuity", NULL, MAVLINK_TYPE_UINT8_T, 0, 24, offsetof(mavlink_rocket_status_t, pyro_continuity) }, \
         { "pyro_fired", NULL, MAVLINK_TYPE_UINT8_T, 0, 25, offsetof(mavlink_rocket_status_t, pyro_fired) }, \
         { "gps_fix_type", NULL, MAVLINK_TYPE_UINT8_T, 0, 26, offsetof(mavlink_rocket_status_t, gps_fix_type) }, \
         { "gps_sats", NULL, MAVLINK_TYPE_UINT8_T, 0, 27, offsetof(mavlink_rocket_status_t, gps_sats) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_ROCKET_STATUS { \
    "ROCKET_STATUS", \
    13, \
    {  { "time_boot_ms", NULL, MAVLINK_TYPE_UINT32_T, 0, 0, offsetof(mavlink_rocket_status_t, time_boot_ms) }, \
         { "max_alt_agl", NULL, MAVLINK_TYPE_INT32_T, 0, 4, offsetof(mavlink_rocket_status_t, max_alt_agl) }, \
         { "apogee_time_ms", NULL, MAVLINK_TYPE_UINT32_T, 0, 8, offsetof(mavlink_rocket_status_t, apogee_time_ms) }, \
         { "vbat", NULL, MAVLINK_TYPE_UINT16_T, 0, 12, offsetof(mavlink_rocket_status_t, vbat) }, \
         { "temperature", NULL, MAVLINK_TYPE_INT16_T, 0, 14, offsetof(mavlink_rocket_status_t, temperature) }, \
         { "max_vz", NULL, MAVLINK_TYPE_INT16_T, 0, 16, offsetof(mavlink_rocket_status_t, max_vz) }, \
         { "max_acc", NULL, MAVLINK_TYPE_INT16_T, 0, 18, offsetof(mavlink_rocket_status_t, max_acc) }, \
         { "sensor_health", NULL, MAVLINK_TYPE_UINT16_T, 0, 20, offsetof(mavlink_rocket_status_t, sensor_health) }, \
         { "gps_hdop", NULL, MAVLINK_TYPE_UINT16_T, 0, 22, offsetof(mavlink_rocket_status_t, gps_hdop) }, \
         { "pyro_continuity", NULL, MAVLINK_TYPE_UINT8_T, 0, 24, offsetof(mavlink_rocket_status_t, pyro_continuity) }, \
         { "pyro_fired", NULL, MAVLINK_TYPE_UINT8_T, 0, 25, offsetof(mavlink_rocket_status_t, pyro_fired) }, \
         { "gps_fix_type", NULL, MAVLINK_TYPE_UINT8_T, 0, 26, offsetof(mavlink_rocket_status_t, gps_fix_type) }, \
         { "gps_sats", NULL, MAVLINK_TYPE_UINT8_T, 0, 27, offsetof(mavlink_rocket_status_t, gps_sats) }, \
         } \
}
#endif

/**
 * @brief Pack a rocket_status message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param time_boot_ms [ms] Temps depuis le démarrage de l'ordinateur de bord.
 * @param max_alt_agl [cm] Altitude max atteinte (apogée).
 * @param apogee_time_ms [ms] Instant de détection de l'apogée (time_boot_ms), 0 si non atteinte.
 * @param vbat [mV] Tension batterie.
 * @param temperature [cdegC] Température (baromètre).
 * @param max_vz [cm/s] Vitesse verticale max.
 * @param max_acc [mG] Accélération axiale max.
 * @param sensor_health  Santé des sous-ensembles (1 = OK).
 * @param gps_hdop [1E-2] HDOP GPS x100 (UINT16_MAX si inconnu).
 * @param pyro_continuity  Continuité des inflammateurs (1 = OK).
 * @param pyro_fired  Voies déjà déclenchées.
 * @param gps_fix_type  Type de fix GPS.
 * @param gps_sats  Nombre de satellites utilisés.
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_rocket_status_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint32_t time_boot_ms, int32_t max_alt_agl, uint32_t apogee_time_ms, uint16_t vbat, int16_t temperature, int16_t max_vz, int16_t max_acc, uint16_t sensor_health, uint16_t gps_hdop, uint8_t pyro_continuity, uint8_t pyro_fired, uint8_t gps_fix_type, uint8_t gps_sats)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_ROCKET_STATUS_LEN];
    _mav_put_uint32_t(buf, 0, time_boot_ms);
    _mav_put_int32_t(buf, 4, max_alt_agl);
    _mav_put_uint32_t(buf, 8, apogee_time_ms);
    _mav_put_uint16_t(buf, 12, vbat);
    _mav_put_int16_t(buf, 14, temperature);
    _mav_put_int16_t(buf, 16, max_vz);
    _mav_put_int16_t(buf, 18, max_acc);
    _mav_put_uint16_t(buf, 20, sensor_health);
    _mav_put_uint16_t(buf, 22, gps_hdop);
    _mav_put_uint8_t(buf, 24, pyro_continuity);
    _mav_put_uint8_t(buf, 25, pyro_fired);
    _mav_put_uint8_t(buf, 26, gps_fix_type);
    _mav_put_uint8_t(buf, 27, gps_sats);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_ROCKET_STATUS_LEN);
#else
    mavlink_rocket_status_t packet;
    packet.time_boot_ms = time_boot_ms;
    packet.max_alt_agl = max_alt_agl;
    packet.apogee_time_ms = apogee_time_ms;
    packet.vbat = vbat;
    packet.temperature = temperature;
    packet.max_vz = max_vz;
    packet.max_acc = max_acc;
    packet.sensor_health = sensor_health;
    packet.gps_hdop = gps_hdop;
    packet.pyro_continuity = pyro_continuity;
    packet.pyro_fired = pyro_fired;
    packet.gps_fix_type = gps_fix_type;
    packet.gps_sats = gps_sats;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_ROCKET_STATUS_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_ROCKET_STATUS;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_ROCKET_STATUS_MIN_LEN, MAVLINK_MSG_ID_ROCKET_STATUS_LEN, MAVLINK_MSG_ID_ROCKET_STATUS_CRC);
}

/**
 * @brief Pack a rocket_status message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 *
 * @param time_boot_ms [ms] Temps depuis le démarrage de l'ordinateur de bord.
 * @param max_alt_agl [cm] Altitude max atteinte (apogée).
 * @param apogee_time_ms [ms] Instant de détection de l'apogée (time_boot_ms), 0 si non atteinte.
 * @param vbat [mV] Tension batterie.
 * @param temperature [cdegC] Température (baromètre).
 * @param max_vz [cm/s] Vitesse verticale max.
 * @param max_acc [mG] Accélération axiale max.
 * @param sensor_health  Santé des sous-ensembles (1 = OK).
 * @param gps_hdop [1E-2] HDOP GPS x100 (UINT16_MAX si inconnu).
 * @param pyro_continuity  Continuité des inflammateurs (1 = OK).
 * @param pyro_fired  Voies déjà déclenchées.
 * @param gps_fix_type  Type de fix GPS.
 * @param gps_sats  Nombre de satellites utilisés.
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_rocket_status_pack_status(uint8_t system_id, uint8_t component_id, mavlink_status_t *_status, mavlink_message_t* msg,
                               uint32_t time_boot_ms, int32_t max_alt_agl, uint32_t apogee_time_ms, uint16_t vbat, int16_t temperature, int16_t max_vz, int16_t max_acc, uint16_t sensor_health, uint16_t gps_hdop, uint8_t pyro_continuity, uint8_t pyro_fired, uint8_t gps_fix_type, uint8_t gps_sats)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_ROCKET_STATUS_LEN];
    _mav_put_uint32_t(buf, 0, time_boot_ms);
    _mav_put_int32_t(buf, 4, max_alt_agl);
    _mav_put_uint32_t(buf, 8, apogee_time_ms);
    _mav_put_uint16_t(buf, 12, vbat);
    _mav_put_int16_t(buf, 14, temperature);
    _mav_put_int16_t(buf, 16, max_vz);
    _mav_put_int16_t(buf, 18, max_acc);
    _mav_put_uint16_t(buf, 20, sensor_health);
    _mav_put_uint16_t(buf, 22, gps_hdop);
    _mav_put_uint8_t(buf, 24, pyro_continuity);
    _mav_put_uint8_t(buf, 25, pyro_fired);
    _mav_put_uint8_t(buf, 26, gps_fix_type);
    _mav_put_uint8_t(buf, 27, gps_sats);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_ROCKET_STATUS_LEN);
#else
    mavlink_rocket_status_t packet;
    packet.time_boot_ms = time_boot_ms;
    packet.max_alt_agl = max_alt_agl;
    packet.apogee_time_ms = apogee_time_ms;
    packet.vbat = vbat;
    packet.temperature = temperature;
    packet.max_vz = max_vz;
    packet.max_acc = max_acc;
    packet.sensor_health = sensor_health;
    packet.gps_hdop = gps_hdop;
    packet.pyro_continuity = pyro_continuity;
    packet.pyro_fired = pyro_fired;
    packet.gps_fix_type = gps_fix_type;
    packet.gps_sats = gps_sats;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_ROCKET_STATUS_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_ROCKET_STATUS;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_ROCKET_STATUS_MIN_LEN, MAVLINK_MSG_ID_ROCKET_STATUS_LEN, MAVLINK_MSG_ID_ROCKET_STATUS_CRC);
#else
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_ROCKET_STATUS_MIN_LEN, MAVLINK_MSG_ID_ROCKET_STATUS_LEN);
#endif
}

/**
 * @brief Pack a rocket_status message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param time_boot_ms [ms] Temps depuis le démarrage de l'ordinateur de bord.
 * @param max_alt_agl [cm] Altitude max atteinte (apogée).
 * @param apogee_time_ms [ms] Instant de détection de l'apogée (time_boot_ms), 0 si non atteinte.
 * @param vbat [mV] Tension batterie.
 * @param temperature [cdegC] Température (baromètre).
 * @param max_vz [cm/s] Vitesse verticale max.
 * @param max_acc [mG] Accélération axiale max.
 * @param sensor_health  Santé des sous-ensembles (1 = OK).
 * @param gps_hdop [1E-2] HDOP GPS x100 (UINT16_MAX si inconnu).
 * @param pyro_continuity  Continuité des inflammateurs (1 = OK).
 * @param pyro_fired  Voies déjà déclenchées.
 * @param gps_fix_type  Type de fix GPS.
 * @param gps_sats  Nombre de satellites utilisés.
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_rocket_status_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint32_t time_boot_ms,int32_t max_alt_agl,uint32_t apogee_time_ms,uint16_t vbat,int16_t temperature,int16_t max_vz,int16_t max_acc,uint16_t sensor_health,uint16_t gps_hdop,uint8_t pyro_continuity,uint8_t pyro_fired,uint8_t gps_fix_type,uint8_t gps_sats)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_ROCKET_STATUS_LEN];
    _mav_put_uint32_t(buf, 0, time_boot_ms);
    _mav_put_int32_t(buf, 4, max_alt_agl);
    _mav_put_uint32_t(buf, 8, apogee_time_ms);
    _mav_put_uint16_t(buf, 12, vbat);
    _mav_put_int16_t(buf, 14, temperature);
    _mav_put_int16_t(buf, 16, max_vz);
    _mav_put_int16_t(buf, 18, max_acc);
    _mav_put_uint16_t(buf, 20, sensor_health);
    _mav_put_uint16_t(buf, 22, gps_hdop);
    _mav_put_uint8_t(buf, 24, pyro_continuity);
    _mav_put_uint8_t(buf, 25, pyro_fired);
    _mav_put_uint8_t(buf, 26, gps_fix_type);
    _mav_put_uint8_t(buf, 27, gps_sats);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_ROCKET_STATUS_LEN);
#else
    mavlink_rocket_status_t packet;
    packet.time_boot_ms = time_boot_ms;
    packet.max_alt_agl = max_alt_agl;
    packet.apogee_time_ms = apogee_time_ms;
    packet.vbat = vbat;
    packet.temperature = temperature;
    packet.max_vz = max_vz;
    packet.max_acc = max_acc;
    packet.sensor_health = sensor_health;
    packet.gps_hdop = gps_hdop;
    packet.pyro_continuity = pyro_continuity;
    packet.pyro_fired = pyro_fired;
    packet.gps_fix_type = gps_fix_type;
    packet.gps_sats = gps_sats;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_ROCKET_STATUS_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_ROCKET_STATUS;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_ROCKET_STATUS_MIN_LEN, MAVLINK_MSG_ID_ROCKET_STATUS_LEN, MAVLINK_MSG_ID_ROCKET_STATUS_CRC);
}

/**
 * @brief Encode a rocket_status struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param rocket_status C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_rocket_status_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_rocket_status_t* rocket_status)
{
    return mavlink_msg_rocket_status_pack(system_id, component_id, msg, rocket_status->time_boot_ms, rocket_status->max_alt_agl, rocket_status->apogee_time_ms, rocket_status->vbat, rocket_status->temperature, rocket_status->max_vz, rocket_status->max_acc, rocket_status->sensor_health, rocket_status->gps_hdop, rocket_status->pyro_continuity, rocket_status->pyro_fired, rocket_status->gps_fix_type, rocket_status->gps_sats);
}

/**
 * @brief Encode a rocket_status struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param rocket_status C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_rocket_status_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_rocket_status_t* rocket_status)
{
    return mavlink_msg_rocket_status_pack_chan(system_id, component_id, chan, msg, rocket_status->time_boot_ms, rocket_status->max_alt_agl, rocket_status->apogee_time_ms, rocket_status->vbat, rocket_status->temperature, rocket_status->max_vz, rocket_status->max_acc, rocket_status->sensor_health, rocket_status->gps_hdop, rocket_status->pyro_continuity, rocket_status->pyro_fired, rocket_status->gps_fix_type, rocket_status->gps_sats);
}

/**
 * @brief Encode a rocket_status struct with provided status structure
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 * @param rocket_status C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_rocket_status_encode_status(uint8_t system_id, uint8_t component_id, mavlink_status_t* _status, mavlink_message_t* msg, const mavlink_rocket_status_t* rocket_status)
{
    return mavlink_msg_rocket_status_pack_status(system_id, component_id, _status, msg,  rocket_status->time_boot_ms, rocket_status->max_alt_agl, rocket_status->apogee_time_ms, rocket_status->vbat, rocket_status->temperature, rocket_status->max_vz, rocket_status->max_acc, rocket_status->sensor_health, rocket_status->gps_hdop, rocket_status->pyro_continuity, rocket_status->pyro_fired, rocket_status->gps_fix_type, rocket_status->gps_sats);
}

/**
 * @brief Send a rocket_status message
 * @param chan MAVLink channel to send the message
 *
 * @param time_boot_ms [ms] Temps depuis le démarrage de l'ordinateur de bord.
 * @param max_alt_agl [cm] Altitude max atteinte (apogée).
 * @param apogee_time_ms [ms] Instant de détection de l'apogée (time_boot_ms), 0 si non atteinte.
 * @param vbat [mV] Tension batterie.
 * @param temperature [cdegC] Température (baromètre).
 * @param max_vz [cm/s] Vitesse verticale max.
 * @param max_acc [mG] Accélération axiale max.
 * @param sensor_health  Santé des sous-ensembles (1 = OK).
 * @param gps_hdop [1E-2] HDOP GPS x100 (UINT16_MAX si inconnu).
 * @param pyro_continuity  Continuité des inflammateurs (1 = OK).
 * @param pyro_fired  Voies déjà déclenchées.
 * @param gps_fix_type  Type de fix GPS.
 * @param gps_sats  Nombre de satellites utilisés.
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_rocket_status_send(mavlink_channel_t chan, uint32_t time_boot_ms, int32_t max_alt_agl, uint32_t apogee_time_ms, uint16_t vbat, int16_t temperature, int16_t max_vz, int16_t max_acc, uint16_t sensor_health, uint16_t gps_hdop, uint8_t pyro_continuity, uint8_t pyro_fired, uint8_t gps_fix_type, uint8_t gps_sats)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_ROCKET_STATUS_LEN];
    _mav_put_uint32_t(buf, 0, time_boot_ms);
    _mav_put_int32_t(buf, 4, max_alt_agl);
    _mav_put_uint32_t(buf, 8, apogee_time_ms);
    _mav_put_uint16_t(buf, 12, vbat);
    _mav_put_int16_t(buf, 14, temperature);
    _mav_put_int16_t(buf, 16, max_vz);
    _mav_put_int16_t(buf, 18, max_acc);
    _mav_put_uint16_t(buf, 20, sensor_health);
    _mav_put_uint16_t(buf, 22, gps_hdop);
    _mav_put_uint8_t(buf, 24, pyro_continuity);
    _mav_put_uint8_t(buf, 25, pyro_fired);
    _mav_put_uint8_t(buf, 26, gps_fix_type);
    _mav_put_uint8_t(buf, 27, gps_sats);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_ROCKET_STATUS, buf, MAVLINK_MSG_ID_ROCKET_STATUS_MIN_LEN, MAVLINK_MSG_ID_ROCKET_STATUS_LEN, MAVLINK_MSG_ID_ROCKET_STATUS_CRC);
#else
    mavlink_rocket_status_t packet;
    packet.time_boot_ms = time_boot_ms;
    packet.max_alt_agl = max_alt_agl;
    packet.apogee_time_ms = apogee_time_ms;
    packet.vbat = vbat;
    packet.temperature = temperature;
    packet.max_vz = max_vz;
    packet.max_acc = max_acc;
    packet.sensor_health = sensor_health;
    packet.gps_hdop = gps_hdop;
    packet.pyro_continuity = pyro_continuity;
    packet.pyro_fired = pyro_fired;
    packet.gps_fix_type = gps_fix_type;
    packet.gps_sats = gps_sats;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_ROCKET_STATUS, (const char *)&packet, MAVLINK_MSG_ID_ROCKET_STATUS_MIN_LEN, MAVLINK_MSG_ID_ROCKET_STATUS_LEN, MAVLINK_MSG_ID_ROCKET_STATUS_CRC);
#endif
}

/**
 * @brief Send a rocket_status message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_rocket_status_send_struct(mavlink_channel_t chan, const mavlink_rocket_status_t* rocket_status)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_rocket_status_send(chan, rocket_status->time_boot_ms, rocket_status->max_alt_agl, rocket_status->apogee_time_ms, rocket_status->vbat, rocket_status->temperature, rocket_status->max_vz, rocket_status->max_acc, rocket_status->sensor_health, rocket_status->gps_hdop, rocket_status->pyro_continuity, rocket_status->pyro_fired, rocket_status->gps_fix_type, rocket_status->gps_sats);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_ROCKET_STATUS, (const char *)rocket_status, MAVLINK_MSG_ID_ROCKET_STATUS_MIN_LEN, MAVLINK_MSG_ID_ROCKET_STATUS_LEN, MAVLINK_MSG_ID_ROCKET_STATUS_CRC);
#endif
}

#if MAVLINK_MSG_ID_ROCKET_STATUS_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This variant of _send() can be used to save stack space by reusing
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_rocket_status_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint32_t time_boot_ms, int32_t max_alt_agl, uint32_t apogee_time_ms, uint16_t vbat, int16_t temperature, int16_t max_vz, int16_t max_acc, uint16_t sensor_health, uint16_t gps_hdop, uint8_t pyro_continuity, uint8_t pyro_fired, uint8_t gps_fix_type, uint8_t gps_sats)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint32_t(buf, 0, time_boot_ms);
    _mav_put_int32_t(buf, 4, max_alt_agl);
    _mav_put_uint32_t(buf, 8, apogee_time_ms);
    _mav_put_uint16_t(buf, 12, vbat);
    _mav_put_int16_t(buf, 14, temperature);
    _mav_put_int16_t(buf, 16, max_vz);
    _mav_put_int16_t(buf, 18, max_acc);
    _mav_put_uint16_t(buf, 20, sensor_health);
    _mav_put_uint16_t(buf, 22, gps_hdop);
    _mav_put_uint8_t(buf, 24, pyro_continuity);
    _mav_put_uint8_t(buf, 25, pyro_fired);
    _mav_put_uint8_t(buf, 26, gps_fix_type);
    _mav_put_uint8_t(buf, 27, gps_sats);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_ROCKET_STATUS, buf, MAVLINK_MSG_ID_ROCKET_STATUS_MIN_LEN, MAVLINK_MSG_ID_ROCKET_STATUS_LEN, MAVLINK_MSG_ID_ROCKET_STATUS_CRC);
#else
    mavlink_rocket_status_t *packet = (mavlink_rocket_status_t *)msgbuf;
    packet->time_boot_ms = time_boot_ms;
    packet->max_alt_agl = max_alt_agl;
    packet->apogee_time_ms = apogee_time_ms;
    packet->vbat = vbat;
    packet->temperature = temperature;
    packet->max_vz = max_vz;
    packet->max_acc = max_acc;
    packet->sensor_health = sensor_health;
    packet->gps_hdop = gps_hdop;
    packet->pyro_continuity = pyro_continuity;
    packet->pyro_fired = pyro_fired;
    packet->gps_fix_type = gps_fix_type;
    packet->gps_sats = gps_sats;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_ROCKET_STATUS, (const char *)packet, MAVLINK_MSG_ID_ROCKET_STATUS_MIN_LEN, MAVLINK_MSG_ID_ROCKET_STATUS_LEN, MAVLINK_MSG_ID_ROCKET_STATUS_CRC);
#endif
}
#endif

#endif

// MESSAGE ROCKET_STATUS UNPACKING


/**
 * @brief Get field time_boot_ms from rocket_status message
 *
 * @return [ms] Temps depuis le démarrage de l'ordinateur de bord.
 */
static inline uint32_t mavlink_msg_rocket_status_get_time_boot_ms(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint32_t(msg,  0);
}

/**
 * @brief Get field max_alt_agl from rocket_status message
 *
 * @return [cm] Altitude max atteinte (apogée).
 */
static inline int32_t mavlink_msg_rocket_status_get_max_alt_agl(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int32_t(msg,  4);
}

/**
 * @brief Get field apogee_time_ms from rocket_status message
 *
 * @return [ms] Instant de détection de l'apogée (time_boot_ms), 0 si non atteinte.
 */
static inline uint32_t mavlink_msg_rocket_status_get_apogee_time_ms(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint32_t(msg,  8);
}

/**
 * @brief Get field vbat from rocket_status message
 *
 * @return [mV] Tension batterie.
 */
static inline uint16_t mavlink_msg_rocket_status_get_vbat(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint16_t(msg,  12);
}

/**
 * @brief Get field temperature from rocket_status message
 *
 * @return [cdegC] Température (baromètre).
 */
static inline int16_t mavlink_msg_rocket_status_get_temperature(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  14);
}

/**
 * @brief Get field max_vz from rocket_status message
 *
 * @return [cm/s] Vitesse verticale max.
 */
static inline int16_t mavlink_msg_rocket_status_get_max_vz(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  16);
}

/**
 * @brief Get field max_acc from rocket_status message
 *
 * @return [mG] Accélération axiale max.
 */
static inline int16_t mavlink_msg_rocket_status_get_max_acc(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  18);
}

/**
 * @brief Get field sensor_health from rocket_status message
 *
 * @return  Santé des sous-ensembles (1 = OK).
 */
static inline uint16_t mavlink_msg_rocket_status_get_sensor_health(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint16_t(msg,  20);
}

/**
 * @brief Get field gps_hdop from rocket_status message
 *
 * @return [1E-2] HDOP GPS x100 (UINT16_MAX si inconnu).
 */
static inline uint16_t mavlink_msg_rocket_status_get_gps_hdop(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint16_t(msg,  22);
}

/**
 * @brief Get field pyro_continuity from rocket_status message
 *
 * @return  Continuité des inflammateurs (1 = OK).
 */
static inline uint8_t mavlink_msg_rocket_status_get_pyro_continuity(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  24);
}

/**
 * @brief Get field pyro_fired from rocket_status message
 *
 * @return  Voies déjà déclenchées.
 */
static inline uint8_t mavlink_msg_rocket_status_get_pyro_fired(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  25);
}

/**
 * @brief Get field gps_fix_type from rocket_status message
 *
 * @return  Type de fix GPS.
 */
static inline uint8_t mavlink_msg_rocket_status_get_gps_fix_type(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  26);
}

/**
 * @brief Get field gps_sats from rocket_status message
 *
 * @return  Nombre de satellites utilisés.
 */
static inline uint8_t mavlink_msg_rocket_status_get_gps_sats(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  27);
}

/**
 * @brief Decode a rocket_status message into a struct
 *
 * @param msg The message to decode
 * @param rocket_status C-struct to decode the message contents into
 */
static inline void mavlink_msg_rocket_status_decode(const mavlink_message_t* msg, mavlink_rocket_status_t* rocket_status)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    rocket_status->time_boot_ms = mavlink_msg_rocket_status_get_time_boot_ms(msg);
    rocket_status->max_alt_agl = mavlink_msg_rocket_status_get_max_alt_agl(msg);
    rocket_status->apogee_time_ms = mavlink_msg_rocket_status_get_apogee_time_ms(msg);
    rocket_status->vbat = mavlink_msg_rocket_status_get_vbat(msg);
    rocket_status->temperature = mavlink_msg_rocket_status_get_temperature(msg);
    rocket_status->max_vz = mavlink_msg_rocket_status_get_max_vz(msg);
    rocket_status->max_acc = mavlink_msg_rocket_status_get_max_acc(msg);
    rocket_status->sensor_health = mavlink_msg_rocket_status_get_sensor_health(msg);
    rocket_status->gps_hdop = mavlink_msg_rocket_status_get_gps_hdop(msg);
    rocket_status->pyro_continuity = mavlink_msg_rocket_status_get_pyro_continuity(msg);
    rocket_status->pyro_fired = mavlink_msg_rocket_status_get_pyro_fired(msg);
    rocket_status->gps_fix_type = mavlink_msg_rocket_status_get_gps_fix_type(msg);
    rocket_status->gps_sats = mavlink_msg_rocket_status_get_gps_sats(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_ROCKET_STATUS_LEN? msg->len : MAVLINK_MSG_ID_ROCKET_STATUS_LEN;
        memset(rocket_status, 0, MAVLINK_MSG_ID_ROCKET_STATUS_LEN);
    memcpy(rocket_status, _MAV_PAYLOAD(msg), len);
#endif
}
