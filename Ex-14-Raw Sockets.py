import socket
import struct
import textwrap


TAB_1 = '\t - '
TAB_2 = '\t\t - '
TAB_3 = '\t\t\t - '
TAB_4 = '\t\t\t\t - '

DATA_TAB_1 = '\t   '
DATA_TAB_2 = '\t\t   '
DATA_TAB_3 = '\t\t\t   '
DATA_TAB_4 = '\t\t\t\t   '


def main():
   
    conn = socket.socket(socket.AF_PACKET, socket.SOCK_RAW, socket.ntohs(3))

    while True:
        raw_data, addr = conn.recvfrom(65536)
        dest_mac, src_mac, eth_proto, data = ethernet_frame(raw_data)
        print('\nEthernet Frame:')
        print(f'{TAB_1}Destination MAC: {dest_mac}, Source MAC: {src_mac}, Protocol: {eth_proto}')

        
        if eth_proto == 8:
            (version, header_length, ttl, proto, src, target, data) = ipv4_packet(data)
            print(TAB_1 + 'IPv4 Packet:')
            print(f'{TAB_2}Version: {version}, Header Length: {header_length}, TTL: {ttl}')
            print(f'{TAB_2}Protocol: {proto}, Source: {src}, Target: {target}')

          
            if proto == 6:
                (src_port, dest_port, sequence, acknowledgment, flags, data) = tcp_segment(data)
                print(TAB_1 + 'TCP Segment:')
                print(f'{TAB_2}Source Port: {src_port}, Destination Port: {dest_port}')
                print(f'{TAB_2}Sequence: {sequence}, Acknowledgment: {acknowledgment}')
                print(f'{TAB_2}Flags: {flags}')


def ethernet_frame(data):
    dest_mac, src_mac, proto = struct.unpack('! 6s 6s H', data[:14])
    return get_mac_addr(dest_mac), get_mac_addr(src_mac), socket.htons(proto), data[14:]

def get_mac_addr(bytes_addr):
    bytes_str = map('{:02x}'.format, bytes_addr)
    return ':'.join(bytes_str).upper()

def ipv4_packet(data):
    version_header_length = data[0]
    version = version_header_length >> 4
    header_length = (version_header_length & 15) * 4
    ttl, proto, src, target = struct.unpack('! 8x B B 2x 4s 4s', data[:20])
    return version, header_length, ttl, proto, ipv4(src), ipv4(target), data[header_length:]

def ipv4(addr):
    return '.'.join(map(str, addr))

def tcp_segment(data):
    (src_port, dest_port, sequence, acknowledgment, offset_reserved_flags) = struct.unpack('! H H L L H', data[:14])
    offset = (offset_reserved_flags >> 12) * 4
    flags = offset_reserved_flags & 0x3F
    return src_port, dest_port, sequence, acknowledgment, flags, data[offset:]

if __name__ == "__main__":
    main()
