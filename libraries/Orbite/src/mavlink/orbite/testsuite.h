/** @file
 *    @brief MAVLink comm protocol testsuite generated from orbite.xml
 *    @see https://mavlink.io/en/
 */
#pragma once
#ifndef ORBITE_TESTSUITE_H
#define ORBITE_TESTSUITE_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef MAVLINK_TEST_ALL
#define MAVLINK_TEST_ALL
static void mavlink_test_common(uint8_t, uint8_t, mavlink_message_t *last_msg);
static void mavlink_test_orbite(uint8_t, uint8_t, mavlink_message_t *last_msg);

static void mavlink_test_all(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
    mavlink_test_common(system_id, component_id, last_msg);
    mavlink_test_orbite(system_id, component_id, last_msg);
}
#endif

#include "../common/testsuite.h"


static void mavlink_test_rocket_telemetry(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_ROCKET_TELEMETRY >= 256) {
            return;
        }
#endif
    mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
    mavlink_rocket_telemetry_t packet_in = {
        963497464,963497672,963497880,963498088,18067,18171,18275,18379,18483,211,22
    };
    mavlink_rocket_telemetry_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.time_boot_ms = packet_in.time_boot_ms;
        packet1.alt_agl = packet_in.alt_agl;
        packet1.lat = packet_in.lat;
        packet1.lon = packet_in.lon;
        packet1.alt_gps = packet_in.alt_gps;
        packet1.vz = packet_in.vz;
        packet1.acc_axial = packet_in.acc_axial;
        packet1.roll_rate = packet_in.roll_rate;
        packet1.tilt = packet_in.tilt;
        packet1.flight_state = packet_in.flight_state;
        packet1.flags = packet_in.flags;
        
        
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) {
           // cope with extensions
           memset(MAVLINK_MSG_ID_ROCKET_TELEMETRY_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_ROCKET_TELEMETRY_MIN_LEN);
        }
#endif
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_rocket_telemetry_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_rocket_telemetry_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_rocket_telemetry_pack(system_id, component_id, &msg , packet1.time_boot_ms , packet1.alt_agl , packet1.lat , packet1.lon , packet1.alt_gps , packet1.vz , packet1.acc_axial , packet1.roll_rate , packet1.tilt , packet1.flight_state , packet1.flags );
    mavlink_msg_rocket_telemetry_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_rocket_telemetry_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.time_boot_ms , packet1.alt_agl , packet1.lat , packet1.lon , packet1.alt_gps , packet1.vz , packet1.acc_axial , packet1.roll_rate , packet1.tilt , packet1.flight_state , packet1.flags );
    mavlink_msg_rocket_telemetry_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
            comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
    mavlink_msg_rocket_telemetry_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_rocket_telemetry_send(MAVLINK_COMM_1 , packet1.time_boot_ms , packet1.alt_agl , packet1.lat , packet1.lon , packet1.alt_gps , packet1.vz , packet1.acc_axial , packet1.roll_rate , packet1.tilt , packet1.flight_state , packet1.flags );
    mavlink_msg_rocket_telemetry_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

#ifdef MAVLINK_HAVE_GET_MESSAGE_INFO
    MAVLINK_ASSERT(mavlink_get_message_info_by_name("ROCKET_TELEMETRY") != NULL);
    MAVLINK_ASSERT(mavlink_get_message_info_by_id(MAVLINK_MSG_ID_ROCKET_TELEMETRY) != NULL);
#endif
}

static void mavlink_test_rocket_status(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_ROCKET_STATUS >= 256) {
            return;
        }
#endif
    mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
    mavlink_rocket_status_t packet_in = {
        963497464,963497672,963497880,17859,17963,18067,18171,18275,18379,77,144,211,22
    };
    mavlink_rocket_status_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.time_boot_ms = packet_in.time_boot_ms;
        packet1.max_alt_agl = packet_in.max_alt_agl;
        packet1.apogee_time_ms = packet_in.apogee_time_ms;
        packet1.vbat = packet_in.vbat;
        packet1.temperature = packet_in.temperature;
        packet1.max_vz = packet_in.max_vz;
        packet1.max_acc = packet_in.max_acc;
        packet1.sensor_health = packet_in.sensor_health;
        packet1.gps_hdop = packet_in.gps_hdop;
        packet1.pyro_continuity = packet_in.pyro_continuity;
        packet1.pyro_fired = packet_in.pyro_fired;
        packet1.gps_fix_type = packet_in.gps_fix_type;
        packet1.gps_sats = packet_in.gps_sats;
        
        
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) {
           // cope with extensions
           memset(MAVLINK_MSG_ID_ROCKET_STATUS_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_ROCKET_STATUS_MIN_LEN);
        }
