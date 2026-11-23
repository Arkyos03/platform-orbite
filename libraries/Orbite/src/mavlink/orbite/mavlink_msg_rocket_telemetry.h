#pragma once
// MESSAGE ROCKET_TELEMETRY PACKING

#include <stdint.h>

#define MAVLINK_MSG_ID_ROCKET_TELEMETRY 51000


typedef struct __mavlink_rocket_telemetry_t {
 uint32_t time_boot_ms; /*< [ms] Temps depuis le démarrage de l'ordinateur de bord.*/
 int32_t alt_agl; /*< [cm] Altitude barométrique au-dessus du pas de tir.*/
 int32_t lat; /*< [degE7] Latitude GPS (0 si pas de fix).*/
 int32_t lon; /*< [degE7] Longitude GPS (0 si pas de fix).*/
 int16_t alt_gps; /*< [m] Altitude GPS (MSL).*/
 int16_t vz; /*< [cm/s] Vitesse verticale (positive vers le haut).*/
 int16_t acc_axial; /*< [mG] Accélération selon l'axe de la fusée (saturée à +/-32767).*/
 int16_t roll_rate; /*< [deg/s] Vitesse de roulis.*/
 uint16_t tilt; /*< [cdeg] Inclinaison par rapport à la verticale (0 = vertical).*/
 uint8_t flight_state; /*<  Phase de vol.*/
 uint8_t flags; /*<  Indicateurs rapides.*/
} mavlink_rocket_telemetry_t;

#define MAVLINK_MSG_ID_ROCKET_TELEMETRY_LEN 28
#define MAVLINK_MSG_ID_ROCKET_TELEMETRY_MIN_LEN 28
#define MAVLINK_MSG_ID_51000_LEN 28
#define MAVLINK_MSG_ID_51000_MIN_LEN 28

