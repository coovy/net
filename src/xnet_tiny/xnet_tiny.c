#include "xnet_tiny.h"

#define min(a, b) ((a) > (b) ? (b) : (a))
#define swap_order16(v) (((v) & 0xFF << 8) | (((v) >> 8) & 0xFF))

static uint8_t netif_mac [XNET_MAC_ADDR_SIZE];
static xnet_packet_t tx_packet, rx_packet;  // tx_packet发送，rx_packet接收


/**
 * Assign a network packet to send data
 * @param data_size Data size
 * @return The resulting package structure
 * **/
xnet_packet_t * xnet_alloc_for_send(uint16_t data_size){
    tx_packet.data = tx_packet.payload + XNET_CFG_PACKET_MAX_SIZE - data_size;
    tx_packet.size = data_size;
    return &tx_packet;
} 

/**
 * Assign a network packet to read data
 * @param data_size Data size
 * @return The resulting package structure
 * **/
xnet_packet_t * xnet_alloc_for_read(uint16_t data_size){
    rx_packet.data = rx_packet.payload;
    rx_packet.size = data_size;
    return &rx_packet;
}

/**Add a header to the network package
 * @param packet 
 * @param header_size
 * **/
static void add_header(xnet_packet_t *packet, uint16_t header_size) {
    packet->data -= header_size;
    packet->size += header_size;
}

/**
 * Remove a header
 * @param packet
 * @param header_size
 */
static void remove_header(xnet_packet_t *packet, uint16_t header_size) {
    packet->data += header_size;
    packet->size -= header_size;
}

/**
 * Truncate the length of the network packet to size
 * **/
static void truncate_packet(xnet_packet_t *packet, uint16_t size) {
    packet->size = min(packet->size, size);
}


static xnet_err_t ethernet_init(void){
    xnet_err_t err = xnet_driver_open(netif_mac);
    if(err < 0){
        return err;
    }
    return XNET_ERR_OK;
}

static xnet_err_t ethernet_out_to(xnet_protocol_t protocol, const uint8_t* mac_addr, xnet_packet_t* packet){
    xether_hdr_t* ether_hdr;
    add_header(packet, sizeof(xether_hdr_t));
    ether_hdr = (xether_hdr_t*)packet->data;
    memcpy(ether_hdr->dest, mac_addr, XNET_MAC_ADDR_SIZE);
    memcpy(ether_hdr->src, netif_mac, XNET_MAC_ADDR_SIZE);
    ether_hdr->protocol = protocol;

    return xnet_driver_send(packet);
}

static void ethernet_in(xnet_packet_t* packet){
    xether_hdr_t* ether_hdr;
    if(packet->size <= sizeof(xether_hdr_t)){
        return;
    }
    ether_hdr = packet->data;
    switch(swap_order16(ether_hdr->protocol)){
        case XNET_PROTOCOL_ARP:
            break;
        case XNET_PROTOCOL_IP:
            break;
    }
}

static void ethernet_poll(void){
    xnet_packet_t *packet;
    if(xnet_driver_read(&packet) == XNET_ERR_OK){
        ethernet_in(packet);
    }
}

/**
 * Polling processes network packets and processes them in the protocol stack
 */
void xnet_poll(void) {
    ethernet_poll();
}


void xnet_init(void) {
    ethernet_init();
}