#endif
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_rocket_status_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_rocket_status_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_rocket_status_pack(system_id, component_id, &msg , packet1.time_boot_ms , packet1.max_alt_agl , packet1.apogee_time_ms , packet1.vbat , packet1.temperature , packet1.max_vz , packet1.max_acc , packet1.sensor_health , packet1.gps_hdop , packet1.pyro_continuity , packet1.pyro_fired , packet1.gps_fix_type , packet1.gps_sats );
    mavlink_msg_rocket_status_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_rocket_status_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.time_boot_ms , packet1.max_alt_agl , packet1.apogee_time_ms , packet1.vbat , packet1.temperature , packet1.max_vz , packet1.max_acc , packet1.sensor_health , packet1.gps_hdop , packet1.pyro_continuity , packet1.pyro_fired , packet1.gps_fix_type , packet1.gps_sats );
    mavlink_msg_rocket_status_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
            comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
    mavlink_msg_rocket_status_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_rocket_status_send(MAVLINK_COMM_1 , packet1.time_boot_ms , packet1.max_alt_agl , packet1.apogee_time_ms , packet1.vbat , packet1.temperature , packet1.max_vz , packet1.max_acc , packet1.sensor_health , packet1.gps_hdop , packet1.pyro_continuity , packet1.pyro_fired , packet1.gps_fix_type , packet1.gps_sats );
    mavlink_msg_rocket_status_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

#ifdef MAVLINK_HAVE_GET_MESSAGE_INFO
    MAVLINK_ASSERT(mavlink_get_message_info_by_name("ROCKET_STATUS") != NULL);
    MAVLINK_ASSERT(mavlink_get_message_info_by_id(MAVLINK_MSG_ID_ROCKET_STATUS) != NULL);
#endif
}

static void mavlink_test_ground_link_status(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_GROUND_LINK_STATUS >= 256) {
            return;
        }
#endif
    mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
    mavlink_ground_link_status_t packet_in = {
        963497464,963497672,963497880,963498088,963498296,963498504,185.0,213.0,241.0,113,180,247
    };
    mavlink_ground_link_status_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.time_boot_ms = packet_in.time_boot_ms;
        packet1.rx_packets = packet_in.rx_packets;
        packet1.crc_errors = packet_in.crc_errors;
        packet1.frames_lost = packet_in.frames_lost;
        packet1.malformed = packet_in.malformed;
        packet1.last_rx_age_ms = packet_in.last_rx_age_ms;
        packet1.rssi = packet_in.rssi;
        packet1.snr = packet_in.snr;
        packet1.freq_error = packet_in.freq_error;
        packet1.link_quality = packet_in.link_quality;
        packet1.rx_rate = packet_in.rx_rate;
        packet1.fem_type = packet_in.fem_type;
        
        
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) {
           // cope with extensions
           memset(MAVLINK_MSG_ID_GROUND_LINK_STATUS_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_GROUND_LINK_STATUS_MIN_LEN);
        }
#endif
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_ground_link_status_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_ground_link_status_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_ground_link_status_pack(system_id, component_id, &msg , packet1.time_boot_ms , packet1.rx_packets , packet1.crc_errors , packet1.frames_lost , packet1.malformed , packet1.last_rx_age_ms , packet1.rssi , packet1.snr , packet1.freq_error , packet1.link_quality , packet1.rx_rate , packet1.fem_type );
    mavlink_msg_ground_link_status_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_ground_link_status_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.time_boot_ms , packet1.rx_packets , packet1.crc_errors , packet1.frames_lost , packet1.malformed , packet1.last_rx_age_ms , packet1.rssi , packet1.snr , packet1.freq_error , packet1.link_quality , packet1.rx_rate , packet1.fem_type );
    mavlink_msg_ground_link_status_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
            comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
    mavlink_msg_ground_link_status_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_ground_link_status_send(MAVLINK_COMM_1 , packet1.time_boot_ms , packet1.rx_packets , packet1.crc_errors , packet1.frames_lost , packet1.malformed , packet1.last_rx_age_ms , packet1.rssi , packet1.snr , packet1.freq_error , packet1.link_quality , packet1.rx_rate , packet1.fem_type );
    mavlink_msg_ground_link_status_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

#ifdef MAVLINK_HAVE_GET_MESSAGE_INFO
    MAVLINK_ASSERT(mavlink_get_message_info_by_name("GROUND_LINK_STATUS") != NULL);
    MAVLINK_ASSERT(mavlink_get_message_info_by_id(MAVLINK_MSG_ID_GROUND_LINK_STATUS) != NULL);
#endif
}

static void mavlink_test_orbite(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
    mavlink_test_rocket_telemetry(system_id, component_id, last_msg);
    mavlink_test_rocket_status(system_id, component_id, last_msg);
    mavlink_test_ground_link_status(system_id, component_id, last_msg);
}

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // ORBITE_TESTSUITE_H