#define MAVLINK_MSG_ID_ROCKET_TELEMETRY_CRC 35
#define MAVLINK_MSG_ID_51000_CRC 35



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_ROCKET_TELEMETRY { \
    51000, \
    "ROCKET_TELEMETRY", \
    11, \
    {  { "time_boot_ms", NULL, MAVLINK_TYPE_UINT32_T, 0, 0, offsetof(mavlink_rocket_telemetry_t, time_boot_ms) }, \
         { "alt_agl", NULL, MAVLINK_TYPE_INT32_T, 0, 4, offsetof(mavlink_rocket_telemetry_t, alt_agl) }, \
         { "lat", NULL, MAVLINK_TYPE_INT32_T, 0, 8, offsetof(mavlink_rocket_telemetry_t, lat) }, \
         { "lon", NULL, MAVLINK_TYPE_INT32_T, 0, 12, offsetof(mavlink_rocket_telemetry_t, lon) }, \
         { "alt_gps", NULL, MAVLINK_TYPE_INT16_T, 0, 16, offsetof(mavlink_rocket_telemetry_t, alt_gps) }, \
         { "vz", NULL, MAVLINK_TYPE_INT16_T, 0, 18, offsetof(mavlink_rocket_telemetry_t, vz) }, \
         { "acc_axial", NULL, MAVLINK_TYPE_INT16_T, 0, 20, offsetof(mavlink_rocket_telemetry_t, acc_axial) }, \
         { "roll_rate", NULL, MAVLINK_TYPE_INT16_T, 0, 22, offsetof(mavlink_rocket_telemetry_t, roll_rate) }, \
         { "tilt", NULL, MAVLINK_TYPE_UINT16_T, 0, 24, offsetof(mavlink_rocket_telemetry_t, tilt) }, \
         { "flight_state", NULL, MAVLINK_TYPE_UINT8_T, 0, 26, offsetof(mavlink_rocket_telemetry_t, flight_state) }, \
         { "flags", NULL, MAVLINK_TYPE_UINT8_T, 0, 27, offsetof(mavlink_rocket_telemetry_t, flags) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_ROCKET_TELEMETRY { \
    "ROCKET_TELEMETRY", \
    11, \
    {  { "time_boot_ms", NULL, MAVLINK_TYPE_UINT32_T, 0, 0, offsetof(mavlink_rocket_telemetry_t, time_boot_ms) }, \
         { "alt_agl", NULL, MAVLINK_TYPE_INT32_T, 0, 4, offsetof(mavlink_rocket_telemetry_t, alt_agl) }, \
         { "lat", NULL, MAVLINK_TYPE_INT32_T, 0, 8, offsetof(mavlink_rocket_telemetry_t, lat) }, \
         { "lon", NULL, MAVLINK_TYPE_INT32_T, 0, 12, offsetof(mavlink_rocket_telemetry_t, lon) }, \
         { "alt_gps", NULL, MAVLINK_TYPE_INT16_T, 0, 16, offsetof(mavlink_rocket_telemetry_t, alt_gps) }, \
         { "vz", NULL, MAVLINK_TYPE_INT16_T, 0, 18, offsetof(mavlink_rocket_telemetry_t, vz) }, \
         { "acc_axial", NULL, MAVLINK_TYPE_INT16_T, 0, 20, offsetof(mavlink_rocket_telemetry_t, acc_axial) }, \
         { "roll_rate", NULL, MAVLINK_TYPE_INT16_T, 0, 22, offsetof(mavlink_rocket_telemetry_t, roll_rate) }, \
         { "tilt", NULL, MAVLINK_TYPE_UINT16_T, 0, 24, offsetof(mavlink_rocket_telemetry_t, tilt) }, \
         { "flight_state", NULL, MAVLINK_TYPE_UINT8_T, 0, 26, offsetof(mavlink_rocket_telemetry_t, flight_state) }, \
         { "flags", NULL, MAVLINK_TYPE_UINT8_T, 0, 27, offsetof(mavlink_rocket_telemetry_t, flags) }, \
         } \
}
#endif

/**
 * @brief Pack a rocket_telemetry message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param time_boot_ms [ms] Temps depuis le démarrage de l'ordinateur de bord.
 * @param alt_agl [cm] Altitude barométrique au-dessus du pas de tir.
 * @param lat [degE7] Latitude GPS (0 si pas de fix).
 * @param lon [degE7] Longitude GPS (0 si pas de fix).
 * @param alt_gps [m] Altitude GPS (MSL).
 * @param vz [cm/s] Vitesse verticale (positive vers le haut).
 * @param acc_axial [mG] Accélération selon l'axe de la fusée (saturée à +/-32767).
 * @param roll_rate [deg/s] Vitesse de roulis.
 * @param tilt [cdeg] Inclinaison par rapport à la verticale (0 = vertical).
 * @param flight_state  Phase de vol.
 * @param flags  Indicateurs rapides.
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_rocket_telemetry_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint32_t time_boot_ms, int32_t alt_agl, int32_t lat, int32_t lon, int16_t alt_gps, int16_t vz, int16_t acc_axial, int16_t roll_rate, uint16_t tilt, uint8_t flight_state, uint8_t flags)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_ROCKET_TELEMETRY_LEN];
    _mav_put_uint32_t(buf, 0, time_boot_ms);
    _mav_put_int32_t(buf, 4, alt_agl);
    _mav_put_int32_t(buf, 8, lat);
    _mav_put_int32_t(buf, 12, lon);
    _mav_put_int16_t(buf, 16, alt_gps);
    _mav_put_int16_t(buf, 18, vz);
    _mav_put_int16_t(buf, 20, acc_axial);
    _mav_put_int16_t(buf, 22, roll_rate);
    _mav_put_uint16_t(buf, 24, tilt);
    _mav_put_uint8_t(buf, 26, flight_state);
    _mav_put_uint8_t(buf, 27, flags);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_ROCKET_TELEMETRY_LEN);
#else
    mavlink_rocket_telemetry_t packet;
    packet.time_boot_ms = time_boot_ms;
    packet.alt_agl = alt_agl;
    packet.lat = lat;
    packet.lon = lon;
    packet.alt_gps = alt_gps;
    packet.vz = vz;
    packet.acc_axial = acc_axial;
    packet.roll_rate = roll_rate;
    packet.tilt = tilt;
    packet.flight_state = flight_state;
    packet.flags = flags;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_ROCKET_TELEMETRY_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_ROCKET_TELEMETRY;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_ROCKET_TELEMETRY_MIN_LEN, MAVLINK_MSG_ID_ROCKET_TELEMETRY_LEN, MAVLINK_MSG_ID_ROCKET_TELEMETRY_CRC);
}

/**
 * @brief Pack a rocket_telemetry message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 *
 * @param time_boot_ms [ms] Temps depuis le démarrage de l'ordinateur de bord.
 * @param alt_agl [cm] Altitude barométrique au-dessus du pas de tir.
 * @param lat [degE7] Latitude GPS (0 si pas de fix).
 * @param lon [degE7] Longitude GPS (0 si pas de fix).
 * @param alt_gps [m] Altitude GPS (MSL).
 * @param vz [cm/s] Vitesse verticale (positive vers le haut).
 * @param acc_axial [mG] Accélération selon l'axe de la fusée (saturée à +/-32767).
 * @param roll_rate [deg/s] Vitesse de roulis.
 * @param tilt [cdeg] Inclinaison par rapport à la verticale (0 = vertical).
 * @param flight_state  Phase de vol.
 * @param flags  Indicateurs rapides.
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_rocket_telemetry_pack_status(uint8_t system_id, uint8_t component_id, mavlink_status_t *_status, mavlink_message_t* msg,
                               uint32_t time_boot_ms, int32_t alt_agl, int32_t lat, int32_t lon, int16_t alt_gps, int16_t vz, int16_t acc_axial, int16_t roll_rate, uint16_t tilt, uint8_t flight_state, uint8_t flags)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_ROCKET_TELEMETRY_LEN];
    _mav_put_uint32_t(buf, 0, time_boot_ms);
    _mav_put_int32_t(buf, 4, alt_agl);
    _mav_put_int32_t(buf, 8, lat);
    _mav_put_int32_t(buf, 12, lon);
    _mav_put_int16_t(buf, 16, alt_gps);
    _mav_put_int16_t(buf, 18, vz);
    _mav_put_int16_t(buf, 20, acc_axial);
    _mav_put_int16_t(buf, 22, roll_rate);
    _mav_put_uint16_t(buf, 24, tilt);
    _mav_put_uint8_t(buf, 26, flight_state);
    _mav_put_uint8_t(buf, 27, flags);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_ROCKET_TELEMETRY_LEN);
#else
    mavlink_rocket_telemetry_t packet;
    packet.time_boot_ms = time_boot_ms;
    packet.alt_agl = alt_agl;
    packet.lat = lat;
    packet.lon = lon;
    packet.alt_gps = alt_gps;
    packet.vz = vz;
    packet.acc_axial = acc_axial;
    packet.roll_rate = roll_rate;
    packet.tilt = tilt;
    packet.flight_state = flight_state;
    packet.flags = flags;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_ROCKET_TELEMETRY_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_ROCKET_TELEMETRY;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_ROCKET_TELEMETRY_MIN_LEN, MAVLINK_MSG_ID_ROCKET_TELEMETRY_LEN, MAVLINK_MSG_ID_ROCKET_TELEMETRY_CRC);
#else
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_ROCKET_TELEMETRY_MIN_LEN, MAVLINK_MSG_ID_ROCKET_TELEMETRY_LEN);
#endif
}

/**
 * @brief Pack a rocket_telemetry message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param time_boot_ms [ms] Temps depuis le démarrage de l'ordinateur de bord.
 * @param alt_agl [cm] Altitude barométrique au-dessus du pas de tir.
 * @param lat [degE7] Latitude GPS (0 si pas de fix).
 * @param lon [degE7] Longitude GPS (0 si pas de fix).
 * @param alt_gps [m] Altitude GPS (MSL).
 * @param vz [cm/s] Vitesse verticale (positive vers le haut).
 * @param acc_axial [mG] Accélération selon l'axe de la fusée (saturée à +/-32767).
 * @param roll_rate [deg/s] Vitesse de roulis.
 * @param tilt [cdeg] Inclinaison par rapport à la verticale (0 = vertical).
 * @param flight_state  Phase de vol.
 * @param flags  Indicateurs rapides.
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_rocket_telemetry_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint32_t time_boot_ms,int32_t alt_agl,int32_t lat,int32_t lon,int16_t alt_gps,int16_t vz,int16_t acc_axial,int16_t roll_rate,uint16_t tilt,uint8_t flight_state,uint8_t flags)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_ROCKET_TELEMETRY_LEN];
    _mav_put_uint32_t(buf, 0, time_boot_ms);
    _mav_put_int32_t(buf, 4, alt_agl);
    _mav_put_int32_t(buf, 8, lat);
    _mav_put_int32_t(buf, 12, lon);
    _mav_put_int16_t(buf, 16, alt_gps);
    _mav_put_int16_t(buf, 18, vz);
    _mav_put_int16_t(buf, 20, acc_axial);
    _mav_put_int16_t(buf, 22, roll_rate);
    _mav_put_uint16_t(buf, 24, tilt);
    _mav_put_uint8_t(buf, 26, flight_state);
    _mav_put_uint8_t(buf, 27, flags);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_ROCKET_TELEMETRY_LEN);
#else
    mavlink_rocket_telemetry_t packet;
    packet.time_boot_ms = time_boot_ms;
    packet.alt_agl = alt_agl;
    packet.lat = lat;
    packet.lon = lon;
    packet.alt_gps = alt_gps;
    packet.vz = vz;
    packet.acc_axial = acc_axial;
    packet.roll_rate = roll_rate;
    packet.tilt = tilt;
    packet.flight_state = flight_state;
    packet.flags = flags;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_ROCKET_TELEMETRY_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_ROCKET_TELEMETRY;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_ROCKET_TELEMETRY_MIN_LEN, MAVLINK_MSG_ID_ROCKET_TELEMETRY_LEN, MAVLINK_MSG_ID_ROCKET_TELEMETRY_CRC);
}

/**
 * @brief Encode a rocket_telemetry struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param rocket_telemetry C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_rocket_telemetry_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_rocket_telemetry_t* rocket_telemetry)
{
    return mavlink_msg_rocket_telemetry_pack(system_id, component_id, msg, rocket_telemetry->time_boot_ms, rocket_telemetry->alt_agl, rocket_telemetry->lat, rocket_telemetry->lon, rocket_telemetry->alt_gps, rocket_telemetry->vz, rocket_telemetry->acc_axial, rocket_telemetry->roll_rate, rocket_telemetry->tilt, rocket_telemetry->flight_state, rocket_telemetry->flags);
}

/**
 * @brief Encode a rocket_telemetry struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param rocket_telemetry C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_rocket_telemetry_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_rocket_telemetry_t* rocket_telemetry)
{
    return mavlink_msg_rocket_telemetry_pack_chan(system_id, component_id, chan, msg, rocket_telemetry->time_boot_ms, rocket_telemetry->alt_agl, rocket_telemetry->lat, rocket_telemetry->lon, rocket_telemetry->alt_gps, rocket_telemetry->vz, rocket_telemetry->acc_axial, rocket_telemetry->roll_rate, rocket_telemetry->tilt, rocket_telemetry->flight_state, rocket_telemetry->flags);
}

/**
 * @brief Encode a rocket_telemetry struct with provided status structure
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 * @param rocket_telemetry C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_rocket_telemetry_encode_status(uint8_t system_id, uint8_t component_id, mavlink_status_t* _status, mavlink_message_t* msg, const mavlink_rocket_telemetry_t* rocket_telemetry)
{
    return mavlink_msg_rocket_telemetry_pack_status(system_id, component_id, _status, msg,  rocket_telemetry->time_boot_ms, rocket_telemetry->alt_agl, rocket_telemetry->lat, rocket_telemetry->lon, rocket_telemetry->alt_gps, rocket_telemetry->vz, rocket_telemetry->acc_axial, rocket_telemetry->roll_rate, rocket_telemetry->tilt, rocket_telemetry->flight_state, rocket_telemetry->flags);
}

/**
 * @brief Send a rocket_telemetry message
 * @param chan MAVLink channel to send the message
 *
 * @param time_boot_ms [ms] Temps depuis le démarrage de l'ordinateur de bord.
 * @param alt_agl [cm] Altitude barométrique au-dessus du pas de tir.
 * @param lat [degE7] Latitude GPS (0 si pas de fix).
 * @param lon [degE7] Longitude GPS (0 si pas de fix).
 * @param alt_gps [m] Altitude GPS (MSL).
 * @param vz [cm/s] Vitesse verticale (positive vers le haut).
 * @param acc_axial [mG] Accélération selon l'axe de la fusée (saturée à +/-32767).
 * @param roll_rate [deg/s] Vitesse de roulis.
 * @param tilt [cdeg] Inclinaison par rapport à la verticale (0 = vertical).
 * @param flight_state  Phase de vol.
 * @param flags  Indicateurs rapides.
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_rocket_telemetry_send(mavlink_channel_t chan, uint32_t time_boot_ms, int32_t alt_agl, int32_t lat, int32_t lon, int16_t alt_gps, int16_t vz, int16_t acc_axial, int16_t roll_rate, uint16_t tilt, uint8_t flight_state, uint8_t flags)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_ROCKET_TELEMETRY_LEN];
    _mav_put_uint32_t(buf, 0, time_boot_ms);
    _mav_put_int32_t(buf, 4, alt_agl);
    _mav_put_int32_t(buf, 8, lat);
    _mav_put_int32_t(buf, 12, lon);
    _mav_put_int16_t(buf, 16, alt_gps);
    _mav_put_int16_t(buf, 18, vz);
    _mav_put_int16_t(buf, 20, acc_axial);
    _mav_put_int16_t(buf, 22, roll_rate);
    _mav_put_uint16_t(buf, 24, tilt);
    _mav_put_uint8_t(buf, 26, flight_state);
    _mav_put_uint8_t(buf, 27, flags);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_ROCKET_TELEMETRY, buf, MAVLINK_MSG_ID_ROCKET_TELEMETRY_MIN_LEN, MAVLINK_MSG_ID_ROCKET_TELEMETRY_LEN, MAVLINK_MSG_ID_ROCKET_TELEMETRY_CRC);
#else
    mavlink_rocket_telemetry_t packet;
    packet.time_boot_ms = time_boot_ms;
    packet.alt_agl = alt_agl;
    packet.lat = lat;
    packet.lon = lon;
    packet.alt_gps = alt_gps;
    packet.vz = vz;
    packet.acc_axial = acc_axial;
    packet.roll_rate = roll_rate;
    packet.tilt = tilt;
    packet.flight_state = flight_state;
    packet.flags = flags;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_ROCKET_TELEMETRY, (const char *)&packet, MAVLINK_MSG_ID_ROCKET_TELEMETRY_MIN_LEN, MAVLINK_MSG_ID_ROCKET_TELEMETRY_LEN, MAVLINK_MSG_ID_ROCKET_TELEMETRY_CRC);
#endif
}

/**
 * @brief Send a rocket_telemetry message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_rocket_telemetry_send_struct(mavlink_channel_t chan, const mavlink_rocket_telemetry_t* rocket_telemetry)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_rocket_telemetry_send(chan, rocket_telemetry->time_boot_ms, rocket_telemetry->alt_agl, rocket_telemetry->lat, rocket_telemetry->lon, rocket_telemetry->alt_gps, rocket_telemetry->vz, rocket_telemetry->acc_axial, rocket_telemetry->roll_rate, rocket_telemetry->tilt, rocket_telemetry->flight_state, rocket_telemetry->flags);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_ROCKET_TELEMETRY, (const char *)rocket_telemetry, MAVLINK_MSG_ID_ROCKET_TELEMETRY_MIN_LEN, MAVLINK_MSG_ID_ROCKET_TELEMETRY_LEN, MAVLINK_MSG_ID_ROCKET_TELEMETRY_CRC);
#endif
}

#if MAVLINK_MSG_ID_ROCKET_TELEMETRY_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This variant of _send() can be used to save stack space by reusing
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_rocket_telemetry_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint32_t time_boot_ms, int32_t alt_agl, int32_t lat, int32_t lon, int16_t alt_gps, int16_t vz, int16_t acc_axial, int16_t roll_rate, uint16_t tilt, uint8_t flight_state, uint8_t flags)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint32_t(buf, 0, time_boot_ms);
    _mav_put_int32_t(buf, 4, alt_agl);
    _mav_put_int32_t(buf, 8, lat);
    _mav_put_int32_t(buf, 12, lon);
    _mav_put_int16_t(buf, 16, alt_gps);
    _mav_put_int16_t(buf, 18, vz);
    _mav_put_int16_t(buf, 20, acc_axial);
    _mav_put_int16_t(buf, 22, roll_rate);
    _mav_put_uint16_t(buf, 24, tilt);
    _mav_put_uint8_t(buf, 26, flight_state);
    _mav_put_uint8_t(buf, 27, flags);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_ROCKET_TELEMETRY, buf, MAVLINK_MSG_ID_ROCKET_TELEMETRY_MIN_LEN, MAVLINK_MSG_ID_ROCKET_TELEMETRY_LEN, MAVLINK_MSG_ID_ROCKET_TELEMETRY_CRC);
#else
    mavlink_rocket_telemetry_t *packet = (mavlink_rocket_telemetry_t *)msgbuf;
    packet->time_boot_ms = time_boot_ms;
    packet->alt_agl = alt_agl;
    packet->lat = lat;
    packet->lon = lon;
    packet->alt_gps = alt_gps;
    packet->vz = vz;
    packet->acc_axial = acc_axial;
    packet->roll_rate = roll_rate;
    packet->tilt = tilt;
    packet->flight_state = flight_state;
    packet->flags = flags;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_ROCKET_TELEMETRY, (const char *)packet, MAVLINK_MSG_ID_ROCKET_TELEMETRY_MIN_LEN, MAVLINK_MSG_ID_ROCKET_TELEMETRY_LEN, MAVLINK_MSG_ID_ROCKET_TELEMETRY_CRC);
#endif
}
#endif

#endif

// MESSAGE ROCKET_TELEMETRY UNPACKING


/**
 * @brief Get field time_boot_ms from rocket_telemetry message
 *
 * @return [ms] Temps depuis le démarrage de l'ordinateur de bord.
 */
static inline uint32_t mavlink_msg_rocket_telemetry_get_time_boot_ms(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint32_t(msg,  0);
}

/**
 * @brief Get field alt_agl from rocket_telemetry message
 *
 * @return [cm] Altitude barométrique au-dessus du pas de tir.
 */
static inline int32_t mavlink_msg_rocket_telemetry_get_alt_agl(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int32_t(msg,  4);
}

/**
 * @brief Get field lat from rocket_telemetry message
 *
 * @return [degE7] Latitude GPS (0 si pas de fix).
 */
static inline int32_t mavlink_msg_rocket_telemetry_get_lat(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int32_t(msg,  8);
}

/**
 * @brief Get field lon from rocket_telemetry message
 *
 * @return [degE7] Longitude GPS (0 si pas de fix).
 */
static inline int32_t mavlink_msg_rocket_telemetry_get_lon(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int32_t(msg,  12);
}

/**
 * @brief Get field alt_gps from rocket_telemetry message
 *
 * @return [m] Altitude GPS (MSL).
 */
static inline int16_t mavlink_msg_rocket_telemetry_get_alt_gps(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  16);
}

/**
 * @brief Get field vz from rocket_telemetry message
 *
 * @return [cm/s] Vitesse verticale (positive vers le haut).
 */
static inline int16_t mavlink_msg_rocket_telemetry_get_vz(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  18);
}

/**
 * @brief Get field acc_axial from rocket_telemetry message
 *
 * @return [mG] Accélération selon l'axe de la fusée (saturée à +/-32767).
 */
static inline int16_t mavlink_msg_rocket_telemetry_get_acc_axial(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  20);
}

