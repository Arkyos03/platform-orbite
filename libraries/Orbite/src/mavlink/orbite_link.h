/*
 * orbite_link.h — Définition commune de la liaison LoRa ORBITE-I <-> station sol.
 *
 * Ce fichier est IDENTIQUE côté fusée (STM32F4 + E22-900M22S) et côté station
 * (Heltec WiFi LoRa 32 V4). Toute modification doit être reportée des deux côtés.
 *
 * Contenu :
 *   - paramètres radio (fréquence, BW, SF, CR, sync word, préambule)
 *   - identifiants MAVLink (sysid / compid)
 *   - débits par phase de vol
 *   - format d'un paquet LoRa : une suite de trames MAVLink v2 complètes
 *   - orbite_build_packet() : construction du paquet côté fusée
 *
 * Dépend des headers générés depuis dialect/orbite.xml (mavgen, C, MAVLink 2).
 */
#ifndef ORBITE_LINK_H
#define ORBITE_LINK_H

#include <stdint.h>
#include <stddef.h>
#include "orbite/mavlink.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------------------------------------------- */
/* Paramètres radio — doivent être strictement identiques des deux côtés     */
/* ------------------------------------------------------------------------- */
#define ORBITE_LORA_FREQ_MHZ        869.525f  /* centre de la sous-bande 869,40–869,65 MHz (10 % DC, 500 mW ERP) */
#define ORBITE_LORA_BW_KHZ          250.0f
#define ORBITE_LORA_SF              6
#define ORBITE_LORA_CR_DENOM        5         /* coding rate 4/5 */
#define ORBITE_LORA_SYNC_WORD       0x12      /* LoRa "privé" (RadioLib : 0x12 -> registre 0x1424) */
#define ORBITE_LORA_PREAMBLE        12
#define ORBITE_LORA_CRC_ON          1
#define ORBITE_LORA_TCXO_V          1.8f      /* E22-900M22S et Heltec V4 : TCXO piloté par DIO3 */
#define ORBITE_LORA_TX_POWER_DBM    22        /* E22-900M22S, côté fusée uniquement */
#define ORBITE_LORA_MAX_PAYLOAD     255

/* ------------------------------------------------------------------------- */
/* Identifiants MAVLink                                                       */
/* ------------------------------------------------------------------------- */
#define ORBITE_SYSID_ROCKET         1
#define ORBITE_COMPID_ROCKET        MAV_COMP_ID_AUTOPILOT1          /* 1  */
#define ORBITE_SYSID_GROUND         51
#define ORBITE_COMPID_GROUND        MAV_COMP_ID_TELEMETRY_RADIO     /* 68 */

/* ------------------------------------------------------------------------- */
/* Débits (fusée)                                                             */
/* ------------------------------------------------------------------------- */
#define ORBITE_RATE_PAD_HZ          2     /* pas de tir et après atterrissage */
#define ORBITE_RATE_FLIGHT_HZ       10    /* de BOOST à LANDED                 */
#define ORBITE_FLIGHT_TIMEOUT_S     300   /* retour forcé à 2 Hz après 5 min   */
#define ORBITE_STATUS_PERIOD_MS     1000  /* ROCKET_STATUS ajouté 1x/s         */

/*
 * Format d'un paquet LoRa (payload, CRC LoRa matériel en plus) :
 *
 *   [ trame MAVLink v2 ROCKET_TELEMETRY ]                      toujours, 40 octets max
 *   [ trame MAVLink v2 ROCKET_STATUS    ]                      1 fois par seconde, 40 octets max
 *
 * Les trames sont concaténées sans séparateur : MAVLink v2 est auto-délimité
 * (0xFD, longueur, ..., CRC). La station sol les recopie telles quelles sur l'USB.
 * Temps d'antenne SF6/BW250 : ~25 ms (TELEMETRY seul), ~43 ms (TELEMETRY + STATUS).
 */

/*
 * Construit un paquet LoRa côté fusée.
 *   buf    : tampon de sortie (>= 2 * MAVLINK_MAX_PACKET_LEN conseillé, 255 max utile)
 *   cap    : taille du tampon
 *   tlm    : télémétrie rapide (obligatoire)
 *   status : état détaillé, ou NULL si pas à envoyer dans ce paquet
 * Retourne la longueur du paquet, 0 si le tampon est trop petit.
 * Utilise le canal MAVLINK_COMM_0 (le numéro de séquence s'incrémente à chaque trame).
 */
static inline uint16_t orbite_build_packet(uint8_t *buf, uint16_t cap,
                                           const mavlink_rocket_telemetry_t *tlm,
                                           const mavlink_rocket_status_t *status)
{
    mavlink_message_t msg;
    uint8_t frame[MAVLINK_MAX_PACKET_LEN];
    uint16_t len = 0, n;

    mavlink_msg_rocket_telemetry_encode_chan(ORBITE_SYSID_ROCKET, ORBITE_COMPID_ROCKET,
                                             MAVLINK_COMM_0, &msg, tlm);
    n = mavlink_msg_to_send_buffer(frame, &msg);
    if ((uint32_t)len + n > cap || (uint32_t)len + n > ORBITE_LORA_MAX_PAYLOAD) return 0;
    for (uint16_t i = 0; i < n; i++) buf[len++] = frame[i];

    if (status) {
        mavlink_msg_rocket_status_encode_chan(ORBITE_SYSID_ROCKET, ORBITE_COMPID_ROCKET,
                                              MAVLINK_COMM_0, &msg, status);
        n = mavlink_msg_to_send_buffer(frame, &msg);
        if ((uint32_t)len + n > cap || (uint32_t)len + n > ORBITE_LORA_MAX_PAYLOAD) return 0;
        for (uint16_t i = 0; i < n; i++) buf[len++] = frame[i];
    }
    return len;
}

#ifdef __cplusplus
}
#endif
#endif /* ORBITE_LINK_H */
