'''
The primary objective of this script is to determine the property number corresponding to amplitude and frequency.
Review the server output in the logs directory.
'''
import socket 
 
UDP_IP = "127.0.0.1" 
UDP_PORT = 4000 
object_bytes = b'\x00\x01' # out1
read_op_bytes = b'\x00\x01' # read operation

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

# Loop through all possible values for the last two bytes (property)
for i in range(65536):  
    # Convert the current value of i to a 16-bit big-endian binary string
    property_bytes = i.to_bytes(2, byteorder='big')

    # Define the payload with the operation, object, and property bytes
    payload = read_op_bytes + object_bytes + property_bytes
    print(payload)

    # Send the payload
    sock.sendto(payload, (UDP_IP, UDP_PORT))