/**
 * @brief Get field roll_rate from rocket_telemetry message
 *
 * @return [deg/s] Vitesse de roulis.
 */
static inline int16_t mavlink_msg_rocket_telemetry_get_roll_rate(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  22);
}

/**
 * @brief Get field tilt from rocket_telemetry message
 *
 * @return [cdeg] Inclinaison par rapport à la verticale (0 = vertical).
 */
static inline uint16_t mavlink_msg_rocket_telemetry_get_tilt(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint16_t(msg,  24);
}

/**
 * @brief Get field flight_state from rocket_telemetry message
 *
 * @return  Phase de vol.
 */
static inline uint8_t mavlink_msg_rocket_telemetry_get_flight_state(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  26);
}

/**
 * @brief Get field flags from rocket_telemetry message
 *
 * @return  Indicateurs rapides.
 */
static inline uint8_t mavlink_msg_rocket_telemetry_get_flags(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  27);
}

/**
 * @brief Decode a rocket_telemetry message into a struct
 *
 * @param msg The message to decode
 * @param rocket_telemetry C-struct to decode the message contents into
 */
static inline void mavlink_msg_rocket_telemetry_decode(const mavlink_message_t* msg, mavlink_rocket_telemetry_t* rocket_telemetry)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    rocket_telemetry->time_boot_ms = mavlink_msg_rocket_telemetry_get_time_boot_ms(msg);
    rocket_telemetry->alt_agl = mavlink_msg_rocket_telemetry_get_alt_agl(msg);
    rocket_telemetry->lat = mavlink_msg_rocket_telemetry_get_lat(msg);
    rocket_telemetry->lon = mavlink_msg_rocket_telemetry_get_lon(msg);
    rocket_telemetry->alt_gps = mavlink_msg_rocket_telemetry_get_alt_gps(msg);
    rocket_telemetry->vz = mavlink_msg_rocket_telemetry_get_vz(msg);
    rocket_telemetry->acc_axial = mavlink_msg_rocket_telemetry_get_acc_axial(msg);
    rocket_telemetry->roll_rate = mavlink_msg_rocket_telemetry_get_roll_rate(msg);
    rocket_telemetry->tilt = mavlink_msg_rocket_telemetry_get_tilt(msg);
    rocket_telemetry->flight_state = mavlink_msg_rocket_telemetry_get_flight_state(msg);
    rocket_telemetry->flags = mavlink_msg_rocket_telemetry_get_flags(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_ROCKET_TELEMETRY_LEN? msg->len : MAVLINK_MSG_ID_ROCKET_TELEMETRY_LEN;
        memset(rocket_telemetry, 0, MAVLINK_MSG_ID_ROCKET_TELEMETRY_LEN);
    memcpy(rocket_telemetry, _MAV_PAYLOAD(msg), len);
#endif
}
